set(CMAKE_AUTOMOC ON)

add_library(qcustomplot
  "${CMAKE_CURRENT_LIST_DIR}/qcustomplot.cpp"
)

target_include_directories(qcustomplot
  PRIVATE "${CMAKE_CURRENT_LIST_DIR}"
  PUBLIC "${CMAKE_CURRENT_LIST_DIR}/.."
)

target_link_libraries(qcustomplot
  Qt6::Core
  Qt6::PrintSupport
  Qt6::Widgets
)

find_package(Qt6 REQUIRED COMPONENTS
  Core
  PrintSupport
  Widgets
)
