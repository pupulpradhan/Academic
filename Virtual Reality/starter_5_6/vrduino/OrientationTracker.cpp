#include "OrientationTracker.h"

//TODO: fill in from hw 4 as necessary

OrientationTracker::OrientationTracker(double imuFilterAlphaIn,  bool simulateImuIn) :

  imu(),
  gyr{0,0,0},
  acc{0,0,0},
  gyrBias{0,0,0},
  gyrVariance{0,0,0},
  accBias{0,0,0},
  accVariance{0,0,0},
  previousTimeImu(0),
  imuFilterAlpha(imuFilterAlphaIn),
  deltaT(0.0),
  simulateImu(simulateImuIn),
  simulateImuCounter(0),
  flatlandRollGyr(0),
  flatlandRollAcc(0),
  flatlandRollComp(0),
  quaternionGyr{1,0,0,0},
  eulerAcc{0,0,0},
  quaternionComp{1,0,0,0}

  {

}

void OrientationTracker::initImu() {
  Wire1.setSCL(ICM_SCL);
  Wire1.setSDA(ICM_SDA);
  Wire1.begin(ICM_ADR);

  // Try to initialize!
  if (!imu.begin_I2C(ICM_ADR, &Wire1)) {
    Serial.println("Failed to find ICM20948 chip");
    while (1) {
      delay(10);
    }
  }
}


/**
 * TODO: see documentation in header file
 */
void OrientationTracker::measureImuBiasVariance() {

  //check if imu.read() returns true
  //then read imu.gyrX, imu.accX, ...

  //compute bias, variance.
  //update:
  //gyrBias[0], gyrBias[1], gyrBias[2]
  //gyrVariance[0], gyrBias[1], gyrBias[2]
  //accBias[0], accBias[1], accBias[2]
  //accVariance[0], accBias[1], accBias[2]
  
float gyrXSum = 0.0;
float gyrYSum = 0.0;
float gyrZSum = 0.0;
float accXSum = 0.0;
float accYSum = 0.0;
float accZSum = 0.0;
float gyrVarianceSumX = 0.0;
float gyrVarianceSumY = 0.0;
float gyrVarianceSumZ = 0.0;
float accVarianceSumX = 0.0;
float accVarianceSumY = 0.0;
float accVarianceSumZ = 0.0;
float gyrXArray[1000];
float gyrYArray[1000];
float gyrZArray[1000];
float accXArray[1000];
float accYArray[1000];
float accZArray[1000];

for(int i = 0; i<1000.0; i++){
  updateImuVariables();
  gyrXSum += gyr[0];
  gyrYSum += gyr[1];
  gyrZSum += gyr[2];
  accXSum += acc[0];
  accYSum += acc[1];
  accZSum += acc[2];
  gyrXArray[i] = gyr[0];
  gyrYArray[i] = gyr[1];
  gyrZArray[i] = gyr[2];
  accXArray[i] = acc[0];
  accYArray[i] = acc[1];
  accZArray[i] = acc[2];
  delay(2);
}
gyrBias[0] = gyrXSum/1000.0;
  gyrBias[1] = gyrYSum/1000.0;
  gyrBias[2] = gyrZSum/1000.0;
  accBias[0] = accXSum/1000.0;
  accBias[1] = accYSum/1000.0;
  accBias[2] = accZSum/1000.0;
for(int i = 0; i<1000; i++){ 
gyrVarianceSumX += pow((gyrXArray[i] - gyrBias[0]),2);
  gyrVarianceSumY += pow((gyrYArray[i] - gyrBias[1]),2);
  gyrVarianceSumZ += pow((gyrZArray[i] - gyrBias[2]),2);
  accVarianceSumX += pow((accXArray[i] - accBias[0]),2);
  accVarianceSumY += pow((accYArray[i] - accBias[1]),2);
  accVarianceSumZ += pow((accZArray[i]  - accBias[2]),2);
}
gyrVariance[0] = gyrVarianceSumX/999.0;
gyrVariance[1] = gyrVarianceSumY/999.0;
gyrVariance[2] = gyrVarianceSumZ/999.0;
accVariance[0] = accVarianceSumX/999.0;
accVariance[1] = accVarianceSumY/999.0;
accVariance[2] = accVarianceSumZ/999.0;
}


