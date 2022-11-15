#include "serial_connection_manager.h"

#include <string>
#include <codecvt>


#define RX_BUFFER_SIZE		(100u)
#define TX_BUFFER_SIZE		(100u)

#define MAX_NUM_PORTS		(128)


SerialManager::SerialManager(std::string comPort)
{
	HANDLE ComPort;

	/* Parse through com port selection text */
	

	//CreateFile()

	/* Set input and output buffer sizes too 100 */
	//SetupComm(ComPort, 100, 100);
}


SerialManager::~SerialManager()
{
}


std::string  SerialManager::parseComsel(std::string string)
{
	return "HHello";
}


/**
 * Detect available Com ports.
 * 
 * \param ports
 */
std::vector<std::string> SerialManager::getPorts(void)
{
	std::vector <std::string> portList;

	HANDLE comHandle;
	std::string port;
	std::wstring wport;
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

	std::vector<std::string> portContainer;

	/* Open all com ports and add to the vector the com ports that do not return an error */
	for (int portNum = 0; portNum < MAX_NUM_PORTS; portNum++)
	{
		port = "\\\\.\\COM" + std::to_string(portNum);
		wport = converter.from_bytes(port);
				
		comHandle = CreateFile(wport.c_str(), GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

		if (comHandle == INVALID_HANDLE_VALUE) { continue; }
		
		/* If a port is found, add to the vector container and close the file */
		portContainer.push_back(port);
		CloseHandle(comHandle);
	}

	return portContainer;
}

