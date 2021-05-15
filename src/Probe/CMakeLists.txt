add_executable(audilets-probe
  "${CMAKE_CURRENT_LIST_DIR}/Probe.cpp"
)

target_link_libraries(audilets-probe
  audilets
)
