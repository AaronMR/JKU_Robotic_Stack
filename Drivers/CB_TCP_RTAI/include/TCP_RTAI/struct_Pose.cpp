#include "pack2.hpp"
#include "structType_C.hpp"

struct_Pose::struct_Pose()
{
    haveSubscriber = false;
    havePublisher = false;

    ros::spinOnce();

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

int struct_Pose::spinOnce()
{
    ros::spinOnce();
}

bool struct_Pose::canSend()
{
    /*
    int rc = 0;
    int aux = 0;

    rc = pthread_mutex_lock(&mutex);
    aux = canSend_t;
    rc = pthread_mutex_unlock(&mutex);

    return aux;
    */
}

bool struct_Pose::canRecv()
{
    /*
    int rc = 0;
    int aux = 0;

    rc = pthread_mutex_lock(&mutex);
    aux = canRecv_t;
    rc = pthread_mutex_unlock(&mutex);

    return aux;
    */
}

void struct_Pose::storeData(Joy *joy)
{

}


void struct_Pose::cmdCallback(const geometry_msgs::Pose& msg)
{
    auxSerialize.position.x = msg.position.x;
    auxSerialize.position.y = msg.position.y;
    auxSerialize.position.z = msg.position.z;

    auxSerialize.orientation.x = msg.orientation.x;
    auxSerialize.orientation.y = msg.orientation.y;
    auxSerialize.orientation.z = msg.orientation.z;
    auxSerialize.orientation.w = msg.orientation.w;

}

void* struct_Pose::set_Subscriber(char* name)
{
    Pose_sub = n.subscribe(name, 10, &struct_Pose::cmdCallback, this);
    haveSubscriber = true;
    return NULL;
}

void* struct_Pose::set_Publisher(char* name)
{
    Pose_pub  = n.advertise<geometry_msgs::Pose>(name, 1);

    havePublisher = true;
    return NULL;
}



int struct_Pose::serialize(char* buf3)
{
    ros::spinOnce();
	unsigned char buf[1024];
	unsigned char magic;
	unsigned int packetsize, ps2;

    packetsize = pack(buf, "CHddddddd",    'A',
                                            0,
                                            auxSerialize.position.x,
                                            auxSerialize.position.y,
                                            auxSerialize.position.z,
                                            auxSerialize.orientation.x,
                                            auxSerialize.orientation.y,
                                            auxSerialize.orientation.z,
                                            auxSerialize.orientation.w
                                            );

	packi16(buf+1, packetsize); // store packet size in packet for kicks


    memcpy((unsigned char*)buf3, buf, packetsize);


	unpack((unsigned char*)buf3, "CHddddddd",  &magic,
                                            &ps2,
                                            &auxSerialize.position.x,
                                            &auxSerialize.position.y,
                                            &auxSerialize.position.z,
                                            &auxSerialize.orientation.x,
                                            &auxSerialize.orientation.y,
                                            &auxSerialize.orientation.z,
                                            &auxSerialize.orientation.w);

    return 0;
}

int struct_Pose::printStruct(char* data2print)
{
    unsigned char buf[1024];
	unsigned char magic;
    unsigned int ps2;

    memcpy(buf, data2print, 1024);

    Pose aux;

    unpack((unsigned char*)buf, "CHddddddd",  &magic,
                                            &ps2,
                                            &aux.position.x,
                                            &aux.position.y,
                                            &aux.position.z,
                                            &aux.orientation.x,
                                            &aux.orientation.y,
                                            &aux.orientation.z,
                                            &aux.orientation.w);


	printf("send: '%c' %hhu %f %f %f %f %f %f %f\n",   magic,
                                                        ps2,
                                                        aux.position.x,
                                                        aux.position.y,
                                                        aux.position.z,
                                                        aux.orientation.x,
                                                        aux.orientation.y,
                                                        aux.orientation.z,
                                                        aux.orientation.w);
}
int struct_Pose::Unserialize(char* buf3)
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
                                            &auxUnSerialize.orientation.w);


    if(havePublisher)
    {

        pose_msg.position.x = auxUnSerialize.position.x;
        pose_msg.position.y = auxUnSerialize.position.y;
        pose_msg.position.z = auxUnSerialize.position.z;

        pose_msg.orientation.x = auxUnSerialize.orientation.x;
        pose_msg.orientation.y = auxUnSerialize.orientation.y;
        pose_msg.orientation.z = auxUnSerialize.orientation.z;
        pose_msg.orientation.w = auxUnSerialize.orientation.w;

        Pose_pub.publish(pose_msg);

    }
    return  0;
}
