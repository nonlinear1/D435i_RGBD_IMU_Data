#include <stdio.h>
#include <librealsense2/rs.hpp>	//realsense 2.0 sdk lib
#include "imu_odometry.h"

using namespace rs2;
#define t_max 60	//times of operation

void D435iOperate(pipeline *pipe);

int main(int argc, char* argv[])
{
	config cfg;
	pipeline pipe;
	/*
	cfg.enable_stream(RS2_STREAM_COLOR, 1920, 1080, RS2_FORMAT_RGB8, 30);
	cfg.enable_stream(RS2_STREAM_DEPTH, 1280, 720, RS2_FORMAT_Z16, 30);
	cfg.enable_stream(RS2_STREAM_ACCEL, RS2_FORMAT_MOTION_XYZ32F);
	cfg.enable_stream(RS2_STREAM_GYRO, RS2_FORMAT_MOTION_XYZ32F);
	*/
	cfg.enable_device_from_file("H:\\Downloads\\d435i_sample\\d435i_walk_around.bag");	//offline data pack
	imu_odometry imu_odo;
	double gyro_array[3];
	double accel_array[3];
	pipe.start(cfg);
	/*
	pipe.start(cfg, [&](rs2::frame fra)
	{
		// Cast the frame that arrived to motion frame
		motion_frame motion = fra.as<rs2::motion_frame>();
		// If casting succeeded and the arrived frame is from gyro stream
		if (motion && motion.get_profile().stream_type() == RS2_STREAM_GYRO && motion.get_profile().format() == RS2_FORMAT_MOTION_XYZ32F)
		{
			// Get the timestamp of the current frame
			double ts = motion.get_timestamp();
			// Get gyro measures
			rs2_vector gyro_data = motion.get_motion_data();
			gyro_array[0] = gyro_data.x;
			gyro_array[1] = gyro_data.y;
			gyro_array[2] = gyro_data.z;
			imu_odo.process_gyro(gyro_array, ts);
		}
		// If casting succeeded and the arrived frame is from accelerometer stream
		if (motion && motion.get_profile().stream_type() == RS2_STREAM_ACCEL && motion.get_profile().format() == RS2_FORMAT_MOTION_XYZ32F)
		{
			// Get the timestamp of the current frame
			double ts = motion.get_timestamp();
			// Get accelerometer measures
			rs2_vector accel_data = motion.get_motion_data();
			accel_array[0] = accel_data.x;
			accel_array[1] = accel_data.y;
			accel_array[2] = accel_data.z;
			imu_odo.process_accel(accel_array, ts);
		}
	});
	*/

	// Capture 30 frames to give autoexposure, etc. a chance to settle
	for (int i = 0; i < 30; ++i) pipe.wait_for_frames();

	D435iOperate(&pipe);

	pipe.stop();
	return EXIT_SUCCESS;
}

void D435iOperate(pipeline *pipe)
{
	
	rs2::pointcloud pc;
	rs2::points points;
	rs2::points points_full;
	
	const rs2::vertex *point_3d_co;
	const texture_coordinate *point_text_co;
	
	
	for (int i_t = 0; i_t < t_max; i_t++)
	{
		frameset frames = pipe->wait_for_frames();
		depth_frame depth = frames.get_depth_frame();
		depth_frame color = frames.get_color_frame();
		/*
		pc.map_to(color);
		points = pc.calculate(depth);
		point_3d_co = points.get_vertices();	//3d coordinates (x,y,z) of pointcloud
		point_text_co = points.get_texture_coordinates();	//the texture coordinates of (u,v) of pointcloud
		*/
		if (depth)
		{
			int width = depth.get_width();
			int height = depth.get_height();
			float dist_to_center = depth.get_distance(width / 2.0, height / 2.0);
			printf("%.2f\n", dist_to_center);
		}
	}
}