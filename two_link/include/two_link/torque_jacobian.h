#ifndef TORQUE_JACOBIAN_H_
#define TORQUE_JACOBIAN_H_

#include <ros/ros.h>
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Dense>
#include <iostream>
#include <sensor_msgs/JointState.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/String.h>
#include <dynamixel_workbench_msgs/DasomDynamixel.h>
#include <dynamixel_workbench_msgs/EECommand.h>
#include "two_link/movingFlag.h"
#include "two_link/admittanceTest.h"
#include <kdl/chain.hpp>
#include "geometry_msgs/PoseStamped.h"

#include "tf/transform_datatypes.h"
#include <tf2/LinearMath/Quaternion.h>
#include <tf2_msgs/TFMessage.h>
#include <geometry_msgs/TransformStamped.h>
#include <geometry_msgs/Transform.h>
#include <tf2_ros/transform_listener.h>
#include <tf2_ros/static_transform_broadcaster.h>

#define PI 3.14159256359

class TorqJ
{
 public:
  TorqJ();
  ~TorqJ();

  double X = 0.23209;
  double Y = 0;

  //----------Link Lengths---------//
	double Link1 = 0.10375;
	double Link2 = 0.13634;
  double Link3 = 0.104;
  //----------Link Lengths---------//
	double CoM1 = 0.07821;
	double CoM2 = 0.1117;
	double delta = 0.261;
	double mass1 = 0.10772;
	double mass2 = 0.29951;
  double Kt_1 = 1;
  double Kt_2 = 1;


  //--Offset for gravity matrix--//
  double offset_1 = 2.62;
  double offset_2 = 0.;
  double offset_3 = 0.0;


  // Eigen::Vector3d Wrist_Position;
  Eigen::Vector3d Orientation_ref;
  Eigen::VectorXd X_test;
  Eigen::Vector3d X_ref;
  Eigen::VectorXd X_cmd;
  Eigen::Vector3d X_Command;
  Eigen::Vector3d V_measured;
  Eigen::VectorXd angle_measured;
  Eigen::Vector3d HapticRPY;

  Eigen::MatrixXd J;
  Eigen::MatrixXd JT;
  Eigen::MatrixXd JTI;


  Eigen::VectorXd angle_ref;
  Eigen::VectorXd tau_gravity;
  Eigen::Vector3d FK_EE_pos;
  Eigen::Vector3d FK_EE_ori;
  Eigen::Vector2d POSITION_2;


  //--DoB--//

  Eigen::VectorXd angle_d;
  Eigen::Vector2d angle_d_hat;
  Eigen::Vector2d angle_d_lpf;
  Eigen::VectorXd d_hat;
  Eigen::VectorXd angle_d_safe;
  Eigen::VectorXd angle_command;

  double position_p_gain;
  double position_i_gain;
  double position_d_gain;

  double polar_moment_1;
  double polar_moment_2;
  
  double Cut_Off_Freq;
  double Cut_Off_Freq2;
  Eigen::Vector4d Q_M;
  Eigen::Vector4d Q_M_dot;
  Eigen::Vector4d Q_M_2;
  Eigen::Vector4d Q_M_dot_2;  
  Eigen::Matrix4d Q_M_A;
  Eigen::Vector4d Q_M_B;
  Eigen::Vector4d Q_M_C;

  Eigen::Vector2d Q_angle_d;
  Eigen::Vector2d Q_angle_d_dot;
  Eigen::Vector2d Q_angle_d_2;
  Eigen::Vector2d Q_angle_d_dot_2;
  Eigen::Matrix2d Q_angle_d_A;
  Eigen::Vector2d Q_angle_d_B;
  Eigen::Vector2d Q_angle_d_C;
  //--end--//


//--External_Force_Estimation--//
  Eigen::VectorXd tau_measured;
  Eigen::VectorXd hysteresis_max;
  Eigen::VectorXd hysteresis_min;
  Eigen::VectorXd tau_ext;
  Eigen::VectorXd tau_ext_not_deadzone;  
  Eigen::Vector3d Force_ext;
  Eigen::Vector3d Force_ext_not_deadzone;
  Eigen::Vector3d Force_max;
  Eigen::Vector3d Force_min;

//--Admittance Control--//
  Eigen::Matrix2d A_x;
  Eigen::Vector2d B_x;
  Eigen::Vector2d C_x;
  Eigen::Vector2d D_x;

