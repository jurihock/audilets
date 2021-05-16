#include <Audilets/IO/AudioSource.h>

#include <QApplication>
#include <QMainWindow>

#include <Audilets/DSP/Monitor.h>
#include <Audilets/UI/QPlotWidget.h>

#include <iostream>
#include <chrono>
#include <thread>

using namespace Audilets::DSP;
using namespace Audilets::DSP::Algorithms;
using namespace Audilets::IO;
using namespace Audilets::UI;

const std::string audio_device_name = "apple";
const size_t frame_sample_rate = 48000;
const size_t frame_size = 4 * 1024;
const size_t frame_buffer_size = 10;

AudioSource source(audio_device_name, frame_sample_rate, frame_size, frame_buffer_size);
Monitor monitor(frame_sample_rate, frame_size);
std::vector<float> last_frame(frame_size);
std::shared_ptr<QPlotWidget> plot;

volatile bool $continue;
std::condition_variable $signal;
std::mutex $mutex;

void acquire()
{
  // wait for the start signal
  {
    std::unique_lock<std::mutex> lock($mutex);

    const auto timeout = std::chrono::seconds(5);

    if ($signal.wait_for(lock, timeout) == std::cv_status::timeout)
    {
      return;
    }
  }

  source.open();
  source.start();

  std::this_thread::sleep_for(
    5 * source.frame_sample_period());

  while ($continue)
  {
    source.read([&](const std::vector<float> frame)
    {
      std::unique_lock<std::mutex> lock($mutex);
      last_frame = frame;
    });
  }

  source.stop();
  source.close();
}

void analyze()
{
  // wait for the start signal
  {
    std::unique_lock<std::mutex> lock($mutex);

    const auto timeout = std::chrono::seconds(5);

    if ($signal.wait_for(lock, timeout) == std::cv_status::timeout)
    {
      return;
    }
  }

  std::this_thread::sleep_for(
    std::chrono::milliseconds(750));

  while ($continue)
  {
    std::this_thread::sleep_for(
      std::chrono::milliseconds(250));

    std::vector<float> frame;
    {
      std::unique_lock<std::mutex> lock($mutex);
      frame = last_frame;
    }

    std::vector<short> buffer(frame.size());

    for (size_t i = 0; i < buffer.size(); ++i)
    {
      buffer[i] = static_cast<short>(frame[i] * std::numeric_limits<short>::max());
    }

    std::vector<float> milliseconds;
    std::vector<float> frequencies;
    std::vector<float> magnitudes;

    monitor.milliseconds(milliseconds);
    monitor.frequencies(buffer.data(), frequencies);
    monitor.magnitudes(buffer.data(), magnitudes);
    Math::decibel(magnitudes.begin(), magnitudes.end());

    //std::cout << " " << frequencies[0] << " " << magnitudes[0] << std::endl;

    plot->setPlotData<0, 0>(milliseconds, frame);
    plot->setPlotData<1, 0>(frequencies, magnitudes);

    plot->replot<0, 0>();
    plot->replot<1, 0>();

    // const auto mean = Math::mean(buffer.begin(), buffer.end());
    // const auto stdev = Math::stdev(buffer.begin(), buffer.end(), mean);
    // std::cout << "mean=" << mean << " stdev=" << stdev << std::endl;
  }
}

int exec(int argc, char* argv[])
{
  QApplication application(argc, argv);

  plot = std::make_shared<QPlotWidget>();

  plot->addPlot<0, 0>(1);
  plot->setPlotLabelX<0, 0>("ms");
  plot->setPlotRangeY<0, 0>(-1, +1);
  plot->setPlotLabelY<0, 0>(" ");
  plot->setPlotRangeX<0, 0>(0, source.frame_sample_period().count());

  plot->addPlot<1, 0>(1);
  plot->setPlotLabelX<1, 0>("Hz");
  plot->setPlotRangeX<1, 0>(0, 2500);
  plot->setPlotLabelY<1, 0>("dB");
  plot->setPlotRangeY<1, 0>(-60, +30);

  plot->resize(800, 600);

  plot->show();

  $continue = true;
  $signal.notify_all();

  const int result = application.exec();

  $continue = false;

  return result;
}

int main(int argc, char* argv[])
{
  std::thread thread1(acquire);
  std::thread thread2(analyze);

  const int result = exec(argc, argv);

  thread2.join();
  thread1.join();

  return result;
}
