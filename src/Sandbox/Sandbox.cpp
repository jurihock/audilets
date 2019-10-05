#include <chrono>
#include <condition_variable>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

#include <QApplication>

#include <Audilets/DSP/Algorithms/SinglePitchShift.h>
#include <Audilets/DSP/FFT.h>
#include <Audilets/DSP/Monitor.h>
#include <Audilets/DSP/SineWave.h>
#include <Audilets/DSP/STFT.h>
#include <Audilets/GUI/MultiPlotWindow.h>

using namespace Audilets::DSP::Algorithms;
using namespace Audilets::DSP;
using namespace Audilets::GUI;

std::mutex appMutex;
std::condition_variable appSignal;

std::vector<short> buffer(2*1024);
SineWave sine(44100, 512);
SinglePitchShift stft(44100, buffer.size() / 2, 4);
Monitor monitor(44100, buffer.size());

std::shared_ptr<MultiPlotWindow> plot;

void background()
{
  // sync
  {
    std::unique_lock<std::mutex> lock(appMutex);

    const auto timeout = std::chrono::seconds(5);

    if (appSignal.wait_for(lock, timeout) == std::cv_status::timeout)
    {
      return;
    }
  }

  while (true)
  {
    // sync
    {
      std::unique_lock<std::mutex> lock(appMutex);


      std::vector<float> magnitudes;
      std::vector<float> frequencies;

      sine.getNextValue(buffer.data(), buffer.size(), (short)1000);
      //std::fill(buffer.begin(), buffer.end(), 1000);

      monitor.magnitudes(buffer.data(), magnitudes);
      monitor.frequencies(buffer.data(), frequencies);
      Math::decibel(magnitudes.begin(), magnitudes.end());
      plot->setPlotData<0, 0, 0>(buffer);
      plot->setPlotData<1, 0, 0>(frequencies, magnitudes);

      stft.setPitchShiftingFactor(0.5);
      stft.stft(buffer.data(), buffer.data());
      stft.stft(buffer.data() + buffer.size() / 2, buffer.data() + buffer.size() / 2);

      monitor.magnitudes(buffer.data(), magnitudes);
      monitor.frequencies(buffer.data(), frequencies);
      Math::decibel(magnitudes.begin(), magnitudes.end());
      plot->setPlotData<0, 0, 1>(buffer);
      plot->setPlotData<1, 0, 1>(frequencies, magnitudes);

      plot->replot<0, 0>();
      plot->replot<1, 0>();

      // const auto mean = Math::mean(buffer.begin(), buffer.end());
      // const auto stdev = Math::stdev(buffer.begin(), buffer.end(), mean);
      // std::cout << "mean=" << mean << " stdev=" << stdev << std::endl;
    }

    // sync
    {
      std::unique_lock<std::mutex> lock(appMutex);

      const auto timeout = std::chrono::milliseconds(1000);

      if (appSignal.wait_for(lock, timeout) == std::cv_status::no_timeout)
      {
        return;
      }
    }
  }
}

int foreground(int argc, char* argv[])
{
  QApplication application(argc, argv);

  plot = std::make_shared<MultiPlotWindow>();

  plot->addPlot<0, 0>(2);
  plot->setPlotRangeY<0, 0>(-1000, +1000);
  plot->setPlotRangeX<0, 0>(0, buffer.size());

  plot->addPlot<1, 0>(2);
  plot->setPlotRangeY<1, 0>(0, 20);
  plot->setPlotRangeX<1, 0>(0, 44100 / 2);
  plot->setPlotLabelX<1, 0>("Hz");
  plot->setPlotLabelY<1, 0>("dB");

  plot->show();

  // sync
  {
    std::unique_lock<std::mutex> lock(appMutex);
    appSignal.notify_all();
  }

  const int result = application.exec();

  // sync
  {
    std::unique_lock<std::mutex> lock(appMutex);
    appSignal.notify_all();
  }

  return result;
}

int main(int argc, char* argv[])
{
  std::thread thread(background);

  const int result = foreground(argc, argv);

  thread.join();

  return result;
}
