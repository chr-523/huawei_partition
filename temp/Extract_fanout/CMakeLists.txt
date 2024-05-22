cmake_minimum_required(VERSION 3.7.0)

project(Extract)

#set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}" CACHE PATH "Prefix prepended to install directories")

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build, Debug|Release" FORCE)
endif(NOT CMAKE_BUILD_TYPE)
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

# deal with GCC dual ABI issue 
if(NOT CMAKE_CXX_ABI)
    set(CMAKE_CXX_ABI 0 CACHE STRING
        "Choose the value for _GLIBCXX_USE_CXX11_ABI, options are: 0|1."
        FORCE)
endif(NOT CMAKE_CXX_ABI)
message(STATUS "CMAKE_CXX_ABI: _GLIBCXX_USE_CXX11_ABI=${CMAKE_CXX_ABI}")
add_definitions(-D_GLIBCXX_USE_CXX11_ABI=${CMAKE_CXX_ABI})


# packages like Csdp and SDPColoringCsdp are dependent to OPENBLAS
option(OPENBLAS "Choose to enable OpenBLAS or not, ON|OFF" OFF)
message(STATUS "OPENBLAS: ${OPENBLAS}")

# whether install the library or not 
option(INSTALL_Extract "Whether install ${PROJECT_NAME} or not, ON|OFF" OFF)
message(STATUS "INSTALL_Extract: ${INSTALL_Extract}")

# whether generate documentations 
option(GENERATE_DOCS "Whether generate documentations, ON|OFF" OFF)
message(STATUS "GENERATE_DOCS: ${GENERATE_DOCS}")

# whether compile test component 
# option(ENABLE_TEST "Whether enable test, ON|OFF" OFF)
# message(STATUS "ENABLE_TEST: ${ENABLE_TEST}")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)


find_package(BISON 3.3 REQUIRED)
find_package(FLEX REQUIRED) 
find_package(Boost 1.55.0 REQUIRED COMPONENTS graph regex)


add_subdirectory(parser/bison)
add_subdirectory(test)
# if (ENABLE_TEST)
#     add_subdirectory(test)
# endif (ENABLE_TEST)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

cmake_minimum_required(VERSION 3.0.2)
