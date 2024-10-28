/*
    https://github.com/ThomasKNR/RotaryStewartPlatform
    <Controlling code for Arduino Controlled Rotary Stewart Platform>
    Copyright (C) <2014>  <Tomas Korgo>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.*/

/*
    https://github.com/wotupfoo/SimRacing
    <Stewart Platform for Sim Racing>
    Copyright (C) <2024>  <Edward Richards>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.*/


// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// STEWART PLATFORM CONSTANTS
// ADJUST THE GEOMETRY FOR THINGS LIKE ROD LENGTH AND SPACING
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
#include "stewartplatform_private.h"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// STEWART PLATFORM VARIABLES
// ADJUST THE DEFAULTS AS NEEDED
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// Actual degree of rotation of all servo arms, they start at 0 - horizontal, used to reduce
// complexity of calculating new degree of rotation
float servo_angle_theta_a[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

// Array of current servo positions in us
int servo_angle_usec[6];

// Translation Matrix of Platform
float PlatformTranslationMatrix[3];
// Rotation Matrix of Platform
float PlatformRotationMatrix[3][3];
float rxp[3][6];

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Calculate the new angle of actuator[i] based on the desired new Translation and Rotation of
// the Platform. 
// Returns actuator[i] angle 'theta' in Radians.
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
static float getAlpha(int *i){
   int n = 0;
   float th = 0;
   float q[3];  // 3axis vector from the base actuators to platform attachment points
   float dl[3]; // Desired leg length[x,y,z] of virtual arm
   float dl2;   // Desired leg length (this is a virtual length of actual arm + actuator arm)

   float servo_min_th = SERVO_MIN_ANGLE_RADIANS;
   float servo_max_th = SERVO_MAX_ANGLE_RADIANS;

   // X,Y vector from the center of the base to each actuator i.
   cos_BETA_i = COS_BETA[*i];
   sin_BETA_i = SIN_BETA[*i];

   while(n<20){
      // Use Inverse Kinematics to calculate where the 
      // calculation of position of base attachment point (point on servo arm where is leg connected)
      // one per co-ordinate axis
      q[0] = L1*cos(th)*cos_BETA_i + p[0][*i];
      q[1] = L1*cos(th)*sin_BETA_i + p[1][*i];
      q[2] = L1*sin(th);

      // calculation of distance between according platform attachment point and base attachment point
      dl[0] = rxp[0][*i] - q[0]; // X
      dl[1] = rxp[1][*i] - q[1]; // Y
      dl[2] = rxp[2][*i] - q[2]; // Z
      dl2 = sqrt(dl[0]*dl[0] + dl[1]*dl[1] + dl[2]*dl[2]);  // Leg length from X,Y,Z distances

      // If this new distance is the same as leg length, value of servo_angle_theta_a is correct,
      // we return it
      if( abs(L2 - dl2) < L1_DEADZONE){
         return th;
      }

      // If not, we split the searched space in half, then try next value
      if(dl2 < L2){
         d_servo_max_th = th;
      }else{
         d_servo_min_th = th;
      }

      n++;

      if(servo_max_th == SERVO_MIN_ANGLE_RADIANS || servo_min_th == SERVO_MAX_ANGLE_RADIANS){
         return th;
      }
      th = servo_min_th + (d_servo_max_th - d_servo_min_th)/2;
   }

   return th;
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Create the new Platform Rotation Matrix based on the new rot[x,y,z]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
static void getPlatformRotationMatrix(float pe[])
{
   float psi=pe[5];     // rot(z)
   float theta=pe[4];   // rot(y)
   float phi=pe[3];     // rot(x)

   // Pre-calculation of sin|cos psi|theta optimization
   float cos_psi=cos(psi);
   float cos_theta=cos(theta);
   float sin_psi=sin(psi);
   float sin_theta=sin(theta);

   PlatformRotationMatrix[0][0] = cos_psi*cos_theta;
   PlatformRotationMatrix[1][0] = -sin_psi*cos_phi+cos_psi*sin_theta*sin_phi;
   PlatformRotationMatrix[2][0] = sin_phi*sin_phi+cos_psi*cos_phi*sin_theta;

   PlatformRotationMatrix[0][1] = sin_phi*cos_theta;
   PlatformRotationMatrix[1][1] = cos_psi*cos_phi+sin_phi*sin_theta*sin_phi;
   PlatformRotationMatrix[2][1] = cos_theta*sin_phi;

   PlatformRotationMatrix[0][2] = -sin_theta;
   PlatformRotationMatrix[1][2] = -cos_psi*sin_phi+sin_phi*sin_theta*cos_phi;
   PlatformRotationMatrix[2][2] = cos_theta*cos_phi;
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Calculating translation vector - desired move vector + home translation vector
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
static void getPlatformTranslationMatrix(float pe[])
{
   PlatformTranslationMatrix[0] = pe[0]+H[0];
   PlatformTranslationMatrix[1] = pe[1]+H[1];
   PlatformTranslationMatrix[2] = pe[2]+H[2];
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Calculates wanted position of platform attachment points using calculated rotation matrix
// and translation vector
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
static void getrxp(float pe[])
{
   for(int i=0;i<6;i++){
      rxp[0][i] = PlatformTranslationMatrix[0]+PlatformRotationMatrix[0][0]*(re[0][i])+PlatformRotationMatrix[0][1]*(re[1][i])+PlatformRotationMatrix[0][2]*(re[2][i]);
      rxp[1][i] = PlatformTranslationMatrix[1]+PlatformRotationMatrix[1][0]*(re[0][i])+PlatformRotationMatrix[1][1]*(re[1][i])+PlatformRotationMatrix[1][2]*(re[2][i]);
      rxp[2][i] = PlatformTranslationMatrix[2]+PlatformRotationMatrix[2][0]*(re[0][i])+PlatformRotationMatrix[2][1]*(re[1][i])+PlatformRotationMatrix[2][2]*(re[2][i]);
   }
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// PUBLIC FUNCTION
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void updateStewartPlatform(float platform_pos[], float actuators[]){
   for(int i = 0; i < 6; i++)
   {
       getPlatformTranslationMatrix(platform_pos);
       getPlatformRotationMatrix(platform_pos);
       getrxp(platform_pos);
       actuators[i]=getAlpha(&i);
   }
}
