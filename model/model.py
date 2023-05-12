import torch
import numpy as np
from torch import nn
from torch.utils.data import Dataset

class NeuralNetwork(nn.Module):
  # Constuctor
  def __init__(self, input_features, output_features, hidden_units = 64):
    """
      Args:
        input_features (int): number of input features of the model
        output_features (int): number of output features of the model
        hidden_units (int): Number of hidden units between layers, default 64
    """
    super().__init__()

    # Model Parameters
    self.linear_layers = nn.Sequential(
      nn.Linear(in_features = input_features, out_features = hidden_units),
      nn.ReLU(),
      nn.Linear(in_features = hidden_units, out_features = hidden_units),
      nn.ReLU(),
      nn.Linear(in_features = hidden_units, out_features = hidden_units),
      nn.ReLU(),
      nn.Linear(in_features = hidden_units, out_features = hidden_units),
      nn.ReLU(),
      nn.Linear(in_features = hidden_units, out_features = output_features)
    )

    # Define the computation in the model
  def forward(self, x):
    return self.linear_layers(x)

class CustomDataset(Dataset):
  def __init__(self, samples, labels, device):
    self.samples = torch.from_numpy(
      np.array(
        samples,
        dtype = np.single
      )
    ).to(device)
    self.labels = torch.from_numpy(
      np.array(
        labels,
        dtype = np.single
      )
    ).to(device)

  def __len__(self):
    return len(self.samples)

  def __getitem__(self,idx):
    chess_poss = self.samples[idx]
    label = self.labels[idx]
    return chess_poss, label
