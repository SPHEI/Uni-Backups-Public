module;

import std;

import collision;
import player;
import enemies;

import timer;

import <SDL.h>;
import <SDL_ttf.h>;
import <SDL_mixer.h>;
export import sdlLoad;

export module renderers;

export class Gameplay
{
    //References
    SDL_Renderer* renderer;
    TTF_Font* Font;
    TTF_Font* Font_Outline;

    //Tetures
    SDL_Texture* levelTxt;   //level
    SDL_Texture* bgTxt;     //level background
    SDL_Texture* pauseTxt; //pause texture
    SDL_Texture* plrTxt;  //player textures
    SDL_Texture* tomateTxt; //tomate 
    SDL_Texture* piksawTxt;  //piksaw
    SDL_Texture* shieldTxt; //shields

    //Rects
    SDL_Rect pauseRect = { 0,0,800,600 };
    SDL_Rect pauseRectAnim = { 800,0,800,600 };
    SDL_Rect timerRectOutline = { 629,-3,174,30 };
    SDL_Rect timerRect = { 632,0,168,24 };
    SDL_Rect plrRectAnim = { 0,0,32,32 };

public:
    SDL_RendererFlip plrflip = SDL_FLIP_NONE;

    //GameObjects
    Collision* col;
    Player* plr;
    std::vector<Entity*> entities;
    TimeCtr* timerClass;

    //Variables
    bool holdjump = false;
    bool lastSpace = false;
    bool lastEsc = false;

    bool jump = false;

    bool savetime = false;

    std::mutex pause_jump_mutex;
    bool mtxOwnedAndLocked = false;

    bool resume = true;
    bool pause = false;

