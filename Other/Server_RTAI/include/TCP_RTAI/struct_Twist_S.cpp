#include "AaronMR_S.hpp"
#include "pack2.hpp"
#include <rtai_shm.h>
//############################################### struct_Twist ##############################

struct_Twist::struct_Twist()
{
    auxSerialize.linear.x = 0.0;
    auxSerialize.linear.y = 0.0;
    auxSerialize.linear.z = 0.0;

    auxSerialize.angular.x = 0.0;
    auxSerialize.angular.y = 0.0;
    auxSerialize.angular.z = 0.0;

    auxUnSerialize.linear.x = 0.0;
    auxUnSerialize.linear.y = 0.0;
    auxUnSerialize.linear.z = 0.0;

    auxUnSerialize.angular.x = 0.0;
    auxUnSerialize.angular.y = 0.0;
    auxUnSerialize.angular.z = 0.0;

}

void struct_Twist::iniSHM(int shm_in, int shm_out, char* SHM_name)
{
    if (shm_in == 1)
    {
        printf("shm_in = %s", SHM_name);
        //dataIN = (Twist*)rtai_malloc (nam2num(SHMNAM_IN), sizeof(struct Twist)) ;
        dataIN = (Twist*)rtai_malloc (nam2num(SHM_name), sizeof(struct Twist)) ;
        dataIN->linear.x = 0.0;
        dataIN->linear.y = 0.0;
        dataIN->linear.z = 0.0;

        dataIN->angular.x = 0.0;
        dataIN->angular.y = 0.0;
        dataIN->angular.z = 0.0;
    }

    if (shm_out == 1)
    {
        printf("shm_out = %s", SHM_name);
        //dataOUT = (Twist*)rtai_malloc (nam2num(SHMNAM_OUT), sizeof(struct Pose)) ;
        dataOUT = (Twist*)rtai_malloc (nam2num(SHM_name), sizeof(struct Twist)) ;
        dataOUT->linear.x = 0.0;
        dataOUT->linear.y = 0.0;
        dataOUT->linear.z = 0.0;

        dataOUT->angular.x = 0.0;
        dataOUT->angular.y = 0.0;
        dataOUT->angular.z = 0.0;
    }

}


void struct_Twist::storeData(Joy *joy)
{
    return;
}

char *struct_Twist::serialize(char* buf3)
{
	unsigned char buf[1024];
	unsigned int packetsize = 0;

    auxSerialize.angular.x = dataOUT->angular.x;
    auxSerialize.angular.y = dataOUT->angular.y;
    auxSerialize.angular.z = dataOUT->angular.z;

    auxSerialize.linear.x = dataOUT->linear.x;
    auxSerialize.linear.y = dataOUT->linear.y;
    auxSerialize.linear.z = dataOUT->linear.z;

	packetsize = pack(buf, "CHdddddd",    'A',
                                            0,
                                            auxSerialize.angular.x,
                                            auxSerialize.angular.y,
                                            auxSerialize.angular.z,
                                            auxSerialize.linear.x,
                                            auxSerialize.linear.y,
                                            auxSerialize.linear.z);

	packi16(buf+1, packetsize); // store packet size in packet for kicks

    memcpy((unsigned char*)buf3, buf, packetsize);


    return NULL;
}


int struct_Twist::printStruct(char* data2print)
{
    unsigned char buf[1024];
	unsigned char magic;
	unsigned int ps2;

    memcpy(buf, data2print, 1024);

    unpack((unsigned char*)buf, "CHdddddd", &magic,
                                            &ps2,
                                            &auxUnSerialize.linear.x,
                                            &auxUnSerialize.linear.y,
                                            &auxUnSerialize.linear.z,
                                            &auxUnSerialize.angular.x,
                                            &auxUnSerialize.angular.y,
                                            &auxUnSerialize.angular.z);

	printf("data: '%c' %hhu %f %f %f %f %f %f\n",   magic,
                                                    ps2,
                                                    auxUnSerialize.linear.x,
                                                    auxUnSerialize.linear.y,
                                                    auxUnSerialize.linear.z,
                                                    auxUnSerialize.angular.x,
                                                    auxUnSerialize.angular.y,
                                                    auxUnSerialize.angular.z);
    return 0;
}

char *struct_Twist::Unserialize(char* buf3)
{
	unsigned char buf[1024];
    unsigned char magic;
	unsigned int ps2;

    memcpy(buf, buf3, 1024);

	unpack((unsigned char*)buf, "CHdddddd", &magic,
                                            &ps2,
                                            &auxUnSerialize.linear.x,
                                            &auxUnSerialize.linear.y,
                                            &auxUnSerialize.linear.z,
                                            &auxUnSerialize.angular.x,
                                            &auxUnSerialize.angular.y,
                                            &auxUnSerialize.angular.z);

    dataIN->angular.x = auxUnSerialize.angular.x;
	dataIN->angular.y = auxUnSerialize.angular.y;
    dataIN->angular.z = auxUnSerialize.angular.z;
	dataIN->linear.x = auxUnSerialize.linear.x;
    dataIN->linear.y = auxUnSerialize.linear.y;
	dataIN->linear.z = auxUnSerialize.linear.z;
    dataIN->newValue = true;

    return NULL;
}
