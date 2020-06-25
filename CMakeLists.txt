cmake_minimum_required(VERSION 3.18)

project(IdeaBot)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "-Wall -std=c++11 -pthread -g")

include_directories(include)

aux_source_directory(core CORE_SRCS)
aux_source_directory(device DEVICE_SRCS)

aux_source_directory(bot BOT_SRCS)
list(APPEND BOT_SRCS ${CORE_SRCS} ${DEVICE_SRCS})

message(STATUS "BOT SRCS: ${BOT_SRCS}")

ADD_EXECUTABLE(${PROJECT_NAME} ${BOT_SRCS})
