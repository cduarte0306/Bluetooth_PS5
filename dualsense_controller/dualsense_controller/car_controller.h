#pragma once

#ifndef CAR_CONTROLLER_H

#include <vector>
#include <string>
#include <iostream>

#include "serial_connection_manager.h"
	
class CarController
{
public:
	CarController();
	~CarController();

private:
	std::vector<std::string> comPorts;
	SerialManager* serial = nullptr;

	int parseNumber(std::string portFound);

	
};

#endif // !CAR_CONTROLLER_H

