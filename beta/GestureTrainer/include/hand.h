/*

	Created by: Jason Carlisle Mann (on2valhalla | jcm2207@columbia.edu)

	Creates a Hand, stores information on its geometry, and classifies
	it as a certain type of gesture based on hardcoded geometric data

*/


#ifndef HAND_H
#define HAND_H

#include <QDebug>
#include <QString>

#include <opencv2/core/core.hpp>

#include <iostream>
#include <string>
#include <algorithm>

#include "../include/user.h"

	
// COLORS
static cv::Scalar HALF_GREY = cv::Scalar(75,75,75),
				GREY = cv::Scalar(125,125,125),
				BLACK = cv::Scalar(0,0,0),
				WHITE = cv::Scalar(255,255,255),
				HALF_BLUE = cv::Scalar(125,0,0),
				HALF_RED = cv::Scalar(0,0,125),
				HALF_GREEN = cv::Scalar(0,125,0);


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
	cv::Moments mom;
	cv::Point2f palmCenter;
	float palmRadius;
	cv::Rect handOnly;
	// cv::RotatedRect palmEllipse;

	int numFingers;
	std::vector<std::vector<cv::Point > > fingerContours;
	std::vector<cv::RotatedRect> fingerShapes;


	static const unsigned int MAX_DEFECTS = 8,
                            MAX_FINGER_SIZE = 3000,
							MIN_FINGER_SIZE = 700;
	const double MIN_DEFECT_SIZE = 10.0;



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
		numFingers = h.numFingers;
		fingerContours = h.fingerContours;
		fingerShapes = h.fingerShapes;
		handOnly = h.handOnly;
		// palmEllipse = h.palmEllipse;
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
			numFingers = rhs.numFingers;
			fingerContours = rhs.fingerContours;
			fingerShapes = rhs.fingerShapes;
			handOnly = rhs.handOnly;
			// palmEllipse = rhs.palmEllipse;
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


//	END Modifiers/Accessors
//##############################################################################


//##############################################################################
//	Calculation Functions

	void calcTraits()
	{
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
		defects.clear();
		std::vector<cv::Vec4i> tmpDefects;
		cv::convexityDefects(contour[0], hullIdxs[0], tmpDefects);
		// cv::convexityDefects(contour[0], hullIdxs[0], defects);
		std::vector<cv::Point> palmPoints;

		if(tmpDefects.size() <= 0)
			return;
		/* Average depth points to get hand center */
		// int x = 0, y = 0;
		for (cv::Vec4i defect : tmpDefects)
		{
			if(defect[3]/256.0 < MIN_DEFECT_SIZE)
				// || defects.size() > MAX_DEFECTS)
				continue;
			// x += contour[0][defect[2]].x;
			// y += contour[0][defect[2]].y;
			defects.push_back(defect);
			palmPoints.push_back(contour[0][defect[2]]);
		}

		palmCenter = cv::Point2f(0,0);
		palmRadius = 0;

		if(palmPoints.size() <= 0)
			return;

		cv::minEnclosingCircle(palmPoints, palmCenter, palmRadius);
	}

	cv::Mat findFingers(const cv::Mat binaryImg)
	{
		if(type == NONE || palmRadius == 0 || 
			boxRect.height <= 0 || !binaryImg.data)
			return binaryImg;
		
		cv::Mat handROI = binaryImg(boxRect).clone();

		cv::Point2f palmCenterROI(palmCenter.x - boxRect.tl().x,
							palmCenter.y - boxRect.tl().y);


		// fill in palm and below to cut off fingers
		cv::circle( handROI, palmCenterROI, palmRadius, BLACK, CV_FILLED);
		// eliminate remnants of wrist
		cv::rectangle( handROI,
			cv::Point(palmCenterROI.x - palmRadius, palmCenterROI.y),
			cv::Point(palmCenterROI.x + palmRadius, handROI.rows),
			BLACK, CV_FILLED);
		cv::rectangle( handROI,
			cv::Point(0, palmCenterROI.y + palmRadius/2),
			cv::Point(handROI.cols, handROI.rows),
			BLACK, CV_FILLED);

		// Re-find contours
		cv::Mat cloneROI = handROI.clone();
		std::vector<std::vector<cv::Point> > tmpContours;
		cv::findContours(cloneROI, tmpContours, CV_RETR_EXTERNAL,
						CV_CHAIN_APPROX_TC89_L1);

		// Create hand mask drawing
		cv::Mat handDrawing = cv::Mat::zeros(handROI.size(), CV_8UC3);
		cv::circle( handDrawing, palmCenterROI, palmRadius, HALF_GREY, CV_FILLED);

		fingerContours.clear();
		fingerShapes.clear();
		for(unsigned int i = 0; i < tmpContours.size(); i++)
		{
            unsigned int area = cv::contourArea(tmpContours[i]);
			QString str = QString("%1").arg(area);
			putText(handROI, str.toStdString(), tmpContours[i][0],
				cv::FONT_HERSHEY_COMPLEX_SMALL, 1, GREY);
			if(area > MIN_FINGER_SIZE && area < MAX_FINGER_SIZE)
			{
				fingerContours.push_back(tmpContours[i]);
				cv::RotatedRect tmpEllipse = cv::fitEllipse(tmpContours[i]);
				fingerShapes.push_back(tmpEllipse);
				cv::ellipse(handDrawing, tmpEllipse, HALF_BLUE, CV_FILLED);
			}
		}

		QString str = QString("%1")
							.arg(fingerContours.size());
		putText(handROI, str.toStdString(), cv::Point(20,20),
			cv::FONT_HERSHEY_COMPLEX_SMALL, 1, GREY);


		return handDrawing;

	}

	// /*
	// 	Shrinks the box rectangle to exclude the wrist

	// 	requires calcTraits to run first
	// */
	// bool eliminateWrist(const cv::Mat binaryImg)
	// {
	// 	if(type == NONE || palmRadius == 0 || 
	// 		boxRect.height <= 0 || !binaryImg.data)
	// 		return false;

	// 	qDebug() <<boxRect.x << " " << boxRect.y << " " << boxRect.height
	// 			<< " " << boxRect.width;

	// 	int newHeight = (palmCenter.y - boxRect.tl().y + palmRadius);
	// 	if(newHeight > boxRect.height)
	// 		newHeight = boxRect.height;
	// 	boxRect = cv::Rect(boxRect.tl().x, boxRect.tl().y, 
	// 						boxRect.width, newHeight);

	// 	qDebug() <<"\t"<<boxRect.x << " " << boxRect.y << " " << boxRect.height
	// 			<< " " << boxRect.width;
		
	// 	cv::Mat handROI(binaryImg, boxRect);
	// 	if(!handROI.data)
	// 		return false;

	// 	cv::Mat cloneROI = handROI.clone();
	// 	std::vector<std::vector<cv::Point> > tmpContours;
	// 	cv::findContours(cloneROI, tmpContours, CV_RETR_EXTERNAL,
	// 					CV_CHAIN_APPROX_TC89_L1);
	// 	//------------------Find Largest Contour----------------
	// 	int maxMass = 0;
	// 	std::vector<cv::Point> maxContour;

	// 	// iterate through all the top-level contours
	// 	for(unsigned int i = 0; i < tmpContours.size(); i++)
	// 	{
	// 		// Find the largest contour
	// 		int curMass = cv::contourArea( tmpContours[i] );
	// 		if(curMass > maxMass)
	// 		{
	// 			maxMass = curMass;
	// 			maxContour = tmpContours[i];
	// 		}
	// 	}	

	// 	if(maxMass == 0)
	// 		return false;

	// 	//adjust the max contour back to the entire frame
	// 	for(unsigned int i = 0; i < maxContour.size(); i++)
	// 	{
	// 		maxContour[i] += boxRect.tl();
	// 	}


	// 	contour.clear();
	// 	contour.push_back(maxContour);

	// 	calcTraits();

	// 	return true;
	// }

