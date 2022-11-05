#include <Windows.h>
#include <iostream>

#include "ds5w.h"
#include "controller.h"

int main(int argc, char** argv) {

	controller* controller_object = new controller;
	
	delete controller_object;

	std::cout << "Leaving";
	// Return zero
	return 0;
}