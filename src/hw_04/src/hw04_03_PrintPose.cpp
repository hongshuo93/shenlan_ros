#include <iostream>
#include <ros/ros.h>
#include <tf/transform_broadcaster.h>

#include <tf2_geometry_msgs/tf2_geometry_msgs.h>
#include <tf2/LinearMath/Quaternion.h>
#include <tf/transform_listener.h>
int main(int argc,char **argv){
    ros::init(argc,argv,"hw04_03_print");
    ros::NodeHandle node;
    ros::Rate loop_rate(1);
    // tf2::TransformListener listener;
    tf2_ros::Buffer buf;
    tf2_ros::TransformListener listener(buf);
    while(ros::ok()){
        try{
            geometry_msgs::TransformStamped  trans;
            trans = buf.lookupTransform("base_link","base_laser",ros::Time(0),ros::Duration(99));
            geometry_msgs::PoseStamped targetPosInLaser;
            geometry_msgs::PoseStamped targetPosInBaseLink;
            targetPosInLaser.pose.position.x = 0.3;
            tf2::Quaternion q;
            q.setRPY(0,0,0);
            targetPosInLaser.pose.orientation.x = q.x();
            targetPosInLaser.pose.orientation.y = q.y();
            targetPosInLaser.pose.orientation.z = q.z();
            targetPosInLaser.pose.orientation.w = q.w();
            tf2::doTransform(targetPosInLaser,targetPosInBaseLink,trans);

            ROS_INFO("trans %f, %f %f", trans.transform.translation.x,trans.transform.translation.y,trans.transform.translation.z);
            ROS_INFO("link  %f, %f %f", targetPosInBaseLink.pose.position.x,
                                        targetPosInBaseLink.pose.position.y,
                                        targetPosInBaseLink.pose.position.z);
        }catch(ros::Exception excep){
            ROS_INFO("error %s",excep.what());
        }

        ros::spinOnce();
        loop_rate.sleep();
    }
    return 0;
}