#include <iostream>
#include <thread>
#include <chrono>

#include "controller.h"
#include "serial_connection_manager.h"

using namespace DS5W;


controller::controller()
{
	std::cout << "Controller created!!!\n";
	 
	if (!enumController())
	{
		cout << "No controller is found. Exiting program\n";
		exit(0);
	}

	SerialManager::getPorts();

	initDeviceContext(infos, &devCont);
}


controller::~controller()
{
	
}


bool controller::enumController(void)
{
	bool ret = CONTROLLER_FAIL;

	switch (enumDevices(infos, 16, &controllersCount))
	{
		case DS5W_OK:
			break;
		case DS5W_E_INSUFFICIENT_BUFFER:
			std::cout << "No controller found. Exiting\n";
			ret = CONTROLLER_FAIL;
			break;
		default:
			std::cout << "ID not recognized. Exiting application.\n";
			ret = CONTROLLER_FAIL;
			break;
	}

	if (controllersCount == 0) { return CONTROLLER_FAIL; }

	return CONTROLLER_OK;
}


/**
 * Listen to input from the controller.
 * 
 */
bool controller::controllerListen(DS5InputState *inputState, DeviceContext *devCont)
{
	if (getDeviceInputState(devCont, inputState) != DS5W_OK)
	{
		return CONTROLLER_FAIL;
	}

	printf("Analog stick X-axis is %i\n",	inputState->leftStick.x);
	printf("Analog stick y-axis is %i\n\n", inputState->leftStick.y);

	return CONTROLLER_OK;
}


int controller::getControllerInput(struct controllerInput* input)
{
	if (getDeviceInputState(&devCont, &inputState) != DS5W_OK)
	{
		return CONTROLLER_FAIL;
	}

	/* Place the read data inside the context struct */
	input->rightJoystick.x = inputState.rightStick.x;
	input->rightJoystick.y = inputState.rightStick.y;

	input->leftJoystick.x = inputState.leftStick.x;
	input->leftJoystick.y = inputState.leftStick.y;

	input->circle_state   = inputState.buttonsAndDpad && DS5W_ISTATE_BTX_CIRCLE;
	input->triangle_state = inputState.buttonsAndDpad && DS5W_ISTATE_BTX_TRIANGLE;
	input->x_state		  = inputState.buttonsAndDpad && DS5W_ISTATE_BTX_CROSS;
	input->square_state   = inputState.buttonsAndDpad && DS5W_ISTATE_BTX_SQUARE;

	return CONTROLLER_OK;
}
