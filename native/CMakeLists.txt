cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)

project (RC-CAR-WebControl)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


find_package (Threads)
# Video Server build
find_package( OpenCV REQUIRED )
add_executable(VideoServer src/video_server_main.cpp
src/video/Capture.cpp src/video/Capture.hpp
src/tcp/TcpServer.cpp src/tcp/TcpServer.hpp
src/tcp/TcpSocket.cpp src/tcp/TcpSocket.hpp
src/misc/Clock.hpp)
target_link_libraries( VideoServer ${OpenCV_LIBS} ${CMAKE_THREAD_LIBS_INIT})
target_compile_features(VideoServer PRIVATE cxx_delegating_constructors)
