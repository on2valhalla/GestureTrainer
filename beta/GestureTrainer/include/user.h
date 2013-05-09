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

}