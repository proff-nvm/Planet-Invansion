#include "gameobject.h"
#include "player.h"
#include "enemy.h"
#include <string>
#include "sgg/graphics.h"

class GameState {

public :
    enum State {START, RUNNING, GAME_OVER , WINNING};


private:
    static GameState* m_unique_instance;
    std::string m_asset_path = "assets/";
    float m_canvas_width = 6.0f;
    float m_canvas_height = 6.0f;


    GameState();


    class Player* m_player = nullptr;
    class Level* m_current_level = nullptr;
    int m_level_number = 1; 
    const int m_max_level = 3;

    State m_current_state = START;

    bool start_menu_drawn = false; 





public:
    float m_global_offset_x = 5.0f;
    float m_global_offset_y = 5.0f;
    static GameState* getInstance();
    void init();
    void update(float dt);
    void draw();
    ~GameState();

    Level* getLevel() const { return m_current_level; }

    float getCanvasWidth() { return m_canvas_width; }
    float getCanvasHeight() { return m_canvas_height; }

    void setGameOver();
    void nextLevel();
    State getCurrentState() const { return m_current_state; }
    void drawWinningScreen();


    std::string getAssetDir();
    std::string getAssetPath(const std::string& asset);

    void resetPlayer();

    void setRunning();

    void resetGame();

    class Player* getPlayer() { return m_player; }
};

