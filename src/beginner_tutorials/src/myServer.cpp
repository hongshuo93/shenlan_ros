#include "ros/ros.h"
#include "std_msgs/String.h"
#include "beginner_tutorials/AddTwoInts.h"
#include <unistd.h>
bool add(beginner_tutorials::AddTwoInts::Request &req, beginner_tutorials::AddTwoInts::Response &res){
    int a = req.a;
    int b = req.b;
    int ans = a+b;
    ROS_INFO("recv: a:%4d, b:%4d, ans:%4d, in node:%s, tid:%ld",a,b,ans,ros::this_node::getName().c_str(),gettid());
    res.sum = ans;
    return true;
}
#include <iostream>
int main(int argc,char** argv){
    ros::init(argc,argv,"myServer");
    ros::NodeHandle n;
    ros::Rate loop_rate(1);
    ros::ServiceServer server = n.advertiseService("add_two_ints",add);
    int count = 0;
    while(ros::ok()){
        ROS_INFO(" in node:%s loop:%d, tid:%sd",ros::this_node::getName().c_str(),count++,gettid());
        ros::spinOnce();
        loop_rate.sleep();
    }
  
    return 0;
}