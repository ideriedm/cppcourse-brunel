cmake_minimum_required (VERSION 2.6)
project (Neuron)

set(SRC "src/")
set(TST "tests/")
set(CMAKE_CXX_FLAGS "-W -Wall -pedantic -std=c++11 -O3")


enable_testing()
add_subdirectory(googletest)
include_directories(${SRC} ${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_library(ProjectLibs STATIC ${SRC}neuron.cpp ${SRC}network.cpp)
add_executable (Neuron ${SRC}main.cpp)
target_link_libraries(Neuron ProjectLibs)

add_executable (Neuron_unittest ${TST}neuron_unittest.cpp)
target_link_libraries(Neuron_unittest ProjectLibs gtest gtest_main)
add_test(Neuron_unittest Neuron_unittest)

# check if Doxygen is installed
find_package(Doxygen)
if (DOXYGEN_FOUND)
    # set input and output files
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    # request to configure the file
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

    # note the option ALL which allows to build the docs together with the application
    add_custom_target( doc_doxygen ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM )
else (DOXYGEN_FOUND)
  message("Doxygen need to be installed to generate the doxygen documentation")
endif (DOXYGEN_FOUND)
