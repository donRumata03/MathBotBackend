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
	size_t n_vars = variables.size();

	/// 													Define some order for variables:
	std::vector<std::string> var_seq;
	std::vector<std::pair<double, double>> search_domain;
	var_seq.reserve(n_vars);
	search_domain.reserve(n_vars);

	for (auto& range : variable_ranges) {
		var_seq.emplace_back(range.first);
		search_domain.emplace_back(range.second);
	}

	/// 														Count the derivatives:
	std::vector<std::unique_ptr<expression_tree>> first_derivative_trees;
	first_derivative_trees.reserve(n_vars);
	for (size_t dim_number = 0; dim_number < n_vars; ++dim_number) {
		auto this_tree = tree->generate_derivative_tree(var_seq[dim_number]);
		first_derivative_trees.emplace_back(this_tree);
	}

	std::vector<std::unique_ptr<expression_tree>> second_derivative_trees;
	second_derivative_trees.reserve(n_vars);
	for (size_t dim_number = 0; dim_number < n_vars; ++dim_number) {
		auto this_tree = first_derivative_trees[dim_number]->generate_derivative_tree(var_seq[dim_number]);
		second_derivative_trees.emplace_back(this_tree);
	}

	double function_tree_nodes = tree->count_collective_size();
	double first_derivative_tree_nodes = first_derivative_trees[0]->count_collective_size();
	double second_derivative_tree_nodes = second_derivative_trees[0]->count_collective_size();
	printvar(function_tree_nodes);
	printvar(first_derivative_tree_nodes);
	printvar(second_derivative_tree_nodes);




	return std::pair<std::unordered_map<std::string, double>, double>();
}
