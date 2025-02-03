#include "level.h"
#include "gamestate.h"
#include "player.h"
#include <iostream>
#include <random>
#include <thread>
#include <chrono>

void Level::drawBlock(int i)
{
	Box& box = m_blocks[i];
	graphics::Brush m_block_brush;

	m_block_brush.fill_opacity = 1.0f;
	m_block_brush.outline_opacity = 0.0f;
	m_block_brush.texture = m_state->getAssetPath("obstacle.png");

	graphics::drawRect(box.m_pos_x, box.m_pos_y, box.m_width, box.m_height, m_block_brush);


}

Enemy* Level::getRandomActiveEnemy(bool is_top_row)
{
	std::vector<Enemy*> active_enemies;

	for (Enemy* enemy : m_enemies) {
		if (enemy->isActive() && enemy->isTopRow() == is_top_row) {
			active_enemies.push_back(enemy);
		}
	}

	if (active_enemies.empty()) {
		return nullptr;
	}

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, active_enemies.size() - 1);

	return active_enemies[dis(gen)];
}

void Level::update(float dt)
{
	if (m_state->getCurrentState() != GameState::RUNNING) return;

	if (m_state->getPlayer()->isActive())
		m_state->getPlayer()->update(dt);

	static float timer = 0.0f; 
	timer += dt;
	m_enemy_shoot_timer += dt;

	if (timer >= 800.0f) { 
		moveEnemies();
		timer = 0.0f; 
	}

	for (Enemy* enemy : m_enemies) {
		if (enemy->isActive()) {
			enemy->update(dt);
		}
	}

	m_state->getPlayer()->shoot(m_bullets);
	m_state->getPlayer()->updateBullets(dt);

	Player* player = m_state->getPlayer();
	if (player && player->isActive()) {
		player->shoot(m_bullets); 
	}

	if (m_enemy_shoot_timer >= m_enemy_shoot_interval) {
		
		m_enemy_shoot_timer = 0.0f;

		
		shootEnemyBullets();
	}


	
	for (Bullet* bullet : m_bullets) {
		bullet->update(dt); 
	}

	
	handleBulletCollisions();

	
	m_bullets.erase(std::remove_if(m_bullets.begin(), m_bullets.end(),
		[](Bullet* b) { return !b->isActive(); }),
		m_bullets.end());

	checkEnemyPositions();

	GameObject::update(dt);
}

void Level::init()
{ 
	if (m_level_name.empty()) {
		m_level_name = "level1";  
	}
	setLevelName(m_level_name);

	// pairno to number apo to level gia na bazo anebazo tin dyskolia dinamika sta lvls
	int level_number = std::stoi(m_level_name.substr(5)); 
	setDifficulty(level_number);


	m_blocks.clear();
	m_enemies.clear();
	m_bullets.clear();
	m_rows.clear();

	m_brush_background.outline_opacity = 0.f;

	m_brush_background.texture = GameState::getInstance()->getAssetPath(m_level_name + "_background.png");


	graphics::setFont(m_state->getAssetPath("font_1.ttf"));
	

	m_blocks.clear(); 
	m_rows.clear();

	//mallon tha ta kratiso stathera an den prolabo
	Box obstacle1;
	obstacle1.m_pos_x = m_state->getCanvasWidth() / 2.0f-0.4f; 
	obstacle1.m_pos_y = m_state->getCanvasHeight() / 2.0f - 0.5f;
	obstacle1.m_width = 0.25f; 
	obstacle1.m_height = 0.35f; 
	m_blocks.push_back(obstacle1);

	Box obstacle2;
	obstacle2.m_pos_x = m_state->getCanvasWidth() / 2.0f+ 1.8f; 
	obstacle2.m_pos_y = m_state->getCanvasHeight() / 2.0f + 0.5f;
	obstacle2.m_width = 0.25f;  
	obstacle2.m_height = 0.35f; 
	m_blocks.push_back(obstacle2);

	spawnEnemies();

}

