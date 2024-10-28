#ifndef STEWARTPLATFORM_PRIVATE_H_
#define STEWARTPLATFORM_PRIVATE_H_
#include "stewartplatform.h"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Constants for computation of positions of connection points
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
#define STEWART_PI  3.14159
//#define deg2rad 180/pi
#define DEG30 STEWART_PI/6

// Looking down on the base plate with the X-axis going to the right
// These are the angles of rotation in the Z axis to place each actuator
// i.e. Rotation of servo arms (base location) in respect to axis x
const float BETA[] = {  STEWART_PI/2,
                        -STEWART_PI/2,
                        -STEWART_PI/6,
                        5*STEWART_PI/6,
                        -5*STEWART_PI/6,
                        STEWART_PI/6 };

const float COS_BETA[] = { cos(BETA[0]),
                           cos(BETA[1]),
                           cos(BETA[2]),
                           cos(BETA[3]),
                           cos(BETA[4]),
                           cos(BETA[5]) };
                        
const float SIN_BETA[] = { sin(BETA[0]),
                           sin(BETA[1]),
                           sin(BETA[2]),
                           sin(BETA[3]),
                           sin(BETA[4]),
                           sin(BETA[5]) };

// Maximum servo rotation, 0 is horizontal position. Defaults SERVO_MAX_ANGLE is 45 degrees (90 total rotation).
const float SERVO_MIN_ANGLE_RADIANS = radians(-SERVO_MAX_ANGLE);
const float SERVO_MAX_ANGLE_RADIANS = radians(SERVO_MAX_ANGLE);

// servo_mult - multiplier used for conversion radians->servo pulse in us
const float SERVO_MULT = 400/(STEWART_PI/4);

// theta_p     - angle between two servo axis points,
// THETHA_R     - between platform attachment points
// THETA_ANGLE - helper variable
const float THETHA_P = radians(37.5);
const float THETHA_R = radians(8);
const float THETA_ANGLE = (STEWART_PI/3-THETHA_P)/2;

// Equations used for p and re will affect postion of X,Y axis, they can be changed to achieve 
// specific X,Y axis position

//p[][]        =x y values for servo rotation points
const float      p[2][6]={
                    {
                    -PD*cos(DEG30-THETA_ANGLE),
                    -PD*cos(DEG30-THETA_ANGLE),
                    PD*sin(THETA_ANGLE),
                    PD*cos(DEG30+THETA_ANGLE),
                    PD*cos(DEG30+THETA_ANGLE),
                    PD*sin(THETA_ANGLE)
                    },
                    {
                    -PD*sin(DEG30-THETA_ANGLE),
                    PD*sin(DEG30-THETA_ANGLE),
                    PD*cos(THETA_ANGLE),
                    PD*sin(DEG30+THETA_ANGLE),
                    -PD*sin(DEG30+THETA_ANGLE),
                    -PD*cos(THETA_ANGLE)
                    }
                  };

//re[]{}       =x y z values of platform attachment points positions
const float      re[3][6] = {
                    {
                        -RD*sin(DEG30+THETHA_R/2),
                        -RD*sin(DEG30+THETHA_R/2),
                        -RD*sin(DEG30-THETHA_R/2),
                        RD*cos(THETHA_R/2),
                        RD*cos(THETHA_R/2),
                        -RD*sin(DEG30-THETHA_R/2),
                    },
                    {
                        -RD*cos(DEG30+THETHA_R/2),
                        RD*cos(DEG30+THETHA_R/2),
                        RD*cos(DEG30-THETHA_R/2),
                        RD*sin(THETHA_R/2),
                        -RD*sin(THETHA_R/2),
                        -RD*cos(DEG30-THETHA_R/2),
                    },
                    {
                        0,
                        0,
                        0,
                        0,
                        0,
                        0
                    }
                  };

// H[] - center position of platform can be moved with respect to base, this is
const float H[3] = {0,0,Z_HOME}; // X,Y,Z

// Platform Translation and Rotation Matrix Generation
static void getPlatformRotationMatrix(float pe[]);
static void getPlatformTranslationMatrix(float pe[]);
static void getrxp(float pe[]);
#endif// ifdef STEWARTPLATFORM_PRIVATE_H_