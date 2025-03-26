module;

import std;

export module collision;

struct Color
{
    unsigned int red;
    unsigned int green;
    unsigned int blue;
};
export class Point
{
public:
    int x;
    int y;
    Point()
    {
        x = 0;
        y = 0;
    }
    Point(int a, int b)
    {
        x = a;
        y = b;
    }
    Point& operator+=(const Point& a)
    {
        x += a.x;
        y += a.y;
        return *this;
    }
    friend Point operator+(Point b,const Point& a)
    {
        b += a;
        return b;
    }
};

export bool Compatible(std::string x)
{
    std::ifstream in(x, std::ios::binary);

    char header[54];

    in.read(header, 54);

    char compression = header[30];
    char bits = header[28];

    if (bits != 24 && bits != 32)
    {
        std::cout << "\tINCOMPATIBLE \t\t WRONG BIT DEPTH - " << (int)bits << "!\n";
        return false;
    }
    if (compression != 0 && compression != 3)
    {
        std::cout << "\tINCOMPATIBLE \t\t COMPRESSION ENABLED - " << (int)compression << "!\n";
        return false;
    }
    std::cout << "\tCOMPATIBLE\n";
    return true;
}

export class Collision
{
    std::vector<std::pair<Point, Point>> hitboxes;
    std::vector<std::pair<Point, Point>> goals;
    std::vector<std::pair<Point, Point>> hazards;

    int width;
    int height;

    Point plrStartPos;
    bool StartPosFound = false;

    bool hitContains(int x, int y, std::vector<std::pair<Point, Point>>& b)
    {
        for (auto& z : b)
        {
            if (z.first.x <= x && z.second.x >= x && z.first.y <= y && z.second.y >= y)
            {
                return true;
            }
        }
        return false;
    }
    bool entityContains(int x, int y, int wx, int wy, std::vector<Point>& b)
    {
        for (auto& z : b)
        {
            if (z.x <= x && z.x + wx - 1 >= x && z.y <= y && z.y + wy - 1 >= y)
            {
                return true;
            }
        }
        return false;
    }
    bool collisionCheck(std::pair<Point, Point> x, std::vector<std::pair<Point, Point>>& b)
    {
        for (auto& z : b)
        {
            bool xaxis = false;
            bool yaxis = false;
            if ((x.first.x >= z.first.x && x.first.x <= z.second.x) || (x.second.x >= z.first.x && x.second.x <= z.second.x) 
                || (z.first.x >= x.first.x && z.first.x <= x.second.x) || (z.second.x >= x.first.x && z.second.x <= x.second.x))
            {
                xaxis = true;
            }
            if ((x.first.y >= z.first.y && x.first.y <= z.second.y) || (x.second.y >= z.first.y && x.second.y <= z.second.y) 
                || (z.first.y <= x.first.y && z.first.y >= x.second.y) || (z.second.y <= x.first.y && z.second.y >= x.second.y))
            {
                yaxis = true;
            }
            if (xaxis && yaxis)
            {
                return true;
            }
        }
        return false;
    }
    bool IsColor(Color& x, std::string y)
    {
        if (y == "Black")
        {
            return (x.red == 0 && x.green == 0 && x.blue == 0);
        }
        if (y == "Green")
        {
            return (x.red == 0 && x.green == 255 && x.blue == 0);
        }
        if (y == "Yellow")
        {
            return (x.red == 255 && x.green == 255 && x.blue == 0);
        }
        if (y == "Red")
        {
            return (x.red == 255 && x.green == 0 && x.blue == 0);
        }
        if (y == "Tomate")
        {
            return (x.red == 100 && x.green == 0 && x.blue == 0);
        }
        if (y == "Piksaw")
        {
            return (x.red == 0 && x.green == 100 && x.blue == 0);
        }
        if (y == "Shield")
        {
            return (x.red == 0 && x.green == 0 && x.blue == 100);
        }
        return false;
    }
public:
    std::vector<Point> tomatePositions;
    std::vector<Point> piksawPositions;
    std::vector<Point> shieldPositions;

