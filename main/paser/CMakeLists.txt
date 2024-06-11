include_directories(
    ${PROJECT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${FLEX_INCLUDE_DIRS}
    )

find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

BISON_TARGET(VerilogParser
             VerilogParser.yy
             ${CMAKE_CURRENT_BINARY_DIR}/VerilogParser.cc
             DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/VerilogParser.h
             )
FLEX_TARGET(VerilogLexer
            VerilogScanner.ll
            ${CMAKE_CURRENT_BINARY_DIR}/VerilogScanner.cc)
ADD_FLEX_BISON_DEPENDENCY(VerilogLexer VerilogParser)

file(GLOB SOURCES
    VerilogDataBase.cc VerilogDriver.cc VerilogDriver_1.cc
    )
add_library(verilogparser STATIC ${SOURCES} ${BISON_VerilogParser_OUTPUTS} ${FLEX_VerilogLexer_OUTPUTS})
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(verilogparser PRIVATE DEBUG_VERILOGPARSER)
endif()

if(INSTALL_Extract)
    install(TARGETS verilogparser DESTINATION lib)
    install(FILES VerilogDataBase.h VerilogDriver.h DESTINATION parser/bison)
endif(INSTALL_Extract)
