"""

Implementing Standard Signal Analysis MEthods

Fluvio L Lobo Fenoglietto       06/28/2017          - Original build
"""

import      numpy               as      np
import      matplotlib.pylab    as      plt
from        scipy               import  signal
"""
    if scipy is not present;
    >>> python -m pip install -U scipy
"""


np.random.seed(1234)
#Generate a test signal, a 2 Vrms sine wave at 1234 Hz, corrupted by 0.001 V**2/Hz of white noise sampled at 10 kHz.

fs = 10e3
N = 1e5
amp = 2*np.sqrt(2)
freq = 1234.0
noise_power = 0.001 * fs / 2
time = np.arange(N) / fs
x = amp*np.sin(2*np.pi*freq*time)
x += np.random.normal(scale=np.sqrt(noise_power), size=time.shape)
#Compute and plot the power spectral density.

f, Pxx_den = signal.welch(x, fs, nperseg=1024)
plt.semilogy(f, Pxx_den)
plt.ylim([0.5e-3, 1])
plt.xlabel('frequency [Hz]')
plt.ylabel('PSD [V**2/Hz]')
plt.show()
