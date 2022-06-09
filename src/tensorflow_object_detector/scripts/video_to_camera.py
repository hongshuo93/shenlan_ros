#!/usr/bin/env python

import os
import sys
import cv2
import numpy as np
import rospy
from std_msgs.msg import String , Header
from sensor_msgs.msg import Image
from cv_bridge import CvBridge, CvBridgeError


def main(args):
    rospy.init_node('video_to_camera')
    video_path = rospy.get_param("video_path",'/home/sun/2022-06-10 00-33-31.mp4')
    print('video_path:',video_path)
    image_pub = rospy.Publisher("/usb_cam/image_raw",Image, queue_size=1)
    if not os.path.exists(video_path):
        rospy.loginfo("ERROR!!! open vidoe ERROR:"+video_path)
        return
    cap = cv2.VideoCapture(video_path)
    if not cap.isOpened():
        rospy.loginfo("ERROR!!! open vidoe ERROR:"+video_path)
        return
    bridge = CvBridge()
    fps = cap.get(cv2.CAP_PROP_FPS)
    rate = rospy.Rate(fps)
    while(not rospy.is_shutdown()):
        ret, cv_frame = cap.read() 
        if not ret:
            print('no video')
            cap.set(cv2.CAP_PROP_POS_FRAMES, 0)
            continue
        cv_frame = cv2.resize(cv_frame,(640,480))
        #cv2.imshow("Image", frame)
        wait_ms = 30
        if fps:
            wait_ms = 1000/fps
        #cv2.waitKey(int(wait_ms))

        ros_image = bridge.cv2_to_imgmsg(cv_frame, "bgr8")
        image_pub.publish(ros_image)
        rate.sleep()

    cap.release()

if __name__=='__main__':
    main(sys.argv)
