#include <iostream>
#include <thread>
#include <chrono>

#include "controller.h"

#define RET_OK		(1U)
#define RET_FAIL	(0U)

using namespace DS5W;


controller::controller()
{
	std::cout << "Controller created!!!\n";
	 
	if (!enumController())
		delete this;

	/* Create a controller thread */
	initController();
	
}


controller::~controller()
{
	
}


bool controller::enumController(void)
{
	bool ret = RET_FAIL;

	switch (enumDevices(infos, 16, &controllersCount))
	{
		case DS5W_OK:
			break;
		case DS5W_E_INSUFFICIENT_BUFFER:
			std::cout << "No controller found. Exiting\n";
			ret = RET_FAIL;
			break;
		default:
			std::cout << "ID not recognized. Exiting application.\n";
			ret = RET_FAIL;
			break;
	}

	if (controllersCount == 0) { return RET_FAIL; }

	return RET_OK;
}


/**
 * Initialize controller.
 */
void controller::initController(void)
{
	std::thread controllerThread(&controller::mainThread, this);
	controllerThread.join();
}


/**
 * Listen to input from the controller.
 * 
 */
bool controller::controllerListen(DS5InputState *inputState, DeviceContext *devCont)
{
	if (getDeviceInputState(devCont, inputState) != DS5W_OK)
	{
		return RET_FAIL;
	}

	printf("Analog stick X-axis is %i\n",	inputState->leftStick.x);
	printf("Analog stick y-axis is %i\n\n", inputState->leftStick.y);

	return RET_OK;
}


/**
 * Main Controller thread.
 * 
 */
void controller::mainThread()
{
	/* Intialize device context */
	DeviceContext devCont;
	DS5InputState inputState;

	//if (initDeviceContext(&enumInfo, &devCont) != DS5W_OK) { delete this; }
	initDeviceContext(infos, &devCont);
	while (true)
	{
		controllerListen(&inputState, &devCont);

		/* Sleep thread for 1 millisecond */
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}
