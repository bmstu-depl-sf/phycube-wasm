#pragma once

#include <memory>

#include "PhyCoub.h"
#include "CyclicBorder.h"
#include "LeapFrog.h"
#include "CulonInterworking.h"
#include "MagneticInterworking.h"
#include "Constants.h"
#include "ElectricField.h"
#include "ElectricHomogeneousDirectField.h"
#include "MagneticHomogeneousDirectField.h"
#include "HomogeneousFieldCreator.h"
#include "FieldReceiver.h"
#include "BorderFieldCondition.h"
#include "InterCommunication.h"
#include "InterGroupCommunication.h"
#include "ConeParticleSource.h"
#include "BornPeriodLifeTimeController.h"
#include "StdErrLogObserver.h"

namespace phycoub
{

class MyWilsonCloudChamber : public PhyCoub
{
  public:
    MyWilsonCloudChamber();
    virtual ~MyWilsonCloudChamber() override = default;

    Vector getBorders() const;
    void setBorders( Vector borders );

    void setElectricFieldDirection( Vector direction );
    Vector getElectricFieldDirection() const;
    void setElectricFieldCharge( double charge );
    double getElectricFieldCharge() const;

    void setMagneticFieldDirection( Vector direction );
    Vector getMagneticFieldDirection() const;
    void setMagneticFieldInduction( double B );
    double getMagneticFieldInduction() const;

    void setSpecificSourceDirection( Vector direction );
    void setSpecificSourcBornPeriod( double bornPeriod );

    ParticleOptions getSpecificSourceParticleOptions() const;

  private:
    void initLog();
    void initCalculationGroup();
    void initElectricField();
    void initMagneticField();
    void initInterCommunication();
    void initSourcesAndLifeTimeControllers();
    void initWithParticleGroups();

    CyclicBorderPtr cyclicBorder_;
    LeapFrogPtr leapFrog_;
    CalculationGroupPtr leapFrogCalculationGroup_;

    CulonInterworkingPtr culonInterworking_;
    ElectricHomogeneousDirectFieldPtr electricHomogeneousDirectField_;
    HomogeneousFieldCreatorPtr electricHomogeneousDirectFieldCreator_;
    FieldReceiverPtr feelElectricHomogeneousDirectWithCulonInterworking_;

    MagneticInterworkingPtr magneticInterworking_;
    MagneticHomogeneousDirectFieldPtr magneticHomogeneousDirectField_;
    HomogeneousFieldCreatorPtr magneticHomogeneousDirectFieldCreator_;
    FieldReceiverPtr feelWithMagneticInterworking_;

    InterworkingPtr interworking_;
    BorderFieldConditionPtr borderFieldCondition_;
    ElectricFieldPtr electricField_;

    InterGroupCommunicationPtr groupInterCommunication_;
    std::vector<ConeParticleSourcePtr> coneParticleSource_;
    std::vector<BornPeriodLifeTimeControllerPtr> bornPeriodLifeTimeController_;

    LogPtr stdErrLog;
    StdErrLogObserverPtr stdErrLogObserver;
};

using MyWilsonCloudChamberPtr = std::shared_ptr< MyWilsonCloudChamber >;

} // namespace phycoub