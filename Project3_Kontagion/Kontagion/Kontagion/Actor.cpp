//Drew Letvin
//UID: 405382898

#include "Actor.h"
#include "StudentWorld.h"
using namespace std;

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
Actor::Actor(int imageID, double startX, double startY, int dir, int depth, StudentWorld* world)
	:GraphObject(imageID, startX, startY, dir, depth)
{
	m_world = world;
	m_isAlive = true;
}

Actor::~Actor() {} //Actor destructor

bool Actor::isAlive() const
{
	return m_isAlive;
}

bool Actor::isEdible() const
{
	return false;
}

bool Actor::isDirt() const
{
	return false;
}

bool Actor::inBounds(double x, double y, int r) const
{
	int calcR = abs(distance(VIEW_WIDTH / 2, VIEW_HEIGHT / 2, x, y));
	if (calcR < r)
		return true;
	else
		return false;
}

void Actor::setDead()
{
	m_isAlive = false;
}

StudentWorld* Actor::getWorld() const
{
	return m_world;
}

int Actor::distance(double x1, double y1, double x2, double y2) const
{
	return sqrt(((x2 - x1) * (x2 - x1)) + ((y2 - y1) * (y2 - y1)));
}

bool Actor::isOverlap(Actor* actor, int collideParam) const
{
	int dist = distance(this->getX(), this->getY(), actor->getX(), actor->getY());

	if (actor->isAlive() && dist <= collideParam)
	{
		return true;
	}

	return false;
}

bool Actor::isOverlap(double x, double y, Actor* actor, int collideParam) const
{
	int dist = distance(x, y, actor->getX(), actor->getY());

	if (actor->isAlive() && dist <= collideParam)
	{
		return true;
	}

	return false;
}

void Actor::doDamage(Actor* target) { return; }

bool Actor::isProjectile() const { return false; }

bool Actor::isBaddie() const { return false; }

bool Actor::isPit() const { return false; }

//SOCRATES IMPLEMENTATION/////////////////////////////////////////////////////////////////////////////////////
Socrates::Socrates(int imageID, double startX, double startY, int dir, int depth, StudentWorld* world)
	:Actor(imageID, startX, startY, dir, depth, world)
{
	m_sprayCharges = 20;
	m_flameCharges = 5;
	m_hp = 100;
}

Socrates::~Socrates() {} //Socrates Destructor

bool Socrates::isDamageable() const
{
	return true;
}

void Socrates::damage(int x)
{
	m_hp -= x;

	this->getWorld()->playSound(SOUND_PLAYER_HURT);
}

void Socrates::heal()
{
	m_hp = 100;
}

void Socrates::addFlame()
{
	m_flameCharges += 5;
}

int Socrates::numFlame() const
{
	return m_flameCharges;
}

int Socrates::numSpray() const
{
	return m_sprayCharges;
}

int Socrates::getHP() const
{
	return m_hp;
}

void Socrates::doSomething()
{
	if (!(m_hp > 0))
	{
		this->setDead();
		this->getWorld()->decLives();
		return;
	}

	int input;
	const double PI = 4 * atan(1);

	if (getWorld()->getKey(input))
	{
		if (input == KEY_PRESS_LEFT)
		{
			setDirection(getDirection() + 5);
			this->moveTo(VIEW_RADIUS * (1 - cos(getDirection() * (PI / 180))), VIEW_RADIUS * (1 - sin(getDirection() * (PI / 180))));
		}
		if (input == KEY_PRESS_RIGHT)
		{
			setDirection(getDirection() - 5);
			this->moveTo(VIEW_RADIUS * (1 - cos(getDirection() * (PI / 180))), VIEW_RADIUS * (1 - sin(getDirection() * (PI / 180))));
		}
		if (input == KEY_PRESS_SPACE && m_sprayCharges > 0)
		{
			double x = 0, y = 0;
			getPositionInThisDirection(this->getDirection(), SPRITE_RADIUS * 2, x, y);

			Spray* superMegaDeathRocket = new Spray(IID_SPRAY, x, y, this->getDirection(), 1, 112, this->getWorld());
			this->getWorld()->addToActors(superMegaDeathRocket);
			m_sprayCharges--;
			this->getWorld()->playSound(SOUND_PLAYER_SPRAY);
		}
		if (input == KEY_PRESS_ENTER && m_flameCharges > 0)
		{
			double x = 0, y = 0;
			int dir = this->getDirection();

			for (int i = 0; i < 16; i++)
			{
				getPositionInThisDirection(dir, SPRITE_RADIUS * 2, x, y);
				Flame* fuego = new Flame(IID_FLAME, x, y, dir, 1, 32, this->getWorld());
				this->getWorld()->addToActors(fuego);
				dir += 22;
			}
			this->getWorld()->playSound(SOUND_PLAYER_FIRE);
			m_flameCharges--;
		}
	}
	else if(m_sprayCharges < 20)
	{
		m_sprayCharges++;
	}

}

