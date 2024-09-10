#pragma once
#include <iostream>
#include <gl/glut.h>
#include <vector>
#include <chrono>
#include <thread>
#include <string>
#include <map>
#include <queue>
#include<windows.h> 

#define FieldSizeX 15
#define FieldSizeY 10
#define CellSize 50
#define FireballSpeed 0.25

using namespace std;
using namespace std::chrono;



struct Point {
    int x, y;

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }

    bool operator<(const Point& other) const {
        if (x == other.x) {
            return y < other.y;
        }
        return x < other.x;
    }

    bool operator!=(const Point& other) const {
        return x != other.x or y != other.y;
    }
};

struct Color {
    float r, g, b;
};

class Cell {
public:
    Point AbsolutePosition;
    Point FieldPosition;
    bool IsEmerald = false;
    bool IsGoldBag = false;
    bool IsNobin = false;
    bool IsLand = false;
    bool IsPath = false;
    bool IsPlayer = false;

    void Draw(Color color);
};

class GameObject {
public:
    bool IsAlive;
    GLuint texture;
    int textureWidth, textureHeight;
    float posX, posY; // Position of the texture
    float angle; // Rotation angle of the texture
    Point FieldPos;

    GameObject() : posX(23.0f), posY(23.0f), angle(0.0f), texture(0), textureWidth(0), textureHeight(0), FieldPos{ 0,0 } {}

    GLuint loadTexture(const std::string& filename);

    void Draw(const string& file, bool NeedToLoad = false);
};



class FireBall {
public:
    Point FieldPos;
    float posX, posY;
    bool isActive;
    float direction; 

    FireBall(Point startPos, float dir) : FieldPos(startPos), posX(startPos.x* CellSize), posY(startPos.y* CellSize), isActive(true), direction(dir) {}

    void Move();

    void Draw();
};


class GoldBag:public GameObject {
public:

    Cell cell;  
    bool RightDirection;
    bool LeftDirection;
    bool IsBroken;
    bool isFalling;
    bool NeedToLoadBroken;

    GoldBag(const Cell& cell);
};

class Emerald:public GameObject{
public:
    Cell cell;  

    Emerald(const Cell& cell);

    void Draw() { GameObject::Draw("emerald.png"); }

};

class GameLevel;

class Nobbin : public GameObject {

public:
    Point curpos;
    vector<Point> path;

    Nobbin(float x, float y, int X, int Y);

    void BFSPathfinding(Point target, GameLevel& level);

    void die() { delete this;}

};

class Game;

class Player : public GameObject {
public:
    int Emeralds;
    bool IsAlive;
    Point curpos;
    vector<FireBall> fireballs;
    int lives;  
    int goldbags;
    int fireball_numb;

    Player() : lives(3) {}  

    void PlayerDeath(Game& game);

    void ResumeAfterDeath(Game& game);

    void Shoot();

    void UpdateFireballs(vector<Cell>& earth, vector<Nobbin>& nobbins);

    void DrawFireballs();



};

class GameLevel {
private:

    Color pathColor = { 0, 0, 0 };
    Color earthColor = { 139, 69, 19 };

public:

    vector<Cell> Earth;
    vector<Cell> Path;
    vector<Emerald> Emeralds;
    vector<GoldBag> GoldBags;
    vector<Nobbin> Nobbins;
    int CurrentNumberOfGoldBags;
    int CurrentNumberOfEmeralds;

    void DrawLevel();

    void InitializeLevel();
};

class Game {
private:
    unsigned int lastMoveTime = 0;  // Переменная для отслеживания времени последнего движения
    const unsigned int moveDelay = 200; // Задержка в миллисекундах между перемещениями
public:
    GameLevel level;
    static Game* instance;
    Player player;
    vector<class Nobbin> enemies;
    bool NobbinsCreated;

    static void ResumeAfterDeathWrapper(int value) { instance->player.ResumeAfterDeath(*instance); }

    static void DisplayWrapper() { instance->display(); }

    static void KeyWrapper(unsigned char key, int x, int y) { instance->handleKey(key, x, y); }

    void CheckConditions();

    void handleKey(unsigned char key, int x, int y);

    bool IsPositionOccupiedByNobbin(Point position);

    void ChangePositionForNobbin();

    void MoveNobbin(int value);

    void FallingGoldBags(int value);

    void CreateNobbin(int index);

    void timer(int value);

    static void MoveNobbinWrapper(int value) { instance->MoveNobbin(value);}

    static void CreateNobbinWrapper(int value) { instance->CreateNobbin(value);}
    
    static void FallingGoldBagsWrapper(int value) { instance->FallingGoldBags(value);}

    static void TimerWrapper(int value) { instance->timer(value);}

    void display();

    void setupCoordinateSystem();

    void Run(int argc, char* argv[]);
};
