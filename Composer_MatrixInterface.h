#ifndef _MATRIX_INTERFACE_H_
#define _MATRIX_INTERFACE_H_
#include <Wire.h>


class MATRIX_INTERFACE {
 public:
  	MATRIX_INTERFACE(void);
  	void begin();
  
	bool isKeyPressed(uint8_t k); 
	bool wasKeyPressed(uint8_t k);
	bool justPressed(uint8_t k);
	bool justReleased(uint8_t k);

	bool read_IO_Expander(long _t);
	void processInputExpander1(int _newData);
	void processInputExpander2(int _newData);

	void attachHandler_PlayButton( void (*)(bool) );
	void attachHandler_StopButton( void (*)(bool) );
	void attachHandler_RecordButton( void (*)(bool) );
	void attachHandler_TapButton( void (*)(bool) );
	void attachHandler_Switches( void (*)(bool,uint8_t) );
	void attachHandler_FunctionButtons( void (*)(bool,uint8_t) );
	void attachHandler_SnapshotButtons( void (*)(bool,uint8_t) );
	void attachHandler_ModeButtons( void (*)(bool,uint8_t) );

 private:
	void expanderSetInput16(int i2caddr, int dir);
	void expanderSetInvert16(int i2caddr, int dir);
	int expanderRead16(int i2caddr);

	// TCA_Expander Inputs
	bool buttonState[26];        // NUM_BUTTONS = 26  (Inputs from TCA9555)
	bool buttonState_Old[26];  

	static void (*user_playButton_handler)(bool _state);
	static void (*user_stopButton_handler)(bool _state);
	static void (*user_recordButton_handler)(bool _state);
	static void (*user_tapButton_handler)(bool _state);
	static void (*user_switches_handler)(bool _state, uint8_t _button);
	static void (*user_functionButtons_handler)(bool _state, uint8_t _button);
	static void (*user_snapshotButtons_handler)(bool _state, uint8_t _button);
	static void (*user_modeButtons_handler)(bool _state, uint8_t _button);
};

#endif // _MATRIX_INTERFACE_H_