module;

import std;

import collision;

import <SDL.h>;
import <SDL_mixer.h>;

using namespace std::literals::chrono_literals;

export module player;

export class Player : public Point
{
    //Threads
    std::thread* vertVeloThrd;
    std::thread* walkThrd;
    std::thread* plrAnimThrd;

    //Audio
    Mix_Chunk* gJump; //Freed later 
    Mix_Chunk* gDeath; //Freed later 
    Mix_Chunk* gWin; //Freed later

    Collision* col;

    //Variables
    //Ground
    int dir;
    bool grounded = true;
    //Walljumping
    bool wallAnimLock = false;
    bool wallAnim = false;
    int lastDir = 0;

    void JumpWithSpeed(int n, std::chrono::duration<double,std::ratio<1,1000>> t, std::mutex& mtx, bool& quit, bool& holdJump, bool cancelable)    
    {
        bool temp = false;
        for (int i = 0; i < n; i++)
        {
            if (!jumping && !quit && !die && !win)
            {
                if (col->CheckCollision(x, y, 0, dir, 16, 32) && !(cancelable && !holdJump))
                {
                    y += dir;
                }
                else
                {
                    break;
                }
                if (wallJumping && col->CheckCollision(x, y, walldir, 0, 16, 32))
                {
                    if (run)
                    {
                        x += walldir;
                    }
                    else
                    {
                        if (temp)
                        {
                           x += walldir;
                        }
                        temp = !temp;
                    }
                }
                std::this_thread::sleep_for(t);
            }
            mtx.lock(); mtx.unlock();
        }
    }
    void VertVelo(bool& quit, bool& holdjump, bool& pause, std::mutex& mtx)
    {
        while (!quit && !die && !win)
        {
            if (!pause)
            {
                if (jumping)
                {
                    Mix_PlayChannel(-1, gJump, 0);
                    jumping = false;
                    canJump = false;
                    canWallJump = false;
                    grounded = false;
                    wallAnimLock = true;
                    dir = 1;
                    mtx.lock(); mtx.unlock();
                    JumpWithSpeed(37, 1ms, mtx, quit, holdjump, true);
                    JumpWithSpeed(33, 2ms, mtx, quit, holdjump, true);
                    JumpWithSpeed(15, 4ms, mtx, quit, holdjump, true);
                    wallAnimLock = false;
                    JumpWithSpeed(10, 10ms, mtx, quit, holdjump, true);
                    JumpWithSpeed(2, 20ms, mtx, quit, holdjump, true);
                    wallJumping = false;
                    std::this_thread::sleep_for(100ms);
                    mtx.lock(); mtx.unlock();
                    dir = -1;
                    JumpWithSpeed(10, 10ms, mtx, quit, holdjump, false);
                    JumpWithSpeed(10, 4ms, mtx, quit, holdjump, false);
                    JumpWithSpeed(25, 2ms, mtx, quit, holdjump, false);
                    JumpWithSpeed(25, 1.5ms, mtx, quit, holdjump, false);
                    canJump = false;

                }
                else
                {
                    if (col->CheckCollision(x, y, 0, -1, 16, 32))
                    {
                        grounded = false;
                        if (canJump)
                        {
                            auto l = [&]() {std::this_thread::sleep_for(50ms); canJump = false; };
                            std::thread t(l);
                            t.detach();
                            dir = -1;
                            mtx.lock(); mtx.unlock();
                            JumpWithSpeed(10, 10ms, mtx, quit, holdjump, false);
                            JumpWithSpeed(10, 4ms, mtx, quit, holdjump, false);
                            JumpWithSpeed(25, 2ms, mtx, quit, holdjump, false);
                            JumpWithSpeed(25, 1.5ms, mtx, quit, holdjump, false);
                        }
                        if (col->CheckCollision(x, y, 0, -1, 16, 32))
                        {
                            y--;
                            std::this_thread::sleep_for(1ms);
                        }
                    }
                    else
                    {
                        canJump = true;
                        grounded = true;
                        lastDir = 1;
                        std::this_thread::sleep_for(1ms);
                    }
                }
            }
            else
            {
                std::this_thread::sleep_for(1ms);
            }
        }
    }
    void Walk(bool& quit, bool& pause, bool& resetlevel, bool& savetime)
    {
        while (!quit && !die && !win)
        {
            if (!pause)
            {
                if (canWalk && col->CheckCollision(x, y, horizdir, 0, 16, 32))
                {
                    if (!wallJumping)
                    {
                        x += horizdir;
                    }
                    else
                    {
                        if (run)
                        {
                            if (horizdir == walldir)
                            {
                                x += horizdir;
                            }
                        }
                        else
                        {
                            x += horizdir;
                        }
                    }
                    std::this_thread::sleep_for(4ms);
                    if (!run)
                    {
                        std::this_thread::sleep_for(2ms);
                    }
                }
                else
                {
                    for (int i = 1; i < 5; i++)
                    {
                        if (canWalk && grounded && col->CheckCollision(x, y, horizdir, i, 16, 32))
                        {
                            x += horizdir;
                            y += i;
                            std::this_thread::sleep_for(5ms);
                            if (!run)
                            {
                                std::this_thread::sleep_for(2ms);
                            }
                            break;
                        }
                    }
                    std::this_thread::sleep_for(1ms);
                }

                if (col->CheckCollision(x, y, 0, -1, 16, 32))
                {
                    if (!wallJumping)
                    {
                        if (!col->CheckCollision(x, y, -1, 0, 16, 32) && x > 0)
                        {
                            walldir = 1;
                            canWallJump = true;
                            if (horizdir == -walldir)
                            {
                                wallAnim = true;
                            }
                            else
                            {
                                wallAnim = false;
                            }
                        }
                        else if (!col->CheckCollision(x, y, 1, 0, 16, 32) && x < col->GetWidth() - 16)
                        {
                            walldir = -1;
                            canWallJump = true;
                            if (horizdir == -walldir)
                            {
                                wallAnim = true;
                            }
                            else
                            {
                                wallAnim = false;
                            }
                        }
                        else if (canWallJump)
                        {
                            auto l = [&]() {std::this_thread::sleep_for(50ms); canWallJump = false; };
                            std::thread t(l);
                            t.detach();
                            wallAnim = false;
                        }
                    }
                }
                else
                {
                    canWallJump = false;
                    wallAnim = false;
                }

                if (col->CheckGoal(x, y))
                {
                    win = true;
                    resetlevel = false;
                    savetime = true;
                }
                if (col->CheckHazard(x, y))
                {
                    die = true;
                }
            }
            else
            {
                std::this_thread::sleep_for(1ms);
            }
        }
    }
    void PlrAnim(SDL_Rect& a, bool& quit, bool& pause)
    {
        while (!quit)
        {
            if (!pause)
            {
                //Safeguard
                if (a.x > 1824)
                {
                    a.x = 0;
                }
                //Death
                if (die)
                {
                    if (a.x < 29 * 32 || a.x >= 43 * 32)//before first frame || after last frame
                    {
                        Mix_PlayChannel(-1, gDeath, 0);
                        a.x = 29 * 32;//first frame
                        std::this_thread::sleep_for(75ms);
                    }
                    else
                    {
                        a.x += 32;
                    }
                    std::this_thread::sleep_for(75ms);
                    if (a.x > 39 * 32)
                    {
                        std::this_thread::sleep_for(75ms);
                    }
                    if (a.x == 43 * 32)
                    {
                        a.x = 4 * 32;
                        quit = true;
                    }
                }
                //Goal
                else if (win)
                {
                    if (a.x < 45 * 32 || a.x >= 55 * 32)// before first frame || before last frame
                    {
                        Mix_PlayChannel(-1, gWin, 0);
                        a.x = 45 * 32;//first frame
                    }
                    else
                    {
                        a.x += 32;
                    }
                    std::this_thread::sleep_for(100ms);
                    if (a.x == 55 * 32)
                    {
                        a.x = 4 * 32;
                        quit = true;
                    }
                }
                //Idle
                else if (grounded && !canWalk)
                {
                    if (a.x >= 3 * 32)
                    {
                        a.x = 0;
                    }
                    else
                    {
                        a.x += 32;
                    }
                    std::this_thread::sleep_for(200ms);
                }
                //Walk
                else if (grounded && canWalk && !run)
                {
                    if (a.x < 5 * 32 || a.x >= 10 * 32)// before first frame || before last frame
                    {
                        a.x = 5 * 32;//first frame
                    }
                    else
                    {
                        a.x += 32;
                    }
                    std::this_thread::sleep_for(100ms);
                }
                //Run
                else if (grounded && canWalk && run)
                {
                    if (a.x < 12 * 32 || a.x >= 15 * 32)
                    {
                        a.x = 12 * 32;
                    }
                    else
                    {
                        a.x += 32;
                    }
                    std::this_thread::sleep_for(100ms);
                }
                //Wallcling
                else if (canWallJump && wallAnim && !canJump && !wallAnimLock)
                {
                    if (a.x < 26 * 32 || a.x >= 27 * 32)
                    {
                        a.x = 26 * 32;
                    }
                    else
                    {
                        a.x += 32;
                    }
                    std::this_thread::sleep_for(100ms);
                }
                //Jump Transition Frame
                else if (!grounded && dir != lastDir)
                {
                    a.x = 0;
                    lastDir = dir;
                    std::this_thread::sleep_for(50ms);
                }
                //Jump
                else if (!grounded && dir == 1)
                {
                    if (a.x < 17 * 32 || a.x >= 20 * 32)
                    {
                        a.x = 17 * 32;
                    }
                    else
                    {
                        a.x += 32;
                    }
                    std::this_thread::sleep_for(100ms);
                }
                //Fall
                else if (!grounded && dir == -1)
                {
                    if (a.x < 22 * 32 || a.x >= 24 * 32)
                    {
                        a.x = 22 * 32;
                    }
                    else
                    {
                        a.x += 32;
                    }
                    std::this_thread::sleep_for(100ms);
                }
            }
            else
            {
                std::this_thread::sleep_for(1ms);
            }
        }
    }
public:
    //Self-Explanatory
    bool die = false;
    bool win = false;
    //Jumping/Walljumping
    bool canJump = false;
    bool canWallJump = false;