  Eigen::Matrix2d A_y;
  Eigen::Vector2d B_y;
  Eigen::Vector2d C_y;
  Eigen::Vector2d D_y;

  Eigen::Matrix2d A_z;
  Eigen::Vector2d B_z;
  Eigen::Vector2d C_z;
  Eigen::Vector2d D_z;  

  Eigen::Vector3d virtual_mass;
  Eigen::Vector3d virtual_damper;
  Eigen::Vector3d virtual_spring;

  Eigen::Vector2d X_from_model_matrix;
  Eigen::Vector2d X_dot_from_model_matrix;
  Eigen::Vector2d Y_from_model_matrix;
  Eigen::Vector2d Y_dot_from_model_matrix;
  Eigen::Vector2d Z_from_model_matrix;
  Eigen::Vector2d Z_dot_from_model_matrix;
  Eigen::Vector3d position_from_model;


  double virtual_mass_x;
  double virtual_damper_x;
  double virtual_spring_x;


  double virtual_mass_y;
  double virtual_damper_y;
  double virtual_spring_y;

  double virtual_mass_z;
  double virtual_damper_z;
  double virtual_spring_z;


  //--For safe--//
  Eigen::VectorXd angle_safe;
  Eigen::VectorXd angle_max;
  Eigen::VectorXd angle_min;


  //--for butterworth--//
  Eigen::Vector3d bw2_filtered_current_1_input;
  Eigen::Vector3d bw2_filtered_current_1_output;
  Eigen::Vector3d bw2_filtered_current_2_input;
  Eigen::Vector3d bw2_filtered_current_2_output;
  Eigen::Vector3d bw2_filtered_current_3_input;
  Eigen::Vector3d bw2_filtered_current_3_output;

  double wc;
  double wc2;
  double wc4;
  double wc3;
  double wc_4th;
  double wc2_4th;

  double b0_2nd;
  double b1_2nd;
  double b2_2nd;
  double a0_2nd;
  double a1_2nd;
  double a2_2nd;

  // Init position 맞추는 용도
  bool initPoseFlag;
  double initPoseCnt;
  bool checkFirstPoseFlag;
  Eigen::VectorXd initPose;
  Eigen::VectorXd firstPose;
  Eigen::VectorXd PoseDelta;

  // Current lpf result
  Eigen::VectorXd filtered_current;
  double cut_off_freq_current;

  //--Service Flag--//
  bool movingFlag = false;

  void calc_des();
  void PublishCmdNMeasured();
  void DoB();
  void Calc_Ext_Force();
  void Admittance_control();
  void angle_safe_func();
  void second_order_butterworth();
  void CommandGenerator();
  void solveInverseKinematics();
  void setInitpose();
  bool movingServiceCallback(two_link::movingFlag::Request  &req,
                             two_link::movingFlag::Response &res);
  bool AdmittanceCallback(two_link::admittanceTest::Request  &req,
                          two_link::admittanceTest::Response &res);

 private:
  /*****************************************************************************
  ** ROS NodeHandle
  *****************************************************************************/
  ros::NodeHandle node_handle_;
  ros::NodeHandle priv_node_handle_;

  /*****************************************************************************
  ** ROS Parameters
  *****************************************************************************/
  std::string robot_name_;
  sensor_msgs::JointState command_position;
  /*****************************************************************************
  ** Init Functions
  *****************************************************************************/
  void initPublisher();
  void initSubscriber();

