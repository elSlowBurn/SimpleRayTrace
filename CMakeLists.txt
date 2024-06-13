cmake_minimum_required(VERSION 3.25.0)
project(Donut VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

set(SOURCES
        main.cpp
)
set(HEADERS
        vec3d.h
        vecFunctions.h
        sphere.h
        light.h
        BVH_Sphere.h
)

add_executable(Donut ${SOURCES} ${HEADERS})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
