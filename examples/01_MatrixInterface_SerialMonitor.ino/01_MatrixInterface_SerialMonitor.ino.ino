//#include <Wire.h>
#include <Adafruit_NeoPixel_ZeroDMA.h>
#include <Composer_MatrixInterface.h>

/////////////////////////////////////////////////////////////////

#define TCA9555_INT_PIN      A6

MATRIX_INTERFACE interface;

/////////////////////////////////////////////////////////////////

#define WS28_PIN  A3
#define NUM_LEDS 252
#define MATRIX_FRAMERATE 20
Adafruit_NeoPixel_ZeroDMA rgb_leds(NUM_LEDS, WS28_PIN, NEO_GRB);

/////////////////////////////////////////////////////////////////////////////////

void setup() {
  
  Serial.begin(115200);

  pinMode(TCA9555_INT_PIN, INPUT_PULLUP);     // INT pin requires a pullup
  pinMode(LED_BUILTIN, OUTPUT);      // to show we are working
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  
  interface.begin();
  interface.attachHandler_PlayButton(handler_PlayButton);
  interface.attachHandler_StopButton(handler_StopButton);
  interface.attachHandler_RecordButton(handler_RecordButton);
  interface.attachHandler_TapButton(handler_TapButton);
  interface.attachHandler_Switches(handler_Switches);
  interface.attachHandler_FunctionButtons(handler_FunctionButtons);
  interface.attachHandler_SnapshotButtons(handler_SnapshotButtons);
  interface.attachHandler_ModeButtons(handler_ModeButtons);

  rgb_leds.begin();  
  rgb_leds.setBrightness(35);
  rgb_leds.show();
  delay(100);
  drawLogo();
  rgb_leds.show();
  delay(1000);

  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off
  turnAllLEDsOff();
  rgb_leds.show();
}

/////////////////////////////////////////////////////////////////

long t = millis();
long lastButtonReadout = millis();
int currentMode = 2;
bool playing = true;

void loop() {
  
  t = millis();

  if (digitalRead(TCA9555_INT_PIN) == 0) {
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    interface.read_IO_Expander(t);
    lastButtonReadout = t;
  } else {  
    digitalWrite(LED_BUILTIN, LOW); 
  }
  updateMatrixLEDs(t);
}

/////////////////////////////////////////////////////////////////

void handler_PlayButton(bool _state){
  if(_state) {
    Serial.println("play button pressed");
  } else {
    Serial.println("play button released");
    playing = !playing;
  }
}

void handler_StopButton(bool _state){
  if(_state) {
    Serial.println("stop button pressed");
  } else {
    Serial.println("stop button released");
  }
}

void handler_RecordButton(bool _state){
  if(_state) {
    Serial.println("record button pressed");
  } else {
    Serial.println("record button released");
  }
}

void handler_TapButton(bool _state){
  if(_state) {
    Serial.println("tap button pressed");
  } else {
    Serial.println("tap button released");
  }
}

void handler_Switches(bool _state, uint8_t _button){
  Serial.print("Settings Button ");
  Serial.print(_button+1);
  Serial.print(" was ");

  if(_state) {
    Serial.println("pressed");
  } else {
    Serial.println("released");
  }
}

void handler_FunctionButtons(bool _state, uint8_t _button){
  Serial.print("Functions Button ");
  Serial.print(_button+1);
  Serial.print(" was ");

  if(_state) {
    Serial.println("pressed");
  } else {
    Serial.println("released");
  }
}

void handler_SnapshotButtons(bool _state, uint8_t _button){
  Serial.print("Snapshot Button ");
  Serial.print(_button+1);
  Serial.print(" was ");

  if(_state) {
    Serial.println("pressed");
  } else {
    Serial.println("released");
  }
}

void handler_ModeButtons(bool _state, uint8_t _button){
  Serial.print("Mode Button ");
  Serial.print(_button+1);
  Serial.print(" was ");

  if(_state) {
    Serial.println("pressed");
  } else {
    Serial.println("released");
  }
  currentMode = _button;
}
