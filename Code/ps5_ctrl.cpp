
#include <string>
#include <iostream>
#include <vector>
#include <cstring>

#include "string.h"

#include "serial_controller.h"

#include <wtypesbase.h>

#include "usb.h"


#define BAUDRATE    (9600u)
#define BYTESIZE    (8u)

using namespace std;

serial_controller::serial_controller(void)
{
    cout << "Searching COM ports" << endl;

    /* Open the port */
    // SerHandle = CreateFile(
    //     "COM1",
    //     GENERIC_READ | GENERIC_WRITE,
    //     0,
    //     0,
    //     OPEN_EXISTING,
    //     FILE_ATTRIBUTE_NORMAL,
    //     0   
    // );



    // CString ComName=CString("COM") + CString(str); // converting to COM0, COM1, COM2

    // int test = QueryDosDevice(ComName, (LPSTR)lpTargetPath, 5000);

    // // GetDefaultCommConfig(PortsList);
    
    // /* Configure the port paramters */
    // PortParams.BaudRate = BAUDRATE;
    // PortParams.ByteSize = BYTESIZE;
    // PortParams.EofChar  = '\n';
}


serial_controller::~serial_controller(void)
{
    cout << "Closing COM port" << endl;

    /* Close the port */
    CloseHandle(SerHandle);
}


void serial_controller::RdSerial(void)
{
    
}