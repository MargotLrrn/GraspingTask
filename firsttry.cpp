#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Int16.h"
#include "telehand/my_sensor.h"
#include "std_msgs/Float32MultiArray.h"
#include <math.h>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <windows.h>

class ps{
    
    int MIN_HAND = 1000;
    int MAX_HAND = 2000;
    int TIME_EXEC = 25; //time for the execution in milliseconds

    int hSocket;
    int i,counter=0;
    std_msgs::String S;
    std::string all;
    bool freeWay = true, freeway2=false;
    int action=1;
    int condition[5]={1,1,1,1,1};
    float threshold=10,threshold2=300;
    float forcewanted[5]={0,0,0,0,0};
	float forcemid[5]={10,10,10,10,10};
	float forcesoft[5]={5,5,5,5,5};
	float forcehard[5]={20,20,20,20,20};
	

    private:
    	ros::NodeHandle n; 
    	ros::Subscriber sub; 
    	ros::Subscriber sub2;
    	ros::Subscriber sub3;
		ros::Publisher pub;
    public:
    	ps(){
			printf("initialiser");
        	sub = n.subscribe("socketFlag",1,&ps::callback,this);
        	sub2 = n.subscribe("sensor",1,&ps::callback2,this);
        	pub = n.advertise<std_msgs::Int16>("detectionFlag",1);

    	}


		void callback(std_msgs::Int16 flag){

        	if (flag.data==1){
            	hSocket = socket(AF_INET, SOCK_STREAM, 0);    
            	if (hSocket == -1) perror("Could not create the socket\n");
            	else{    
                	printf("The socket has been created\n");
                	int iRetval=-1;
                	int ServerPort = 1064;
                	struct sockaddr_in remote= {0};
                	remote.sin_addr.s_addr = inet_addr("192.168.0.104");
                	remote.sin_family = AF_INET;
remote.sin_port = htons(ServerPort);
					iRetval = connect(hSocket,(struct sockaddr *)&remote,sizeof(struct sockaddr_in));
               		if (iRetval < 0){
                    	perror("Connection failed");
                    	close(hSocket);
                    	shutdown(hSocket,0);
                    	shutdown(hSocket,1);
                    	shutdown(hSocket,2);
                    	printf("Socket closed\n");              
                	}
                	else {
                   		printf("Sucessfully conected with server\n");
                    	S.data =  "I001-1000-6-1-2000-2-1000-3-1000-4-1000-5-1000-6-1500";
                    	char *outMsg = new char[S.data.length()+1];
                    	strcpy(outMsg, S.data.c_str());
                    	send(hSocket, outMsg, strlen(outMsg), 0); 
                   		ros::Duration(2).sleep();//wait 2 sec
                    	freeWay = true;
                	}
            	}
        	}

        	else{
            	if(flag.data==0){
                	close(hSocket);
               		shutdown(hSocket,0);
                	shutdown(hSocket,1);
                	shutdown(hSocket,2);
                	printf("Socket closed\n");    
                	freeWay = false;        
            	}
       		}
    	}


		void callback2(std_msgs::Float32MultiArray forcedata) {
			
        	if (freeWay){
           		switch(action) { 
					case 1:
						closing(forcedata);
						break; 

					case 2:
						detection();
						break;


					case 3:
						control(forcedata);
						break; 

					case 4:
						printf("relache");
						S.data =  "I001-1000-6-1-2000-2-2000-3-2000-4-2000-5-2000-6-1500";
    					char *outMsg = new char[S.data.length()+1];
						strcpy(outMsg, S.data.c_str());
						send(hSocket, outMsg, strlen(outMsg), 0); 
						ros::Duration(2).sleep();
						break;

				} 
       		}
    	}

    	void callback3(std_msgs::Int16 stiffness){

			
        	int type = stiffness.data;
        	switch(type){
            	case 1:
            		*forcewanted=*forcehard;
					break;

            	case 2:
            		*forcewanted=*forcemid;
					break;

            	case 3:
            		*forcewanted=*forcesoft;
					break;
        	}
			
			freeway2=true;
        
    	}




    	void sendingData(){

        	int fingerstring[5];
        	int fingers[5];
			all = "I001-1000-6-1-2000-2-2000-3-2000-4-2000-5-2000-6-1500";

			fingerstring[0]=all[14]*1000+all[15]*100+all[16]*10+all[17];
			fingerstring[1]=all[21]*1000+all[22]*100+all[23]*10+all[24];
			fingerstring[2]=all[28]*1000+all[29]*100+all[30]*10+all[31];
			fingerstring[3]=all[35]*1000+all[36]*100+all[37]*10+all[38];
			fingerstring[4]=all[42]*1000+all[43]*100+all[44]*10+all[45];

			for (i=0;i<5;i++){
       			fingers[i]=fingerstring[i]+condition[i]*100;
			}


			S.data = "I001-" + std::to_string(TIME_EXEC) + "-6-1-" + std::to_string(fingers[0]) + "-2-" + std::to_string(fingers[1]) + "-3-" + std::to_string(fingers[2]) + "-4-" + std::to_string(fingers[3]) + "-5-" + std::to_string(fingers[4]) + "-6-1500";

			char *outMsg = new char[S.data.length()+1];
			strcpy(outMsg, S.data.c_str());
			send(hSocket, outMsg, strlen(outMsg), 0); 
			ros::Duration(2).sleep();
        
		}


		void closing(std_msgs::Float32MultiArray forcedata){
			

			printf("closing");
    		counter=0;
    		for (int i=0; i<5; i++){
				if (forcedata.data[i]>threshold){
					condition[i]=0;
					counter+=1;
				}
			}
			
			
			if (counter!=5) sendingData();
			else { action+=1; }

		}


		void control(std_msgs::Float32MultiArray forcedata){
    		printf("control");
    		float diff[5];
			counter=0;
			for (i=0;i<5;i++){
   				diff[i]=forcedata.data[i] - forcewanted[i];
        		if (diff[i]>0) {
					condition[i]=0;
					counter+=1;
				}
    		}
   			if (counter!=5) sendingData();
			else { action+=1; }
    
		}




		void detection(){
			
			for (i=0;i++;i<5){
				condition[i]=2;
			}
			sendingData();

			sleep(1000);


			printf("detection");
    		std_msgs::Int16 flag;
    		flag.data = 1;
    		pub.publish(flag);
    		sub3 = n.subscribe("stiffness",1,&ps::callback3,this);
			if (freeway2) {action+=1;};
		}

};




int main(int argc, char **argv){

    ros::init(argc, argv, "mime"); 
    ps my_ps;
    ros::spin();

    return 0;
}

