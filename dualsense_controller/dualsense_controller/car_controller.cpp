#include "car_controller.h"
#include "serial_connection_manager.h"
#include "controller.h"


CarController::CarController()
{
	std::cout << "Starting...\n";
	
	comPorts = SerialManager::getPorts();

	std::vector<std::string>::iterator it;

	/* Open serial handler object */
	
}


CarController::~CarController()
{
	
}
