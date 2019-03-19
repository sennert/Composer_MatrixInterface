#include "Composer_MatrixInterface.h"
#include <Wire.h>


#define NXP_INPUT      (0)  // For NXP9555
#define NXP_OUTPUT     (2)  // See data sheet
#define NXP_INVERT     (4)  // for details:
#define NXP_CONFIG     (6)  // http://www.ti.com/lit/ds/symlink/tca9555.pdf

#define IO_Expander1_adr      0x20  // i2c adress of Transport Buttons TCA9555 IO Expander
#define IO_Expander2_adr      0x24  // i2c adress of TCA9555 IO Expander for Buttons on the left

#define NUM_BUTTONS_EXP1      10
#define NUM_BUTTONS_EXP2      16    // Variable not in use
#define NUM_BUTTONS           26


// Initialize Class Variables //////////////////////////////////////////////////

void (*MATRIX_INTERFACE::user_playButton_handler)(bool);
void (*MATRIX_INTERFACE::user_stopButton_handler)(bool);
void (*MATRIX_INTERFACE::user_recordButton_handler)(bool);
void (*MATRIX_INTERFACE::user_tapButton_handler)(bool);
void (*MATRIX_INTERFACE::user_switches_handler)(bool,uint8_t);
void (*MATRIX_INTERFACE::user_functionButtons_handler)(bool,uint8_t);
void (*MATRIX_INTERFACE::user_snapshotButtons_handler)(bool,uint8_t);
void (*MATRIX_INTERFACE::user_modeButtons_handler)(bool,uint8_t);



// Constructors ////////////////////////////////////////////////////////////////

MATRIX_INTERFACE::MATRIX_INTERFACE(void) {

}


// Public Methods //////////////////////////////////////////////////////////////

void MATRIX_INTERFACE::begin() {
    Wire.begin();  // set pinPeripheral after this line!!!

    expanderSetInput16(IO_Expander1_adr, 0xFF03);    // set Inputs for first TCA, Pins 2-7 not in use, maybe set unconnected Inputs to Outputs?
    expanderSetInvert16(IO_Expander1_adr, 0xFFFF);

    expanderSetInput16(IO_Expander2_adr, 0xFFFF);    // set Inputs for second TCA, uses all 16 IO Pins as Inputs
    expanderSetInvert16(IO_Expander2_adr, 0xFFFF);
    
    for(int i=0; i<NUM_BUTTONS; i++){                // clear Buttonstate
      buttonState[i] = 0;
      buttonState_Old[i] = 1;
    }
    //user_playButton_handler = 0;
}

// I2C routines to talk to a PCA9555
void MATRIX_INTERFACE::expanderSetInput16(int i2caddr, int dir) {
    Wire.beginTransmission(i2caddr);
    Wire.write(NXP_CONFIG);
    Wire.write(0xff & dir);  // low byte
    Wire.write(dir >> 8);    // high byte
    Wire.endTransmission();
}

void MATRIX_INTERFACE::expanderSetInvert16(int i2caddr, int dir) {
    Wire.beginTransmission(i2caddr);
    Wire.write(NXP_INVERT);
    Wire.write(0xff & dir);  // low byte
    Wire.write(dir >> 8);    // high byte
    Wire.endTransmission();
}

int MATRIX_INTERFACE::expanderRead16(int i2caddr) {
    int _data = 0;
    Wire.beginTransmission(i2caddr);
    Wire.write(NXP_INPUT);
    Wire.endTransmission();
    Wire.requestFrom(i2caddr, 4);
    if (Wire.available()) {
      _data = Wire.read();
    }
    if (Wire.available()) {
      _data |= (Wire.read() << 8);
    }
    return _data;
}

