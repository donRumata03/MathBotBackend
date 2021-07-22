//
// Created by Vova on 22.07.2021.
//

#pragma once

#include <math_bot_config.h>


inline bool check_genome_non_nan(const std::vector<double>& genome) {
	return std::all_of(genome.begin(), genome.end(), std::not_fn(std::isnan));
}

