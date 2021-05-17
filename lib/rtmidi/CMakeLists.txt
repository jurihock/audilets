add_library(rtmidi
  "${CMAKE_CURRENT_LIST_DIR}/RtMidi.cpp"
)

target_include_directories(rtmidi
  PRIVATE "${CMAKE_CURRENT_LIST_DIR}"
  PUBLIC "${CMAKE_CURRENT_LIST_DIR}/.."
)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")

  target_compile_definitions(rtmidi
    PRIVATE -D__UNIX_JACK__
  )

  target_link_libraries(rtmidi
    INTERFACE jack
  )

endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")

  target_compile_definitions(rtmidi
    PRIVATE -D__MACOSX_CORE__
  )

  target_link_libraries(rtmidi
    INTERFACE "-framework CoreMIDI"
  )

endif()
