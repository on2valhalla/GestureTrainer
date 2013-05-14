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


enum Direction{

	RIGHT,
	LEFT
};


class User
{

public:
	Hand fist;
	Hand spread;
	cv::Vec4i thumbDefect;
	Direction orient;

	Finger thumb;
	Finger index;
	Finger middle;
	Finger ring;
	Finger pinky;

	Hand curHand;
	std::deque<double> palmRadii;
	std::deque<cv::Point2f> palmCenters;


	double c2eSLOPE;
	double c2bSLOPE;
	double sigSlope;

	//Constructor
	User()
	{
		for(int i = 0; i < 3; i++)
		{
			palmRadii[i] = 0;
		}
		orient = LEFT;
	}

	//copy constructor
	User(const User& h)
	{
		fist = h.fist;
		spread = h.spread;
		curHand = h.curHand;

		palmRadii = h.palmRadii;
		c2eSLOPE = h.c2eSLOPE;
		c2bSLOPE = h.c2bSLOPE;
		orient = h.orient;

		pinky = h.pinky;
		index = h.index;
		middle = h.middle;
		ring = h.ring;
		pinky = h.pinky;

		sigSlope = h.sigSlope;
	}

	//assignment operator
	User& operator=(const User& rhs)
	{
		fist = rhs.fist;
		spread = rhs.spread;
		curHand = rhs.curHand;

		palmRadii = rhs.palmRadii;
		c2eSLOPE = rhs.c2eSLOPE;
		c2bSLOPE = rhs.c2bSLOPE;
		orient = rhs.orient;

		pinky = rhs.pinky;
		index = rhs.index;
		middle = rhs.middle;
		ring = rhs.ring;
		pinky = rhs.pinky;

		sigSlope = rhs.sigSlope;

		return *this;
	}

	//destructor
	~User()
	{
		
	}

	void setSpreadHand(const Hand& hand)
	{
		spread = hand;
		cleanDefects(spread);
		setFingers();
	}

	void setFingers()
	{
		std::vector<Finger> fingers = spread.fingers;

		thumb = fingers[0];
		pinky = fingers[1];
		ring = fingers[2];
		middle = fingers[3];
        index = fingers[4];


		for(Finger finger : fingers)
		{
			if(thumb.angle < finger.angle)
				swapFingers(&thumb, &finger);
		}

		for(Finger finger: fingers)
		{
			if(pinky.angle > finger.angle)
                swapFingers(&pinky, &finger);
		}

		if(ring.angle > index.angle)
			swapFingers(&ring, &index);

		if(ring.angle > middle.angle)
			swapFingers(&ring, &middle);

		if(middle.angle > index.angle)
			swapFingers(&index, &middle);

	}


    void swapFingers(Finger* a, Finger* b)
	{
		Finger temp = *b;
		*b = *a;
		*a = temp;
	}

	void setCurHand(const Hand& hand)
	{
		curHand = hand;
        if(curHand.isNone())
			return;
		
		radiusSmoothing();
		centerSmoothing();

		curHand.findFingers();
		curHand.findClass();

		if(curHand.type == FIST)
			fistClass();
		else if (curHand.type == PALM)
			palmClass();
	}

	double calcSlope(cv::Point a, cv::Point b)
	{
		double rise = (a.y-b.y);
		double run = (a.x-b.x);
		double slope = rise/run;
		return slope;
	}

	void fistClass()
	{		
		std::vector<cv::Point> contour = curHand.contour[0];

		int c = 0;
		int b = 5;
		int e = contour.size()-5;
		int max = contour.size()/2;

		int search = 1;

		while((b < max) && search)
		{
			switch(orient)
			{
				case LEFT:
					c2eSLOPE = calcSlope(contour[c], contour[e]);
					sigSlope = c2eSLOPE;
					if(c2eSLOPE > 0.5)
						curHand.type = T;  //EXPAND
					else
						curHand.type = A;
					search = 0;
					break;
				case RIGHT:
					c2bSLOPE = calcSlope(contour[c], contour[b]);
					sigSlope = c2bSLOPE;
					if(c2bSLOPE < 0.5)
						curHand.type = T; //EXPAND
					else
						curHand.type = A;
					search = 0;
					break;			
				default:
					curHand.type = FIST;
					break;
			}

			b = b+5;
			e = e-5;
		}
		
	}

