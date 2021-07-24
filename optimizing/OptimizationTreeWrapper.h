//
// Created by Vova on 07.07.2021.
//

#pragma once

#include <optimizing/OptimizationTree.h>

class OptimizationTreeWrapper
{
private:
	std::optional<OptimizationTree> optimization_tree;

public:
	explicit OptimizationTreeWrapper(const fs::path& tree_descriptor_path);

	std::pair<std::unordered_map<std::string, double>, double> optimize (
			expression_tree *tree,
			const std::unordered_map<std::string, std::pair<double, double>> &variable_ranges, const std::unordered_set<std::string>& variables,
			double target_minimum, size_t iterations
	);
};


