#pragma once

#ifndef  SERIAL_CONNECTION_MANAGER_H
#define  SERIAL_CONNECTION_MANAGER_H

#include <iostream>
#include <vector>
#include <string>

#include <Windows.h>
#include <WinBase.h>

using namespace std;


class SerialManager
{
public:
	SerialManager(string comPort);
	~SerialManager();

	static vector<string> getPorts(void);

private:
	string parseComsel(string string);
	HANDLE portHandle;

};

#endif // ! SERIAL_CONNECTION_MANAGER_H
