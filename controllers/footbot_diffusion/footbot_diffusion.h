/*
 * AUTHOR: Carlo Pinciroli <cpinciro@ulb.ac.be>
 *
 * An example diffusion controller for the foot-bot.
 *
 * This controller makes the robots behave as gas particles. The robots
 * go straight until they get close enough to another robot, in which
 * case they turn, loosely simulating an elastic collision. The net effect
 * is that over time the robots diffuse in the environment.
 *
 * The controller uses the proximity sensor to detect obstacles and the
 * wheels to move the robot around.
 *
 * This controller is meant to be used with the XML files:
 *    experiments/diffusion_1.argos
 *    experiments/diffusion_10.argos
 */

#ifndef FOOTBOT_DIFFUSION_H
#define FOOTBOT_DIFFUSION_H

/*
 * Include some necessary headers.
 */
/* Definition of the CCI_Controller class. */
#include <argos3/core/control_interface/ci_controller.h>
/* Definition of the differential steering actuator */
#include <argos3/plugins/robots/generic/control_interface/ci_differential_steering_actuator.h>
/* Definition of the foot-bot proximity sensor */
#include <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_proximity_sensor.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/plugins/robots/generic/control_interface/ci_differential_steering_sensor.h>
#include <argos3/plugins/robots/generic/control_interface/ci_range_and_bearing_sensor.h>
#include <argos3/plugins/robots/generic/control_interface/ci_range_and_bearing_actuator.h>
#include <boost/circular_buffer.hpp>
#include <argos3/plugins/robots/foot-bot/simulator/footbot_entity.h>
#include <numeric>
#include <argos3/core/simulator/loop_functions.h>
/*
 * All the ARGoS stuff in the 'argos' namespace.
 * With this statement, you save typing argos:: every time.
 */
using namespace argos;


boost::circular_buffer<Real>* FlockData;
boost::circular_buffer<Real>* FlockCoordData;
std::vector<Real> FlockRange;
std::vector<Real> FlockBearing;
std::vector<Real> FlockID;
std::vector<Real> FlockHeadings;
std::vector<Real> FlockCoords;
std::vector<int> DrRobo;
std::vector<double> DrDistance;
std::vector<double> Ambulance;
int Stuck = 0;
int StuckBounce = 0;
int ClassifierSuccess;
int FaultID = 0;
int DrID = 0;
int MemoryBits = 18;
int BeginDiagnostic = 0;
int BeginMOT = 0;
int ClassBounce = 0;
int Fault = 0;
int PMFangle = 60;
int DetectDelay = 100;
int Behaviour = 0;
int Diagnosis = 0;
int FaultBounce = 0;
int BounceCount = 0;
int BehaviourCount = 0;
std::vector<Real> Candidates;
boost::circular_buffer<int>* FeatureVectors3;
boost::circular_buffer<int>* MemoryLog;
boost::circular_buffer<Real>* IntCoord;
boost::circular_buffer<Real>* TrueIntCoord;
boost::circular_buffer<Real>* YawHolder;
std::vector<Real> AggX;
std::vector<Real> AggY;
std::vector<Real> Update;
int PowerCycle [2] =  {1,2};
int SensorReplacement [2] = {3,6};
int MotorReplacement [2] = {4,5};
int Detectmin;
int Detect;
int Detected;
int InRange;
std::ofstream SnapshotFile;
std::ofstream DataFile;
std::ofstream SpartanPercent;
int foldernum;
int F1hang;
int F2hang;
int F3hang;
int F4hang;
int F5hang;
Real Fail = 0;
int Eligibility;
int MOT = 0;
Real Total = 0;
Real Class = 0;
std::vector<Real> Checklist;
Real Drift = 0;
int timeweight = 0;
/*
 * A controller is simply an implementation of the CCI_Controller class.
 */
class CFootBotDiffusion : public CCI_Controller {

public:



   /* Class constructor. */
   CFootBotDiffusion();

   /* Class destructor. */
   virtual ~CFootBotDiffusion() {}

   /*
    * This function initializes the controller.
    * The 't_node' variable points to the <parameters> section in the XML
    * file in the <controllers><footbot_diffusion_controller> section.
    */
   virtual void Init(TConfigurationNode& t_node);


   /*
    * This function is called once every time step.
    * The length of the time step is set in the XML file.
    */
   virtual void ControlStep();
    CCI_RangeAndBearingSensor::TReadings GetRABSensorReadings();


   /*
    * This function resets the controller to its state right after the
    * Init().
    * It is called when you press the reset button in the GUI.
    * In this example controller there is no need for resetting anything,
    * so the function could have been omitted. It's here just for
    * completeness.
    */
   virtual void Reset() {}

   /*
    * Called to cleanup what done by Init() when the experiment finishes.
    * In this example controller there is no need for clean anything up,
    * so the function could have been omitted. It's here just for
    * completeness.
    */
   virtual void Destroy() {}

    inline CCI_DifferentialSteeringSensor::SReading GetWheelVelocity() const {

        return wheel_encoders->GetReading();

    }

    inline CCI_FootBotProximitySensor::TReadings GetProximity() const {

        return m_pcProximity->GetReadings();
    }
    inline Real GetSpoofLeftWheelVelocity() const {

        return LeftWheel;

    }
    inline Real GetSpoofRightWheelVelocity() const {

        return RightWheel;

    }
    inline Real GetFaultID() const {

        return FaultID;

    }
    Real NoiseLeft;
    Real NoiseRight;

    Real hangRight;
    Real hangLeft;

    CVector2 NeighbourDistance;
    std::vector<Real> Agent;
    std::vector<Real> AgentCoord;

    std::vector<Real> Indices;
    std::vector<Real> Indices1;

    std::vector<Real> Indices4;




private:

   /* Pointer to the differential steering actuator */
   CCI_DifferentialSteeringActuator* m_pcWheels;
   /* Pointer to the foot-bot proximity sensor */
   CCI_FootBotProximitySensor* m_pcProximity;
    CCI_DifferentialSteeringSensor* wheel_encoders;
    Real* timestep;
    CCI_RangeAndBearingActuator* range_and_bearing_actuator;
    CCI_RangeAndBearingSensor* range_and_bearing_sensor;


   /*
    * The following variables are used as parameters for the
    * algorithm. You can set their value in the <parameters> section
    * of the XML configuration file, under the
    * <controllers><footbot_diffusion_controller> section.
    */

   /* Maximum tolerance for the angle between
    * the robot heading direction and
    * the closest obstacle detected. */
   CDegrees m_cAlpha;
   /* Maximum tolerance for the proximity reading between
    * the robot and the closest obstacle.
    * The proximity reading is 0 when nothing is detected
    * and grows exponentially to 1 when the obstacle is
    * touching the robot.
    */
   Real m_fDelta;
   /* Wheel speed. */
   Real m_fWheelVelocity;
   /* Angle tolerance range to go straight.
    * It is set to [-alpha,alpha]. */
   CRange<CRadians> m_cGoStraightAngleRange;
    CRadians cAngle;
    Real LeftWheel;
    Real RightWheel;

};

#endif
