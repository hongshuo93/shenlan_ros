#include "ros/ros.h"
#include "hw_07/face_position.h"
#include "geometry_msgs/Twist.h"
#include "std_msgs/Int32.h"

ros::Publisher pub;
void callback(const hw_07::face_position::ConstPtr& size)
{
  static bool hasFacePose = false;
  static std_msgs::Int32 size_width, size_high, size_x;
  const int sizeThreshold = 20;
  const double linearSpeed =  0.2;
  const double angularSpeed = 1;
  if(!hasFacePose){
      size_width.data = size->width;
      size_high.data = size->high;
      size_x.data = size->x;
      hasFacePose = true;
      return;
  }
  else{
      if((size->width>size_width.data+sizeThreshold)&&(size->high>size_high.data+sizeThreshold/2)){
        size_width.data = size->width;
        size_high.data = size->high;
        size_x.data = size->x;
        geometry_msgs::Twist msg;
        msg.linear.x = linearSpeed;
        msg.angular.z = 0;
        pub.publish(msg);
      }
      else if((size->width<size_width.data-sizeThreshold)&&(size->high<size_high.data-sizeThreshold)){

        size_width.data = size->width;
        size_high.data = size->high;
        size_x.data = size->x;
        geometry_msgs::Twist msg;
        msg.linear.x = -1.0*linearSpeed;
        msg.angular.z = 0;
        pub.publish(msg);
      }
      else if( (size->x + size->width/2) < (size_x.data + size_width.data/2 - sizeThreshold) ){
        size_width.data = size->width;
        size_high.data = size->high;
        size_x.data = size->x;
        geometry_msgs::Twist msg;
        msg.linear.x = 0;
        msg.angular.z = -1.0*angularSpeed;
        pub.publish(msg);
      }
      else if( (size->x + size->width/2) > (size_x.data + size_width.data/2 + sizeThreshold) ){
  
        size_width.data = size->width;
        size_high.data = size->high;
        size_x.data = size->x;
        geometry_msgs::Twist msg;
        msg.linear.x = 0;
        msg.angular.z = angularSpeed;
        pub.publish(msg);
      }
  }
}
int main(int argc, char **argv)
{
  ros::init(argc, argv, "node1");
  ros::NodeHandle n;
  pub  = n.advertise<geometry_msgs::Twist>("cmd_vel", 1);
  ros::Subscriber sub = n.subscribe("face_position", 1, &callback);
  ros::spin();
  return 0;
}