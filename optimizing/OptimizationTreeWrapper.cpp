//
// Created by Vova on 07.07.2021.
//

#include "OptimizationTreeWrapper.h"

OptimizationTreeWrapper::OptimizationTreeWrapper (const fs::path& tree_descriptor_path)
	: optimization_tree(
			json::parse(*read_file(tree_descriptor_path))
			)
{

}

std::pair<std::unordered_map<std::string, double>, double> OptimizationTreeWrapper::optimize (expression_tree *tree,
                                                                                              const std::unordered_map<std::string, std::pair<double, double>>& variable_ranges,
                                                                                              std::unordered_set<std::string>& variables,
                                                                                              double target_minimum,
                                                                                              size_t iterations)
{


	return std::pair<std::unordered_map<std::string, double>, double>();
}
