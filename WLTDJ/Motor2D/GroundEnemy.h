#pragma once
#include "Entity.h"
#include "j1App.h"
#include "j1Map.h"
#include "j1Input.h"
#include "Pathfinding.h"
#include "j1Entities.h"
#include "Animation.h"
#include "Player.h"
#include "GroundEntity.h"
#include "Cronometer.h"


class GroundEnemy : public GroundEntity
{
public:
	GroundEnemy(int x, int y, Ground_Enemy_Initial_Inf initial_inf);
	~GroundEnemy();
	Animation run;
	Animation jump;

	void Update(float dt, bool do_logic);
	void OnCollision(Collider* collider);

	void ManagePhysics(float dt);

	float jumping_multiplier = 1;
	bool front_of_unwalkable = false;
	bool turn = false;
	uint height; // To determine wether if the enemy jumps upon reching a wall or not

private:

	void Jump(float dt);
	void Exec_idle();
	void Exec_attack();

	float walk_time; // Time that the enemy spends walking from one side to the other
	bool is_idle = true; // bool to know when the enemy should stop ideling and follow the player
	Cronometer walk_timer; // Timer to mesure when enemy should be moving
	int idle_speed; // speed at which the enemy idles
	int player_pos; // -1 if the player is on the left, 1 if the player is on the right
};