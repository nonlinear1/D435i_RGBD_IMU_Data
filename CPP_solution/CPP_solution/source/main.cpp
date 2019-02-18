#include <stdio.h>
#include <librealsense2/rs.hpp>	//realsense 2.0 sdk lib

using namespace rs2;

#define t_max 60	//Maximum steps of running

void D435i_Operate(pipeline *pipe);

int main(int argc, char* argv[])
{
	config cfg;
	pipeline pipe;
	
	cfg.enable_stream(RS2_STREAM_COLOR, 1920, 1080, RS2_FORMAT_RGB8, 30);
	cfg.enable_stream(RS2_STREAM_DEPTH, 1280, 720, RS2_FORMAT_Z16, 30);
	cfg.enable_stream(RS2_STREAM_GYRO);
	cfg.enable_stream(RS2_STREAM_ACCEL);
	cfg.enable_stream(RS2_STREAM_POSE);
	
	//cfg.enable_device_from_file("H:\\Downloads\\d435i_sample\\d435i_walk_around.bag");	//offline data pack
	pipe.start(cfg);
	// Capture 30 frames to give autoexposure, etc. a chance to settle
	for (int i = 0; i < 30; ++i) pipe.wait_for_frames();

	D435i_Operate(&pipe);

	pipe.stop();
	return EXIT_SUCCESS;
}

void D435i_Operate(pipeline *pipe)
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

		//Read acceleration data
		if (rs2::motion_frame accel_frame = frames.first_or_default(RS2_STREAM_ACCEL))
		{
			rs2_vector accel_sample = accel_frame.get_motion_data();
			printf("accel: (%.3f,%.3f,%.3f)\n", accel_sample.x, accel_sample.y, accel_sample.z);
		}
		//Read gyro data
		if (rs2::motion_frame gyro_frame = frames.first_or_default(RS2_STREAM_GYRO))
		{
			rs2_vector gyro_sample = gyro_frame.get_motion_data();
			printf("gyrp: (%.3f,%.3f,%.3f)\n", gyro_sample.x, gyro_sample.y, gyro_sample.z);
		}
		//Read pose tracing data produced by D435i
		if (rs2::pose_frame pose_frame = frames.first_or_default(RS2_STREAM_POSE))
		{
			rs2_pose pose_sample = pose_frame.get_pose_data();
			printf("tran: (%.3f,%.3f,%.3f)\n", pose_sample.translation.x, pose_sample.translation.y, pose_sample.translation.z);
			printf("tran_v: (%.3f,%.3f,%.3f)\n", pose_sample.velocity.x, pose_sample.velocity.y, pose_sample.velocity.z);
			printf("tran_a: (%.3f,%.3f,%.3f)\n", pose_sample.acceleration.x, pose_sample.acceleration.y, pose_sample.acceleration.z);
			printf("ang: (%.3f,%.3f,%.3f,%.3f)\n", pose_sample.rotation.x, pose_sample.rotation.y, pose_sample.rotation.z, pose_sample.rotation.w);
			printf("ang_v: (%.3f,%.3f,%.3f)\n", pose_sample.angular_velocity.x, pose_sample.angular_velocity.y, pose_sample.angular_velocity.z);
			printf("ang_a: (%.3f,%.3f,%.3f)\n", pose_sample.angular_acceleration.x, pose_sample.angular_acceleration.y, pose_sample.angular_acceleration.z);
			printf("tracker_conf: (%d)\n", pose_sample.tracker_confidence);
			printf("mapper_conf: (%d)\n", pose_sample.mapper_confidence);
		}
		//Measure the distance of centre of the image
		if (depth)
		{
			int width = depth.get_width();
			int height = depth.get_height();
			float dist_to_center = depth.get_distance(width / 2, height / 2);
			printf("%.2f\n", dist_to_center);
		}
	}
}