#ifndef _DEFINES_H_
#define _DEFINES_H_
#include "sys.h"

// RC Feel roll/pitch definitions
#define RC_FEEL_RP_VERY_SOFT        0
#define RC_FEEL_RP_SOFT             25
#define RC_FEEL_RP_MEDIUM           50
#define RC_FEEL_RP_CRISP            75
#define RC_FEEL_RP_VERY_CRISP       100


//RCIN
#define RCIN_SBUS
#define RCIN_PPM
#define RCIN_PWM

// Radio failsafe definitions (FS_THR parameter)
#define FS_THR_DISABLED                    	0
#define FS_THR_ENABLED                    	1


// Arming Check Enable/Disable bits
#define ARMING_CHECK_NONE                   0x00
#define ARMING_CHECK_BARO                   0x01
#define ARMING_CHECK_COMPASS                0x02
#define ARMING_CHECK_GPS                    0x04
#define ARMING_CHECK_INS                    0x08
#define ARMING_CHECK_PARAMETERS             0x10
#define ARMING_CHECK_RC                     0x20
#define ARMING_CHECK_VOLTAGE                0x40
#define ARMING_CHECK_ALL                    (ARMING_CHECK_BARO|ARMING_CHECK_COMPASS|ARMING_CHECK_GPS|\
												ARMING_CHECK_INS|ARMING_CHECK_PARAMETERS|ARMING_CHECK_RC|\
												ARMING_CHECK_VOLTAGE)


//Flight Mode
enum 
{
	// Auto Pilot modes
	// ----------------
	STABILIZE = 0,                     // hold level position
	ALT_HOLD,                     // AUTO control
	ACRO,								// rate control
	POSHOLD,                  // position hold with manual override

// 	AUTO,                         // AUTO control
// 	GUIDED,                        // AUTO control
// 	LOITER,                       // Hold a single location
// 	RTL,                          // AUTO control
// 	CIRCLE,                        // AUTO control
// 	LAND,                        // AUTO control
// 	OF_LOITER,                   // Hold a single location using optical flow sensor
// 	DRIFT,                      // DRIFT mode (Note: 12 is no longer used)
// 	SPORT,                       // earth frame rate control
// 	FLIP,                 // flip the vehicle on the roll axis
// 	AUTOTUNE,                 // autotune the vehicle's roll and pitch gains
	NUM_MODES,

};

#endif

