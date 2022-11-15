#pragma once

#ifndef  SERIAL_CONNECTION_MANAGER_H
#define  SERIAL_CONNECTION_MANAGER_H

#include <iostream>
#include <vector>
#include <string>

#include <Windows.h>
#include <WinBase.h>


class SerialManager
{
public:
	SerialManager(std::string comPort);
	~SerialManager();

	static std::vector<std::string> getPorts(void);

private:
	std::string parseComsel(std::string string);
};

#endif // ! SERIAL_CONNECTION_MANAGER_H
