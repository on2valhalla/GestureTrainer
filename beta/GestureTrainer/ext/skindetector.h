/*
	Modified from ColorDetector from chapter 3 of the cookbook:
	Computer Vision Programming using the OpenCV Library.
	by Robert Laganiere, Packt Publishing, 2011.

	Created by: Jason Carlisle Mann (on2valhalla | jcm2207@columbia.edu)

	This class uses holds input threshold min and max
	masks to process an image for skin blobs in HSV colorspace
*/

#if !defined SKINDETECT
#define SKINDETECT

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>



class SkinDetector
{

	private:

		// HSV min and max limits as array of Scalars
        cv::Scalar hsvThreshold[2];

		// image containing result of processing
		cv::Mat resultImg;

		// image containing converted color space
		cv::Mat converted;


	public:
		//empty Constructor
		SkinDetector()
		{
			// initialize default parameters
			hsvThreshold[0][0] = 0;
			hsvThreshold[0][1] = 0;
			hsvThreshold[0][2] = 0;
			hsvThreshold[1][0] = 180;
			hsvThreshold[1][1] = 255;
			hsvThreshold[1][2] = 255;
		}

        void setThreshold(cv::Scalar min, cv::Scalar max)
		{
			hsvThreshold[0] = min;
			hsvThreshold[1] = max;
			//std::cout << "min: " << min << "\t" << "max" << max << "\n";
		}

		void getThreshold(cv::Scalar &min, cv::Scalar &max)
		{
			min = hsvThreshold[0];
			max = hsvThreshold[1];
		}

		// Processes an already HSV image. Returns a 1-channel binary image.
		cv::Mat processHSV(const cv::Mat &image);
};

#endif
