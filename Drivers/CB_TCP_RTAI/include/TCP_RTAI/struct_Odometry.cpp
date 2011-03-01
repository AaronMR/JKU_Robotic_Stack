#include "pack2.hpp"
#include "structType_C.hpp"

struct_Odometry::struct_Odometry()
{

    haveSubscriber  = false;
    havePublisher   = false;
    canRecv_t       = true;
    canSend_t       = true;
    mutex           = PTHREAD_MUTEX_INITIALIZER;

    // initialitation structure auxSerialize
    auxSerialize.pose.pose.position.x = 0.0;
    auxSerialize.pose.pose.position.y = 0.0;
    auxSerialize.pose.pose.position.z = 0.0;

    auxSerialize.pose.pose.orientation.x = 0.0;
    auxSerialize.pose.pose.orientation.y = 0.0;
    auxSerialize.pose.pose.orientation.z = 0.0;
    auxSerialize.pose.pose.orientation.w = 1.0;

    auxSerialize.twist.twist.angular.x = 0.0;
    auxSerialize.twist.twist.angular.y = 0.0;
    auxSerialize.twist.twist.angular.z = 0.0;

    auxSerialize.twist.twist.linear.x = 0.0;
    auxSerialize.twist.twist.linear.y = 0.0;
    auxSerialize.twist.twist.linear.z = 0.0;

    // initialitation structure auxUnSerialize
    auxUnSerialize.pose.pose.position.x = 0.0;
    auxUnSerialize.pose.pose.position.y = 0.0;
    auxUnSerialize.pose.pose.position.z = 0.0;

    auxUnSerialize.pose.pose.orientation.x = 0.0;
    auxUnSerialize.pose.pose.orientation.y = 0.0;
    auxUnSerialize.pose.pose.orientation.z = 0.0;
    auxUnSerialize.pose.pose.orientation.w = 1.0;

    auxUnSerialize.twist.twist.angular.x = 0.0;
    auxUnSerialize.twist.twist.angular.y = 0.0;
    auxUnSerialize.twist.twist.angular.z = 0.0;

    auxUnSerialize.twist.twist.linear.x = 0.0;
    auxUnSerialize.twist.twist.linear.y = 0.0;
    auxUnSerialize.twist.twist.linear.z = 0.0;

}

void struct_Odometry::cmdCallback(const nav_msgs::Odometry &data_)
{

    auxSerialize.pose.pose.position.x = 0.0;
    auxSerialize.pose.pose.position.y = 0.0;
    auxSerialize.pose.pose.position.z = 0.0;

    auxSerialize.pose.pose.orientation.x = 0.0;
    auxSerialize.pose.pose.orientation.y = 0.0;
    auxSerialize.pose.pose.orientation.z = 0.0;
    auxSerialize.pose.pose.orientation.w = 0.0;

    auxSerialize.twist.twist.angular.x = 0.0;
    auxSerialize.twist.twist.angular.y = 0.0;
    auxSerialize.twist.twist.angular.z = 0.0;

    auxSerialize.twist.twist.linear.x = 0.0;
    auxSerialize.twist.twist.linear.y = 0.0;
    auxSerialize.twist.twist.linear.z = 0.0;


    pthread_mutex_lock(&mutex);

    canSend_t = true;

    pthread_mutex_unlock(&mutex);

}

void* struct_Odometry::set_Subscriber(char* name)
{

    odometry_sub = n.subscribe(name, 10, &struct_Odometry::cmdCallback, this);
    haveSubscriber = true;
    return NULL;

}

void* struct_Odometry::set_Publisher(char* name)
{

    odometry_pub  = n.advertise<nav_msgs::Odometry>(name, 1);

    havePublisher = true;
    return NULL;

    odometry_msg.pose.pose.position.x = 0.0;
    odometry_msg.pose.pose.position.y = 0.0;
    odometry_msg.pose.pose.position.z = 0.0;

    odometry_msg.pose.pose.orientation.x = 0.0;
    odometry_msg.pose.pose.orientation.y = 0.0;
    odometry_msg.pose.pose.orientation.z = 0.0;
    odometry_msg.pose.pose.orientation.w = 1.0;

    odometry_msg.twist.twist.angular.x = 0.0;
    odometry_msg.twist.twist.angular.y = 0.0;
    odometry_msg.twist.twist.angular.z = 0.0;

    odometry_msg.twist.twist.angular.x = 0.0;
    odometry_msg.twist.twist.angular.y = 0.0;
    odometry_msg.twist.twist.angular.z = 0.0;
    odometry_pub.publish(odometry_msg);

}

bool struct_Odometry::canSend()
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

bool struct_Odometry::canRecv()
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

int struct_Odometry::spinOnce()
{

}

