#ifndef STEWARTPLATFORM_H_
#define STEWARTPLATFORM_H_
#include "stewartplatform_config.h"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// PUBLIC VARIABLES
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// The array of requested position for platform - x,y,z,rot(x),rot(y),rot(z)
// (ie StewartPlatorm INPUT)
float platform_pos[6]   =   {0.0, 0.0, 0.0, radians(0), radians(0), radians(0)};

// The array of motor position (radian angles) to get platform_pos
// (ie StewartPlatorm OUTPUT)
float actuators[6]      =   {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// PUBLIC FUNCTION
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// updateStewartPlatform();
// Platform Position/Rotation IN, Actuator angles (radians) OUT.
// Returns non-zero error
// Typical Usage:
// .................................................................................................
// unsigned char error = 0;
// error = updateStewartPlatform(platform_pos, actuators);
// .................................................................................................
//
int updateStewartPlatform(float platform_pos[], float actuators[]);

#endif// ifdef STEWARTPLATFORM_H_