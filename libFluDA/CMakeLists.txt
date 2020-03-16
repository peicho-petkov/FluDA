cmake_minimum_required(VERSION 3.10)
project(libFluDA)
set(CMAKE_BUILD_TYPE Release)

set(CMAKE_INSTALL_PREFIX ${CMAKE_HOME_DIRECTORY}/local)

include_directories(include)
file(GLOB SOURCES "src/*.cpp")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

add_library(FluDA STATIC ${SOURCES})

install(DIRECTORY include DESTINATION ${CMAKE_INSTALL_PREFIX}) 
install(TARGETS FluDA DESTINATION lib)
