#pragma once
#include "Entity.h"
#include "j1Player.h"
#include "j1App.h"
#include "j1Map.h"
#include "j1Input.h"
#include "Pathfinding.h"
#include "j1Entities.h"
#include "Animation.h"
#include "Cronometer.h"
#include "Player.h"

class Air_enemy : public Entity
{
public:
	Air_enemy(int x, int y);
	~Air_enemy();
	Animation idle;

	void Update(float dt);
	void OnCollision();
	void Exec_idle(float dt);
	void Exec_atack();
	void Find_path();
private:
	
	float side_fly_time;
	int ideling_heigh;
	bool is_idle = true;
	Cronometer side_fly_timer;
	
};
