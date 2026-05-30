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

# Optional FLTK UI
option(BUILD_FLTK "Build FLTK-based UI" OFF)
if(BUILD_FLTK)
  find_package(FLTK REQUIRED)
  add_executable(auction_skeleton_fltk
    src/main_fltk.cpp
    src/app.cpp
    src/ui_fltk.cpp
  )
  target_include_directories(auction_skeleton_fltk PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include ${FLTK_INCLUDE_DIR})
  target_link_libraries(auction_skeleton_fltk PRIVATE ${FLTK_LIBRARIES})
endif()
