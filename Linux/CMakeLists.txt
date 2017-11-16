
cmake_minimum_required(VERSION 2.8)

if(POLICY CMP0015)
  cmake_policy(SET CMP0015 OLD)
endif()

link_directories(./)

add_subdirectory(LightInkLLM)

