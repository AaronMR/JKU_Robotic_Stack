#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>
#include <joy/Joy.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>



joy::Joy auxJoy1;

geometry_msgs::Twist twist;
geometry_msgs::Pose pose;
nav_msgs::Odometry odom_2;
nav_msgs::Odometry odom_3;
nav_msgs::Odometry odom_4;

void cmdCallback(const joy::Joy::ConstPtr& joy)
{
    
}

void cmdTwistCallback(const nav_msgs::Odometry &pose_aux)
{

    odom_2.pose.pose.position.x = pose_aux.pose.pose.position.x;
    odom_2.pose.pose.position.y = pose_aux.pose.pose.position.y;
    odom_2.pose.pose.position.z = 0.0;

    //odom_2.pose.pose.orientation.x = pose_aux.pose.pose.orientation.x;
    //odom_2.pose.pose.orientation.y = pose_aux.pose.pose.orientation.y;
    odom_2.pose.pose.orientation.z = pose_aux.pose.pose.orientation.z;
    odom_2.pose.pose.orientation.w = pose_aux.pose.pose.orientation.w;

}

void cmd2TwistCallback(const geometry_msgs::PoseWithCovarianceStamped& pose2_aux)
{

    odom_3.pose.pose.position.x = pose2_aux.pose.pose.position.x;
    odom_3.pose.pose.position.y = pose2_aux.pose.pose.position.y;
    odom_3.pose.pose.position.z = pose2_aux.pose.pose.position.z;

    odom_3.pose.pose.orientation.x = pose2_aux.pose.pose.orientation.x;
    odom_3.pose.pose.orientation.y = pose2_aux.pose.pose.orientation.y;
    odom_3.pose.pose.orientation.z = pose2_aux.pose.pose.orientation.z;
    odom_3.pose.pose.orientation.w = pose2_aux.pose.pose.orientation.w;
/*
  twist.linear.x = twist_aux.linear.x;
  twist.linear.y = twist_aux.linear.y;
  twist.linear.z = twist_aux.linear.z;
  twist.angular.x = twist_aux.angular.x;
  twist.angular.y = twist_aux.angular.y;
  twist.angular.z = twist_aux.angular.z;
*/
}

