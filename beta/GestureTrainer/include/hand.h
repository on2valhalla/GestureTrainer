/*

	Created by: Jason Carlisle Mann (on2valhalla | jcm2207@columbia.edu)

	Creates a Hand, stores information on its geometry, and classifies
	it as a certain type of gesture based on hardcoded geometric data

*/


#ifndef HAND_H
#define HAND_H

#define PI 3.1415926

#include <QDebug>
#include <QString>

#include <opencv2/core/core.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

#include "../include/user.h"

	


enum HandType{

	//FILL IN MORE TYPES AS WE FIGURE THEM OUT
	UNK,
	FIST,
	PALM,
	FINGERS,
	NONE
};

class Hand
{
	 friend class User;

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
	double bRatio;
	double mRatio;
	double phRatio;

	cv::Moments mom;
	cv::Point palmCenter;
	int palmRadius;
	double palmArea;
  

	// COLORS
	cv::Scalar HALF_GREY = cv::Scalar(75,75,75),
				GREY = cv::Scalar(125,125,125),
				HALF_BLUE = cv::Scalar(125,0,0),
				HALF_RED = cv::Scalar(0,0,125),
				HALF_GREEN = cv::Scalar(0,125,0);

	static const unsigned int MAX_DEFECTS = 8;
	const double MIN_DEFECT_SIZE = 5.0;



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
		if(c.size() <= 0)
			Hand();
		else
		{
			type = UNK;

			contour.push_back(c);

			calcTraits();
			findType();
		}
	}

	//copy constructor
	Hand(const Hand& h)
	{
		type = h.type;
		contour = h.contour;
		hull = h.hull;
		defects = h.defects;
		rotRect = h.rotRect;
		rotRect.points(rotPoints);
		boxRect = h.boxRect;
		bRatio = h.bRatio;
		mRatio = h.mRatio;
		mom = h.mom;
		palmCenter = h.palmCenter;
		palmRadius = h.palmRadius;
		palmArea = h.palmArea;
		phRatio = h.phRatio;

	}

	//assignment operator
	Hand& operator=(const Hand& rhs)
	{
		if(this != &rhs)
		{
			type = rhs.type;
			contour = rhs.contour;
			hull = rhs.hull;
			defects = rhs.defects;
			rotRect = rhs.rotRect;
			rotRect.points(rotPoints);
			boxRect = rhs.boxRect;
			bRatio = rhs.bRatio;
			mRatio = rhs.mRatio;
			mom = rhs.mom;
			palmCenter = rhs.palmCenter;
			palmRadius = rhs.palmRadius;
			palmArea = rhs.palmArea;
			phRatio = rhs.phRatio;
		}
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

	double getB() const
	{
		return bRatio;
	}

	double getM() const
	{
		return mRatio;
	}

	QString getType() const
	{
		switch(type)
		{
			case UNK:
				return QString("UNK");
			case FIST:
				return QString("FIST");
			case PALM:
				return QString("PALM");
			case FINGERS:
				return QString("FINGERS");
			default:
				return QString("NONE");
		}
	}

	bool isNone() const
	{
		return type == NONE;
	}

	void calcTraits()
	{
		palmCenter = cv::Point(0,0);
		palmRadius = 0;


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
		defects.empty();
		std::vector<cv::Vec4i> tmpDefects;
		cv::convexityDefects(contour[0], hullIdxs[0], tmpDefects);
		// cv::convexityDefects(contour[0], hullIdxs[0], defects);

		if(tmpDefects.size() <= 0)
			return;
		/* Average depth points to get hand center */
		int x = 0, y = 0;
		for (cv::Vec4i defect : tmpDefects)
		{
			if(defect[3]/256.0 < MIN_DEFECT_SIZE)
				// || defects.size() > MAX_DEFECTS)
				continue;
			x += contour[0][defect[2]].x;
			y += contour[0][defect[2]].y;
			defects.push_back(defect);
		}

		if(defects.size() <= 0)
			return;

		palmCenter.x = x/defects.size();
		palmCenter.y = y/defects.size();

		/* Compute hand radius as mean of distances of
		   defects' depth point to hand center */
		int dist = 0;
		for(cv::Vec4i defect : defects)
		{
			dist += pointDist(palmCenter, contour[0][defect[2]]);
		}

		palmRadius = ((double)dist) / defects.size();
		palmArea = PI * (palmRadius * palmRadius);

		bRatio = static_cast<double>(boxRect.width)/boxRect.height;
		mRatio = (static_cast<double>(boxRect.width)*boxRect.height)/mom.m00;

		std::cout << "In Hand: " << palmArea << std::endl;

		qDebug() << "radius: " << palmRadius;

	}


//	END Modifiers/Accessors
//##############################################################################


//##############################################################################
//	Utility Functions

	// Easy Calculation of Euclidean Distance
	double pointDist(cv::Point &p1, cv::Point &p2)
	{
		double dx = p1.x - p2.x;
		double dy = p1.y - p2.y;
		return sqrt(dx * dx + dy * dy);
	}

	double pointDist(cv::Point2f &p1, cv::Point2f &p2)
	{
		double dx = p1.x - p2.x;
		double dy = p1.y - p2.y;
		return sqrt(dx * dx + dy * dy);
	}

	cv::Point midPoint(const cv::Point &p1, const cv::Point &p2)
	{
		return cv::Point( (p1.x + p2.x) / 2, (p1.y + p2.y) / 2);
	}

	void findType()
	{
        double handMass = mom.m00;

        phRatio = palmArea/handMass;
        if(phRatio > 0.7)
        {
        	type = FIST;
        }
        else
        {
        	type = PALM;
        }

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
							HALF_GREY, CV_FILLED, CV_AA);
		cv::GaussianBlur( contourImg, contourImg, cv::Size(3,3), 0);
		image += contourImg;

		// Draw convex hull
		cv::drawContours( image, hull, 0, GREY, 2, CV_AA);

		// Draw defects
		for(cv::Vec4i defect : defects)
		{
			// Skip the smaller defects
			if(defect[3]/256.0 < MIN_DEFECT_SIZE)
				continue;
			cv::circle( image, contour[0][defect[0]], 5, HALF_BLUE, -1 );
			cv::circle( image, contour[0][defect[1]], 5, HALF_GREEN, -1 );
			cv::circle( image, contour[0][defect[2]], 5, HALF_RED, -1 );
			cv::Point midPoint( 
				(contour[0][defect[0]].x + contour[0][defect[1]].x) / 2,
				(contour[0][defect[0]].y + contour[0][defect[1]].y) / 2);

			line( image, midPoint, contour[0][defect[2]],
					HALF_BLUE, 3, CV_AA );
		}

		if(palmCenter.x > 0 && palmRadius)
			cv::circle( image, palmCenter, palmRadius, HALF_GREEN, 3 );

		// // draw bounding rotated rectangles
		// for( int j = 0; j < 4; j++ )
		// 	line( image, rotPoints[j], rotPoints[(j+1)%4],
		// 			HALF_BLUE, 3, CV_AA );

		// rectangle(image, boxRect, HALF_GREEN, 3);

		// QString str = QString("%1").arg(mom.m00);
		// putText(image, str.toString(), boxRect.br(),
		// 	cv::FONT_HERSHEY_COMPLEX_SMALL, 1, cv::Scalar(200,200,250));

	}


	QString getData()
	{
		std::cout << "SHOULD MATCH HAND: " << palmArea << std::endl;		
		QString data = QString(
					"Type: %10"
					"\nB Width: %1  B Height: %2"
					"\nR Width: %3  R Height: %4"
					"\nB Ratio: %5  M Ratio : %6"
					"\nDefects: %7  \n"
					"Hand Mass: %8   \nPalm Area: %9\n"
					"PHratio: %11\n")
					.arg(boxRect.width)
					.arg(boxRect.height)
					.arg(pointDist(rotPoints[1], rotPoints[0]), 3, 'g')
					.arg(pointDist(rotPoints[1], rotPoints[2]), 3, 'g')
					.arg(bRatio, 3, 'g')
					.arg(mRatio, 3, 'g')
					.arg(defects.size())
					.arg(mom.m00)
					.arg(palmArea)
					.arg(getType())
					.arg(phRatio);

		for(cv::Vec4i defect : defects)
		{
			data.append(QString("\nDefect Lengths: %1")
							.arg(defect[3]/256.0, 4, 'g'));
		}


		return data;
	}


//	END Utility Functions
//##############################################################################



};

#endif
