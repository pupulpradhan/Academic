#include "OrientationMath.h"

//TODO: fill in from hw 4 as necessary

/** TODO: see documentation in header file */
double computeAccPitch(double acc[3]) {

  double accPitch = - atan2(acc[2],acc[1]) * (180.0/3.14159265);
  return accPitch;
}

/** TODO: see documentation in header file */
double computeAccRoll(double acc[3]) {

   double accRoll = atan2(acc[0],acc[1]) * (180.0/3.14159265);
  return accRoll;

}

/** TODO: see documentation in header file */
double computeFlatlandRollGyr(double flatlandRollGyrPrev, double gyr[3], double deltaT) {

double gyroOrientation = flatlandRollGyrPrev + gyr[2] * deltaT;
  return gyroOrientation;

}

/** TODO: see documentation in header file */
double computeFlatlandRollAcc(double acc[3]) {

  double flatlandRollAcc = atan(acc[0]/acc[1]) * (180.0/3.14159265);
  return flatlandRollAcc;

}

/** TODO: see documentation in header file */
double computeFlatlandRollComp(double flatlandRollCompPrev, double gyr[3], double flatlandRollAcc, double deltaT, double alpha) {

double flatlandRollComp = alpha * ((gyr[2] * deltaT) + flatlandRollCompPrev) + (1 - alpha) * flatlandRollAcc;
  return flatlandRollComp ;

}


/** TODO: see documentation in header file */
void updateQuaternionGyr(Quaternion& q, double gyr[3], double deltaT) {
  // q is the previous quaternion estimate
  // update it to be the new quaternion estimate
  
  double omegaMagnitude = sqrt(pow(gyr[0], 2) + pow(gyr[1], 2) + pow(gyr[2], 2));
  double deltaThita = deltaT * omegaMagnitude;
  double deltaV[3] = {(gyr[0]/omegaMagnitude), (gyr[1]/omegaMagnitude), (gyr[2]/omegaMagnitude)};
  
 Quaternion deltaQuaternion = Quaternion().setFromAngleAxis(deltaThita, deltaV[0], deltaV[1], deltaV[2]);
 q = q.multiply(q, deltaQuaternion);
}


/** TODO: see documentation in header file */
void updateQuaternionComp(Quaternion& q, double gyr[3], double acc[3], double deltaT, double alpha) {
  // q is the previous quaternion estimate
  // update it to be the new quaternion estimate
  double omegaMagnitude = sqrt(pow(gyr[0], 2) + pow(gyr[1], 2) + pow(gyr[2], 2));
  double deltaThita = deltaT * omegaMagnitude;
  double deltaV[3] = {(gyr[0]/omegaMagnitude), (gyr[1]/omegaMagnitude), (gyr[2]/omegaMagnitude)};
  
 Quaternion deltaQuaternion = Quaternion().setFromAngleAxis(deltaThita, deltaV[0], deltaV[1], deltaV[2]);
 q = q.multiply(q, deltaQuaternion);
 
 Quaternion accQuaternion = Quaternion(0, acc[0], acc[1], acc[2]);
 accQuaternion = accQuaternion.rotate(q).normalize();
 double phi = acos(accQuaternion.q[2]) * (180.0/3.14159265);
 double tiltAxis[3] = {-accQuaternion.q[3], 0, accQuaternion.q[1]};
 q = q.multiply(Quaternion().setFromAngleAxis(phi * (1-alpha), tiltAxis[0], tiltAxis[1], tiltAxis[2]), q).normalize();

}
