#include <Ethernet.h>

// To use the TCP version of rosserial_arduino
#define ROSSERIAL_ARDUINO_TCP

#include <ros.h>
#include "tutorial_rosardu_customtopic/packetsFromExcavator.h" // custom message
#include "tutorial_rosardu_customtopic/packetsToExcavator.h"   // custom message

// Setup a trigger digitalOutput pin & shield settings
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,1,76);
IPAddress server(192,168,1,1);
const uint16_t serverPort = 11411; // rosserial socket server port, 11411

// timestamp when the trigger signal is fired.
bool trigger_state = false;
volatile unsigned long trigger_time = 0;
unsigned long time_sec = 0;
unsigned long time_nsec = 0;
volatile unsigned long triggerCounter = 0;

// node handler
ros::NodeHandle nh;

// publisher for timestamp
tutorial_rosardu_customtopic::packetsFromExcavator msg;
ros::Publisher pub_msg("/ardu/msg", &msg);

void setup() {
  // Use serial to monitor the process
  Serial.begin(115200);
  
  // Connect the Ethernet
  Ethernet.begin(mac, ip);

  // Let some time for the Ethernet Shield to be initialized
  delay(1000);

  Serial.println("");
  Serial.println("Ethernet connection info...");
  Serial.println("IP address: ");
  Serial.println(Ethernet.localIP());

  // Set the connection to rosserial socket server
  nh.getHardware()->setConnection(server, serverPort);
  nh.initNode();

  // Another way to get IP
  Serial.print("MY IP = ");
  Serial.println(nh.getHardware()->getLocalIP());

  // advertise publisher and initialize msg.
  nh.advertise(pub_msg);
  msg.bytes_length = 72;
  msg.bytes = (uint8_t*)realloc(msg.bytes, msg.bytes_length * sizeof(uint8_t));
}

uint8_t cnt = 0;

void loop() {
  if (nh.connected()) {// if ROS master is connected,
      trigger_time = micros(); // microseconds
      time_sec  = trigger_time/1000000;
      time_nsec = (trigger_time - time_sec*1000000)*1000;
      msg.header.seq = 0;
      msg.header.stamp.sec  = time_sec;
      msg.header.stamp.nsec = time_nsec;
      msg.header.frame_id = "my_arduino";

      // fill data!
      msg.bytes[0] = cnt;
      for(int i = 1; i < msg.bytes_length; ++i){
        msg.bytes[i] = i;
      }
      msg.n_bytes = msg.bytes_length;
      pub_msg.publish( &msg );  
      ++cnt;
      if(cnt > 255) cnt = 0;
  } 
  else{}// When TCP/IP connection is lost!
  
  nh.spinOnce();
}
