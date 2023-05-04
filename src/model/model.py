import tensorflow as tf
from tensorflow import keras
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from tensorflow.keras import layers
from sklearn import datasets
from datetime import datetime

data = pd.read_csv("~/Projects/part1_o2test.csv")

# Set seed for random number generation
time = int(round(datetime.now().timestamp()))

# Pawn e2 -> e3
test_prediction = np.asarray([
25163152444444440000000000000000000000000000000044444444251631520000000000000000000000000000000000000000000000000000000000000000,
75,
83,
1
]).astype(np.float64)

#print('test_prediction is of size: ', tf.shape(test_prediction).numpy())

# Split data
training_data = data.sample(frac=0.8, random_state=time)
testing_data = data.drop(training_data.index)

X_train = training_data.iloc[:, [0,1,3]]
y_train = training_data.iloc[:, 2]
X_test = testing_data.iloc[:, [0,1,3]]
y_test = testing_data.iloc[:, 2]

#X_train = tf.keras.utils.normalize(X_train)
#y_train = tf.keras.utils.normalize(y_train)
#X_test = tf.keras.utils.normalize(X_test)
#y_test = tf.keras.utils.normalize(y_test)

X_train = np.asarray(X_train).astype(np.float64)
#X_train = X_train.reshape(799999,3)
y_train = np.asarray(y_train).astype(np.float64)
X_test = np.asarray(X_test).astype(np.float64)
y_test = np.asarray(y_test).astype(np.float64)

#print('X_train is of size: ', tf.shape(X_train).numpy())
#print('y_train is of size: ', tf.shape(y_train).numpy())
#print('X_test is of size: ', tf.shape(X_test).numpy())
#print('y_test is of size: ', tf.shape(y_test).numpy())

model = tf.keras.Sequential([
  #keras.layers.Input(shape=(3,)),
 keras.layers.Dense(5000, activation="relu"),
 # keras.layers.Dense(4000, activation="relu"),
 # keras.layers.Dense(3000, activation="relu"),
 # keras.layers.Dense(2500, activation="relu"),
  keras.layers.Dense(192, activation="softmax")
])

model.compile(optimizer="adam", loss="sparse_categorical_crossentropy", metrics=["accuracy"])
model.fit(X_train, y_train, epochs=1)

test_loss, test_acc = model.evaluate(X_test, y_test)
print('\nTest Accuracy:', test_acc)

#prediction = model.predict(test_prediction)
#print('\nPrediction:', prediction)
