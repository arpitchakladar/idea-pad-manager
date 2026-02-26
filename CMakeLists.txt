cmake_minimum_required(VERSION 3.15)

project(IdeaPadManager
	VERSION 0.1.0
	LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	message(STATUS "Configuring for Debug Mode...")
	target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG=1)
	
	target_compile_options(${PROJECT_NAME} PRIVATE -g -O0 -Wall -Wextra)
else()
	message(STATUS "Configuring for Release Mode...")

	target_compile_options(${PROJECT_NAME} PRIVATE -O3 -march=native)
endif()