void Level::draw() {


	float canvas_width = GameState::getInstance()->getCanvasWidth();
	float canvas_height = GameState::getInstance()->getCanvasHeight();
	float texture_width, texture_height;

	if (m_level_name == "level1") {
		texture_width = 550.0f;
		texture_height = 360.0f;
	}
	else if (m_level_name == "level2") {
		texture_width = 610.0f;
		texture_height = 300.0f;
	}
	else if (m_level_name == "level3") {
		texture_width = 1164.0f;
		texture_height = 655.0f;
	}
	else {
		texture_width = 1.0f; 
		texture_height = 1.0f;
	}

	// boithitiko gt den mpainoun kala ta background ston canva (to brika intertne so)
	float texture_aspect_ratio = texture_width / texture_height;
	float canvas_aspect_ratio = canvas_width / canvas_height;

	if (texture_aspect_ratio > canvas_aspect_ratio) {
		graphics::drawRect(canvas_width / 2.0f, canvas_height / 2.0f,
			canvas_height * texture_aspect_ratio, canvas_height, m_brush_background);
	}
	else {
		graphics::drawRect(canvas_width / 2.0f, canvas_height / 2.0f,
			canvas_width, canvas_width / texture_aspect_ratio, m_brush_background);
	}
	std::string planet_name = getPlanetName(m_level_name);
	graphics::setFont(GameState::getInstance()->getAssetPath("planet_font.ttf"));

	//to onoma tis pistas ousiastika (oxi to level1 ktlp kathe pista enas platinits)
	graphics::Brush text_brush;
	text_brush.fill_opacity = 1.0f;
	text_brush.outline_opacity = 0.0f;
	SETCOLOR(text_brush.fill_color, 0.0f, 0.0f, 0.0f);

	float text_size = 0.3f;
	float text_x = canvas_width - 1.1f; 
	float text_y = 0.5f;                
	graphics::drawText(text_x, text_y, text_size, planet_name, text_brush);

	if (m_state->getPlayer()->isActive())
		m_state->getPlayer()->draw();

	m_state->getPlayer()->drawLives();

	for (auto p_gob : m_static_object)
		if (p_gob) p_gob->draw();
	for (auto p_gob : m_dynamic_object)
		if (p_gob) p_gob->draw();

	for (Enemy* enemy : m_enemies) {
		enemy->draw();
	}

	for (int i = 0; i < m_blocks.size(); i++) {
		drawBlock(i);
	}

	m_state->getPlayer()->drawBullets();

	for (Bullet* bullet : m_bullets) {
		bullet->draw();
	}
}


void Level::spawnEnemies()
{

	m_enemies.clear(); 


	float canvas_width = m_state->getCanvasWidth();
	float canvas_height = m_state->getCanvasHeight();

	std::cout << "Canvas Height: " << canvas_height << std::endl;

	float gap = 0.5f; 
	float row_space = 0.5f;          

	// pano enemies
	for (int row = 0; row < rows; ++row) {
		std::vector<Enemy*> row_enemies; 
		for (int i = 0; i < enemies_per_row; ++i) { 
			float x = (canvas_width / 2.0f) - 2.4f + (i * gap); 
			float y = 0.8f + (row * row_space); 
			Enemy* enemy = new Enemy("EnemyTop", true);
			enemy->init(x, y);
			m_enemies.push_back(enemy); 
			row_enemies.push_back(enemy);
		}
		
		m_rows.push_back(row_enemies);
	}

	// kato enemies
	for (int row = 0; row < rows; ++row) {
		std::vector<Enemy*> row_enemies; 
		for (int i = 0; i < enemies_per_row; ++i) { 
			float x = (canvas_width / 2.0f) - 2.4f + (i * gap); 
			float y = m_state->getCanvasHeight() - 0.8f - (row * row_space); 
			Enemy* enemy = new Enemy("EnemyBottom", false);
			enemy->init(x, y);
			m_enemies.push_back(enemy); 
			row_enemies.push_back(enemy);
		}
		m_rows.push_back(row_enemies); 
	}
}

void Level::setDifficulty(int level_number) {
	//se kathe level 1 neos enemy se kathe row
	enemies_per_row = 7 + (level_number - 1);

	// barane ligo pio fast
	m_enemy_shoot_interval = std::max(500.0f, m_enemy_shoot_interval - 100.0f * (level_number - 1));

	std::cout << "Difficulty adjusted: Enemies per row = " << enemies_per_row
		<< ", Shoot interval = " << m_enemy_shoot_interval << "ms" << std::endl;
}


