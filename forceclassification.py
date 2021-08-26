#!/usr/bin/env python

import numpy as np
import pandas as pd
import tensorflow as tf
from tensorflow.keras import Sequential
from tensorflow.keras import layers
from tensorflow.keras.layers import Dense
import rospy
from std_msgs.msg import Float32


force_train = pd.read_csv("fingersdatabase.csv",names=["thumb","index","middle","ring","little","stiffness"])

force_train.head()

force_features = force_train.copy()
force_labels = force_features.pop('stiffness')

force_features = np.array(force_features)
force_labels = np.array(force_labels)



model = tf.keras.Sequential([
    tf.keras.layers.Dense(units=256, activation='relu'),
    tf.keras.layers.Dense(units=192, activation='relu'),
    tf.keras.layers.Dense(units=128, activation='relu'),
    tf.keras.layers.Dense(units=3, activation='softmax')
])

model.compile(optimizer='adam', 
              loss='sparse_categorical_crossentropy',
              metrics=['accuracy'])

history = model.fit(
    force_features,force_labels, 
    epochs=10, 
)


model.save("simplenn.h5")




