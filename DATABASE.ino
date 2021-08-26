#include <Arduino.h>
#include <ros.h>
#include <std_msgs/MultiArrayLayout.h>
#include <std_msgs/MultiArrayDimension.h>
#include <std_msgs/Float32MultiArray.h>

#include <std_msgs/Float32.h>

#define PRESS_MIN	20
#define PRESS_MAX	1500
#define VOLTAGE_MIN 100
#define VOLTAGE_MAX 3300


int sensorPin[5]={A0, A1, A2, A3, A4};
int hard=0,midsoft=1,soft=2;

ros::NodeHandle nh;

std_msgs::Float32MultiArray tab_msg;

ros::Publisher pub("forcedatabase", &tab_msg);

void setup()
{
  nh.initNode();           // initialisation of ROS node handle

  nh.advertise(pub);
  
  tab_msg.data = (float*)malloc(sizeof(float)*6);
  
  tab_msg.data_length = 6;
}


void loop()
{
  //faire les petites affaires avec les mesures de doigts
  
   for (int i=0; i<5; i++)
  {
    float value = getPressValue(sensorPin[i]);
   tab_msg.data[i]=value;
 }
  
  tab_msg.data[5]=midsoft;

  pub.publish( &tab_msg );
  
  nh.spinOnce();
  delay(100);
  
}


double getPressValue(int pin)
{
	double PRESS_AO = 0;
	double VOLTAGE_AO = 0;
	double value = analogRead(pin);
        //on lit la data

	//DEBUGSerial.print("AD = ");
	//DEBUGSerial.print(value);
	//DEBUGSerial.print(" ,");

	VOLTAGE_AO = map(value, 0, 664, 0, 3300);
        //on la transforme en courant allant de 0 a 3300mV

	//DEBUGSerial.print("V = ");
	//DEBUGSerial.print(VOLTAGE_AO);
	//DEBUGSerial.print(" mv,");
        //affichage de cette valeur en mV

	if(VOLTAGE_AO < VOLTAGE_MIN)
	{
		PRESS_AO = 0;
                //voltage trop petit indique absence de force
	}
	else if(VOLTAGE_AO > VOLTAGE_MAX)
	{
		PRESS_AO = PRESS_MAX;
                //voltage trop grand indique force trop grande a supporter ou mesurer par le capteur
	}
	else
	{
		//PRESS_AO = map(VOLTAGE_AO, VOLTAGE_MIN, VOLTAGE_MAX, PRESS_MIN, PRESS_MAX);
	        //mettre la pression entre sa valeur min et max par rapport aux valeurs min et max de voltage
                //EN REALITE la relation ne se fait pas en mappant
                PRESS_AO = polymodele(VOLTAGE_AO);
        }
	
        //DEBUGSerial.print("F (in g)=");
        
	return PRESS_AO;
}



double polymodele(double VOLTAGE){
        
        double PRESS;
        
        PRESS = 8.0403*pow(10,-22)*pow(VOLTAGE,7)+(-9.0055)*pow(10,-18)*pow(VOLTAGE,6)+4.0069*pow(10,-14)*pow(VOLTAGE,5)+(-9.0355)*pow(10,-11)*pow(VOLTAGE,4)+1.0905*pow(10,-7)*pow(VOLTAGE,3)+(-6.6896)*pow(10,-5)*pow(VOLTAGE,2)+0.0192*VOLTAGE-1.3612;
        return PRESS*100;
}
