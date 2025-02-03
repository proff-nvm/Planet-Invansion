#pragma once
#include "gameobject.h"
#include "box.h"
#include "sgg/graphics.h"
#include <vector>
#include <memory>
#include "bullet.h"

enum Direction { UP, DOWN };

class Player : public GameObject, public Box {

    bool m_player_active = true;
    graphics::Brush m_brush_player;
    Direction m_direction = UP;
    std::vector<Bullet*> m_bullets;

private:

    float m_shoot_cooldown = 0.8f; // 0.8 sec to space
    float m_last_shot_time = 0.0f; 
    int m_lives;

public:
    Player(std::string name) : GameObject(name) {}
    
    void update(float dt) override;
    void init() override;
    void draw() override;

    void shoot(std::vector<Bullet*>& level_bullets);
    void setShootCooldown(float cooldown); // tha balo powerup oste na meionete
    void updateBullets(float dt);
    void drawBullets();
    bool collidesWith(const Box& other) const;
    void loseLife();
    int getLives() const; // getter

    void drawLives() const;

    void deactivate();       
    bool isActive() const;   


    void setDirection(Direction dir) { m_direction = dir; }
    Direction getDirection() const { return m_direction; }
};