// DIRT IMPLEMENTATION/////////////////////////////////////////////////////////////////////////////////////////
Dirt::Dirt(int ImageID, double startX, double startY, int dir, int depth, StudentWorld* world)
	:Actor(ImageID, startX, startY, dir, depth, world)
{

}

Dirt::~Dirt() {} //Dirt Destructor

bool Dirt::isDamageable() const
{
	return true;
}

void Dirt::damage(int x)
{
	this->setDead();
}

bool Dirt::isDirt() const
{
	return true;
}

void Dirt::doSomething()
{
	return;
}

// FOOD IMPLEMENTATION/////////////////////////////////////////////////////////////////////////////////////////
Food::Food(int ImageID, double startX, double startY, int dir, int depth, StudentWorld* world)
	:Actor(ImageID, startX, startY, dir, depth, world)
{

}

Food::~Food() {} //Food Destructor

bool Food::isDamageable() const
{
	return false;
}

void Food::damage(int x)
{
	return;
}

bool Food::isEdible() const
{
	return true;
}

void Food::doSomething()
{
	return;
}

// PROJECTILE IMPLEMENTATION/////////////////////////////////////////////////////////////////////////////////////////
Projectile::Projectile(int ImageID, double startX, double startY, int dir, int depth, int maxDist, StudentWorld* world)
	:Actor(ImageID, startX, startY, dir, depth, world)
{
	m_traveled = 0;
	m_maxDist = maxDist;
}

Projectile::~Projectile() {} //Projectile destructor

void Projectile::damage(int x)
{
	return;
}

bool Projectile::isProjectile() const
{
	return true;
}

bool Projectile::isDamageable() const
{
	return false;
}

void Projectile::doSomething()
{
	if (!isAlive())
		return;

	if (this->getWorld()->onCollision(this, SPRITE_RADIUS * 2))
	{
		return;
	}
	else
	{
		double x = 0, y = 0;
		getPositionInThisDirection(this->getDirection(), SPRITE_RADIUS * 2, x, y);
		this->moveTo(x, y);
		m_traveled += (SPRITE_RADIUS * 2);
	}

	if (m_traveled >= m_maxDist)
		this->setDead();
}

// FLAME IMPLEMENTATION/////////////////////////////////////////////////////////////////////////////////////////
Flame::Flame(int ImageID, double startX, double startY, int dir, int depth, int maxDist, StudentWorld* world)
	:Projectile(ImageID, startX, startY, dir, depth, maxDist, world)
{

}

Flame::~Flame() {} //Flame Destructor


void Flame::doDamage(Actor* target)
{
	if(target->isDamageable())
		target->damage(5);
}

// SPRAY IMPLEMENTATION/////////////////////////////////////////////////////////////////////////////////////////
Spray::Spray(int ImageID, double startX, double startY, int dir, int depth, int maxDist, StudentWorld* world)
	:Projectile(ImageID, startX, startY, dir, depth, maxDist, world)
{

}

