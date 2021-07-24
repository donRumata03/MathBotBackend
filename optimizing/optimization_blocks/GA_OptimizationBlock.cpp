//
// Created by Vova on 07.07.2021.
//

#include "GA_OptimizationBlock.h"


GA_OptimizationBlock::GA_OptimizationBlock (GA::continuous_GA_params _params)
                                            : params(std::move(_params))
{}


void GA_OptimizationBlock::run (const std::vector<double>& parent_genome,
                                const std::vector<std::pair<double, double>>& point_ranges)
{
	if (not fitness_function) throw std::logic_error("Can't run without objective");
	if (computations_resource_in_units <= 0) throw std::logic_error("Can't run without computational resource");

	// Distribute computations:
	double fitness_function_computations = std::max(double(min_it), computations_resource_in_units);
	computation_distribution = GA::distribute_computations_defaultly(fitness_function_computations);
	params.population_size = computation_distribution.population_size;
	std::cout << "[GA_OptimizationBlock]: " << computation_distribution << std::endl;

	// Parameters:
	size_t variable_number = point_ranges.size();
	params.mutation_params.target_gene_mutation_number = 0.3 * double(variable_number);

	/// Initialize GA optimizer (ignore parent error):
	GA::GA_optimizer optimizer(fitness_function, point_ranges, params);

	/// Callback (counts error by genome; prints genome if its length <= 5):
	auto callback = [&](const size_t iterations_performed, const double best_fitness, const std::vector<double>& best_genome)
	{
		if (pythonic_random() > 15. / computation_distribution.epoch_number) { // ≈15 outputs / algo
			return;
		}

		std::cout << "GA: " << percent_plotter(iterations_performed, computation_distribution.epoch_number, 2);
		std::cout << " iterations performed; best error now: " << error_function(best_genome);

		if (best_genome.size() <= 5) std::cout << "; best genome: " << best_genome;

		std::cout << std::endl;
	};
	optimizer.set_informer(callback);


	/// Run
	optimizer.run_all_iterations(computation_distribution.epoch_number);

	resultant_genome = optimizer.get_best_genome();
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
		const std::function<std::vector<double> (const std::vector<double>&)>& _second_gradient,
		const std::vector<double>& gradient_tree_sizes)
{
	fitness_function = _fitness_function;
	error_function = _error_function;


	// Reset results:
	best_error = std::numeric_limits<double>::max();
	resultant_genome.clear();
	computation_distribution = GA::ComputationDistribution{};
}

void GA_OptimizationBlock::collect_specific_hyperparameters (const json& params_json)
{
	if (params_json.contains("hazing_percent")) this->hazing_percent = params_json["hazing_percent"].get<double>();
}

