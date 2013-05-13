/*

	Created by: Jason Carlisle Mann (on2valhalla | jcm2207@columbia.edu)

	Stores a users profile, containing simply two hands for now

*/


#ifndef USER_H
#define USER_H

#define PI 3.1415926

#include <string>
#include <deque>
#include "../include/hand.h"

class User
{

public:
	Hand fist;
    Hand spread;
	Hand curHand;
    bool isLeft;
	std::deque<double> palmRadii;

	//Constructor
    User()
	{
		for(int i = 0; i < 3; i++)
		{
			palmRadii[i] = 0;
		}
	}

	//copy constructor
	User(const User& h)
	{
		fist = h.fist;
		spread = h.spread;
		curHand = h.curHand;

		palmRadii = h.palmRadii;
	}

	//assignment operator
	User& operator=(const User& rhs)
	{
		fist = rhs.fist;
		spread = rhs.spread;
		curHand = rhs.curHand;

		palmRadii = rhs.palmRadii;

        return *this;
	}

	//destructor
	~User()
	{
		
	}

    void setCurHand(const Hand& hand)
	{
		curHand = hand;
		radiusSmoothing();
	}

	// Easy Calculation of Euclidean Distance
	double pointDist(cv::Point &p1, cv::Point &p2)
	{
		double dx = p1.x - p2.x;
		double dy = p1.y - p2.y;
		return sqrt(dx * dx + dy * dy);
	}

	void radiusSmoothing()
	{
		palmRadii.push_back(curHand.palmRadius);


        if(palmRadii.size() > 3)
			palmRadii.pop_front();

		double localRadius = 0;
		for(int i = 0; i < 3; i++)
		{
			localRadius += palmRadii[i] * (i+1);

		}

		localRadius = localRadius / 6;
		curHand.palmArea = PI * (localRadius * localRadius);

		curHand.findType();

    }


	void setLeft(bool left)
	{
		isLeft = left;
    }
};

#endif
