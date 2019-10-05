add_executable(audilets-sandbox
  "${CMAKE_CURRENT_LIST_DIR}/Sandbox.cpp"
)

target_link_libraries(audilets-sandbox
  audilets
)
