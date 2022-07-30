#include <iostream>

#include "serial_controller.h"
#include "winsock.h"


int main(void)
{
    serial_controller* object = new serial_controller;
    delete object;
    return 0;
}