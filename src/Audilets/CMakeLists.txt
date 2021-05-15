add_library(audilets
  "${CMAKE_CURRENT_LIST_DIR}/DSP/Algorithms/BernseeVocoder.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/DSP/Algorithms/SinglePitchShift.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/DSP/FFT.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/DSP/Monitor.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/DSP/SineWave.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/DSP/STFT.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/DSP/Window.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/GUI/MultiPlotWindow.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/IO/AudioSource.cpp")

target_include_directories(audilets
  PUBLIC "${CMAKE_CURRENT_LIST_DIR}/.."
)

target_link_libraries(audilets
  kissfft
  pthread
  qcustomplot
  readerwriterqueue
  rtaudio
)

target_compile_features(audilets
  PUBLIC cxx_std_17
)
