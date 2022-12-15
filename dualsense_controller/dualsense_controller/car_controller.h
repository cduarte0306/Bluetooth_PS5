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

private:
	std::vector<std::string> comPorts;
	SerialManager* serial = NULL;
	controller* dswController = NULL;

	void MainThread(void);

	int parseNumber(std::string portFound);

	
};

#endif // !CAR_CONTROLLER_H

