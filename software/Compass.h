#ifndef _COMPASS_H_
#define _COMPASS_H_
#include "sys.h"
#include "QMath.h"


//单位uT
class Compass
{
public:	
	Compass() :
		_healthy(false),
		_available(false),
		_have_calibration(false),
		_board_orientation(ROTATION_NONE)
	{

	}

	//更新传感器数据
	virtual void update() = 0;		
	//初始化
	virtual void init(void) {};
	//检查是否校准
	void check_calibration(void);
	bool calibrated() {
		return _have_calibration;
	}
	//传感器是否可用
	bool healthy(void)const { return _healthy; }
	//设置方向
	void set_board_orientation(Rotation orientation) {
		_board_orientation = orientation;
	}

	bool get_available(void)const { return _available; }
	void set_available(bool v) { _available = v; }
	const Vector3f& get_mag(void)const{
		return _mag;
	}

protected:
	//获取传感器数据后需要经过此函数校准
	void _rotate_and_offset(const Vector3f& mag);
	bool _healthy;
	bool _available;
private:

	
	bool _have_calibration;
	Rotation _board_orientation;

	Vector3f _mag;

};

#endif


