add_library(rtaudio
  "${CMAKE_CURRENT_LIST_DIR}/RtAudio.cpp"
)

target_include_directories(rtaudio
  PRIVATE "${CMAKE_CURRENT_LIST_DIR}"
  PUBLIC "${CMAKE_CURRENT_LIST_DIR}/.."
)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")

  target_compile_definitions(rtaudio
    PRIVATE -D__UNIX_JACK__
  )

  target_link_libraries(rtaudio
    INTERFACE jack
  )

endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")

  target_compile_definitions(rtaudio
    PRIVATE -D__MACOSX_CORE__
  )

  target_link_libraries(rtaudio
    INTERFACE "-framework CoreAudio"
    INTERFACE "-framework CoreFoundation"
  )

endif()
