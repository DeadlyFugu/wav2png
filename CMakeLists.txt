cmake_minimum_required(VERSION 2.6)
project(wav2png)

set(CMAKE_C_STANDARD 99)

set(W2P_SOURCE_FILES wav2png.c)
add_executable(wav2png ${W2P_SOURCE_FILES})

set(P2W_SOURCE_FILES png2wav.c)
add_executable(png2wav ${P2W_SOURCE_FILES})

if(UNIX AND NOT APPLE)
    target_link_libraries(wav2png m)
    target_link_libraries(png2wav m)
endif()
