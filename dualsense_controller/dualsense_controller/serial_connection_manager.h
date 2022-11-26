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
		uint8_t term_1;
		uint8_t term_2;
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
		uint16_t servo;
		uint8_t eeprom_command;
	} cmd_data_t;

	#pragma pack(1)
	typedef struct
	{
		uint8_t start_byte;
		cmd_data_t cmds;
		metadata_t metadata;
	} tx_data_t;

	static vector<string> getPorts(void);
	static write_ret_t writeData(SerialManager* serMng, tx_data_t* data, DWORD len);

	HANDLE portHandle;
	
	void getData(rx_data_t* rx_data, DWORD len);


private:

	read_ret_t readData(rx_data_t* data, DWORD len);	
	void parseData(rx_data_t* data, size_t len);

	void MainThread(void);
	
	bool readStatus = false;

};

#endif // ! SERIAL_CONNECTION_MANAGER_H
