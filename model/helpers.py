import torch
import matplotlib.pyplot as plt
from pathlib import Path # Save/Load Model

def plot_predictions(train_data,
                     train_labels,
                     test_data,
                     test_labels,
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

# Calculate accuracy
def accuracy_fn(y_true, y_pred):
  correct = torch.eq(y_true, y_pred).sum().item()
  acc = (correct / len(y_pred)) * 100
  return acc

# Time runtime
def print_train_time(start: float,
                     end: float,
                     device: torch.device):
  """
    Prints the difference between the start and end time

    Args:
      start (float): start time of computation
      end (float): End time of computation
      device ([type]): device that compute operation is running on

    Returns:
      float: time between start and end in seconds
  """
  total_time = end - start
  print(f"Train time on {device}: {total_time:.3f} seconds")
  return total_time
