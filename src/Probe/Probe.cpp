#include <rtaudio/RtAudio.h>

#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include <vector>

const std::map<std::string, RtAudioFormat> all_data_formats
{
  { "8-bit int", RTAUDIO_SINT8 },
  { "16-bit int", RTAUDIO_SINT16 },
  { "24-bit int", RTAUDIO_SINT24 },
  { "32-bit int", RTAUDIO_SINT32 },
  { "32-bit float", RTAUDIO_FLOAT32 },
  { "64-bit float", RTAUDIO_FLOAT64 },
};

int main(int argc, char* argv[])
{
  RtAudio audio;

  const std::vector<std::string> column_keys =
  {
    "#",
    "Device Name",
    "Inputs",
    "Outputs",
    "Duplex",
    "Sample Rates",
    "Data Formats"
  };

  std::map<std::string, std::vector<std::string>> columns
  {
    { "#", {} },
    { "Device Name", {} },
    { "Inputs", {} },
    { "Outputs", {} },
    { "Duplex", {} },
    { "Sample Rates", {} },
    { "Data Formats", {} },
  };

  const size_t devices = audio.getDeviceCount();

  for (size_t index = 0; index < devices; ++index)
  {
    const RtAudio::DeviceInfo device = audio.getDeviceInfo(index);

    if (!device.probed)
    {
      continue;
    }

    const std::string name = device.name;
    const std::string inputs = std::to_string(device.inputChannels) + (device.isDefaultInput ? " *" : "");
    const std::string outputs = std::to_string(device.outputChannels) + (device.isDefaultOutput ? " *" : "");
    const std::string duplex = std::to_string(device.duplexChannels);
    std::vector<std::string> sample_rates(device.sampleRates.size());
    std::vector<std::string> data_formats;

    for (size_t i = 0; i < sample_rates.size(); ++i)
    {
      sample_rates[i] = std::to_string(device.sampleRates[i]);

      if (device.sampleRates[i] == device.preferredSampleRate)
      {
        sample_rates[i] += " *";
      }
    }

    for (const auto& data_format : all_data_formats)
    {
      if (data_format.second & device.nativeFormats)
      {
        data_formats.push_back(data_format.first);
      }
    }

    const size_t offset = columns[column_keys.front()].size() + 1;

    const size_t total_rows = std::max(
    {
      sample_rates.size(), data_formats.size(), (size_t)1
    });

    for (const auto& [column, rows] : columns)
    {
      columns[column].resize(offset + total_rows);
    }

    columns["#"][offset - 1] = "-";
    columns["#"][offset] = std::to_string(index);
    columns["Device Name"][offset] = name;
    columns["Inputs"][offset] = inputs;
    columns["Outputs"][offset] = outputs;
    columns["Duplex"][offset] = duplex;

    for (size_t i = 0; i < sample_rates.size(); ++i)
    {
      columns["Sample Rates"][offset + i] = sample_rates[i];
    }

    for (size_t i = 0; i < data_formats.size(); ++i)
    {
      columns["Data Formats"][offset + i] = data_formats[i];
    }
  }

  const size_t total_rows = columns[column_keys.front()].size();
  size_t total_column_width = 0;
  std::map<std::string, size_t> column_widths;

  for (const auto& column : column_keys)
  {
    size_t column_width = column.size();

    for (const auto& row : columns[column])
    {
      column_width = std::max(column_width, row.size());
    }

    column_widths[column] = column_width + 5;
    total_column_width += column_widths[column];
  }

  for (const auto& column : column_keys)
  {
    std::cout << std::setw(column_widths[column]) << std::left << column;

    if (column == column_keys.back())
    {
      std::cout << std::endl;
    }
  }

  for (size_t i = 0; i < total_rows; ++i)
  {
    if (columns[column_keys.front()][i] == "-")
    {
      std::cout << std::string(total_column_width, '-');
    }
    else
    {
      for (const auto& column : column_keys)
      {
        std::cout << std::setw(column_widths[column]) << std::left << columns[column][i];
      }
    }
    std::cout << std::endl;
  }

  return 0;
}
