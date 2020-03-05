//Drew Letvin
//UID: 405382898

#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include <string>
#include <vector>

class Socrates;
class Projectile;
class Bacteria;
class Actor;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
	virtual ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();

	bool isCollision(Actor* a, int collideParam) const;
	bool isDirtCollision(double x, double y, Actor* a, int collideParam) const;
	bool onCollision(Projectile* a, int collideParam);
	bool onCollision(Bacteria* a, int collideParam);
	void addToActors(Actor* actor);
	void noMoreDormant();
	Socrates* getHero() const;
	int findClosestFood(Actor* a, int r);

private:
	void genRandPos(double& r, double& theta);
	bool levelWon() const;

	Socrates* hero;
	std::vector<Actor*> actors;
	bool DormantBacteria;
};

#endif // STUDENTWORLD_H_
