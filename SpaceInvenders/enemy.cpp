#include "enemy.h"
#include "gamestate.h"
#include <iostream>

void Enemy::init(float x, float y) {
    m_pos_x = x;
    m_pos_y = y;
    m_active = true;

    m_width = 0.3f;  
    m_height = 0.2f; 



    m_brush_enemy.fill_opacity = 1.0f;
    m_brush_enemy.outline_opacity = 0.0f;

    if (m_is_top_row) {
        m_brush_enemy.texture = "assets/enemy_1_front.png"; 
    }
    else {
        m_brush_enemy.texture = "assets/enemy_1_back.png"; 
    }
}

void Enemy::move() {
    float horizontal_step = 0.3f;
    float vertical_step = 0.2f;

    // Horizontal movement
    if (m_going_right) {
        m_pos_x += horizontal_step;
    }
    else {
        m_pos_x -= horizontal_step;
    }

   
    float canvas_width = GameState::getInstance()->getCanvasWidth();
    m_pos_x = std::max(0.3f, std::min(m_pos_x, canvas_width - 0.3f));
}

void Enemy::update(float dt) {
    
}

void Enemy::draw() {

    graphics::drawRect(m_pos_x, m_pos_y, 0.3f, 0.2f, m_brush_enemy);
   

}

void Enemy::deactivate()
{
    m_active = false;
}

bool Enemy::isActive() const
{
    return m_active;
}

bool Enemy::collidesWith(const Box& other) const {
    float enemy_half_width = m_width / 2.0f;
    float enemy_half_height = m_height / 2.0f;

    float other_half_width = other.m_width / 2.0f;
    float other_half_height = other.m_height / 2.0f;

    return !(m_pos_x + enemy_half_width < other.m_pos_x - other_half_width ||
        m_pos_x - enemy_half_width > other.m_pos_x + other_half_width ||
        m_pos_y + enemy_half_height < other.m_pos_y - other_half_height ||
        m_pos_y - enemy_half_height > other.m_pos_y + other_half_height);
}

