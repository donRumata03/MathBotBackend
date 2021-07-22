//
// Created by Vova on 22.07.2021.
//

#pragma once

#include <optimizing/OptimizationBlock.h>


class GD_OptimizationBlock : OptimizationBlock
{
	type get_type () override;

	void update_optimization_objective (const std::function<double (const std::vector<double>&)>& _error_function,
	                                    const std::function<double (const std::vector<double>&)>& _fitness_function,
	                                    const std::function<std::vector<double> (
			                                    const std::vector<double>&)>& _first_gradient,
	                                    const std::function<std::vector<double> (
			                                    const std::vector<double>&)>& _second_gradient,
	                                    const std::vector<double>& gradient_tree_sizes) override;

	void run (const std::vector<double>& parent_genome, const std::vector<std::pair<double, double>>& point_ranges) override;

	double iteration_cost_units () override;

	std::pair<double, std::vector<double>> get_result () override;
};


