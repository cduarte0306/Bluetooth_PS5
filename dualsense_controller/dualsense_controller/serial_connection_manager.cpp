#include "serial_connection_manager.h"

#include <string>
#include <codecvt>

#include <thread>
#include <chrono>


#define RX_BUFFER_SIZE		(100u)
#define TX_BUFFER_SIZE		(100u)

#define MAX_NUM_PORTS		(128)

#define START_BYTE			(0x7E)

using namespace std;


SerialManager::SerialManager(string comPort, int baudrate)
{
	string port = "\\\\.\\" + comPort;
	cout << port << endl;
	wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;
	wstring wport = converter.from_bytes(port);

	CloseHandle(portHandle);
	
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

	dcbSerialParams.BaudRate = baudrate;
	dcbSerialParams.ByteSize = 8;
	dcbSerialParams.StopBits = ONESTOPBIT;
	dcbSerialParams.Parity = NOPARITY;

	GetCommTimeouts(portHandle, &timeout);
	COMMTIMEOUTS timeout = { 0 };
	timeout.ReadIntervalTimeout = 50;
	timeout.ReadTotalTimeoutConstant = 50;
	timeout.ReadTotalTimeoutMultiplier = 50;
	timeout.WriteTotalTimeoutConstant = 50;
	timeout.WriteTotalTimeoutMultiplier = 10;
	SetCommTimeouts(portHandle, &timeout);

	if (!SetCommState(portHandle, &dcbSerialParams)) {
		cout << "Unable to set parameters\n";
		exit(0);
	}

	std::cout << "Starting serial thread... \n";

	/* Create a serial thread to handle reads */
	thread serialThread(&SerialManager::MainThread, this);
	serialThread.join();
}


SerialManager::~SerialManager()
{
	CloseHandle(portHandle);
}


/* The following port function is to be called by other thread whenever data needs to be transmitted */
SerialManager::write_ret_t SerialManager::writeData(SerialManager* serMng, tx_data_t* data, DWORD len)
{
	DWORD bytesWritten;

	/* Write all data bytes to the output buffer */
	bool ret = WriteFile(serMng->portHandle, data, len, &bytesWritten, NULL);

	if (ret == false) { return WRITE_FAIL; }

	return WRITE_PASS;
}


SerialManager::read_ret_t SerialManager::readData(rx_data_t* data, DWORD len)
{
	DWORD  bytesRead;
	bool ret;

	ret = ReadFile(portHandle, data, len, &bytesRead, NULL);

	if (ret == false) { return READ_FAIL; }
	
	return READ_PASS;
}


/**
 * Detect available Com ports.
 * 
 * \param ports
 */
vector<string> SerialManager::getPorts(void)
{
	vector <string> portList;

	HANDLE comHandle;
	string port;
	wstring wport;
	wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;

	vector<string> portContainer;

	/* Open all com ports and add to the vector the com ports that do not return an error */
	for (int portNum = 0; portNum < MAX_NUM_PORTS; portNum++)
	{
		port = "\\\\.\\COM" + to_string(portNum);
		wport = converter.from_bytes(port);
				
		comHandle = CreateFile(wport.c_str(), GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

		if (comHandle == INVALID_HANDLE_VALUE) { continue; }
		
		/* If a port is found, add to the vector container and close the file */
		portContainer.push_back(port);
		CloseHandle(comHandle);
	}

	return portContainer;
}


int SerialManager::calcChecksum(PUINT8 data, DWORD len)
{
	UINT32 sum = 0;

	for (DWORD i = 0; i < len; i++)
	{
		sum += data[i];
	}

	return sum & 0xFF;
}


bool SerialManager::parseData(rx_data_t* data, DWORD len)
{
	if (data->start_byte!= START_BYTE) { return false; }

	rc_telemetry_t tlm;

	memcpy(&tlm, &data->telemetry, sizeof(rc_telemetry_t));

	int checksum = calcChecksum((PUINT8)&tlm, sizeof(rc_telemetry_t));

	if (checksum != data->metadata.key) { return false; }

	return true;
}


void SerialManager::MainThread(void)
{
	rx_data_t rx_data;

	while(true)
	{
		readData(&rx_data, sizeof(rc_telemetry_t));
		
		if (parseData(&rx_data, sizeof(rc_telemetry_t)))
		{
			/* Flush the buffer */
			bool retPurge = PurgeComm(portHandle, PURGE_RXCLEAR);
			if (!retPurge)
			{
				cout << "Error when flushing buffer\n";
				exit(0);
			}
			
			/* Place data in the main telemetry buffer */
			memcpy(&rc_telemetry, &rx_data.telemetry, sizeof(rc_telemetry_t));
		}

		/* Wait for 1 millisecond */
		this_thread::sleep_for(chrono::milliseconds(1));
	}
}
