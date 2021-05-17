# Audilets

The Audilets is a kind of successor to the [Voicesmith](https://github.com/jurihock/voicesmith) project.
There is currently no specific product idea behind this, but the intention to rethink past things and discover new ones.

The main focus should be on frequency domain processing according to the STFT.
Another interesting thing I found is the Stephan M. Bernsee technique described in
[Pitch Shifting Using The Fourier Transform](http://blogs.zynaptiq.com/bernsee/pitch-shifting-using-the-ft),
which could also be useful in general DAFX.

Sometimes I'm also thinking about such fancy stuff like pitch tracking and replacing the synthesis step by the MIDI output.
Perhaps to control further parameters by playing a musical instrument or just forward to a MIDI expander. So why not?

I have no plans to go back to the Android OS, but to go easy with the [RtAudio](https://github.com/thestk/rtaudio) and C++, and be more platform independent.
Otherwise the integration of development boards like [Teensy](https://www.pjrc.com/teensy) could be potentially a subject in case of a concrete idea.