    void SaveTimeIfWin(std::string currentLevel)
    {
        if (savetime)
        {
            if (!std::filesystem::exists("bestTimes/"))
            {
                std::filesystem::create_directory("bestTimes");
            }
            timerClass->SaveIfHigher("bestTimes/" + currentLevel);
        }
        savetime = true;
    }
    void UnlockMtxIfOwned()
    {
        if (mtxOwnedAndLocked)
        {
            pause_jump_mutex.unlock();
            mtxOwnedAndLocked = false;
        }
    }
    void Render(float zoom)
    {
        //Setup rects
        SDL_Rect levelRect = { static_cast<int>((-plr->x * zoom) + 400 - 16 * zoom),static_cast<int> (plr->y * zoom - (col->GetHeight() - 134) * zoom + (2 - zoom) * 150),static_cast<int>(col->GetWidth() * zoom), static_cast<int>(col->GetHeight() * zoom) };
        SDL_Rect plrRect = { 400 - 24 * zoom,300 - 16 * zoom,32 * zoom,32 * zoom };

        //Setup Timer Graphic
        SDL_Texture* timerOutlineTxt = CreateTextureFromString(renderer, Font_Outline, (timerClass->GetMinutes() + ":" + timerClass->GetSeconds() + ":" + timerClass->GetMiliseconds()), { 0, 0, 0 }); //Freed later
        SDL_Texture* timerTxt = CreateTextureFromString(renderer, Font, (timerClass->GetMinutes() + ":" + timerClass->GetSeconds() + ":" + timerClass->GetMiliseconds()), { 255, 255, 255 });//Freed later


        SDL_RenderCopy(renderer, bgTxt, NULL, &pauseRect);
        SDL_RenderCopy(renderer, levelTxt, NULL, &levelRect);

        SDL_RenderCopyEx(renderer, plrTxt, &plrRectAnim, &plrRect, 0, NULL, plrflip);

        //Render Entities
        for (int i = 0; i < entities.size(); i++)
        {
            if (std::abs(plr->x - entities[i]->x) < 450 && std::abs(plr->y - entities[i]->y) < 350)
            {
                SDL_Rect temp = { 400 - 16 * zoom - (plr->x - entities[i]->x) * zoom ,300 - 16 * zoom + (plr->y - entities[i]->y) * zoom,entities[i]->GetWidth() * zoom,entities[i]->GetHeight() * zoom };
                SDL_RendererFlip flp = SDL_FLIP_NONE;
                if (entities[i]->dir == 1)
                {
                    flp = SDL_FLIP_HORIZONTAL;
                }
                SDL_RenderCopyEx(renderer, entities[i]->texture, &(entities[i]->a), &temp, entities[i]->rot, NULL, flp);
            }
        }


        //Render timer
        SDL_RenderCopy(renderer, timerOutlineTxt, NULL, &timerRectOutline);
        SDL_RenderCopy(renderer, timerTxt, NULL, &timerRect);


        //Render pause menu
        if (pause)
        {
            if (resume)
            {
                pauseRectAnim = { 800,0,800,600 };
            }
            else
            {
                pauseRectAnim = { 0,0,800,600 };
            }
            SDL_RenderCopyEx(renderer, pauseTxt, &pauseRectAnim, &pauseRect, 0, NULL, SDL_FLIP_NONE);
        }

        //Destroy timer textures
        SDL_DestroyTexture(timerOutlineTxt);
        SDL_DestroyTexture(timerTxt);
    }
    Gameplay(std::string currentLevel, Collision* c, SDL_Renderer* r, TTF_Font* s, TTF_Font* so, bool dontloadgraphics, bool& quit, bool& resetlevel)
    {
        renderer = r;
        Font = s;
        Font_Outline = so;
        col = c;

        std::cout << " done.\n";
        //Setup Player
        std::cout << std::setw(40) << "Creating player -";
        plr = new Player(*col, quit, holdjump, pause, pause_jump_mutex, resetlevel, savetime, plrRectAnim);

        std::cout << " done.\n";

        //Setup timer
        std::cout << std::setw(40) << "Creating timer -";
        timerClass = new TimeCtr();
        std::cout << " done.\n";

        std::cout << std::setw(40) << "Loading textures -";
        //Load level image
        if (std::filesystem::exists("levels/" + currentLevel + "_Graphics.bmp") && !dontloadgraphics)
        {
            levelTxt = LoadTextureFromFile(renderer, ("levels/" + currentLevel + "_Graphics.bmp"));
        }
        else
        {
            levelTxt = LoadTextureFromFile(renderer, ("levels/" + currentLevel + ".bmp"));
        }

        //Load Bg image
        if (std::filesystem::exists("levels/" + currentLevel + "_Bg.bmp") && !dontloadgraphics)
        {
            bgTxt = LoadTextureFromFile(renderer, ("levels/" + currentLevel + "_Bg.bmp"));
        }
        else
        {
            bgTxt = LoadTextureFromFile(renderer, "assets/graphics/black.bmp");
        }

        pauseTxt = LoadTextureFromFile(renderer, "assets/graphics/pause.bmp"); //Freed later
        plrTxt = LoadTextureFromFile(renderer, "assets/graphics/plr.bmp"); //Freed later

        tomateTxt = LoadTextureFromFile(renderer, "assets/graphics/tomate.bmp"); //Freed later
        piksawTxt = LoadTextureFromFile(renderer, "assets/graphics/piksaw.bmp"); //Freed later
        shieldTxt = LoadTextureFromFile(renderer, "assets/graphics/shieldBoi.bmp"); //Freed later

        std::cout << " done.\n";

        //Spawn Entities
        //Spawn Tomates
        std::cout << std::setw(40) << "Creating tomates -";
        for (auto& x : col->tomatePositions)
        {
            entities.emplace_back(new Tomate(tomateTxt, x.x, x.y, 16, 16, plr, col, quit, pause));
        }
        std::cout << " done. Count: " << col->tomatePositions.size() << "\n";

        //Spawn Piksaws
        std::cout << std::setw(40) << "Creating piksaws -";
        for (auto& x : col->piksawPositions)
        {
            entities.emplace_back(new Piksaw(piksawTxt, x.x, x.y, 16, 16, plr, col, quit, pause));
        }
        std::cout << " done. Count: " << col->piksawPositions.size() << "\n";

        //Spawn Shields
        std::cout << std::setw(40) << "Creating shields -";
        for (auto& x : col->shieldPositions)
        {
            entities.emplace_back(new Shield(shieldTxt, x.x, x.y, 16, 32, plr, col, quit, pause));
        }
        std::cout << " done. Count: " << col->shieldPositions.size() << "\n";


    }
    ~Gameplay()
    {
        SDL_DestroyTexture(levelTxt);
        SDL_DestroyTexture(bgTxt);
        SDL_DestroyTexture(pauseTxt);
        SDL_DestroyTexture(plrTxt);
        SDL_DestroyTexture(tomateTxt);
        SDL_DestroyTexture(piksawTxt);
        SDL_DestroyTexture(shieldTxt);
        if (mtxOwnedAndLocked)
        {
            pause_jump_mutex.unlock();
        }
        //Destroy stuff
        for (auto& x : entities)
        {
            delete(x);
        }
        delete(plr);
        delete(timerClass);
        //std::cout << "GAMEPLAY DESTRUCTOR\n";
    }
};