//	END Calculation
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
		bRatio = static_cast<double>(boxRect.width)/boxRect.height;
		// std::cout << bRatio << std::endl;
		mRatio = (static_cast<double>(boxRect.width)*boxRect.height)/mom.m00;

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
			// draw defect begin, end, and farthest point
			// cv::circle( image, contour[0][defect[0]], 5, HALF_BLUE, -1 );
			// cv::circle( image, contour[0][defect[1]], 5, HALF_GREEN, -1 );
			cv::circle( image, contour[0][defect[2]], 5, HALF_GREEN, -1 );
			// midpoint between start and end
			// cv::Point midPoint( 
			// 	(contour[0][defect[0]].x + contour[0][defect[1]].x) / 2,
			// 	(contour[0][defect[0]].y + contour[0][defect[1]].y) / 2);

			// line from hull to farthest point
			// line( image, midPoint, contour[0][defect[2]],
			// 		HALF_BLUE, 3, CV_AA );
		}

        if(palmCenter.x && palmCenter.y && palmRadius)
			cv::circle( image, palmCenter, palmRadius, HALF_BLUE, 3 );

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
		QString data = QString(
					"Type: %9"
					"\nB Width: %1  B Height: %2"
					"\nR Width: %3  R Height: %4"
					"\nB Ratio: %5  M Ratio : %6"
					"\nDefects: %7  Palm Rad: %8\n")
					.arg(boxRect.width)
					.arg(boxRect.height)
					.arg(pointDist(rotPoints[1], rotPoints[0]), 3, 'g')
					.arg(pointDist(rotPoints[1], rotPoints[2]), 3, 'g')
					.arg(bRatio, 3, 'g')
					.arg(mRatio, 3, 'g')
					.arg(defects.size())
					.arg(palmRadius)
					.arg(getType());

		for(cv::Vec4i defect : defects)
		{
			data.append(QString("\nDefect len: %1")
							.arg(defect[3]/256.0, 4, 'g'));
		}
		return data;
	}

//	END Utility Functions
//##############################################################################



};

#endif
