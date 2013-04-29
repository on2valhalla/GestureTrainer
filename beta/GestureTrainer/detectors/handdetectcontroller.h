/*
	Modified from ColorDetector from chapter 3 of the cookbook:
	Computer Vision Programming using the OpenCV Library.
	by Robert Laganiere, Packt Publishing, 2011.

	Created by: Jason Carlisle Mann (on2valhalla | jcm2207@columbia.edu)

	A singleton wrapper class for HandDetector, to gain continuity
	throughout the program, and also to provide more simple access
	to its methods, and a local cache of the most recent images.
*/

#if !defined HNDDETECTCNTRL_H
#define HNDDETECTCNTRL_H


#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>

#include "handdetector.h"
#include "../detectors/skindetectcontroller.h"
#include "../include/hand.h"

class HandDetectController
{
	private:

		static HandDetectController *singleton;

		HandDetector *handDetect;

		// Image storage of binary blobs, and original color image
		cv::Mat blobImg;
		cv::Mat colorImg;
        cv::Mat resultImg;

		// Last output ROI containing hand
        Hand lastHand;

	public:
		HandDetectController()
		{
			handDetect = new HandDetector();
		}

		// Deletes all processor objects created by the controller.
		~HandDetectController() 
		{
		  delete handDetect;
		}

		// Singleton static members
		static HandDetectController *getInstance() 
		{

		  if (singleton == 0)
			singleton= new HandDetectController;

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


		// Sets the input blob image.
        bool setInputImages(const cv::Mat &colorImage, const cv::Mat &blobImage)
		{
            blobImg = blobImage.clone();
            colorImg = colorImage.clone();

			if (!blobImg.data || !colorImg.data)
			  return false;
			else
			  return true;
		}

		cv::Mat getBlobImage()
		{
			return blobImg;
		}

		cv::Mat getColorImage()
		{
			return colorImg;
		}

		cv::Mat getResultImage()
		{
			return resultImg;
		}

        const Hand getLastHand() const
		{
			return lastHand;
		}

		void findHand() 
		{
            resultImg = handDetect->findHand(colorImg, blobImg);
			lastHand = handDetect->getLastHand();
		}

};

#endif
