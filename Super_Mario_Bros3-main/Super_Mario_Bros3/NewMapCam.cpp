#include "NewMapCam.h"


CNewMapCam::CNewMapCam(int id, float start_cam_x, float y_limit, float end_cam_x)
{
	this->id = id;
	this->start_cam_x = start_cam_x;
	this->end_cam_x = end_cam_x;
	this->y_limit = y_limit;
}
