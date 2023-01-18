#pragma once

#ifndef CAR_CONTROLLER_H

#include <vector>
#include <string>
#include <iostream>

#include "serial_connection_manager.h"
#include "controller.h"
	
class CarController
{
public:
	CarController();
	~CarController();

	void MainThread(void);

private:
	std::vector<std::string> comPorts;
	SerialManager* serial = NULL;
	controller* dswController = NULL;

	int parseNumber(std::string portFound);
	void SetRcCommands(controller::controllerInput* psControllerInput);

	UINT32 remapRange(int val, int y1, int y2, int x1, int x2);
	
};

#endif // !CAR_CONTROLLER_H

