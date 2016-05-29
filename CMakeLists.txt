project(StronglyConnectedComponentsFinder)

cmake_minimum_required(VERSION 3.1.0)

set(CMAKE_CXX_STANDARD 14)

find_package(Boost COMPONENTS graph unit_test_framework program_options)

if(MSVC)
    # Force to always compile with W4
    if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    endif()
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -Werror")
endif()

include_directories(${PROJECT_SOURCE_DIR} ${Boost_INCLUDE_DIRS})

set(SOURCES
    Graph.cpp
    Vertex.cpp
)

add_library(${PROJECT_NAME}
    ${SOURCES}
)

target_include_directories(${PROJECT_NAME}
        PUBLIC ${PROJECT_SOURCE_DIR}
)

add_library(${PROJECT_NAME}_mem
    MemoryHooks.cpp
    ${SOURCES}
)

target_include_directories(${PROJECT_NAME}_mem
        PUBLIC ${PROJECT_SOURCE_DIR}
)

target_compile_definitions(${PROJECT_NAME}_mem
        PUBLIC GIS_MEASURE_STACK=1
)

### Main
set(EXE_NAME find_strongly_connected_components)
add_executable(${EXE_NAME}
        main.cpp
)
target_include_directories(${EXE_NAME}
        PRIVATE $<TARGET_PROPERTY:${PROJECT_NAME},INTERFACE_INCLUDE_DIRECTORIES>
)
target_link_libraries(${EXE_NAME} ${PROJECT_NAME}_mem ${Boost_LIBRARIES})

### Tests
enable_testing()

set(TEST_NAMES
    graph
    vertex
    memory_hooks
)

foreach(TEST_NAME ${TEST_NAMES})
    set(TESTEXE_NAME "tests_${TEST_NAME}")
    add_executable(${TESTEXE_NAME}
        ${TESTEXE_NAME}.cpp
        ${${TEST_NAME}_SOURCES}
    )
    target_include_directories(${TESTEXE_NAME}
            PRIVATE $<TARGET_PROPERTY:${PROJECT_NAME},INTERFACE_INCLUDE_DIRECTORIES>
    )
    target_include_directories(${TESTEXE_NAME} SYSTEM
            PRIVATE ${Boost_INCLUDE_DIRS}
    )
    target_compile_definitions(${TESTEXE_NAME}
            PRIVATE $<TARGET_PROPERTY:${PROJECT_NAME},INTERFACE_COMPILE_DEFINITIONS>
    )
    target_link_libraries(${TESTEXE_NAME} ${PROJECT_NAME}_mem ${Boost_LIBRARIES})
    add_test(${TESTEXE_NAME} ${TESTEXE_NAME})
endforeach()

