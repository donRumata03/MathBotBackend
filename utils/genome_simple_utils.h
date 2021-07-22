//
// Created by Vova on 22.07.2021.
//

#pragma once

#include <math_bot_config.h>


inline bool check_genome_non_nan(const std::vector<double>& genome) {
	return std::none_of(genome.begin(), genome.end(), [](double d) -> bool { return std::isnan(d); });
}

