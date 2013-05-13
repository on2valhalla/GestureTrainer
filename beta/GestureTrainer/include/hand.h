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
#include <fstream>
#include <string>
#include <algorithm>
#include <cmath>

#include "../include/user.h"

	
// COLORS
static cv::Scalar HALF_GREY = cv::Scalar(75,75,75),
				GREY = cv::Scalar(125,125,125),
				BLACK = cv::Scalar(0,0,0),
				WHITE = cv::Scalar(255,255,255),
				HALF_BLUE = cv::Scalar(125,0,0),
				HALF_RED = cv::Scalar(0,0,125),
				HALF_GREEN = cv::Scalar(0,125,0);


enum FingerType {
	THUMB,
	INDEX,
	MIDDLE,
	RING,
	PINKY,
	FINUNK
};

struct Finger {
	FingerType type = FINUNK;
	cv::RotatedRect ellipse;
	std::vector<cv::Point> contour;
	cv::Point tip;
	double angle;
};


enum HandType{

	//FILL IN MORE TYPES AS WE FIGURE THEM OUT
	UNK,
	FIST,
	T,
	A,
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
	cv::Point2f palmCenter;
	// cv::Point palmCenter;
	float palmRadius;
	cv::Rect handOnly;
	// cv::RotatedRect palmEllipse;
	double palmArea;
  

	std::vector<Finger> fingers;


	static const unsigned int MAX_DEFECTS = 8,
							MAX_FINGER_SIZE = 3000,
							MIN_FINGER_SIZE = 700;
	double MIN_DEFECT_SIZE = 10.0,
			MIN_FINGER_RATIO = 6.0,
			MAX_FINGER_RATIO = 30.0;



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
			findClass();
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
		handOnly = h.handOnly;
		// palmEllipse = h.palmEllipse;
		palmArea = h.palmArea;
		phRatio = h.phRatio;
		fingers = h.fingers;

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
			handOnly = rhs.handOnly;
			// palmEllipse = rhs.palmEllipse;
			palmArea = rhs.palmArea;
			phRatio = rhs.phRatio;
			fingers = rhs.fingers;
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
			case T:
				return QString("T");
			case A:
				return QString("A");
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
		if(contour[0].empty())
			return;
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
		std::vector<cv::Point> palmPoints;

		if(tmpDefects.size() <= 0)
			return;
		/* Average depth points to get hand center */
		// int x =0, y=0;
		for (cv::Vec4i defect : tmpDefects)
		{
			if(defect[3]/256.0 < MIN_DEFECT_SIZE)
				continue;

			// x += contour[0][defect[2]].x;
			// y += contour[0][defect[2]].y;

			defects.push_back(defect);
			palmPoints.push_back(contour[0][defect[2]]);
		}


		// Calculate palm center and enclosing circle
		palmCenter = cv::Point(0,0);
		palmRadius = 0;

		// if(defects.size() <= 0)
		// 	return;

		// palmCenter.x = x/defects.size();
		// palmCenter.y = y/defects.size();

		// /* Compute hand radius as mean of distances of
		//    defects' depth point to hand center */
		// int dist = 0;
		// for(cv::Vec4i defect : defects)
		// {
		// 	dist += pointDist(palmCenter, contour[0][defect[2]]);
		// }

		// palmRadius = ((double)dist) / defects.size();

		if(palmPoints.size() <= 0)
			return;

