"""

FFT Analysis of Signals

Fluvio L Lobo Fenoglietto       06/28/2017          - Original build
"""

import numpy                as np
import matplotlib.pylab     as plt

# Data Sample
t0 = 0
tf = 1.5
fs = 1000
N_samples = tf*fs
x_s = np.linspace(1,tf*fs,tf*fs)
x = np.linspace(t0, tf, N_samples)
f1 = 20
f2 = 30
f3 = 40
y = 3*np.cos(2*np.pi*f1*x + 0.2) + np.cos(2*np.pi*f2*x - 0.3) + 2*np.cos(2*np.pi*f3*x + 2.4)
plt.plot(x, y)
plt.xlabel('x')
plt.ylabel('y')
plt.show()


# FFT

Y = np.fft.fft(y)
Y_mag = np.absolute(Y)
X_Hz = (x_s*fs)/(N_samples)
plt.plot(X_Hz,Y_mag)
plt.show()



"""
References

1 - Sin and Cos Funcions - https://docs.scipy.org/doc/numpy-1.10.4/reference/generated/numpy.sin.html
2 - FFT - https://docs.scipy.org/doc/numpy-1.12.0/reference/generated/numpy.fft.fft.html
3 - Absolute Value - https://docs.scipy.org/doc/numpy-1.11.0/reference/generated/numpy.absolute.html
"""
