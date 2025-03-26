using System.Diagnostics;
using System.Drawing.Imaging;
using System.Runtime.InteropServices;
using System.ComponentModel;
using CSharp;
namespace ImageResizingWinForms;

/*
 * Bilinear interpolation project. - GUI.
 * Winter Semester 2024/2025
 * Authors: Jakub (https://github.com/SPHEI), Alicja (https://github.com/Smoka24)
 * Version 1.0
 */
public partial class Form1 : Form
    {
        Bitmap imgToConvert;
        Bitmap imgConverted;
        Byte[] imgConvertedBytes;
        Byte[] imgToConvertBytes;

        public Form1()
        {
            InitializeComponent();
            saveFileDialog1.Filter = "PNG Image|*.png|JPEG Image|*.jpg|Bitmap Image|*.bmp";
            openFileDialog1.Filter = "PNG Image|*.png|JPEG Image|*.jpg|Bitmap Image|*.bmp";
        }
        //DLL IMPORT SECTION
        /*
         * Assembly implementation of the interpolation procedure.
         * Inputs:
         *  inArray - The input image in the form of an array of bytes. The image is represented as color values in Blue-Green-Red order going from top-left to bottom-right of the image.
         *  Width - Width of the original image.
         *  Height - Height of the original image.
         *  TargetWidth - Width of the output image.
         *  TargetHeight - Height of the output image.
         * Output:
         *  outArray - The output array holding the new image. It is initialized with 0s and passed to the program as one of the inputs.
         */
        [DllImport("Asm.dll", CallingConvention = CallingConvention.StdCall)]
        private static extern void interpolate(byte[] inArray, byte[] outArray, int Width, int Height, int TargetWidth, int TargetHeight);

    Interpolator interp = new Interpolator();
        //DLL IMPORT SECTION END

    /*
     * Short function handling image uploading.
     */
    private void btnUpload_Click(object sender, EventArgs e)
        {
            String path = " ";
            path = txtPath.Text;
            String extension = " ";

            if (extension != ".png" || extension != ".jpeg" || extension != ".jpg" || extension != ".bmp")
            {
                openFileDialog1.ShowDialog();
                path = openFileDialog1.FileName;

                extension = Path.GetExtension(path);
                txtPath.Text = path;

                if (extension == ".png" || extension == ".jpeg" || extension == ".jpg" || extension == ".bmp")
                {
                    imgToConvert = new Bitmap(path);
                    pixelBox.Image = imgToConvert;
                }
                else
                {
                    MessageBox.Show("Wrong file format. Try again with png, jpg, jpeg or bitmap.");
                }
            }


        }
        //Copied code
        /*
         * This function turns a bitmap into an array of bytes.
         * The main purpose of this function is to remove any padding the bitmap might have.
         * Input - the bitmap to convert.
         * Output - resulting array of bytes.
         */
        public byte[] GetRgbValues(Bitmap bitmap)
        {
            // Ensure the bitmap is in 24bpp RGB format.
            if (bitmap.PixelFormat != PixelFormat.Format24bppRgb)
            {
                bitmap = bitmap.Clone(new Rectangle(0, 0, bitmap.Width, bitmap.Height), PixelFormat.Format24bppRgb);
            }

            // Lock the bitmap's bits.
            Rectangle rect = new Rectangle(0, 0, bitmap.Width, bitmap.Height);
            BitmapData bmpData = bitmap.LockBits(rect, ImageLockMode.ReadOnly, PixelFormat.Format24bppRgb);

            // Calculate row width in bytes (without padding).
            int bytesPerPixel = 3; // For 24bpp RGB.
            int rowWidth = bitmap.Width * bytesPerPixel;

            // Create an array to hold the RGB values without padding.
            byte[] rgbValues = new byte[rowWidth * bitmap.Height];

            // Copy each row into the rgbValues array, skipping the padding.
            unsafe
            {
                byte* src = (byte*)bmpData.Scan0.ToPointer();
                for (int y = 0; y < bitmap.Height; y++)
                {
                    int srcOffset = y * bmpData.Stride;
                    int destOffset = y * rowWidth;

                    // Copy the row without the padding.
                    System.Buffer.BlockCopy(new Span<byte>(src + srcOffset, rowWidth).ToArray(), 0, rgbValues, destOffset, rowWidth);
                }
            }

            // Unlock the bits.
            bitmap.UnlockBits(bmpData);

            //IMPORTANT - BRG ORDER NOT RGB
            return rgbValues;
        }
        /*
         * This function turns an array of bytes into a bitmap.
         * The main purpose of this function is to add padding if necessary.
         * Inputs:
         *  rgbValues - the byte array to convert.
         *  width - the width of the image.
         *  height - the height of the image.
         * Output - resulting bitmap.
         */
        public Bitmap ByteArrayToBitmap(byte[] rgbValues, int width, int height)
        {
            // Calculate bytes per pixel based on pixel format
            int bytesPerPixel = Image.GetPixelFormatSize(PixelFormat.Format24bppRgb) / 8;

            // Calculate the stride (actual row width including padding for the bitmap)
            int stride = ((width * bytesPerPixel + 3) / 4) * 4; // Align to a multiple of 4 bytes

            // Verify that the input byte array length matches the expected size without padding
            int expectedArrayLength = width * height * bytesPerPixel;
            if (rgbValues.Length != expectedArrayLength)
            {
                throw new ArgumentException("The length of the byte array does not match the expected size.");
            }

            // Create a new bitmap
            Bitmap bitmap = new Bitmap(width, height, PixelFormat.Format24bppRgb);

            // Lock the bitmap's bits
            Rectangle rect = new Rectangle(0, 0, width, height);
            BitmapData bmpData = bitmap.LockBits(rect, ImageLockMode.WriteOnly, PixelFormat.Format24bppRgb);

            // Add padding manually when copying rows
            for (int y = 0; y < height; y++)
            {
                // Get pointer to the start of the row in the bitmap
                IntPtr rowPtr = IntPtr.Add(bmpData.Scan0, y * bmpData.Stride);

                // Copy the row data from the input array to the bitmap
                int rowStartIndex = y * width * bytesPerPixel;
                System.Runtime.InteropServices.Marshal.Copy(rgbValues, rowStartIndex, rowPtr, width * bytesPerPixel);
            }

            // Unlock the bits
            bitmap.UnlockBits(bmpData);

            return bitmap;
        }


        //Copied code end

        /*
         * Handles converting the image.
         * Checks all of the settings set by the user and then runs the interpolation function.
         * Additionally it also measures and displays the execution time of the function.
         */
        private void btnConvert_Click(object sender, EventArgs e)
        {
            if (pixelBox.Image != null)
            {
                FunnyButton.Enabled = false;
                btnConvert.Enabled = false;
                btnUpload.Enabled = false; 
                btnSave.Enabled = false;
                imgToConvertBytes = GetRgbValues(imgToConvert);
                imgConvertedBytes = new Byte[(int)nHeight.Value * (int)nWidth.Value * 3];
                for (int i = 0; i < (int)nHeight.Value * (int)nWidth.Value * 3; i++)
                {
                    imgConvertedBytes[i] = 0;
                }

                Stopwatch stopwatch = new Stopwatch();
                stopwatch.Start();
                ///ASM CODE
                if (radioButton2.Checked)
                {
                    interpolate(imgToConvertBytes, imgConvertedBytes, imgToConvert.Width, imgToConvert.Height, (int)nWidth.Value, (int)nHeight.Value);
                }
                else
                {
                    interp.Interpolate(imgToConvertBytes, imgConvertedBytes, imgToConvert.Width, imgToConvert.Height, (int)nWidth.Value, (int)nHeight.Value);
                }
                stopwatch.Stop();

                imgConverted = ByteArrayToBitmap(imgConvertedBytes, (int)nWidth.Value, (int)nHeight.Value);
                pixelBox.Image = imgConverted;
                lblTime.Text = "Code execution time: " + stopwatch.ElapsedMilliseconds.ToString() + " ms";
                FunnyButton.Enabled = true;
                btnConvert.Enabled = true;
                btnUpload.Enabled = true;
                btnSave.Enabled = true;
            }
            else
            {
                MessageBox.Show("No image to convert.");
            }
        }

        /*
         * Saves the result image to a file.
         */
        private void btnSave_Click(object sender, EventArgs e)
        {
            if (imgConverted != null)
            {
                saveFileDialog1.ShowDialog();

                //Zakomentowane do testów bez assemblera
                //imgConverted = (Bitmap)((new ImageConverter()).ConvertFrom(imgConvertedBytes));
                if (saveFileDialog1.FileName != "")
                {
                    imgConverted.Save(saveFileDialog1.FileName);
                }

            }
            else
            {
                MessageBox.Show("No image to save.");
            }

        }

        

        /*
         * Goes through the same steps as btnConvert_Click.
         * The main difference is that it doesn't convert to the size set by the user, instead it loops through sizes from 2x2 to 1500x1500.
         * Additionally we used this function to get data for speed comparison.
         */
        private void FunnyButton_Click(object sender, EventArgs e)
        {
            if (pixelBox.Image != null)
            { 
            BackgroundWorker bw = new BackgroundWorker();
            imgToConvertBytes = GetRgbValues(imgToConvert);
            FunnyButton.Enabled = false;
            btnConvert.Enabled = false;
            btnUpload.Enabled = false;
            btnSave.Enabled = false;
            bw.DoWork += (s, e) =>
            {
                /*
                StreamWriter f;
                if (radioButton2.Checked)
                {
                    f = File.CreateText("Asm.txt");
                }
                else
                {
                    f = File.CreateText("C#.txt");
                }
                */
                for (int i = 2; i < 1500; i = (int)Math.Ceiling(i * 1.1)) //Change last part to i++ when using this function for data gathering.
                {
                    imgConvertedBytes = new Byte[i * i * 3];
                    for (int j = 0; j < i * i * 3; j++)
                    {
                        imgConvertedBytes[j] = 0;
                    }
                    Stopwatch stopwatch = new Stopwatch();
                    stopwatch.Start();
                    ///ASM CODE
                    if (radioButton2.Checked)
                    {
                        interpolate(imgToConvertBytes, imgConvertedBytes, imgToConvert.Width, imgToConvert.Height, i, i);
                    }
                    else
                    {
                        interp.Interpolate(imgToConvertBytes, imgConvertedBytes, imgToConvert.Width, imgToConvert.Height, i, i);
                    }
                    stopwatch.Stop();
                    imgConverted = ByteArrayToBitmap(imgConvertedBytes, i, i);
                    pixelBox.Invoke((MethodInvoker)(() =>
                    {
                        pixelBox.Image = imgConverted;
                        funnyBar.Value = i;
                        lblTime.Text = "Code execution time: " + stopwatch.ElapsedMilliseconds.ToString() + " ms";
                    }));
                    //f.WriteLine(i + ";" + stopwatch.ElapsedMilliseconds.ToString());
                    Thread.Sleep(1);
                }
                pixelBox.Invoke((MethodInvoker)(() =>
                {
                    pixelBox.Image = imgConverted;
                    funnyBar.Value = 2000;
                    FunnyButton.Enabled = true;
                    btnConvert.Enabled = true;
                    btnUpload.Enabled = true;
                    btnSave.Enabled = true;
                }));
                //f.Close();
            };
            bw.RunWorkerAsync();
            }
            else
            {
                MessageBox.Show("No image to convert.");
            }
        }
    }
