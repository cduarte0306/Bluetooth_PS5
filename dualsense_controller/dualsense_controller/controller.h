#pragma once

#ifndef  CONTROLLER_H
#define  CONTROLLER_H

#include "ds5w.h"

using namespace DS5W;

class controller
{
public:
	controller();
	~controller();

	typedef enum
	{
		IDLE = 0,
		LEFT_JOYSTICK,
		RIGHT_JOYSTICK,
		X_BTN,
		SQUARE_BTN,
		CIRCLE_BTN,
		TRIANGLE_BTN,
		
		R1,
		R2,
		L1, 
		L2
	} inputType_t;

	typedef enum
	{
		CONTROLLER_OK = 1,
		CONTROLLER_FAIL = -1
	} controllerStatus;

	struct joystickDir
	{
		int x;
		int y;
	};
		
	struct controllerInput
	{
		struct joystickDir leftJoystick;
		struct joystickDir rightJoystick;
		
		bool x_state;
		bool square_state;
		bool circle_state;
		bool triangle_state;

		bool r1_state;
		bool r2_state;
		bool l1_state;
		bool l2_state;
	};
	
	unsigned int controllersCount = 0;
	
	int getControllerInput(struct controllerInput* input);
	
	
private:
	bool enumController(void);
	bool controllerListen(DS5InputState* inputState, DeviceContext* devCont);

	DeviceEnumInfo infos[16];
	DeviceContext devCont;
	DS5InputState inputState;
};

#endif 
