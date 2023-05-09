import torch
from torch import nn # neural networks
from torch import optim # optimizers
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from pathlib import Path # Save/Load Model

# Check PyTorch Version
print(f"PyTorch Version: {torch.__version__}")

# Device agnostic setup
device = "cuda" if torch.cuda.is_available() else "cpu"
print(f"Using {device} device")

# Create Data
weight = 0.7
bias = 0.3
start = 0
end = 1
step = 0.0001
# X = features
# y = labels
X = torch.arange(start, end, step).unsqueeze(dim=1)
y = weight * X + bias

train_split = int(0.8 * len(X))
X_train, y_train = X[:train_split], y[:train_split]
X_test, y_test = X[train_split:], y[train_split:]

def plot_predictions(train_data=X_train,
                     train_labels=y_train,
                     test_data=X_test,
                     test_labels=y_test,
                     predictions=None):
  # Change to CPU for matplotlib requirements
  train_data = train_data.to("cpu")
  train_labels = train_labels.to("cpu")
  test_data = test_data.to("cpu")
  test_labels = test_labels("cpu")

  plt.figure(figsize = (10, 7))

  # Plot training data in blue
  plt.scatter(train_data, train_labels, c = "b", s = 4, label = "Training data")

  # Plot testing data in green
  plt.scatter(test_data, test_labels, c = "g", s = 4, label= "Testing data")

  if predictions is not None:
    # Plot the predictions
    predictions = predictions.to("cpu")
    plt.scatter(test_data, predictions, c = "r", s = 4, label = "Predictions")

  plt.legend(prop = {"size": 14})
  plt.show()

class NeuralNetwork(nn.Module):
  # Constuctor
  def __init__(self):
    super().__init__()

    # Model Parameters
    self.linear_layer = nn.Linear(in_features = 1,
                                  out_features = 1)
    # Define the computation in the model
  def forward(self, x: torch.Tensor) -> torch.Tensor:
    return self.linear_layer(x)

# Instance of the model
model = NeuralNetwork()
model.to(device)
print(f"Set model to run on correct device: {next(model.parameters()).device}")

# Put data on the correct device
X_train = X_train.to(device)
y_train = y_train.to(device)
X_test = X_train.to(device)
y_test = y_train.to(device)
print(f"Set data to run on correct device: {device}")

epochs = 1000
lossfn = nn.L1Loss()
optimizer = optim.SGD(params = model.parameters(),
                      lr = 0.01)

# Create empty loss lists to track values
train_loss_values = []
test_loss_values = []
epoch_count = []

for epoch in range(epochs):
    # Set to train mode
    model.train()

    # Forward pass on train data
    y_pred = model(X_train)

    # Calculate the loss
    loss = lossfn(y_pred, y_train)

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
      test_pred = model(X_test)

      # Caculate loss on test data
      test_loss = lossfn(test_pred, y_test.type(torch.float)) # predictions come in torch.float datatype, so comparisons need to be done with tensors of the same type

      if epoch % 10 == 0:
            epoch_count.append(epoch)
            train_loss_values.append(loss.detach().numpy())
            test_loss_values.append(test_loss.detach().numpy())
            print(f"Epoch: {epoch} | Loss: {loss} | Test Loss: {test_loss} ")

"""
plt.plot(epoch_count, train_loss_values, label="Train loss")
plt.plot(epoch_count, test_loss_values, label="Test loss")
plt.title("Training and test loss curves")
plt.ylabel("Loss")
plt.xlabel("Epochs")
plt.legend();
plt.show();
"""
def save_model(model):
  MODEL_PATH = Path("models")
  MODEL_PATH.mkdir(parents = True, exist_ok = True)

  MODEL_NAME = "model.pth"
  MODEL_SAVE_PATH = MODEL_PATH / MODEL_NAME

  print(f"Saving model to: {MODEL_SAVE_PATH}")
  torch.save(obj = model.state_dict(),
             f = MODEL_SAVE_PATH)
  return MODEL_SAVE_PATH

def load_model(f):
  model = NeuralNetwork()
  print(f"Loading model from: {f}")
  return model.load_state_dict(torch.load(f).to(device))

#saved_path = save_model(model)
#new_model = load_model(saved_path)

