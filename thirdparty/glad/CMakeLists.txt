cmake_minimum_required(VERSION 3.0)

PROJECT(glad)

ADD_LIBRARY(${PROJECT_NAME} "glad.c" "glad.h" "KHR/khrplatform.h")
TARGET_INCLUDE_DIRECTORIES( ${PROJECT_NAME} PUBLIC .)
