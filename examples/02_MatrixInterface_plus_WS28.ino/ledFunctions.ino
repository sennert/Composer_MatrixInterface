//////////////////////////////// . LED FUCNTIONS /////////////////////////////////////////

//struct rgb_colour {
//  byte r;
//  byte g;
//  byte b;
//};
//
//
//rgb_colour leds[NUM_LEDS];

/////////////////////////////////////////////////////////////////////////////////

#define playButton_adr    4
#define stopButton_adr    5
#define recordButton_adr  6
#define tapButton_adr     7

uint8_t modusButtons_adr[4] = 
    { 0, 1, 2, 3 };

uint8_t machineStrip_adr[16] = 
    { 25, 24, 23, 22,  21, 20, 19, 18,  17, 16, 15, 14,  13, 12, 11, 10};

uint8_t stepMatrix_adr[12][16] = {
    { 30, 31, 32, 33,  34, 35, 36, 37,  38, 39, 40, 41,  42, 43, 44, 45},
    { 65, 64, 63, 62,  61, 60, 59, 58,  57, 56, 55, 54,  53, 52, 51, 50},
    { 70, 71, 72, 73,  74, 75, 76, 77,  78, 79, 80, 81,  82, 83, 84, 85},
    {105,104,103,102, 101,100, 99, 98,  97, 96, 95, 94,  93, 92, 91, 90},

    {108,109,110,111, 112,113,114,115, 116,117,118,119, 120,121,122,123}, 
    {143,142,141,140, 139,138,137,136, 135,134,133,132, 131,130,129,128},
    {144,145,146,147, 148,149,150,151, 152,153,154,155, 156,157,158,159},
    {179,178,177,176, 175,174,173,172, 171,170,169,168, 167,166,165,164},
    
    {180,181,182,183, 184,185,186,187, 188,189,190,191, 192,193,194,195},
    {215,214,213,212, 211,210,209,208, 207,206,205,204, 203,202,201,200},
    {216,217,218,219, 220,221,222,223, 224,225,226,227, 228,229,230,231},
    {251,250,249,248, 247,246,245,244, 243,242,241,240, 239,238,237,236}
};

uint8_t sceneButtons_adr[2][12] = {
    {235,232,199,196, 163,160,127,124,  89, 86, 49, 46},
    {234,233,198,197, 162,161,126,125,  88, 87, 48, 47}
};

#define sceneMaster1_adr 9
#define sceneMaster2_adr 8

#define snapShot1_adr 27
#define snapShot2_adr 26

uint8_t functionButtons_adr[2][4] = {
    { 28, 67, 68, 107 },
    { 29, 66, 69, 106 }
};

/////////////////////////////////////////////////////////////////////////////////

uint8_t iForXY(uint16_t _x,uint16_t _y){      // returns Index Address for give x and y coordinates of Matrix
  return stepMatrix_adr[_y][_x];
}


///* Converts a color from HSV to RGB.
// * h is hue, as a number between 0 and 360.
// * s is the saturation, as a number between 0 and 255.
// * v is the value, as a number between 0 and 255. */
//rgb_colour hsvToRgb(uint16_t h, uint8_t s, uint8_t v)
//{
//    uint8_t f = (h % 60) * 255 / 60;
//    uint8_t p = (255 - s) * (uint16_t)v / 255;
//    uint8_t q = (255 - f * (uint16_t)s / 255) * (uint16_t)v / 255;
//    uint8_t t = (255 - (255 - f) * (uint16_t)s / 255) * (uint16_t)v / 255;
//    uint8_t r = 0, g = 0, b = 0;
//    switch((h / 60) % 6){
//        case 0: r = v; g = t; b = p; break;
//        case 1: r = q; g = v; b = p; break;
//        case 2: r = p; g = v; b = t; break;
//        case 3: r = p; g = q; b = v; break;
//        case 4: r = t; g = p; b = v; break;
//        case 5: r = v; g = p; b = q; break;
//    }
//    return rgb_leds.Color(r, g, b);
//}


void showYBar(uint8_t _xpos){

//    if(playing){
//      xPos = timerCounter/(PPQ/4.0);   // steps = 16th, 0-384 / (96/4) = 384/24 = 16
//    } else {
//      xPos = 0;
//    }

    for(int y = 0; y < 12; y++){
      rgb_leds.setPixelColor(iForXY(_xpos,y), rgb_leds.Color(240,240,240));   // bright white
    }  
}


void showXBar(uint8_t _ypos){
    for(int x = 0; x < (16); x++){
      rgb_leds.setPixelColor(iForXY(x,_ypos), rgb_leds.Color(240,240,240));   // bright white
    }  
}

void showTrackBar(){
    for(int x = 0; x < (16); x++){
        rgb_leds.setPixelColor(machineStrip_adr[x], rgb_leds.Color(0x3A,0xEF,0xFF));   // 
    }  
}

