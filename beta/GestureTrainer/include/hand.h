/*

	Created by: Jason Carlisle Mann (on2valhalla | jcm2207@columbia.edu)

	Creates a Hand, stores information on its geometry, and classifies
	it as a certain type of gesture based on hardcoded geometric data

*/


#ifndef HAND_H
#define HAND_H

#include <opencv2/core/core.hpp>


enum HandType{

	//FILL IN MORE TYPES AS WE FIGURE THEM OUT
	UNK,
	NONE
};

class Hand
{

private:
	// VARIABLES
	HandType type;
	// contour is stored as a vector for easy drawing (drawContours typically
	// pulls from the entire array of found contours)
	std::vector<std::vector<cv::Point> > contours;
	cv::RotatedRect rotRect;
	cv::Point2f rotPoints[4];
	cv::Rect boxRect;
	cv::Moments mom;

	// CONSTANTS
	cv::Scalar COLOR_CONTOUR = cv::Scalar(0,0,124),
				COLOR_ROT_RECT = cv::Scalar(124,0,0),
				COLOR_BD_RECT = cv::Scalar(0,124,0);


public:
//##############################################################################
//	Constructors / Destructor

	//Default constructor
	Hand()
	{
		type = NONE;
	}

	//Constructor
	Hand(std::vector<cv::Point> c)
	{
		contours.clear();
		contours.push_back(c);

		//calculate the rotated and bounding rectangles of the blobs
		rotRect = cv::minAreaRect( cv::Mat(contours[0]) );
		rotRect.points(rotPoints);

		boxRect = rotRect.boundingRect();

		//calculate the moments of the hand
		mom = cv::moments(cv::Mat(contours[0]));
	}

	//copy constructor
	Hand(const Hand& h)
	{
		type = h.type;
		rotRect = h.rotRect;
		for(unsigned int i = 0; i < sizeof(rotPoints); i++)
			rotPoints[i] = h.rotPoints[i];
		boxRect = h.boxRect;
		mom = h.mom;
	}

	//assignment operator
	Hand& operator=(const Hand& rhs)
	{
		if(this == &rhs)
			return *this;

		type = rhs.type;
		rotRect = rhs.rotRect;
		for(unsigned int i = 0; i < sizeof(rotPoints); i++)
			rotPoints[i] = rhs.rotPoints[i];
		boxRect = rhs.boxRect;
		mom = rhs.mom;
		return *this;
	}

	//destructor
	~Hand()
	{

	}

//	END Constructors / Destructor
//##############################################################################



//##############################################################################
//	Modifiers/Accessors

	// Retrieve the moments of the Hand
	cv::Moments getMoments()
	{
		return mom;
	}

//	END Modifiers/Accessors
//##############################################################################


//##############################################################################
//	Utility Functions

	// Easy Calculation of Euclidean Distance
	double pointDist(cv::Point2f &p1, cv::Point2f &p2)
	{
		double dx = p1.x - p2.x;
		double dy = p1.y - p2.y;
		return sqrt(dx * dx + dy * dy);
	}

	void findType()
	{
		//Use the statistics to caculate which gesture it is
	}

	// Draws all the relevant hand data (bounding and rotated rects, contour)
	// on a cv::Mat that is provided
	void drawHand(cv::Mat &image)
	{
		// draw connected component contour
		cv::Mat contourImg(image.size(), image.type(), cv::Scalar(0));
		cv::drawContours( contourImg, contours, 0, COLOR_CONTOUR, CV_FILLED, 8);
		cv::GaussianBlur( contourImg, contourImg, cv::Size(3,3), 0);
		image += contourImg;

		// draw bounding rotated rectangles
		for( int j = 0; j < 4; j++ )
			line( image, rotPoints[j], rotPoints[(j+1)%4],
					COLOR_ROT_RECT, 3, 8 );

		rectangle(image, boxRect, COLOR_BD_RECT, 3);
	}

	
//	END Utility Functions
//##############################################################################



};

#endif
