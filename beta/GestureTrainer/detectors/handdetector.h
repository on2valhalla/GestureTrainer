/*
	Modified from ColorDetector from chapter 3 of the cookbook:
	Computer Vision Programming using the OpenCV Library.
	by Robert Laganiere, Packt Publishing, 2011.

	Created by: Jason Carlisle Mann (on2valhalla | jcm2207@columbia.edu)

	
*/

#if !defined HANDDETECT_H
#define HANDDETECT_H

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>

#include <iostream>
#include <string>

#include "../include/hand.h"

// Haar Cascade Classifier face file location
static std::string FACEFILE = 
	"/opt/local/share/OpenCV/haarcascades/haarcascade_frontalface_alt_tree.xml";


class HandDetector
{

private:
	// image containing result of processing
	// with overlayed hand and face rectangles
	cv::Mat resultImg;

	// Last resulting Hand
	Hand lastHand;

	// HAAR Cascade for detecting faces
	cv::CascadeClassifier cascadeFace;

	// Constants
	cv::Scalar FACE_COLOR = cv::Scalar(0,0,204),
				HAND_COLOR = cv::Scalar(100,150,255);


public:
	//empty Constructor
	HandDetector()
	{
		cascadeFace = cv::CascadeClassifier(FACEFILE);
	}

	const Hand getLastHand() const
	{
		return lastHand;
	}

	// Uses a binary image of blobs to find a hand and then overlays
	// rectangles on the face and largest hand
	cv::Mat findHand(const cv::Mat &colorImg, const cv::Mat &blobImg);
};

#endif
