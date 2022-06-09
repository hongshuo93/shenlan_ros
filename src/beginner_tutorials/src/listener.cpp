#include <iostream>
#include "ros/ros.h"
#include "std_msgs/String.h"
#include <unistd.h>
void chatterCallback(const std_msgs::String::ConstPtr& msg)
{
     ROS_INFO("I heard: [%s] in thread: %u", msg->data.c_str(),gettid());
}
 
int main(int argc,char** argv){
    ros::init(argc,argv,"listener");
    ros::NodeHandle n;
    auto subscriber = n.subscribe<std_msgs::String>("chatter",1024,chatterCallback);
    ROS_INFO("Init listener in tid: %d",gettid());
    ros::Rate loop_rate(1);
    int count = 0;
    while(ros::ok()){
        ROS_INFO(" in node:%s loop:%d, tid:%d",ros::this_node::getName().c_str(),count++,gettid());
        ros::spinOnce();
        loop_rate.sleep();
    }
   
    return 0;
}