export class MainMenu
{
    //References
    TTF_Font* Font;
    TTF_Font* Font_Outline;
    SDL_Renderer* renderer;

    //Textures
    SDL_Texture* mainTxt; //Main menu frame
    SDL_Texture* mainBgTxt; //Main menu background
    SDL_Texture* cursorTxt; //Cursor

    SDL_Texture* bypassYesOutlineTxt; //Freed Later //Outline
    SDL_Texture* bypassYesTxt; //Freed Later

    SDL_Texture* bypassNoOutlineTxt; //Freed Later //Outline
    SDL_Texture* bypassNoTxt; //Freed Later

    std::vector<SDL_Texture*> levelNamesTxt; //Freed later      //Level Names
    std::vector<SDL_Texture*> levelPrevTxt; //Freed later          //Level Preview
    std::vector<SDL_Texture*> levelPrevBgTxt; //Freed later        //Level Bg Preview
    std::vector<SDL_Texture*> levelTimesTxt; //Freed later         //Best time for each level
    std::vector<SDL_Texture*> levelTimesOutlinesTxt; //Freed later //Outline for levelTimesTxt

    SDL_Texture* lineTxt; //Freed Later


    //Rects
    SDL_Rect mainRect = { 0,0,800,600 };

    SDL_Rect previewRect = { 312, 12, 476, 576 };
    SDL_Rect previewPartRect = { 0,0,476, 576 };
    SDL_Rect cursorRect = { 14, 12 + 48 * 5,288,48 };

    SDL_Rect bypassRectOutline = { 396,547,386,30 };
    SDL_Rect bypassRect = { 400,550,380,24 };

    SDL_Rect timeRectOutline = { 402,12,390,30 };
    SDL_Rect timeRect = { 406,15,384,24 };
public:
    std::vector<std::string> levelOpt;

