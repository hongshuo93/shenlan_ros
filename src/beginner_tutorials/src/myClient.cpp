#include "ros/ros.h"
#include "std_msgs/String.h"
#include "beginner_tutorials/AddTwoInts.h"
#include <iostream>

int main(int argc,char ** argv){
    ros::init(argc,argv,"myClient");
    ros::NodeHandle n;
    ros::ServiceClient client = n.serviceClient<beginner_tutorials::AddTwoInts>("add_two_ints");
    ros::Rate loop_rate(120);
    ROS_INFO("Init node:%s, tid:%d",ros::this_node::getName().c_str(), gettid());
    while(ros::ok()){
        int numA = rand()%10;
        int numB = rand()%10;
       
        beginner_tutorials::AddTwoInts srv;
        srv.request.a = numA;
        srv.request.b = numB;
        if(client.call(srv)){
             ROS_INFO("rand %4d %4d ans:%4d ,in node:%s, tid:%d",numA,numB,srv.response.sum, ros::this_node::getName().c_str(),gettid());
        }else{
            ROS_ERROR("call %s ERROR",client.getService().c_str());
        }
        ros::spinOnce();
        loop_rate.sleep();
    }
    return 0;
}