  /*****************************************************************************
  ** ROS Subscribers, Callback Functions and Relevant Functions
  *****************************************************************************/
  ros::Publisher joint_command_pub_;
  ros::Publisher joint_measured_pub_;
  ros::Subscriber joint_states_sub_;
  ros::Subscriber joystick_sub_;
  ros::ServiceServer movingService;
  ros::ServiceServer admitService;


  //----------Link Lengths---------//
  static double l1;
  static double l2;
  static double l3;
  static double l4;
  static double l5;
  static double l6;
  static double l7;


  ////////////////////////////////////////////
  //////////////--- T Matrix ---//////////////
  ////////////////////////////////////////////
  static Eigen::Matrix4d L1(double theta_1)
  {
    double cosT = cos(theta_1), sinT = sin(theta_1);

    Eigen::Matrix4d L;
    L <<      cosT,     -sinT,     0,       0,
              sinT,      cosT,     0,       0,
                 0,         0,     1,      l1,
                 0,         0,     0,       1;
    return L;
  };

  static Eigen::Matrix4d L2(double theta_2)
  {
    double cosT = cos(theta_2), sinT = sin(theta_2);

    Eigen::Matrix4d L;
    L <<         1,         0,     0,       0,
                 0,      cosT, -sinT, l2*cosT,
                 0,      sinT,  cosT, l2*sinT,
                 0,         0,     0,       1;
    return L;
  };

  static Eigen::Matrix4d L3(double theta_3)
  {
    double cosT = cos(theta_3), sinT = sin(theta_3);

    Eigen::Matrix4d L;
    L <<         1,         0,     0,       0,
                 0,      cosT, -sinT, l3*cosT,
                 0,      sinT,  cosT, l3*sinT,
                 0,         0,     0,       1;
    return L;
  };

  static Eigen::Matrix4d L4(double theta_4)
  {
    double cosT = cos(theta_4), sinT = sin(theta_4);

    Eigen::Matrix4d L;
    L <<      cosT,         0,  sinT, l4*sinT,
                 0,         1,     0,       0,
             -sinT,         0,  cosT, l4*cosT,
                 0,         0,     0,       1;
    return L;
  };

  static Eigen::Matrix4d L5()
  {
    Eigen::Matrix4d L;
    L <<         1,         0,     0,       0,
                 0,         1,     0,      l5,
                 0,         0,     1,       0,
                 0,         0,     0,       1;
    return L;
  };

  static Eigen::Matrix4d L6(double theta_5)
  {
    double cosT = cos(theta_5), sinT = sin(theta_5);

    Eigen::Matrix4d L;
    L <<      cosT,     -sinT,     0,       0,
              sinT,      cosT,     0,       0,
                 0,         0,     1,     -l6,
                 0,         0,     0,       1;
    return L;
  };

  static Eigen::Matrix4d L7(double theta_6)
  {
    double cosT = cos(theta_6), sinT = sin(theta_6);

    Eigen::Matrix4d L;
    L <<         1,         0,     0,       0,
                 0,      cosT, -sinT, l7*cosT,
                 0,      sinT,  cosT, l7*sinT,
                 0,         0,     0,       1;
    return L;
  };

  static Eigen::Matrix3d R03(double theta_1, double theta_2, double theta_3)
  {
    Eigen::Matrix4d L;
    L << L1(theta_1)*L2(theta_2)*L3(theta_3);

    Eigen::Matrix3d R03;
    R03 << L(0,0), L(0,1), L(0,2),
           L(1,0), L(1,1), L(1,2),
           L(2,0), L(2,1), L(2,2);

    return R03;
  }

  //////////////////////////////////////////////////////
  //////////////--- Forward Kinematics ---//////////////
  //////////////////////////////////////////////////////

