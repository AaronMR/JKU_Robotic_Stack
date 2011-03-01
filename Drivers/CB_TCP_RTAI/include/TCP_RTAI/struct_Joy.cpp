#include "pack2.hpp"
#include "structType_C.hpp"

struct_Joy::struct_Joy()
{
    haveSubscriber = false;
    havePublisher = false;

    ros::spinOnce();

    auxSerialize.axes[0]=0.0;
    auxSerialize.axes[1]=0.0;
    auxSerialize.axes[2]=0.0;
    auxSerialize.axes[3]=0.0;

    auxSerialize.buttons[0]=0;
    auxSerialize.buttons[1]=0;
    auxSerialize.buttons[2]=0;
    auxSerialize.buttons[3]=0;

    auxUnSerialize.axes[0]=0.0;
    auxUnSerialize.axes[1]=0.0;
    auxUnSerialize.axes[2]=0.0;
    auxUnSerialize.axes[3]=0.0;

    auxUnSerialize.buttons[0]=0;
    auxUnSerialize.buttons[1]=0;
    auxUnSerialize.buttons[2]=0;
    auxUnSerialize.buttons[3]=0;

    mutex = PTHREAD_MUTEX_INITIALIZER;
    canRecv_t = true;
    canSend_t = true;
}


void struct_Joy::storeData(Joy *joy)
{
    return;
}

void struct_Joy::cmdCallback(const joy::Joy::ConstPtr& joy)
{
    auxSerialize.axes[0]=joy->axes[0];
    auxSerialize.axes[1]=joy->axes[1];
    auxSerialize.axes[2]=joy->axes[2];
    auxSerialize.axes[3]=joy->axes[3];


    auxSerialize.buttons[0]=joy->buttons[0];
    auxSerialize.buttons[1]=joy->buttons[1];
    auxSerialize.buttons[2]=joy->buttons[2];
    auxSerialize.buttons[3]=joy->buttons[3];

    pthread_mutex_lock(&mutex);

    //canRecv_t = true; //rand() % 2 + -1;
    canSend_t = true; //rand() % 2 + -1;

    pthread_mutex_unlock(&mutex);

}

void* struct_Joy::set_Subscriber(char* name)
{
    joy_sub = n.subscribe<joy::Joy>(name, 10, &struct_Joy::cmdCallback, this);
    haveSubscriber = true;
    return NULL;
}

void* struct_Joy::set_Publisher(char* name)
{
    joy_pub  = n.advertise<joy::Joy>(name, 1);

    havePublisher = true;
    return NULL;
}



bool struct_Joy::canSend()
{
    /*
    int rc = 0;
    int aux = 0;

    rc = pthread_mutex_lock(&mutex);
    aux = canSend_t;
    canSend_t = true;
    rc = pthread_mutex_unlock(&mutex);

    return aux;
    */
}

bool struct_Joy::canRecv()
{
    /*
    int rc = 0;
    int aux = 0;

    rc = pthread_mutex_lock(&mutex);
    aux = canRecv_t;
    canRecv_t = true;
    rc = pthread_mutex_unlock(&mutex);

    return aux;
    */
}

int struct_Joy::spinOnce()
{
    //ros::spinOnce();
}

int struct_Joy::serialize(char* data2s)
{
	unsigned char buf[1024];
	unsigned int packetsize = 0;

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

    memcpy((unsigned char*)data2s, buf, packetsize);

    return 0;
}

int struct_Joy::printStruct(char* data2print)
{
    unsigned char buf[1024];
	unsigned char magic;
    unsigned int ps2;

    memcpy(buf, data2print, 1024);

    Joy aux;
	unpack(buf, "CHffffhhhh",  &magic,
                                            &ps2,
                                            &aux.axes[0],
                                            &aux.axes[1],
                                            &aux.axes[2],
                                            &aux.axes[3],
                                            &aux.buttons[0],
                                            &aux.buttons[1],
                                            &aux.buttons[2],
                                            &aux.buttons[3]);


	printf("recv: '%c' %hhu %f %f %f %f %d %d %d %d\n",   magic,
                                                    ps2,
                                                    aux.axes[0],
                                                    aux.axes[1],
                                                    aux.axes[2],
                                                    aux.axes[3],
                                                    aux.buttons[0],
                                                    aux.buttons[1],
                                                    aux.buttons[2],
                                                    aux.buttons[3]);
    return 0;
}

int struct_Joy::Unserialize(char* buf3)
{
	unsigned char buf[1024];
	unsigned char magic;
    unsigned int ps2;

    memcpy(buf, buf3, 1024);


	unpack(buf, "CHffffhhhh",  &magic,
                                            &ps2,
                                            &auxUnSerialize.axes[0],
                                            &auxUnSerialize.axes[1],
                                            &auxUnSerialize.axes[2],
                                            &auxUnSerialize.axes[3],
                                            &auxUnSerialize.buttons[0],
                                            &auxUnSerialize.buttons[1],
                                            &auxUnSerialize.buttons[2],
                                            &auxUnSerialize.buttons[3]);


    if(havePublisher)
    {
        joy_msg.buttons.resize(4);
        joy_msg.axes.resize(4);

        joy_msg.axes[0] = auxUnSerialize.axes[0];
        joy_msg.axes[1] = auxUnSerialize.axes[1];
        joy_msg.axes[2] = auxUnSerialize.axes[2];
        joy_msg.axes[3] = auxUnSerialize.axes[3];

        joy_msg.buttons[0] = auxUnSerialize.buttons[0];
        joy_msg.buttons[1] = auxUnSerialize.buttons[1];
        joy_msg.buttons[2] = auxUnSerialize.buttons[2];
        joy_msg.buttons[3] = auxUnSerialize.buttons[3];

        joy_pub.publish(joy_msg);
    }
    return 0;
}
