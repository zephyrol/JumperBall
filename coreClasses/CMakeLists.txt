CMAKE_MINIMUM_REQUIRED(VERSION 3.2)

PROJECT(JumperBallCore)
	
set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS OFF)


FILE(GLOB SOURCE_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/scene/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/scene/blocks/*.cpp
        scene/blocks/items/*.cpp
        scene/blocks/enemies/*.cpp
        scene/blocks/special/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/player/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/system/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/gameMenu/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/movements/*.cpp)

FILE(GLOB HEADER_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/scene/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/scene/blocks/*.h
        scene/blocks/items/*.h
        scene/blocks/enemies/*.h
        scene/blocks/special/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/player/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/system/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/gameMenu/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/movements/*.h)


ADD_LIBRARY(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES} scene/Shape.cpp scene/Shape.h)
TARGET_INCLUDE_DIRECTORIES( ${PROJECT_NAME} PUBLIC .)