  static Eigen::MatrixXd EE_pos(double theta_1,double theta_2,double theta_3,
			                          double theta_4,double theta_5,double theta_6)
  {
    double cos1 = cos(theta_1), sin1 = sin(theta_1);
    double cos2 = cos(theta_2), sin2 = sin(theta_2);
    double cos3 = cos(theta_3), sin3 = sin(theta_3);
    double cos4 = cos(theta_4), sin4 = sin(theta_4);
    double cos5 = cos(theta_5), sin5 = sin(theta_5);
    double cos6 = cos(theta_6), sin6 = sin(theta_6);
    double sin23 = sin(theta_2 + theta_3);
    double cos23 = cos(theta_2 + theta_3);
    double cos45 = cos(theta_4 + theta_5);
    double sin46 = sin(theta_4 + theta_6);
    double cos4m5 = cos(theta_4 - theta_5);
    double sin4m6 = sin(theta_4 - theta_6);

    Eigen::MatrixXd EE_pos(3,1);

    EE_pos << 
    // X
    l5*(sin1*sin2*sin3 - cos2*cos3*sin1) - l6*(cos1*sin4 + cos4*(cos2*sin1*sin3 + cos3*sin1*sin2)) + l4*cos4*(cos2*sin1*sin3 + cos3*sin1*sin2) - l7*cos6*(sin5*(cos1*cos4 - sin4*(cos2*sin1*sin3 + cos3*sin1*sin2)) - cos5*(sin1*sin2*sin3 - cos2*cos3*sin1)) - l2*cos2*sin1 + l4*cos1*sin4 + l7*sin6*(cos1*sin4 + cos4*(cos2*sin1*sin3 + cos3*sin1*sin2)) - l3*cos2*cos3*sin1 + l3*sin1*sin2*sin3,
    // Y
    l2*cos1*cos2 - l6*(sin1*sin4 - cos4*(cos1*cos2*sin3 + cos1*cos3*sin2)) - l5*(cos1*sin2*sin3 - cos1*cos2*cos3) + l4*sin1*sin4 + l7*sin6*(sin1*sin4 - cos4*(cos1*cos2*sin3 + cos1*cos3*sin2)) - l7*cos6*(sin5*(cos4*sin1 + sin4*(cos1*cos2*sin3 + cos1*cos3*sin2)) + cos5*(cos1*sin2*sin3 - cos1*cos2*cos3)) - l4*cos4*(cos1*cos2*sin3 + cos1*cos3*sin2) + l3*cos1*cos2*cos3 - l3*cos1*sin2*sin3,
    // Z
    l1 + l3*sin23 + l5*sin23 + l2*sin2 + (l7*cos23*sin46)/2 + l4*cos23*cos4 - l6*cos23*cos4 - (l7*sin4m6*cos23)/2 + l7*cos6*((cos4m5*cos23)/2 - (cos23*cos45)/2 + sin23*cos5);

    return EE_pos;
  }


