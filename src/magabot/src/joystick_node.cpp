#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <signal.h>
#include <termios.h>
#include <stdio.h>
#include <sensor_msgs/Joy.h>


class Joystick_Control
{
public:
	Joystick_Control();

private:
	void joyCallback(const sensor_msgs::Joy::ConstPtr& joy); 
	ros::NodeHandle nh_,ph_;
	geometry_msgs::Twist twist;
	int linear_, angular_;
	double vel_linear_max, vel_ang_max;
	double l_scale_, a_scale_;
	ros::Publisher vel_pub_;
	ros::Subscriber joy_sub_;
  
};


Joystick_Control::Joystick_Control():
  ph_("~"),
  vel_linear_max(0.2),
  vel_ang_max(0.8)
{
	ph_.param("vel_linear_max", vel_linear_max, vel_linear_max);
	ph_.param("vel_ang_max", vel_ang_max, vel_ang_max);
	
	//ROS_INFO("vel_linear_max=%f \t vel_ang_max=%f", vel_linear_max, vel_ang_max);
	vel_pub_ = nh_.advertise<geometry_msgs::Twist>("/cmd_vel", 1);
	joy_sub_ = nh_.subscribe<sensor_msgs::Joy>("joy", 10, &Joystick_Control::joyCallback, this);

}

void Joystick_Control::joyCallback(const sensor_msgs::Joy::ConstPtr& joy)
{
	twist.angular.z = joy->axes[0]*vel_ang_max;
	twist.linear.x = joy->axes[1]*vel_linear_max;
	//ROS_INFO("linear: %f  ; angular: %f", twist.linear.x, twist.angular.z);
	vel_pub_.publish(twist);
}


int main(int argc, char** argv)
{
  ros::init(argc, argv, "joy_control");
  Joystick_Control joy_control;

  ros::spin();
}