Spray::~Spray() {} //Spray Destructor

void Spray::doDamage(Actor* target)
{
	if (target->isDamageable())
		target->damage(2);
}

// BACTERIA IMPLEMENTATION//////////////////////////////////////////////////////////////////////////////////////////
Bacteria::Bacteria(int ImageID, double startX, double startY, int dir, int depth, int startHP, StudentWorld* world)
	:Actor(ImageID, startX, startY, dir, depth, world)
{
	m_foodEaten = 0;
	m_hp = startHP;
	m_plan = 0;
}

Bacteria::~Bacteria() {} //Bacteria destructor

bool Bacteria::isDamageable() const
{
	return true;
}

void Bacteria::damage(int x)
{
	m_hp -= x;
}

void Bacteria::doSomething() { return; }

int Bacteria::foodEaten() const
{
	return m_foodEaten;
}

int Bacteria::getHP() const
{
	return m_hp;
}

int Bacteria::getPlan() const
{
	return m_plan;
}

void Bacteria::setFood(int x)
{
	m_foodEaten += x;
}

void Bacteria::decPlan()
{
	m_plan--;
}

void Bacteria::resetPlan()
{
	m_plan = 10;
}

bool Bacteria::isBaddie() const
{
	return true;
}

int Bacteria::findSocrates(Actor* hero, int r)
{
	int dist = 0, dir = -1;
	const double PI = 4 * atan(1);

	dist = distance(this->getX(), this->getY(), hero->getX(), hero->getY());
	if (dist <= r)
	{
		dir = ((180 / PI) * (atan((hero->getY() - this->getY()) / (hero->getX() - this->getX()))));
		if ((this->getY() - hero->getY()) > 0 && (hero->getX() - this->getX()) > 0) //hero to the right and down
			dir = 270 + dir;
		else if ((this->getY() - hero->getY()) > 0 && (this->getX() - hero->getX()) > 0) //hero to the left and down
			dir = 180 + dir;
		else if ((hero->getY() - this->getY()) > 0 && (this->getX() - hero->getX()) > 0) //hero to the left and up
			dir = 90 + dir;
		this->setDirection(dir);
	}

	return dir;
}

bool Bacteria::checkMove()
{
	double x = 0, y = 0, theta = 0;
	getPositionInThisDirection(this->getDirection(), 3, x, y);

	if (!this->getWorld()->isDirtCollision(x, y, this, SPRITE_RADIUS) && this->inBounds(x, y, VIEW_RADIUS))
	{
		this->moveTo(x, y);
		return true;
	}
	else
	{
		theta = randInt(0, 359);
		this->setDirection(theta);
		this->resetPlan();
		return false;
	}
	return false;
}

// SALMONELLA IMPLEMENTATION//////////////////////////////////////////////////////////////////////////////////////////
Salmo::Salmo(int ImageID, double startX, double startY, int dir, int depth, int startHP, StudentWorld* world)
	:Bacteria(ImageID, startX, startY, dir, depth, startHP, world)
{

}

Salmo::~Salmo() {} //Salmo destructor

void Salmo::damage(int x)
{
	Bacteria::damage(x);
	this->getWorld()->playSound(SOUND_SALMONELLA_HURT);
}

void Salmo::doSomething()
{
	if (this->getHP() <= 0)
	{
		this->getWorld()->playSound(SOUND_SALMONELLA_DIE);
		this->setDead();

		if (randInt(1, 2) == 1)
		{
			Food* yum = new Food(IID_FOOD, this->getX(), this->getY(), 90, 1, this->getWorld());
			this->getWorld()->addToActors(yum);
		}

		this->getWorld()->increaseScore(100);

		return;
	}


	if (this->isOverlap(this->getWorld()->getHero(), SPRITE_WIDTH / 2))
	{
		this->getWorld()->getHero()->damage(1);
	}
	else
	{
		if (this->foodEaten() == 3)
		{
			this->divide();
			this->setFood(-3);
		}
		else
		{
			this->getWorld()->onCollision(this, 2 * SPRITE_RADIUS);
		}
	}

	if (this->getPlan() > 0)
	{
		this->decPlan();
		checkMove();
		return;
	}
	else
	{
		if (this->getWorld()->findClosestFood(this, 128) != -1)
		{
			checkMove();
			return;
		}
		else
		{
			int theta = randInt(0, 359);
			this->setDirection(theta);
			this->resetPlan();
			return;
		}
	}
}

