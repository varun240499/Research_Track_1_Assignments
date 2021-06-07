#include "ros/ros.h"
#include "nav_msgs/Odometry.h"
#include "geometry_msgs/Twist.h"
#include "my_srv/Position.h"


ros::Publisher pub;                             //publisher as global
ros::ServiceClient client1;
float targetx=-6;
float targety=6;
geometry_msgs::Twist vel;                    //to get the current position of the odometry


void positionCallback(const nav_msgs::Odometry::ConstPtr&msg)//odometry: estimation of posi& vel robo
{
     ROS_INFO("Robot position [%f, %f]", msg->pose.pose.position.x, msg->pose.pose.position.y);
     float x=msg->pose.pose.position.x;  //to declare X & Y
     float y=msg->pose.pose.position.y;
     
if(x-targetx<0)vel.linear.x=sqrt(pow((x-targetx),2)); //to calculate the dist of X & Y
   
else vel.linear.x=-sqrt(pow((x-targetx),2));
    
if  (y-targety<0)vel.linear.y=sqrt(pow((y-targety),2));
    
else vel.linear.y=-sqrt(pow((y-targety),2));

     pub.publish(vel);
ROS_INFO("distancex= %f distancey= %f",pow((x-targetx),2),pow((y-targety),2));
if(pow((x-targetx),2)+pow((y-targety),2)<0.5)
{
     ROS_INFO("target reached");
     my_srv::Position rec_pos; 
     client1.call(rec_pos);                 // to get the new random target
     targetx=rec_pos.response.x;
     targety=rec_pos.response.y;
}
   
}
    

int main(int argc, char **argv)
{
 
  ros::init(argc, argv, "assignment1");
  ros::NodeHandle n;

  client1 = n.serviceClient<my_srv::Position>("/position");          // to set client2 connect to the service
  pub = n.advertise<geometry_msgs::Twist>("cmd_vel", 1000);
  ros::Subscriber sub = n.subscribe("/odom", 1000, positionCallback); //to sets subscriber to get current posi
  
   
  ros::spin();

  return 0;
}

