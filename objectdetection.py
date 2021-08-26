#!/usr/bin/env python

import numpy as np
import pandas as pd
import csv
import tensorflow as tf
from tensorflow.keras import Sequential
from tensorflow.keras import layers
from tensorflow.keras.layers import Dense
import rospy
from std_msgs.msg import Float32MultiArray
from std_msgs.msg import Int16
from tensorflow.python.keras import losses


def flagcallback(flag) :

	print('le sub est la')
	global freeway
	freeway=flag.data


def sensorcallback(tab):
	global freeway
	global counter
	global pub
	if ((freeway==1) and (counter<5)) :
		fingers=tab.data	
		print('les data sont la')

		with open('fingerstests.csv','a') as f :
				writer = csv.writer(f)
				writer.writerow(fingers)

		counter=counter+1
		print(counter)
	
	elif ((freeway==1) and (counter==5)) :
		print('dedans')
		force_tests = pd.read_csv("fingerstests.csv")
		force_tests = np.array(force_tests)

		loadedmodel=tf.keras.models.load_model("simplenn.h5")
		prediction=loadedmodel.predict(force_tests)
		bestfit=np.argmax(prediction[0])
		print(bestfit)

		while not rospy.is_shutdown():
			pub.publish(bestfit)
			rate.sleep()
			print('cest fait')
			break
		



if __name__ == '__main__':
	try:
	
		freeway=0
		counter=0

		rospy.Subscriber('detectionFlag', Int16, flagcallback)
		rospy.Subscriber('sensor', Float32MultiArray, sensorcallback)
		pub=rospy.Publisher('stiffness',Int16, queue_size=1)
		rospy.init_node('lenoeud', anonymous=False)
		rate = rospy.Rate(10)
		rospy.spin()	
			

	except rospy.ROSInterruptException:
		pass



