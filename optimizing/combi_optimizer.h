//
// Created by Vova on 22.07.2021.
//

#pragma once

#include <math_bot_config.h>
#include "optimize_query_processor.h"
#include "GA/GA_optimizer.h"

#include "other_optimization/local_optimization.h"


/**
 * 																Deprecated!
 * 																Deprecated!
 * 																Deprecated!
 */
std::pair<std::unordered_map<std::string, double>, double> combi_optimize(expression_tree* tree,
                                                                          const std::unordered_map<std::string, std::pair<double, double>> &variable_ranges, const std::unordered_set<std::string> &variables,
                                                                          double target_minimum, size_t iterations);
