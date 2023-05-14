cmake_minimum_required(VERSION 3.22)
project(os_unix_myinit C)

set(CMAKE_C_STANDARD 23)

add_executable(os_unix_myinit src/main.c src/sub_processes.h src/sub_processes.c src/config_reading.c src/config_reading.h src/logger.h src/logger.c src/logger.h src/daemonization.c src/daemonization.h src/utils.c src/utils.h)
