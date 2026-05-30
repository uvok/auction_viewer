cmake_minimum_required(VERSION 3.15)
project(auction_viewer_skeleton LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(auction_skeleton
  src/main.cpp
  src/app.cpp
  src/ui_console.cpp
)

target_include_directories(auction_skeleton PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
