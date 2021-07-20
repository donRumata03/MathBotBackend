//
// Created by Vova on 07.07.2021.
//

#include "GA_OptimizationBlock.h"


GA_OptimizationBlock::GA_OptimizationBlock (GA::continuous_GA_params _params)
                                            : params(std::move(_params))
{

}




void GA_OptimizationBlock::run (double parent_error, const std::vector<double>& parent_genome)
{
	GA::GA_optimizer opt(fitness_function, point_range, params);


}

std::pair<double, std::vector<double>> GA_OptimizationBlock::get_result ()
{
	return {
		best_error,
		resultant_genome
	};
}

OptimizationBlock::type GA_OptimizationBlock::get_type ()
{
	return type::GA;
}


void GA_OptimizationBlock::update_optimization_objective (
		const std::function<double (const std::vector<double>&)>& _error_function,
		const std::function<double (const std::vector<double>&)>& _fitness_function,
		const std::function<std::vector<double> (const std::vector<double>&)>& _first_gradient,
		const std::function<std::vector<double> (const std::vector<double>&)>& _second_gradient)
{
	fitness_function = _fitness_function;


	// Reset results:
	best_error = std::numeric_limits<double>::max();
	resultant_genome.clear();
}

void GA_OptimizationBlock::update_computations (size_t new_countings)
{
	computation_distribution = GA::distribute_computations_defaultly(new_countings);

	// Tune parameters:
	params.population_size = computation_distribution.population_size;
}

