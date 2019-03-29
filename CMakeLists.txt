cmake_minimum_required(VERSION 3.1)

project(coevent)

include(GNUInstallDirs)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

find_package(Libevent REQUIRED)
include_directories(SYSTEM ${LIBEVENT_INCLUDE_DIRS})

include_directories(include)
include_directories(.)

file(GLOB SRCS src/*.cc)

add_library(coevent ${SRCS})
target_link_libraries(coevent ${LIBEVENT_LIBRARIES})

install(DIRECTORY include/coevent 
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(TARGETS coevent
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