void Salmo::divide()
{
	double newx = this->getX() + SPRITE_RADIUS;
	double newy = this->getY() + SPRITE_RADIUS;

	if (newx > (VIEW_WIDTH / 2))
		newx = this->getX() - SPRITE_RADIUS;
	if (newy > (VIEW_HEIGHT / 2))
		newy = this->getY() - SPRITE_RADIUS;

	Salmo* baddie = new Salmo(IID_SALMONELLA, newx, newy, 90, 0, 4, this->getWorld());
	this->getWorld()->addToActors(baddie);
}

// AGRO SALMONELLA IMPLEMENTATION//////////////////////////////////////////////////////////////////////////////////////////
AgroSalmo::AgroSalmo(int ImageID, double startX, double startY, int dir, int depth, int startHP, StudentWorld* world)
	:Bacteria(ImageID, startX, startY, dir, depth, startHP, world)
{

}

AgroSalmo::~AgroSalmo() {} //Salmo destructor

void AgroSalmo::damage(int x)
{
	Bacteria::damage(x);
	this->getWorld()->playSound(SOUND_SALMONELLA_HURT);
}

void AgroSalmo::doSomething()
{
	bool inRange = false;

	if (this->getHP() <= 0)
	{
		this->getWorld()->playSound(SOUND_SALMONELLA_DIE);
		this->setDead();

		if (randInt(1, 2) == 1)
		{
			Food* yum = new Food(IID_FOOD, this->getX(), this->getY(), 90, 1, this->getWorld());
			this->getWorld()->addToActors(yum);
		}

		this->getWorld()->increaseScore(100);

		return;
	}

	if (this->findSocrates(this->getWorld()->getHero(), 72) != -1)
	{
		checkMove();
		inRange = true;
	}

	if (this->isOverlap(this->getWorld()->getHero(), SPRITE_WIDTH / 2))
	{
		this->getWorld()->getHero()->damage(2);
	}
	else
	{
		if (this->foodEaten() == 3)
		{
			this->divide();
			this->setFood(-3);
		}
		else
		{
			this->getWorld()->onCollision(this, 2 * SPRITE_RADIUS);
		}
	}

	if (!inRange)
	{
		if (this->getPlan() > 0)
		{
			this->decPlan();
			checkMove();
			return;
		}
		else
		{
			if (this->getWorld()->findClosestFood(this, 128) != -1)
			{
				checkMove();
				return;
			}
			else
			{
				int theta = randInt(0, 359);
				this->setDirection(theta);
				this->resetPlan();
				return;
			}
		}
	}
}

void AgroSalmo::divide()
{
	double newx = this->getX() + SPRITE_RADIUS;
	double newy = this->getY() + SPRITE_RADIUS;

	if (newx > (VIEW_WIDTH / 2))
		newx = this->getX() - SPRITE_RADIUS;
	if (newy > (VIEW_HEIGHT / 2))
		newy = this->getY() - SPRITE_RADIUS;

	AgroSalmo* baddie = new AgroSalmo(IID_SALMONELLA, newx, newy, 90, 0, 4, this->getWorld());
	this->getWorld()->addToActors(baddie);
}

// ECOLI IMPLEMENTATION//////////////////////////////////////////////////////////////////////////////////////////
Ecoli::Ecoli(int ImageID, double startX, double startY, int dir, int depth, int startHP, StudentWorld* world)
	:Bacteria(ImageID, startX, startY, dir, depth, startHP, world)
{

}

Ecoli::~Ecoli() {} //Salmo destructor

