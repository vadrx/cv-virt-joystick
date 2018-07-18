cmake_minimum_required(VERSION 3.0.0)
project(vidcv)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)

# Add Packages
find_package(OpenCV REQUIRED)

file(GLOB sources
    "src/*.h"
    "src/*.cpp"
)

add_executable(${PROJECT_NAME} ${sources})
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})