#pragma once
#include "gameobject.h"
#include "box.h"

class Bullet : public GameObject, public Box {
    graphics::Brush m_brush_bullet;
    float m_speed;        
    bool m_is_player_bullet; 
    bool m_direction_up;  
    float m_width = 0.1f;  
    float m_height = 0.2f; 

public:
    Bullet(bool is_player_bullet, bool direction_up);

    float getX() const { return m_pos_x; }
    float getY() const { return m_pos_y; }

    void update(float dt) override;
    void draw() override;
    void init(float x, float y, float speed, const std::string& texture) ;
    bool isOffScreen() const; 
    bool isPlayerBullet() const { return m_is_player_bullet; } 
    bool isDirectionUp() const { return m_direction_up; }     
    bool collidesWith(const Box& other) const;
};
