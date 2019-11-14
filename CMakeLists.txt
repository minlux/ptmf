cmake_minimum_required(VERSION 3.10)
project(ptmf ASM C CXX)


set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -O0")
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
#link_libraries(ws2_32 wsock32)

#---------------------------------------------------------------------------------------
# Application
#---------------------------------------------------------------------------------------
add_executable(ptmf
   src/main.cpp
   src/ptmf.cpp
)

#includes
target_include_directories(ptmf PRIVATE
   src
)

