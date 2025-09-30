cmake_minimum_required(VERSION 3.10)
project("TP_STL"
	VERSION 3.0
	DESCRIPTION "TP Standard Library"
	LANGUAGES CXX)

# Build Type (Debug/Release)
if(NOT CMAKE_BUILD_TYPE)
#	set(CMAKE_BUILD_TYPE Release)
	set(CMAKE_BUILD_TYPE Debug)
endif()
# Display build info
message("Build type: " ${CMAKE_BUILD_TYPE})

# Require C++XX support : C++17 is required for filesystem:: C++20 for concepts
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -Wall")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)
set(CMAKE_INCLUDE_CURRENT_DIR ${PROJECT_SOURCE_DIR}/include)
#include_directories(.)

# Checkers scripts (requires either R, scilab or matlab installed)
set(SCRIPTS_DIR scripts)
set(RESOURCES
	${PROJECT_SOURCE_DIR}/${SCRIPTS_DIR}/check_TP_STL.R
	${PROJECT_SOURCE_DIR}/${SCRIPTS_DIR}/check_TP_STL.sce
	${PROJECT_SOURCE_DIR}/${SCRIPTS_DIR}/check_TP_STL.m
)

# Set sources
set(SOURCES
	# Headers
	${CMAKE_INCLUDE_CURRENT_DIR}/Arithmetic.h
	${CMAKE_INCLUDE_CURRENT_DIR}/functors/BoundedRandomGenerator.h
	${CMAKE_INCLUDE_CURRENT_DIR}/functors/StatFunctor.h
	${CMAKE_INCLUDE_CURRENT_DIR}/RunException.h
	${CMAKE_INCLUDE_CURRENT_DIR}/checkers/ExternalCheck.h
	${CMAKE_INCLUDE_CURRENT_DIR}/checkers/RCheck.h
	${CMAKE_INCLUDE_CURRENT_DIR}/checkers/MatlabCheck.h
	# Sources
#	${CMAKE_SOURCE_DIR}/saveToCSV.cpp
	${CMAKE_SOURCE_DIR}/functors/BoundedRandomGenerator.cpp
	${CMAKE_SOURCE_DIR}/functors/StatFunctor.cpp
	${CMAKE_SOURCE_DIR}/RunException.cpp
	${CMAKE_SOURCE_DIR}/checkers/ExternalCheck.cpp
	${CMAKE_SOURCE_DIR}/checkers/RCheck.cpp
	${CMAKE_SOURCE_DIR}/checkers/MatlabCheck.cpp
	${CMAKE_SOURCE_DIR}/main.cpp
	# Scripts
	# ${RESOURCES}
)

add_executable(${PROJECT_NAME} ${SOURCES})

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	# ignore variable length arrays warning (works fine)
	target_compile_options(${PROJECT_NAME} PRIVATE -Wno-vla)
	message(STATUS "Clang Variable length array warning suppressed")
endif()

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_INCLUDE_CURRENT_DIR})

configure_file(${PROJECT_SOURCE_DIR}/${SCRIPTS_DIR}/check_TP_STL.m ${CMAKE_CURRENT_BINARY_DIR}/check_TP_STL.m COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/${SCRIPTS_DIR}/check_TP_STL.R ${CMAKE_CURRENT_BINARY_DIR}/check_TP_STL.R COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/${SCRIPTS_DIR}/check_TP_STL.sce ${CMAKE_CURRENT_BINARY_DIR}/check_TP_STL.sce COPYONLY)

string(TIMESTAMP TODAY "%Y-%m-%d")
set(ARCHIVE_OUTPUT_DIR archives)
if (EXISTS ARCHIVE_OUTPUT_DIR)
	message(STATUS "Archive directory: ${ARCHIVE_OUTPUT_DIR}")
else()
	file(MAKE_DIRECTORY ${ARCHIVE_OUTPUT_DIR})
endif()
set(ARCHIVE_OUTPUT_NAME ${PROJECT_SOURCE_DIR}/${ARCHIVE_OUTPUT_DIR}/${CMAKE_PROJECT_NAME}-${TODAY}.zip)
message(STATUS "archive: ${ARCHIVE_OUTPUT_NAME}")
add_custom_target(archive
	COMMENT "Packaging project in archive ${ARCHIVE_OUTPUT_DIR}/${CMAKE_PROJECT_NAME}-${TODAY}.zip"
	COMMAND ${CMAKE_COMMAND} -E tar cvf ${ARCHIVE_OUTPUT_NAME} --format=zip -- ${SOURCES} ${RESOURCES} ${PROJECT_SOURCE_DIR}/CMakeLists.txt "${PROJECT_SOURCE_DIR}/TP STL.pdf"
	DEPENDS ${SOURCES} ${RESOURCES} ${PROJECT_SOURCE_DIR}/CMakeLists.txt "${PROJECT_SOURCE_DIR}/TP STL.pdf"
)
