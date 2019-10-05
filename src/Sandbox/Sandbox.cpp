#include <chrono>
#include <condition_variable>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

#include <QApplication>

#include <Audilets/DSP/SineWave.h>
#include <Audilets/DSP/FFT.h>
#include <Audilets/DSP/STFT.h>
#include <Audilets/DSP/Algorithms/SinglePitchShift.h>
#include <Audilets/GUI/MultiPlotWindow.h>

using namespace Audilets::DSP;
using namespace Audilets::DSP::Algorithms;
using namespace Audilets::GUI;

std::mutex appMutex;
std::condition_variable appSignal;

SineWave sine(44100, 512);
std::vector<short> buffer(2*1024);

//STFT stft(44100, buffer.size()/2, 4);
SinglePitchShift stft(44100, buffer.size()/2, 4);

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

      sine.getNextValue(buffer.data(), buffer.size(), (short)1000);
      //std::fill(buffer.begin(), buffer.end(), 1000);

      stft.setPitchShiftingFactor(0.5);

      stft.stft(buffer.data(), buffer.data());
      stft.stft(buffer.data() + buffer.size() / 2, buffer.data() + buffer.size() / 2);

      const auto mean = Math::mean(buffer.begin(), buffer.end());
      const auto stdev = Math::stdev(buffer.begin(), buffer.end(), mean);
      std::cout << "mean=" << mean << " stdev=" << stdev << std::endl;

      plot->setPlotData<0, 0>(buffer);
      plot->replot<0, 0>();
    }

    // sync
    {
      std::unique_lock<std::mutex> lock(appMutex);

      const auto timeout = std::chrono::milliseconds(100);

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
  plot->addPlot<0, 0>();
  plot->setPlotRangeY<0, 0>(-1000, +1000);
  plot->setPlotRangeX<0, 0>(0, buffer.size());
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
