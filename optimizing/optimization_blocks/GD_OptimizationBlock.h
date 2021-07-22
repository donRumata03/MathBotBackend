//
// Created by Vova on 22.07.2021.
//

#pragma once

#include <optimizing/OptimizationBlock.h>
#include <other_optimization/local_optimization.h>


class GD_OptimizationBlock : public OptimizationBlock
{
private:
	/// Hyperparameters:
	double learning_rate = -1;

	/// Input data:
	std::function<double (const std::vector<double>&)> error_function;
	std::function<std::vector<double> (const std::vector<double>&)> first_gradient;

	std::vector<double> grad_tree_sizes;

	/// Output:
	double best_error = std::numeric_limits<double>::infinity();
	std::vector<double> best_sequence;

public:

	GD_OptimizationBlock() = default;

	void collect_specific_hyperparameters (const json& params_json) override;

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


