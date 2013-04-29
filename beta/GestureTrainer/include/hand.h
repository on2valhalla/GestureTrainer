/*

	Created by: Jason Carlisle Mann (on2valhalla | jcm2207@columbia.edu)

	Creates a Hand, stores information on its geometry, and classifies
	it as a certain type of gesture based on hardcoded geometric data

*/


#ifndef HAND_H
#define HAND_H

#include <opencv2/core/core.hpp>

#include <iostream>
#include <string>
// #include <boost/lexical_cast.hpp>
#include <algorithm>


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

	// contour and hull are 2d vectors for easy drawing
	std::vector<std::vector< cv::Point > > contour;
	std::vector<std::vector< cv::Point > > hull;
	cv::vector<cv::Vec4i> defects;
	cv::RotatedRect rotRect;
	cv::Point2f rotPoints[4];
	cv::Rect boxRect;
	cv::Moments mom;

	// COLORS
	cv::Scalar COLOR_CONTOUR,
				COLOR_HULL,
				COLOR_ROT_RECT,
				COLOR_BD_RECT;


public:
//##############################################################################
//	Constructors / Destructor

	//Default constructor
	Hand()
	{
		type = NONE;
		contour.push_back(std::vector<cv::Point>());
		hull.push_back(std::vector<cv::Point>());
	}

	//Constructor
	Hand(std::vector<cv::Point> c)
	{
		COLOR_CONTOUR = cv::Scalar(150,150,150);
		COLOR_HULL = cv::Scalar(150,150,150);
		COLOR_ROT_RECT = cv::Scalar(124,0,0);
		COLOR_BD_RECT = cv::Scalar(0,124,0);

		type = UNK;

		contour.push_back(c);

		// min fit rectangle (rotated)
		rotRect = cv::minAreaRect( cv::Mat(contour[0]) );
		rotRect.points(rotPoints);

		// bounding rectangle of the hand
		boxRect = boundingRect(contour[0]);

		//calculate the moments of the hand
		mom = cv::moments(cv::Mat(contour[0]));

		// convex hull and defects
		std::vector<std::vector<int> > hullIdxs;
		hullIdxs.push_back(std::vector<int>());
		cv::convexHull(cv::Mat(contour[0]), hullIdxs[0]);

		// gather the hull points into a vector for drawing
		hull = std::vector<std::vector<cv::Point> >(1);
		for(int i : hullIdxs[0])
			hull[0].push_back(contour[0][i]);

		// std::cout << hull[0].size() << std::endl;

		// defects
		cv::convexityDefects(contour[0], hullIdxs[0], defects);


	}

	//copy constructor
	Hand(const Hand& h)
	{
		contour.push_back(std::vector<cv::Point>());
		for(cv::Point pt : h.contour[0])
			contour[0].push_back(pt);
		hull.push_back(std::vector<cv::Point>());
		for(cv::Point pt : h.hull[0])
			hull[0].push_back(pt);
		// std::cout << "copy: " << hull[0].size() << std::endl;

		type = h.type;

		rotRect = h.rotRect;
		for(unsigned int i = 0; i < sizeof(rotPoints); i++)
			rotPoints[i] = h.rotPoints[i];

		boxRect = h.boxRect;

		mom = h.mom;

		defects = h.defects;
	}

	//assignment operator
	Hand& operator=(const Hand& rhs)
	{
		if(this == &rhs)
			return *this;

		contour.clear();
		contour.push_back(std::vector<cv::Point>());
		if(rhs.contour.size() > 0)
			for(const cv::Point& pt : rhs.contour[0])
				contour[0].push_back(pt);
		hull.clear();
		hull.push_back(std::vector<cv::Point>());
		if(rhs.hull.size() > 0)
			for(const cv::Point& pt : rhs.hull[0])
				hull[0].push_back(pt);
		// std::cout << "copy: " <<hull[0].size() << std::endl;

		type = rhs.type;

		rotRect = rhs.rotRect;
		for(unsigned int i = 0; i < sizeof(rotPoints); i++)
			rotPoints[i] = rhs.rotPoints[i];

		boxRect = rhs.boxRect;

		mom = rhs.mom;

		defects = rhs.defects;


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
	const cv::Moments& getMoments()
	{
		return mom;
	}

	const cv::Rect& getBoundRect()
	{
		return boxRect;
	}

	bool isNone()
	{
		return type == NONE;
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
	void drawHand(cv::Mat image)
	{
		// No hand, don't draw
		if(type == NONE)
			return;

		// draw onto separate Mat for highlighter effect when added
		cv::Mat contourImg(image.size(), image.type(), cv::Scalar(0));
		cv::drawContours( contourImg, contour, 0, 
							COLOR_CONTOUR, CV_FILLED, CV_AA);
		cv::GaussianBlur( contourImg, contourImg, cv::Size(3,3), 0);
		image += contourImg;

		// Draw convex hull
		cv::drawContours( image, hull, 0, COLOR_HULL, 5, CV_AA);

		// Draw defects
		for(cv::Vec4i defect : defects)
		{
			// Skip the smaller defects
			if(defect[3]/256.0 < 30.0)
				continue;
			cv::circle( image, contour[0][defect[0]], 5, cv::Scalar(150,0,0), -1 );
			cv::circle( image, contour[0][defect[1]], 5, cv::Scalar(0,150,0), -1 );
			cv::circle( image, contour[0][defect[2]], 5, cv::Scalar(0,0,150), -1 );
			cv::Point midPoint( 
				(contour[0][defect[0]].x + contour[0][defect[1]].x) / 2,
				(contour[0][defect[0]].y + contour[0][defect[1]].y) / 2);

			line( image, midPoint, contour[0][defect[2]],
					COLOR_ROT_RECT, 3, CV_AA );
		}


		// // draw bounding rotated rectangles
		// for( int j = 0; j < 4; j++ )
		// 	line( image, rotPoints[j], rotPoints[(j+1)%4],
		// 			COLOR_ROT_RECT, 3, CV_AA );

		// rectangle(image, boxRect, COLOR_BD_RECT, 3);

		// putText(image, boost::lexical_cast<std::string>(mom.m00), boxRect.br(),
			// cv::FONT_HERSHEY_COMPLEX_SMALL, 1, cv::Scalar(200,200,250));


	}


//	END Utility Functions
//##############################################################################



};

#endif
