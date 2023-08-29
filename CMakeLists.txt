cmake_minimum_required(VERSION 3.18)
project(wecom-sdk)

set(CMAKE_CXX_STANDARD 14)


find_package(PythonInterp 3.10 REQUIRED)
find_package(PythonLibs 3.10 REQUIRED)


if (${CMAKE_SYSTEM_NAME} MATCHES Linux)
    include_directories(lib/linux)
    link_directories(lib/linux)
elseif (${CMAKE_SYSTEM_NAME} MATCHES Windows)
    include_directories(lib/win)
    link_directories(lib/win)
else ()
    message(FATAL_ERROR "Unsupported Platform: " ${CMAKE_SYSTEM_NAME})
endif()
include_directories(${PYTHON_INCLUDE_PATH})


add_library(wecom MODULE src/wecom_module.cpp)

set_target_properties(
    wecom
    PROPERTIES
        PREFIX ""
        OUTPUT_NAME "wecom"
        LINKER_LANGUAGE CXX
)

target_link_libraries(wecom ${PYTHON_LIBRARIES})
target_link_libraries(wecom WeWorkFinanceSdk_C)