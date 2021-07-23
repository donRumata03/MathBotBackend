//
// Created by Vova on 21.07.2021.
//

#include <other_optimization/local_optimization.h>
#include "Newton_OptimizationBlock.h"

OptimizationBlock::type Newton_OptimizationBlock::get_type ()
{
	return type::Newton;
}

void Newton_OptimizationBlock::update_optimization_objective (
		const std::function<double (const std::vector<double>&)>& _error_function,
		const std::function<double (const std::vector<double>&)>& _fitness_function,
		const std::function<std::vector<double> (const std::vector<double>&)>& _first_gradient,
		const std::function<std::vector<double> (const std::vector<double>&)>& _second_gradient,
		const std::vector<double>& gradient_tree_sizes)
{
	error_function = _error_function;
	fitness_function = _fitness_function;
	first_gradient = _first_gradient;
	second_gradient = _second_gradient;

	grad_tree_sizes = gradient_tree_sizes;

	best_sequence.clear();
	best_error = std::numeric_limits<double>::infinity();
}

void Newton_OptimizationBlock::run (const std::vector<double>& parent_genome,
                                    const std::vector<std::pair<double, double>>& point_ranges)
{
	double it_cost = iteration_cost_units(point_ranges.size());
	auto resultant_iterations = std::max(min_it, size_t(std::round(computations_resource_in_units / it_cost)));
	std::cout << "[Newton]: iteration cost: " << it_cost << "; computations_resource_in_units: " << computations_resource_in_units
	<< " ==> iterations: " << resultant_iterations;
	if (resultant_iterations == min_it) std::cout << " (minimal allowed)" << std::endl;
	std::cout << std::endl;

	newton_optimize(error_function, first_gradient, second_gradient,
	                parent_genome, 0.99,
	                resultant_iterations); // TODO: add some logging
}

std::pair<double, std::vector<double>> Newton_OptimizationBlock::get_result ()
{
	return std::pair<double, std::vector<double>>{
		best_error,
		best_sequence
	};
}

double Newton_OptimizationBlock::iteration_cost_units (size_t parameter_number)
{
	if (not error_function or not first_gradient or not second_gradient) {
		throw std::logic_error("Can't count iteration_cost_units without knowing function, gradient and 2nd gradient!");
	}

	return
		parameter_number
				*
		(grad_tree_sizes[0] + grad_tree_sizes[1] + grad_tree_sizes[2]) / grad_tree_sizes[0];
}
