add_executable(audilets-monitor
  "${CMAKE_CURRENT_LIST_DIR}/Monitor.cpp"
)

target_link_libraries(audilets-monitor
  audilets
)
