using System;

/*
 * Bilinear interpolation project. - C# implementation. 
 * Winter Semester 2024/2025
 * Authors: Jakub (https://github.com/SPHEI), Alicja (https://github.com/Smoka24)
 * Version 1.0
 */
namespace CSharp
{
    public class Interpolator
    {
        /*
         * C# implementation of the interpolation procedure.
         * Inputs:
         *  inArray - The input image in the form of an array of bytes. The image is represented as color values in Blue-Green-Red order going from top-left to bottom-right of the image.
         *  Width - Width of the original image.
         *  Height - Height of the original image.
         *  TargetWidth - Width of the output image.
         *  TargetHeight - Height of the output image.
         * Output:
         *  outArray - The output array holding the new image. It is initialized with 0s and passed to the program as one of the inputs.
         */
        public void Interpolate(byte[] inArray, byte[] outArray, int Width, int Height, int TargetWidth, int TargetHeight)
        {
            for (int i = TargetHeight - 1; i >= 0; i--)
            {
                for (int j = TargetWidth - 1; j >= 0; j--)
                {
                    double Px = (double)((j * (double)(Width - 1)) / (TargetWidth - 1));
                    double Py = (double)((i * (double)(Height - 1)) / (TargetHeight - 1));

                    if (Math.Floor(Px) == Px && Math.Floor(Py) == Py)
                    {
                        int addr1 = (int)Math.Floor(Py) * Width * 3 + (int)Math.Floor(Px) * 3;
                        int addrO = i * TargetWidth * 3 + j * 3;
                        //Exact coordinates, just copy the pixel. 
                        outArray[addrO] = inArray[addr1];
                        outArray[addrO + 1] = inArray[addr1 + 1];
                        outArray[addrO + 2] = inArray[addr1 + 2];
                    }
                    else if (Math.Floor(Px) == Px)
                    {
                        //X cord is the same, only interpolate on y axis
                        int y1 = (int)Math.Floor(Py);
                        int y2 = (int)Math.Floor(Py) + 1;

                        double w1 = (y2 - Py) / (y2 - y1);
                        double w2 = (Py - y1) / (y2 - y1);
                        int addr1 = y1 * Width * 3 + (int)Math.Floor(Px) * 3;
                        int addr2 = y2 * Width * 3 + (int)Math.Floor(Px) * 3;
                        int addrO = i * TargetWidth * 3 + j * 3;
                        outArray[addrO] = (byte)(int)(w1 * inArray[addr1] + w2 * inArray[addr2]);
                        outArray[addrO + 1] = (byte)(int)(w1 * inArray[addr1 + 1] + w2 * inArray[addr2 + 1]);
                        outArray[addrO + 2] = (byte)(int)(w1 * inArray[addr1 + 2] + w2 * inArray[addr2 + 2]);
                    }
                    else if (Math.Floor(Py) == Py)
                    {
                        //Y cord is the same, only interpolate on x axis
                        int x1 = (int)Math.Floor(Px);
                        int x2 = (int)Math.Floor(Px) + 1;

                        double w1 = (x2 - Px);
                        double w2 = (Px - x1);

                        int addr1 = (int)Math.Floor(Py) * Width * 3 + x1 * 3;
                        int addr2 = (int)Math.Floor(Py) * Width * 3 + x2 * 3;
                        int addrO = i * TargetWidth * 3 + j * 3;
                        outArray[addrO] = (byte)(int)(w1 * inArray[addr1] + w2 * inArray[addr2]);
                        outArray[addrO + 1] = (byte)(int)(w1 * inArray[addr1 + 1] + w2 * inArray[addr2 + 1]);
                        outArray[addrO + 2] = (byte)(int)(w1 * inArray[addr1 + 2] + w2 * inArray[addr2 + 2]);
                    }
                    else
                    {
                        //Closest points to P
                        int x1 = (int)Math.Floor(Px);
                        int x2 = (int)Math.Floor(Px) + 1;
                        int y1 = (int)Math.Floor(Py);
                        int y2 = (int)Math.Floor(Py) + 1;
                        //3.Weights
                        double w11 = ((x2 - Px) * (y2 - Py));
                        double w12 = ((x2 - Px) * (Py - y1));
                        double w21 = ((Px - x1) * (y2 - Py));
                        double w22 = ((Px - x1) * (Py - y1));
                        //4.Assign colors
                        int addr1 = y1 * Width * 3 + x1 * 3;
                        int addr2 = y2 * Width * 3 + x1 * 3;
                        int addr3 = y1 * Width * 3 + x2 * 3;
                        int addr4 = y2 * Width * 3 + x2 * 3;
                        int addrO = i * TargetWidth * 3 + j * 3;

                        outArray[addrO] = (byte)(int)(w11 * inArray[addr1] + w12 * inArray[addr2] + w21 * inArray[addr3] + w22 * inArray[addr4]); //Blue
                        outArray[addrO + 1] = (byte)(int)(w11 * inArray[addr1 + 1] + w12 * inArray[addr2 + 1] + w21 * inArray[addr3 + 1] + w22 * inArray[addr4 + 1]); //Green
                        outArray[addrO + 2] = (byte)(int)(w11 * inArray[addr1 + 2] + w12 * inArray[addr2 + 2] + w21 * inArray[addr3 + 2] + w22 * inArray[addr4 + 2]); //Red
                    }
                }
            }
        }
    }
}
