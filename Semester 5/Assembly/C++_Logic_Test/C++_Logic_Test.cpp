#include <iostream>
#include <fstream>
#include <cmath>

struct Color
{
    unsigned int red;
    unsigned int green;
    unsigned int blue;
};

int main()
{
    unsigned int width;
    unsigned int height;

    unsigned int targetWidth ;
    unsigned int targetHeight;
    std::cout << "FILE NAME: ";
	std::string x;
	std::cin >> x;

    std::cout << "TARGET WIDTH: ";
    std::cin >> targetWidth;
    std::cout << "TARGET HEIGHT: ";
    std::cin >> targetHeight;

    //CODE COPIED FROM PREVIOUS PROJECT
    std::ifstream in(x, std::ios::binary);
	if (!in.is_open())
	{
		std::cout << "FILE NOT FOUND\n";
		return 1;
	}

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
                //std::cout << table[i][j].red << " " << table[i][j].green << " " << table[i][j].blue << "\n";
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
    //CODE COPIED FROM PREVIOUS PROJECT END
    std::cout << "\nINTERPOLATION BEGIN\n";
    //Create new, interpolated table
    Color** result = new Color*[targetHeight];
    for (int i = 0; i < targetHeight; i++)
    {
        result[i] = new Color[targetWidth];
        for (int j = 0; j < targetWidth; j++)
        {
            //2.Point position normalized to old image
            float Px = (float)(((float)j * (float)(width-1)) / (float)(targetWidth-1));
            float Py = (float)(((float)i * (float)(height-1)) / (float)(targetHeight-1));
            //std::cout << "Point: " << Px << " , " << Py << "\n";    
            if (floor(Px) == Px && floor(Py) == Py)
            {
                //Exact coordinates, just copy the pixel. 
                result[i][j] = table[(int)floor(Py)][(int)floor(Px)];
            }
            else if (floor(Px) == Px)
            {
                //X cord is the same, only interpolate on y axis
                int y1 = floor(Py);
                int y2 = floor(Py) + 1;

                float w1 = (y2 - Py);
                float w2 = (Py - y1);
                result[i][j].red = w1 * table[y1][(int)floor(Px)].red + w2 * table[y2][(int)floor(Px)].red;
                result[i][j].green = w1 * table[y1][(int)floor(Px)].green + w2 * table[y2][(int)floor(Px)].green;
                result[i][j].blue = w1 * table[y1][(int)floor(Px)].blue + w2 * table[y2][(int)floor(Px)].blue;
            }
            else if (floor(Py) == Py)
            {
                //Y cord is the same, only interpolate on x axis
                int x1 = floor(Px);
                int x2 = floor(Px) + 1;

                float w1 = (x2 - Px);
                float w2 = (Px - x1);

                result[i][j].red = w1 * table[(int)floor(Py)][x1].red + w2 * table[(int)floor(Py)][x2].red;
                result[i][j].green = w1 * table[(int)floor(Py)][x1].green + w2 * table[(int)floor(Py)][x2].green;
                result[i][j].blue = w1 * table[(int)floor(Py)][x1].blue + w2 * table[(int)floor(Py)][x2].blue;
            }
            else
            {
                //Closest points to P
                int x1 = floor(Px);
                int x2 = floor(Px) + 1;
                int y1 = floor(Py);
                int y2 = floor(Py) + 1;
                //3.Weights
                float w11 = ((x2 - Px) * (y2 - Py));
                float w12 = ((x2 - Px) * (Py - y1));
                float w21 = ((Px - x1) * (y2 - Py));
                float w22 = ((Px - x1) * (Py - y1));
                //4.Assign colors
                result[i][j].red = w11 * table[y1][x1].red + w12 * table[y2][x1].red + w21 * table[y1][x2].red + w22 * table[y2][x2].red;
                result[i][j].green = w11 * table[y1][x1].green + w12 * table[y2][x1].green + w21 * table[y1][x2].green + w22 * table[y2][x2].green;
                result[i][j].blue = w11 * table[y1][x1].blue + w12 * table[y2][x1].blue + w21 * table[y1][x2].blue + w22 * table[y2][x2].blue;
            }       
        }
    }
    in.close();
    std::cout << "\nDONE. SAVING TO out.bmp";
    std::ofstream out("out.bmp", std::ios::binary);

    //https://education.siggraph.org/static/HyperVis/asp_data/compimag/bmpfile.htm

    char bh[54];
    for (int i = 0; i < 54; i++)
    {
        bh[i] = 0;
    }
    bh[0] = 'B';
    bh[1] = 'M';
    bh[10] = 54;
    bh[14] = 40;
    bh[18] = targetWidth & 0xFF;
    bh[19] = (targetWidth >> 8) & 0xFF;
    bh[20] = (targetWidth >> 16) & 0xFF;
    bh[21] = (targetWidth >> 24) & 0xFF;
    bh[22] = targetHeight & 0xFF;
    bh[23] = (targetHeight >> 8) & 0xFF;
    bh[24] = (targetHeight >> 16) & 0xFF;
    bh[25] = (targetHeight >> 24) & 0xFF;
    bh[26] = 1;
    bh[28] = 24;

    int bytesPerLine;

    bytesPerLine = targetWidth * 3;  /* (for 24 bit images) */
    /* round up to a dword boundary */
    if (bytesPerLine & 0x0003)
    {
        bytesPerLine |= 0x0003;
        ++bytesPerLine;
    }
    bh[2] = 54 + (long)bytesPerLine * targetHeight;
    //https://education.siggraph.org/static/HyperVis/asp_data/compimag/bmpfile.htm END

    out.write(bh,54);
    for (int i = 0; i < targetHeight; i++)
    {
       char* linebuf = new char[bytesPerLine];
       int ptr = 0;
       for (int j = 0; j < targetWidth; j++)
       {
           linebuf[ptr] = result[i][j].blue;
           linebuf[ptr + 1] = result[i][j].green;
           linebuf[ptr + 2] = result[i][j].red;
           ptr += 3;
       }
       out.write(linebuf,bytesPerLine);
    }
    //Cleanup
    delete(file);
    for (int i = 0; i < height; i++)
    {
        delete(table[i]);
    }
    delete(table);
    for (int i = 0; i < targetHeight; i++)
    {
        delete(result[i]);
    }
    delete(result);
    std::cout << "\nDONE";
}
