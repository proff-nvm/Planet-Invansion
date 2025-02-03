#include "gamestate.h"
#include "level.h"
#include "player.h"
#include <iostream>
#include "configurations.h"
#include <thread>
#include <chrono>

GameState* GameState::m_unique_instance = nullptr;

GameState* GameState::getInstance() {
    if (!m_unique_instance) {
        m_unique_instance = new GameState();
    }
    return m_unique_instance;
}

GameState::GameState() {

    
}

void GameState::init() {
    m_current_level = new Level("level1");
    m_current_level->init();


    m_player = new Player("Player");
    m_player->init();

    graphics::playMusic(getAssetPath("bg_music.mp3"), 0.5f, true); 

    graphics::preloadBitmaps(getAssetDir());

}

void GameState::update(float dt) {

    if(dt > 500) 
        return;

    float sleep_time = std::max(0.0f, 20.0f - dt);
    std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(sleep_time)));

    if (!m_current_level)
        return;

    
    if (m_current_state == START || m_current_state == GAME_OVER) {
        m_current_level->handleMouse();
        return;
    }

    if (m_current_state == RUNNING) {
        m_current_level->update(dt);

        if (m_current_level->isComplete()) {
            nextLevel();
        }
    }
}

void GameState::draw() {
    if (!m_current_level) return;

    if (m_current_state == START) {
        
        m_current_level->drawStartMenu();
        return;
    }

    if (m_current_state == RUNNING) {
        
        m_current_level->draw();

        if (m_player && m_player->isActive()) {
            m_player->draw();
            m_player->drawLives();
        }
    }

    if (m_current_state == GAME_OVER) {
        m_current_level->drawGameOverScrn();
    }

    if (m_current_state == WINNING) {
        drawWinningScreen();
        return;
    }
}



GameState::~GameState() {
    if (m_player) {
        delete m_player;
        m_player = nullptr;
    }
    if (m_current_level) {
        delete m_current_level;
        m_current_level = nullptr;
    }
}

std::string GameState::getAssetDir()
{
    return m_asset_path;
}

std::string GameState::getAssetPath(const std::string& asset)
{
    return m_asset_path + asset;
}

void GameState::resetPlayer() {
    Player* player = getPlayer();
    if (player) {
        player->init(); 
        player->isActive(); 
        
    }
}

void GameState::setRunning()
{
    m_current_state = RUNNING;
    
}


void GameState::setGameOver() {
    if (m_current_state == GAME_OVER) return; 
    m_current_state = GAME_OVER;
   
}

void GameState::nextLevel() {
    if (m_current_level) {
        delete m_current_level; 
        m_current_level = nullptr; 
    }
    if (m_level_number < m_max_level) {
        ++m_level_number;
        m_current_level = new Level("level" + std::to_string(m_level_number));
        m_current_level->init();
        
    }
    else {
        m_current_state = WINNING;
        
    }
}

void GameState::drawWinningScreen()
{
    graphics::Brush bg_brush;
    bg_brush.fill_opacity = 1.0f;
    bg_brush.texture = getAssetPath("winner.png");

    float canvas_width = getCanvasWidth();
    float canvas_height = getCanvasHeight();

    graphics::drawRect(canvas_width / 2.0f, canvas_height / 2.0f,
        canvas_width, canvas_height, bg_brush);

}


void GameState::resetGame() {
    resetPlayer();

    m_level_number = 1;

    
    if (m_current_level) {
        delete m_current_level;
    }

    
    m_current_level = new Level("level1");
    m_current_level->init();
    ;

    m_current_state = RUNNING; 
    
}