#pragma once

#ifndef  CONTROLLER_H
#define  CONTROLLER_H

#include "ds5w.h"

using namespace DS5W;

class controller
{
public:
	controller();
	~controller();
	
	DeviceEnumInfo infos[16];
	unsigned int controllersCount = 0;
	
	
private:
	void initController(void);
	bool enumController(void);

	void mainThread();
	bool controllerListen(DS5InputState* inputState, DeviceContext* devCont);
};

#endif 
