//
// Created by Vova on 22.07.2021.
//

#include "GD_OptimizationBlock.h"

OptimizationBlock::type GD_OptimizationBlock::get_type ()
{
	return type::GD;
}

void GD_OptimizationBlock::update_optimization_objective (
		const std::function<double (const std::vector<double>&)>& _error_function,
		const std::function<double (const std::vector<double>&)>& _fitness_function,
		const std::function<std::vector<double> (const std::vector<double>&)>& _first_gradient,
		const std::function<std::vector<double> (const std::vector<double>&)>& _second_gradient,
		const std::vector<double>& gradient_tree_sizes)
{

}

void GD_OptimizationBlock::run (const std::vector<double>& parent_genome,
                                const std::vector<std::pair<double, double>>& point_ranges)
{

}

std::pair<double, std::vector<double>> GD_OptimizationBlock::get_result ()
{
	return std::pair<double, std::vector<double>>();
}

double GD_OptimizationBlock::iteration_cost_units ()
{
	return OptimizationBlock::iteration_cost_units();
}
