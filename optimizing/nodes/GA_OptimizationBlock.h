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
	std::vector<std::pair<double, double>> point_range;
	GA::continuous_GA_params params;

	/// Temp:
	GA::ComputationDistribution computation_distribution;

	/// Output:
	double best_error = std::numeric_limits<double>::max();
	std::vector<double> resultant_genome;

public:
	explicit GA_OptimizationBlock(GA::continuous_GA_params _params);

	void update_optimization_objective (const std::function<double (const std::vector<double>&)>& _error_function,
	                                    const std::function<double (const std::vector<double>&)>& _fitness_function,
	                                    const std::function<std::vector<double> (const std::vector<double>&)>& _first_gradient,
	                                    const std::function<std::vector<double> (const std::vector<double>&)>& _second_gradient) override;
	void update_computations (size_t new_countings) override;


	void run (double parent_error, const std::vector<double>& parent_genome) override;
	std::pair<double, std::vector<double>> get_result () override;
	type get_type () override;
};


