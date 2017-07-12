"""

Acoustic Signal Level Shifting

Algorithms designed to alter signal gain to statically or dinamically maintain the amplitude or power of parallel signals

Fluvio L Lobo Fenoglietto
07/11/2017
"""

# Import
import numpy as np

# ======================================= #
# GAIN
# ======================================= #
def WAV_GAIN(input_signal, gain):
    mod_signal = np.multiply(input_signal, float(gain))
    return mod_signal

# ======================================= #
# RMS
# ======================================= #
def WAV_RMS(wav_signal):
    wav_RMS = np.sqrt(np.mean(wav_signal**2))
    return wav_RMS

# ======================================= #
# RMS-based Level Shifting
# ======================================= #
def RMS_LVL_SHIFT(ref_signal, input_signal, step, tol):

    # Calculates the RMS of the reference signal
    ref_signal_rms = WAV_RMS(ref_signal)

    # Calculates the RMS of the input signal
    input_signal_rms = WAV_RMS(input_signal)

    rms_flag = 0
    ref_signal_gain = 1
    input_signal_gain = 1
    #step = 0.10
    #tol = 100
    while rms_flag == 0:
        
        # Compare reference and input signal RMS
        if input_signal_rms < (ref_signal_rms - tol):
            # increase input signal gain
            input_signal_gain = input_signal_gain + step
            # update the input signal
            input_signal = WAV_GAIN(input_signal, input_signal_gain)
            #update the rms value of the input signal
            input_signal_rms = WAV_RMS(input_signal)
            print "increasing"
            
        elif input_signal_rms > (ref_signal_rms + tol):
            # decrease input signal gain
            input_signal_gain = input_signal_gain - step
            # update the input signal
            input_signal = WAV_GAIN(input_signal, input_signal_gain)
            #update the rms value of the input signal
            input_signal_rms = WAV_RMS(input_signal)
            print "decreasing"

        elif input_signal_rms >= (ref_signal_rms - tol) or input_signal_rms <= (ref_signal_rms + tol):
            rms_flag = 1
            print "done"

    return input_signal
