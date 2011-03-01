#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <resolv.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>

#include <cstdlib>
#include "AaronMR_C.hpp"

using namespace std;

int exitClient(int hsock);

DataLayout process[5];

turtlesim::Velocity vel;

Struct_3 auxJoy;

int main(int argv, char** argc)
{

    ros::init(argv, argc, argc[2]);

    transporter_C test(argc[1]);

    test.create_Socket();

    if(test.connect_Socket()== -1)
    {
        printf("The connection error\n");
        return -1;
    }

    while(ros::ok())
    {

        usleep(10000);

        if(test.send_Data() == -1)
        {
            printf("The connection error\n");
            return -1;
        }

        if(test.recv_Data() == -1)
        {
            printf("The connection error\n");
            return -1;
        }
    }


    return 0;
}


