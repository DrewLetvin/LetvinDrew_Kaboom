#include "Actor.h"
#include "StudentWorld.h"
using namespace std;

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
Actor::Actor(int imageID, double startX, double startY, int dir, int depth)
	:GraphObject(imageID, startX, startY, dir, depth)
{

}

Actor::~Actor()
{

}

//SOCRATES IMPLEMENTATION/////////////////////////////////////////////////////////////////////////////////////
Socrates::Socrates(int imageID, double startX, double startY, int dir, int depth)
	:Actor(imageID, startX, startY, dir, depth)
{
	m_sprayCharges = 20;
	m_flameCharges = 5;
	m_hp = 100;
}

Socrates::~Socrates() {} //Socrates Destructor

void Socrates::doSomething()
{
	if (!(m_hp > 0))
		return;

}