int main(int argc, char** argv){
  ros::init(argc, argv, "r1_odom_2");

  ros::NodeHandle n;
  ros::Publisher odom_pub = n.advertise<nav_msgs::Odometry>("odom", 50);
  tf::TransformBroadcaster odom_broadcaster;

  //ros::Subscriber joy_sub = n.subscribe("joy", 10, cmdCallback);

  ros::Subscriber pose_sub = n.subscribe("odom_1", 10, cmdTwistCallback);

  ros::Subscriber pose2_sub = n.subscribe("amcl_pose", 10, cmd2TwistCallback);

  odom_2.pose.pose.position.x = 0.0;
  odom_2.pose.pose.position.y = 0.0;
  odom_2.pose.pose.position.z = 0.0;

  odom_2.pose.pose.orientation.x = 0.0;
  odom_2.pose.pose.orientation.y = 0.0;
  odom_2.pose.pose.orientation.z = 0.0;
  odom_2.pose.pose.orientation.w = 1.0;

  twist.linear.x = 0.0;
  twist.linear.y = 0.0;
  twist.linear.z = 0.0;
  twist.angular.x = 0.0;
  twist.angular.y = 0.0;
  twist.angular.z = 0.0;

pose.position.x = 0.0;
pose.position.x = 0.0;
pose.position.x = 0.0;

pose.orientation.x = 0.0;
pose.orientation.y = 0.0;
pose.orientation.z = 0.0;
pose.orientation.w = 1.0;

  auxJoy1.buttons.resize(4);
  auxJoy1.axes.resize(4);

  double x = 0.0;
  double y = 0.0;
  double th = 0.0;

  double vx = 0.0;
  double vy = 0.0;
  double vth = 0.0;

  ros::Time current_time, last_time;
  current_time = ros::Time::now();
  last_time = ros::Time::now();

  ros::Rate r(10.0);
  while(n.ok()){

    odom_4.pose.pose.position.x = odom_2.pose.pose.position.x + odom_3.pose.pose.position.x;
    odom_4.pose.pose.position.y = odom_2.pose.pose.position.y + odom_3.pose.pose.position.y;
    odom_4.pose.pose.position.z = odom_2.pose.pose.position.z + odom_3.pose.pose.position.z;

    odom_4.pose.pose.orientation.x = odom_2.pose.pose.orientation.x + odom_3.pose.pose.orientation.x;
    odom_4.pose.pose.orientation.y = odom_2.pose.pose.orientation.y + odom_3.pose.pose.orientation.y;
    odom_4.pose.pose.orientation.z = odom_2.pose.pose.orientation.z + odom_3.pose.pose.orientation.z;
    odom_4.pose.pose.orientation.w = odom_2.pose.pose.orientation.w + odom_3.pose.pose.orientation.w;

    ros::spinOnce();
    current_time = ros::Time::now();

    vx = twist.linear.x;
    vy = twist.linear.y;
    vth = twist.angular.z;

    	

    //compute odometry in a typical way given the velocities of the robot
    double dt = (current_time - last_time).toSec();
    double delta_x = (vx * cos(th) - vy * sin(th)) * dt;
    double delta_y = (vx * sin(th) + vy * cos(th)) * dt;
    double delta_th = vth * dt;

    x += delta_x;
    y += delta_y;
    th += delta_th;

    //since all odometry is 6DOF we'll need a quaternion created from yaw
    geometry_msgs::Quaternion odom_quat = tf::createQuaternionMsgFromYaw(th);

    //first, we'll publish the transform over tf
    geometry_msgs::TransformStamped odom_trans;
    odom_trans.header.stamp = current_time;
    odom_trans.header.frame_id = "/odom_combined";
    odom_trans.child_frame_id = "/base_footprint";

    odom_trans.transform.translation.x = odom_2.pose.pose.position.x;
    odom_trans.transform.translation.y = odom_2.pose.pose.position.y;
    odom_trans.transform.translation.z = 0.0;
    
    odom_trans.transform.rotation.x = odom_2.pose.pose.orientation.x;
    odom_trans.transform.rotation.y = odom_2.pose.pose.orientation.y;
    odom_trans.transform.rotation.z = odom_2.pose.pose.orientation.z;
    odom_trans.transform.rotation.w = odom_2.pose.pose.orientation.w;

/*
    printf("%f \n",odom_trans.transform.rotation.x);
    printf("%f \n",odom_trans.transform.rotation.y);
    printf("%f \n",odom_trans.transform.rotation.z);
    printf("%f \n",odom_trans.transform.rotation.w);
*/
    //odom_trans.transform.rotation = odom_quat;

    //send the transform
    odom_broadcaster.sendTransform(odom_trans);

    //next, we'll publish the odometry message over ROS
    nav_msgs::Odometry odom;
    odom.header.stamp = current_time;
    odom.header.frame_id = "/odom_combined";
    odom.child_frame_id = "/base_footprint";

    //set the position
    odom.pose.pose.position.x = odom_2.pose.pose.position.x; //x;
    odom.pose.pose.position.y = odom_2.pose.pose.position.y; //y;
    odom.pose.pose.position.z = 0.0;

//    odom.pose.pose.orientation = odom_quat;
    //odom.pose.pose.orientation.x = odom_2.pose.pose.orientation.x;
    //odom.pose.pose.orientation.y = odom_2.pose.pose.orientation.y;
    odom.pose.pose.orientation.z = odom_2.pose.pose.orientation.z;
    odom.pose.pose.orientation.w = odom_2.pose.pose.orientation.w;

    //set the velocity
    
    odom.twist.twist.linear.x = vx;
    odom.twist.twist.linear.y = vy;
    odom.twist.twist.angular.z = vth;

    //publish the message
    odom_pub.publish(odom);

    last_time = current_time;
    r.sleep();
  }
}

