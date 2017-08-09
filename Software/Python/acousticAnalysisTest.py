

from acousticAnalysis   import *
from acoustic_level_shifting import *

#wav_file = browse_WAV()

filename = "C:\Users\WOLF512\Documents\Gits\PD3D\AugmentedStethoscope\Software\Sound Library\PD3D\Recordings 06222017\AT_resting_3_gain_34_06222017.wav"
wav_struct = read_WAV(filename)

plot_WAV(wav_struct)


wav_amp, mod_amp = mod_WAV_gain(wav_struct,2.0)

wav_rms = WAV_RMS(wav_amp)
mod_rms = WAV_RMS(mod_amp)

fixed_mod_signal = RMS_LVL_SHIFT(wav_amp, mod_amp, 0.10, 50)


