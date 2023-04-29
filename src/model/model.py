import tensorflow as tf
from tensorflow import keras
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from tensorflow.keras import layers
from sklearn import datasets

#data = pd.read_csv("~/Projects/3DChess/src/output2.csv")
data = pd.read_csv("~/Projects/3DChess/src/o2test.csv")
#class_names=["Position, Level_from, Bitposition_from, Level_to, Bitposition_to,Result"]
#(train_pos, train_labels), (test_pos, test_labels) = data.load_data()
#print(train_pos[7])
#features = np.array(data.copy())

#(train_pos, train_labels), (test_pos, test_labels) = data.load_data()

training_data = data.sample(frac=0.8, random_state=25)
testing_data = data.drop(training_data.index)

X_train = training_data.iloc[:, [0,1,3]]
y_train = training_data.iloc[:, 2]
X_test = testing_data.iloc[:, [0,1,3]]
y_test = testing_data.iloc[:, 2]

X_train = np.asarray(X_train).astype(np.float64)
y_train = np.asarray(y_train).astype(np.float64)
X_test = np.asarray(X_test).astype(np.float64)
y_test = np.asarray(y_test).astype(np.float64)

model = tf.keras.Sequential([
  keras.layers.Dense(10000, activation="relu"),
  keras.layers.Dense(5000, activation="relu"),
  keras.layers.Dense(4000, activation="relu"),
  keras.layers.Dense(3000, activation="relu"),
  keras.layers.Dense(2500, activation="relu"),
  keras.layers.Dense(192, activation="softmax")
])

#model.compile(loss = tf.keras.losses.MeanSquaredError(), optimizer = tf.keras.optimizers.Adam())
model.compile(optimizer="adam", loss="sparse_categorical_crossentropy", metrics=["accuracy"])

model.fit(X_train, y_train, epochs=5)

test_loss, test_acc = model.evaluate(X_test, y_test)
print('\nTest Accuracy:', test_acc)
