#include <iostream>
#include <ros/ros.h>
#include <tf2_ros/transform_broadcaster.h>
#include <tf2_ros/static_transform_broadcaster.h>
#include <geometry_msgs/TransformStamped.h>
#include <tf2/LinearMath/Quaternion.h>
#include <tf/transform_broadcaster.h>
#include <tf/transform_listener.h>
int main(int argc,char **argv){
    ros::init(argc,argv,"hw04_03_br");
    ros::NodeHandle node;
    ros::Rate loop_rate(1);

    tf2_ros::StaticTransformBroadcaster br;
    geometry_msgs::TransformStamped  trans;


    



        trans.header.stamp = ros::Time(0);
        trans.header.frame_id = "base_link";
        trans.child_frame_id = "base_laser";
        trans.transform.translation.x = 0.1;
        trans.transform.translation.y = 0.0;
        trans.transform.translation.z = 0.2;
        tf2::Quaternion quat;
        quat.setRPY(0,0,0);
        trans.transform.rotation.x = quat.x();
        trans.transform.rotation.y = quat.y();
        trans.transform.rotation.z = quat.z();
        trans.transform.rotation.w = quat.w();
        br.sendTransform(trans);
        ROS_INFO("Spinning until killed publishing %s to %s", trans.header.frame_id.c_str(), trans.child_frame_id.c_str());
        ros::spin();
        
    
    return 0;
}