//
// Created by Vova on 07.07.2021.
//

#include "GA_OptimizationBlock.h"


GA_OptimizationBlock::GA_OptimizationBlock (GA::continuous_GA_params _params)
                                            : params(std::move(_params))
{

}




void GA_OptimizationBlock::run (double parent_error, const std::vector<double>& parent_genome,
                                const std::vector<std::pair<double, double>>& point_ranges)
{
	if (not fitness_function) throw std::logic_error("Can't run without objective");
	if (computations_resource_in_units <= 0) throw std::logic_error("Can't run without computational resource");

	double fitness_function_computations = computations_resource_in_units;
	computation_distribution = GA::distribute_computations_defaultly(fitness_function_computations);
	params.population_size = computation_distribution.population_size;
	std::cout << "[GA_OptimizationBlock]: " << computation_distribution << std::endl;

	/// Initialize:
	GA::GA_optimizer opt(fitness_function, point_ranges, params);
	opt.run_all_iterations(computation_distribution.epoch_number);

	resultant_genome = opt.get_best_genome();
	best_error = error_function(resultant_genome);
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
	computation_distribution = GA::ComputationDistribution{};
}

