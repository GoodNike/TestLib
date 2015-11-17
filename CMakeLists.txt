cmake_minimum_required(VERSION 2.8.0 FATAL_ERROR)

project(TestLib)

set(INCLUDES
    "${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/test.h"
)

set(SOURCES
    "${PROJECT_SOURCE_DIR}/src/test.c"
)

set(TESTS
    "${PROJECT_SOURCE_DIR}/tests/main.c"
)

add_library(${PROJECT_NAME} STATIC ${INCLUDES} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/include")

add_executable (${PROJECT_NAME}Test ${TESTS})
target_link_libraries (${PROJECT_NAME}Test ${PROJECT_NAME})

set_target_properties(${PROJECT_NAME} ${PROJECT_NAME}Test PROPERTIES FOLDER ${FOLDER}/${PROJECT_NAME})

install(
    TARGETS ${PROJECT_NAME}
    ARCHIVE DESTINATION lib
)

install(
    DIRECTORY "${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}" DESTINATION include
)

add_test(
    NAME    ${PROJECT_NAME}
    COMMAND ${PROJECT_NAME}Test
)