void Level::moveEnemies() {
	float canvas_width = m_state->getCanvasWidth();

	for (auto& row : m_rows) { 
		bool row_collision = false;

		
		for (Enemy* enemy : row) {
			if (!enemy->isActive()) continue;

			
			if (enemy->m_pos_x <= 0.3f || enemy->m_pos_x >= canvas_width - 0.3f) {
				row_collision = true;
				break;
			}
		}

		
		if (row_collision) {
			for (Enemy* enemy : row) {
				if (!enemy->isActive()) continue;

				
				float vertical_step = 0.2f; 
				enemy->m_pos_y += (enemy->isTopRow() ? vertical_step : -vertical_step);

				
				float canvas_height = m_state->getCanvasHeight();
				enemy->m_pos_y = std::max(0.3f, std::min(enemy->m_pos_y, canvas_height - 0.3f));
			}

			for (Enemy* enemy : row) {
				enemy->setGoingRight(!enemy->isGoingRight());
			}
		}

		for (Enemy* enemy : row) {
			if (!enemy->isActive()) continue;

			float horizontal_step = 0.3f; 
			enemy->m_pos_x += (enemy->isGoingRight() ? horizontal_step : -horizontal_step);

			enemy->m_pos_x = std::max(0.3f, std::min(enemy->m_pos_x, canvas_width - 0.3f));
		}
	}
}
void Level::checkEnemyPositions() {
	if (m_state->getCurrentState() == GameState::GAME_OVER) {
		return;
	}

	float canvas_middle = m_state->getCanvasHeight() / 2.0f;
	float tolerance = 0.3f; 

	for (Enemy* enemy : m_enemies) {
		
		if (enemy->getY() >= canvas_middle - tolerance && enemy->getY() <= canvas_middle + tolerance) {
			m_state->setGameOver();
			return;
		}
	}
}

void Level::spawnBullet(float x, float y, float speed, bool is_player_bullet, bool direction_up)
{
	std::string bullet_texture;
	if (is_player_bullet) {
		bullet_texture = direction_up ? "player_bullet_top.png" : "player_bullet_down.png";//mporousa kai sto bullet alla klain tr
	}
	else {
		bullet_texture = direction_up ? "enemy_bullet_top.png" : "enemy_bullet_down.png";
	}

	
	Bullet* bullet = new Bullet(is_player_bullet, direction_up);
	bullet->init(x, y, speed, bullet_texture);

	
	m_bullets.push_back(bullet);
}

void Level::handleBulletCollisions()
{
	for (Bullet* bullet : m_bullets) {
		if (!bullet->isActive()) continue;

		if (bullet->isPlayerBullet()) {
			
			for (Enemy* enemy : m_enemies) {
				
				if (enemy->isActive() && enemy->collidesWith(*bullet)) {
					bullet->setActive(false);
					enemy->setActive(false);
					graphics::playSound(m_state->getAssetPath("enemy_hit.wav"), 0.7f);

					break;
				}
			}

			for (Box& obstacle : m_blocks) {
				if (bullet->collidesWith(obstacle)) {
					bullet->setActive(false);
				}
			}
		}
		else {
			
			Player* player = m_state->getPlayer();
			if (player && player->isActive() && player->collidesWith(*bullet)) {
				std::cout << "Player hit by bullet at (" << bullet->getX() << ", " << bullet->getY() << ")" << std::endl;
				bullet->setActive(false);   // delete tin sfera
				player->loseLife();     // -1 zoi
				graphics::playSound(m_state->getAssetPath("player_hit.mp3"), 0.6f);
				break;
			}

			for (Box& obstacle : m_blocks) {
				if (bullet->collidesWith(obstacle)) {
					bullet->setActive(false);
				}
			}
		}
	}

	
	m_bullets.erase(std::remove_if(m_bullets.begin(), m_bullets.end(),
		[](Bullet* b) { return !b->isActive(); }),
		m_bullets.end());

	m_enemies.erase(std::remove_if(m_enemies.begin(), m_enemies.end(),
		[](Enemy* e) { return !e->isActive(); }),
		m_enemies.end());
}


void Level::shootEnemyBullets() {
	
	Enemy* top_enemy = getRandomActiveEnemy(true);
	if (top_enemy) {
		float bullet_x = top_enemy->getX();
		float bullet_y = top_enemy->getY() + 0.3f; 
		spawnBullet(bullet_x, bullet_y, 0.4f, false, false); 
		graphics::playSound(m_state->getAssetPath("enemy_shot.mp3"), 0.5f);
	}

	
	Enemy* bottom_enemy = getRandomActiveEnemy(false);
	if (bottom_enemy) {
		float bullet_x = bottom_enemy->getX();
		float bullet_y = bottom_enemy->getY() - 0.3f; 
		spawnBullet(bullet_x, bullet_y, 0.4f, false, true); 
		graphics::playSound(m_state->getAssetPath("enemy_shot.mp3"), 0.5f);
	}
}

