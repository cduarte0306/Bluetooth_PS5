#include "car_controller.h"
#include "serial_connection_manager.h"
#include "controller.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

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

	int intbr = std::atoi(baudRate.c_str());

	/* Create the serial object */
	serial = new SerialManager(userInput, intbr);
}


CarController::~CarController()
{
	
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
