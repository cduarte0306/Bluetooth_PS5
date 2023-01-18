#pragma once

#ifndef  SERIAL_CONNECTION_MANAGER_H
#define  SERIAL_CONNECTION_MANAGER_H

#include <iostream>
#include <vector>
#include <string>

#include <Windows.h>
#include <WinBase.h>

#include <thread>

using namespace std;


#define END_SEQ_SIZE        (4u)


class SerialManager
{
public:
	
	SerialManager(string comPort, int baudrate);
	~SerialManager();

	typedef enum
	{
		WRITE_FAIL = -1,
		WRITE_PASS = 1
	} write_ret_t;

	typedef enum
	{
		READ_FAIL = -1,
		READ_PASS = 1
	} read_ret_t;

	#pragma pack(1)
	typedef struct
	{
		uint16_t key;
		uint64_t msgCount;
	} metadata_t;

	#pragma pack(1)
	/* Data out */
	typedef struct
	{
		float front_distance;
		float right_distance;
		float left_distance;
	} distance_t;

	#pragma pack(1)
	typedef struct
	{
		float kp;
		float ki;
		float kd;
	} pid_params_t;

	#pragma pack(1)
	typedef struct
	{
		float speed;
		distance_t distance;
		float battery_voltage;
		uint8_t eeprom_flag;
		pid_params_t pid;
	} rc_telemetry_t;

	#pragma pack(1)
	typedef struct
	{
		uint8_t start_byte;
		rc_telemetry_t telemetry;
		metadata_t metadata;
	} rx_data_t;

	#pragma pack(1)
	/* Data out */
	typedef struct
	{
		uint8_t forward;
		uint8_t reverse;
		uint16_t motorSpeed;
		uint16_t servo;
		uint8_t eeprom_command;
	} cmd_data_t;

	#pragma pack(1)
	typedef struct
	{
		uint8_t start_byte;
		cmd_data_t cmds;
		uint8_t endSequence[END_SEQ_SIZE];
		metadata_t metadata;
	} tx_data_t;

	static vector<string> getPorts(void);

	HANDLE portHandle;
	
	static void setTxData(SerialManager* SerMng, cmd_data_t* cmdData);
	static write_ret_t writeData(SerialManager* serMng, tx_data_t* data, DWORD len);
	static uint64_t txCmdCount;


private:
	
	read_ret_t readData(rx_data_t* data, DWORD len);
	bool parseData(rx_data_t* data, DWORD len);
	uint16_t fletchers16Bit(PUINT8 data, DWORD len);
	int SendData(tx_data_t* tx_data);
	void MainSerThread(void);

	rc_telemetry_t rc_telemetry;

	COMMTIMEOUTS timeout;

	std::thread serThread;
	bool isThreadRunning = true;

	uint64_t rxCmdCount = 0;
};

#endif // ! SERIAL_CONNECTION_MANAGER_H