    MainMenu(TTF_Font* f, TTF_Font* fo, SDL_Renderer* r)
    {
        //std::cout << "MAIN MENU CONSTRUCTOR\n";
        Font = f;
        Font_Outline = fo;
        renderer = r;
        mainTxt = LoadTextureFromFile(renderer, "assets/graphics/menu_bg.bmp");; //Freed later
        mainBgTxt = LoadTextureFromFile(renderer, "assets/graphics/menu_bg_color.bmp"); //Freed later
        cursorTxt = LoadTextureFromFile(renderer, "assets/graphics/cursor.bmp"); //Freed later
        //Scan for levels
        std::cout << "SCANNING LEVELS:\n";

        std::filesystem::path levels("levels");
        //Scan for levels
        std::cout << std::setw(40) << "\t\t\tLevel Name\tCompatibility\n===================================================================\n";
        for (const auto& entry : std::filesystem::directory_iterator(levels))
        {
            if (entry.is_regular_file() && (entry.path()).extension() == ".bmp")
            {
                std::cout << std::setw(40);
                std::string name = (entry.path()).stem().generic_string();
                std::regex gr("_Graphics");
                std::regex br("_Bg");
                if (name.length() > 9)
                {
                    if (std::regex_search(name, gr) == 0 && std::regex_search(name, br) == 0)
                    {
                        std::cout << name;
                        if (Compatible("levels/" + name + ".bmp"))
                        {
                            levelOpt.push_back(name);
                        }
                    }
                }
                else if (name.length() > 3)
                {
                    if (std::regex_search(name, br) == 0)
                    {
                        std::cout << name;
                        if (Compatible("levels/" + name + ".bmp"))
                        {
                            levelOpt.push_back(name);
                        }
                    }
                }
                else
                {
                    std::cout << name;
                    if (Compatible("levels/" + name + ".bmp"))
                    {
                        levelOpt.push_back(name);
                    }
                }
            }
        }
        std::cout << "===================================================================\n";
        //Load Level texts
        for (auto& n : levelOpt)
        {
            levelNamesTxt.push_back(CreateTextureFromString(renderer, Font, n, { 10, 10, 10 }));
        }
        //Load previews
        for (int i = 0; i < levelOpt.size(); i++)
        {
            if (std::filesystem::exists("levels/" + levelOpt[i] + "_Graphics.bmp"))
            {
                levelPrevTxt.push_back(LoadTextureFromFile(renderer, ("levels/" + levelOpt[i] + "_Graphics.bmp")));
            }
            else
            {
                levelPrevTxt.push_back(LoadTextureFromFile(renderer, ("levels/" + levelOpt[i] + ".bmp")));
            }
            if (std::filesystem::exists("levels/" + levelOpt[i] + "_Bg.bmp"))
            {
                levelPrevBgTxt.push_back(LoadTextureFromFile(renderer, ("levels/" + levelOpt[i] + "_Bg.bmp")));
            }
            else
            {
                levelPrevBgTxt.push_back(LoadTextureFromFile(renderer, "assets/graphics/black.bmp"));
            }
        }
        //Load times
        for (auto& n : levelOpt)
        {
            std::string temp;
            if (std::filesystem::exists("bestTimes/" + n))
            {
                std::string filename = "bestTimes/" + n;
                std::ifstream in;
                in.open(filename);
                int a;
                in >> a;
                in.close();
                int hr = a / 6000;
                a = a % 6000;
                int mn = a / 100;
                int sc = a % 100;
                std::string mnstr;
                if (mn > 9)
                {
                    mnstr = std::to_string(mn);
                }
                else
                {
                    mnstr = "0" + std::to_string(mn);
                }
                std::string scstr;
                if (sc > 9)
                {
                    scstr = std::to_string(sc);
                }
                else
                {
                    scstr = "0" + std::to_string(sc);
                }
                temp = "Best time: " + std::to_string(hr) + ":" + mnstr + ":" + scstr;
            }
            else
            {
                temp = "Best time: 9:99:99";
            }
            levelTimesTxt.push_back(CreateTextureFromString(renderer, Font, temp, { 255, 255, 255 }));
            levelTimesOutlinesTxt.push_back(CreateTextureFromString(renderer, Font_Outline, temp, { 0, 0, 0 }));
        }
        //Level list bounds
        lineTxt = CreateTextureFromString(renderer, Font, "------------", { 10, 10, 10 }); //Freed later
        //Load Graphics Text
        bypassYesOutlineTxt = CreateTextureFromString(renderer, Font_Outline, "[R] Load Graphics: YES", { 0, 0, 0 });//Freed later
        bypassYesTxt = CreateTextureFromString(renderer, Font, "[R] Load Graphics: YES", { 255, 255, 255 });//Freed later
        bypassNoOutlineTxt = CreateTextureFromString(renderer, Font_Outline, "[R] Load Graphics: NO", { 0, 0, 0 });
        bypassNoTxt = CreateTextureFromString(renderer, Font, "[R] Load Graphics: NO", { 255, 255, 255 });
    }
    ~MainMenu()
    {
        //free all menu textures
        SDL_DestroyTexture(mainTxt);
        SDL_DestroyTexture(mainBgTxt);
        SDL_DestroyTexture(cursorTxt);
        SDL_DestroyTexture(lineTxt);
        SDL_DestroyTexture(bypassYesOutlineTxt);
        SDL_DestroyTexture(bypassYesTxt);
        SDL_DestroyTexture(bypassNoOutlineTxt);
        SDL_DestroyTexture(bypassNoTxt);
        for (auto& x : levelNamesTxt)
        {
            SDL_DestroyTexture(x);
        }
        for (auto& x : levelPrevTxt)
        {
            SDL_DestroyTexture(x);
        }
        for (auto& x : levelPrevBgTxt)
        {
            SDL_DestroyTexture(x);
        }
        for (auto& x : levelTimesTxt)
        {
            SDL_DestroyTexture(x);
        }
        for (auto& x : levelTimesOutlinesTxt)
        {
            SDL_DestroyTexture(x);
        }
        //std::cout << "MAIN MENU DESTRUCTOR\n";
    }
    void Render(int levelChoice, float levelChoiceAnim, bool dontloadgraphics)
    {

        //Render cursor, bg and preview
        SDL_RenderCopy(renderer, mainBgTxt, NULL, &mainRect);
        SDL_RenderCopy(renderer, cursorTxt, NULL, &cursorRect);
        SDL_RenderCopy(renderer, levelPrevBgTxt[levelChoice], &previewPartRect, &previewRect);
        SDL_RenderCopy(renderer, levelPrevTxt[levelChoice], &previewPartRect, &previewRect);

        //Render level list   
        int p = 0;
        SDL_Rect LineRect = { 14, 48 * 5 + 12 - 48 - 48 * levelChoiceAnim,286,48 };
        SDL_RenderCopy(renderer, lineTxt, NULL, &LineRect);
        for (int i = 0; i < levelOpt.size(); i++)
        {
            SDL_Rect MessageRect;
            MessageRect.x = 14;
            MessageRect.y = 48 * 5 + 12 + 48 * p - 48 * levelChoiceAnim;
            MessageRect.h = 48;
            if (levelOpt[i].length() < 13)
            {
                MessageRect.w = 24 * levelOpt[i].length();
            }
            else
            {
                MessageRect.w = 286;
            }
            SDL_RenderCopy(renderer, levelNamesTxt[i], NULL, &MessageRect);
            p++;
        }
        SDL_Rect LineRect2 = { 14,48 * 5 + 12 + 48 * p - 48 * levelChoiceAnim,286,48 };
        SDL_RenderCopy(renderer, lineTxt, NULL, &LineRect2);

        //Render main graphic
        SDL_RenderCopy(renderer, mainTxt, NULL, &mainRect);

        //Render best time
        SDL_RenderCopy(renderer, levelTimesOutlinesTxt[levelChoice], NULL, &timeRectOutline);
        SDL_RenderCopy(renderer, levelTimesTxt[levelChoice], NULL, &timeRect);

        //Render Graphics Bypass Info
        if (!dontloadgraphics)
        {
            SDL_RenderCopy(renderer, bypassYesOutlineTxt, NULL, &bypassRectOutline);
            SDL_RenderCopy(renderer, bypassYesTxt, NULL, &bypassRect);
        }
        else
        {
            SDL_RenderCopy(renderer, bypassNoOutlineTxt, NULL, &bypassRectOutline);
            SDL_RenderCopy(renderer, bypassNoTxt, NULL, &bypassRect);
        }

    }
};
