#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class Actor : public GraphObject
{
public:
	Actor(int imageID, double startX, double startY, int dir, int depth);
	virtual ~Actor();
	virtual void doSomething() = 0;
};

class Socrates: public Actor
{
public:
	Socrates(int imageID, double startX, double startY, int dir, int depth);
	~Socrates();
	void doSomething();

private:
	int m_sprayCharges;
	int m_flameCharges;
	int m_hp;
};

#endif // ACTOR_H_
