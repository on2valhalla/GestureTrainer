/*

	Created by: Jason Carlisle Mann (on2valhalla | jcm2207@columbia.edu)

	Stores a users profile, containing simply two hands for now

*/


#ifndef USER_H
#define USER_H


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


	// Easy Calculation of Euclidean Distance
	double pointDist(cv::Point2f &p1, cv::Point2f &p2)
	{
		double dx = p1.x - p2.x;
		double dy = p1.y - p2.y;
		return sqrt(dx * dx + dy * dy);
	}

	void getFingers()
	{
		cv::vector<cv::Vec4i> defects = spread.defects;
		std::vector<std::vector< cv::Point > > contour = spread.contour;

		cv::Vec4i minDefect = defects[0];
		cv::Vec4i thumbDefect = defects[0];


		while(defects.size() > 4)
		{
			for(cv::Vec4i defect : defects)
			{
				if(contour[0][defect[3]]/256.0 < contour[0][minDefect[3]]/256.0)
					minDefect = defect;
			}

			defects.erase(std::remove(defects.begin(), defects.end(), minDefect), defects.end());
		}


		for(cv::Vec4i defect : defects)
		{
			
			if(pointDist(contour[0][defect[0]], contour[0][defect[1]]) > 
				pointDist(contour[0][thumbDefect[0]], contour[0][thumbDefect[1]]))
					thumbDefect = defect;
		}



	}
};

#endif