void OrientationTracker::setImuBias(double bias[3]) {

  for (int i = 0; i < 3; i++) {
    gyrBias[i] = bias[i];
  }

}

void OrientationTracker::resetOrientation() {

  flatlandRollGyr = 0;
  flatlandRollAcc = 0;
  flatlandRollComp = 0;
  quaternionGyr = Quaternion();
  eulerAcc[0] = 0;
  eulerAcc[1] = 0;
  eulerAcc[2] = 0;
  quaternionComp = Quaternion();

}

bool OrientationTracker::processImu() {

  if (simulateImu) {

    //get imu values from simulation
    updateImuVariablesFromSimulation();

  } else {

    //get imu values from actual sensor
    if (!updateImuVariables()) {

      //imu data not available
      return false;

    }

  }

  //run orientation tracking algorithms
  updateOrientation();

  return true;

}

void OrientationTracker::updateImuVariablesFromSimulation() {

    deltaT = 0.002;
    //get simulated imu values from external file
    for (int i = 0; i < 3; i++) {
      gyr[i] = imuData[simulateImuCounter + i];
    }
    simulateImuCounter += 3;
    for (int i = 0; i < 3; i++) {
      acc[i] = imuData[simulateImuCounter + i];
    }
    simulateImuCounter += 3;
    simulateImuCounter = simulateImuCounter % nImuSamples;

    //simulate delay
    delay(1);

}

/**
 * TODO: see documentation in header file
 */
bool OrientationTracker::updateImuVariables() {

  //sample imu values
  sensors_event_t accel;
  sensors_event_t gyro;
  sensors_event_t mag;
  sensors_event_t temp;
  imu.getEvent(&accel, &gyro, &temp, &mag);

  //call micros() to get current time in microseconds
  //update:
  //previousTimeImu (in seconds)
  //deltaT (in seconds)

  //read imu.gyrX, imu.accX ...
  //update:
  //gyr[0], ...
  //acc[0], ...

  // You also need to appropriately modify the update of gyr as instructed in (2.1.3).
  gyr[0] = gyro.gyro.x - gyrBias[0];
  gyr[1] = gyro.gyro.y - gyrBias[1];
  gyr[2] = gyro.gyro.z - gyrBias[2];

  acc[0] = accel.acceleration.x - accBias[0];
  acc[1] = accel.acceleration.y - accBias[1];
  acc[2] = accel.acceleration.z - accBias[2];

  float currentTime = micros();
  deltaT = currentTime - previousTimeImu;
  previousTimeImu = currentTime;

  return true;

}


/**
 * TODO: see documentation in header file
 */
void OrientationTracker::updateOrientation() {

  //call functions in OrientationMath.cpp.
  //use only class variables as arguments to functions.

  //update:
  //flatlandRollGyr
  flatlandRollGyr = computeFlatlandRollGyr(flatlandRollGyr, gyr, deltaT);
  //flatlandRollAcc
  flatlandRollAcc = computeFlatlandRollAcc(acc);
  //flatlandRollComp
  flatlandRollComp = computeFlatlandRollComp(flatlandRollComp, gyr, flatlandRollAcc, deltaT, imuFilterAlpha);
  //quaternionGyr
  updateQuaternionGyr(quaternionGyr, gyr, deltaT);
  //eulerAcc
  eulerAcc[0] = computeAccPitch(acc);
  eulerAcc[2] = computeAccRoll(acc);
  //quaternionComp
  updateQuaternionComp(quaternionGyr, gyr, acc, deltaT, imuFilterAlpha);




}
