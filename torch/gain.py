import torch
from torch import nn

import time
import soundfile as sf


def convert_tensor_to_numpy(tensor, is_squeeze=True):
    if is_squeeze:
        tensor = tensor.squeeze()
    if tensor.requires_grad:
        tensor = tensor.detach()
    if tensor.is_cuda:
        tensor = tensor.cpu()
    return tensor.numpy()


class Net(nn.Module):
    def __init__(self):
        super(Net, self).__init__()
        pass

    def forward(self, tensor_in, gain):
        return tensor_in * gain

if __name__ == '__main__':
    # model
    core = Net()

    # jit 
    jitted_model = torch.jit.script(core)
    path_jit = 'gain.pt'
    jitted_model.save(path_jit)

    # load test audio
    wav, sr = sf.read('test_16k.wav')
    print('wav info:', wav.shape, sr)

    # run
    device = 'cpu'
    start_time = time.time()
    y = torch.from_numpy(wav).float().to(device)
    y_out = core(y, 0.01)
    print('runtime:', time.time() - start_time)
    print('y_out:', y_out.shape)
    y_out = convert_tensor_to_numpy(y_out)
    
    # output
    sf.write('test_16k_out.wav', y_out, sr)
    