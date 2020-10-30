cmake_minimum_required(VERSION 3.16)


if (UNIX)
    set(pythonic_current_dir            $ENV{HOME}/dev/pythonic)
    set(powerful_GA_current_dir         $ENV{HOME}/dev/PowerfulGA)
    set(expression_parsing_current_dir  $ENV{HOME}/dev/ExpressionParsing)
elseif(WIN32)
    set(pythonic_current_dir            D:/pythonic)
    set(powerful_GA_current_dir         D:/Projects/PowerfulGA)
    set(expression_parsing_current_dir  D:/Projects/ExpressionParsing)
endif()


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



# Find Pythonic:
set(Pythonic_DIR ${current_pythonic_dir})
find_package(Pythonic)
include_directories(${pythonic_include_directories})
target_link_libraries(${pythonic_static_libraries})
# Find PowerfulGA:
set(PowerfulGA_DIR ${current_powerful_ga_dir})
find_package(PowerfulGA)
include_directories(${powerful_ga_include_directories})
target_link_libraries(${powerful_ga_static_libraries})


# Find ExpressionParsing:
set(ExpressionParsing_DIR ${current_expression_parsing_dir})
find_package(ExpressionParsing)
include_directories(${expression_parsing_include_directories})
target_link_libraries(${expression_parsing_static_libraries})





cotire(Math_bot_backend)
