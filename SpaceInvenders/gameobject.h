#pragma once
#include "sgg/graphics.h"
#include <string>

class GameObject {
protected:
    class GameState* m_state;
    std::string m_name;
    bool m_active = true;
    int m_id; 
    static int m_next_id; 
     

   
public:
   
    
    GameObject(const std::string& name = "");
    virtual void update(float dt) {};
    virtual void init() {}
    virtual void draw() {}
    virtual ~GameObject() {}

   

    bool isActive() const { return m_active; }
    void setActive(bool active) { m_active = active; }

  
};