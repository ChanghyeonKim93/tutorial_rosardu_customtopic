#include <ros/ros.h>
#include <iostream>
#include <time.h>
#include <string>
#include <sstream>

#include "tutorial_rosardu_customtopic/resultStates.h" // server for 'GCS node'

using namespace std;

void callback(const tutorial_rosardu_customtopic::resultStatesConstPtr& msg){
    for(unsigned int i = 0; i < 72; ++i){
       // msg.bytes.push_back(i);
    }
    //msg.n_bytes = msg.bytes.size();
}   

int main(int argc, char **argv) {
    ros::init(argc, argv, "rosardu_communication");
    ros::NodeHandle nh("~");
    // user input manual.

    tutorial_rosardu_customtopic::resultStates msg_;
    ros::Publisher pub = nh.advertise<tutorial_rosardu_customtopic::resultStates>("/ardu/msg",1);

    unsigned int cnt = 0;
    while(ros::ok()){
        ros::spinOnce();
        ros::Duration(0.25).sleep();

        msg_.n_bytes = 0;
        msg_.bytes.clear();
	msg_.bytes.push_back(cnt);
        for(unsigned int i = 1; i < 71; ++i){
           msg_.bytes.push_back(i);
        }
        msg_.n_bytes = msg_.bytes.size();

	pub.publish(msg_);

        cout << "cnt : " << cnt << "\n";
        ++cnt;
        if(cnt == 255) cnt = 0;
    }
    
    ROS_INFO_STREAM("End of the program.\n");
    return -1;
}
