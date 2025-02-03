#include "player.h"
#include "gamestate.h"
#include "sgg/graphics.h"
#include <iostream>



void Player::update(float dt) {
    
    float delta_time = dt / 1000.0f;
    const float velocity = 3.0f;

    if (graphics::getKeyState(graphics::SCANCODE_LEFT) || graphics::getKeyState(graphics::SCANCODE_A)) {
        m_pos_x -= delta_time * velocity;
    }
    if (graphics::getKeyState(graphics::SCANCODE_RIGHT) || graphics::getKeyState(graphics::SCANCODE_D)) {
        m_pos_x += delta_time * velocity;
    }
    if (graphics::getKeyState(graphics::SCANCODE_W) || graphics::getKeyState(graphics::SCANCODE_UP)) {
        setDirection(UP);
    }
    if (graphics::getKeyState(graphics::SCANCODE_S) || graphics::getKeyState(graphics::SCANCODE_DOWN)) {
        setDirection(DOWN);
    }

    //m_state->m_global_offset_x = m_state->getCanvasWidth() / 2.0f - m_pos_x;
   // m_state->m_global_offset_y = m_state->getCanvasHeight() / 2.0f - m_pos_y;

    float canvas_width = m_state->getCanvasWidth();
    float canvas_height = m_state->getCanvasHeight();

    

    // Update the bounding box
    m_pos_x = std::max(0.1f, std::min(m_pos_x, canvas_width - 0.1f));
    m_pos_y = std::max(0.1f, std::min(m_pos_y, canvas_height - 0.1f));
}

void Player::init()
{
    m_pos_x = m_state->getCanvasWidth() / 2.0f;
    m_pos_y = m_state->getCanvasHeight() /2.0f ;

    m_width = 0.2f;  
    m_height = 0.3f; 

    m_player_active = true;
    m_lives = 3; 
    

    m_brush_player.fill_opacity = 1.0f;
    m_brush_player.outline_opacity = 0.0f;
    m_brush_player.texture = m_state->getAssetPath("player_up.png");

}


void Player::draw() {
    float center_x = m_state->getCanvasWidth() * 0.5;
    float center_y = m_state->getCanvasHeight() * 0.5;

    
    if (m_direction == UP) {
        m_brush_player.texture = m_state->getAssetPath("player_up.png");
    }
    else {
        m_brush_player.texture = m_state->getAssetPath("player_down.png");
    }

    graphics::drawRect(m_pos_x, m_pos_y, 0.2f, 0.3f, m_brush_player);

    

   


}

void Player::shoot(std::vector<Bullet*>& level_bullets) {
    float current_time = graphics::getGlobalTime() / 1000.0f;

    if (graphics::getKeyState(graphics::SCANCODE_SPACE)) {
        if (current_time - m_last_shot_time >= m_shoot_cooldown) {
            std::string bullet_texture = (m_direction == UP) ?
                "player_bullet_top.png" : "player_bullet_down.png";

            float bullet_speed = 1.0f;
            float bullet_x = m_pos_x;
            float bullet_y = m_pos_y + (m_direction == UP ? -0.3f : 0.3f);

            Bullet* bullet = new Bullet(true, m_direction == UP);
            bullet->init(bullet_x, bullet_y, bullet_speed, bullet_texture);

            level_bullets.push_back(bullet);
            graphics::playSound(m_state->getAssetPath("shot.wav"), 0.6f);


            m_last_shot_time = current_time;
            
        }
    }
}


void Player::setShootCooldown(float cooldown)
{
    m_shoot_cooldown = cooldown;
   
}


void Player::updateBullets(float dt) {
    for (auto it = m_bullets.begin(); it != m_bullets.end();) {
        (*it)->update(dt);
        if (!(*it)->isActive()) {
            delete* it;
            it = m_bullets.erase(it); 
        }
        else {
            ++it;
        }
    }
}

void Player::drawBullets() {
    for (Bullet* bullet : m_bullets) {
        bullet->draw();
    }
}

bool Player::collidesWith(const Box& other) const {
    float player_half_width = m_width * 0.8f/ 2.0f;
    float player_half_height = m_height * 0.8f / 2.0f;

    float other_half_width = other.m_width / 2.0f;
    float other_half_height = other.m_height / 2.0f;

    bool collision = !(m_pos_x + player_half_width < other.m_pos_x - other_half_width ||
        m_pos_x - player_half_width > other.m_pos_x + other_half_width ||
        m_pos_y + player_half_height < other.m_pos_y - other_half_height ||
        m_pos_y - player_half_height > other.m_pos_y + other_half_height);

    

    return collision;
}

void Player::loseLife()
{
    if (m_lives > 0) {
        --m_lives;
        
    }

    // eixame player (prepei na mpei ena gameoverscreen mallon)
    if (m_lives == 0 && m_state->getCurrentState() != GameState::GAME_OVER) {
        m_state->setGameOver(); // Notify game over
        
    }
    
}

int Player::getLives() const {
    return m_lives;
}

void Player::drawLives() const {
    for (int i = 0; i < m_lives; ++i) {
        float x = 0.2f + i * 0.3f; 
        float y = 0.1f; 
        graphics::Brush brush;
        brush.texture = m_state->getAssetPath("heart.png"); 
        graphics::drawRect(x, y, 0.2f, 0.2f, brush);
    }
}




void Player::deactivate()
{
    m_player_active = false;
}

bool Player::isActive() const
{
    return m_player_active;
}
