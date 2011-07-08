cmake_minimum_required(VERSION 2.6)
project(libvm)

find_package(Qt4 REQUIRED)

include(${QT_USE_FILE})

add_definitions( ${QT_DEFINITIONS} )
include_directories( ${QT_INCLUDES} )

set(libvm_SRCS  vmattach.cpp vmpty.cpp)
set(libvm_HEADERS vmattach.h vmpty.h)
set(test_SRCS example/test.cpp)

QT4_WRAP_CPP(libvm_HEADERS_MOC ${libvm_HEADERS})

add_library(vm SHARED ${libvm_SRCS} ${libvm_HEADERS_MOC})

add_executable(test ${test_SRCS})

target_link_libraries(test ${QT_LIBRARIES} vm)