  static Eigen::MatrixXd EE_orientation(double theta_1,double theta_2,double theta_3,
			                                  double theta_4,double theta_5,double theta_6)
  {
    double r11 = cos(theta_5)*(cos(theta_1)*cos(theta_4) - sin(theta_4)*(cos(theta_2)*sin(theta_1)*sin(theta_3) + cos(theta_3)*sin(theta_1)*sin(theta_2))) + sin(theta_5)*(sin(theta_1)*sin(theta_2)*sin(theta_3) - cos(theta_2)*cos(theta_3)*sin(theta_1));
    double r12 = sin(theta_6)*(cos(theta_1)*sin(theta_4) + cos(theta_4)*(cos(theta_2)*sin(theta_1)*sin(theta_3) + cos(theta_3)*sin(theta_1)*sin(theta_2))) - cos(theta_6)*(sin(theta_5)*(cos(theta_1)*cos(theta_4) - sin(theta_4)*(cos(theta_2)*sin(theta_1)*sin(theta_3) + cos(theta_3)*sin(theta_1)*sin(theta_2))) - cos(theta_5)*(sin(theta_1)*sin(theta_2)*sin(theta_3) - cos(theta_2)*cos(theta_3)*sin(theta_1)));
    double r13 = cos(theta_6)*(cos(theta_1)*sin(theta_4) + cos(theta_4)*(cos(theta_2)*sin(theta_1)*sin(theta_3) + cos(theta_3)*sin(theta_1)*sin(theta_2))) + sin(theta_6)*(sin(theta_5)*(cos(theta_1)*cos(theta_4) - sin(theta_4)*(cos(theta_2)*sin(theta_1)*sin(theta_3) + cos(theta_3)*sin(theta_1)*sin(theta_2))) - cos(theta_5)*(sin(theta_1)*sin(theta_2)*sin(theta_3) - cos(theta_2)*cos(theta_3)*sin(theta_1)));
    double r21 = cos(theta_5)*(cos(theta_4)*sin(theta_1) + sin(theta_4)*(cos(theta_1)*cos(theta_2)*sin(theta_3) + cos(theta_1)*cos(theta_3)*sin(theta_2))) - sin(theta_5)*(cos(theta_1)*sin(theta_2)*sin(theta_3) - cos(theta_1)*cos(theta_2)*cos(theta_3));
    double r22 = sin(theta_6)*(sin(theta_1)*sin(theta_4) - cos(theta_4)*(cos(theta_1)*cos(theta_2)*sin(theta_3) + cos(theta_1)*cos(theta_3)*sin(theta_2))) - cos(theta_6)*(sin(theta_5)*(cos(theta_4)*sin(theta_1) + sin(theta_4)*(cos(theta_1)*cos(theta_2)*sin(theta_3) + cos(theta_1)*cos(theta_3)*sin(theta_2))) + cos(theta_5)*(cos(theta_1)*sin(theta_2)*sin(theta_3) - cos(theta_1)*cos(theta_2)*cos(theta_3)));
    double r23 = cos(theta_6)*(sin(theta_1)*sin(theta_4) - cos(theta_4)*(cos(theta_1)*cos(theta_2)*sin(theta_3) + cos(theta_1)*cos(theta_3)*sin(theta_2))) + sin(theta_6)*(sin(theta_5)*(cos(theta_4)*sin(theta_1) + sin(theta_4)*(cos(theta_1)*cos(theta_2)*sin(theta_3) + cos(theta_1)*cos(theta_3)*sin(theta_2))) + cos(theta_5)*(cos(theta_1)*sin(theta_2)*sin(theta_3) - cos(theta_1)*cos(theta_2)*cos(theta_3)));
    double r31 = sin(theta_2 + theta_3)*sin(theta_5) - cos(theta_2 + theta_3)*cos(theta_5)*sin(theta_4);
    double r32 = cos(theta_6)*(sin(theta_2 + theta_3)*cos(theta_5) + cos(theta_2 + theta_3)*sin(theta_4)*sin(theta_5)) + cos(theta_2 + theta_3)*cos(theta_4)*sin(theta_6);
    double r33 = cos(theta_2 + theta_3)*cos(theta_4)*cos(theta_6) - sin(theta_6)*(sin(theta_2 + theta_3)*cos(theta_5) + cos(theta_2 + theta_3)*sin(theta_4)*sin(theta_5));

    double alpha = atan2(r21,r11);

    Eigen::MatrixXd EE_Orientation(3,1);
    
    EE_Orientation << 
    // roll
    atan2(sin(alpha)*r13-cos(alpha)*r23,-sin(alpha)*r12+cos(alpha)*r22),
    // pitch
    atan2(-r31,cos(alpha)*r11+sin(alpha)*r21),				// limit pitch direction
    // yaw
    alpha;

    return EE_Orientation;
  }

