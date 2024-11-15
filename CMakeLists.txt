cmake_minimum_required(VERSION 3.28)
project(erp_system_solution)

set(CMAKE_CXX_STANDARD 17)

add_executable(erp_system_solution main.c admin.c erp.h
        employee.c
        salary.c)


