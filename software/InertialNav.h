#ifndef _INERTIALNAV_H_
#define _INERTIALNAV_H_
#include "sys.h"

#include "AHRS.h"
#include "Barometer.h"
#include "Buffer.h"
#include "QMath.h"


#define INTERTIALNAV_TC_XY   2.5f // default time constant for complementary filter's X & Y axis
#define INTERTIALNAV_TC_Z    5.0f // default time constant for complementary filter's Z axis

// #defines to control how often historical accel based positions are saved
// so they can later be compared to laggy gps readings
#define INTERTIALNAV_SAVE_POS_AFTER_ITERATIONS   10
#define INTERTIALNAV_GPS_LAG_IN_10HZ_INCREMENTS  4       // must not be larger than size of _hist_position_estimate_x and _hist_position_estimate_y
#define INTERTIALNAV_GPS_TIMEOUT_MS              300     // timeout after which position error from GPS will fall to zero



class InertialNav
{
public:
	InertialNav(AHRS &ahrs, Barometer &baro) :
		_ahrs(ahrs),
		_baro(baro),

		_time_constant_xy(0),
		_time_constant_z(INTERTIALNAV_TC_Z),

		_xy_enabled(false),
		_k1_xy(0.0f),
		_k2_xy(0.0f),
		_k3_xy(0.0f),
		_historic_xy_counter(0),

		_k1_z(0.0f),
		_k2_z(0.0f),
		_k3_z(0.0f),
		_baro_last_update(0),

		_error_count(0)
	{

	}

	virtual void init();	//更新增益_kn_xy,_kn_z
	virtual void update(float dt);

	virtual bool position_ok()const { return _xy_enabled; }

	virtual const Vector3f& get_position()const { return _postion; }
	virtual const Vector3f& get_velocity() const { return _velocity; }
	
	virtual float get_velocity_xy()const { return pythagorous2(_velocity.x, _velocity.y); }
	void set_velocity_xy(float x, float y);


	virtual bool altitude_ok()const {return true;}
	virtual float get_altitude()const {	return _postion.z;}
	void set_altitude(float new_altitude);
	virtual float get_velocity_z() const { return _velocity.z; }

	uint8_t error_count()const { return _error_count; }
	void ignore_next_error() { _flags.ignore_error = 7; }

	Vector3f accel_correction_hbf;

// 	virtual int32_t get_latitude()const;
// 	virtual int32_t get_longitude()const;
// 	virtual float get_latitude_diff()const;
// 	virtual float get_longitude_diff()const;
//	void setup_home_postion(void);

protected:

	//void correct_with_gps(uint32_t now, int32_t lon, int32_t lat);
	//void check_gps();

	void check_baro();
	/**
	* correct_with_baro - calculates vertical position error using barometer.
	*
	* @param baro_alt : altitude in cm
	* @param dt : time since last baro reading in s
	*/
	void correct_with_baro(float baro_alt, float dt);
	void update_gains();

	//void set_postion_xy(float x, float y);

	struct InertialNav_flags {
		uint8_t gps_glitching : 1;                // 1 if glitch detector was previously indicating a gps glitch
		uint8_t baro_glitching : 1;                // 1 if baro glitch detector was previously indicating a baro glitch
		uint8_t ignore_error : 3;                // the number of iterations for which we should ignore errors
	} _flags;

	AHRS &_ahrs;
	Barometer &_baro;


	float _time_constant_xy;
	float _time_constant_z;


	//xy估计
	bool _xy_enabled;
	float _k1_xy;
	float _k2_xy;
	float _k3_xy;
	uint8_t _historic_xy_counter;
	BufferFloat_Size5 _hist_position_estimate_x;
	BufferFloat_Size5 _hist_position_estimate_y;


	//z估计
	float _k1_z;
	float _k2_z;
	float _k3_z;
	uint32_t _baro_last_update;
	BufferFloat_Size15 _hist_position_estimate_z;

	//	
	Vector3f _postion_error;
	Vector3f _postion_base;
	Vector3f _postion_correction;
	Vector3f _postion;				//in cm _position = _position_base + _position_correction
	Vector3f _velocity;				//in cm
	//
	uint8_t _error_count;





};

#endif


