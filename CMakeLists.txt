cmake_minimum_required(VERSION 2.8)

project(StegoPy)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

################ CXX FLAGS ##################

if(NOT WIN32)
  set(warnings "-Wall -Wextra") #-Werror
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ${warnings}")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${warnings} -fPIC")
endif()

################ CXX FLAGS ##################


################ BOOST & PYTHON ##################

set(Python_ADDITIONAL_VERSIONS 3.5)
find_package( PythonLibs 3.5 REQUIRED ) #TODO 3>=
include_directories( ${PYTHON_INCLUDE_DIRS} )

#set(Boost_USE_STATIC_LIBS ON)
find_package( Boost COMPONENTS python-py35 REQUIRED )
include_directories( ${Boost_INCLUDE_DIR} )


################ BOOST & PYTHON ##################

################ STEGODISK ##################

option(BUILD_TESTS OFF)
add_subdirectory(StegoDisk)
set(STEGODISK_LIBRARIES ${CMAKE_BINARY_DIR}/StegoDisk/lib/)
include_directories(${CMAKE_SOURCE_DIR}/StegoDisk/src/)

if(NOT WIN32)
    link_directories(${CMAKE_BINARY_DIR}/StegoDisk/lib/)
    set(STEGODISK_LIBRARIES stego-disk)
    set(LIBJPEGTURBO_LIBRARIES jpeg)
else()
    link_directories(${CMAKE_BINARY_DIR}/StegoDisk/bin/*/)
    set(LIBJPEGTURBO_LIBRARIES jpeg-static)
    set(STEGODISK_LIBRARIES stego-disk)
endif()

################ STEGODISK ##################

add_library( stego_py SHARED stego_py.cc )
target_link_libraries( stego_py ${STEGODISK_LIBRARIES} ${LIBJPEGTURBO_LIBRARIES} ${Boost_LIBRARIES})
set_target_properties( stego_py PROPERTIES PREFIX "" )
