add_library(kissfft
  "${CMAKE_CURRENT_LIST_DIR}/kiss_fft.c"
  "${CMAKE_CURRENT_LIST_DIR}/kiss_fftr.c"
)

target_include_directories(kissfft
  PUBLIC "${CMAKE_CURRENT_LIST_DIR}/.."
)

target_include_directories(kissfft
  PRIVATE "${CMAKE_CURRENT_LIST_DIR}"
)

target_compile_definitions(kissfft
  PRIVATE kiss_fft_scalar=float
)
