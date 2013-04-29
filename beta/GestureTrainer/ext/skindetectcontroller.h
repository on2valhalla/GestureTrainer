/*
	Modified from ColorDetector from chapter 3 of the cookbook:
	Computer Vision Programming using the OpenCV Library.
	by Robert Laganiere, Packt Publishing, 2011.

	Created by: Jason Carlisle Mann (on2valhalla | jcm2207@columbia.edu)

	A singleton wrapper class for SkinDetector, to gain continuity
	throughout the program, and also to provide more simple access
	to its methods, and a local cache of the most recent images.
*/

#if !defined SKN_CNTRLLR
#define SKN_CNTRLLR


#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>

#include "../ext/skindetector.h"

class SkinDetectController
{
	private:

		static SkinDetectController *singleton;

		SkinDetector *sknDetect;

		// hsvImage storage
		cv::Mat hsvImage;
		cv::Mat resultImg;

	public:
		SkinDetectController()
		{
            sknDetect = new SkinDetector();
		}

		// Deletes all processor objects created by the controller.
		~SkinDetectController() 
		{

		  delete sknDetect;
		}

		// Singleton static members
		static SkinDetectController *getInstance() 
		{

		  if (singleton == 0)
		    singleton= new SkinDetectController;

		  return singleton;
		}

		// Releases the singleton instance of this controller.
		static void destroy() 
		{
		  if (singleton != 0)
		  {
		      delete singleton;
		      singleton= 0;
		  }
		}
		



		// Sets the input hsvImage. Reads it from file.
		bool setInputImage(std::string filename)
		{

			cv::Mat imgIn = cv::imread(filename);
			// convert color space
			cv::cvtColor(imgIn, hsvImage, CV_BGR2HSV);

			if (!hsvImage.data)
			  return false;
			else
		      return true;
		}

		// Sets the input hsvImage.
		bool setInputImage(cv::Mat imgIn)
		{
			// convert color space
			cv::cvtColor(imgIn, hsvImage, CV_BGR2HSV);

			if (!hsvImage.data)
			  return false;
			else
			  return true;
		}

		cv::Mat getInputImage()
		{
			cv::Mat imgOut;
			cv::cvtColor(hsvImage, imgOut, CV_HSV2BGR);
			return imgOut;
		}

		// Returns the current input hsvImage.
		// NOTE: this returns HSV!!!!!
		const cv::Mat getHSVImage() const 
		{
			return hsvImage;
		}

		const cv::Mat getLastResult() const
		{
			return resultImg;
		}

		void setThreshold(cv::Scalar &min, cv::Scalar &max)
		{
			sknDetect->setThreshold(min, max);
		}

		void setInvert(bool set)
		{
			sknDetect->setInvert(set);
		}

		void setErode(bool set)
		{
			sknDetect->setErode(set);
		}

		void setDilate(bool set)
		{
			sknDetect->setDilate(set);
		}

		void setBlur(bool set)
		{
			sknDetect->setBlur(set);
		}

		void process() 
		{
			resultImg = sknDetect->processHSV(hsvImage);
		}

};

#endif
