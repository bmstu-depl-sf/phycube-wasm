#include "chamber.h"

namespace phycoub
{

MyWilsonCloudChamber::MyWilsonCloudChamber()
{
    initLog();
    initCalculationGroup();
    initElectricField();
    initMagneticField();
    initSourcesAndLifeTimeControllers();
    initWithParticleGroups();

    setDeltaTime( 1E-16 );
    updateUniqParticleGroupList();
}

Vector MyWilsonCloudChamber::getBorders() const
{
    return cyclicBorder_->getBorders();
}

void MyWilsonCloudChamber::setBorders( Vector borders )
{
    cyclicBorder_->setBorders( borders );
}

void MyWilsonCloudChamber::setElectricFieldDirection( Vector direction )
{
    electricHomogeneousDirectField_->setDirection( direction );
}

Vector MyWilsonCloudChamber::getElectricFieldDirection() const
{
    return electricHomogeneousDirectField_->getDirection();
}

void MyWilsonCloudChamber::setElectricFieldCharge( double charge )
{
    electricHomogeneousDirectField_->setCharge(
        charge * ElectricConstants::electronCharge );
}

double MyWilsonCloudChamber::getElectricFieldCharge() const
{
    return electricHomogeneousDirectField_->getCharge();
}

void MyWilsonCloudChamber::setMagneticFieldDirection( Vector direction )
{
    magneticHomogeneousDirectField_->setDirection( direction );
}

Vector MyWilsonCloudChamber::getMagneticFieldDirection() const
{
    return magneticHomogeneousDirectField_->getDirection();
}

void MyWilsonCloudChamber::setMagneticFieldInduction( double B )
{
    magneticHomogeneousDirectField_->setMagneticInduction( B );
}

double MyWilsonCloudChamber::getMagneticFieldInduction() const
{
    return magneticHomogeneousDirectField_->getMagneticInduction();
}

void MyWilsonCloudChamber::setSpecificSourceDirection( Vector direction )
{
    for(auto sourceIt = coneParticleSource_.begin(); sourceIt != coneParticleSource_.end(); sourceIt++)
        (*sourceIt)->setDirection( direction );
}

void MyWilsonCloudChamber::setSpecificSourcBornPeriod( double bornPeriod )
{
    for (auto controllerIt = bornPeriodLifeTimeController_.begin(); controllerIt != bornPeriodLifeTimeController_.end(); controllerIt++)
        (*controllerIt)->setBornPeriod( bornPeriod );
}

void MyWilsonCloudChamber::initLog()
{
    stdErrLog = std::make_shared< Log >();
    stdErrLogObserver = std::make_shared< StdErrLogObserver >();

    setLog( stdErrLog );
    stdErrLog->subsribeForUpdates( stdErrLogObserver );
}

void MyWilsonCloudChamber::initCalculationGroup()
{
    constexpr static double borderSize_ = 1.e-4;
    cyclicBorder_ = std::make_shared< CyclicBorder >( Vector{ borderSize_ } );
    cyclicBorder_->setLog( stdErrLog );

    leapFrog_ = std::make_shared< LeapFrog >();
    leapFrogCalculationGroup_
        = std::make_shared< CalculationGroup >( leapFrog_, cyclicBorder_ );

    addCalculationGroup( leapFrogCalculationGroup_ );
    addContainParticleGroup( leapFrogCalculationGroup_ );
}

void MyWilsonCloudChamber::initElectricField()
{
    culonInterworking_ = std::make_shared< CulonInterworking >();

    electricHomogeneousDirectField_ = std::make_shared< ElectricHomogeneousDirectField >(
        Vector{ 0, 0, 1 }, ElectricConstants::electronCharge * 0 );
    electricHomogeneousDirectFieldCreator_ = std::make_shared< HomogeneousFieldCreator >(
        electricHomogeneousDirectField_, "ElectricHomogeneousField" );
    feelElectricHomogeneousDirectWithCulonInterworking_
        = std::make_shared< FieldReceiver >( electricHomogeneousDirectFieldCreator_,
            culonInterworking_, "CulonInterworking" );

    addFieldResponsive( feelElectricHomogeneousDirectWithCulonInterworking_ );
    addContainParticleGroup( feelElectricHomogeneousDirectWithCulonInterworking_ );
}

void MyWilsonCloudChamber::initMagneticField()
{
    magneticHomogeneousDirectField_
        = std::make_shared< MagneticHomogeneousDirectField >( Vector{ 0, 1, 1 }, 3e-2 );
    magneticHomogeneousDirectFieldCreator_ = std::make_shared< HomogeneousFieldCreator >(
        magneticHomogeneousDirectField_, "MagneticHomogeneousField" );
    magneticInterworking_ = std::make_shared< MagneticInterworking >();
    feelWithMagneticInterworking_
        = std::make_shared< FieldReceiver >( magneticHomogeneousDirectFieldCreator_,
            magneticInterworking_, "MagneticInterworking" );

    addFieldResponsive( feelWithMagneticInterworking_ );
    addContainParticleGroup( feelWithMagneticInterworking_ );
}

void MyWilsonCloudChamber::initSourcesAndLifeTimeControllers()
{
    const Vector& borders = cyclicBorder_->getBorders();

    const double electronBornPeriod = 1e-11;
    const double electronEnergy = 1e-17;
    const double protonEnergy = 1e-15;

    const Vector direction{ 1., 0., 0. };
    const double coneAngle = .1;
    const Vector sourceCoordinate = { 0., borders.y() / 2, borders.z() / 2 };

    const ParticleOptions electronOptions{ ElectricConstants::electronWeight,
        ElectricConstants::electronCharge };
    const ParticleOptions protonOptions{ ElectricConstants::protonWeight,
        ElectricConstants::protonCharge };

    std::vector<std::pair<const double, const ParticleOptions>> specs;
    specs.push_back(std::make_pair(electronEnergy, electronOptions));
    specs.push_back(std::make_pair(protonEnergy, protonOptions));

    for (auto specsIt = specs.begin(); specsIt != specs.end(); specsIt++)
    {
        ConeParticleSourcePtr specificConeParticleSource_;
        BornPeriodLifeTimeControllerPtr specificBornPeriodLifeTimeController_;

        specificConeParticleSource_ = std::make_shared< ConeParticleSource >(
            direction, coneAngle, sourceCoordinate, specsIt->second, specsIt->first );
        specificBornPeriodLifeTimeController_
            = std::make_shared< BornPeriodLifeTimeController >(
                electronBornPeriod, specificConeParticleSource_ );

        cyclicBorder_->addBorderReachedObserver( specificBornPeriodLifeTimeController_ );
        addLifeTimeController( specificBornPeriodLifeTimeController_ );
        addContainParticleGroup( specificBornPeriodLifeTimeController_ );
        coneParticleSource_.push_back(specificConeParticleSource_);
        bornPeriodLifeTimeController_.push_back(specificBornPeriodLifeTimeController_);
    }
}

void MyWilsonCloudChamber::initWithParticleGroups()
{
    ParticleGroupPtr specific = std::make_shared< ParticleGroup >();
    leapFrogCalculationGroup_->addParticleGroup( specific );
    feelElectricHomogeneousDirectWithCulonInterworking_->addParticleGroup( specific );
    feelWithMagneticInterworking_->addParticleGroup( specific );
    groupInterCommunication_->addParticleGroup( specific );
    for (auto controllerIt = bornPeriodLifeTimeController_.begin(); controllerIt != bornPeriodLifeTimeController_.end(); controllerIt++)
        (*controllerIt)->addParticleGroup( specific );
}

} // namespace phycoub