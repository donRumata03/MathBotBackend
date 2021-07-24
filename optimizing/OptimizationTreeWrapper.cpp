//
// Created by Vova on 07.07.2021.
//

#include "OptimizationTreeWrapper.h"

OptimizationTreeWrapper::OptimizationTreeWrapper (const fs::path& tree_descriptor_path)

{
	if (not fs::exists(tree_descriptor_path)) throw std::runtime_error("File with tree doesn't exist!");

	optimization_tree.emplace(
			json::parse(*read_file(tree_descriptor_path))
	);
}

std::pair<std::unordered_map<std::string, double>, double> OptimizationTreeWrapper::optimize (expression_tree *tree,
                                                                                              const std::unordered_map<std::string, std::pair<double, double>>& variable_ranges,
                                                                                              const std::unordered_set<std::string>& variables,
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

	/// 							Backwards converter (sequence --> umap):
	auto convert_variable_sequence = [&](const std::vector<double>& variable_values) -> std::unordered_map<std::string, double>
	{
		std::unordered_map<std::string, double> res;
		for (size_t var_index = 0; var_index < n_vars; var_index++) res[var_seq[var_index]] = variable_values[var_index];

		return res;
	};



	/// 								Sequence ->> Error (ℝ^n → ℝ)
	auto generated_error_function = [&](const std::vector<double>& variable_values) -> double
	{
		std::unordered_map < std::string, double > vars = convert_variable_sequence(variable_values);

		try {
			return tree->compute(vars);
		}
		catch (std::exception& e)
		{
			std::cout << console_colors::red << "[OptimizationTreeWrapper ->> GeneratedErrorFunction] Error caught: " << e.what() << std::endl;
			throw;
		}
	};

	///												Gradient counter generator
	auto generate_some_gradient_counter = [&convert_variable_sequence]
			(const std::vector<std::unique_ptr<expression_tree>>& derivative_trees)
	{
		return [&convert_variable_sequence, &derivative_trees] (const std::vector<double>& variable_values) -> std::vector<double>
		{
			std::vector<double> res(variable_values.size());
			std::unordered_map < std::string, double > vars = convert_variable_sequence(variable_values);

			for (size_t dim_number = 0; dim_number < variable_values.size(); ++dim_number) {
				res[dim_number] = derivative_trees[dim_number]->compute(vars);
			}

			return res;
		};
	};


	/// 								Sequence ->> First gradient (ℝ^n → ℝ^n)
	auto generated_first_gradient = generate_some_gradient_counter(first_derivative_trees);

	/// 								Sequence ->> Second gradient (ℝ^n → ℝ^n)
	auto generated_second_gradient = generate_some_gradient_counter(second_derivative_trees);

	/// 								Sequence ->> Fitness (ℝ^n → ℝ)
	auto generated_fitness_function = [&](const std::vector<double>& variable_values) -> double
	{
		double function_result = generated_error_function(variable_values);

		if (function_result - target_minimum < 1e-10) return 2e+15;

		double fitness = 1 / std::abs(function_result - target_minimum);
		return fitness;
	};

	/// Push iterations:
	optimization_tree->push_iteration_plan(iterations);

	/// Finally, RUN:
	optimization_tree->run(
			generated_error_function,
			generated_fitness_function,
			generated_first_gradient,
			generated_second_gradient,
			{ function_tree_nodes, first_derivative_tree_nodes, second_derivative_tree_nodes },
			search_domain
			);

	auto sequence_result = optimization_tree->get_result();
	if (not sequence_result) throw std::runtime_error("Optimization Algorithm Tree returned nothing! You function is probably incorrect…");

	auto best_variables = convert_variable_sequence(sequence_result->second);

	assert(almost_equal(sequence_result->first, tree->compute(best_variables)));

	return {
		best_variables,
		sequence_result->first
	};
}
