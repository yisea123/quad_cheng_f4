#ifndef _CONFIG_H_
#define _CONFIG_H_
#include "sys.h"
#include "defines.h"



//////////////////////////////////////////////////////////////////////////////
// Acro Mode
//////////////////////////////////////////////////////////////////////////////
#ifndef ACRO_RP_P
# define ACRO_RP_P                 4.5f
#endif

#ifndef ACRO_YAW_P
# define ACRO_YAW_P                4.5f		//
#endif


#ifndef ACRO_LEVEL_MAX_ANGLE
# define ACRO_LEVEL_MAX_ANGLE      3000
#endif

#ifndef ACRO_BALANCE_ROLL
#define ACRO_BALANCE_ROLL          1.0f
#endif

#ifndef ACRO_BALANCE_PITCH
#define ACRO_BALANCE_PITCH         1.0f
#endif

#ifndef ACRO_EXPO_DEFAULT
#define ACRO_EXPO_DEFAULT          0.3f
#endif

//////////////////////////////////////////////////////////////////////////////
// RCIN
//////////////////////////////////////////////////////////////////////////////

#ifndef RCIN_TYPE
#define  RCIN_TYPE RCIN_SBUS
#endif


#ifndef RC_RPY_INPUT_MAX
# define RC_RPY_INPUT_MAX      4500            // roll, pitch input range
#endif



#ifndef DEFAULT_ANGLE_MAX
# define DEFAULT_ANGLE_MAX         4500            // ANGLE_MAX parameters default value
#endif

#ifndef ANGLE_RATE_MAX
# define ANGLE_RATE_MAX            18000           // default maximum rotation rate in roll/pitch axis requested by angle controller used in stabilize, loiter, rtl, auto flight modes
#endif



//////////////////////////////////////////////////////////////////////////////
// Rate Control PID
//////////////////////////////////////////////////////////////////////////////
//==========ROLL_RATE============================
#ifndef RATE_ROLL_P
# define RATE_ROLL_P        		0.150f
#endif
#ifndef RATE_ROLL_I
# define RATE_ROLL_I        		0.100f
#endif
#ifndef RATE_ROLL_D
# define RATE_ROLL_D        		0.004f
#endif

#ifndef RATE_ROLL_IMAX
# define RATE_ROLL_IMAX         	1000
#endif
//==========PITCH_RATE============================
#ifndef RATE_PITCH_P
# define RATE_PITCH_P       		0.150f
#endif
#ifndef RATE_PITCH_I
# define RATE_PITCH_I       		0.100f
#endif
#ifndef RATE_PITCH_D
# define RATE_PITCH_D       		0.004f
#endif
#ifndef RATE_PITCH_IMAX
# define RATE_PITCH_IMAX        	1000
#endif
//==========YAW_RATE============================
#ifndef RATE_YAW_P
# define RATE_YAW_P              	0.200f
#endif
#ifndef RATE_YAW_I
# define RATE_YAW_I              	0.020f
#endif
#ifndef RATE_YAW_D
# define RATE_YAW_D              	0.000f
#endif
#ifndef RATE_YAW_IMAX
# define RATE_YAW_IMAX            	1000
#endif

// Stabilize (angle controller) gains
#ifndef STABILIZE_ROLL_P
# define STABILIZE_ROLL_P          2.5f
#endif

#ifndef STABILIZE_PITCH_P
# define STABILIZE_PITCH_P         2.5f
#endif

#ifndef  STABILIZE_YAW_P
# define STABILIZE_YAW_P           3.5f
#endif



//////////////////////////////////////////////////////////////////////////////
// Throttle control gains
//
#ifndef THROTTLE_CRUISE
# define THROTTLE_CRUISE       450             // default estimate of throttle required for vehicle to maintain a hover
#endif

#ifndef THR_MID_DEFAULT
# define THR_MID_DEFAULT       500             // Throttle output (0 ~ 1000) when throttle stick is in mid position
#endif

#ifndef THR_MIN_DEFAULT
# define THR_MIN_DEFAULT       130             // minimum throttle sent to the motors when armed and pilot throttle above zero
#endif
#ifndef THR_MAX_DEFAULT
# define THR_MAX_DEFAULT       1000            // maximum throttle sent to the motors
#endif

#ifndef THR_DZ_DEFAULT
# define THR_DZ_DEFAULT         50             // the deadzone above and below mid throttle while in althold or loiter
#endif

#ifndef ALT_HOLD_P
# define ALT_HOLD_P            1.0f
#endif

// RATE control
#ifndef THROTTLE_RATE_P
# define THROTTLE_RATE_P       5.0f
#endif

// Throttle Accel control
#ifndef THROTTLE_ACCEL_P
# define THROTTLE_ACCEL_P      0.50f
#endif
#ifndef THROTTLE_ACCEL_I
# define THROTTLE_ACCEL_I      1.00f
#endif
#ifndef THROTTLE_ACCEL_D
# define THROTTLE_ACCEL_D      0.0f
#endif
#ifndef THROTTLE_ACCEL_IMAX
# define THROTTLE_ACCEL_IMAX   800
#endif

// default maximum vertical velocity and acceleration the pilot may request
#ifndef PILOT_VELZ_MAX
# define PILOT_VELZ_MAX    250     // maximum vertical velocity in cm/s
#endif
#ifndef PILOT_ACCEL_Z_DEFAULT
# define PILOT_ACCEL_Z_DEFAULT 250 // vertical acceleration in cm/s/s while altitude is under pilot control
#endif


//////////////////////////////////////////////////////////////////////////////
// Throttle control gains
//////////////////////////////////////////////////////////////////////////////
#ifndef THROTTLE_CRUISE						
# define THROTTLE_CRUISE       450				// default estimate of throttle required for vehicle to maintain a hover
#endif

#ifndef THR_MID_DEFAULT										
# define THR_MID_DEFAULT       500				// Throttle output (0 ~ 1000) when throttle stick is in mid position
#endif

#ifndef THR_MIN_DEFAULT							
# define THR_MIN_DEFAULT       100				 // minimum throttle sent to the motors when armed and pilot throttle above zero,最小0
#endif

#ifndef THR_MAX_DEFAULT														
# define THR_MAX_DEFAULT       1000				// maximum throttle sent to the motors,最大1000
#endif

#ifndef THR_DZ_DEFAULT				
# define THR_DZ_DEFAULT         100				//the deadzone above and below mid throttle while in althold or loiter
#endif


//////////////////////////////////////////////////////////////////////////////
// Throttle Failsafe
//////////////////////////////////////////////////////////////////////////////
#ifndef FS_THR_VALUE_DEFAULT				//低于此值启用油门保护
# define FS_THR_VALUE_DEFAULT       310		//SBUS,最小值350左右(+600=975)  
#endif
#endif

#ifndef LAND_SPEED
# define LAND_SPEED    50          // the descent speed for the final stage of landing in cm/s
#endif



// Radio failsafe
#ifndef FS_RADIO_TIMEOUT_MS						//无线信号丢失保护时间
#define FS_RADIO_TIMEOUT_MS            500		// RC Radio Failsafe triggers after 500 miliseconds with No RC Input
#endif


