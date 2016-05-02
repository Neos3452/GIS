project(StronglyConnectedComponentsFinder)

cmake_minimum_required(VERSION 3.1.0)

set(CMAKE_CXX_STANDARD 14)

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

include_directories(${PROJECT_SOURCE_DIR})

add_library(${PROJECT_NAME}
    Graph.cpp
    Vertex.cpp
)

target_include_directories(${PROJECT_NAME}
        PUBLIC ${PROJECT_SOURCE_DIR}
)

### Main
set(EXE_NAME find_strongly_connected_components)
add_executable(${EXE_NAME}
        main.cpp
)
target_include_directories(${EXE_NAME}
        PRIVATE $<TARGET_PROPERTY:${PROJECT_NAME},INTERFACE_INCLUDE_DIRECTORIES>
)
target_link_libraries(${EXE_NAME} ${PROJECT_NAME})

### Tests
set(Boost_USE_STATIC_LIBS        ON) # only find static libs otherwise there is a segfault
set(Boost_USE_MULTITHREADED      ON)
find_package(Boost COMPONENTS graph unit_test_framework)
if(Boost_FOUND)
    enable_testing()

    set(TEST_NAMES
        graph
        vertex
        memory_hooks
    )

    set(memory_hooks_SOURCES
        MemoryHooks.cpp
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
        target_link_libraries(${TESTEXE_NAME} ${PROJECT_NAME} ${Boost_LIBRARIES})
        add_test(${TESTEXE_NAME} ${TESTEXE_NAME})
    endforeach()
else()
    message(warning "Missing boost tests module, building without tests")
endif()