void Level::drawStartMenu()
{
	graphics::Brush bg_brush;
	bg_brush.fill_opacity = 1.0f; 
	bg_brush.texture = GameState::getInstance()->getAssetPath("start.png");

	graphics::drawRect(m_state->getCanvasWidth() / 2.0f, m_state->getCanvasHeight() / 2.0f,
		m_state->getCanvasWidth(), m_state->getCanvasHeight(), bg_brush);

	graphics::Brush brush;
	float canvas_width = m_state->getCanvasWidth();
	float canvas_height = m_state->getCanvasHeight();

	// Mouse state
	graphics::MouseState mouse_state;
	graphics::getMouseState(mouse_state);

	float mx = mouse_state.cur_pos_x * canvas_width / 800.0f;
	float my = mouse_state.cur_pos_y * canvas_height / 800.0f;

	//buttons
	float button_width = 1.5f;
	float button_height = 0.6f;

	// Start 
	float start_x = canvas_width / 2.0f;
	float start_y = canvas_height / 2.0f - 0.2f;

	// Quit 
	float quit_x = canvas_width / 2.0f;
	float quit_y = canvas_height / 2.0f + 0.7f;

	// gia hover na alalzei xroma
	bool is_start_hovered = mx >= start_x - button_width / 2 &&
		mx <= start_x + button_width / 2 &&
		my >= start_y - button_height / 2 &&
		my <= start_y + button_height / 2;

	bool is_quit_hovered = mx >= quit_x - button_width / 2 &&
		mx <= quit_x + button_width / 2 &&
		my >= quit_y - button_height / 2 &&
		my <= quit_y + button_height / 2;

	
	if (is_start_hovered) {
		SETCOLOR(brush.fill_color, 0.0f, 1.0f, 0.0f);
	}
	else {
		SETCOLOR(brush.fill_color, 0.0f, 0.0f, 0.0f);
	}
	graphics::drawText(start_x - 0.6f, start_y, 0.5f, "Start", brush);

	// Draw "Quit" button
	if (is_quit_hovered) {
		SETCOLOR(brush.fill_color, 1.0f, 0.0f, 0.0f);
	}
	else {
		SETCOLOR(brush.fill_color, 0.0f, 0.0f, 0.0f);
	}
	graphics::drawText(quit_x - 0.6f, quit_y, 0.5f, "Quit", brush);
}


void Level::drawGameOverScrn()
{
	graphics::Brush bg_brush;
	bg_brush.fill_opacity = 1.0f; 
	bg_brush.texture = m_state->getAssetPath("game_over.png");
	graphics::drawRect(m_state->getCanvasWidth() / 2.0f, m_state->getCanvasHeight() / 2.0f,
		m_state->getCanvasWidth(), m_state->getCanvasHeight(), bg_brush);
	

	graphics::Brush brush;
	float canvas_width = m_state->getCanvasWidth();
	float canvas_height = m_state->getCanvasHeight();

	// Mouse state
	graphics::MouseState mouse_state;
	graphics::getMouseState(mouse_state);

	float mx = mouse_state.cur_pos_x * canvas_width / 800.0f;  // Scale mouse X to canvas
	float my = mouse_state.cur_pos_y * canvas_height / 800.0f; // Scale mouse Y to canvas

	// Button dimensions
	float button_width = 1.0f;
	float button_height = 0.5f;

	// Restart button
	float restart_x = canvas_width / 2.0f;
	float restart_y = canvas_height / 2.0f + 0.2f;

	// Quit button
	float quit_x = canvas_width / 2.0f;
	float quit_y = canvas_height / 2.0f + 0.8f;

	// Hover states
	bool is_restart_hovered = mx >= restart_x - button_width / 2 &&
		mx <= restart_x + button_width / 2 &&
		my >= restart_y - button_height / 2 &&
		my <= restart_y + button_height / 2;

	bool is_quit_hovered = mx >= quit_x - button_width / 2 &&
		mx <= quit_x + button_width / 2 &&
		my >= quit_y - button_height / 2 &&
		my <= quit_y + button_height / 2;

	

	// Draw Restart button
	if (is_restart_hovered) {
		SETCOLOR(brush.fill_color, 0.0f, 1.0f, 0.0f);
	}
	else {
		SETCOLOR(brush.fill_color, 0.0f, 0.0f, 0.0f);
	}
	graphics::drawText(restart_x - 0.4f, restart_y, 0.4f, "Restart", brush);

	// Draw Quit button
	if (is_quit_hovered) {
		SETCOLOR(brush.fill_color, 1.0f, 0.0f, 0.0f);
	}
	else {
		SETCOLOR(brush.fill_color, 0.0f, 0.0f, 0.0f);
	}
	graphics::drawText(quit_x - 0.4f, quit_y, 0.4f, "Quit", brush);

	
}



