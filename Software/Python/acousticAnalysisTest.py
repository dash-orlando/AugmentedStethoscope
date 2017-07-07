

from acousticAnalysis   import browse_WAV
from acousticAnalysis   import read_WAV
from acousticAnalysis   import plot_WAV

#wav_file = browse_WAV()

filename = "C:\Users\WOLF512\Documents\Gits\PD3D\AugmentedStethoscope\Software\Sound Library\PD3D\Recordings 06222017\AT_resting_3_gain_34_06222017.wav"
wav_struct = read_WAV(filename)

plot_WAV(wav_struct)