void Ecoli::damage(int x)
{
	Bacteria::damage(x);
	this->getWorld()->playSound(SOUND_ECOLI_HURT);
}

void Ecoli::doSomething()
{
	if (this->getHP() <= 0)
	{
		this->getWorld()->playSound(SOUND_ECOLI_DIE);
		this->setDead();

		if (randInt(1, 2) == 1)
		{
			Food* yum = new Food(IID_FOOD, this->getX(), this->getY(), 90, 1, this->getWorld());
			this->getWorld()->addToActors(yum);
		}

		this->getWorld()->increaseScore(100);

		return;
	}


	if (this->isOverlap(this->getWorld()->getHero(), SPRITE_WIDTH / 2))
	{
		this->getWorld()->getHero()->damage(4);
	}
	else
	{
		if (this->foodEaten() == 3)
		{
			this->divide();
			this->setFood(-3);
		}
		else
		{
			this->getWorld()->onCollision(this, 2 * SPRITE_RADIUS);
		}
	}

	if (this->findSocrates(this->getWorld()->getHero(), 256) != -1)
	{
		for (int i = 0; i < 10; i++)
		{
			int tempDir = this->getDirection();
			if (checkMove())
				return;
			this->setDirection(tempDir + 10);
		}
		return;
	}

}

void Ecoli::divide()
{
	double newx = this->getX() + SPRITE_RADIUS;
	double newy = this->getY() + SPRITE_RADIUS;

	if (newx > (VIEW_WIDTH / 2))
		newx = this->getX() - SPRITE_RADIUS;
	if (newy > (VIEW_HEIGHT / 2))
		newy = this->getY() - SPRITE_RADIUS;

	Ecoli* baddie = new Ecoli(IID_ECOLI, newx, newy, 90, 0, 4, this->getWorld());
	this->getWorld()->addToActors(baddie);
}

// PIT IMPLEMENTATION//////////////////////////////////////////////////////////////////////////////////////////
Pit::Pit(int ImageID, double startX, double startY, int dir, int depth, StudentWorld* world)
	:Actor(ImageID, startX, startY, dir, depth, world)
{
	m_numRegSals = 5;
	m_numAgroSals = 3;
	m_numEColi = 2;
}

Pit::~Pit() {} //Pit destructor

bool Pit::isDamageable() const
{
	return false;
}

void Pit::damage(int x)
{
	return;
}

bool Pit::isPit() const
{
	return true;
}

void Pit::doSomething()
{
	if (m_numAgroSals == 0 && m_numRegSals == 0 && m_numEColi == 0)
	{
		this->getWorld()->noMoreDormant();
		this->setDead();
	}

	double x = this->getX(), y = this->getY();
	int numBacs = m_numAgroSals + m_numEColi + m_numRegSals;
	int p = randInt(1, 50);
	if (p == 1)
	{
		p = randInt(1, numBacs);
		if (p <= m_numEColi)
		{
			Ecoli* baddie = new Ecoli(IID_ECOLI, x, y, 90, 0, 5, this->getWorld());
			this->getWorld()->addToActors(baddie);
			m_numEColi--;
		}
		else if (p <= m_numEColi + m_numAgroSals)
		{
			AgroSalmo* baddie = new AgroSalmo(IID_SALMONELLA, x, y, 90, 0, 10, this->getWorld());
			this->getWorld()->addToActors(baddie);
			m_numAgroSals--;
		}
		else
		{
			Salmo* baddie = new Salmo(IID_SALMONELLA, x, y, 90, 0, 4, this->getWorld());
			this->getWorld()->addToActors(baddie);
			m_numRegSals--;
		}
		this->getWorld()->playSound(SOUND_BACTERIUM_BORN);
	}
}

// GOODIE IMPLEMENTATION//////////////////////////////////////////////////////////////////////////////////////////
Goodie::Goodie(int ImageID, double startX, double startY, int dir, int depth, StudentWorld* world)
	:Actor(ImageID, startX, startY, dir, depth, world)
{
	m_lifetime = max(rand() % (300 - (10 * this->getWorld()->getLevel())), 50); 
}

