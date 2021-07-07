//
// Created by Vova on 07.07.2021.
//

#include "GA_OptimizationBlock.h"


GA_OptimizationBlock::GA_OptimizationBlock (std::function<double (const GA::Genome&)> _fitness_function,
                                            std::vector<std::pair<double, double>> _point_range,
                                            GA::continuous_GA_params _params)
                                            :
                                            fitness_function(std::move(_fitness_function)),
                                            point_range(std::move(_point_range)),
                                            params(std::move(_params))
{

}




void GA_OptimizationBlock::run ()
{
	GA::GA_optimizer opt(fitness_function, point_range, params);


}

std::pair<double, std::vector<double>> GA_OptimizationBlock::get_result ()
{
	return {
		best_fitness,
		resultant_genome
	};
}

OptimizationBlock::type GA_OptimizationBlock::get_type ()
{
	return type::GA;
}

