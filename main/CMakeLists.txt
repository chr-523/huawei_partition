cmake_minimum_required(VERSION 3.7.0)

project(huawei_)
# 主目录的CMakeLists.txt

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

find_package(Boost REQUIRED)
if(Boost_INCLUDE_DIRS)
    set(INCLUDES ${Boost_INCLUDE_DIRS})
    set(LIBS ${Boost_LIBRARIES})
else(Boost_INCLUDE_DIRS)
    set(LIBS Boost::boost)
endif(Boost_INCLUDE_DIRS)
include_directories(
    ${PROJECT_SOURCE_DIR}
    ${INCLUDES}
    )
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(COMPILE_DEFINITIONS DEBUG_VERILOGPARSER)
else()
    set(COMPILE_DEFINITIONS "")
endif()
#23123

# 包含头文件目录
include_directories(include)

# 添加子目录parser
add_subdirectory(paser)

# 定义源文件变量
set(SRC_FILE main.cc src/edge.cc src/mymodule.cc src/process.cc src/instance.cc)
    # 可能还需要添加parser目录生成的对象文件或其他源文件
# 链接parser库
add_executable(huawei_ ${SRC_FILE})

set_target_properties(huawei_ PROPERTIES OUTPUT_NAME "test_bison")
target_link_libraries(huawei_ PRIVATE verilogparser ${LIBS})
####
#target_link_libraries(huawei_ ${PROJECT_SOURCE_DIR}/lib/libverilogparser.a)
if(COMPILE_DEFINITIONS)
    target_compile_definitions(huawei_ PRIVATE ${COMPILE_DEFINITIONS})
endif(COMPILE_DEFINITIONS)
if(INSTALL_Extract)
    install(TARGETS huawei_ DESTINATION test/parsers/verilog)
    install(DIRECTORY benchmarks DESTINATION test/parsers/verilog)
endif(INSTALL_Extract)