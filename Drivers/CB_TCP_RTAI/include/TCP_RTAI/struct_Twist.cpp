#include "transporter_C.hpp"
#include "pack2.hpp"

//############################################### struct_Twist ##############################

struct_Twist::struct_Twist()
{
    haveSubscriber = false;
    havePublisher = false;

    ros::spinOnce();

    mutex = PTHREAD_MUTEX_INITIALIZER;
    canRecv_t = true;
    canSend_t = true;

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


int struct_Twist::spinOnce()
{
    ros::spinOnce();
}

bool struct_Twist::canSend()
{
    int rc = 0;
    int aux = 0;

    rc = pthread_mutex_lock(&mutex);
    aux = canSend_t;
    rc = pthread_mutex_unlock(&mutex);

    return aux;
}

bool struct_Twist::canRecv()
{
    int rc = 0;
    int aux = 0;

    rc = pthread_mutex_lock(&mutex);
    aux = canRecv_t;
    rc = pthread_mutex_unlock(&mutex);

    return aux;
}

void struct_Twist::storeData(Joy *joy)
{
    return;
}


void struct_Twist::cmdCallback(const geometry_msgs::Twist &msg)
{

    auxSerialize.angular.x = msg.angular.x;
    auxSerialize.angular.y = msg.angular.y;
    auxSerialize.angular.z = msg.angular.z;

    auxSerialize.linear.x = msg.linear.x;
    auxSerialize.linear.y = msg.linear.y;
    auxSerialize.linear.z = msg.linear.z;

    //cout << "estoy en el callback del struct_Joy_R" << endl;

    //cout << "Estoy aki dentro" << endl;


}

void* struct_Twist::set_Subscriber(char* name)
{
    twist_sub = n.subscribe(name, 10, &struct_Twist::cmdCallback, this);
    haveSubscriber = true;
    return NULL;
}

void* struct_Twist::set_Publisher(char* name)
{
    twist_pub  = n.advertise<geometry_msgs::Twist>(name, 1);

    havePublisher = true;
    return NULL;
}



int struct_Twist::serialize(char* buf3)
{
    ros::spinOnce();
	unsigned char buf[1024];

	unsigned int packetsize = 0;

	packetsize = pack(buf, "CHdddddd",    'A',
                                            0,
                                            auxSerialize.linear.x,
                                            auxSerialize.linear.y,
                                            auxSerialize.linear.z,
                                            auxSerialize.angular.x,
                                            auxSerialize.angular.y,
                                            auxSerialize.angular.z
                                            );

	packi16(buf+1, packetsize); // store packet size in packet for kicks


    memcpy((unsigned char*)buf3, buf, packetsize);

    return 0;
}


int struct_Twist::printStruct(char* data2print)
{
    unsigned char buf[1024];
	unsigned char magic;
    unsigned int ps2;

    memcpy(buf, data2print, 1024);

    Twist aux;

	unpack((unsigned char*)buf, "CHdddddd",  &magic,
                                            &ps2,
                                            &aux.angular.x,
                                            &aux.angular.y,
                                            &aux.angular.z,
                                            &aux.linear.x,
                                            &aux.linear.y,
                                            &aux.linear.z);


	printf("data: '%c' %hhu %f %f %f %f %f %f\n",   magic,
                                                    ps2,
                                                    aux.angular.x,
                                                    aux.angular.y,
                                                    aux.angular.z,
                                                    aux.linear.x,
                                                    aux.linear.y,
                                                    aux.linear.z);

}
int struct_Twist::Unserialize(char* buf3)
{
	unsigned char buf[1024];
	unsigned char magic;
    unsigned int ps2;

    memcpy(buf, buf3, 1024);

	unpack((unsigned char*)buf, "CHdddddd",  &magic,
                                            &ps2,
                                            &auxUnSerialize.angular.x,
                                            &auxUnSerialize.angular.y,
                                            &auxUnSerialize.angular.z,
                                            &auxUnSerialize.linear.x,
                                            &auxUnSerialize.linear.y,
                                            &auxUnSerialize.linear.z);

    if(havePublisher)
    {

        twist_msg.angular.x = auxUnSerialize.angular.x;
        twist_msg.angular.y = auxUnSerialize.angular.y;
        twist_msg.angular.z = auxUnSerialize.angular.z;
        twist_msg.linear.x = auxUnSerialize.linear.x;
        twist_msg.linear.y = auxUnSerialize.linear.y;
        twist_msg.linear.z = auxUnSerialize.linear.z;

        twist_pub.publish(twist_msg);
    }
    return  NULL;
}

