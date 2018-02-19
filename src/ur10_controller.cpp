#include "agile_robotics_industrial_automation/ur10_controller.hpp"

UR10Controller::UR10Controller() : robot_move_group_("manipulator") {
  // robot_move_group_("manipulator");
  robot_move_group_.setPlanningTime(10);
  robot_move_group_.setNumPlanningAttempts(10);
  robot_move_group_.setPlannerId("RRTConnectkConfigDefault");
  home_position_ = {0.0, 3.1, -1.1, 1.9, 3.9, 4.7, 0};
  offset_ = 0.02;

  robot_tf_listener_.waitForTransform("linear_arm_actuator", "ee_link",
                                      ros::Time(0), ros::Duration(10));
  robot_tf_listener_.lookupTransform("/linear_arm_actuator", "/ee_link",
                                     ros::Time(0), robot_tf_transform_);

  sendRobotHome();

  fixed_orientation_.x = robot_tf_transform_.getRotation().x();
  fixed_orientation_.y = robot_tf_transform_.getRotation().y();
  fixed_orientation_.z = robot_tf_transform_.getRotation().z();
  fixed_orientation_.w = robot_tf_transform_.getRotation().w();

  end_position_ = home_position_;
  end_position_[0] = -2.2;
  end_position_[1] = 4.5;
  // end_position_[2] = -1.2;

  agv_tf_listener_.waitForTransform("world", "agv2_load_point_frame",
                                    ros::Time(0), ros::Duration(10));
  agv_tf_listener_.lookupTransform("/world", "/agv2_load_point_frame",
                                   ros::Time(0), agv_tf_transform_);
  agv_position_.position.x = agv_tf_transform_.getOrigin().x();
  agv_position_.position.y = agv_tf_transform_.getOrigin().y();
  agv_position_.position.z = agv_tf_transform_.getOrigin().z() + offset_;

  gripper_client_ = ur10_nh_.serviceClient<osrf_gear::VacuumGripperControl>(
      "/ariac/gripper/control");
}

UR10Controller::~UR10Controller() {}

bool UR10Controller::planner() {
  ROS_INFO_STREAM("Planning started...");
  if (robot_move_group_.plan(robot_planner_) ==
      moveit::planning_interface::MoveItErrorCode::SUCCESS) {
    plan_success_ = true;
    ROS_INFO_STREAM("Planner succeeded!");
  } else {
    plan_success_ = false;
    ROS_WARN_STREAM("Planner failed!");
  }

  return plan_success_;
}

void UR10Controller::execute() {
  ros::AsyncSpinner spinner(2);
  spinner.start();
  if (this->planner()) {
    robot_move_group_.move();
    ros::Duration(3.0).sleep();
  }
}

void UR10Controller::setTarget(const geometry_msgs::Pose& pose) {
  target_pose_.orientation = fixed_orientation_;
  target_pose_.position = pose.position;
  robot_move_group_.setPoseTarget(target_pose_);
}

void UR10Controller::sendRobotHome() {
  ROS_INFO_STREAM("Going to Home position!");
  // ros::Duration(2.0).sleep();
  robot_move_group_.setJointValueTarget(home_position_);
  this->execute();
  ros::Duration(2.0).sleep();
}

void UR10Controller::gripperToggle(const bool& state) {
  gripper_service_.request.enable = state;
  if (gripper_client_.call(gripper_service_)) {
    ROS_INFO_STREAM("Gripper activated!");
  } else {
    ROS_WARN_STREAM("Gripper activation failed!");
  }
}

void UR10Controller::dropPart() {
  ROS_WARN_STREAM("Dropping the part...");

  ROS_INFO_STREAM("Moving to end of conveyor...");
  robot_move_group_.setJointValueTarget(end_position_);
  this->execute();
  ros::Duration(2.0).sleep();

  ROS_INFO_STREAM("Dropping on AGV...");
  this->setTarget(agv_position_);
  this->execute();
  ros::Duration(2.0).sleep();

  ROS_INFO_STREAM("Actuating the gripper...");
  this->gripperToggle(false);

  ROS_INFO_STREAM("Moving to end of conveyor...");
  robot_move_group_.setJointValueTarget(end_position_);
  this->execute();
  ros::Duration(2.0).sleep();

  ROS_INFO_STREAM("Going to home...");
  this->sendRobotHome();
}

void UR10Controller::pickPart(geometry_msgs::Pose& part_pose) {
  ROS_INFO_STREAM("Picking the part...");

  ROS_INFO_STREAM("Moving to part...");
  part_pose.position.z = part_pose.position.z + offset_;
  this->setTarget(part_pose);
  this->execute();
  ros::Duration(2.0).sleep();

  ROS_INFO_STREAM("Actuating the gripper...");
  this->gripperToggle(true);

  ROS_INFO_STREAM("Going to home...");
  this->sendRobotHome();
}