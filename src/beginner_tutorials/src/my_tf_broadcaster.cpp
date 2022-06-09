#include <iostream>
#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <string>
#include <turtlesim/Pose.h>
std::string turtleName;
void subPoseCallback(const turtlesim::PoseConstPtr& msg){
    static tf::TransformBroadcaster caster;
    tf::Transform trans;
    trans.setOrigin(tf::Vector3(msg->x,msg->y,0.0));
    tf::Quaternion q;
    q.setRPY(0,0,msg->theta);
    trans.setRotation(q);
    caster.sendTransform(tf::StampedTransform(trans,ros::Time::now(),"world",turtleName));

}


int main(int argc,char** argv){
    ros::init(argc,argv,"my_tf_broadcaster");
    ros::NodeHandle node;
    turtleName = argv[1];
    ros::Subscriber sub = node.subscribe(turtleName+"/pose",10,subPoseCallback);
    ros::spin();
    return 0;
}