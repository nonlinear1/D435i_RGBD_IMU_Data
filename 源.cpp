#include <windows.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include<conio.h>
#include<vector>
#include "librealsense2/rs.hpp"
#pragma comment(lib, "realsense2.lib") //等同于在链接器的输入中添加附加依赖项
using namespace rs2;
using namespace std;

int main()
{
	// Create a Pipeline, which serves as a top-level API for streaming and processing frames
	pipeline p;

	// Configure and start the pipeline
	p.start();
	vector<float>dist_to_center;	
	char ch;
	while (!_kbhit())
	{
		// Block program until frames arrive
		frameset frames = p.wait_for_frames();

		// Try to get a frame of a depth image
		depth_frame depth = frames.get_depth_frame();
		// The frameset might not contain a depth frame, if so continue until it does
		if (!depth) continue;

		// Get the depth frame's dimensions
		float width = depth.get_width();
		float height = depth.get_height();

		// Query the distance from the camera to the object in the center of the image
		float ftem = depth.get_distance(width / 2, height / 2);
		dist_to_center.push_back(ftem);

		// Print the distance 
		cout << "The camera is facing an object " << ftem << " meters away. Press ESC stop!\r";
		
		if (_kbhit())
		{
			ch = _getch();
			if (27 == ch)break;
		}
		Sleep(5000);
	}
	
	FILE*fp;
	fp = fopen("test.TXT", "w");
	for (int i = 0; i < dist_to_center.size(); i++)
	{
		fprintf(fp, "%.4f %d\n", dist_to_center[i], i);
	}
	
	fclose(fp);
	return 0;
}