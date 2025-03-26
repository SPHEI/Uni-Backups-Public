module;
import std;
import renderers;
import inputHandler;
import <SDL.h>;
import <SDL_ttf.h>;
import <SDL_mixer.h>;
export module app;

export class App
{
    //Rendering
    SDL_Window* window; //Freed later
    SDL_Renderer* renderer; //Freed later

    //Font
    TTF_Font* Font; //Closed later;
    TTF_Font* Font_Outline; //Closed later;

    //global variables
    bool quit = false;
    bool appquit = false;
    bool menuquit = false;
    bool resetlevel = false;
    bool dontloadgraphics = false;

    float zoom = 2;
    float zoomAnim = 2;

    int levelChoice;

    SDL_Event event;

    //The one global texture left
    SDL_Texture* blackTxt; //Freed later
public:
    App()
    {
        //std::cout << "APP CONSTRUCTOR\n";
        //==================================================|INITIAL APP SETUP|==================================================\\

        SDL_Init(SDL_INIT_VIDEO);
        TTF_Init();
        SDL_Init(SDL_INIT_AUDIO);
        SDL_PauseAudio(0);

        //Create window and renderer
        window = SDL_CreateWindow("Platformer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, 0);
        renderer = SDL_CreateRenderer(window, -1, 0);

        //Load font
        Font = TTF_OpenFont("assets/dpcomic.ttf", 24); 
        Font_Outline = TTF_OpenFont("assets/dpcomic.ttf", 24); 
        TTF_SetFontOutline(Font_Outline, 3);

        //Open Audio
        Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048); //Closed later

        //Load textures
        blackTxt = LoadTextureFromFile(renderer, "assets/graphics/black.bmp"); 
    }
	void Run()
	{
        bool appquit = false;
        //==================================================|App Loop|==================================================\\

        while (!appquit)
        {
            //Initial setup
            float levelChoiceAnim = 0;
            int transitionTimer = 0;
            InputDealer Inpt;
            std::string currentLevel;

            //============================Main Menu============================\\

            if (!appquit)
            {

                MainMenu a(Font, Font_Outline, renderer);
                if (a.levelOpt.size() == 0)
                {
                    std::cout << "NO LEVELS";
                    appquit = true;
                    menuquit = true;
                    quit = true;
                }

                levelChoice = 0;

                //============================Main Menu Loop============================\\

                while (!menuquit || transitionTimer > -1)
                {
                    Uint64 start = SDL_GetPerformanceCounter();

                    //Take input, unless in the middle of an animation.
                    if (levelChoice == levelChoiceAnim && transitionTimer > 799)
                    {
                        SDL_PollEvent(&event);
                        switch (event.type)
                        {
                        case SDL_QUIT:
                            menuquit = true;
                            quit = true;
                            appquit = true;
                            break;
                        case SDL_KEYDOWN:

                            Inpt.MainMenuDeal(menuquit, appquit, dontloadgraphics, levelChoice, a.levelOpt.size(), event);
                            break;
                        }
                    }
                    else
                    {
                        levelChoiceAnim += (levelChoice - levelChoiceAnim) / 4;
                        if (levelChoice - levelChoiceAnim < 0.1 && levelChoice - levelChoiceAnim > -0.1)
                        {
                            levelChoiceAnim = levelChoice;
                        }
                    }

                    //Render Everything
                    a.Render(levelChoice, levelChoiceAnim, dontloadgraphics);

                    //Transition
                    if (transitionTimer < 800 && !menuquit)
                    {
                        SDL_Rect top = { 0 + transitionTimer,0,800,300 };
                        SDL_Rect bottom = { 0 - transitionTimer,300,800,300 };
                        transitionTimer += 20;
                        SDL_RenderCopy(renderer, blackTxt, NULL, &top);
                        SDL_RenderCopy(renderer, blackTxt, NULL, &bottom);
                    }
                    if (menuquit)
                    {
                        SDL_Rect top = { 0 + transitionTimer,0,800,300 };
                        SDL_Rect bottom = { 0 - transitionTimer,300,800,300 };
                        transitionTimer -= 20;
                        SDL_RenderCopy(renderer, blackTxt, NULL, &top);
                        SDL_RenderCopy(renderer, blackTxt, NULL, &bottom);
                    }
                    SDL_RenderPresent(renderer);

                    //60 fps
                    Uint64 end = SDL_GetPerformanceCounter();
                    float elapsedMS = (end - start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;
                    if (elapsedMS < 16.666f)
                    {
                        SDL_Delay(floor(16.666f - elapsedMS));
                    }
                }
                currentLevel = a.levelOpt[levelChoice];
            }
            menuquit = false;

            //============================Gameplay============================\\

            if (!appquit)
            {
                resetlevel = true;
                //Setup Collision
                Collision col("levels/" + currentLevel + ".bmp");

                while (resetlevel)
                {
                    std::cout << "------------------------------------------------------------------\n";
                    std::cout << std::setw(40) << "Reseting variables -";
                    transitionTimer = 0;
                    quit = false;

                    Gameplay g(currentLevel, &col, renderer, Font, Font_Outline, dontloadgraphics, quit, resetlevel);

                    std::cout << "------------------------------------------------------------------\n";
                    std::cout << "GAMEPLAY START\n";

                    //Fps counter
                    /*
                    int fps = 0;
                    Uint64 fpsCtrStart = SDL_GetPerformanceCounter();
                    */


                    //============================Gameplay Loop============================\\

                    while (!quit || transitionTimer > -1)
                    {
                        Uint64 start = SDL_GetPerformanceCounter();

                        //Read input during gameplay, unless a transition is happening.
                        if (transitionTimer > 799)
                        {
                            if (!g.timerClass->wasReset)
                            {
                                g.timerClass->Start(g.savetime, g.pause);
                            }
                            Inpt.InputDeal(*g.plr, g.plrflip, g.holdjump, g.lastSpace, g.jump, g.pause, g.lastEsc, g.pause_jump_mutex, g.mtxOwnedAndLocked, zoom);
                        }

                        //std::cout << plr.x << " " << plr.y << "\n";

                        //Take input during pause menu.
                        SDL_PollEvent(&event);
                        switch (event.type)
                        {
                        case SDL_QUIT:
                            menuquit = true;
                            g.pause = false;
                            quit = true;
                            appquit = true;
                            resetlevel = false;
                            g.UnlockMtxIfOwned();
                            break;
                        case SDL_KEYDOWN:
                            if (g.pause)
                            {
                                Inpt.PauseMenuDeal(quit, g.pause, g.resume, resetlevel, event, g.pause_jump_mutex, g.mtxOwnedAndLocked);
                            }
                            break;
                        }

                        //Render bg, fg and player
                        SDL_RenderClear(renderer);
                        g.Render(zoomAnim);

                        //Transition
                        if (transitionTimer < 800 && !quit)
                        {
                            SDL_Rect top = { 0 + transitionTimer,0,800,300 };
                            SDL_Rect bottom = { 0 - transitionTimer,300,800,300 };
                            transitionTimer += 20;
                            SDL_RenderCopy(renderer, blackTxt, NULL, &top);
                            SDL_RenderCopy(renderer, blackTxt, NULL, &bottom);
                        }
                        if (quit)
                        {
                            SDL_Rect top = { 0 + transitionTimer,0,800,300 };
                            SDL_Rect bottom = { 0 - transitionTimer,300,800,300 };
                            transitionTimer -= 20;
                            SDL_RenderCopy(renderer, blackTxt, NULL, &top);
                            SDL_RenderCopy(renderer, blackTxt, NULL, &bottom);
                        }

                        SDL_RenderPresent(renderer);

                        //60 fps
                        Uint64 end = SDL_GetPerformanceCounter();
                        float elapsedMS = (end - start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;
                        if (elapsedMS < 16.666f)
                        {
                            SDL_Delay(floor(16.666f - elapsedMS));
                        }

                        /*
                        fps++;
                        if (((end - fpsCtrStart)/ (float)SDL_GetPerformanceFrequency() * 1000.0f) > 1000)
                        {
                            std::cout << fps << "\n";
                            fps = 0;
                            fpsCtrStart = SDL_GetPerformanceCounter();
                        }
                        */

                        if (zoom != zoomAnim)
                        {
                            zoomAnim += (zoom - zoomAnim) / 4;
                            if (zoom - zoomAnim < 0.001 && zoom - zoomAnim > -0.001)
                            {
                                zoomAnim = zoom;
                            }
                        }
                    }
                    g.UnlockMtxIfOwned();
                    g.SaveTimeIfWin(currentLevel);

                    std::cout << "------------------------------------------------------------------\n";
                    std::cout << "GAMEPLAY END\n";
                }
            }
            std::cout << "===================================================================\n\n\n\n\n\n";
        }
        return;

	}
    ~App()
    {
        //==================================================|App Cleanup|==================================================\\

        SDL_DestroyTexture(blackTxt);

        TTF_CloseFont(Font);
        TTF_CloseFont(Font_Outline);

        Mix_CloseAudio();

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);

        SDL_Quit();
        //std::cout << "APP DESTRUCTOR\n";
    }
};