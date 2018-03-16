


//返回修正后的加速度，不能添加到xp修正
//完全崩溃！！！
float acc_corect(float dt,float& ap,float& vp,float& xp,float& ae,float& ve,float& xm,float& k)
{
	
	xp += (vp-ve)*dt;
	float dpm = xp - xm;	//xe + R
	ve += k*dpm/dt;			//用于下次修正xp
	ae += ve/dt;
	
	float ar = ap -ae;
	vp += ar*dt;
	
	float ar;
}