// WARNING: remove this header when compiling
#include "headers/NBCCommon.h"

/*
 * ██████████
 * █ STATES █
 * ██████████
 */
#define STATE_INITIALIZE 0
#define STATE_FOLLOW_LINE 1

/*
 * █████████████
 * █ CONSTANTS █
 * █████████████
 */
#define SENSOR_LEFT IN_1
#define SENSOR_RIGHT IN_2
#define SENSOR_LINE IN_3
#define SENSOR_ULTRASONIC IN_4

#define MOTOR_LEFT OUT_A
#define MOTOR_RIGHT OUT_B
#define MOTOR_BOTH OUT_AB
#define MOTOR_CURVE_DRAW 20

/*
 * ████████████████████
 * █ GLOBAL VARIABLES █
 * ████████████████████
 */
int state = STATE_INITIALIZE;

/*
 * ████████████████████
 * █ GLOBAL VARIABLES █
 * ████████████████████
 */
int light_threshold = -1; // NOTE: lighter color reflects more light -> Higher measurement

int initialize() {
  // initialize sensors
  SetSensorLight(SENSOR_LEFT);
  SetSensorLight(SENSOR_RIGHT);
  SetSensorLight(SENSOR_LINE);
  SetSensorUltrasonic(SENSOR_ULTRASONIC);

  // calculate difference between floor and line
  int test = Sensor(SENSOR_LEFT);
  unsigned int light_average = (Sensor(SENSOR_LEFT) + Sensor(SENSOR_RIGHT)) / 2;
  light_threshold = 0.3 * (light_average - Sensor(SENSOR_LINE));

  return STATE_FOLLOW_LINE;
}

int follow_line() {
  unsigned int value_line = Sensor(SENSOR_LINE);
  unsigned int difference_left = Sensor(SENSOR_LEFT) - value_line;
  unsigned int difference_right = Sensor(SENSOR_RIGHT) - value_line;

  if (difference_left > light_threshold && difference_right > light_threshold) {
    // NOTE: move forward
    OnFwd(MOTOR_BOTH, 100);
  } else if (difference_left <= light_threshold && difference_right > light_threshold) {
    // NOTE: move left
    OnFwd(MOTOR_RIGHT, 100);
    OnFwd(MOTOR_LEFT, 100 - MOTOR_CURVE_DRAW);
  } else if (difference_left > light_threshold && difference_right <= light_threshold) {
    // NOTE: move right
    OnFwd(MOTOR_LEFT, 100);
    OnFwd(MOTOR_RIGHT, 100 - MOTOR_CURVE_DRAW);
  } else {
    // TODO: move forward and count gap
  }
  return -1;
}

task main() {
  while (1) {
    switch (state) {
    case STATE_INITIALIZE:
      state = initialize();
      break;
    case STATE_FOLLOW_LINE:
      state = follow_line();
      break;
    }
  }
}
