#include "AaronMR_S.hpp"
#include "pack2.hpp"


#include <rtai_shm.h>

struct_Joy::struct_Joy()
{
    auxSerialize.axes[0] = 0.0;
    auxSerialize.axes[1] = 0.0;
    auxSerialize.axes[2] = 0.0;
    auxSerialize.axes[3] = 0.0;
    auxSerialize.buttons[0] = 0;
    auxSerialize.buttons[1] = 0;
    auxSerialize.buttons[2] = 0;
    auxSerialize.buttons[3] = 0;

    auxUnSerialize.axes[0] = 0.0;
    auxUnSerialize.axes[1] = 0.0;
    auxUnSerialize.axes[2] = 0.0;
    auxUnSerialize.axes[3] = 0.0;
    auxUnSerialize.buttons[0] = 0;
    auxUnSerialize.buttons[1] = 0;
    auxUnSerialize.buttons[2] = 0;
    auxUnSerialize.buttons[3] = 0;
}

void struct_Joy::iniSHM(int shm_in, int shm_out, char* SHM_name)
{
    if (shm_in == 1)
    {
        dataIN = (Joy*)rtai_malloc (nam2num(SHM_name), sizeof(struct Joy)) ;
        dataIN->axes[0] = 0.0;
        dataIN->axes[1] = 0.0;
        dataIN->axes[2] = 0.0;
        dataIN->axes[3] = 0.0;

        dataIN->buttons[0] = 0;
        dataIN->buttons[1] = 0;
        dataIN->buttons[2] = 0;
        dataIN->buttons[3] = 0;
        dataIN->newValue = true;
    }

    if (shm_out == 1)
    {
        dataOUT = (Joy*)rtai_malloc (nam2num(SHM_name), sizeof(struct Joy)) ;
        dataOUT->axes[0] = 0.0;
        dataOUT->axes[1] = 0.0;
        dataOUT->axes[2] = 0.0;
        dataOUT->axes[3] = 0.0;

        dataOUT->buttons[0] = 0;
        dataOUT->buttons[1] = 0;
        dataOUT->buttons[2] = 0;
        dataOUT->buttons[3] = 0;
        dataOUT->newValue = true;
    }
}

void struct_Joy::storeData(Joy *joy)
{
    return;
}

char *struct_Joy::serialize(char* buf3)
{
	unsigned char buf[1024];
    unsigned int packetsize = 0;

    auxSerialize.axes[0] = dataOUT->axes[0];
    auxSerialize.axes[1] = dataOUT->axes[1];
    auxSerialize.axes[2] = dataOUT->axes[2];
    auxSerialize.axes[3] = dataOUT->axes[3];
    auxSerialize.buttons[0] = dataOUT->buttons[0];
    auxSerialize.buttons[1] = dataOUT->buttons[1];
    auxSerialize.buttons[2] = dataOUT->buttons[2];
    auxSerialize.buttons[3] = dataOUT->buttons[3];

	packetsize = pack(buf, "CHffffhhhh",    'A',
                                            0,
                                            auxSerialize.axes[0],
                                            auxSerialize.axes[1],
                                            auxSerialize.axes[2],
                                            auxSerialize.axes[3],
                                            auxSerialize.buttons[0],
                                            auxSerialize.buttons[1],
                                            auxSerialize.buttons[2],
                                            auxSerialize.buttons[3]);

	packi16(buf+1, packetsize); // store packet size in packet for kicks

    memcpy((unsigned char*)buf3, buf, packetsize);

    return NULL;
}

int struct_Joy::printStruct(char* data2print)
{
    unsigned char buf[1024];
	unsigned char magic;
	unsigned int ps2;

    memcpy(buf, data2print, 1024);

    unpack(buf, "CHffffhhhh",   &magic,
                            &ps2,
                            &auxUnSerialize.axes[0],
                            &auxUnSerialize.axes[1],
                            &auxUnSerialize.axes[2],
                            &auxUnSerialize.axes[3],
                            &auxUnSerialize.buttons[0],
                            &auxUnSerialize.buttons[1],
                            &auxUnSerialize.buttons[2],
                            &auxUnSerialize.buttons[3]
                            );

    printf("data: '%c' %hhu %f %f %f %f %d %d %d %d\n",   magic,
                                                ps2,
                                                auxUnSerialize.axes[0],
                                                auxUnSerialize.axes[1],
                                                auxUnSerialize.axes[2],
                                                auxUnSerialize.axes[3],
                                                auxUnSerialize.buttons[0],
                                                auxUnSerialize.buttons[1],
                                                auxUnSerialize.buttons[2],
                                                auxUnSerialize.buttons[3]
                                                );

    return 0;
}

char *struct_Joy::Unserialize(char* buf3)
{

	unsigned char buf[1024];
	unsigned char magic;
	unsigned int ps2;

    memcpy(buf, buf3, 1024);

	unpack(buf, "CHffffhhhh",   &magic,
                                &ps2,
                                &auxUnSerialize.axes[0],
                                &auxUnSerialize.axes[1],
                                &auxUnSerialize.axes[2],
                                &auxUnSerialize.axes[3],
                                &auxUnSerialize.buttons[0],
                                &auxUnSerialize.buttons[1],
                                &auxUnSerialize.buttons[2],
                                &auxUnSerialize.buttons[3]
                                );

    dataIN->axes[0] = auxUnSerialize.axes[0];
	dataIN->axes[1] = auxUnSerialize.axes[1];
    dataIN->axes[2] = auxUnSerialize.axes[2];
	dataIN->axes[3] = auxUnSerialize.axes[3];
    dataIN->buttons[0] = auxUnSerialize.buttons[0];
	dataIN->buttons[1] = auxUnSerialize.buttons[1];
    dataIN->buttons[2] = auxUnSerialize.buttons[2];
	dataIN->buttons[3] = auxUnSerialize.buttons[3];

	dataIN->newValue = true;

    return NULL;
}
