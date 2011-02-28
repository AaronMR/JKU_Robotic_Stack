
#include "AaronMR_S.hpp"
#include "pack2.hpp"
#include <rtai_shm.h>
//############################################### struct_Pose ##############################

struct_Pose::struct_Pose()
{
    auxSerialize.position.x = 0.0;
    auxSerialize.position.y = 0.0;
    auxSerialize.position.z = 0.0;

    auxSerialize.orientation.x = 0.0;
    auxSerialize.orientation.y = 0.0;
    auxSerialize.orientation.z = 0.0;
    auxSerialize.orientation.w = 0.0;

    auxUnSerialize.position.x = 0.0;
    auxUnSerialize.position.y = 0.0;
    auxUnSerialize.position.z = 0.0;

    auxUnSerialize.orientation.x = 0.0;
    auxUnSerialize.orientation.y = 0.0;
    auxUnSerialize.orientation.z = 0.0;
    auxUnSerialize.orientation.w = 0.0;
}

void struct_Pose::iniSHM(int shm_in, int shm_out, char* SHM_name)
{

    if (shm_in == 1)
    {
        printf("shm_in = %s", SHM_name);
        dataIN = (Pose*)rtai_malloc (nam2num(SHM_name), sizeof(struct Pose)) ;
        dataIN->position.x = 0.0;
        dataIN->position.y = 0.0;
        dataIN->position.z = 0.0;

        dataIN->orientation.x = 0.0;
        dataIN->orientation.y = 0.0;
        dataIN->orientation.z = 0.0;
        dataIN->orientation.w = 0.0;
        dataIN->newValue = 0;

    }


    if (shm_out == 1)
    {
        printf("shm_out = %s", SHM_name);
        dataOUT = (Pose*)rtai_malloc (nam2num(SHM_name), sizeof(struct Pose)) ;
        dataOUT->position.x = 1.1;
        dataOUT->position.y = 2.0;
        dataOUT->position.z = 3.3;

        dataOUT->orientation.x = 4.4;
        dataOUT->orientation.y = 5.0;
        dataOUT->orientation.z = 6.0;
        dataOUT->orientation.w = 7.7;
    }

}

void struct_Pose::storeData(Joy *joy)
{
    return;
}

char *struct_Pose::serialize(char* buf3)
{
	unsigned char buf[1024];
	unsigned int packetsize = 0;

    auxSerialize.position.x = dataOUT->position.x;
    auxSerialize.position.y = dataOUT->position.y;
    auxSerialize.position.z = dataOUT->position.z;

    auxSerialize.orientation.x = dataOUT->orientation.x;
    auxSerialize.orientation.y = dataOUT->orientation.y;
    auxSerialize.orientation.z = dataOUT->orientation.z;
    auxSerialize.orientation.w = dataOUT->orientation.w;

	packetsize = pack(buf, "CHddddddd",    'A',
                                            0,
                                            auxSerialize.position.x,
                                            auxSerialize.position.y,
                                            auxSerialize.position.z,
                                            auxSerialize.orientation.x,
                                            auxSerialize.orientation.y,
                                            auxSerialize.orientation.z,
                                            auxSerialize.orientation.w);

	packi16(buf+1, packetsize); // store packet size in packet for kicks

    memcpy((unsigned char*)buf3, buf, packetsize);

    return NULL;
}

int struct_Pose::printStruct(char* data2print)
{
    unsigned char buf[1024];
	unsigned char magic;
	unsigned int ps2;

    memcpy(buf, data2print, 1024);

    unpack((unsigned char*)buf, "CHddddddd",  &magic,
                                            &ps2,
                                            &auxUnSerialize.position.x,
                                            &auxUnSerialize.position.y,
                                            &auxUnSerialize.position.z,
                                            &auxUnSerialize.orientation.x,
                                            &auxUnSerialize.orientation.y,
                                            &auxUnSerialize.orientation.z,
                                            &auxUnSerialize.orientation.w
                                            );

    printf("data: '%c' %hhu %f %f %f %f %f %f %f\n",   magic,
                                                    ps2,
                                                    auxUnSerialize.position.x,
                                                    auxUnSerialize.position.y,
                                                    auxUnSerialize.position.z,
                                                    auxUnSerialize.orientation.x,
                                                    auxUnSerialize.orientation.y,
                                                    auxUnSerialize.orientation.z,
                                                    auxUnSerialize.orientation.w
                                                    );
    return 0;
}

char *struct_Pose::Unserialize(char* buf3)
{
	unsigned char buf[1024];
	unsigned char magic;
	unsigned int ps2;


    memcpy(buf, buf3, 1024);

    unpack((unsigned char*)buf, "CHddddddd",  &magic,
                                            &ps2,
                                            &auxUnSerialize.position.x,
                                            &auxUnSerialize.position.y,
                                            &auxUnSerialize.position.z,
                                            &auxUnSerialize.orientation.x,
                                            &auxUnSerialize.orientation.y,
                                            &auxUnSerialize.orientation.z,
                                            &auxUnSerialize.orientation.w
                                            );

    dataIN->position.x = auxUnSerialize.position.x;
    dataIN->position.y = auxUnSerialize.position.y;
    dataIN->position.z = auxUnSerialize.position.z;

    dataIN->orientation.x = auxUnSerialize.orientation.x;
    dataIN->orientation.y = auxUnSerialize.orientation.y;
    dataIN->orientation.z = auxUnSerialize.orientation.z;
    dataIN->orientation.w = auxUnSerialize.orientation.w;

    return NULL;
}
