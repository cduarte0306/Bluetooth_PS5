#include "serial_connection_manager.h"

#include <string>
#include <codecvt>

#include <thread>
#include <chrono>
#include <functional>


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

	/* Configure timeout */
	timeout.ReadIntervalTimeout = 10;
	timeout.ReadTotalTimeoutConstant = 10;
	timeout.ReadTotalTimeoutMultiplier = 10;
	timeout.WriteTotalTimeoutConstant = 10;
	timeout.WriteTotalTimeoutMultiplier = 10;
	SetCommTimeouts(portHandle, &timeout);

	if (!SetCommState(portHandle, &dcbSerialParams)) {
		cout << "Unable to set parameters\n";
		exit(0);
	}

	std::cout << "Starting serial thread... \n";

	/* Create a serial thread to handle reads */
	serThread = thread (std::bind(&SerialManager::MainSerThread, this));
}


SerialManager::~SerialManager()
{
	isThreadRunning = false;
	serThread.join();
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
	bool ret = false;

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


/**
 * @brief Calculater the 16-bit fletcher checksum
 *
 * @param data Pointer to the data from which the checksum will be calculated
 * @param len  Number of bytes to be transmitted
 * @return uint16 Checksum result
 */
uint16_t SerialManager::fletchers16Bit(PUINT8 data, DWORD len)
{
	size_t index = -1;
	uint32_t c1 = 0;
	uint32_t c2 = 0;

	while ((index++) < len)
	{
		c1 += *(data + index) % 255;
		c2 = (c1 + c2) % 255;
	}

	return (c2 << 8) | (c1);
}


bool SerialManager::parseData(rx_data_t* data, DWORD len)
{
	if (data->start_byte!= START_BYTE) { return false; }

	rc_telemetry_t tlm;

	memcpy(&tlm, &data->telemetry, sizeof(rc_telemetry_t));

	UINT16 checksum = fletchers16Bit((PUINT8)&tlm, sizeof(rc_telemetry_t));

	if (checksum != data->metadata.key) { return false; }

	return true;
}


void SerialManager::setTxData(SerialManager* SerMng, cmd_data_t* cmdData)
{
	/* Calculate the checksum */
	UINT16 checksum = SerMng->fletchers16Bit((PUINT8)cmdData, sizeof(cmd_data_t));
	
	tx_data_t tx_data;
	memcpy((PUINT8)&tx_data, (PUINT8)cmdData, sizeof(cmd_data_t));

	tx_data.start_byte = START_BYTE;
	tx_data.endSequence[0] = 0xD;
	tx_data.endSequence[1] = 0xE;
	tx_data.endSequence[2] = 0xA;
	tx_data.endSequence[3] = 0xD;

	/* Increment the message count */
	txCmdCount++;
	tx_data.metadata.msgCount = txCmdCount;
	
	writeData(SerMng, &tx_data, sizeof(tx_data));
}


void SerialManager::MainSerThread(void)
{
	rx_data_t rx_data;
	isThreadRunning = true;

	while(isThreadRunning)
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

		/* Wait for 10 milliseconds */
		this_thread::sleep_for(chrono::milliseconds(10));
	}
}
