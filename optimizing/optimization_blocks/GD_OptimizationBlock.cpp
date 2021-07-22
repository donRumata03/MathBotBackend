//
// Created by Vova on 22.07.2021.
//

#include "GD_OptimizationBlock.h"

OptimizationBlock::type GD_OptimizationBlock::get_type ()
{
	return type::GD;
}

void GD_OptimizationBlock::collect_specific_hyperparameters (const json& params_json)
{
	if (params_json.contains("learning_rate")) this->learning_rate = params_json["learning_rate"].get<double>();
}

void GD_OptimizationBlock::update_optimization_objective (
		const std::function<double (const std::vector<double>&)>& _error_function,
		const std::function<double (const std::vector<double>&)>& _fitness_function,
		const std::function<std::vector<double> (const std::vector<double>&)>& _first_gradient,
		const std::function<std::vector<double> (const std::vector<double>&)>& _second_gradient,
		const std::vector<double>& gradient_tree_sizes)
{
	error_function = _error_function;
	first_gradient = _first_gradient;

	grad_tree_sizes = gradient_tree_sizes;

	best_sequence.clear();
	best_error = std::numeric_limits<double>::infinity();
}

void GD_OptimizationBlock::run (const std::vector<double>& parent_genome,
                                const std::vector<std::pair<double, double>>& point_ranges)
{
	double it_cost = iteration_cost_units();
	auto resultant_iterations = size_t(std::round(computations_resource_in_units / it_cost));
	std::cout << "[GD]: iteration cost: " << it_cost << "; computations_resource_in_units: " << computations_resource_in_units
	          << " ==> iterations: " << resultant_iterations << std::endl;


	gradient_optimize(error_function, first_gradient, parent_genome, learning_rate, resultant_iterations); // TODO: add some logging
}

std::pair<double, std::vector<double>> GD_OptimizationBlock::get_result ()
{
	return std::pair<double, std::vector<double>>{
		best_error,
		best_sequence
	};
}

double GD_OptimizationBlock::iteration_cost_units ()
{
	if (not error_function or not first_gradient) {
		throw std::logic_error("Can't count iteration_cost_units without knowing function and its gradient!");
	}

	return (grad_tree_sizes[0] + grad_tree_sizes[1]) / grad_tree_sizes[0];
}
