#include "ros/ros.h"
#include "std_msgs/Int64.h"
#include "std_msgs/String.h"
#include "geometry_msgs/Twist.h"
#include "hw_07/object_position.h"

ros::Publisher pub;
void callback(const hw_07::object_position::ConstPtr& pose)
{
    static bool hasCup = 0;
    static std_msgs::Int64 pose_width, pose_high, pose_x;
    const int sizeThreshold = 20;
    const double linearSpeed =  1;
    const double angularSpeed = 1;
    if(!hasCup){
        pose_width.data = pose->size_x;
        pose_high.data = pose->size_y;
        pose_x.data = pose->position_x;
        hasCup = true;
        return;
    }
    else{
        //width 变大，代表离摄像头近
        if((pose->size_x>pose_width.data+sizeThreshold)&&(pose->size_y>pose_high.data+sizeThreshold)){
            pose_width.data = pose->size_x;
            pose_high.data = pose->size_y;
            pose_x.data = pose->position_x;
            geometry_msgs::Twist msg;
            msg.linear.x = linearSpeed;
            msg.angular.z = 0;
            pub.publish(msg);
        }
        //width 变小，代表离摄像头近
        else if((pose->size_x<pose_width.data-sizeThreshold)&&(pose->size_y<pose_high.data-sizeThreshold)){
            pose_width.data = pose->size_x;
            pose_high.data = pose->size_y;
            pose_x.data = pose->position_x;
            geometry_msgs::Twist msg;
            msg.linear.x = -1.0 *linearSpeed;
            msg.angular.z = 0;
            pub.publish(msg);
        }
        // 边界左移代表左转
        else if( (pose->position_x + pose->size_x/2) < (pose_x.data + pose_width.data/2 - sizeThreshold) ){
            pose_width.data = pose->size_x;
            pose_high.data = pose->size_y;
            pose_x.data = pose->position_x;
            geometry_msgs::Twist msg;
            msg.linear.x = 0;
            msg.angular.z = angularSpeed;
            pub.publish(msg);
        }
         // 边界右移代表右转
        else if( (pose->position_x + pose->size_x/2) > (pose_x.data + pose_width.data/2 + sizeThreshold) ){
            pose_width.data = pose->size_x;
            pose_high.data = pose->size_y;
            pose_x.data = pose->position_x;
            geometry_msgs::Twist msg;
            msg.linear.x = 0;
            msg.angular.z = -1.0 * angularSpeed;
            pub.publish(msg);
        }
    }
}

int main(int argc, char** argv){
    ros::init(argc, argv, "node2");   
    ros::NodeHandle n;
    pub = n.advertise<geometry_msgs::Twist>("cmd_vel", 1);
    ros::Subscriber sub= n.subscribe("object_position", 1, &callback);
    ros::spin();

    return 0;
}