#include <iostream>
#include <string>
#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <turtlesim/Pose.h>
std::string turtle_name;
void posCallback(const turtlesim::PoseConstPtr& msg){
    static tf::TransformBroadcaster br;
    tf::Transform transform;
    transform.setOrigin(tf::Vector3(msg->x,msg->y,0.0));
    tf::Quaternion q;
    q.setRPY(0,0,msg->theta);
    transform.setRotation(q);
    br.sendTransform(tf::StampedTransform(transform,ros::Time::now(),"world",turtle_name));
}


int main(int argc,char **argv){
    ros::init(argc,argv,"turtle_tf_broadcaster");
    turtle_name = argv[1];
    ros::NodeHandle node;
    ros::Subscriber sub = node.subscribe(turtle_name+"/pose",10,&posCallback);
    ros::spin();
    return 0;
}