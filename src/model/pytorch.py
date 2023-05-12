import torch
from torch import nn # neural networks
from torch import optim # optimizers
from torchmetrics import Accuracy
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from sklearn.model_selection import train_test_split
from sklearn.datasets import make_blobs
from helpers import load_model, save_model, plot_predictions, accuracy_fn
import platform

# Check PyTorch Version
print(f"PyTorch Version: {torch.__version__}")

# Device agnostic setup
device = "cuda" if torch.cuda.is_available() else "cpu"
pc = platform.node()

# Make exception for desktop on AMD using ROCM
if pc == 'eden':
  device = "cpu"
print(f"Using {device} device\nNumber of cores available: {torch.get_num_threads()}")

# Create Data
n_samples = 100000
n_features = 2
n_classes = 4
random_state = 42
X, y = make_blobs(n_samples,
                    n_features = n_features,
                    centers = n_classes,
                    cluster_std = 1.5,
                    random_state = random_state)

# Set up metrics
# torchmetric_accuracy = Accuracy()

# Turn data into tensors
X = torch.from_numpy(X).type(torch.LongTensor)
y = torch.from_numpy(y).type(torch.LongTensor)

X_train, X_test, y_train, y_test = train_test_split(X,
                                                    y,
                                                    test_size = 0.2,
                                                    random_state = random_state)

X_train, y_train = X_train.type(torch.LongTensor), y_train.type(torch.LongTensor)
X_test, y_test = X_test.type(torch.LongTensor), y_test.type(torch.LongTensor)

print(f"Length of X_train: {len(X_train)}\nLength of y_train: {len(y_train)}")
print(f"Length of X_test: {len(X_test)}\nLength of y_test: {len(y_test)}")


class NeuralNetwork(nn.Module):
  # Constuctor
  def __init__(self, input_features, output_features, hidden_units = 8):
    """
      Args:
        input_features (int): number of input features of the model
        output_features (int): number of output features of the model
        hidden_units (int): Number of hidden units between layers, default 8
    """
    super().__init__()

    # Model Parameters
    self.linear_layers = nn.Sequential(
      nn.Linear(in_features = input_features, out_features = hidden_units),
      nn.ReLU(),
      nn.Linear(in_features = hidden_units, out_features = hidden_units),
      nn.ReLU(),
      nn.Linear(in_features = hidden_units, out_features = output_features)
    )

    # Define the computation in the model
  def forward(self, x):
    return self.linear_layers(x)

# Instance of the model
u_model = NeuralNetwork(input_features = 2, # Matches with how many features X has
                      output_features = 4, # Matches up with the number of classes
                      hidden_units = 128).to(device)
model = torch.compile(u_model).to(device)
print(f"Set model to run on correct device: {next(model.parameters()).device}")

# Put data on the correct device
X_train = X_train.to(device)
y_train = y_train.to(device)
X_test = X_train.to(device)
y_test = y_train.to(device)
print(f"Set data to run on correct device: {device}")

epochs = 1000
loss_fn = nn.CrossEntropyLoss()
optimizer = optim.Adam(params = model.parameters(),
                      lr = 0.001)

for epoch in range(epochs):

    model.train()
    y_logits = model(X_train.type(torch.float32))
    y_pred = torch.softmax(y_logits, dim = 1).argmax(dim = 1)
    # logits (raw output) -> prediction probs (softmax) -> prediction labels (argmax)
    loss = loss_fn(y_logits, y_train)
    acc = accuracy_fn(y_true = y_train, y_pred = y_pred)
    # torchmetric_accuracy(y_preds.to(device), y_test.to(device))
    optimizer.zero_grad()
    loss.backward()
    optimizer.step()
    model.eval()

    with torch.inference_mode():
      test_logits = model(X_test.type(torch.float32))
      test_pred = torch.softmax(test_logits, dim = 1).argmax(dim = 1)

      test_loss = loss_fn(test_logits.to(device), y_test.type(torch.LongTensor).to(device))
      test_acc = accuracy_fn(y_true = y_test,
                            y_pred = test_pred)

      if epoch % 10 == 0:
            print(f"Epoch: {epoch} | Loss: {loss:.5f}, Acc: {acc:.2f}% | Test Loss: {test_loss:.5f}, Test_Acc: {test_acc:.2f}%")


model.eval()
with torch.inference_mode():
  y_logits = model(X_test.type(torch.float32).to(device))

y_pred_probs = torch.softmax(y_logits.type(torch.float32).to(device), dim = 1)
y_preds = torch.argmax(y_pred_probs, dim = 1)
print(f"y predictions: {y_preds[:10]}")
print(f"y actual: {y_test[:10]}")

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

