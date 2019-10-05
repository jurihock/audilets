add_library(rtaudio
  "${CMAKE_CURRENT_LIST_DIR}/RtAudio.cpp"
)

target_include_directories(rtaudio
  PUBLIC "${CMAKE_CURRENT_LIST_DIR}/.."
)

target_include_directories(rtaudio
  PRIVATE "${CMAKE_CURRENT_LIST_DIR}"
)

target_compile_definitions(rtaudio
  PRIVATE -D__UNIX_JACK__
)

target_link_libraries(rtaudio
  INTERFACE jack
)
