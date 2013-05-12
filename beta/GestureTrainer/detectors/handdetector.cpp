/*
	Modified from ColorDetector from chapter 3 of the cookbook:
	Computer Vision Programming using the OpenCV Library.
	by Robert Laganiere, Packt Publishing, 2011.

	Created by: Jason Carlisle Mann (on2valhalla | jcm2207@columbia.edu)


*/

#include "handdetector.h"



cv::Mat HandDetector::findHand(const cv::Mat colorImg, const cv::Mat binImg)
{
	if (binImg.empty() || colorImg.empty())
	  return cv::Mat();
	resultImg = colorImg.clone();
	cv::Mat binaryImg = binImg.clone();


	//------------------Find Faces----------------
	//preprocess for face recognition
	std::vector< cv::Rect > faces;
	cv::Mat gray;
	//shrink the image for speed
	cv::resize(resultImg, gray, cv::Size2i(resultImg.cols/4, 
											resultImg.rows/4));
	cv::cvtColor(gray, gray, CV_BGR2GRAY);
	cv::equalizeHist(gray, gray);
	cascadeFace.detectMultiScale(gray, faces);

	// draw bounds for faces
	for (unsigned int i = 0; i < faces.size(); i++ )
	{
		//resize the rectangle to match the img
		faces[i] += cv::Point(faces[i].x * 3,faces[i].y * 3);
		// multiplication is not implemented for sizes...
		faces[i].width *= 4;
		faces[i].height *= 5;
		rectangle(resultImg, faces[i], FACE_COLOR, 3);
	}
	//----------------End Faces--------------------


	//------------------Find Contours----------------
	// find contours in the blob image
	std::vector< std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::findContours(binaryImg,
					contours, // a vector of contours
					hierarchy, // a hierarchy of contours if there are parent
								//child relations in the image
					CV_RETR_EXTERNAL, // retrieve the external contours
					CV_CHAIN_APPROX_TC89_L1); // an approximation algorithm
	//----------------END Contours------------------

	if(contours.size() <= 0)
		return resultImg;

	//------------------Find Largest Contour----------------
	int maxMass = 0;
	std::vector<cv::Point> maxContour;

	// iterate through all the top-level contours
	int idx = 0;
	for( ; idx >= 0; idx = hierarchy[idx][0] )
	{
		// find if current contour intersects with a face
		bool faceOverlap = false;
		for(unsigned int i = 0; i < faces.size(); i++)
		{
			for(cv::Point p : contours[idx])
			{
				if(faces[i].contains(p))
					faceOverlap = true;
			}
		}

		// skip the contour if it intersects with the face
		if(faceOverlap)
			continue;

		// Find the largest contour
		int curMass = cv::contourArea( contours[idx] );
		if(curMass > MIN_HAND_SIZE && curMass > maxMass)
		{
			maxMass = curMass;
			maxContour = contours[idx];
		}
	}
	
	if(!maxContour.empty())
		approxPolyDP(maxContour, maxContour, 2, true);

	lastHand = Hand(maxContour);
	lastHand.drawHand(resultImg);

	cv::namedWindow("fingers");
	cv::Mat fingers = lastHand.findFingers(binImg);
	cv::imshow("fingers", fingers);
	//------------------END Find Hand--------------------


	return resultImg;
}
