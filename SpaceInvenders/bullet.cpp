#include "bullet.h"
#include "gamestate.h"
#include <iostream>

Bullet::Bullet(bool is_player_bullet, bool direction_up)
    : m_is_player_bullet(is_player_bullet), m_direction_up(direction_up), m_speed(0.0f) {
    m_active = false; 
}

void Bullet::init(float x, float y, float speed, const std::string& texture) {
    m_pos_x = x;
    m_pos_y = y;
    m_width = 0.1f;  
    m_height = 0.2f; 
    m_speed = (m_direction_up ? -speed : speed); 
    m_active = true; 

    m_brush_bullet.texture = GameState::getInstance()->getAssetPath(texture);
    m_brush_bullet.fill_opacity = 1.0f;
    m_brush_bullet.outline_opacity = 0.0f;

    
}

    
   

void Bullet::update(float dt) {
    if (!m_active) return;

    
    float scaled_dt = dt / 1000.0f;

    m_pos_y += m_speed * scaled_dt;

    if (m_pos_y < 0 || m_pos_y > GameState::getInstance()->getCanvasHeight()) {
        m_active = false;
    }

   
}

    



void Bullet::draw() {
    if (!m_active) return;

    graphics::drawRect(m_pos_x, m_pos_y, 0.1f, 0.2f, m_brush_bullet);

    
}



bool Bullet::isOffScreen() const {
    float canvas_height = GameState::getInstance()->getCanvasHeight();
    if (m_pos_y < 0.0f || m_pos_y > canvas_height) {
      
        return true;
    }
    return false;
}

bool Bullet::collidesWith(const Box& other) const
{
    
    float this_left = m_pos_x - m_width / 2.0f;
    float this_right = m_pos_x + m_width / 2.0f;
    float this_top = m_pos_y - m_height / 2.0f;
    float this_bottom = m_pos_y + m_height / 2.0f;

    
    float other_left = other.m_pos_x - other.m_width / 2.0f;
    float other_right = other.m_pos_x + other.m_width / 2.0f;
    float other_top = other.m_pos_y - other.m_height / 2.0f;
    float other_bottom = other.m_pos_y + other.m_height / 2.0f;

    

    const float epsilon = 0.01f;
    return !(this_right < other_left - epsilon || this_left > other_right + epsilon ||
        this_bottom < other_top - epsilon || this_top > other_bottom + epsilon);

}





