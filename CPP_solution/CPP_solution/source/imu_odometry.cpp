#include "imu_odometry.h"

imu_odometry::imu_odometry()
{
}


imu_odometry::~imu_odometry()
{
}

void imu_odometry::process_gyro(double gyro_data[3], double ts)
{
}

void imu_odometry::process_accel(double accel_data[3], double ts)
{
}

void imu_odometry::get_pos(double pos_data[3])
{
	pos_data[0] = x;
	pos_data[1] = y;
	pos_data[2] = z;
}

void imu_odometry::get_qua(double qua_data[4])
{
	qua_data[0] = a;
	qua_data[1] = b;
	qua_data[2] = c;
	qua_data[3] = d;
}