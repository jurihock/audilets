import numpy as np
import matplotlib.pyplot as plot

from matplotlib.patches \
    import Circle

from numpy.polynomial.polynomial \
    import polyroots as polyroots

from numpy.polynomial.polynomial \
    import polyval as polyval

def zplot(z, p, axis):

    circle = Circle((0,0), radius=1, linestyle=':', linewidth=1, color='black', fill=False, alpha=0.3)
    axis.add_patch(circle)

    axis.axvline(0, linestyle=':', linewidth=1, color='black', alpha=0.3)
    axis.axhline(0, linestyle=':', linewidth=1, color='black', alpha=0.3)

    axis.plot(z.real, z.imag, 'o', markersize=10, color='none', markeredgecolor='blue', alpha=0.7)
    axis.plot(p.real, p.imag, 'x', markersize=10, color='red', alpha=0.7)

    axis.set_xlabel('re')
    axis.set_ylabel('im')

    r = 1.5 * np.amax(np.concatenate((np.abs(z), np.abs(p), [1])))
    axis.axis('scaled')
    axis.axis([-r, r, -r, r])

def roots(b, a):

    b = np.array(b) / a[0]
    a = np.array(a) / a[0]
    b /= b[0]
    z = polyroots(b)
    p = polyroots(a)

    return z, p

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

lp['z'], lp['p'] = roots(lp['b'], lp['a'])
hp['z'], hp['p'] = roots(hp['b'], hp['a'])
bp['z'], bp['p'] = roots(bp['b'], bp['a'])

figure, axes = plot.subplots(nrows=1, ncols=2)

figure.suptitle('Chamberlin \nFS={}Hz, FC={}Hz, \nQ={}'.format(FS, FC, Q))

axes[0].axvline(FC, linestyle='--', color='black', alpha=0.3)
axes[0].plot(lp['w'], abs(lp['h']), label='LP', color='tab:blue')
axes[0].plot(hp['w'], abs(hp['h']), label='HP', color='tab:green')
axes[0].plot(bp['w'], abs(bp['h']), label='BP', color='tab:red')
axes[0].set_xscale('log')
axes[0].set_xlabel('Frequency [Hz]')
axes[0].set_ylabel('Gain [dB]')
axes[0].legend()
axes[0].grid()

axes[1].axvline(FC, linestyle='--', color='black', alpha=0.3)
axes[1].plot(lp['w'], arg(lp['h']), label='LP', color='tab:blue')
axes[1].plot(hp['w'], arg(hp['h']), label='HP', color='tab:green')
axes[1].plot(bp['w'], arg(bp['h']), label='BP', color='tab:red')
axes[1].set_xscale('log')
axes[1].set_xlabel('Frequency [Hz]')
axes[1].set_ylabel('Phase [rad]')
axes[1].legend()
axes[1].grid()

figure, axes = plot.subplots(nrows=1, ncols=3)

figure.suptitle('Chamberlin \nFS={}Hz, FC={}Hz, \nQ={}'.format(FS, FC, Q))

zplot(lp['z'], lp['p'], axes[0])
zplot(hp['z'], hp['p'], axes[1])
zplot(bp['z'], bp['p'], axes[2])

axes[0].set_title('LP')
axes[1].set_title('HP')
axes[2].set_title('BP')

plot.show()