void updateSceneButtons(){
  
    for(int y = 0; y < (12); y++){
        rgb_leds.setPixelColor(sceneButtons_adr[0][y], rgb_leds.Color(0xFF, 0xAD, 0x3A));          
    } 
    for(int y = 0; y < (12); y++){
        rgb_leds.setPixelColor(sceneButtons_adr[1][y], rgb_leds.Color(0xFF, 0x4A, 0x3A));   //      
    }  
}

void updateFunctionButtons(){
  
    for(int y = 0; y < (4); y++){
        rgb_leds.setPixelColor(functionButtons_adr[0][y], rgb_leds.Color(0xAD,0x3A,0xFF));          
    } 
    for(int y = 0; y < (4); y++){
        rgb_leds.setPixelColor(functionButtons_adr[1][y], rgb_leds.Color(0xFF,0x3A,0xEF));   //      
    }  
}

void updateModeButtons(uint8_t _currentMode){
  
    rgb_leds.setPixelColor(modusButtons_adr[0], rgb_leds.Color(0xDA,0xE2,0xE2));   // projectview
    rgb_leds.setPixelColor(modusButtons_adr[1], rgb_leds.Color(0xDA,0xE2,0xE2));   // sceneview
    rgb_leds.setPixelColor(modusButtons_adr[2], rgb_leds.Color(0xDA,0xE2,0xE2));   // sequeceview
    rgb_leds.setPixelColor(modusButtons_adr[3], rgb_leds.Color(0xDA,0xE2,0xE2));   // performview 

    rgb_leds.setPixelColor(modusButtons_adr[_currentMode], rgb_leds.Color(0x3A,0x8C,0xFF));   // sequeceview  
}


void updateMatrixLEDs(long _t) {
  static double lastMatrixUpdate = 0;
  static int xcounter = 0;
  
  if(_t-lastMatrixUpdate > MATRIX_FRAMERATE){
    
    turnAllLEDsOff();   // remove later

    updateModeButtons(currentMode);
    
    switch(currentMode){
      case 0:  // Project View
//          for(uint16_t i = 64; i < NUM_LEDS; i++)
//          {
//            leds[i] = hsvToRgb(20+(i*3), 255, 200);
//          }
//          leds[(iForXY( 6,10))] = rgb_color(0, 100, 200);      // projectview
          break;
      case 1:   // Scene View
//          showScene8x16();   
//          leds[(iForXY( 7,10))] = rgb_color(0, 160, 140);      // sceneview
          showTrackBar();
          updateSceneButtons();
          updateFunctionButtons();
          break;
      case 2:   // Step View
//          showSteps8x16(selected_Machine);   
          if(playing) {showYBar(xcounter/2);};       // turn column at Position x
          showTrackBar();
          updateSceneButtons();
          updateFunctionButtons();
//          leds[(iForXY( 8,10))] = rgb_color(20, 120, 150);      // sequeceview
          break;
      case 3:   // Key View
          showTrackBar();
//          showSteps4x16(selected_Machine);   
//          showYBar(xPos);       // turn column at Position x
          showKeyboard();
//          leds[(iForXY (9,10))] = rgb_color(0, 200, 150);      // performview
          break;
      default:
          break;
    }

    rgb_leds.setPixelColor(snapShot1_adr, rgb_leds.Color(150, 150, 150));   // Snapshot Button 1
    rgb_leds.setPixelColor(snapShot2_adr, rgb_leds.Color(30, 30, 30));      // Snapshot Button 2

    rgb_leds.setPixelColor(playButton_adr, rgb_leds.Color(20, 200, 0));     // playbutton
    rgb_leds.setPixelColor(stopButton_adr, rgb_leds.Color(0, 0, 200));      // stopbutton
    rgb_leds.setPixelColor(recordButton_adr, rgb_leds.Color(200, 0, 0));    // recordbutton
    rgb_leds.setPixelColor(tapButton_adr, rgb_leds.Color(200, 150, 0));     // tapbutton

    rgb_leds.setPixelColor(sceneMaster1_adr, rgb_leds.Color(0, 0, 240));    // SceneMaster1 button
    rgb_leds.setPixelColor(sceneMaster2_adr, rgb_leds.Color(0, 240, 0));    // SceneMaster2 button

    
    rgb_leds.show();

    xcounter++;
    if(xcounter>=32) xcounter = 0;
    
    lastMatrixUpdate=_t;
  }
}


void turnAllLEDsOff(){
      for (uint16_t i=0; i < rgb_leds.numPixels(); i++) {
        rgb_leds.setPixelColor(i, 0);    //turn every pixel off
      }
}



uint8_t logoCoordinates[][2] ={
  {1,8},{2,8},{6,8},{8,8},{9,8},{13,8},
  {1,7},{3,7},{5,7},{6,7},{8,7},{10,7},{12,7},{13,7},
  {1,6},{3,6},{5,6},{7,6},{8,6},{10,6},{12,6},{14,6},
  {1,5},{3,5},{5,5},{7,5},{8,5},{10,5},{12,5},{14,5},
  {1,4},{3,4},{4,4},{5,4},{6,4},{7,4},{8,4},{10,4},{11,4},{12,4},{13,4},{14,4},
  {1,3},{2,3},{4,3},{7,3},{8,3},{9,3},{11,3},{14,3}};


  
