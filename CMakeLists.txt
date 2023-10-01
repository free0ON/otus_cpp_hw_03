# CMake minimum version
set(PRJ_CMAKE_MIN_VERSION 3.2 CACHE INTERNAL "CMake version")
cmake_minimum_required(VERSION ${PRJ_CMAKE_MIN_VERSION})
# Project information
set(PRJ_NAME "allocator" CACHE INTERNAL "Project name")
set(PRJ_VERSION_MAJOR "0" CACHE INTERNAL "Minor version")
set(PRJ_VERSION_MINOR "0" CACHE INTERNAL "Major version")
set(PRJ_VERSION_PATCH "1" CACHE INTERNAL "Patch version")
set(PRJ_DESCRIPTION "Allocator template class" CACHE INTERNAL "Project description")
set(PRJ_HOMEPAGE_URL "https://github.com/free0ON/otus_cpp_HW_03" CACHE INTERNAL "Project URL")
set(EMAIL_CONTACT "dmitrii.lozovoi@yandex.ru" CACHE INTERNAL "Email contact")
set(TEST_NAME "test_all" CACHE INTERNAL "Test name")

project(${PRJ_NAME} VERSION ${PRJ_VERSION_MAJOR}.${PRJ_VERSION_MINOR}.${PRJ_VERSION_PATCH} DESCRIPTION "${PRJ_DESCRIPTION}" HOMEPAGE_URL "${PRJ_HOMEPAGE_URL}")
#message("PROJECT_NAME: ${PROJECT_NAME} VERSION: ${PROJECT_VERSION}")
#message("DESCRIPTION: ${PRJ_DESCRIPTION}")
#message("HOMEPAGE_URL: ${PRJ_HOMEPAGE_URL}")
#message("CMAKE_MINIMUM_REQUIRED_VERSION: ${CMAKE_MINIMUM_REQUIRED_VERSION}")

configure_file(src/version.h.in version.h)

add_executable(${PRJ_NAME} src/main.cpp src/TFactorial.cpp src/TFactorial.h
        src/TAllocator.cpp src/TAllocator.h src/TVector.cpp src/TVector.h
        src/TList.cpp src/TList.h src/TIterator.cpp src/TIterator.h)
add_executable(${TEST_NAME} test/test_all.cpp src/TFactorial.cpp src/TFactorial.h
        src/TAllocator.cpp src/TAllocator.h src/TVector.cpp src/TVector.h
        src/TList.cpp src/TList.h)
set_target_properties(${PRJ_NAME} ${TEST_NAME} PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        )
target_include_directories(${PRJ_NAME}
        PUBLIC
        "${CMAKE_BINARY_DIR}"
        src
        )
target_include_directories(${TEST_NAME}
        PUBLIC
        "${CMAKE_BINARY_DIR}"
        src
        )

find_package(Threads REQUIRED)
find_package(GTest REQUIRED)

target_link_libraries(
        ${PRJ_NAME}
        PUBLIC
        gtest
        gtest_main
        ${CMAKE_THREAD_LIBS_INIT}
)

target_link_libraries(
        ${TEST_NAME}
        PUBLIC
        gtest
        gtest_main
        ${CMAKE_THREAD_LIBS_INIT}
)

enable_testing()
include(GoogleTest)
add_test(${TEST_NAME} test_all)

if (MSVC)
    target_compile_options(${PRJ_NAME}  PRIVATE  /W4)
    target_compile_options(${TEST_NAME} PRIVATE  /W4)

else()
    target_compile_options(${PRJ_NAME} PRIVATE
            -Wall -Wextra -pedantic -Werror)
    target_compile_options(${TEST_NAME} PRIVATE
            -Wall -Wextra -pedantic -Werror)

endif(MSVC)

install(TARGETS ${PRJ_NAME} RUNTIME DESTINATION bin)
set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PRJ_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PRJ_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PRJ_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT "${EMAIL_CONTACT}")

include(CPack)
