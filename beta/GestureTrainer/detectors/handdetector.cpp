/*
	Modified from ColorDetector from chapter 3 of the cookbook:
	Computer Vision Programming using the OpenCV Library.
	by Robert Laganiere, Packt Publishing, 2011.

	Created by: Jason Carlisle Mann (on2valhalla | jcm2207@columbia.edu)


*/

#include "handdetector.h"



cv::Mat HandDetector::findHand(const cv::Mat &colorImg, const cv::Mat &blobImg)
{
	resultImg.create( colorImg.rows, colorImg.cols, colorImg.type() );


	//------------------Find Faces----------------
	//preprocess for face recognition
	char rszFactor = 4;
	std::vector< cv::Rect > faces;
	cv::Mat gray;
	//shrink the image for speed
	cv::resize(colorImg, gray, cv::Size2i(colorImg.cols/rszFactor, 
											colorImg.rows/rszFactor));
	cv::cvtColor(gray, gray, CV_BGR2GRAY);
	cv::equalizeHist(gray, gray);
	cascadeFace.detectMultiScale(gray, faces);

	// draw bounds for faces
    for (unsigned int i = 0; i < faces.size(); i++ )
	{
		//resize the rectangle to match the img
		faces[i] += cv::Point(faces[i].x * rszFactor,faces[i].y * rszFactor);
		// multiplication is not implemented for sizes...
		faces[i] += faces[i].size() +faces[i].size() 
					+faces[i].size() +faces[i].size();
		rectangle(resultImg, faces[i], FACE_COLOR, rszFactor);
	}
	//----------------End Faces--------------------


	//------------------Find Contours----------------
	// find contours in the blob image
	std::vector< std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::findContours(blobImg,
					contours, // a vector of contours
					hierarchy, // a hierarchy of contours if there are parent
								//child relations in the image
					CV_RETR_EXTERNAL, // retrieve the external contours
					CV_CHAIN_APPROX_TC89_L1); // an approximation algorithm
	//----------------END Contours------------------



	//------------------Find Largest Hand----------------
    int maxMass = 0;
    Hand maxHand = Hand();

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
		Hand curHand(contours[idx]);
		int curMass = curHand.getMoments().m00;
		if(curMass > maxMass)
		{
			maxMass = curMass;
			maxHand = curHand;
		}
	}

	lastHand = maxHand;
	lastHand.drawHand(resultImg);
	//------------------END Find Hand--------------------


	return resultImg;
}
