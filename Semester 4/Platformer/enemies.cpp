module;

import std;

import collision;
import player;

import <SDL.h>;

using namespace std::literals::chrono_literals;

export module enemies;

export class Entity : public Point
{
	std::thread* behaviourThrd;
	std::thread* animateThrd;
protected:
	int width;
	int height;

	Player* plr;
	Collision* col;
public:
	SDL_Rect a;
	SDL_Texture* texture;
	int dir;
	int rot;


	Entity(SDL_Texture* t, int ex, int ey,int w, int h, Player* p, Collision* c, bool& quit, bool& pause)
	{
		dir = -1;
		rot = 0;
		texture = t;
		x = ex;
		y = ey + h;
		width = w;
		height = h;
		plr = p;
		col = c;
		a = SDL_Rect{ 0, 0, w, h };

		behaviourThrd = new std::thread(&Entity::Behaviour, this, std::ref(quit), std::ref(pause));
		animateThrd = new std::thread(&Entity::Animate, this, std::ref(quit), std::ref(pause));
	}
	virtual ~Entity()
	{
		//std::cout << "Entity Destructor\n";
		behaviourThrd->join();
		animateThrd->join();
		delete(behaviourThrd);
		delete(animateThrd);
	}
	int GetWidth()
	{
		return width;
	}
	int GetHeight()
	{
		return height;
	}
	virtual void Animate(bool& quit, bool& pause) {
		std::cout << "This Entity Has No Animations\n";
	};
	virtual void Behaviour(bool& quit, bool& pause)
	{
		std::cout << "This Entity Has No Behaviour\n";
	};
};

export class Tomate : public Entity
{
	using Entity::Entity;
public:
	~Tomate()
	{
		//std::cout << "Tomate Destructor ";
	}
	void Animate(bool& quit, bool& pause) override
	{
		while (!quit)
		{
			if (!pause)
			{
				if (a.x >= 7 * 16)
				{
					a.x = 0;
				}
				else
				{
					a.x += 16;
				}
			}
			std::this_thread::sleep_for(100ms);
		}
	}
	void Behaviour(bool& quit, bool& pause) override
	{
		int del = 0;
		while (!quit)
		{
			if (!pause)
			{
				if (++del > 3)
				{
					del = 0;
					if (col->CheckCollision(x, y, dir, 0, 16, 16))
					{
						x += dir;
					}
					else
					{
						dir *= -1;
					}
				}
				if (col->CheckCollision(x, y, 0, -1, 16, 16))
				{
					y -= 1;
				}
				if (col->CheckOverlap(std::make_pair(Point(x, y), Point(x + 16, y - 16)), std::make_pair(plr->GetPos(), plr->GetPos() + Point(15, -31))))
				{
					plr->die = true;
				}
			}
			std::this_thread::sleep_for(7ms);
		}
	}
};

export class Piksaw : public Entity
{
	using Entity::Entity;
	int groundDir = 0;
public:
	~Piksaw()
	{
		//std::cout << "piksaw Destructor ";
	}
	void Animate(bool& quit, bool& pause) override
	{
		while (!quit)
		{
			if (!pause)
			{
				if (a.x > 3 * 16)
				{
					a.x = 0;
				}
				else
				{
					a.x += 16;
				}
			}
			std::this_thread::sleep_for(100ms);
			if (a.x == 0)
			{
				std::this_thread::sleep_for(400ms);
			}
		}
	}
	void Behaviour(bool& quit, bool& pause) override
	{
		while (!quit)
		{
			if (!pause)
			{
				switch (groundDir)
				{
					//Down
				case 0:
					if (col->CheckCollision(x, y, 0, -1, 16, 16))
					{
						groundDir = 3;
						y--;
					}
					else if (col->CheckCollision(x, y, -1, 0, 16, 16))
					{
						x--;
					}
					else
					{
						groundDir = 2;
						y++;
					}
					break;
					//Up
				case 1:
					if (col->CheckCollision(x, y, 0, 1, 16, 16))
					{
						groundDir = 2;
						y++;
					}
					else if (col->CheckCollision(x, y, 1, 0, 16, 16))
					{
						x++;
					}
					else
					{
						groundDir = 3;
						y--;
					}
					break;
					//Left
				case 2:
					if (col->CheckCollision(x, y, -1, 0, 16, 16))
					{
						groundDir = 0;
						x--;
					}
					else if (col->CheckCollision(x, y, 0, 1, 16, 16))
					{
						y++;
					}
					else
					{
						groundDir = 1;
						x++;
					}
					break;
					//Right
				case 3:
					if (col->CheckCollision(x, y, 1, 0, 16, 16))
					{
						groundDir = 1;
						x++;
					}
					else if (col->CheckCollision(x, y, 0, -1, 16, 16))
					{
						y--;
					}
					else
					{
						groundDir = 0;
						x--;
					}
					break;
				}
				if (col->CheckOverlap(std::make_pair(Point(x, y), Point(x + 16, y - 16)), std::make_pair(plr->GetPos(), plr->GetPos() + Point(15, -31))))
				{
					plr->die = true;
				}
				for (int i = 0; i < 10; i++)
				{
					rot--;
					if (rot > 359)
					{
						rot = 0;
					}
					std::this_thread::sleep_for(1ms);
				}
			}
		}
	}
};

export class Shield : public Entity
{
	using Entity::Entity;
	int anim = 0;
	bool animToggle = true;
public:
	~Shield()
	{
		//std::cout << "Shield Destructor ";
	}

	void Animate(bool& quit, bool& pause) override
	{
		while (!quit)
		{
			if (!pause)
			{
				switch (anim)
				{
				case 0:
					if (a.x != 0)
					{
						a.x = 0;
					}
					else
					{
						a.x += 16;
					}
					std::this_thread::sleep_for(250ms);
					break;
				case 1:
					if (a.x != 7 * 16)
					{
						a.x = 7 * 16;
						animToggle = !animToggle;
					}
					else
					{
						a.x += 16;
					}
					if (animToggle && a.x == 7 * 16)
					{
						dir *= -1;
					}
					std::this_thread::sleep_for(100ms);
					if (animToggle && a.x == 7 * 16)
					{
						anim = 0;
					}
					break;
				case 2:
					if (a.x < 2 * 16 || a.x >= 4 * 16)
					{
						a.x = 2 * 16;
					}
					else
					{
						a.x += 16;
					}
					std::this_thread::sleep_for(100ms);
					if (a.x == 4 * 16)
					{
						anim = 3;
					}
					break;
				case 3:
					if (a.x != 5 * 16)
					{
						a.x = 5 * 16;
					}
					else
					{
						a.x += 16;
					}
					std::this_thread::sleep_for(100ms);
					break;
				}
			}
		}
	}
	void Behaviour(bool& quit, bool& pause) override
	{
		int timer = 0;
		while (!quit)
		{
			if (!pause)
			{
				if (col->CheckOverlap(std::make_pair(Point(x, y), Point(x + 16, y - 32)), std::make_pair(plr->GetPos(), plr->GetPos() + Point(15, -31))))
				{
					plr->die = true;
				}
				if (anim == 3)
				{
					if (col->CheckCollision(x, y, dir, 0, 15, 32))
					{
						x += dir;
					}
					else
					{
						anim = 1;
					}
					std::this_thread::sleep_for(5ms);
				}
				else
				{
					if (anim == 0)
					{
						timer++;
						if (timer > 149)
						{
							timer = 0;
							anim = 2;
						}
					}
					std::this_thread::sleep_for(10ms);
				}
			}
		}
	}
};