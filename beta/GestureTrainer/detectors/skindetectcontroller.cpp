
/*
	Modified from ColorDetector from chapter 3 of the cookbook:
	Computer Vision Programming using the OpenCV Library.
	by Robert Laganiere, Packt Publishing, 2011.

	Created by: Jason Carlisle Mann (on2valhalla | jcm2207@columbia.edu)

	A singleton wrapper class for SkinDetector, to gain continuity
	throughout the program, and also to provide more simple access
	to its methods, and a local cache of the most recent images.
*/

#include "skindetectcontroller.h"


SkinDetectController *SkinDetectController::singleton=0;
