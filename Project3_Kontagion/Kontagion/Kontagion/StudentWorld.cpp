//Drew Letvin
//UID: 405382898

#include "StudentWorld.h"
#include "GameConstants.h"
#include <climits>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <iostream>

using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
	DormantBacteria = false;
}

StudentWorld::~StudentWorld()
{
	this->cleanUp();
}

Socrates* StudentWorld::getHero() const
{
	return hero;
}

void StudentWorld::noMoreDormant()
{
	int numPits = 0;
	for (int i = 0; i < actors.size(); i++)
	{
		if (actors[i]->isPit())
		{
			numPits++;
		}
	}

	if(numPits <= 1)
		DormantBacteria = false;
}

int StudentWorld::init()
{
	hero = new Socrates(IID_PLAYER, 0, VIEW_HEIGHT / 2, 0, 0, this);
	int i = max(180 - (20 * this->getLevel()), 20);
	int j = this->getLevel();
	int k = min(5 * this->getLevel(), 25);
	double r = 0, theta = 0;

	for (; j > 0; j--)
	{
		genRandPos(r, theta);

		Pit* pit = new Pit(IID_PIT, r * cos(theta) + (VIEW_WIDTH / 2), r * sin(theta) + (VIEW_HEIGHT / 2), 0, 1, this);
		actors.push_back(pit);
		DormantBacteria = true;
	}

	for (; k > 0; k--)
	{
		genRandPos(r, theta);
		int x = r * cos(theta) + (VIEW_WIDTH / 2);
		int y = r * sin(theta) + (VIEW_HEIGHT / 2);

		Food* yum = new Food(IID_FOOD, x, y, 90, 1, this);

		if (!this->isCollision(yum, SPRITE_RADIUS * 2))
		{
			actors.push_back(yum);
		}
		else
		{
			delete yum;
			k++;
		}
	}

	for (; i > 0; i--)
	{
		genRandPos(r, theta);
		int x = r * cos(theta) + (VIEW_WIDTH / 2);
		int y = r * sin(theta) + (VIEW_HEIGHT / 2);

		Dirt* dirtPile = new Dirt(IID_DIRT, x, y, 0, 1, this);

		if (!this->isCollision(dirtPile, SPRITE_RADIUS * 2))
		{
			actors.push_back(dirtPile);
		}
		else
		{
			delete dirtPile;
			k++;
		}
	}

    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	double dummy = 0, r = VIEW_RADIUS, theta = 0;
	int x = 0, y = 0, p = 0;
	
	ostringstream oss;
	oss.fill('0');
	if(this->getScore() < 0)
		oss << "Score: -" << setw(5) << abs(this->getScore());
	else
		oss << "Score: " << setw(6) << this->getScore();
	oss.fill(' ');
	oss << "  Level:" << setw(3) << this->getLevel();
	oss << "  Lives:" << setw(2) << this->getLives();
	oss << "  health:" << setw(4) << hero->getHP();
	oss << "  Sprays:" << setw(3) << hero->numSpray();
	oss << "  Flames:" << setw(3) << hero->numFlame();
	setGameStatText(oss.str());

	if (hero->isAlive())
		hero->doSomething();
	else
	{
		this->playSound(SOUND_PLAYER_DIE);
		return GWSTATUS_PLAYER_DIED;
	}

	genRandPos(dummy, theta);
	x = r * cos(theta) + (VIEW_WIDTH / 2);
	y = r * sin(theta) + (VIEW_HEIGHT / 2);

	int ChanceFungus = max(510 - (this->getLevel() * 10), 200);
	p = randInt(0, ChanceFungus);
	if (p == 0)
	{
		fungus* fungi = new fungus(IID_FUNGUS, x, y, 0, 1, this);
		actors.push_back(fungi);

	}

	genRandPos(dummy, theta);
	x = r * cos(theta) + (VIEW_WIDTH / 2);
	y = r * sin(theta) + (VIEW_HEIGHT / 2);

	int ChanceGoodie = max(510 - (this->getLevel() * 10), 200);
	p = randInt(0, ChanceGoodie);
	if (p == 0)
	{
		p = randInt(1, 100);
		if (p <= 10)
		{
			lifeGoodie* goods = new lifeGoodie(IID_EXTRA_LIFE_GOODIE, x, y, 0, 1, this);
			actors.push_back(goods);
		}
		else if (p <= 40)
		{
			flameGoodie* goods = new flameGoodie(IID_FLAME_THROWER_GOODIE, x, y, 0, 1, this);
			actors.push_back(goods);
		}
		else
		{
			healthGoodie* goods = new healthGoodie(IID_RESTORE_HEALTH_GOODIE, x, y, 0, 1, this);
			actors.push_back(goods);
		}

	}

	for (int i = 0; i < actors.size(); i++)
	{
		if (!hero->isAlive())
		{
			this->playSound(SOUND_PLAYER_DIE);
			return GWSTATUS_PLAYER_DIED;
		}
		else if (actors[i]->isAlive())
		{
			actors[i]->doSomething();
		}
		else
		{
			delete actors[i];
			actors.erase(actors.begin() + i);
			i--;
		}
	}
	
	if (this->getLives() == 0)
		return GWSTATUS_PLAYER_WON;
	else if (this->levelWon())
	{
		this->playSound(SOUND_FINISHED_LEVEL);
		return GWSTATUS_FINISHED_LEVEL;
	}
	else
		return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
	delete hero;
	hero = nullptr;
	
	vector<Actor*>::iterator it;
	for (it = actors.begin(); it != actors.end(); it++)
	{
		delete* it;
	}
	actors.clear();
	
}

