cmake_minimum_required(VERSION 3.16)

if (UNIX)
    message("UNIX => Setting Compiler to gcc-10")
    set(CMAKE_CXX_COMPILER "/usr/bin/g++-10")
    set(CMAKE_C_COMPILER "/usr/bin/gcc-10")
endif()


######################################################
## Config paths:

if (UNIX)
    set(pythonic_current_dir            $ENV{HOME}/dev/pythonic)
    set(powerful_ga_current_dir         $ENV{HOME}/dev/PowerfulGA)
    set(expression_parsing_current_dir  $ENV{HOME}/dev/ExpressionParsing)
elseif(WIN32)
    set(pythonic_current_dir            D:/pythonic)
    set(powerful_ga_current_dir         D:/Projects/Utilities/GA)
    set(expression_parsing_current_dir  D:/Projects/Utilities/Parsing_expressions)
endif()

########################################

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


include_directories(./)



add_executable(Math_bot_backend

        main.cpp

        query_processor.h
        query_processor.cpp

        plotting/plot_manager.h

        optimizing/optimize_query_processor.cpp
        optimizing/optimize_query_processor.h

        solving/solver.h

        benchmark/length_benchmark.cpp
        benchmark/length_benchmark.h

        benchmark/newton_benchmark.cpp
        benchmark/newton_benchmark.h utils/rounding.cpp utils/rounding.h optimizing/OptimizationTree.cpp optimizing/OptimizationTree.h math_bot_config.h optimizing/OptimizationBlock.h optimizing/OptimizationTreeWrapper.cpp optimizing/OptimizationTreeWrapper.h tests/optimization_tree_tests/parsing_tests.h optimizing/optimization_blocks/GA_OptimizationBlock.cpp optimizing/optimization_blocks/GA_OptimizationBlock.h utils/sequence_plotter.h tests/optimization_tree_tests/raw_optimization_tree_test.h optimizing/optimization_blocks/Newton_OptimizationBlock.cpp optimizing/optimization_blocks/Newton_OptimizationBlock.h optimizing/optimization_blocks/GD_OptimizationBlock.cpp optimizing/optimization_blocks/GD_OptimizationBlock.h utils/genome_simple_utils.h optimizing/combi_optimizer.cpp optimizing/combi_optimizer.h)

# Find ExpressionParsing (doesn't require pythonic):
set(ExpressionParsing_DIR ${expression_parsing_current_dir})
find_package(ExpressionParsing)
include_directories(${expression_parsing_include_directories})
target_link_libraries(Math_bot_backend ${expression_parsing_static_libraries})



# Find PowerfulGA:
set(PowerfulGA_DIR ${powerful_ga_current_dir})
find_package(PowerfulGA)

include_directories(${PowerfulGA_include_directories})
target_link_libraries(Math_bot_backend ${PowerfulGA_static_libraries})
message(${PowerfulGA_static_libraries})



# Find Pythonic:
set(Pythonic_DIR ${pythonic_current_dir})
find_package(Pythonic)
include_directories(${pythonic_include_directories})
target_link_libraries(Math_bot_backend ${pythonic_static_libraries})







cotire(Math_bot_backend)
