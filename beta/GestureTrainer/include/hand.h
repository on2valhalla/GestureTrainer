/*

	Created by: Jason Carlisle Mann (on2valhalla | jcm2207@columbia.edu)

	Creates a Hand, stores information on its geometry, and classifies
	it as a certain type of gesture based on hardcoded geometric data

*/


#ifndef HAND_H
#define HAND_H

#include "../src/gesturedetector.h"
#include <opencv2/core/core.hpp>


enum HandType{
	FIST,
	PALM,
	POINT,
	UNK,
	NONE
};

class Hand
{

public:
	// VARIABLES
    HandType type;
	cv::RotatedRect rotRect;
	cv::Point2f rotPoints[4];
    cv::Rect boxRect;
	cv::Moments mom;	
	double rRatio;
	double bRatio;
	double mRatio; //mass ratio of bounding versus moments


	// Easy Calculation of Euclidean Distance
	double pointDist(cv::Point2f &p1, cv::Point2f &p2)
	{
		double dx = p1.x - p2.x;
		double dy = p1.y - p2.y;
		return sqrt(dx * dx + dy * dy);
	}

	void setType()
	{
		// based on collected data
		// of my own gestures, see doc/gesture data.xlsx

		//decision for palm gesture
		if 	(mom.m00 > 8500 &&
					bRatio > .5 && bRatio < .95 &&
					((rRatio > .5 && rRatio < .6) ||
					(rRatio > 1.5 && rRatio < 2)))
			type = PALM;
		//fist
		else if(mom.m00 < 10000 &&
					bRatio > .80 && rRatio >.7 &&
			 		mRatio < 2)
			type = FIST;
		//decision for point gesture
		else if(mom.m00 < 10000 &&
					((rRatio > 1.4 && rRatio < 2.2) ||
                    (rRatio > .35 && rRatio < .75)) &&
					bRatio > .4 && bRatio < 1.3)
			type = POINT;
		else  //unknown gesture
			type = UNK;
	}

	QString toQString()
	{
		QString typeStr("");
		switch (type) 
		{
			case FIST:
				typeStr = "FIST";
				break;
			case PALM:
				typeStr = "PALM";
				break;
			case POINT:
				typeStr = "POINT";
				break;
			case UNK:
				typeStr = "UNK";
				break;
			default:
				typeStr = "NONE";
				break;
		}
		return QString("%1 area: %2  bratio: %3  rratio: %4  mratio: %5 ")
											.arg(typeStr)
											.arg(mom.m00)
											.arg(bRatio,0,'f',3)
											.arg(rRatio,0,'f',3)
											.arg(mRatio,0,'f',3);
	}







	//Default constructor
	Hand()
	{
		type = NONE;
	}

	//Constructor
	Hand(cv::RotatedRect r, cv::Moments m)
	{
		rotRect = r;
		mom = m;

		rotRect.points(rotPoints);
		boxRect = rotRect.boundingRect();

		rRatio = pointDist(rotPoints[2],rotPoints[1]) / pointDist(rotPoints[2], rotPoints[3]);
		bRatio = static_cast<double>(boxRect.width)/boxRect.height;
		mRatio = (static_cast<double>(boxRect.width)*boxRect.height)/mom.m00;

		setType();
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
		bRatio = h.bRatio;
		rRatio = h.rRatio;
		mRatio = h.mRatio;
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
		bRatio = rhs.bRatio;
		rRatio = rhs.rRatio;
		mRatio = rhs.mRatio;

		return *this;
	}

	//destructor
	~Hand()
	{

	}
};


#endif