  static Eigen::MatrixXd Jacobian(double theta_1,double theta_2,double theta_3,
                                  double theta_4,double theta_5,double theta_6)
  {
    double cos1 = cos(theta_1), sin1 = sin(theta_1);
    double cos2 = cos(theta_2), sin2 = sin(theta_2);
    double cos3 = cos(theta_3), sin3 = sin(theta_3);
    double cos4 = cos(theta_4), sin4 = sin(theta_4);
    double cos5 = cos(theta_5), sin5 = sin(theta_5);
    double cos6 = cos(theta_6), sin6 = sin(theta_6);
    double sin23 = sin(theta_2 + theta_3);
    double cos23 = cos(theta_2 + theta_3);
    double cos45 = cos(theta_4 + theta_5);
    double sin46 = sin(theta_4 + theta_6);
    double cos4m5 = cos(theta_4 - theta_5);
    double sin4m6 = sin(theta_4 - theta_6);

    Eigen::MatrixXd J(6,6);

    J <<
    // 1X1
    l5*(cos1*sin2*sin3 - cos1*cos2*cos3) + l6*(sin1*sin4 - cos4*(cos1*cos2*sin3 + cos1*cos3*sin2)) - l2*cos1*cos2 - l4*sin1*sin4 - l7*sin6*(sin1*sin4 - cos4*(cos1*cos2*sin3 + cos1*cos3*sin2)) + l7*cos6*(sin5*(cos4*sin1 + sin4*(cos1*cos2*sin3 + cos1*cos3*sin2)) + cos5*(cos1*sin2*sin3 - cos1*cos2*cos3)) + l4*cos4*(cos1*cos2*sin3 + cos1*cos3*sin2) - l3*cos1*cos2*cos3 + l3*cos1*sin2*sin3,
    // 1X2
    sin1*(l5*sin23 + l2*sin2 + l4*cos23*cos4 - l6*cos23*cos4 + l3*cos2*sin3 + l3*cos3*sin2 + l7*cos23*cos4*sin6 + l7*sin23*cos5*cos6 + l7*cos23*cos6*sin4*sin5),
    // 1X3
    sin1*(l5*sin23 + l4*cos23*cos4 - l6*cos23*cos4 + l3*cos2*sin3 + l3*cos3*sin2 + l7*cos23*cos4*sin6 + l7*sin23*cos5*cos6 + l7*cos23*cos6*sin4*sin5),
    // 1X4
    l4*cos1*cos4 - l4*sin4*(cos2*sin1*sin3 + cos3*sin1*sin2) - l6*(cos1*cos4 - sin4*(cos2*sin1*sin3 + cos3*sin1*sin2)) + l7*sin6*(cos1*cos4 - sin4*(cos2*sin1*sin3 + cos3*sin1*sin2)) + l7*cos6*sin5*(cos1*sin4 + cos4*(cos2*sin1*sin3 + cos3*sin1*sin2)),
    // 1X5
    -l7*cos6*(cos5*(cos1*cos4 - sin4*(cos2*sin1*sin3 + cos3*sin1*sin2)) + sin5*(sin1*sin2*sin3 - cos2*cos3*sin1)),
    // 1X6
    l7*sin6*(sin5*(cos1*cos4 - sin4*(cos2*sin1*sin3 + cos3*sin1*sin2)) - cos5*(sin1*sin2*sin3 - cos2*cos3*sin1)) + l7*cos6*(cos1*sin4 + cos4*(cos2*sin1*sin3 + cos3*sin1*sin2)),
    // 2X1
    l5*(sin1*sin2*sin3 - cos2*cos3*sin1) - l6*(cos1*sin4 + cos4*(cos2*sin1*sin3 + cos3*sin1*sin2)) + l4*cos4*(cos2*sin1*sin3 + cos3*sin1*sin2) - l7*cos6*(sin5*(cos1*cos4 - sin4*(cos2*sin1*sin3 + cos3*sin1*sin2)) - cos5*(sin1*sin2*sin3 - cos2*cos3*sin1)) - l2*cos2*sin1 + l4*cos1*sin4 + l7*sin6*(cos1*sin4 + cos4*(cos2*sin1*sin3 + cos3*sin1*sin2)) - l3*cos2*cos3*sin1 + l3*sin1*sin2*sin3,
    // 2X2
    -cos1*(l5*sin23 + l2*sin2 + l4*cos23*cos4 - l6*cos23*cos4 + l3*cos2*sin3 + l3*cos3*sin2 + l7*cos23*cos4*sin6 + l7*sin23*cos5*cos6 + l7*cos23*cos6*sin4*sin5),
    // 2X3
    -cos1*(l5*sin23 + l4*cos23*cos4 - l6*cos23*cos4 + l3*cos2*sin3 + l3*cos3*sin2 + l7*cos23*cos4*sin6 + l7*sin23*cos5*cos6 + l7*cos23*cos6*sin4*sin5),
    // 2X4
    l4*sin4*(cos1*cos2*sin3 + cos1*cos3*sin2) - l6*(cos4*sin1 + sin4*(cos1*cos2*sin3 + cos1*cos3*sin2)) + l4*cos4*sin1 + l7*sin6*(cos4*sin1 + sin4*(cos1*cos2*sin3 + cos1*cos3*sin2)) + l7*cos6*sin5*(sin1*sin4 - cos4*(cos1*cos2*sin3 + cos1*cos3*sin2)),
    // 2X5
    -l7*cos6*(cos5*(cos4*sin1 + sin4*(cos1*cos2*sin3 + cos1*cos3*sin2)) - sin5*(cos1*sin2*sin3 - cos1*cos2*cos3)),
    // 2X6
    l7*cos6*(sin1*sin4 - cos4*(cos1*cos2*sin3 + cos1*cos3*sin2)) + l7*sin6*(sin5*(cos4*sin1 + sin4*(cos1*cos2*sin3 + cos1*cos3*sin2)) + cos5*(cos1*sin2*sin3 - cos1*cos2*cos3)),
    // 3X1
    0,
    // 3X2
    l3*cos23 + l5*cos23 + l2*cos2 - (l7*sin23*sin46)/2 - l4*sin23*cos4 + l6*sin23*cos4 + (l7*sin4m6*sin23)/2 + l7*cos6*((cos45*sin23)/2 - (cos4m5*sin23)/2 + cos23*cos5),
    // 3X3
    l3*cos23 + l5*cos23 - l4*sin23*cos4 + l6*sin23*cos4 + l7*cos23*cos5*cos6 - l7*sin23*cos4*sin6 - l7*sin23*cos6*sin4*sin5,
    // 3X4
    -cos23*(l4*sin4 - l6*sin4 + l7*sin4*sin6 -l7*cos4*cos6*sin5),
    // 3X5
    -l7*cos6*(sin23*sin5 -cos23*cos5*sin4),
    // 3X6
    l7*cos23*cos4*cos6 - l7*sin23*cos5*sin6 - l7*cos23*sin4*sin5*sin6,
    // 4X1
    0,
    // 4X2
    cos1,
    // 4X3
    cos1,
    // 4X4
    -cos23*sin1,
    // 4X5
    cos1*sin4 + cos4*(cos2*sin1*sin3 + cos3*sin1*sin2),
    // 4X6
    cos5*(cos1*cos4 - sin4*(cos2*sin1*sin3 + cos3*sin1*sin2)) + sin5*(sin1*sin2*sin3 - cos2*cos3*sin1),
    // 5X1
    0,
    // 5X2
    sin1,
    // 5X3
    sin1,
    // 5X4
    cos23*cos1,
    // 5X5
    sin1*sin4 - cos4*(cos1*cos2*sin3 + cos1*cos3*sin2),
    // 5X6
    cos5*(cos4*sin1 + sin4*(cos1*cos2*sin3 + cos1*cos3*sin2)) - sin5*(cos1*sin2*sin3 - cos1*cos2*cos3),
    // 6X1
    1,
    // 6X2
    0,
    // 6X3
    0,
    // 6X4
    sin23,
    // 6X5
    cos23*cos4,
    // 6X6
    sin23*sin5 - cos23*cos5*sin4;

    return J;
  }

