#!/usr/bin/env python

import rospy
from std_msgs.msg import Float32MultiArray
import numpy as np
import csv

def sensorcallback(tab):
	fingers=tab.data
	
	with open('fingersdatabase.csv','a') as f :
			writer = csv.writer(f)
			writer.writerow(fingers)


def messageSubscriber():

	rospy.init_node('forcedata', anonymous=False) #initialization of node called receiverforcedata

	rospy.Subscriber('forcedatabase', Float32MultiArray, sensorcallback) #subscription to sensorforce (the topic of our sensor's publisher)

	rospy.spin()


if __name__ == '__main__':
	try:
				
		messageSubscriber()

	except rospy.ROSInterruptException:
		pass


