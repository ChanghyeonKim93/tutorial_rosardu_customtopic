#include <ros/ros.h>
#include <iostream>
#include <time.h>
#include <string>
#include <sstream>

#include "tutorial_rosardu_customtopic/packetsToExcavator.h"   // custom messages
#include "tutorial_rosardu_customtopic/packetsFromExcavator.h" // custom messages

using namespace std;

void callbackFromExcavator(const tutorial_rosardu_customtopic::packetsFromExcavatorConstPtr& msg_from_ex){
   int n_bytes = msg_from_ex->n_bytes;
   cout << "subscriber callback ... n_bytes: " << n_bytes << "\n";

   // Receiver... do it yourself...
};



int main(int argc, char **argv) {
    ros::init(argc, argv, "rosardu_communication");
    ros::NodeHandle nh("~");

    // user input manual.
    tutorial_rosardu_customtopic::packetsToExcavator   msg_to_ex;
    ros::Publisher  pub_to_ex   = nh.advertise<tutorial_rosardu_customtopic::packetsToExcavator>  ("/adb", 1);

    tutorial_rosardu_customtopic::packetsFromExcavator msg_from_ex;
    ros::Subscriber sub_from_ex = nh.subscribe<tutorial_rosardu_customtopic::packetsFromExcavator>("/ardu/msg", 1, callbackFromExcavator);

    unsigned int cnt = 0;
    while(ros::ok()){
        ros::spinOnce();
        ros::Duration(0.1).sleep();

        msg_to_ex.n_bytes = 0;
        msg_to_ex.bytes.clear();
	msg_to_ex.bytes.push_back(cnt);
        for(unsigned int i = 1; i < 72; ++i){
           msg_to_ex.bytes.push_back(i);
        }
        msg_to_ex.n_bytes = msg_to_ex.bytes.size();
	pub_to_ex.publish(msg_to_ex);

        ++cnt;
        if(cnt == 255) cnt = 0;
    }
    
    ROS_INFO_STREAM("End of the program.\n");
    return -1;
}