  static Eigen::Matrix3d CmdOrientation(double roll, double pitch, double yaw)
  {
    Eigen::Matrix3d R;
    R <<
    // 1X1
    cos(pitch)*cos(yaw),
    // 1X2
    cos(yaw)*sin(pitch)*sin(roll) - cos(roll)*sin(yaw),
    // 1X3
    sin(roll)*sin(yaw) + cos(roll)*cos(yaw)*sin(pitch),
    // 2X1
    cos(pitch)*sin(yaw),
    // 2X2
    cos(roll)*cos(yaw) + sin(pitch)*sin(roll)*sin(yaw),
    // 2X3
    cos(roll)*sin(pitch)*sin(yaw) - cos(yaw)*sin(roll),
    // 3X1
    -sin(pitch),
    // 3X2
    cos(pitch)*sin(roll),
    // 3X3
    cos(pitch)*cos(roll);

    return R;
  }

  static Eigen::VectorXd InverseKinematics(double X, double Y, double Z, double r, double p, double y)
  {
    double theta1;
    double theta2;
    double D_theta2;
    double theta3;
    double D_theta3;
    double theta4;
    double theta5;
    double theta6;
    double r2;
    double R36_r11;
    double R36_r21;
    double R36_r22;
    double R36_r23;
    double R36_r31;

    Eigen::Vector3d Wrist_Position;
    Wrist_Position <<
    // 1X1
    X - l7 * CmdOrientation(r,p,y)(0,1),
    // 2X1
    Y - l7 * CmdOrientation(r,p,y)(1,1),
    // 3X1
    Z - l7 * CmdOrientation(r,p,y)(2,1);

    r2 = sqrt(pow(Wrist_Position[0],2) + pow(Wrist_Position[1],2) + pow(Wrist_Position[2] - l1,2));

    theta1 = atan2(Wrist_Position[1], Wrist_Position[0]) - M_PI / 2;

    D_theta2 = (pow(l2,2) + pow(r2,2) - pow((l3 + l5),2)) / (2 * l2 * r2);

    theta2 = atan2(Wrist_Position[2] - l1, sqrt(pow(Wrist_Position[0],2) + pow(Wrist_Position[1],2)))
          + atan2(sqrt(1-pow(D_theta2,2)),D_theta2); // sign

    // ROS_INFO("%lf, %lf", D_theta2, theta2);

    D_theta3 = (pow(l2,2) + pow((l3 + l5),2) - pow(r2,2)) / (2 * l2 * (l3 + l5));
    theta3 = -(PI - atan2(sqrt(1 - pow(D_theta3,2)), D_theta3)); // sign

    Eigen::Matrix3d R36;
    R36 = R03(theta1, theta2, theta3).transpose() * CmdOrientation(r,p,y);

    R36_r11 = R36(0,0);
    R36_r21 = R36(1,0);
    R36_r22 = R36(1,1);
    R36_r23 = R36(1,2);
    R36_r31 = R36(2,0);

    theta4 = atan2(-R36_r31, R36_r11); // pitch
    theta5 = atan2(R36_r21*cos(theta4), R36_r11); // yaw
    theta6 = atan2(-R36_r23, R36_r22); // roll

    Eigen::VectorXd theta(6);
    theta << theta1, theta2, theta3, theta4, theta5, theta6;

    return theta;
  }

  void poseCallback(const geometry_msgs::Twist::ConstPtr &msg);
  void commandCallback(const sensor_msgs::JointState::ConstPtr &msg);
  void jointCallback(const sensor_msgs::JointState::ConstPtr &msg);
  void joystickCallback(const geometry_msgs::PoseStamped& msg);

};

double TorqJ::l1 = 0.05465;
double TorqJ::l2 = 0.1585;
double TorqJ::l3 = 0.099;
double TorqJ::l4 = 0.001;
double TorqJ::l5 = 0.06483;
double TorqJ::l6 = 0.001;
double TorqJ::l7 = 0.16949;

#endif //TorqJ_H_