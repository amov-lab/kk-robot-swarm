/*
 * @Author: Chaozheng Zhu && Loren
 * @Date: 2022-05-07 09:33:11
 * @LastEditTime: 2022-06-26 21:01:51
 * @FilePath:
 * /kk-robot-swarm/src/global_vision/include/global_vision/odomPub.hpp
 * @Description:
 *
 * Copyright (c) 2022 by AmovLab, All Rights Reserved.
 */
#pragma once

#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/TransformStamped.h>
#include <nav_msgs/Odometry.h>
#include <ros/ros.h>
#include <std_msgs/String.h>
#include <tf/transform_broadcaster.h>
#include <tf/transform_listener.h>

#include <iostream>
#include <map>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

#include "kkswarm_msgs/kkSwarmPose.h"
#include "kkswarm_msgs/kkSwarmState.h"

class odomPub {
 public:
  ros::NodeHandle nh_;
  std::vector<ros::Publisher> odom_pub_set_;
  ros::Subscriber communication_sub_;

  // car_odom_[1] means No.1 car's odom data.
  nav_msgs::Odometry car_odom_[11];

  // car_pose_[1] means No.1 car's pose data.
  kkswarm_msgs::kkSwarmPose car_pose_[11];

  tf::TransformListener car_tf_listener_;
  tf::StampedTransform transform_;
  tf::Quaternion quat_;

  int car_total_num_ = 6;
  odomPub();
  ~odomPub() = default;

  /**
   * \brief: Subscribe TF data form camera.
   * \param: {*}
   * \return: {*}
   */
  void subFromTf();

  /**
   * \brief: Subscribe /comm topic from tcpDriverNode.cpp
   * \param: {*}
   * \return: {*}
   */
  void subFromTcp();

  /**
   * \brief: This callback save the car's id and linear speed and angular speed.
   * \param: {const kkswarm_msgs::kkSwarmState::ConstPtr: Custom msgs.
   *          See /kk-robot-swarm/src/kkswarm_msgs/msg/kkSwarmState.msg
   *          for more information.}
   * \return: {*}
   */
  void communicationCallBack(const kkswarm_msgs::kkSwarmState::ConstPtr& msgs);

  /**
   * \brief: Creates /robot_x/base_link tf frame. x is car's id.
   * \param: {int mark: Car's id.}
   * \return: {std::string: /robot_x/base_link tf frame}
   */
  std::string tfNameGenerate(int mark) {
    std::string front_str = "/robot_";
    std::stringstream ss_base;
    std::string end_str_base = "/base_link";
    ss_base << front_str << mark << end_str_base;
    std::string tf_base = ss_base.str();
    return tf_base;
  }

  /**
   * \brief: Creates /robot_x/base_link tf frame. x is car's id.
   * \param: {int mark: Car's id.}
   * \return: {std::string: /robot_x/base_link tf frame}
   */
  std::string tagNameGenerate(int mark) {
    std::string front_str = "/tag_";
    std::stringstream ss_base;
    ss_base << front_str << mark;
    std::string tf_base = ss_base.str();
    return tf_base;
  }

  /**
   * \brief: Creates /robot_x/pose topic. x is car's id.
   * \param: {int mark: Car's id.}
   * \return: {std::string: /robot_x/pose topic name.}
   */
  std::string topicNameGenerate(int mark) {
    std::string front_str = "/robot_";
    std::string end_str = "/pose";
    std::stringstream ssss;
    ssss << front_str << mark << end_str;
    std::string topic = ssss.str();
    return topic;
  }

  /**
   * \brief: Convert rad to degree.
   * \param: {double yaw: car's yaw in degree.}
   * \return: {double, car's yaw in rad.}
   */
  double convertDegree(double yaw) {
    if (yaw < 0) yaw = yaw + 2 * 3.1415926;
    return 180 * yaw / 3.1415926;
  }
};
