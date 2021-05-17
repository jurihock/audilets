cmake_minimum_required(VERSION 3.5)

project(Audilets)

# lib
include("lib/dr/CMakeLists.txt")
include("lib/kissfft/CMakeLists.txt")
include("lib/qcustomplot/CMakeLists.txt")
include("lib/readerwriterqueue/CMakeLists.txt")
include("lib/rtaudio/CMakeLists.txt")
include("lib/rtmidi/CMakeLists.txt")

# src
include("src/Audilets/CMakeLists.txt")
include("src/Monitor/CMakeLists.txt")
include("src/Probe/CMakeLists.txt")
