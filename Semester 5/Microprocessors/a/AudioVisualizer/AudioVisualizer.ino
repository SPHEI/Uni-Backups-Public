#include <Adafruit_GFX.h>  
#include <XTronical_ST7735.h> 
#include <SPI.h>
#include <arduinoFFT.h>

#define TFT_DC 2 
#define TFT_RST 4    
#define TFT_CS 5   

#define MIC_PIN 35 
#define BUTTON_PIN 25
#define COLOR_PIN 26

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);  

const uint16_t samples = 1024; //This value MUST ALWAYS be a power of 2
const double samplingFrequency = 40000;

double vReal[samples];
double vImag[samples];

int outBins[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int lastBins[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

ArduinoFFT<double> FFT = ArduinoFFT<double>(vReal, vImag, samples, samplingFrequency);

int barMult = 16;
int noiseGate = 0;

unsigned int sampling_period_us;
unsigned long microseconds;

int displayMode = 0;
int colors[] = {0xF900, 0x07E0, 0x001f, 0xffe0};
int currentColor = 0; 

void setup(void) {
  sampling_period_us = round(1000000*(1.0/samplingFrequency));
  tft.init();  
  tft.setRotation(2);
  for(int i = 0; i < 16; i++)
  {
    lastBins[i] = 0;
  }
  tft.fillScreen(ST7735_BLACK);
}

bool pressed = false;
bool cpressed = false;
int cnt = 0;
int ccnt = 0;
void loop() {
  HandleInput();
  for(int i = 0; i < 16; i++)
  {
    outBins[i] = 0;
  }
  for(int i=0; i<samples; i++)
  {
    microseconds = micros();
    vReal[i] = analogRead(MIC_PIN);
    vImag[i] = 0;
    while(micros() - microseconds < sampling_period_us){/*Wait*/}
    microseconds += sampling_period_us;
  }
  FFT.dcRemoval();
  FFT.windowing(FFTWindow::Hamming, FFTDirection::Forward);	
  FFT.compute(FFTDirection::Forward);            
  FFT.complexToMagnitude();       

  switch(displayMode)
  {
    case 0:
    DisplayBars();
    break;
    case 1:
    DisplayCircle();
    break;
    case 2:
    Display2Bars();
    break;
    case 3:
    DisplayLines();
    break;
  }
}

void HandleInput()
{
  if(analogRead(BUTTON_PIN) == 0)
  {
    cnt++;
    if(cnt > 10 && !pressed)
    {
      pressed = true;
      displayMode++;
      tft.fillScreen(ST7735_BLACK);
      if(displayMode > 3)
      {
        displayMode = 0;
      }
    }
  }
  else
  {
    cnt = 0;
    pressed = false;
  }
  if(analogRead(COLOR_PIN) == 0)
  {
    ccnt++;
    if(ccnt > 10 && !cpressed)
    {
      cpressed = true;
      currentColor++;
      tft.fillScreen(ST7735_BLACK);
      if(currentColor > 3)
      {
        currentColor = 0;
      }
    }
  }
  else
  {
    ccnt = 0;
    cpressed = false;
  }
}

void DisplayBars()
{
  for(int i = 2; i < samples/2;i++)     
  {
    if(vReal[i] > noiseGate)
    {
      if(i <= 2)                outBins[0]  += (int)vReal[i]*2;
      if(i > 2 && i <= 3)       outBins[1]  += (int)vReal[i]*1.5;
      if(i > 3 && i <= 5)       outBins[2]  += (int)vReal[i];
      if(i > 5 && i <= 7)       outBins[3]  += (int)vReal[i];
      if(i > 7 && i <= 9)       outBins[4]  += (int)vReal[i];
      if(i > 9 && i <= 13)      outBins[5]  += (int)vReal[i];
      if(i > 13 && i <= 18)     outBins[6]  += (int)vReal[i];
      if(i > 18 && i <= 25)     outBins[7]  += (int)vReal[i];
      if(i > 25 && i <= 36)     outBins[8]  += (int)vReal[i];
      if(i > 36 && i <= 50)     outBins[9]  += (int)vReal[i];
      if(i > 50 && i <= 69)     outBins[10] += (int)vReal[i];
      if(i > 69 && i <= 97)     outBins[11] += (int)vReal[i];
      if(i > 97 && i <= 135)    outBins[12] += (int)vReal[i];
      if(i > 135 && i <= 189)   outBins[13] += (int)vReal[i];
      if(i > 189 && i <= 264)   outBins[14] += (int)vReal[i]/1.5;
      if(i > 264)               outBins[15] += (int)vReal[i]/2;
    }
  }    
  int color = colors[currentColor];
  for (int i = 0; i < 16; i++) { 
    color *= 0.999999;
    if(outBins[i] >= lastBins[i])
    {
      tft.fillRect(120 - (i * 8),0,8, (outBins[i]/samplingFrequency) * barMult,color);
    }
    else
    {
      tft.fillRect(120 - (i * 8),(outBins[i]/samplingFrequency) * barMult,8, 128 - ((outBins[i]/samplingFrequency) * barMult) + 1,ST7735_BLACK);
    }
    lastBins[i] = outBins[i];
  }
}

void DisplayCircle()
{
  for(int i = 2; i < samples/2;i++)     
  {
    if(vReal[i] > noiseGate)
    {
      if(i <= 2)                outBins[0]  += (int)vReal[i]*2;
      if(i > 2 && i <= 3)       outBins[0]  += (int)vReal[i]*1.5;
      if(i > 3 && i <= 5)       outBins[0]  += (int)vReal[i];
      if(i > 5 && i <= 7)       outBins[0]  += (int)vReal[i];
      if(i > 7 && i <= 9)       outBins[0]  += (int)vReal[i];
      if(i > 9 && i <= 13)      outBins[0]  += (int)vReal[i];
      if(i > 13 && i <= 18)     outBins[1]  += (int)vReal[i];
      if(i > 18 && i <= 25)     outBins[1]  += (int)vReal[i];
      if(i > 25 && i <= 36)     outBins[1]  += (int)vReal[i];
      if(i > 36 && i <= 50)     outBins[1]  += (int)vReal[i];
      if(i > 50 && i <= 69)     outBins[2] += (int)vReal[i];
      if(i > 69 && i <= 97)     outBins[2] += (int)vReal[i];
      if(i > 97 && i <= 135)    outBins[2] += (int)vReal[i];
      if(i > 135 && i <= 189)   outBins[2] += (int)vReal[i];
      if(i > 189 && i <= 264)   outBins[2] += (int)vReal[i]/1.5;
      if(i > 264)               outBins[2] += (int)vReal[i]/2;
    }
  } 

  int color = colors[currentColor];
  tft.drawCircle(63, 63, (lastBins[15]/samplingFrequency)*2,ST7735_BLACK);
  tft.drawCircle(63, 63, (lastBins[14]/samplingFrequency)*2,color * 0.7);
  tft.drawCircle(63, 63, (lastBins[13]/samplingFrequency)*2,color * 0.85);
  tft.drawCircle(63, 63, (lastBins[0]/samplingFrequency)*2,color * 0.9);
  tft.drawCircle(63, 63, (outBins[0]/samplingFrequency)*2,color);
  

  lastBins[15] = lastBins[14];
  lastBins[14] = lastBins[13];
  lastBins[13] = lastBins[0];
  lastBins[0] = outBins[0];

  if(lastBins[1] > outBins[1])
  {
    tft.fillCircle(63, 127, (lastBins[1]/samplingFrequency)*1,ST7735_BLACK);
  }
  tft.fillCircle(63, 127, (outBins[1]/samplingFrequency)*1,ST7735_BLUE);
  lastBins[1] = outBins[1];

  if(lastBins[2] > outBins[2])
  {
    tft.fillCircle(63, 0, (lastBins[2]/samplingFrequency)*0.5,ST7735_BLACK);
  }
  tft.fillCircle(63, 0, (outBins[2]/samplingFrequency)*0.5,ST7735_GREEN);
  lastBins[2] = outBins[2];
}

void Display2Bars()
{
  for(int i = 2; i < samples/2;i++)     
  {
    if(vReal[i] > noiseGate)
    {
      if(i <= 2)                outBins[0]  += (int)vReal[i]*2;
      if(i > 2 && i <= 3)       outBins[1]  += (int)vReal[i]*1.5;
      if(i > 3 && i <= 5)       outBins[2]  += (int)vReal[i];
      if(i > 5 && i <= 7)       outBins[3]  += (int)vReal[i];
      if(i > 7 && i <= 9)       outBins[4]  += (int)vReal[i];
      if(i > 9 && i <= 13)      outBins[5]  += (int)vReal[i];
      if(i > 13 && i <= 18)     outBins[6]  += (int)vReal[i];
      if(i > 18 && i <= 25)     outBins[7]  += (int)vReal[i];
      if(i > 25 && i <= 36)     outBins[8]  += (int)vReal[i];
      if(i > 36 && i <= 50)     outBins[9]  += (int)vReal[i];
      if(i > 50 && i <= 69)     outBins[10] += (int)vReal[i];
      if(i > 69 && i <= 97)     outBins[11] += (int)vReal[i];
      if(i > 97 && i <= 135)    outBins[12] += (int)vReal[i];
      if(i > 135 && i <= 189)   outBins[13] += (int)vReal[i];
      if(i > 189 && i <= 264)   outBins[14] += (int)vReal[i]/1.5;
      if(i > 264)               outBins[15] += (int)vReal[i]/2;
    }
  }    
  int color = colors[currentColor];
  for (int i = 0; i < 16; i++) { 
    color *= 0.999999;
    if(outBins[i] >= lastBins[i])
    {
      tft.fillRect(120 - (i * 8),63 - (outBins[i]/samplingFrequency) * barMult/2,8, (outBins[i]/samplingFrequency) * barMult + 1,color);
    }
    else
    {
      tft.fillRect(120 - (i * 8),64 + (outBins[i]/samplingFrequency) * barMult/2,8, (128 - ((outBins[i]/samplingFrequency) * barMult) + 1)/2,ST7735_BLACK);
      tft.fillRect(120 - (i * 8),0,8, (128 - ((outBins[i]/samplingFrequency) * barMult) + 1)/2,ST7735_BLACK);
    }
    lastBins[i] = outBins[i];
  }
}

void DisplayLines()
{
  for(int i = 2; i < samples/2;i++)     
  {
    if(vReal[i] > noiseGate)
    {
      if(i <= 2)                outBins[0]  += (int)vReal[i]*2;
      if(i > 2 && i <= 3)       outBins[1]  += (int)vReal[i]*1.5;
      if(i > 3 && i <= 5)       outBins[2]  += (int)vReal[i];
      if(i > 5 && i <= 7)       outBins[3]  += (int)vReal[i];
      if(i > 7 && i <= 9)       outBins[4]  += (int)vReal[i];
      if(i > 9 && i <= 13)      outBins[5]  += (int)vReal[i];
      if(i > 13 && i <= 18)     outBins[6]  += (int)vReal[i];
      if(i > 18 && i <= 25)     outBins[7]  += (int)vReal[i];
      if(i > 25 && i <= 36)     outBins[8]  += (int)vReal[i];
      if(i > 36 && i <= 50)     outBins[9]  += (int)vReal[i];
      if(i > 50 && i <= 69)     outBins[10] += (int)vReal[i];
      if(i > 69 && i <= 97)     outBins[11] += (int)vReal[i];
      if(i > 97 && i <= 135)    outBins[12] += (int)vReal[i];
      if(i > 135 && i <= 189)   outBins[13] += (int)vReal[i];
      if(i > 189 && i <= 264)   outBins[14] += (int)vReal[i]/1.5;
      if(i > 264)               outBins[15] += (int)vReal[i]/2;
    }
  }    
  int color = colors[currentColor];
  tft.fillScreen(ST7735_BLACK);
  color *= 0.999999;
  tft.drawLine(63,127, (outBins[0]/samplingFrequency) * barMult/2 + 64, 120, color);
  for (int i = 1; i < 16; i++) { 
    color *= 0.999999;
    if(i%2 == 0)
    {
      tft.drawLine((outBins[i]/samplingFrequency) * barMult/2 + 64,120 - (i * 8) , -((outBins[i-1]/samplingFrequency) * barMult/2) + 64, 120 - ((i - 1) * 8),color);
    }
    else
    {
      tft.drawLine(-((outBins[i]/samplingFrequency) * barMult/2) + 64, 120 - (i * 8) , (outBins[i-1]/samplingFrequency) * barMult/2 + 64, 120 - ((i - 1) * 8),color);
    }
    lastBins[i] = outBins[i];
  }
}