int struct_Odometry::serialize(char* data2s)
{
	unsigned char buf[1024];
	unsigned int packetsize;



	packetsize = pack(buf, "CHddddddddddddd",    'A',
                                                0,
                                                auxSerialize.pose.pose.position.x,
                                                auxSerialize.pose.pose.position.y,
                                                auxSerialize.pose.pose.position.z,
                                                auxSerialize.pose.pose.orientation.x,
                                                auxSerialize.pose.pose.orientation.y,
                                                auxSerialize.pose.pose.orientation.z,
                                                auxSerialize.pose.pose.orientation.w,
                                                auxSerialize.twist.twist.angular.x,
                                                auxSerialize.twist.twist.angular.y,
                                                auxSerialize.twist.twist.angular.z,
                                                auxSerialize.twist.twist.linear.x,
                                                auxSerialize.twist.twist.linear.y,
                                                auxSerialize.twist.twist.linear.z
                                                );

	packi16(buf+1, packetsize); // store packet size in packet for kicks

    memcpy((unsigned char*)data2s, buf, packetsize);

    return 0;
}

int struct_Odometry::printStruct(char* data2print)
{
    unsigned char buf[1024];
	unsigned char magic;
    unsigned int ps2;

    memcpy(buf, data2print, 1024);

    odometry_t aux;
    unpack((unsigned char*)buf, "CHddddddddddddd",
                                &magic,
                                &ps2,
                                &aux.pose.pose.position.x,
                                &aux.pose.pose.position.y,
                                &aux.pose.pose.position.z,
                                &aux.pose.pose.orientation.x,
                                &aux.pose.pose.orientation.y,
                                &aux.pose.pose.orientation.z,
                                &aux.pose.pose.orientation.w,
                                &aux.twist.twist.angular.x,
                                &aux.twist.twist.angular.y,
                                &aux.twist.twist.angular.z,
                                &aux.twist.twist.linear.x,
                                &aux.twist.twist.linear.y,
                                &aux.twist.twist.linear.z
                                );
	printf("data: '%c' %hhu %f %f %f %f %f %f %f %f %f %f %f %f %f\n",    magic,
                                            ps2,
                                            aux.pose.pose.position.x,
                                            aux.pose.pose.position.y,
                                            aux.pose.pose.position.z,
                                            aux.pose.pose.orientation.x,
                                            aux.pose.pose.orientation.y,
                                            aux.pose.pose.orientation.z,
                                            aux.pose.pose.orientation.w,
                                            aux.twist.twist.angular.x,
                                            aux.twist.twist.angular.y,
                                            aux.twist.twist.angular.z,
                                            aux.twist.twist.linear.x,
                                            aux.twist.twist.linear.y,
                                            aux.twist.twist.linear.z
                                            );

    return 0;
}
int struct_Odometry::Unserialize(char* data2us)
{

	unsigned char buf[1024];
	unsigned char magic;
    unsigned int ps2;

    memcpy(buf, data2us, 1024);

    unpack((unsigned char*)buf, "CHddddddddddddd",
                                &magic,
                                &ps2,
                                &auxUnSerialize.pose.pose.position.x,
                                &auxUnSerialize.pose.pose.position.y,
                                &auxUnSerialize.pose.pose.position.z,
                                &auxUnSerialize.pose.pose.orientation.x,
                                &auxUnSerialize.pose.pose.orientation.y,
                                &auxUnSerialize.pose.pose.orientation.z,
                                &auxUnSerialize.pose.pose.orientation.w,
                                &auxUnSerialize.twist.twist.angular.x,
                                &auxUnSerialize.twist.twist.angular.y,
                                &auxUnSerialize.twist.twist.angular.z,
                                &auxUnSerialize.twist.twist.linear.x,
                                &auxUnSerialize.twist.twist.linear.y,
                                &auxUnSerialize.twist.twist.linear.z
                                );


    if(havePublisher)
    {

        odometry_msg.pose.pose.position.x = auxUnSerialize.pose.pose.position.x;
        odometry_msg.pose.pose.position.y = auxUnSerialize.pose.pose.position.y;
        odometry_msg.pose.pose.position.z = auxUnSerialize.pose.pose.position.z;

        odometry_msg.pose.pose.orientation.x = auxUnSerialize.pose.pose.orientation.x;
        odometry_msg.pose.pose.orientation.y = auxUnSerialize.pose.pose.orientation.y;
        odometry_msg.pose.pose.orientation.z = auxUnSerialize.pose.pose.orientation.z;
        odometry_msg.pose.pose.orientation.w = auxUnSerialize.pose.pose.orientation.w;

        odometry_msg.twist.twist.angular.x = auxUnSerialize.twist.twist.angular.x;
        odometry_msg.twist.twist.angular.y = auxUnSerialize.twist.twist.angular.y;
        odometry_msg.twist.twist.angular.z = auxUnSerialize.twist.twist.angular.z;

        odometry_msg.twist.twist.angular.x = auxUnSerialize.twist.twist.linear.x;
        odometry_msg.twist.twist.angular.y = auxUnSerialize.twist.twist.linear.y;
        odometry_msg.twist.twist.angular.z = auxUnSerialize.twist.twist.linear.z;

        odometry_pub.publish(odometry_msg);

    }

    return 0;
}
