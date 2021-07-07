import numpy as np
import matplotlib.pyplot as plot

from numpy.polynomial.polynomial \
    import polyval as polyval

def freqz(b, a, fs, fn):

    w = np.logspace(np.log10(1), np.log10(fs / 2), fn, base=10)
    w = (2 * np.pi) * (w / fs)
    z = np.exp(-1j * w)
    h = polyval(z, b, tensor=False) / polyval(z, a, tensor=False)
    w = (w * fs) / (2 * np.pi)

    return w, h

def abs(_): return 20 * np.log10(np.abs(_))
def arg(_): return np.unwrap(np.angle(_))

FC = 100
FS = 44100
FN = 1024

F = 2 * np.sin(np.pi * FC / FS)
Q = 1 / np.sqrt(2)

f = F
q = 1 - f / Q

lp = { 'b': [f**2],     'a': [1, f**2 - q - 1, q] }
hp = { 'b': [1, -2, 1], 'a': [1, f**2 - q - 1, q] }
bp = { 'b': [f, -f],    'a': [1, f**2 - q - 1, q] }

lp['w'], lp['h'] = freqz(b=lp['b'], a=lp['a'], fs=FS, fn=FN)
hp['w'], hp['h'] = freqz(b=hp['b'], a=hp['a'], fs=FS, fn=FN)
bp['w'], bp['h'] = freqz(b=bp['b'], a=bp['a'], fs=FS, fn=FN)

figure, axes = plot.subplots(nrows=1, ncols=2)

figure.suptitle('Chamberlin \nFS={}Hz, FC={}Hz, \nQ={}'.format(FS, FC, Q))

axes[0].axvline(FC, linestyle='--', color='black', alpha=0.3)
axes[0].plot(lp['w'], abs(lp['h']), label='LP')
axes[0].plot(hp['w'], abs(hp['h']), label='HP')
axes[0].plot(bp['w'], abs(bp['h']), label='BP')
axes[0].set_xscale('log')
axes[0].set_xlabel('Frequency [Hz]')
axes[0].set_ylabel('Gain [dB]')
axes[0].legend()
axes[0].grid()

axes[1].axvline(FC, linestyle='--', color='black', alpha=0.3)
axes[1].plot(lp['w'], arg(lp['h']), label='LP')
axes[1].plot(hp['w'], arg(hp['h']), label='HP')
axes[1].plot(bp['w'], arg(bp['h']), label='BP')
axes[1].set_xscale('log')
axes[1].set_xlabel('Frequency [Hz]')
axes[1].set_ylabel('Phase [rad]')
axes[1].legend()
axes[1].grid()

plot.show()
