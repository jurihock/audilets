set(CMAKE_AUTOMOC ON)

add_library(qcustomplot
  "${CMAKE_CURRENT_LIST_DIR}/qcustomplot.cpp"
)

target_include_directories(qcustomplot
  PUBLIC "${CMAKE_CURRENT_LIST_DIR}/.."
)

target_include_directories(qcustomplot
  PRIVATE "${CMAKE_CURRENT_LIST_DIR}"
)

target_link_libraries(qcustomplot
  Qt5::Core
  Qt5::PrintSupport
  Qt5::Widgets
)

find_package(Qt5 REQUIRED COMPONENTS
  Core
  PrintSupport
  Widgets
)
