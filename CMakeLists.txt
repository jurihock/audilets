cmake_minimum_required(VERSION 3.5)

project(Audilets)

# lib
include("lib/kissfft/CMakeLists.txt")
include("lib/qcustomplot/CMakeLists.txt")
include("lib/rtaudio/CMakeLists.txt")

# src
include("src/Audilets/CMakeLists.txt")
include("src/Sandbox/CMakeLists.txt")
