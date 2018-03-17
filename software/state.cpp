#include "state.h"
#include "Notify.h"


void set_pre_arm_check(bool b)
{
	if (state.pre_arm_check != b) {
		state.pre_arm_check = b;
		Notify::flags.pre_arm_check = b;
	}
}
void set_pre_arm_rc_check(bool b)
{
	if (state.pre_arm_rc_check != b){
		state.pre_arm_rc_check = b;
	}

}


#define THROTTLE_ZERO_DEBOUNCE_TIME_MS 400				//¼ì²âµ½0ÓÍÃÅ³¬¹ý400ms
void set_throttle_zero(int16_t throttle_control)
{
	static uint32_t last_nonzero_throttle_ms = 0;
	uint32_t tnow_ms = millis();
	if (throttle_control > 0.0f)
	{
		last_nonzero_throttle_ms = tnow_ms;
		state.throttle_zero = false;
	}
	else if ((tnow_ms - last_nonzero_throttle_ms) > THROTTLE_ZERO_DEBOUNCE_TIME_MS)
	{
		state.throttle_zero = true;
	}
}




// ---------------------------------------------
void set_land_complete(bool b)
{
	// if no change, exit immediately
	if (state.land_complete == b)
		return;

// 	if (b) {
// 		Log_Write_Event(DATA_LAND_COMPLETE);
// 	}
// 	else {
// 		Log_Write_Event(DATA_NOT_LANDED);
// 	}

	state.land_complete = b;
}

// ---------------------------------------------
// set land complete maybe flag
void set_land_complete_maybe(bool b)
{
	// if no change, exit immediately
	if (state.land_complete_maybe == b)
		return;

// 	if (b) {
// 		Log_Write_Event(DATA_LAND_COMPLETE_MAYBE);
// 	}

	state.land_complete_maybe = b;
}


