#include <ros/ros.h>
#include <tf/transform_listener.h>
#include <turtlesim/Spawn.h>
#include <geometry_msgs/Twist.h>
#include <string>
int main(int argc,char** argv){
    ros::init(argc,argv,"my_tf_listener");
    ros::NodeHandle n;
    std::string newTurtleName = "";
    {
        ros::service::waitForService("spawn");
        ros::ServiceClient add_turtle = n.serviceClient<turtlesim::Spawn>("spawn");
        turtlesim::Spawn srv;
        add_turtle.call(srv);
        newTurtleName = srv.response.name;
    }
    ROS_INFO("add new turtle name:%s",newTurtleName.c_str());
    ros::Publisher turtle_vel = n.advertise<geometry_msgs::Twist>("/"+newTurtleName+"/cmd_vel",32);
    tf::TransformListener listener;
    ros::Rate rate(10.0);
    while(ros::ok()){
        tf::StampedTransform trans;
        try{
            listener.waitForTransform("/turtle1","/"+newTurtleName,ros::Time(0),ros::Duration(3.0));
            listener.lookupTransform("/"+newTurtleName,"/turtle1",ros::Time(0),trans);

            geometry_msgs::Twist vel_msg;
            vel_msg.angular.z = 4.0*atan2(trans.getOrigin().y(),trans.getOrigin().x());
            vel_msg.linear.x = 0.5*sqrt(pow(trans.getOrigin().x(),2)+pow(trans.getOrigin().y(),2));
            turtle_vel.publish(vel_msg);
            ros::spinOnce();
            rate.sleep();
        }catch(std::exception exce){
            ROS_ERROR(" error in %s, msg: %s",ros::this_node::getName(),exce.what());
        }
      
    }
    return 0;
}