void Level::handleMouse() {

	if (m_state->getCurrentState() == GameState::START) {

		graphics::MouseState mouse_state;
		graphics::getMouseState(mouse_state);

		const float w = m_state->getCanvasWidth();
		const float h = m_state->getCanvasHeight();

		float mx = mouse_state.cur_pos_x * w / 800.0f; // Scale mouse X
		float my = mouse_state.cur_pos_y * h / 800.0f; // Scale mouse Y
		float button_width = 1.5f;
		float button_height = 0.6f;

		// Start 
		float start_x = w / 2.0f;
		float start_y = h / 2.0f - 0.5f;

		// Quit 
		float quit_x = w / 2.0f;
		float quit_y = h / 2.0f + 0.5f;

		if (mouse_state.button_left_pressed) {
			if (mx >= start_x - button_width / 2 && mx <= start_x + button_width / 2 &&
				my >= start_y - button_height / 2 && my <= start_y + button_height / 2) {
				graphics::playSound(m_state->getAssetPath("click.mp3"), 0.7f); 
				m_state->setRunning();
			}

			if (mx >= quit_x - button_width / 2 && mx <= quit_x + button_width / 2 &&
				my >= quit_y - button_height / 2 && my <= quit_y + button_height / 2) {
				graphics::playSound(m_state->getAssetPath("click.mp3"), 0.7f);
				graphics::stopMessageLoop();
			}
		}
	}

	if (m_state->getCurrentState() == GameState::GAME_OVER) {
		
		graphics::MouseState mouse_state;
		graphics::getMouseState(mouse_state);

		const float w = m_state->getCanvasWidth();
		const float h = m_state->getCanvasHeight();

		float mx = mouse_state.cur_pos_x * w / 800.0f; // Scale mouse X
		float my = mouse_state.cur_pos_y * h / 800.0f; // Scale mouse Y
		float button_width = 1.5f;
		float button_height = 0.6f;


		
		float restart_x = w / 2.0f;
		float restart_y = h / 2.0f;
		float restart_width = 1.0f;
		float restart_height = 0.5f;

		
		float quit_x = w / 2.0f;
		float quit_y = h / 2.0f + 0.7f;
		float quit_width = 1.0f;
		float quit_height = 0.5f;

		
		if (mouse_state.button_left_pressed) {
			if (mx >= restart_x - restart_width / 2 && mx <= restart_x + restart_width / 2 &&
				my >= restart_y - restart_height / 2 && my <= restart_y + restart_height / 2) {
				graphics::playSound(m_state->getAssetPath("click.mp3"), 0.7f);
				m_state->resetGame();
			}

			if (mx >= quit_x - quit_width / 2 && mx <= quit_x + quit_width / 2 &&
				my >= quit_y - quit_height / 2 && my <= quit_y + quit_height / 2) {
				graphics::playSound(m_state->getAssetPath("click.mp3"), 0.7f);
				graphics::stopMessageLoop();
			}
		}
	}
}


void Level::resetEntities() {

	for (Bullet* bullet : m_bullets) {
		delete bullet;
	}
	m_bullets.clear();


	for (Enemy* enemy : m_enemies) {
		delete enemy;
	}
	m_enemies.clear();

	spawnEnemies();
}
	

bool Level::isComplete() const {
	return m_enemies.empty(); 
}



Level::Level(const std::string& name) : m_level_name(name) {
	std::cout << "Level created: " << m_level_name << std::endl;
}


Level::~Level()
{
	for (auto p_gob : m_static_object)
		if (p_gob) delete p_gob;
	for (auto p_gob : m_dynamic_object)
		if (p_gob) delete p_gob;
}