/* 
Helper button functions, the data is updated every read_IO_Expander() call!
*/
bool MATRIX_INTERFACE::isKeyPressed(uint8_t k) {
    if (k > NUM_BUTTONS) return false;
  return buttonState[k];
}
bool MATRIX_INTERFACE::wasKeyPressed(uint8_t k) {
    if (k > NUM_BUTTONS) return false;
  return buttonState_Old[k];
}
bool MATRIX_INTERFACE::justPressed(uint8_t k) {
  return (isKeyPressed(k) & !wasKeyPressed(k));
}
bool MATRIX_INTERFACE::justReleased(uint8_t k) {
  return (!isKeyPressed(k) & wasKeyPressed(k));
}

//////////////////////////////////////////////////////////////

bool MATRIX_INTERFACE::read_IO_Expander(long _t){
  
    bool somethingChanged = false;    
    static int  oldData[2] = {0,0};              // 2 IO Expander
    int newData[2];

    memcpy(buttonState_Old, buttonState, sizeof(buttonState));    // copy old State to OLDSTATE
    
    newData[0] = expanderRead16(IO_Expander1_adr); 
    newData[1] = expanderRead16(IO_Expander2_adr); 
    
//    // Serial.print(newData[0]); 
//    // Serial.print("  -   "); 
//    // Serial.print(newData[1]); 
//    // Serial.println("--------"); 

    for(int g = 0; g < 2; g++){                 // for both TCA9555s do
      if (newData[g] != oldData[g]){
           if (newData[g] != -1) {        
                  processInputExpander1(newData[0]);
                  processInputExpander2(newData[1]);
            }
          oldData[g] = newData[g];   // something changed
          somethingChanged = true;          
      }
    }
    return somethingChanged;
}


void MATRIX_INTERFACE::processInputExpander1(int _newData){
   for (int i = 0; i < 2; i++) {
            if (_newData & (1 << i)) {                    
                    buttonState[(i)] = true;  // Button is pressed
                    if (justPressed(i)) {
                        // Serial.print("just pressed ");
                        switch(i){
                          case 0:
                            // Serial.println("Snapshot 1 Button");
                            if(user_snapshotButtons_handler){
                              user_snapshotButtons_handler(true, 0);
                            }                            
                            break;
                          case 1:
                            // Serial.println("Snapshot 2 Button");
                            if(user_snapshotButtons_handler){
                              user_snapshotButtons_handler(true, 1);
                            }                            
                            break;
                          default:
                            break;
                        }
                    }                
                   } else {
                    buttonState[(i)] = false;   // Button is released
                    if (justReleased((i))) {
                        // Serial.print("just released ");
                        switch(i){
                          case 0:
                            // Serial.println("Snapshot 1 Button");
                            if(user_snapshotButtons_handler){
                              user_snapshotButtons_handler(false, 0);
                            }                            
                            break;
                          case 1:
                            // Serial.println("Snapshot 2 Button");
                            if(user_snapshotButtons_handler){
                              user_snapshotButtons_handler(true, 1);
                            }                            
                            break;
                          default:
                            break;
                        }
                    }      
              }
     }
  
    for (int i = 8; i < 16; i++) {
            if (_newData & (1 << i)) {                    
                    buttonState[(i-6)] = true;  // Button is pressed, left 6 Buttons out that are not connected
                    if (justPressed(i-6)) {
                        // Serial.print("just pressed ");
                        switch(i){
                          case 15:
                            // Serial.println("Mode 1 Button");
                            // currentMode = 0;
                            if(user_modeButtons_handler){
                              user_modeButtons_handler(true, 0);
                            }                             
                            break;
                          case 14:
                            // Serial.println("Mode 2 Button");
                            // currentMode = 1;
                            if(user_modeButtons_handler){
                              user_modeButtons_handler(true, 1);
                            }                             
                            break;
                          case 13:
                            // Serial.println("Mode 3 Button");
                            // currentMode = 2;
                            if(user_modeButtons_handler){
                              user_modeButtons_handler(true, 2);
                            }                             
                            break;
                          case 12:
                            // Serial.println("Mode 4 Button");
                            // currentMode = 3;
                            if(user_modeButtons_handler){
                              user_modeButtons_handler(true, 3);
                            }                             
                            break;
                          case 11:
                            // Serial.println("Play Button");
                            if(user_playButton_handler){
                              user_playButton_handler(true);
                            }                            
                            break;
                          case 10:
                            // Serial.println("Stop Button");
                          if(user_stopButton_handler){
                              user_stopButton_handler(true);
                            }                            
                            break;
                          case  9:
                            // Serial.println("Arm Button");
                          if(user_recordButton_handler){
                              user_recordButton_handler(true);
                            }                            
                            break;
                          case 8:
                            // Serial.println("Tap Button");
                          if(user_tapButton_handler){
                              user_tapButton_handler(true);
                            }                            
                            break;
                          default:
                            break;
                        }
                    }                
                   } else {
                    buttonState[(i-6)] = false;   // Button is released
                    if (justReleased((i-6))) {
                        // Serial.print("just released ");
                        switch(i){
                          case 15:
                            // Serial.println("Mode 1 Button");
                            if(user_modeButtons_handler){
                              user_modeButtons_handler(false, 0);
                            }                             
                            break;
                          case 14:
                            // Serial.println("Mode 2 Button");
                            if(user_modeButtons_handler){
                              user_modeButtons_handler(false, 1);
                            }                             
                            break;
                          case 13:
                            // Serial.println("Mode 3 Button");
                            if(user_modeButtons_handler){
                              user_modeButtons_handler(false, 2);
                            }                             
                            break;
                          case 12:
                            // Serial.println("Mode 4 Button");
                            if(user_modeButtons_handler){
                              user_modeButtons_handler(false, 3);
                            }                             
                            break;
                          case 11:
                            // Serial.println("Play Button");
                            if(user_playButton_handler){
                              user_playButton_handler(false);
                            }
                            break;
                          case 10:
                            // Serial.println("Stop Button");
                            if(user_stopButton_handler){
                              user_stopButton_handler(false);
                            }                            
                            break;
                          case 9:
                            // Serial.println("Arm Button");
                          if(user_recordButton_handler){
                              user_recordButton_handler(false);
                            }                            
                            break;
                          case 8:
                            // Serial.println("Tap Button");
                          if(user_tapButton_handler){
                              user_tapButton_handler(false);
                            }                            
                            break;
                          default:
                            break;
                        }
                    }      
              }
     }
}

