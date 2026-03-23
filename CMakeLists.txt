cmake_minimum_required(VERSION 3.15)

project(IdeaPadManager
	VERSION 0.1.1
	LANGUAGES CXX
)

message(STATUS "Project Version: ${CMAKE_PROJECT_VERSION}")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(EXISTS "${CMAKE_BINARY_DIR}/compile_commands.json")
	execute_process(COMMAND ${CMAKE_COMMAND} -E copy
		"${CMAKE_BINARY_DIR}/compile_commands.json"
		"${CMAKE_SOURCE_DIR}/compile_commands.json")
endif()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

set(FTXUI_ENABLE_INSTALL ON CACHE BOOL "" FORCE)
set(FTXUI_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(FTXUI_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(FTXUI_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(FTXUI_ENABLE_COVERAGE OFF CACHE BOOL "" FORCE)
set(FTXUI_DEV_WARNINGS OFF CACHE BOOL "" FORCE)
set(FTXUI_CLANG_TIDY OFF CACHE BOOL "" FORCE)
set(FTXUI_QUIET ON CACHE BOOL "" FORCE)

FetchContent_Declare(
	ftxui
	GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI.git
	GIT_TAG v6.1.9
)

FetchContent_MakeAvailable(ftxui)

FetchContent_Declare(
	spdlog
	GIT_REPOSITORY https://github.com/gabime/spdlog.git
	GIT_TAG v1.17.0
)

FetchContent_MakeAvailable(spdlog)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME}
	PRIVATE
		ftxui::screen
		ftxui::dom
		ftxui::component
)

target_link_libraries(${PROJECT_NAME}
	PRIVATE
		spdlog
)

target_include_directories(${PROJECT_NAME}
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
	$<$<CONFIG:Debug>:SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE>
	$<$<CONFIG:Release>:SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_OFF>
	$<$<CONFIG:Debug>:DEBUG_LOG_PATH=\"build/debug.log\">
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	message(STATUS "Configuring for Debug Mode...")
	target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG=1)
	target_compile_options(${PROJECT_NAME} PRIVATE -g -O0 -Wall -Wextra)
else()
	message(STATUS "Configuring for Release Mode...")
	target_compile_options(${PROJECT_NAME} PRIVATE -O3 -march=native)
endif()
