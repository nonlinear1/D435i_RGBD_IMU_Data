#pragma once
class imu_odometry
{
public:
	imu_odometry();
	~imu_odometry();
	void process_gyro(double gyro_data[3], double ts);
	void process_accel(double accel_data[3], double ts);
	void get_pos(double pos_data[3]);
	void get_qua(double qua_data[4]);
private:
	// (x,y,z) position of camera
	float x=0;
	float y=0;
	float z=0;
	// (a,b,c,d) quaternion of camera
	float a=0;
	float b=0;
	float c=0;
	float d=0;
};

