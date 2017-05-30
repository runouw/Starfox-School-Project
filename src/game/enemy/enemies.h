#pragma once

#include <gsim\gs_mat.h>
#include <gsim\gs_vec.h>

#include <rtools\ai\model.h>

#include <game\projectile\projectiles.h>

#include <queue>

class Enemy;

struct DamageData{
	int amount; // amount of hitpoints dealt
	int type; // Elemental type of damage
	GsVec force; // knockback direction and magnitude
	void* sender;
	void(*onhit)(const DamageData& data, const Enemy* enemy);

	static void doNothing(const DamageData& data, const Enemy* enemy){};

	DamageData(int amount, int type, GsVec force, void* sender, void(*f)(const DamageData& data, const Enemy* enemy)) :amount(amount), type(type), force(force), sender(sender){
		this->onhit = f;

		if (this->onhit == NULL){
			this->onhit = doNothing;
		}
	}
};

// Interface for enemy
class Enemy{

public:
	bool exists; // set to false to be removed from array (and the destructor will be called)

	GsVec pos;

	int hp; // if hp is set to 0, enemy should trigger death animation

	virtual void idle(float dt) = 0;

	virtual void draw(const GsMat& tr, const GsMat& pr) = 0;
	virtual void postDraw(const GsMat& tr, const GsMat& pr) = 0;
	virtual void addDraw(const GsMat& tr, const GsMat& pr) = 0;
	virtual void putLights() = 0;

	virtual bool projectileCollisionCheck(GsVec center, float radius, DamageData& data) = 0;

	static void doLaser(const GsVec& pos, const GsVec& dir, float dis);
	static void doLaserHintDraw(const GsVec& pos, const GsVec& dir, float dis, float str, const GsMat& tr, const GsMat& pr);
	static void doLaserDraw(const GsVec& pos, const GsVec& dir, float dis, float str, const GsMat& tr, const GsMat& pr);

	static void doLaserLights(const GsVec& pos, const GsVec& dir, float dis);
};

class Enemy_Ship1 : public Enemy{
	static Model* ship;

	GsVec dir;
	// TODO: path

	float laserWait;
	float laserTimer;
	float t;
	bool shootingLaser;

	float rz;
	float rzv;

public:
	static void init();
	Enemy_Ship1(GsVec pos, GsVec dir);

	void idle(float dt);

	void draw(const GsMat& tr, const GsMat& pr);
	void postDraw(const GsMat& tr, const GsMat& pr){};
	void addDraw(const GsMat& tr, const GsMat& pr);
	void putLights();

	bool projectileCollisionCheck(GsVec center, float radius, DamageData& data);
};

class Enemy_Ship2 : public Enemy{
	static Model* ship;

	GsVec dir;
	// TODO: path

	float laserWait;
	float laserTimer;
	float t;
	bool shootingLaser;

	float rz;
	float rzv;

public:
	static void init();
	Enemy_Ship2(GsVec pos, GsVec dir);

	void idle(float dt);

	void draw(const GsMat& tr, const GsMat& pr);
	void postDraw(const GsMat& tr, const GsMat& pr){};
	void addDraw(const GsMat& tr, const GsMat& pr);
	void putLights();

	bool projectileCollisionCheck(GsVec center, float radius, DamageData& data);
};

#define BOSS_BIGGUN_IDLE 0
#define BOSS_BIGGUN_IDLE_AIM 1
#define BOSS_BIGGUN_LEFTRIGHT 2
#define BOSS_BIGGUN_BARRELROLL 3
#define BOSS_BIGGUN_ROT90 4
#define BOSS_BIGGUN_ENCOUNTERIDLE 99

#define STATE1 1
class Boss_BigGun : public Enemy{
	static Model* ship;
	static Model* shipLights;
	static Model* button;

	GsVec dir;
	// TODO: path

	int* b_hp;
	int big_hp;

	float laserWait;
	float laserTimer;
	float t;

	float rz;
	float rzv;

	bool toggleMidLasers;
	bool toggleTopLasers;

	float midLasersCharge;
	float topLasersCharge;

	bool allowRotFix;

	int state;
	std::queue<int> stateQueue;
	int getNextState(){
		if (!stateQueue.empty()){
			int state = stateQueue.front();
			stateQueue.pop();
			return state;
		}
		else{
			stateQueue.push(BOSS_BIGGUN_IDLE);
			stateQueue.push(BOSS_BIGGUN_IDLE_AIM);
			stateQueue.push(BOSS_BIGGUN_ROT90);
			stateQueue.push(BOSS_BIGGUN_IDLE_AIM);
			stateQueue.push(BOSS_BIGGUN_ROT90);
			stateQueue.push(BOSS_BIGGUN_LEFTRIGHT);
			stateQueue.push(BOSS_BIGGUN_IDLE);
			stateQueue.push(BOSS_BIGGUN_BARRELROLL);
			stateQueue.push(BOSS_BIGGUN_IDLE);
			stateQueue.push(BOSS_BIGGUN_IDLE_AIM);
			return getNextState();
		}
	}
	float stateTimer;

	void addSmallExplosion(GsVec pos);

	void setNextState(){
		state = getNextState();
		stateTimer = 0;
	}

public:
	static void init();
	Boss_BigGun(GsVec pos, GsVec dir);

	void idle(float dt);

	void draw(const GsMat& tr, const GsMat& pr);
	void postDraw(const GsMat& tr, const GsMat& pr);
	void addDraw(const GsMat& tr, const GsMat& pr);
	void putLights();

	bool projectileCollisionCheck(GsVec center, float radius, DamageData& data);
};