//
// Created by Vova on 21.07.2021.
//

#include "Newton_OptimizationBlock.h"

OptimizationBlock::type Newton_OptimizationBlock::get_type ()
{
	return type::Newton;
}

void Newton_OptimizationBlock::update_optimization_objective (
		const std::function<double (const std::vector<double>&)>& _error_function,
		const std::function<double (const std::vector<double>&)>& _fitness_function,
		const std::function<std::vector<double> (const std::vector<double>&)>& _first_gradient,
		const std::function<std::vector<double> (const std::vector<double>&)>& _second_gradient)
{
	error_function = _error_function;
	fitness_function = _fitness_function;
	first_gradient = _first_gradient;
	second_gradient = _second_gradient;
}

void Newton_OptimizationBlock::run (const std::vector<double>& parent_genome,
                                    const std::vector<std::pair<double, double>>& point_ranges)
{
	newton_optimize(error_function, first_gradient, second_gradient,
				    parent_genome, 0.99, computations_resource_in_units / iteration_cost_units(), …);
}

std::pair<double, std::vector<double>> Newton_OptimizationBlock::get_result ()
{
	return std::pair<double, std::vector<double>>{
		best_error,
		best_sequence
	};
}

double Newton_OptimizationBlock::iteration_cost_units ()
{
	if (not error_function or not first_gradient or not second_gradient) {
		throw std::logic_error("Can't count iteration_cost_units without knowing function, gradient and 2nd gradient!");
	}

	// TODO
	return ?;
}
