#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include <string>
#include <vector>

class Socrates;


// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
	virtual ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();

private:
	Socrates* hero;
	vector<Actor*> actors; 
};

#endif // STUDENTWORLD_H_
