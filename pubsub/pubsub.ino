/*
 * rosserial PubSub Example
 * Prints "hello world!" and toggles led
 */

#include <ros.h>
#include <sensor_msgs/Joy.h>
#include <std_msgs/String.h>

ros::NodeHandle  nh;
std_msgs::String str_msg;
ros::Publisher chatter("ArduinoMessage", &str_msg);

void messageCb( const sensor_msgs::Joy& joymsg){
     // blink the led
     double s1 = joymsg.axes[1];
     double s2 = joymsg.axes[4];
     driveSaberChannel(5,s1,3);
     driveSaberChannel(6,s2,4);
}

ros::Subscriber<sensor_msgs::Joy> sub("/joy", messageCb );

void setupSaberChannel(int pins, int pind){

  pinMode(pins, OUTPUT);
  pinMode(pind, OUTPUT);
  analogWrite(pins, 0);
  
}

void driveSaberChannel(int pins, double mspeed, int pind){

  if (mspeed < 0){
    mspeed = mspeed *-1;
    digitalWrite(pind,LOW);
  }else
  {
    digitalWrite(pind,HIGH);
  }
  analogWrite(pins, (int)(mspeed*255));
String s = String(((int)mspeed*255),DEC);
  str_msg.data = s.c_str();
  chatter.publish( &str_msg );
}

void setup()
{
  setupSaberChannel(5,3);
  setupSaberChannel(6,4);
  nh.initNode();
  nh.subscribe(sub);
  nh.advertise(chatter);
}

void loop()
{
  nh.spinOnce();
  delay(10);
}