Goodie::~Goodie() {} //Goodie destructor

bool Goodie::isDamageable() const
{
	return false;
}

void Goodie::damage(int x)
{
	return;
}

int Goodie::getLifetime()
{
	return m_lifetime;
}

void Goodie::decLifetime()
{
	m_lifetime--;
}

void Goodie::doSomething()
{
	return;
}

// HEALTH GOODIE IMPLEMENTATION//////////////////////////////////////////////////////////////////////////////////////////
healthGoodie::healthGoodie(int ImageID, double startX, double startY, int dir, int depth, StudentWorld* world)
	:Goodie(ImageID, startX, startY, dir, depth, world)
{

}

healthGoodie::~healthGoodie() {} //Health Goodie destructor

void healthGoodie::doSomething()
{
	if (!this->isAlive())
		return;

	this->decLifetime();
	if (this->isOverlap(this->getWorld()->getHero(), SPRITE_RADIUS * 2))
	{
		this->getWorld()->increaseScore(250);
		this->setDead();
		this->getWorld()->playSound(SOUND_GOT_GOODIE);
		this->getWorld()->getHero()->heal();
		return;
	}

	if (this->getLifetime() == 0)
		this->setDead();
}

// FLAME GOODIE IMPLEMENTATION//////////////////////////////////////////////////////////////////////////////////////////
flameGoodie::flameGoodie(int ImageID, double startX, double startY, int dir, int depth, StudentWorld* world)
	:Goodie(ImageID, startX, startY, dir, depth, world)
{

}

flameGoodie::~flameGoodie() {} //Flame Goodie destructor

void flameGoodie::doSomething()
{
	if (!this->isAlive())
		return;

	this->decLifetime();
	if (this->isOverlap(this->getWorld()->getHero(), SPRITE_RADIUS * 2))
	{
		this->getWorld()->increaseScore(300);
		this->setDead();
		this->getWorld()->playSound(SOUND_GOT_GOODIE);
		this->getWorld()->getHero()->addFlame();
		return;
	}

	if (this->getLifetime() == 0)
		this->setDead();
}

// LIFE GOODIE IMPLEMENTATION//////////////////////////////////////////////////////////////////////////////////////////
lifeGoodie::lifeGoodie(int ImageID, double startX, double startY, int dir, int depth, StudentWorld* world)
	:Goodie(ImageID, startX, startY, dir, depth, world)
{

}

lifeGoodie::~lifeGoodie() {} //Life Goodie destructor

void lifeGoodie::doSomething()
{
	if (!this->isAlive())
		return;

	this->decLifetime();
	if (this->isOverlap(this->getWorld()->getHero(), SPRITE_RADIUS * 2))
	{
		this->getWorld()->increaseScore(500);
		this->setDead();
		this->getWorld()->playSound(SOUND_GOT_GOODIE);
		this->getWorld()->incLives();
		return;
	}

	if (this->getLifetime() == 0)
		this->setDead();
}

// FUNGUS IMPLEMENTATION//////////////////////////////////////////////////////////////////////////////////////////
fungus::fungus(int ImageID, double startX, double startY, int dir, int depth, StudentWorld* world)
	:Goodie(ImageID, startX, startY, dir, depth, world)
{

}

fungus::~fungus() {} //Fungus destructor

void fungus::doSomething()
{
	if (!this->isAlive())
		return;

	this->decLifetime();
	if (this->isOverlap(this->getWorld()->getHero(), SPRITE_RADIUS * 2))
	{
		this->getWorld()->increaseScore(-50);
		this->setDead();
		this->getWorld()->getHero()->damage(20);
		return;
	}

	if (this->getLifetime() == 0)
		this->setDead();
}

bool fungus::isDamageable() const
{
	return true;
}

void fungus::damage(int x)
{
	this->setDead();
}