//https://www.xsimulator.net/community/threads/simtools-v3-rpm-dash-example-with-arduino-sketch.17871/

//################################################
// RPM Dash Example
// 4/8/2023
// Started by Dustin Arrell
//################################################
#include <FastLED.h>

//**************** Leds *****************
#define NUM_LEDS 60 //The number of led's used in your led strip
#define COLOR2_START 20 //Switch to color #2 after this led
#define COLOR3_START 40 //Switch to color #3 after this led
#define BRIGHTNESS 255 //255 max
#define DATA_LEDPIN1 12 //LED output pin1
//#define DATA_LEDPIN2 13 //add another led strip?

//************** Variables **************
int RpmOld = 0, bufferArray[2];
CRGB color_1, color_2, color_3, leds[NUM_LEDS];

//**************** Setup ****************
void setup() {
  //Led Colors (Know that some LED Strips swap R and G)
  color_1.setRGB( 255, 0, 0); //Set color #1
  color_2.setRGB( 0, 255, 0); //Set color #2
  color_3.setRGB( 0, 0, 255); //Set color #3

  //Initialize the LED Strip pin's
  FastLED.addLeds<WS2812B, DATA_LEDPIN1, RGB>(leds, NUM_LEDS);  
  //FastLED.addLeds<WS2812B, DATA_LEDPIN2, RGB>(leds, NUM_LEDS); //add another strip

  //Set Brightness
  FastLED.setBrightness(BRIGHTNESS);
  
  //Color_1
  Set_Color1();
  FastLED.show();   
  delay(250);

 //Color_2 
  Set_Color2();
  FastLED.show();  
  delay(250);
  
  //Color_3
  Set_Color3(); 
  FastLED.show();  
  delay(250);
  
  //Clear
  Clear_Strip();  
  FastLED.show(); 

  //Start Serial
  Serial.begin(115200); // opens serial port, sets data rate to 11520 bps
}

//**************** Loop *****************
void loop(){   
  if (Serial.available() >= 2){ //if 2 bits available in the serial buffer...     
    bufferArray[0] = Serial.read();    
    if (bufferArray[0] == 'R'){ 
      bufferArray[0] = Serial.read(); 

      //Set sign      
      bufferArray[1] = map(bufferArray[0], 0, 255, 0, NUM_LEDS);
      if (RpmOld != bufferArray[1]){
        RpmOld = bufferArray[1];          
        Set_Strip(bufferArray[1]);        
        FastLED.show();    
      }
    }
  } 
}

//************ Set Color 1 **************
void Set_Color1 (){ 
  //set all leds to color_1
   for (int i=0; i < NUM_LEDS; i++) {
    leds[i] = color_1;
  }  
}

//************ Set Color 2 **************
void Set_Color2 (){ 
  //set all leds to color_2
  for (int i=0; i < NUM_LEDS; i++) {
    leds[i] = color_2;
  }  
}

//************ Set Color 3 **************
void Set_Color3 (){
  //set all leds to color_3
  for (int i=0; i < NUM_LEDS; i++) {
    leds[i] = color_3;
  }  
}

//************ Clear Leds ***************
void Clear_Strip (){
  //set all leds off
  for (int i=0; i < NUM_LEDS; i++) {  
    leds[i] = CRGB::Black;
  } 
}

//************** Set Strip ***************
void Set_Strip (int End){ 
  if (End < COLOR2_START){    
    for (int a=0; a < End; a++) {
      leds[a] = color_1;
    }
    for (int b=End; b < NUM_LEDS; b++) {
      leds[b] = CRGB::Black;
    }            
  } else { 
    for (int s=0; s < COLOR2_START; s++) {
      leds[s] = color_1;
    }
    if (End < COLOR3_START){      
      for (int q=COLOR2_START; q < End; q++) {
        leds[q] = color_2;
      } 
      for (int c=End; c < NUM_LEDS; c++) {
        leds[c] = CRGB::Black;
      }    
    } else {      
      for (int w=COLOR2_START; w < COLOR3_START; w++) {
        leds[w] = color_2;
      }       
      for (int e=COLOR3_START; e < End; e++) {
        leds[e] = color_3;
      } 
      for (int f=End; f < NUM_LEDS; f++) {
        leds[f] = CRGB::Black;
      }      
    }             
  }        
}
