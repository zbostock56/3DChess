import torch
from torch import nn # neural networks
from torch import optim # optimizers
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from sklearn.model_selection import train_test_split
from sklearn.datasets import make_circles
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
print(f"Using {device} device")

# Create Data
n_samples = 10000
X, y = make_circles(n_samples,
                    noise = 0.03,
                    random_state = 42)

X = torch.from_numpy(X).type(torch.float32)
y = torch.from_numpy(y).type(torch.float32)

X_train, X_test, y_train, y_test = train_test_split(X,
                                                    y,
                                                    test_size = 0.2,
                                                    random_state = 42)

print(f"Length of X_train: {len(X_train)}\nLength of y_train: {len(y_train)}")
print(f"Length of X_test: {len(X_test)}\nLength of y_test: {len(y_test)}")

class NeuralNetwork(nn.Module):
  # Constuctor
  def __init__(self):
    super().__init__()

    # Model Parameters
    self.linear_layers = nn.Sequential(
      nn.Linear(in_features = 2, out_features = 8),
      nn.Linear(in_features = 8, out_features = 1)
    )
    
    # Define the computation in the model
  def forward(self, x):
    return self.linear_layers(x)
    # x -> layer1 -> layer2 -> output

# Instance of the model
model = NeuralNetwork().to(device)
print(f"Set model to run on correct device: {next(model.parameters()).device}")

# Put data on the correct device
X_train = X_train.to(device)
y_train = y_train.to(device)
X_test = X_train.to(device)
y_test = y_train.to(device)
print(f"Set data to run on correct device: {device}")

epochs = 10000
loss_fn = nn.BCEWithLogitsLoss()
optimizer = optim.Adam(params = model.parameters(),
                      lr = 0.000001)

for epoch in range(epochs):
    # Set to train mode
    model.train()
    # Forward pass on train data
    y_logits = model(X_train).squeeze()
    y_pred = torch.round(torch.sigmoid(y_logits)) # logits -> prediction probs -> prediction labels
    
    # Calculate the loss
    loss = loss_fn(y_logits, y_train)
    acc = accuracy_fn(y_true = y_train,
                      y_pred = y_pred)

    # Zero grad of the optimizer
    optimizer.zero_grad()

    # Backpropagation
    loss.backward()

    # Progress the optimizer
    optimizer.step()

    # Put the model in evaluation mode
    model.eval()

    with torch.inference_mode():
      # Forward pass on test data
      test_logits = model(X_test).squeeze()
      test_pred = torch.round(torch.sigmoid(test_logits))

      # Caculate loss on test data
      test_loss = loss_fn(test_logits, y_test.type(torch.float))
      test_acc = accuracy_fn(y_true = y_test,
                            y_pred = test_pred)

      if epoch % 50 == 0:
            print(f"Epoch: {epoch} | Loss: {loss:.5f} , Acc: {acc:.2f} | Test Loss: {test_loss:.5f}, Test_Acc: {test_acc:.2f}")

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

