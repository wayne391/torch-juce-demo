import torch
from torch import nn

import time
import soundfile as sf

from utils import convert_tensor_to_numpy


class Distortion(nn.Module):
    def __init__(self):
        super(Distortion, self).__init__()
        self.name = 'distortion'
        pass

    def forward(self, tensor_in):
        x = torch.clamp(tensor_in, min=-0.5, max=0.5) 
        x = torch.abs(x)
        return x


class Gain(nn.Module):
    def __init__(self):
        super(Gain, self).__init__()
        self.name = 'gain'
        self.gain = 0.01

    def forward(self, tensor_in):
        x = tensor_in * self.gain 
        return x


class Bypass(nn.Module):
    def __init__(self):
        super(Bypass, self).__init__()
        self.name = 'bypass'

    def forward(self, tensor_in):
        return tensor_in


if __name__ == '__main__':
    # model
    core = Bypass()

    # jit 
    jitted_model = torch.jit.script(core)
    path_jit = core.name + '.pt'
    jitted_model.save(path_jit)