    Collision(std::string x)
    {
        std::cout << "STARTING DATA LOAD: \n\nFILEPATH: " << x << "\n\n";
        std::ifstream in(x, std::ios::binary);

        char header[54];

        in.read(header, 54);

        width = *(int*)&header[18];
        height = *(int*)&header[22];
        int filesize = *(int*)&header[2];
        int compression = *(int*)&header[30];
        char bits = header[28];
        std::cout << "IMG SIZE: " << width << " x " << height << " pixels\nFILESIZE: " << filesize << " bytes\nCOMPRESSION?: " << ((compression == 0 || (bits == 32 && compression == 3)) ? "No" : "Yes") << "\nBIT DEPTH: " << bits << "\n\n";

        
        if (!((bits == 24 && compression == 0) || (bits == 32 && compression == 3)))
        {
            std::cout << "WRONG BIT DEPTH OR COMPRESSION ENABLED - INCOMPATIBLE FILE\n\nBIT DEPTH SHOULD BE 24 OR 32\n\n";
            exit(-1);
        }
        
        char* file; //deleted later
        if (bits == 24)
        {
            file = new char[filesize - 54];
            in.read(file, filesize - 54);
            in.close();
        }
        else
        {
            //32 bit has longer headder so we need to burn through the remaining bytes
            char temp[96];
            in.read(temp, 96);

            file = new char[filesize - 150];
            in.read(file, filesize - 150);
            in.close();
        }
        Color** table = new Color * [height]; //deleted later
        int pos = 0;

        //Load Colors
        if (bits == 24)
        {
            for (int i = 0; i < height; i++)
            {
                table[i] = new Color[width];
                for (int j = 0; j < width; j++)
                {
                    table[i][j].blue = (unsigned char)*(file + pos);
                    table[i][j].green = (unsigned char)*(file + pos + 1);
                    table[i][j].red = (unsigned char)*(file + pos + 2);

                    pos += 3;
                }
                while (pos % 4 != 0)
                {
                    pos++;
                }
            }
        }
        else 
        {
            for (int i = 0; i < height; i++)
            {
                table[i] = new Color[width];
                for (int j = 0; j < width; j++)
                {
                    table[i][j].blue = (unsigned char)*(file + pos);
                    table[i][j].green = (unsigned char)*(file + pos + 1);
                    table[i][j].red = (unsigned char)*(file + pos + 2);

                    pos += 4;
                }
            }
        }
        delete(file);
        //Load data from color table
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                //Hitboxes
                if (IsColor(table[i][j],"Black") && !hitContains(j, i, hitboxes))
                {
                    Point first;
                    first.x = j;
                    first.y = i;
                    Point second;
                    second.x = j;
                    second.y = i;

                    int tempx = j + 1;
                    while (IsColor(table[i][tempx],"Black") && tempx < width)
                    {
                        second.x++;
                        tempx++;
                    }
                    int tempy = i + 1;
                    while (tempy < height)
                    {
                        bool works = true;
                        for (int a = j; a < tempx; a++)
                        {
                            if (a >= width)
                            {
                                break;
                            }
                            if (!IsColor(table[tempy][a],"Black"))
                            {
                                works = false;
                            }
                        }
                        if (works)
                        {
                            second.y++;
                            tempy++;
                        }
                        else
                        {
                            break;
                        }
                    }
                    j = tempx;
                    hitboxes.push_back(std::make_pair(first, second));


                
                }
                //Start Pos
                if (IsColor(table[i][j],"Green") && !StartPosFound)
                {
                    bool yes = true;
                    for (int k = 0; k < 32; k++)
                    {
                        if (i + k >= height)
                        {
                            break;
                        }
                        for (int l = 0; l < 16; l++)
                        {
                            if (j + l >= width)
                            {
                                break;
                            }
                            if (!IsColor(table[i + k][j + l],"Green"))
                            {
                                yes = false;
                            }
                        }
                    }

                    if (yes)
                    {
                        plrStartPos.x = j;
                        plrStartPos.y = i + 32;
                        StartPosFound = true;
                    }
                }
                //Goals
                if (IsColor(table[i][j],"Yellow") && !hitContains(j, i, goals))
                {
                    Point first;
                    first.x = j;
                    first.y = i;
                    Point second;
                    second.x = j;
                    second.y = i;

                    int tempx = j + 1;
                    while (IsColor(table[i][tempx],"Yellow") && tempx < width)
                    {
                        second.x++;
                        tempx++;
                    }
                    int tempy = i + 1;
                    while (tempy < height)
                    {
                        bool works = true;
                        for (int a = j; a < tempx; a++)
                        {
                            if (a >= width)
                            {
                                break;
                            }
                            if (!IsColor(table[tempy][a],"Yellow"))
                            {
                                works = false;
                            }
                        }
                        if (works)
                        {
                            second.y++;
                            tempy++;
                        }
                        else
                        {
                            break;
                        }
                    }
                    j = tempx;
                    goals.push_back(std::make_pair(first, second));
                }
                //Static Hazards
                if (IsColor(table[i][j], "Red") && !hitContains(j, i, hazards))
                {
                    Point first;
                    first.x = j;
                    first.y = i;
                    Point second;
                    second.x = j;
                    second.y = i;

                    int tempx = j + 1;
                    while (IsColor(table[i][tempx], "Red") && tempx < width)
                    {
                        second.x++;
                        tempx++;
                    }
                    int tempy = i + 1;
                    while (tempy < height)
                    {
                        bool works = true;
                        for (int a = j; a < tempx; a++)
                        {
                            if (a >= width)
                            {
                                break;
                            }
                            if (!IsColor(table[tempy][a], "Red"))
                            {
                                works = false;
                            }
                        }
                        if (works)
                        {
                            second.y++;
                            tempy++;
                        }
                        else
                        {
                            break;
                        }
                    }
                    j = tempx;
                    hazards.push_back(std::make_pair(first, second));
                }
                //Tomate Spawns
                if (IsColor(table[i][j], "Tomate") && !entityContains(j,i,16,16,tomatePositions))
                {
                    bool yes = true;
                    for (int k = 0; k < 16; k++)
                    {
                        if (i + k >= height)
                        {
                            break;
                        }
                        for (int l = 0; l < 16; l++)
                        {
                            if (j + l >= width)
                            {
                                break;
                            }
                            if (!IsColor(table[i + k][j + l], "Tomate"))
                            {
                                yes = false;
                            }
                        }
                    }

                    if (yes)
                    {
                        Point result;
                        result.x = j;
                        result.y = i;
                        tomatePositions.push_back(result);
                    }
                }
                //Piksaws Spawns
                if (IsColor(table[i][j], "Piksaw") && !entityContains(j, i, 16, 16, piksawPositions))
                {
                    bool yes = true;
                    for (int k = 0; k < 16; k++)
                    {
                        if (i + k >= height)
                        {
                            break;
                        }
                        for (int l = 0; l < 16; l++)
                        {
                            if (j + l >= width)
                            {
                                break;
                            }
                            if (!IsColor(table[i + k][j + l], "Piksaw"))
                            {
                                yes = false;
                            }
                        }
                    }

                    if (yes)
                    {
                        Point result;
                        result.x = j;
                        result.y = i;
                        piksawPositions.push_back(result);
                    }
                }
                //Shield Spawns
                if (IsColor(table[i][j], "Shield") && !entityContains(j, i, 16, 32, shieldPositions))
                {
                    bool yes = true;
                    for (int k = 0; k < 32; k++)
                    {
                        if (i + k >= height)
                        {
                            break;
                        }
                        for (int l = 0; l < 16; l++)
                        {
                            if (j + l >= width)
                            {
                                break;
                            }
                            if (!IsColor(table[i + k][j + l], "Shield"))
                            {
                                yes = false;
                            }
                        }
                    }

                    if (yes)
                    {
                        Point result;
                        result.x = j;
                        result.y = i;
                        shieldPositions.push_back(result);
                    }
                }
            }
        }

        if (!StartPosFound)
        {
            plrStartPos.x = 0;
            plrStartPos.y = height;
            std::cout << "START POS NOT FOUND\n";
        }

        //Cleanup
        for (int i = 0; i < height; i++)
        {
            delete(table[i]);
        }
        delete(table);
        std::cout << "FINISHED DATA LOAD\n\n";
    }
    ~Collision()
    {
        //std::cout << "COLLISION DESTRUCTOR\n";
    }
    Point GetPlrStartPos()
    {
        return plrStartPos;
    }

    int GetWidth()
    {
        return width;
    }
    int GetHeight()
    {
        return height;
    }

    bool CheckGoal(int posx, int posy)
    {
        Point first;
        Point second;
        first.x = posx;
        first.y = posy;
        second.x = posx + 16;
        second.y = posy - 32;
        if (collisionCheck(std::make_pair(first, second),goals))
        {
            return true;
        }
        return false;
    }
    bool CheckHazard(int posx, int posy)
    {
        Point first;
        Point second;
        first.x = posx;
        first.y = posy;
        second.x = posx + 16;
        second.y = posy - 32;
        if (collisionCheck(std::make_pair(first, second), hazards))
        {
            return true;
        }
        return false;
    }
    bool CheckCollision(int posx, int posy,int dirx, int diry, int uwidth, int uheight)
    {
        Point first;
        Point second;
        first.x = posx + dirx;
        first.y = posy + diry - 1;
        second.x = posx + dirx + uwidth;
        second.y = posy + diry - uheight;

        if (collisionCheck(std::make_pair(first, second),hitboxes))
        {
            return false;
        }
        if (dirx == 1)
        {
            if (posx + uwidth + 1 > width)
            {
                return false;
            }
            if (posx + uwidth + 1 == width)
            {
                return true;
            }
        }
        if (dirx == -1)
        {
            if (posx - 1 < 0)
            {
                return false;
            }
        }
        if (diry == 1)
        {
            if (posy >= height)
            {
                return false;
            }
        }
        if (diry == -1)
        {
            if (posy - uheight - 1 < 0)
            {
                return false;
            }
        }
        return true;
    }
    bool CheckOverlap(std::pair<Point, Point> x, std::pair<Point, Point> z)
    {
        bool xaxis = false;
        bool yaxis = false;
        if ((x.first.x >= z.first.x && x.first.x <= z.second.x) || (x.second.x >= z.first.x && x.second.x <= z.second.x)
            || (z.first.x >= x.first.x && z.first.x <= x.second.x) || (z.second.x >= x.first.x && z.second.x <= x.second.x))
        {
            xaxis = true;
        }
        if ((x.first.y >= z.first.y && x.first.y <= z.second.y) || (x.second.y >= z.first.y && x.second.y <= z.second.y)
            || (z.first.y <= x.first.y && z.first.y >= x.second.y) || (z.second.y <= x.first.y && z.second.y >= x.second.y))
        {
            yaxis = true;
        }
        if (xaxis && yaxis)
        {
            return true;
        }
        return false;
    }
};