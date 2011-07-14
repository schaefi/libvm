cmake_minimum_required(VERSION 2.6)
project(libvm)

find_package(Qt4 REQUIRED)

include(${QT_USE_FILE})

add_definitions( ${QT_DEFINITIONS} )
include_directories( ${QT_INCLUDES} )

set(libvm_SRCS  vmattach.cpp vmpty.cpp config.cpp)
set(libvm_HEADERS vmattach.h vmpty.h)
set(vmc_SRCS vmc.cpp)

QT4_WRAP_CPP(libvm_HEADERS_MOC ${libvm_HEADERS})

add_library(vm SHARED ${libvm_SRCS} ${libvm_HEADERS_MOC})

add_executable(vmc ${vmc_SRCS})

target_link_libraries(vmc ${QT_LIBRARIES} vm)

