//
// Created by Vova on 21.07.2021.
//

#pragma once

#include <optimizing/OptimizationTreeWrapper.h>
#include "optimizing/optimize_combiner.h"

#include <tests/test_functions.h>

inline void test_raw_tree_with_GA () {
	json tree_description = json::parse(*read_file/*<given_filename_encoding::utf8>*/(math_bot_base_dir / "data" / "only_GA_tree.json"));

	OptimizationTree tree(tree_description);
	tree.push_iteration_plan(100'000.);
	tree.run(
			schaffer_function,
			shaffer_fit,
			[](auto a){ return std::vector<double>{}; },
			[](auto a){ return std::vector<double>{}; },
			{ { -100, 100 } , { -100, 100 } }
			);

	auto [best_error, best_genome] = tree.get_result();
	//	std::cout << "Best error: " << best_error <<  << std::endl;

	printvar(best_error);
	printvar(best_genome);
}

