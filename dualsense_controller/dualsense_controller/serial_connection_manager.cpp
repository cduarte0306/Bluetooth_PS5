#include "serial_connection_manager.h"

#include <string>
#include <codecvt>


#define RX_BUFFER_SIZE		(100u)
#define TX_BUFFER_SIZE		(100u)

#define MAX_NUM_PORTS		(128)

using namespace std;


SerialManager::SerialManager(string comPort)
{
	string port = "\\\\.\\" + comPort;
	cout << port << endl;
	wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;
	wstring wport = converter.from_bytes(port);
	
	/* Parse through com port selection text */
	portHandle = CreateFile(wport.c_str(), GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (portHandle == INVALID_HANDLE_VALUE) {
		cout << "Handle invalid\n";
		exit(0);
	}

	DCB dcbSerialParams = { 0 };

	if (!GetCommState(portHandle, &dcbSerialParams)) {
		cout << "Unable to get parameters\n";
		exit(0);
	}

	dcbSerialParams.BaudRate = CBR_115200;
	dcbSerialParams.ByteSize = 8;
	dcbSerialParams.StopBits = ONESTOPBIT;
	dcbSerialParams.Parity = NOPARITY;

	if (!SetCommState(portHandle, &dcbSerialParams)) {
		cout << "Unable to set parameters\n";
		exit(0);
	}

}


SerialManager::~SerialManager()
{
}


string  SerialManager::parseComsel(std::string string)
{
	return "HHello";
}


/**
 * Detect available Com ports.
 * 
 * \param ports
 */
vector<std::string> SerialManager::getPorts(void)
{
	std::vector <std::string> portList;

	HANDLE comHandle;
	string port;
	wstring wport;
	wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

	vector<std::string> portContainer;

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

