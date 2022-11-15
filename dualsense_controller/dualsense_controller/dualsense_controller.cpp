#include <Windows.h>
#include <iostream>

#include "car_controller.h"

int main(int argc, char** argv) {

	CarController* controller_object = new CarController();
		
	delete controller_object;

	std::cout << "Leaving";

	return 0;
}