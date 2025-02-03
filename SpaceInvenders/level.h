#pragma once
#include "gameobject.h"
#include "sgg/graphics.h"
#include <string>
#include <vector>
#include <list>
#include "box.h"
#include "enemy.h"
#include "bullet.h"
#include "player.h"
#include "util.h"

class Level : public GameObject {
	graphics::Brush m_brush_background;

	std::list<Enemy*> m_enemies; // thelo na ballo kai allous enemy sigoura 

	std::vector<Bullet*> m_bullets; 

	std::vector<std::vector<Enemy*>> m_rows;

	float m_center_x = 5.0f;
	float m_center_y = 5.0f;
	const float m_block_size = 1.0f;

	void drawBlock(int i);

	graphics::Brush m_block_brush;
	graphics::Brush m_block_brush_debug;

	std::vector<GameObject*>m_static_object;
	std::list<GameObject*>m_dynamic_object;

	std::vector<Box> m_blocks;
	std::vector<std::string> m_block_names;

	
private:
	int rows = 2;               
	int enemies_per_row = 7;    

	float m_enemy_shoot_timer = 0.0f; 
	float m_enemy_shoot_interval = 1800.0f; 
	Enemy* getRandomActiveEnemy(bool is_top_row);
	std::string m_level_name;


public:
	void update(float dt) override;
	void init() ;
	void draw() override;
	void spawnEnemies();
	void setDifficulty(int level_number);
	void moveEnemies();
	void checkEnemyPositions();
	std::string getLevelName() const { return m_level_name; }
	void setLevelName(const std::string& name) { m_level_name = name; }

	void spawnBullet(float x, float y, float speed, bool is_player_bullet, bool direction_up);
	void handleBulletCollisions();

	void shootEnemyBullets();

	void drawStartMenu();

	void drawGameOverScrn();


	void handleMouse();

	void resetEntities();

	bool isComplete() const;


	Level(const std::string& name = "level0");
	~Level();

};