//////////////////////////////////////////////////////////////


void MATRIX_INTERFACE::processInputExpander2(int _newData){
    for (int i = 0; i < 16; i++) {
            if (_newData & (1 << i)) {
                    
                    buttonState[(i+NUM_BUTTONS_EXP1)] = true;  // Button is pressed
                    if (justPressed(i+NUM_BUTTONS_EXP1)) {
                        // Serial.print("just pressed ");
                        switch(i){
                          case 15:
                            // Serial.println("Switch 1");
                            user_switches_handler(true, 0);
                            break;
                          case 11:
                            // Serial.println("Switch 2");
                            user_switches_handler(true, 1);
                            break;
                          case 14:
                            // Serial.println("Switch 3");
                            user_switches_handler(true, 2);
                            break;
                          case 10:
                            // Serial.println("Switch 4");
                            user_switches_handler(true, 3);
                            break;
                          case 13:
                            // Serial.println("Switch 5");
                            user_switches_handler(true, 4);
                            break;
                          case 9:
                            // Serial.println("Switch 6");
                            user_switches_handler(true, 5);
                            break;
                          case 12:
                            // Serial.println("Switch 7");
                            user_switches_handler(true, 6);
                            break;
                          case 8:
                            // Serial.println("Switch 8");
                            user_switches_handler(true, 7);
                            break;
                          case 6:
                            // Serial.println("BTN 1");
                            user_functionButtons_handler(true, 0);
                            break;
                          case 7:
                            // Serial.println("BTN 2");
                            user_functionButtons_handler(true, 1);
                            break;
                          case 4:
                            // Serial.println("BTN 3");
                            user_functionButtons_handler(true, 2);
                            break;
                          case 5:
                            // Serial.println("BTN 4");
                            user_functionButtons_handler(true, 3);
                            break;
                          case 2:
                            // Serial.println("BTN 5");
                            user_functionButtons_handler(true, 4);
                            break;
                          case 3:
                            // Serial.println("BTN 6");
                            user_functionButtons_handler(true, 5);
                            break;
                          case 0:
                            // Serial.println("BTN 7");
                            user_functionButtons_handler(true, 6);
                            break;
                          case 1:
                            // Serial.println("BTN 8");
                            user_functionButtons_handler(true, 7);
                            break;
                          default:
                            break;
                        }
                    }                
                   } else {
                    buttonState[(i+NUM_BUTTONS_EXP1)] = false;   // Button is released
                    if (justReleased((i+NUM_BUTTONS_EXP1))) {
                        // Serial.print("just released ");
                        switch(i){
                          case 15:
                            // Serial.println("Switch 1");
                            user_switches_handler(false, 0);
                            break;
                          case 11:
                            // Serial.println("Switch 2");
                            user_switches_handler(false, 1);
                            break;
                          case 14:
                            // Serial.println("Switch 3");
                            user_switches_handler(false, 2);
                            break;
                          case 10:
                            // Serial.println("Switch 4");
                            user_switches_handler(false, 3);
                            break;
                          case 13:
                            // Serial.println("Switch 5");
                            user_switches_handler(false, 4);
                            break;
                          case 9:
                            // Serial.println("Switch 6");
                            user_switches_handler(false, 5);
                            break;
                          case 12:
                            // Serial.println("Switch 7");
                            user_switches_handler(false, 6);
                            break;
                          case 8:
                            // Serial.println("Switch 8");
                            user_switches_handler(false, 7);
                            break;
                          case 6:
                            // Serial.println("BTN 1");
                            user_functionButtons_handler(false, 0);
                            break;
                          case 7:
                            // Serial.println("BTN 2");
                            user_functionButtons_handler(false, 1);
                            break;
                          case 4:
                            // Serial.println("BTN 3");
                            user_functionButtons_handler(false, 2);
                            break;
                          case 5:
                            // Serial.println("BTN 4");
                            user_functionButtons_handler(false, 3);
                            break;
                          case 2:
                            // Serial.println("BTN 5");
                            user_functionButtons_handler(false, 4);
                            break;
                          case 3:
                            // Serial.println("BTN 6");
                            user_functionButtons_handler(false, 5);
                            break;
                          case 0:
                            // Serial.println("BTN 7");
                            user_functionButtons_handler(false, 6);
                            break;
                          case 1:
                            // Serial.println("BTN 8");
                            user_functionButtons_handler(false, 7);
                            break;
                          default:
                            break;
                        }
                    }      
              }
     }
}

// sets function called on slave read
void MATRIX_INTERFACE::attachHandler_PlayButton( void (*function)(bool _state) ){
  user_playButton_handler = function;
}
void MATRIX_INTERFACE::attachHandler_StopButton( void (*function)(bool _state) ){
  user_stopButton_handler = function;
}
void MATRIX_INTERFACE::attachHandler_RecordButton( void (*function)(bool _state) ){
  user_recordButton_handler = function;
}
void MATRIX_INTERFACE::attachHandler_TapButton( void (*function)(bool _state) ){
  user_tapButton_handler = function;
}
void MATRIX_INTERFACE::attachHandler_Switches( void (*function)(bool _state, uint8_t _button) ){
  user_switches_handler = function;
}
void MATRIX_INTERFACE::attachHandler_FunctionButtons( void (*function)(bool _state, uint8_t _button) ){
  user_functionButtons_handler = function;
}
void MATRIX_INTERFACE::attachHandler_SnapshotButtons( void (*function)(bool _state, uint8_t _button) ){
  user_snapshotButtons_handler = function;
}
void MATRIX_INTERFACE::attachHandler_ModeButtons( void (*function)(bool _state, uint8_t _button) ){
  user_modeButtons_handler = function;
}
#