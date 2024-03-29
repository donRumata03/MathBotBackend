//
// Created by Vova on 07.07.2021.
//

#pragma once
#include "optimizing/OptimizationBlock.h"


class GA_OptimizationBlock : public OptimizationBlock
{
private:
	/// Input:
	std::function<double (const GA::Genome&)> fitness_function;
	GA::continuous_GA_params params;

	std::function<double (const GA::Genome&)> error_function; // To get error by genome in callbacks and in the end

	/// GA-Specific Parameters
	double hazing_percent = 0.4;

	/// Temp:
	GA::ComputationDistribution computation_distribution;

	/// Output:
	double best_error = std::numeric_limits<double>::max();
	std::vector<double> resultant_genome;

public:
	explicit GA_OptimizationBlock(GA::continuous_GA_params _params);

	void collect_specific_hyperparameters (const json& params_json) override;

	void update_optimization_objective (
			const std::function<double (const std::vector<double>&)>& _error_function,
			const std::function<double (const std::vector<double>&)>& _fitness_function,
			const std::function<std::vector<double> (const std::vector<double>&)>& _first_gradient,
			const std::function<std::vector<double> (const std::vector<double>&)>& _second_gradient,
			const std::vector<double>& gradient_tree_sizes) override;


	void run (const std::vector<double>& parent_genome, const std::vector<std::pair<double, double>>& point_ranges) override;

	std::pair<double, std::vector<double>> get_result () override;
	type get_type () override;
};


