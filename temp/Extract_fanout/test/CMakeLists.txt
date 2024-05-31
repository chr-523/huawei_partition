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
add_executable(test_verilog_bison test_bison.cpp)
set_target_properties(test_verilog_bison PROPERTIES OUTPUT_NAME "test_bison")
target_link_libraries(test_verilog_bison PRIVATE verilogparser ${LIBS})
####
#target_link_libraries(test_verilog_bison ${PROJECT_SOURCE_DIR}/lib/libverilogparser.a)
if(COMPILE_DEFINITIONS)
    target_compile_definitions(test_verilog_bison PRIVATE ${COMPILE_DEFINITIONS})
endif(COMPILE_DEFINITIONS)
if(INSTALL_Extract)
    install(TARGETS test_verilog_bison DESTINATION test/parsers/verilog)
    install(DIRECTORY benchmarks DESTINATION test/parsers/verilog)
endif(INSTALL_Extract)