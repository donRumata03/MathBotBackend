cmake_minimum_required(VERSION 3.16)

macro(add_debug_release_library target debug_path release_path)
    if (CMAKE_BUILD_TYPE STREQUAL Debug)
        message(It`s\ Debug\ mode!\ =>\ Liking\ Debug\ static\ library\ versions...)
        target_link_libraries(${target} ${debug_path})
    else()
        if (CMAKE_BUILD_TYPE STREQUAL Release)
            message(It`s\ Release\ mode!\ =>\ Liking\ Release\ static\ library\ versions...)
            target_link_libraries(${target} ${release_path})
        else ()
            message(It`s\ an\ Unknown\ build\ mode!\ Can`t\ link\ the library!!!)
            exit()
        endif()

    endif()
endmacro()

macro(print_target_libraries target_name)
    message(___________________________________________)
    get_target_property(OUT ${target_name} LINK_LIBRARIES)
    if (${OUT} STREQUAL OUT-NOTFOUND)
        message(No\ linked\ libraries\ found\ for\ project\ "${target_name}"\ :\()
    else()
        message(Here\ are\ the\ libraries\ linked\ to\ project\ "${target_name}":)
        message(STATUS ${OUT})

    endif()

    message(___________________________________________)
endmacro()



project(Math_bot_backend)

set(CMAKE_CXX_STANDARD 20)

set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")
include(cotire)


include_directories(D:/pythonic)
include_directories(D:/Cpp_Libs/json)
include_directories(D:/Projects/Parsing_expressions/include)
include_directories(D:/Projects/Utilities/GA/)
include_directories(D:/Projects/Utilities/GA/GA)
include_directories(.)


add_executable(Math_bot_backend

        main.cpp

        query_processor.h
        query_processor.cpp

        plotting/plot_manager.h

        optimizing/optimizer.cpp
        optimizing/optimizer.h

        solving/solver.h

        benchmark/length_benchmark.cpp
        benchmark/length_benchmark.h

        benchmark/newton_benchmark.cpp
        benchmark/newton_benchmark.h utils/rounding.cpp utils/rounding.h)


add_debug_release_library(Math_bot_backend D:/pythonic/cmake-build-debug/pythonic.lib D:/pythonic/cmake-build-release/pythonic.lib)

add_debug_release_library(Math_bot_backend
        D:/Projects/Parsing_expressions/cmake-build-debug/Expression_parsing_library.lib
        D:/Projects/Parsing_expressions/cmake-build-release/Expression_parsing_library.lib
)

add_debug_release_library(Math_bot_backend
        D:/Projects/Utilities/GA/cmake-build-debug/GA.lib
        D:/Projects/Utilities/GA/cmake-build-release/GA.lib
)


cotire(Math_bot_backend)