    bool jumping = false;
    bool wallJumping = false;

    int walldir;
    //Ground Movement
    int horizdir = 1;
    bool run = false;
    bool canWalk = false;

    Player(Collision& c, bool& quit, bool& holdjump, bool& pause, std::mutex& pause_jump_mutex, bool& resetlevel, bool& savetime, SDL_Rect& plrrectanim)
    {
        gJump = Mix_LoadWAV("assets/audio/jump.wav");
        gDeath = Mix_LoadWAV("assets/audio/death.wav");
        gWin = Mix_LoadWAV("assets/audio/win.wav");

        col = &c;
        x = c.GetPlrStartPos().x;
        y = c.GetPlrStartPos().y;
        dir = -1;

        vertVeloThrd = new std::thread(&Player::VertVelo, this, std::ref(quit), std::ref(holdjump), std::ref(pause), std::ref(pause_jump_mutex));
        walkThrd = new std::thread(&Player::Walk, this, std::ref(quit), std::ref(pause), std::ref(resetlevel), std::ref(savetime));
        plrAnimThrd = new std::thread(&Player::PlrAnim, this, std::ref(plrrectanim), std::ref(quit), std::ref(pause));
    }
    ~Player()
    {
        Mix_FreeChunk(gJump);
        Mix_FreeChunk(gDeath);
        Mix_FreeChunk(gWin);
        //std::cout << "PLAYER DESTRUCTOR\n";
        vertVeloThrd->join();
        walkThrd->join();
        plrAnimThrd->join();
        delete(vertVeloThrd);
        delete(walkThrd);
        delete(plrAnimThrd);
    }
    Point GetPos()
    {
        Point temp; 
        temp.x = x;
        temp.y = y;
        return temp;
    }
};