		cv::minEnclosingCircle(palmPoints, palmCenter, palmRadius);
		// adjust the palm to be smaller/larger if necessary
		// palmRadius *= .9;
		palmArea = PI * (palmRadius * palmRadius);
	}


	cv::Mat extractFingers(cv::Mat handROI)
	{

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


		// Re-find contours (whats left are fingers)
		cv::Mat cloneROI = handROI.clone();
		std::vector<std::vector<cv::Point> > tmpContours;
		cv::findContours(cloneROI, tmpContours, CV_RETR_EXTERNAL,
						CV_CHAIN_APPROX_TC89_L1);

		fingers.clear();
		// qDebug() << "fingers: ";
		for(unsigned int i = 0; i < tmpContours.size(); i++)
		{
			unsigned int area = cv::contourArea(tmpContours[i]);
			double Aratio = (double)area/palmRadius;
			// qDebug() << "\t" << (double)area/palmRadius;

			// if(area > MIN_FINGER_SIZE && area < MAX_FINGER_SIZE)
			if(Aratio > MIN_FINGER_RATIO && Aratio < MAX_FINGER_RATIO)
			{
				// qDebug()<< (double)area/palmRadius;
				Finger tmpFing;
				tmpFing.contour = tmpContours[i];
				fingers.push_back(tmpFing);
			}
		}

		return handROI;
	}

	void fingerTraits()
	{
		cv::Point2f tmpPoint;
		for (unsigned int i = 0; i < fingers.size(); i++)
		{
			double maxDist = 0;
			for (unsigned int j = 0; j < fingers[i].contour.size(); ++j)
			{
				tmpPoint = fingers[i].contour[j] + boxRect.tl();
				double curDist = pointDist(palmCenter, tmpPoint);

				if(curDist > maxDist)
				{
					maxDist = curDist;
					fingers[i].tip = fingers[i].contour[j];
				}
			}

			fingers[i].ellipse = cv::fitEllipse(fingers[i].contour);
			tmpPoint = fingers[i].tip + boxRect.tl();
			fingers[i].angle = angleOfPoints(palmCenter, tmpPoint);
		}
	}


	cv::Mat findFingers()
	{
		// Create a blank image to draw in
		cv::Mat binaryImg = 
				cv::Mat::zeros(boxRect.br().y, boxRect.br().x, CV_8UC1);
		if(type == NONE || palmRadius == 0 || 
			boxRect.height <= 0)
			return binaryImg;

		// draw the contour
		cv::drawContours(binaryImg, contour, 0, WHITE, CV_FILLED);

		// get the points around the palm
		std::vector<cv::Point> palmPoints;
		cv::Point2f newCenter;
		float newRadius;
		for (cv::Vec4i defect : defects)
		{
			palmPoints.push_back(contour[0][defect[2]]);
		}
		cv::minEnclosingCircle(palmPoints, newCenter, newRadius);
		
		cv::Mat handROI = binaryImg(boxRect);

		extractFingers(handROI);

		// if fewer than 4 fingers are detected, run again with a larger
		// radius and check result
		if(fingers.size() < 4)
		{
			// qDebug() << "Give it another shot";
			// backup
			std::vector<Finger> oldfing = fingers;
			cv::vector<cv::Vec4i> olddef = defects;


			// adjust vars
			palmRadius *= 1.1;


			// convex hull and defects
			std::vector<std::vector<int> > hullIdxs;
			hullIdxs.push_back(std::vector<int>());
			cv::convexHull( contour[0], hullIdxs[0]);

			defects.clear();
			cv::convexityDefects(contour[0], hullIdxs[0], defects);
			extractFingers(handROI);
			if(fingers.size() <= oldfing.size() && fingers.size() > 0)
			{
				fingers = oldfing;
			}

			// restore defects and radius
			defects = olddef;
			palmRadius /= 1.1;
		}

		// get traits for fingers after finalized
		fingerTraits();

		return handROI;
	}

	/*
		requires calcTraits to run first
	*/
	bool eliminateWrist(const cv::Mat binaryImg)
	{
		if(type == NONE || palmRadius == 0 || 
			boxRect.height <= 0 || !binaryImg.data)
			return false;

		// qDebug() <<boxRect.x << " " << boxRect.y << " " << boxRect.height
		// 		<< " " << boxRect.width;

		int newHeight = (palmCenter.y - boxRect.tl().y + palmRadius);
		if(newHeight > boxRect.height)
			newHeight = boxRect.height;
		handOnly = cv::Rect(boxRect.tl().x, boxRect.tl().y, 
							boxRect.width, newHeight);

		// qDebug() <<"\t"<<boxRect.x << " " << boxRect.y << " " << boxRect.height
		// 		<< " " << boxRect.width;
		

		return true;
	}

	

	void findClass()
	{
		// double handMass = mom.m00;

		// phRatio = palmArea/handMass;
		// if(phRatio > 0.70)
		// 	type = FIST;
		// else
		// 	type = PALM;

		if(fingers.size() >= 1)
		{
			type = PALM;
			MIN_DEFECT_SIZE = 10.0;
		}
		else
		{
			MIN_DEFECT_SIZE = 0.0;
			calcTraits();
			type = FIST;
		}
		// qDebug() << getType();
	}


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

	/**
	 * Determines the angle of a straight line drawn between point one and two. 
	 The number returned, which is a float in degrees, tells us how much we 
	 have to rotate a horizontal line clockwise for it to match the line 
	 between the two points.If you prefer to deal with angles using radians 
	 instead of degrees, just change the last line to: 
	 "return Math.Atan2(yDiff, xDiff);"
	 */
	float angleOfPoints(cv::Point p1, cv::Point p2)
	{
		float xDiff = p2.x - p1.x;
		float yDiff = p2.y - p1.y;
		return atan2(yDiff, xDiff);
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
			// if(defect[3]/256.0 < MIN_DEFECT_SIZE)
			// 	continue;
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

		///QString str = QString("%1").arg(mom.m00);

		for(unsigned int i = 0; i < contour[0].size(); i=i+10)
		{
			QString str =  QString::number(i);
			putText(image, str.toStdString(), contour[0][i],
					cv::FONT_HERSHEY_COMPLEX_SMALL, 1, cv::Scalar(200,200,250));
		}

		displayType(image);
			

	}

	void displayType(cv::Mat image)
	{
		putText(image, getType().toStdString(), cv::Point(5,5), 
			cv::FONT_HERSHEY_COMPLEX_SMALL, 5, cv::Scalar(200,200,250));
	}


	QString getData()
	{
		QString data = QString(
					"Type: %10"
					"\nNumFingers: %12"
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
					.arg(phRatio)
					.arg(fingers.size());

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
