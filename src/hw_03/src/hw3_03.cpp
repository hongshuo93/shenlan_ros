#include <iostream>
#include <string>
#include <ros/ros.h>
#include <ros/service.h>
#include <hw_03/turtleSpawnCtrl.h>
#include <hw_03/turtleRun.h>
#include <hw_03/turtleStop.h>
#include <turtlesim/Spawn.h>
#include <geometry_msgs/Twist.h>
#include <unordered_map>
#include <unordered_set>

struct TurtleTwistPublisher{
    ros::Publisher pub;
    geometry_msgs::Twist twist;
};
ros::NodeHandle *nodePtr;
ros::ServiceClient *reqTurtleSpawnPtr;

//当前控制运动的海龟的哈希表，key  海龟名， value  海龟对应的publisher和geometry_msgs::Twist
std::unordered_map<std::string,TurtleTwistPublisher> ctrledTurtleMap;

//当前已添加的海龟哈希表 key 海龟名
std::unordered_set<std::string> addedTurtleSet;

//响应添加海龟，成功添加返回true，并将添加的海龟名加入海龟哈希表中
bool spawnResponse(hw_03::turtleSpawnCtrl::Request &req, hw_03::turtleSpawnCtrl::Response &res){
    if(req.reqTurtleName.length()<1 || addedTurtleSet.count(req.reqTurtleName)>0){
        return false;
    }
    turtlesim::Spawn srv;
    srv.request.name = req.reqTurtleName;
    srv.request.x = ((float)rand()/RAND_MAX)*10.0f;
    srv.request.y = ((float)rand()/RAND_MAX)*10.0f;
    srv.request.theta = ((float)rand()/RAND_MAX);
    if(reqTurtleSpawnPtr->call(srv)){
        res.resTurtleName = srv.response.name;
        addedTurtleSet.insert(res.resTurtleName);
        return true;
    }else{
        return false;
    }
}

//响应海龟开始运动，将速度数据保存在ctrledTurtleMap哈希表中
bool turtleRunResponse(hw_03::turtleRun::Request &req, hw_03::turtleRun::Response &res){
    if(req.turtleName.length()<1){
        res.result =-1;
        return false;
    }
    geometry_msgs::Twist twist;
    twist.linear.x = req.speed;
    if(std::abs(req.radius)>std::numeric_limits<float>::epsilon()){
        twist.angular.z = req.speed/req.radius;
    }
    if(ctrledTurtleMap.count(req.turtleName)<1){
        TurtleTwistPublisher turPub;
        turPub.pub =nodePtr->advertise<geometry_msgs::Twist>(req.turtleName+"/cmd_vel",32);
        turPub.twist = twist;
        ctrledTurtleMap.insert(std::make_pair(req.turtleName,turPub));
    }else{
        ctrledTurtleMap[req.turtleName].twist = twist;
    }   
    res.result = 0;
    return true;
}

//响应海龟停止运动，将海龟运动数据从ctrledTurtleMap哈希表中移除
bool turtleStopResponse(hw_03::turtleStop::Request &req, hw_03::turtleStop::Response &res){
    if(req.turtleName.length()<1){
        res.result =-2;
        return false;
    }
    auto findIt = ctrledTurtleMap.find(req.turtleName);
    if(findIt!=ctrledTurtleMap.end()){
        ctrledTurtleMap.erase(findIt);
        res.result = 0;
        return true;
    }else{
        res.result = -1;
        return false;
    }
}

int main(int argc,char** argv){
    srand(time(NULL));
    ros::init(argc,argv,"hw3_03_node");
    ros::NodeHandle node;
    nodePtr = &node;

    //等待turtlesim运行
    ros::service::waitForService("/spawn");
    ros::ServiceClient reqTurtleSpawn = nodePtr->serviceClient<turtlesim::Spawn>("/spawn");
    reqTurtleSpawnPtr = &reqTurtleSpawn;
    addedTurtleSet.insert("/turtle1");
    
    ros::ServiceServer spawnServer = node.advertiseService("/hw_03_spawn",spawnResponse);
    ros::ServiceServer turtleRunServer = node.advertiseService("/turtleRun",turtleRunResponse);
    ros::ServiceServer turtleStopServer = node.advertiseService("/turtleStop",turtleStopResponse);

    ros::Rate rate(1);
    while(ros::ok()){
        //遍历ctrledTurtleMap哈希表，表中保存所有控制海龟运动的数据
        for(auto it = ctrledTurtleMap.cbegin();it!=ctrledTurtleMap.cend();it++){
            it->second.pub.publish(it->second.twist);
        }
        ros::spinOnce();
        rate.sleep();
    }
    return 0;
}