//
// Created by Vova on 21.07.2021.
//

#pragma once

#include <optimizing/OptimizationBlock.h>


class Newton_OptimizationBlock : public OptimizationBlock
{
private:
	/// Input:
	std::function<double (const std::vector<double>&)> error_function;
	std::function<double (const std::vector<double>&)> fitness_function;
	std::function<std::vector<double> (const std::vector<double>&)> first_gradient;
	std::function<std::vector<double> (const std::vector<double>&)> second_gradient;

	std::vector<double> grad_tree_sizes;

	/// Output:
	double best_error = std::numeric_limits<double>::infinity();
	std::vector<double> best_sequence;


public:
	type get_type () override;

	double iteration_cost_units () override;

	void update_optimization_objective (
			const std::function<double (const std::vector<double>&)>& _error_function,
			const std::function<double (const std::vector<double>&)>& _fitness_function,
			const std::function<std::vector<double> (const std::vector<double>&)>& _first_gradient,
			const std::function<std::vector<double> (const std::vector<double>&)>& _second_gradient,
			const std::vector<double>& gradient_tree_sizes) override;

	void run (const std::vector<double>& parent_genome, const std::vector<std::pair<double, double>>& point_ranges) override;

	std::pair<double, std::vector<double>> get_result () override;
};


