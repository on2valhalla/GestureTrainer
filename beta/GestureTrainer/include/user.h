/*

	Created by: Jason Carlisle Mann (on2valhalla | jcm2207@columbia.edu)

	Stores a users profile, containing simply two hands for now

*/


#ifndef USER_H
#define USER_H

#include <string>
#include "../include/hand.h"

class User
{

public:
	Hand fist;
	Hand spread;
	Hand curHand;

	double pinky;
	double ring;
	double pointer;
	double middle;
	double thumb;

	//Constructor
    User()
	{

	}

	//copy constructor
	User(const User& h)
	{
		fist = h.fist;
		spread = h.spread;
		curHand = h.curHand;
	}

	//assignment operator
	User& operator=(const User& rhs)
	{
		fist = rhs.fist;
		spread = rhs.spread;
		curHand = rhs.curHand;

        return *this;
	}

	//destructor
	~User()
	{
		
	}

	// Easy Calculation of Euclidean Distance
	double pointDist(cv::Point &p1, cv::Point &p2)
	{
		double dx = p1.x - p2.x;
		double dy = p1.y - p2.y;
		return sqrt(dx * dx + dy * dy);
	}

	void curType()
	{

	}

	void getFingers()
	{
		cv::vector<cv::Vec4i> defects = spread.defects;
		std::vector<std::vector< cv::Point > > contour = spread.contour;
		std::string orientation;

		cv::Vec4i minDefect = defects[0];

		while(defects.size() > 4)
		{
			for(cv::Vec4i defect : defects)
			{
				if(defect[3]/256.0 < minDefect[3]/256.0)
					minDefect = defect;
			}

			defects.erase(std::remove(defects.begin(), defects.end(), minDefect), defects.end());
		}

		cv::Vec4i rightMost = defects[0];
		cv::Vec4i leftMost = defects[3];

		if(pointDist(contour[0][rightMost[0]], contour[0][rightMost[1]]) > 
				pointDist(contour[0][leftMost[0]], contour[0][leftMost[1]]))
					orientation = "left";
		else orientation = "right";



	}
};

#endif
