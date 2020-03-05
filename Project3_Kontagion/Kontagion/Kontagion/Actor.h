//Drew Letvin
//UID: 405382898

#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"

class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class Actor : public GraphObject
{
public:
	Actor(int imageID, double startX, double startY, int dir, int depth, StudentWorld* world);
	virtual ~Actor();
	bool isAlive() const;
	bool inBounds(double x, double y, int r) const;
	void setDead();
	bool isOverlap(Actor* actor, int collideParam) const;
	bool isOverlap(double x, double y, Actor* actor, int collideParam) const;
	int distance(double x1, double y1, double x2, double y2) const;
	StudentWorld* getWorld() const;
	virtual bool isDamageable() const = 0;
	virtual void doSomething() = 0;
	virtual void damage(int x) = 0;
	virtual bool isEdible() const;
	virtual bool isDirt() const;
	virtual bool isPit() const;
	virtual void doDamage(Actor* target);
	virtual bool isProjectile() const;
	virtual bool isBaddie() const;

private:
	StudentWorld* m_world;
	bool m_isAlive;
};

class Socrates: public Actor
{
public:
	Socrates(int imageID, double startX, double startY, int dir, int depth, StudentWorld* world);
	virtual ~Socrates();
	virtual bool isDamageable() const;
	virtual void damage(int x);
	void heal();
	void addFlame();
	void doSomething();
	int numFlame() const;
	int numSpray() const;
	int getHP() const;

private:
	int m_sprayCharges;
	int m_flameCharges;
	int m_hp;
};

class Dirt: public Actor
{
public:
	Dirt(int imageID, double startX, double startY, int dir, int depth, StudentWorld* world);
	virtual ~Dirt();
	virtual bool isDamageable() const;
	virtual void damage(int x);
	virtual bool isDirt() const;
	void doSomething();
};


class Food: public Actor
{
public:
	Food(int imageID, double startX, double startY, int dir, int depth, StudentWorld* world);
	virtual ~Food();
	virtual bool isDamageable() const;
	virtual void damage(int x);
	virtual bool isEdible() const;
	void doSomething();
};

class Projectile : public Actor
{
public:
	Projectile(int imageID, double startX, double startY, int dir, int depth, int maxDist, StudentWorld* world);
	virtual ~Projectile();
	virtual bool isDamageable() const;
	virtual void damage(int x);
	virtual bool isProjectile() const;
	void doSomething();

private:
	int m_traveled;
	int m_maxDist;
};

class Flame: public Projectile
{
public:
	Flame(int imageID, double startX, double startY, int dir, int depth, int maxDist, StudentWorld* world);
	virtual ~Flame();
	virtual void doDamage(Actor* target);
};

class Spray : public Projectile
{
public:
	Spray(int imageID, double startX, double startY, int dir, int depth, int maxDist, StudentWorld* world);
	virtual ~Spray();
	virtual void doDamage(Actor* target);
};


class Pit : public Actor
{
public:
	Pit(int imageID, double startX, double startY, int dir, int depth, StudentWorld* world);
	virtual ~Pit();
	virtual bool isDamageable() const;
	virtual void damage(int x);
	void doSomething();
	virtual bool isPit() const;

private:
	int m_numRegSals;
	int m_numAgroSals;
	int m_numEColi;
};

class Bacteria : public Actor
{
public:
	Bacteria(int ImageID, double startX, double startY, int dir, int depth, int startHP, StudentWorld* world);
	virtual ~Bacteria();
	virtual bool isDamageable() const;
	virtual void damage(int x);
	int getHP() const;
	bool checkMove();
	int foodEaten() const;
	int getPlan() const;
	void decPlan();
	void setFood(int x);
	virtual void divide() = 0;
	void doSomething();
	void resetPlan();
	virtual bool isBaddie() const;
	int findSocrates(Actor* hero, int r);

private:
	int m_foodEaten;
	int m_hp;
	int m_plan;
};

class Salmo : public Bacteria
{
public:
	Salmo(int ImageID, double startX, double startY, int dir, int depth, int startHP, StudentWorld* world);
	virtual ~Salmo();
	virtual void damage(int x);
	void doSomething();
	virtual void divide();
};

class AgroSalmo : public Bacteria
{
public:
	AgroSalmo(int ImageID, double startX, double startY, int dir, int depth, int startHP, StudentWorld* world);
	virtual ~AgroSalmo();
	virtual void damage(int x);
	void doSomething();
	virtual void divide();
};

class Ecoli : public Bacteria
{
public:
	Ecoli(int ImageID, double startX, double startY, int dir, int depth, int startHP, StudentWorld* world);
	virtual ~Ecoli();
	virtual void damage(int x);
	void doSomething();
	virtual void divide();
};

class Goodie : public Actor
{
public:
	Goodie(int ImageID, double startX, double startY, int dir, int depth, StudentWorld* world);
	virtual ~Goodie();
	virtual bool isDamageable() const;
	virtual void damage(int x);
	int getLifetime();
	void decLifetime();
	void doSomething();

private:
	int m_lifetime;
};

class healthGoodie : public Goodie
{
public:
	healthGoodie(int ImageID, double startX, double startY, int dir, int depth, StudentWorld* world);
	virtual ~healthGoodie();
	void doSomething();
};

class flameGoodie : public Goodie
{
public:
	flameGoodie(int ImageID, double startX, double startY, int dir, int depth, StudentWorld* world);
	virtual ~flameGoodie();
	void doSomething();
};

class lifeGoodie : public Goodie
{
public:
	lifeGoodie(int ImageID, double startX, double startY, int dir, int depth, StudentWorld* world);
	virtual ~lifeGoodie();
	void doSomething();
};

class fungus : public Goodie
{
public:
	fungus(int ImageID, double startX, double startY, int dir, int depth, StudentWorld* world);
	virtual ~fungus();
	void doSomething();
	virtual bool isDamageable() const;
	virtual void damage(int x);
};

#endif // ACTOR_H_
