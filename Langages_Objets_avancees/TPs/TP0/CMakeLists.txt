cmake_minimum_required(VERSION 3.10)
project("Classroom_Example"
	VERSION 2.0
	DESCRIPTION "Standard Algos examples"
	LANGUAGES CXX)

# Build Type (Debug/Release)
if(NOT CMAKE_BUILD_TYPE)
#	set(CMAKE_BUILD_TYPE Release)
	set(CMAKE_BUILD_TYPE Debug)
endif()
# Display build info
message("Build type: " ${CMAKE_BUILD_TYPE})

# Require C++XX support
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a -Wall")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)
set(CMAKE_INCLUDE_CURRENT_DIR ${PROJECT_SOURCE_DIR}/include)
#include_directories(.)

# Set sources
set(SOURCES
	# Headers
	${CMAKE_INCLUDE_CURRENT_DIR}/BoundedRandom.h
	${CMAKE_INCLUDE_CURRENT_DIR}/IncrementalMeanOperator.h
	${CMAKE_INCLUDE_CURRENT_DIR}/ConstantSubstractor.h
	# Sources
	${CMAKE_SOURCE_DIR}/BoundedRandom.cpp
	${CMAKE_SOURCE_DIR}/ConstantSubstractor.cpp
	${CMAKE_SOURCE_DIR}/IncrementalMeanOperator.cpp
	${CMAKE_SOURCE_DIR}/main.cpp
)

set(ALL_FILES
	${SOURCES}
	${PROJECT_SOURCE_DIR}/CMakeLists.txt
)


add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_INCLUDE_CURRENT_DIR})

string(TIMESTAMP TODAY "%Y-%m-%d")
set(ARCHIVE_OUTPUT_DIR archives)
if (EXISTS ARCHIVE_OUTPUT_DIR)
	message(STATUS "Archive directory: ${ARCHIVE_OUTPUT_DIR}")
else()
	file(MAKE_DIRECTORY ${ARCHIVE_OUTPUT_DIR})
	message(STATUS "Archive directory: \"${ARCHIVE_OUTPUT_DIR}\" already exists")
endif()
set(ARCHIVE_OUTPUT_NAME ${PROJECT_SOURCE_DIR}/${ARCHIVE_OUTPUT_DIR}/${CMAKE_PROJECT_NAME}-${TODAY}.zip)
message(STATUS "archive: ${ARCHIVE_OUTPUT_NAME}")
add_custom_target(archive
	COMMENT "Packaging project in archive ${ARCHIVE_OUTPUT_NAME}"
	COMMAND ${CMAKE_COMMAND} -E tar cvf ${ARCHIVE_OUTPUT_NAME} --format=zip -- ${SOURCES} ${PROJECT_SOURCE_DIR}/CMakeLists.txt
	DEPENDS ${SOURCES} ${PROJECT_SOURCE_DIR}/CMakeLists.txt
)

add_custom_target(listing
	COMMENT "Printing all files in PDF format"
	DEPENDS ${ALL_FILES}
	COMMAND pdfPrint ${ALL_FILES}
	# foreach (file ${ALL_FILES})
	# 	message(STATUS "current file is ${file}")
	# 	execute_process(COMMAND file ${file})
	# endforeach()
)
set_target_properties(listing
	PROPERTIES ADDITIONAL_CLEAN_FILES output.pdf
)
