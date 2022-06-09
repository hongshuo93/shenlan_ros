#include <iostream>
#include <ros/ros.h>
#include <ros/service.h>
#include <ros/service_server.h>
#include <turtlesim/Spawn.h>

int main(int argc,char** argv){
    ros::init(argc,argv,"hw03_02_node");
    ros::NodeHandle node;
    ros::service::waitForService("/spawn");
    ros::ServiceClient client = node.serviceClient<turtlesim::Spawn>("/spawn");
    turtlesim::Spawn srv;
    srand(time(NULL));
    srv.request.x = ((float)rand()/INT_MAX)*10.0f;
    srv.request.y =((float)rand()/INT_MAX)*10.0f;
    srv.request.theta = (float)rand()/INT_MAX;
    if(client.call(srv)){
        ROS_INFO("spawn a new turtle name:%s",srv.response.name.c_str());
    }else{
        ROS_ERROR("call /spawn ERROR  %s",srv.response.name.c_str());
    }

    return 0;
}