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

	/// Output:
	double best_fitness = 0;
	std::vector<double> resultant_genome;

public:
	GA_OptimizationBlock(
			std::function<double(const GA::Genome&)> _fitness_function,
			std::vector<std::pair<double, double>> _point_range,
			GA::continuous_GA_params _params
			);

	void run () override;
	std::pair<double, std::vector<double>> get_result () override;
	type get_type () override;
};


