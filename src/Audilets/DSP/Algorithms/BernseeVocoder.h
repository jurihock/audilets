#pragma once

#include <Audilets/DSP/Math.h>

namespace audilets::dsp::algorithms
{
  /**
   * Pitch Shifting Using The Fourier Transform (Version 1.2)
   * http://blogs.zynaptiq.com/bernsee/pitch-shifting-using-the-ft/
   *
   * Copyright by Stephan M. Bernsee
   *
   * The Wide Open License (WOL)
   *
   * Permission to use, copy, modify, distribute and sell this software and its
   * documentation for any purpose is hereby granted without fee, provided that
   * the above copyright notice and this license appear in all source copies.
   * THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT EXPRESS OR IMPLIED WARRANTY OF
   * ANY KIND. See http://www.dspguru.com/wol.htm for more information.
   */
  class BernseeVocoder
  {
  public:

    BernseeVocoder(const size_t frameSampleRate, const size_t frameSize, const size_t frameHopRate);
    ~BernseeVocoder();

    /**
     * Encodes the specified FFT spectrum as (magnitude,frequency) tuples.
     */
    void analyze(std::complex<float>* fft);

    /**
     * Decodes the specified (magnitude,frequency) tuples as regular FFT spectrum.
     */
    void synthesize(std::complex<float>* fft);

  private:

    const size_t frameSampleRate;
    const size_t frameSize;
    const size_t frameHalfSize;
    const size_t frameHopRate;

    const float stftFreqInc;
    const float stftFreqIncInv;
    const float stftPhaseInc;
    const float stftPhaseIncInv;

    float* backedPhaseBuffer;
    float* accumPhaseBuffer;
  };
}
