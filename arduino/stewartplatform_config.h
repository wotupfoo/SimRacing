#ifndef STEWARTPLATFORM_CONFIG_H_
#define STEWARTPLATFORM_CONFIG_H_

// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
// NOTE - ALL UNITS ARE IN MM unless noted otherwise (eg degrees)
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-

// Maximim angle up|down from horizontal of the servo.
//  Default is 45degrees to keep motion as linear as possible
//  The closer you get to the extreme up/down, the less linear 
//  change will be. eg sin(90)~=sin(85) so you want to stay away
//  from that.
const float SERVO_MAX_ANGLE = 45; // (in degrees)

// L1 - Effective length of servo arm (in millimeters)
// This is the length of the actuator's lever
const float L1 = 50.0;  // eg 50mm

// Allowed error in the Inverse Kinematic calculation of how long L1 is
// based on the newly desired location of the platform. If the new location
// is less than this value the servo angle won't be updated.
//
// Increase to remove "twitching" of the actuator.
// L2_DEADZONE MUST BE GREATER THAN ZERO.
const float L1_DEADZONE = 1.0; // eg 1mm

// L2 - Actual Length of base and platform connecting arms (all 6 must be the same)
// This is the length of the arm conneting each actuator to the platform
const float L2 = 250;  // eg 250mm

// Z_HOME - height of platform above base, 0 is height of servo arms horizontal plane
// Typically the length of the rods (L2) if the actuators are level at angle=0;
// Non-zero in NOT TESTED, and is likely not to work.
const float Z_HOME = 0.0;

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Stewart Platform Geometry Values
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Background on the Inverse Kinematics of a Stewart Platform using Rotary Actutors (vs linear actuators
// where the length of the arms changes).
// https://www.instructables.com/Stewart-Platform/
// https://raw.org/research/inverse-kinematics-of-a-stewart-platform/

//RD distance from center of base to center of servo rotation points (servo axis)
const float RD = 2.42;

//PD distance from center of platform to attachment points (arm attachment point)
const float PD =2.99;


#endif// ifdef STEWARTPLATFORM_CONFIG_H_