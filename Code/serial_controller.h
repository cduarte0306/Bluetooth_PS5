#ifndef SERIAL_CONTROLLER_H
#define SERIAL_CONTROLLER_H

#include <wtypesbase.h>
#include <vector>

class serial_controller
{
    public:
        serial_controller(void);
        ~serial_controller(void);

        void RdSerial(void);

    private:
        HANDLE SerHandle;
        DCB PortParams;

        std::vector<std::string> PortsList;
};


#endif