void drawLogo(){
 for(int x = 0; x <50; x++){
      rgb_leds.setPixelColor((iForXY((logoCoordinates[x][0]),(logoCoordinates[x][1]-0))), rgb_leds.Color((250), (250), (250)));   
  }
}


//void showSteps12x16(int _machine){
//    for(int y=0; y<12; y++){
//      for(int x=0; x<16; x++){
//        if (stepIsOn[(iForXY(x,y))]) {
////          leds[(iForXY(x,y))] =  hsvToRgb((((y+3)*7)-20), 255, 255);
//          rgb_leds.setPixelColor((iForXY(x,y)), (((y+3)*7)-20));
//
//        } else {
////          leds[(iForXY(x,y))] =  hsvToRgb(150, 0, 30);
//          rgb_leds.setPixelColor((iForXY(x,y)), rgb_leds.Color(40, 40, 40));
//        }
//      }
//    } 
//}

void showKeyboard(){
  showXBar(0);
  
  rgb_leds.setPixelColor((iForXY( 0,1)), rgb_leds.Color(0, 0, 0));     
  rgb_leds.setPixelColor((iForXY( 1,1)), rgb_leds.Color(0, 80, 200));     
  rgb_leds.setPixelColor((iForXY( 2,1)), rgb_leds.Color(0, 80, 200));     
  rgb_leds.setPixelColor((iForXY( 3,1)), rgb_leds.Color(0, 0, 0));     
  rgb_leds.setPixelColor((iForXY( 4,1)), rgb_leds.Color(0, 80, 200));     
  rgb_leds.setPixelColor((iForXY( 5,1)), rgb_leds.Color(0, 80, 200));     
  rgb_leds.setPixelColor((iForXY( 6,1)), rgb_leds.Color(0, 80, 200));  
  rgb_leds.setPixelColor((iForXY( 7,1)), rgb_leds.Color(0, 0, 0));     
  rgb_leds.setPixelColor((iForXY( 8,1)), rgb_leds.Color(0, 0, 0));    
  rgb_leds.setPixelColor((iForXY( 9,1)), rgb_leds.Color(0, 80, 200));     
  rgb_leds.setPixelColor((iForXY(10,1)), rgb_leds.Color(0, 80, 200));     
  rgb_leds.setPixelColor((iForXY(11,1)), rgb_leds.Color(0, 0, 0));     
  rgb_leds.setPixelColor((iForXY(12,1)), rgb_leds.Color(0, 80, 200));     
  rgb_leds.setPixelColor((iForXY(13,1)), rgb_leds.Color(0, 80, 200));     
  rgb_leds.setPixelColor((iForXY(14,1)), rgb_leds.Color(0, 80, 200)); 
  rgb_leds.setPixelColor((iForXY(15,1)), rgb_leds.Color(0, 0, 0));     
  showXBar(2);
  rgb_leds.setPixelColor((iForXY( 0,3)), rgb_leds.Color(0, 0, 0));     
  rgb_leds.setPixelColor((iForXY( 1,3)), rgb_leds.Color(0, 80, 200));     
  rgb_leds.setPixelColor((iForXY( 2,3)), rgb_leds.Color(0, 80, 200));     
  rgb_leds.setPixelColor((iForXY( 3,3)), rgb_leds.Color(0, 0, 0));     
  rgb_leds.setPixelColor((iForXY( 4,3)), rgb_leds.Color(0, 80, 200));     
  rgb_leds.setPixelColor((iForXY( 5,3)), rgb_leds.Color(0, 80, 200));     
  rgb_leds.setPixelColor((iForXY( 6,3)), rgb_leds.Color(0, 80, 200));  
  rgb_leds.setPixelColor((iForXY( 7,3)), rgb_leds.Color(0, 0, 0));    
  rgb_leds.setPixelColor((iForXY( 8,3)), rgb_leds.Color(0, 0, 0));   
  rgb_leds.setPixelColor((iForXY( 9,3)), rgb_leds.Color(0, 80, 200));     
  rgb_leds.setPixelColor((iForXY(10,3)), rgb_leds.Color(0, 80, 200));     
  rgb_leds.setPixelColor((iForXY(11,3)), rgb_leds.Color(0, 0, 0));     
  rgb_leds.setPixelColor((iForXY(12,3)), rgb_leds.Color(0, 80, 200));     
  rgb_leds.setPixelColor((iForXY(13,3)), rgb_leds.Color(0, 80, 200));     
  rgb_leds.setPixelColor((iForXY(14,3)), rgb_leds.Color(0, 80, 200)); 
  rgb_leds.setPixelColor((iForXY(15,3)), rgb_leds.Color(0, 0, 0));     
}
