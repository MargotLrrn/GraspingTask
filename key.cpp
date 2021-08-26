/* This is the source code of the node "key".
It handles the keyboard interaction with the user.
Through this interaction the user can start and stop the communication with the hand.
*/


#include "ros/ros.h"
#include "std_msgs/Int16.h"

int main(int argc, char **argv){

	ros::init(argc, argv, "key"); 
	ros::NodeHandle n;
	ros::Publisher pub = n.advertise<std_msgs::Int16>("socketFlag",1);

	std_msgs::Int16 flag;
	flag.data = 0;

	while (ros::ok()){

		while (flag.data != 1){
		std::cout << "\nTo start the communication press 1, then ENTER \n";
		std::cin >> flag.data;
		}
		pub.publish(flag);
		printf("ON flag sent");

		while (flag.data != 0){
		std::cout << "\nTo stop the communication press 0, then ENTER \n";
		std::cin >> flag.data;
		}
		pub.publish(flag);
		printf("OFF flag sent \n");

	}
	return 0;
}
