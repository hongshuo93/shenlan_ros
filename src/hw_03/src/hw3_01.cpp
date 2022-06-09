#include <iostream>
#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <turtlesim/Pose.h>
void TurtlePoseCallback(const turtlesim::PoseConstPtr& ptr){
    ROS_INFO("subscribe /turtle1/pose (%f,%f), %f",ptr->x,ptr->y,ptr->theta);
}
int main(int argc,char** argv){
    ros::init(argc,argv,"hw03_01_node");
    ros::NodeHandle node;
    ros::Rate rate(10);
    ros::Publisher velPublisher = node.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel",16);
    ros::Subscriber poseSub = node.subscribe<turtlesim::Pose>("/turtle1/pose",16,TurtlePoseCallback);
    while(ros::ok()){
        geometry_msgs::Twist cmd;
        cmd.angular.z = 0.5;
        cmd.linear.x = 1;
        velPublisher.publish(cmd);
        ros::spinOnce();
        rate.sleep();
    }
    return 0;
}