	void palmClass()
	{
		int count = curHand.fingers.size();

		if(count == 1)
		{
			curHand.type = I;
			
		}
		else if(count == 2)
		{
			std::vector<Finger> fingers = curHand.fingers;

			double subAngle = std::abs(fingers[0].angle - fingers[1].angle);
			//qDebug() << "subAngle: " << subAngle;

			double ind2mid = std::abs(index.angle-middle.angle);
			//qDebug() << "ind2mid: " << ind2mid;
			double thumb2pink = std::abs(thumb.angle-pinky.angle);
			//qDebug() << "thumb2pink: " << thumb2pink;
			double thumb2ind = std::abs(thumb.angle-index.angle);
			//qDebug() << "thumb2ind: " << thumb2ind;


			double subVind2mid = std::abs(subAngle - ind2mid);
			// qDebug() << "subVind2mid: " << subVind2mid; 
			double subVthumb2pink = std::abs(subAngle - thumb2pink);
			// qDebug() << "subVthumb2pink: " << subVthumb2pink;
			double subVthumb2ind = std::abs(subAngle - thumb2ind);
			// qDebug() << "subVthumb2ind: " << subVthumb2ind;
			

			if(subVind2mid < subVthumb2pink)
			{
				if(subVind2mid < subVthumb2ind)
					curHand.type = V;
				else
					curHand.type = L;
			}
			else if(subVthumb2pink < subVthumb2ind)
				curHand.type = Y;
			else
				curHand.type = V;
            
			// if(((thumb2pink-0.5) < subAngle) && (subAngle < (thumb2pink+0.5)))
			// 	curHand.type = Y;
			// else if(((thumb2ind-0.5) < subAngle) && (subAngle < (thumb2ind+0.5)))
			// 	curHand.type = L;
			// if(((ind2mid-0.2) < subAngle) && (subAngle < (ind2mid+0.2)))
			// 	curHand.type = V;
			
		}
		else if(count == 3)
		{
			curHand.type = W;
		}
		else 
			curHand.type = PALM;

	}

	bool contComparing(std::string goal)
	{
		std::string type = curHand.getType().toStdString();
		
		if(type.compare(goal) != 0)
			return false;
		else
			return true;
	}

	void cleanDefects(Hand& hand)
	{
		cv::vector<cv::Vec4i> defects = hand.defects;
		std::vector<std::vector<cv::Point>> contour = hand.contour;

		int minDefect = 0;
		while(defects.size() > 4)
		{
			for(unsigned int i = 0; i < defects.size(); i++)
			{
				if(defects[i][3]/256.0 < defects[minDefect][3]/256.0)
					minDefect = i;
			}

			defects.erase(defects.begin() + minDefect);
		}

		// cv::Vec4i leftMost = defects[defects.size() - 1];
		// cv::Vec4i rightMost = defects[0];

		// if(leftMost[3]/256.0 < rightMost[3]/256.0)
		// 	orient = LEFT;
		// else
		// 	orient = RIGHT;
	}

	void radiusSmoothing()
	{
		palmRadii.push_back(curHand.palmRadius);


		if(palmRadii.size() > 3)
			palmRadii.pop_front();

		double localRadius = 0;
		for(int i = 0; i < 3; i++)
			localRadius += palmRadii[i] * (i+1);

		localRadius = localRadius / 6;

		curHand.palmRadius = localRadius;
		curHand.palmArea = PI * (localRadius * localRadius);

	}

	void centerSmoothing()
	{
		palmCenters.push_back(curHand.palmCenter);


		if(palmCenters.size() > 3)
			palmCenters.pop_front();

		cv::Point2f localCenter;
		for(int i = 0; i < 3; i++)
			localCenter += palmCenters[i] * (i+1);

		// qDebug() << "new: "<< localCenter.x << ", " << localCenter.y;
		//!!!!!!!!!!//
		localCenter.x /= 6;
		localCenter.y /= 6;
		// qDebug() << "new: "<< localCenter.x << ", " << localCenter.y
		// 	<<"  old: " <<curHand.palmCenter.x << ", " << curHand.palmCenter.y;

		curHand.palmCenter = localCenter;
	}
	

	QString getOrient()
	{
		switch(orient)
		{
			case LEFT:
				return QString("Left");
			case RIGHT:
				return QString("Right");
			default:
				return QString("Unknown");
		}

	}

	void setLeft(bool left)
	{
		if(left)
			orient = LEFT;
		else
			orient = RIGHT;
	}

	QString getData()
	{
		QString data = QString(
					"Orientation: %1\n"
					"SLOPE: %2\n"
					"THUMB: %3\nINDEX: %4"
					"\nMIDDLE: %5\nRING: %6"
					"\nPINKY: %7\n")
					   .arg(getOrient())
					   .arg(sigSlope)
					   .arg(thumb.angle)
					   .arg(index.angle)
					   .arg(middle.angle)
					   .arg(ring.angle)
					   .arg(pinky.angle);

		return data;
	}

	bool isLeft()
	{
		switch(orient)
		{
			case LEFT:
				return true;
			default:
				return false;
		}
	}
	
};

#endif
