#pragma once
#include "gameobject.h"
#include "box.h"
#include "sgg/graphics.h"

class Enemy : public GameObject, public Box {
    graphics::Brush m_brush_enemy;
    bool m_going_right; // True an kanei move  right, false an paei left
    bool m_is_top_row;  // True an einai sto  top row, false an einai bot

public:
    Enemy(std::string name, bool is_top_row)
        : GameObject(name), m_is_top_row(is_top_row), m_going_right(true) {
    }

    void update(float dt) override;
    void init(float x, float y) ;
    void move();
    void draw() override;
    void deactivate();
    bool isActive() const;
    float getX() const { return m_pos_x; }
    float getY() const { return m_pos_y; }
    bool collidesWith(const Box& other) const; // bazo diko tou collision oste otan pane sti mesi tou canva na ginei gameover
    

    //accessor gia tis private pano

    bool isTopRow() const { return m_is_top_row; }
    void setTopRow(bool is_top) { m_is_top_row = is_top; }

    bool isGoingRight() const { return m_going_right; }
    void setGoingRight(bool going_right) { m_going_right = going_right; }
};