bool StudentWorld::isCollision(Actor* a, int collisionParam) const
{
	for (int i = 0; i < actors.size(); i++)
	{
		if (actors[i]->isOverlap(a, collisionParam))
		{
			return true;
		}
	}

	return false;
}

bool StudentWorld::isDirtCollision(double x, double y, Actor* a, int collideParam) const
{
	for (int i = 0; i < actors.size(); i++)
	{
		if (a->isOverlap(x, y, actors[i], collideParam) && actors[i]->isDirt())
		{
			return true;
		}
	}

	return false;
}

bool StudentWorld::onCollision(Projectile* a, int collisionParam)
{
	for (int i = 0; i < actors.size(); i++)
	{
		if (actors[i]->isOverlap(a, collisionParam) && actors[i]->isDamageable() && actors[i]->isProjectile() == false)
		{
			a->doDamage(actors[i]);
			a->setDead();
			return true;
		}
	}

	return false;
}

bool StudentWorld::onCollision(Bacteria* a, int collisionParam)
{
	for (int i = 0; i < actors.size(); i++)
	{
		if (actors[i]->isOverlap(a, collisionParam) && actors[i]->isEdible())
		{
			a->setFood(1);
			actors[i]->setDead();
			return true;
		}
	}

	return false;
}

void StudentWorld::addToActors(Actor* actor)
{
	actors.push_back(actor);
}

void StudentWorld::genRandPos(double& r, double& theta)
{
	const double PI = 4 * atan(1);

	r = 120 * sqrt(static_cast<double>(randInt(0, INT_MAX)) / INT_MAX);
	theta = randInt(0, 360) * (PI / 180);
}

bool StudentWorld::levelWon() const
{
	int numBadLeft = 0;
	for (int i = 0; i < actors.size(); i++)
	{
		if (actors[i]->isBaddie())
		{
			numBadLeft++;
		}
	}

	if (!this->DormantBacteria && hero->isAlive() && numBadLeft == 0)
		return true;
	return false;
}

int StudentWorld::findClosestFood(Actor* a, int r)
{
	int minDist = r, dist = r + 1, dir = 0;
	const double PI = 4 * atan(1);

	for (int i = 0; i < actors.size(); i++)
	{
		if (actors[i]->isEdible())
		{
			dist = a->distance(a->getX(), a->getY(), actors[i]->getX(), actors[i]->getY());
			if (dist <= minDist)
			{
				minDist = dist;
				dir = ((180 / PI) * (atan((actors[i]->getY() - a->getY()) / (actors[i]->getX() - a->getX()))));
				if ((a->getY() - actors[i]->getY()) > 0 && (actors[i]->getX() - a->getX()) > 0) //hero to the right and down
					dir = 270 + dir;
				else if ((a->getY() - actors[i]->getY()) > 0 && (a->getX() - actors[i]->getX()) > 0) //hero to the left and down
					dir = 180 + dir;
				else if ((actors[i]->getY() - a->getY()) > 0 && (a->getX() - actors[i]->getX()) > 0) //hero to the left and up
					dir = 90 + dir;
			}

		}
	}

	if (dist > minDist)
		return -1;

	a->setDirection(dir);
	return dir;
}