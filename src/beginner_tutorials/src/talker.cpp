#include "ros/ros.h"
#include "std_msgs/String.h"
#include <sstream>
#include <iostream>
#include <unistd.h>
int main(int argc,char **argv){
    ros::init(argc,argv,"talker");
    ros::NodeHandle n;
    ros::Publisher chatter_pub = n.advertise<std_msgs::String>("chatter",1024);
    ros::Rate loop_rate(60);
    int count =0;
    while(ros::ok()){
        std_msgs::String msg;
        std::stringstream ss;
        ss<<count;
        msg.data = ss.str();
        ROS_INFO("%s",msg.data.c_str());
        chatter_pub.publish(msg);
        ROS_INFO("sendMsg in node:%s loop:%d, tid:%d",ros::this_node::getName().c_str(),count++,gettid());
        ros::spinOnce();
        loop_rate.sleep();
        ++count;
    }
    return 0;
}