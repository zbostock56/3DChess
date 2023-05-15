import torch
from torch import nn, optim
from torchmetrics import Accuracy
from torch.utils.data import DataLoader, Dataset
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from sklearn.model_selection import train_test_split
from sklearn import datasets
from datetime import datetime
from helpers import load_model, save_model, plot_predictions, accuracy_fn, print_train_time
from train_test import test_step, train_step, eval_model
from model import NeuralNetwork, CustomDataset
import platform
from timeit import default_timer as timer

# Set seed for RNG
seed = int(round(datetime.now().timestamp()))
print(f"Time seed: {seed}")

# import data
samples = pd.read_csv("~/move_data/samples.csv")
labels = pd.read_csv("~/move_data/lables.csv")

# Check PyTorch Version
print(f"PyTorch Version: {torch.__version__}")

# Device agnostic setup
# Make exception for desktop on AMD using ROCM
pc = platform.node()
if pc == 'eden':
  device = "cpu"
else:
  device = "cuda" if torch.cuda.is_available() else "cpu"
print(f"Using {device} device\nNumber of cores available: {torch.get_num_threads()}")

# Batch size hyperparameter
BATCH_SIZE = 32

X_train = samples.sample(frac=0.8, random_state=seed)
X_test = samples.drop(X_train.index)
y_train = labels.sample(frac=0.8, random_state=seed)
y_test = labels.drop(y_train.index)

training_data = CustomDataset(samples = X_train,
                              labels = y_train,
                              device = device)
testing_data = CustomDataset(samples = X_test,
                             labels = y_test,
                             device = device)

# Batches that are randomized
train_dataloader = DataLoader(dataset = training_data,
                                 batch_size = BATCH_SIZE,
                                 shuffle = True,
                                 num_workers = 8)
test_dataloader = DataLoader(dataset = testing_data,
                                batch_size = BATCH_SIZE,
                                shuffle = False,
                                num_workers = 8)
exit(0)
# Set up metrics
# torchmetric_accuracy = Accuracy()

# Instance of the model
u_model = NeuralNetwork(
  input_features = 193, # Matches with how many features X has
  output_features = 192, # Matches up with the number of classes
  hidden_units = 512
  )
model = torch.compile(u_model).to(device)
print(f"Set model to run on correct device: {next(model.parameters()).device}")

EPOCHS = 3
loss_fn = nn.CrossEntropyLoss()
optimizer = optim.Adam(params = model.parameters(),
                      lr = 0.001)

train_time_start_on_gpu = timer()

for EPOCH in range(EPOCHS):
  print(f"Epoch: {EPOCH}\n------------------------")
  train_step(data_loader = train_dataloader,
             model = model,
             loss_fn = loss_fn,
             optimizer = optimizer,
             accuracy_fn = accuracy_fn,
             device = device)
  test_step(data_loader = test_dataloader,
            model = model,
            loss_fn = loss_fn,
            accuracy_fn = accuracy_fn,
            device = device)

train_time_end_on_gpu = timer()
total_train_time_model = print_train_time(start = train_time_start_on_gpu,
                                          end = train_time_end_on_gpu,
                                          device = device)

"""
plt.plot(epoch_count, train_loss_values, label="Train loss")
plt.plot(epoch_count, test_loss_values, label="Test loss")
plt.title("Training and test loss curves")
plt.ylabel("Loss")
plt.xlabel("Epochs")
plt.legend();
plt.show();
"""
# saved_path = save_model(model)
# new_model = load_model(saved_path)

