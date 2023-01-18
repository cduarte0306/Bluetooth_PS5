#include "car_controller.h"
#include "serial_connection_manager.h"
#include "controller.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include <functional>
#include <thread>
#include <chrono>


/* The following values determine the point at which the PWM speed will be considered a forward setting or a reverse setting */
#define REVERSE_THRESHOLD  490
#define FORWARD_THRESHOLD  510


using namespace std;


CarController::CarController()
{
	cout << "Starting...\n";
	
	comPorts = SerialManager::getPorts();

	vector<string>::iterator it;
	string selectedPort;
	string userInput;

	string baudRate;

	/* Open serial handler object */
	cout << "Select the following com ports:\n";
	
	for (vector<string>::iterator t = comPorts.begin(); t != comPorts.end(); t++)
	{
		selectedPort = *t;
			
		int portNum = parseNumber(selectedPort);

		cout << string("COM") + to_string(portNum) << endl;
	}

	cin >> userInput;

	cout << "Select baurate: \n";
	cin >> baudRate;

	int intbr = atoi(baudRate.c_str());

	/* Create the serial object */
	serial = new SerialManager(userInput, intbr);
	dswController = new controller();

	std::cout << "Initiating car controller thread...\n";

}


CarController::~CarController()
{
	delete serial;
	delete dswController;
}


int CarController::parseNumber(string portFound)
{
	string str;

	for (int i = 0; i < portFound.length(); i++)
	{
		if (isdigit(portFound[i])) 
		{ 
			str.push_back(portFound[i]);
			return stoi(str);
		}
	}

	return 0;
}


/**
 * Remap the given value into the new range from the provided value range.
 * 
 * \param val
 * \param y1
 * \param y2
 * \param x1
 * \param x2
 * \return 
 */
uint32_t CarController::remapRange(int val, int y1, int y2, int x1, int x2)
{
	float slope = (float)((y2 - y1) / (x2 - x1));
	float offset = (float)(y2 - (slope * x2));

	return (uint32_t)((slope * val) + offset);
}


void CarController::SetRcCommands(controller::controllerInput* psControllerInput)
{
	SerialManager::cmd_data_t cmdData;

	/* Check all controller inputs before transmitting data to the RC car */
	/* Convert joystick data into PWM counts from 0 to 1000 (10-bit) */
	uint32_t motorSetting = remapRange(psControllerInput->leftJoystick.y, 0, 1023, -127, 127);
	
	if (motorSetting < REVERSE_THRESHOLD)
	{
		cmdData.forward = false;
		cmdData.reverse = true;
	}
	else if (motorSetting > FORWARD_THRESHOLD)
	{
		cmdData.forward = true;
		cmdData.reverse = false;
	}
	else
	{
		cmdData.forward = false;
		cmdData.reverse = false;
	}

	SerialManager::setTxData(serial, &cmdData);
	
	printf("Motor setting is %lu\n", motorSetting);
}


void CarController::MainThread(void)
{
	struct controller::controllerInput inputField;
	SerialManager::cmd_data_t rcCommands;

	while(true)
	{
		/* Get controller input */
		if (dswController->getControllerInput(&inputField) == controller::CONTROLLER_FAIL) { exit(0); }

		/* Transmit the data to the device via the serial prot */
		SetRcCommands(&inputField);

		//std::cout << "Input is " << inputField.circle_state << std::endl;

		/* Wait for 20ms to allow other threads to run */
		this_thread::sleep_for(chrono::milliseconds(20));
	}
}
