module;

import std;

import player;

import <SDL.h>;
import <SDL_mixer.h>;

using namespace std::literals::chrono_literals;

export module inputHandler;

export class InputDealer
{
    Mix_Chunk* gConfirm; //Freed Later
    Mix_Chunk* gCancel; //Freed Later
    Mix_Chunk* gPoint; //Freed Later
    void InputBuffer(bool& x)
    {
        std::this_thread::sleep_for(50ms);
        x = false;
    };
public:
    InputDealer()
    {
        gConfirm = Mix_LoadWAV("assets/audio/confirm.wav");
        gCancel = Mix_LoadWAV("assets/audio/cancel.wav");
        gPoint = Mix_LoadWAV("assets/audio/point.wav");
    }
    void InputDeal(Player& x, SDL_RendererFlip& flip, bool& holdjump, bool& lastSpace, bool& jump, bool& pause, bool& lastEsc, std::mutex& mtx, bool& mtxOwnedAndLocked, float& zoom)
    {
        const Uint8* state = SDL_GetKeyboardState(NULL);
        if (state[SDL_SCANCODE_EQUALS])
        {
            zoom = 2;
        }
        if (state[SDL_SCANCODE_MINUS])
        {
            zoom = 1;
        }
        x.run = state[SDL_SCANCODE_LSHIFT];
        if (state[SDL_SCANCODE_LEFT])
        {
            x.horizdir = -1;
            x.canWalk = true;
            if (!x.die && !x.win && !pause && !x.wallJumping)
            {
                flip = SDL_FLIP_HORIZONTAL;
            }
        }
        else if (state[SDL_SCANCODE_RIGHT])
        {
            x.horizdir = 1;
            x.canWalk = true;
            if (!x.die && !x.win && !pause && !x.wallJumping)
            {
                flip = SDL_FLIP_NONE;
            }
        }
        else
        {
            x.canWalk = false;
            x.horizdir = 0;
        }
        if (state[SDL_SCANCODE_SPACE])
        {
            if (!lastSpace)
            {
                jump = true;
                std::thread z(&InputDealer::InputBuffer,this, std::ref(jump));
                z.detach();
            }
            holdjump = true;
            if (x.canJump && jump)
            {
                jump = false;
                x.canJump = false;
                x.jumping = true;
            }
            if (x.canWallJump && jump)
            {
                if (x.walldir == -1)
                {
                    flip = SDL_FLIP_HORIZONTAL;
                }
                else
                {
                    flip = SDL_FLIP_NONE;
                }
                jump = false;
                x.canWallJump = false;
                x.wallJumping = true;
                x.jumping = true;
            }
            lastSpace = true;
        }
        else
        {
            holdjump = false;
            lastSpace = false;
        }

        if (state[SDL_SCANCODE_ESCAPE])
        {
            if (!lastEsc)
            {
                pause = !pause;
                if (pause)
                {
                    mtx.try_lock();
                    mtxOwnedAndLocked = true;
                    Mix_PlayChannel(-1, gConfirm, 0);
                }
                else
                {
                    mtx.unlock();
                    mtxOwnedAndLocked = false;
                }
            }
            lastEsc = true;
        }
        else
        {
            lastEsc = false;
        }
    }
    void PauseMenuDeal(bool& quit, bool& pause, bool& resume, bool& resetlevel, SDL_Event& event, std::mutex& mtx, bool& mtxOwnedAndLocked)
    {
        if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_UP)
        {
            Mix_PlayChannel(-1, gPoint, 0);
            resume = !resume;
        }
        if (event.key.keysym.sym == SDLK_SPACE)
        {
            if (resume)
            {
                Mix_PlayChannel(-1, gCancel, 0);
                pause = false;
                mtx.unlock();
                mtxOwnedAndLocked = false;
            }
            else
            {
                Mix_PlayChannel(-1, gConfirm, 0);
                quit = true;
                resetlevel = false;
                pause = false;
            }
        }
    }
    void MainMenuDeal(bool& menuquit, bool& appquit, bool& dontloadgraphics, int& choice, int max, SDL_Event& event)
    {
        if (event.key.keysym.sym == SDLK_UP)
        {
            Mix_PlayChannel(-1, gPoint, 0);
            if (choice > 0)
            {
                choice--;
            }
            else
            {
                choice = max - 1;
            }
        }
        if (event.key.keysym.sym == SDLK_DOWN)
        {
            Mix_PlayChannel(-1, gPoint, 0);
            if (choice < max - 1)
            {
                choice++;
            }
            else
            {
                choice = 0;
            }
        }
        if (event.key.keysym.sym == SDLK_SPACE)
        {
            menuquit = true;
            Mix_PlayChannel(-1, gConfirm, 0);
        }
        if (event.key.keysym.sym == SDLK_ESCAPE)
        {
            appquit = true;
            menuquit = true;
            Mix_PlayChannel(-1, gCancel, 0);
        }
        if (event.key.keysym.sym == SDLK_r)
        {
            dontloadgraphics = !dontloadgraphics;
            Mix_PlayChannel(-1, gConfirm, 0);
        }
    }
    ~InputDealer()
    {
        Mix_FreeChunk(gConfirm);
        Mix_FreeChunk(gCancel);
        Mix_FreeChunk(gPoint);
    }
};