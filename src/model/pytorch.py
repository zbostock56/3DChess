import torch
from torch import nn # neural networks
from torch import optim # optimizers
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

# Check PyTorch Version
print(f"PyTorch Version: {torch.__version__}")

device = (
    "cuda"
    if torch.cuda.is_available()
    else "mps"
    if torch.backends.mps.is_available()
    else "cpu"
)
print(f"Using {device} device")

# Create Data
weight = 0.7
bias = 0.3
start = 0
end = 1
step = 0.0001
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
  plt.figure(figsize = (10, 7))

  # Plot training data in blue
  plt.scatter(train_data, train_labels, c = "b", s = 4, label = "Training data")

  # Plot testing data in green
  plt.scatter(test_data, test_labels, c = "g", s = 4, label= "Testing data")

  if predictions is not None:
    # Plot the predictions
    plt.scatter(test_data, predictions, c="r", s = 4, label = "Predictions")

  plt.legend(prop = {"size": 14})
  plt.show()

class NeuralNetwork(nn.Module):
  # Constuctor
  def __init__(self):
    super().__init__()

    # Model Parameters
    # requires_grad = true =====> use gradient descent
    self.weights = nn.Parameter(torch.randn(1,
                                            requires_grad = True,
                                            dtype = torch.float32))
    self.bias = nn.Parameter(torch.randn(1,
                                         requires_grad = True,
                                         dtype = torch.float32))

    # Define the computation in the model
  def forward(self, x: torch.Tensor) -> torch.Tensor:
    return self.weights * x + self.bias

# Create a manual random seed
torch.manual_seed(42)

# Instance of the model
model = NeuralNetwork()

# Make predictions with model
with torch.inference_mode():
  y_preds = model(X_test)

print(y_preds)

plot_predictions(predictions=y_preds);
