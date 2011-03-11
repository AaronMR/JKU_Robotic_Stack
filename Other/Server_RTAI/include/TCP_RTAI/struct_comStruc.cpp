
#include "transporter_S.hpp"
#include "pack2.hpp"

#include <stdio.h>
#include <math.h>
#include <rtai_shm.h>


struct_comStruc::struct_comStruc()
{
    auxSerialize.x1 = 0.0;
    auxSerialize.y1 = 0.0;
    auxSerialize.x2 = 0.0;
    auxSerialize.y2 = 0.0;
    auxSerialize.a = 0;
    auxSerialize.b = 0;
    auxSerialize.c = 0;
    auxSerialize.d = 0;

    auxUnSerialize.x1 = 0.0;
    auxUnSerialize.y1 = 0.0;
    auxUnSerialize.x2 = 0.0;
    auxUnSerialize.y2 = 0.0;
    auxUnSerialize.a = 0;
    auxUnSerialize.b = 0;
    auxUnSerialize.c = 0;
    auxUnSerialize.d = 0;

}


void struct_comStruc::iniSHM(int shm_in, int shm_out, char* SHM_name)
{
    if (shm_in == 1)
    {
        dataIN = (comStruc*)rtai_malloc (nam2num(SHM_name), sizeof(struct comStruc)) ;
        dataIN->x1 = 0.0;
        dataIN->y1 = 0.0;
        dataIN->x2 = 0.0;
        dataIN->y2 = 0.0;
        dataIN->a = 0;
        dataIN->b = 0;
        dataIN->c = 0;
        dataIN->d = 0;

    }

    if (shm_out == 1)
    {
        dataOUT = (comStruc*)rtai_malloc (nam2num(SHM_name), sizeof(struct comStruc)) ;
        dataOUT->x1 = 0.0;
        dataOUT->y1 = 0.0;
        dataOUT->x2 = 0.0;
        dataOUT->y2 = 0.0;
        dataOUT->a = 0;
        dataOUT->b = 0;
        dataOUT->c = 0;
        dataOUT->d = 0;
    }
}

char *struct_comStruc::serialize(char* buf3)
{
	unsigned char buf[1024];
    unsigned int packetsize = 0;

    //comStruc_OUT auxSerialize;

    auxSerialize.x1 = dataOUT->x1;
    auxSerialize.y1 = dataOUT->y1;
    auxSerialize.x2 = dataOUT->x2;
    auxSerialize.y2 = dataOUT->y2;
    auxSerialize.a = dataOUT->a;
    auxSerialize.b = dataOUT->b;
    auxSerialize.c = dataOUT->c;
    auxSerialize.d = dataOUT->d;

	packetsize = pack(buf, "CHddddhhhh",    'A',
                                            0,
                                            auxSerialize.x1,
                                            auxSerialize.y1,
                                            auxSerialize.x2,
                                            auxSerialize.y2,
                                            auxSerialize.a,
                                            auxSerialize.b,
                                            auxSerialize.c,
                                            auxSerialize.d
                                            );

	packi16(buf+1, packetsize); // store packet size in packet for kicks

    memcpy((unsigned char*)buf3, buf, packetsize);

    return NULL;
}

int struct_comStruc::printStruct(char* data2print)
{
    unsigned char buf[1024];
	unsigned char magic;
	unsigned int ps2;

    memcpy(buf, data2print, 1024);


	unpack(buf, "CHffffhhhh",   &magic,
                                &ps2,
                                &auxUnSerialize.x1,
                                &auxUnSerialize.y1,
                                &auxUnSerialize.x2,
                                &auxUnSerialize.y2,
                                &auxUnSerialize.a,
                                &auxUnSerialize.b,
                                &auxUnSerialize.c,
                                &auxUnSerialize.d
                                );

	printf("data: '%c' %hhu %f %f %f %f %d %d %d %d\n", magic,
                                                        ps2,
                                                        auxUnSerialize.x1,
                                                        auxUnSerialize.y1,
                                                        auxUnSerialize.x2,
                                                        auxUnSerialize.y2,
                                                        auxUnSerialize.a,
                                                        auxUnSerialize.b,
                                                        auxUnSerialize.c,
                                                        auxUnSerialize.d
                                                        );
    return 0;
}

char *struct_comStruc::Unserialize(char* buf3)
{
	unsigned char buf[1024];
	unsigned char magic;
	unsigned int ps2;

    memcpy(buf, buf3, 1024);

	unpack(buf, "CHffffhhhh",   &magic,
                                &ps2,
                                &auxUnSerialize.x1,
                                &auxUnSerialize.y1,
                                &auxUnSerialize.x2,
                                &auxUnSerialize.y2,
                                &auxUnSerialize.a,
                                &auxUnSerialize.b,
                                &auxUnSerialize.c,
                                &auxUnSerialize.d
                                );


    dataIN->x1 = auxUnSerialize.x1;
	dataIN->y1 = auxUnSerialize.y1;
    dataIN->x2 = auxUnSerialize.x2;
	dataIN->y2 = auxUnSerialize.y2;
    dataIN->a = auxUnSerialize.a;
	dataIN->b = auxUnSerialize.b;
    dataIN->c = auxUnSerialize.c;
	dataIN->d = auxUnSerialize.d;

	dataIN->newValue = true;

    return NULL;
}
