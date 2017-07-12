"""


"""

from        scipy.io.wavfile        import  read

import      Tkinter
import      tkFileDialog
import      numpy                   as      np
import      matplotlib.pyplot       as      plt




# Browse WAV file
def browse_WAV():
    root = Tkinter.Tk()
    file = tkFileDialog.askopenfile(parent=root,mode='rb',title='Choose a file')
    #if file != None:
    #    data = file.read()
    #    file.close()
    #    #print "I got %d bytes from this file." % len(data)
    root.destroy()
    #return file, data
    return file


# Read WAV file
def read_WAV(wav_file):
    wav_struct = dict()                                                                     # data structure
    raw_data = read(wav_file)                                                               # reading the wav file
    wav_struct['raw'] = raw_data                                                            # store raw output from the read() function
    wav_struct['fs'] = raw_data[0]                                                          # sampling rate (Hz)
    wav_struct['amp'] = np.array(raw_data[1],dtype=float)                                   # amplitude (signal) 
    wav_struct['n_samples'] = len(raw_data[1])                                              # number of samples in signal
    wav_struct['samples'] = np.linspace(1,len(raw_data[1]),len(raw_data[1]))
    wav_struct['tf'] = len(raw_data[1])/float(raw_data[0])                                  # time length of recording
    wav_struct['dt'] = 1./raw_data[0]                                                       # time interval
    wav_struct['t'] = np.linspace(0,len(raw_data[1])/float(raw_data[0]),len(raw_data[1]))   # approximation of time array (using linespace)

    return wav_struct

# Plot WAV file
def plot_WAV(wav_struct):                                   

    plt.subplot(211)                                                                        # widescreen, top plot
    plt.plot(wav_struct['samples'],wav_struct['amp'],color='b',linewidth=0.25)                                    # plot in sample domain
    plt.xlabel('Samples')                                                                   # x-label
    plt.ylabel('Amplitude')                                                                 # y-label
    plt.subplot(212)                                                                        # widescreen, bottom plot
    plt.plot(wav_struct['t'],wav_struct['amp'],color='r',linewidth=0.25)                    # plot in time domain
    plt.xlabel('Time [sec.]')
    plt.ylabel('Amplitude')
    plt.show()                                                                              # show plot

# Signal Gain Mod.
#   - Modify the signal amplitude using a gain
def mod_WAV_gain(wav_struct,gain):

    wav_amp = wav_struct['amp']
    mod_amp = np.multiply(wav_amp,float(gain))
    samples = wav_struct['samples']
    time = wav_struct['t']

    plt.subplot(211)                                                                        # widescreen, top plot
    plt.plot(samples,wav_amp,linestyle='-',color='b',linewidth=0.50)
    plt.plot(samples,mod_amp,linestyle='-',color='r',linewidth=0.25)
    plt.xlabel('Samples')                                                                   # x-label
    plt.ylabel('Amplitude')                                                                 # y-label
    plt.subplot(212)                                                                        # widescreen, bottom plot
    plt.plot(time,wav_amp,linestyle='-',color='b',linewidth=0.50)
    plt.plot(time,mod_amp,linestyle='-',color='r',linewidth=0.25)
    plt.xlabel('Time [sec.]')
    plt.ylabel('Amplitude')

    plt.show()
    
    return wav_amp, mod_amp
    

# Signal RMS
#   - Calculates the RMS of the input signal
def WAV_RMS(wav_amp):

    wav_RMS = np.sqrt(np.mean(wav_amp**2))

    return wav_RMS
        

"""
References
1 - WAV file import and read - https://docs.scipy.org/doc/scipy-0.19.0/reference/generated/scipy.io.wavfile.read.html
2 - Data plotting - https://matplotlib.org/users/pyplot_tutorial.html
"""
