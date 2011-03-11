#include <ros/ros.h>
#include "comStruct.h"
#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/Twist.h>
#include <joy/Joy.h>
#include <geometry_msgs/Point.h>
#include <geometry_msgs/Polygon.h>

class structType_C {
public:

    virtual int serialize(char* data2s) = 0;
    virtual int Unserialize(char* data2us) = 0;
    virtual void* set_Publisher(char* name) = 0;
    virtual void* set_Subscriber(char* name) = 0;
    virtual int printStruct(char* data2print)= 0;
    virtual bool canSend() = 0;
    virtual bool canRecv() = 0;
    virtual int spinOnce() = 0;
    void storeData();
};

class struct_Joy : public structType_C {
public:
    struct_Joy();

    int serialize(char* data2s);
    int Unserialize(char* data2us);
    int printStruct(char* data2print);
    void storeData(Joy *joy);
    void* set_Publisher(char* name);
    void* set_Subscriber(char* name);

    ros::NodeHandle n;
    ros::Publisher joy_pub;
    ros::Subscriber joy_sub;

    // struct to send and receive
    Joy auxSerialize;
    Joy auxUnSerialize;

    joy::Joy joy_msg;

    void cmdCallback(const joy::Joy::ConstPtr& data);

    bool haveSubscriber;
    bool havePublisher;

    pthread_mutex_t mutex;

    bool canRecv_t;
    bool canSend_t;

    bool canSend();
    bool canRecv();

    int spinOnce();

};

class struct_Twist : public structType_C {
public:
    struct_Twist();

    int serialize(char* data2s);
    int Unserialize(char* data2us);
    int printStruct(char* data2print);
    void storeData(Joy *joy);
    void* set_Publisher(char* name);
    void* set_Subscriber(char* name);

    ros::NodeHandle n;
    ros::Publisher twist_pub;
    ros::Subscriber twist_sub;

    // struct to send and receive
    Twist auxSerialize;
    Twist auxUnSerialize;

    geometry_msgs::Twist twist_msg;

    void cmdCallback(const geometry_msgs::Twist& data);

    bool haveSubscriber;
    bool havePublisher;

    pthread_mutex_t mutex;

    bool canRecv_t;
    bool canSend_t;

    bool canSend();
    bool canRecv();

    int spinOnce();

};


class struct_Pose : public structType_C {
public:
    struct_Pose();

    int serialize(char* data2s);
    int Unserialize(char* data2us);
    int printStruct(char* data2print);
    void storeData(Joy *joy);
    void* set_Publisher(char* name);
    void* set_Subscriber(char* name);

    ros::NodeHandle n;
    ros::Publisher Pose_pub;
    ros::Subscriber Pose_sub;

    // struct to send and receive
    Pose auxSerialize;
    Pose auxUnSerialize;

    geometry_msgs::Pose pose_msg;

    void cmdCallback(const geometry_msgs::Pose& data);

    bool haveSubscriber;
    bool havePublisher;

    pthread_mutex_t mutex;

    bool canRecv_t;
    bool canSend_t;

    bool canSend();
    bool canRecv();

    int spinOnce();

};

class struct_posWheels : public structType_C {

public:

    struct_posWheels();
    int serialize(char* data2s);
    int Unserialize(char* data2us);
    int printStruct(char* data2print);

    void* set_Publisher(char* name);
    void* set_Subscriber(char* name);

    ros::NodeHandle n;
    ros::Publisher posWheels_pub;
    ros::Subscriber posWheels_sub;

    // struct to send and receive
    posWheels_t auxSerialize;
    posWheels_t auxUnSerialize;

    geometry_msgs::Polygon polygon_msg;

    void cmdCallback(const geometry_msgs::Polygon &data);
    bool haveSubscriber;
    bool havePublisher;
    pthread_mutex_t mutex;
    bool canRecv_t;
    bool canSend_t;
    bool canSend();
    bool canRecv();
    int spinOnce();
};

class struct_Odometry : public structType_C {

public:

    struct_Odometry();
    int serialize(char* data2s);
    int Unserialize(char* data2us);
    int printStruct(char* data2print);
    void* set_Publisher(char* name);
    void* set_Subscriber(char* name);

    ros::NodeHandle n;
    ros::Publisher odometry_pub;
    ros::Subscriber odometry_sub;

    // struct to send and receive
    odometry_t auxSerialize;
    odometry_t auxUnSerialize;

    nav_msgs::Odometry odometry_msg;

    void cmdCallback(const nav_msgs::Odometry &data);
    bool haveSubscriber;
    bool havePublisher;
    pthread_mutex_t mutex;
    bool canRecv_t;
    bool canSend_t;
    bool canSend();
    bool canRecv();
    int spinOnce();
};
