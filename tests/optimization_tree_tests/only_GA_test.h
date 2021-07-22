//
// Created by Vova on 21.07.2021.
//

#pragma once

#include "optimizing/OptimizationTree.h"

#include <tests/test_functions.h>

inline void test_raw_optimization_tree () {
	json tree_description = json::parse(*read_file/*<given_filename_encoding::utf8>*/(math_bot_base_dir / "data" / "only_implemented_tree.json"));

	OptimizationTree tree(tree_description);
	tree.push_iteration_plan(100'000.);
	try {
		tree.run(
				schaffer_function,
				shaffer_fit,
				[] (auto a) { return std::vector<double> {}; },
				[] (auto a) { return std::vector<double> {}; },
				{ 20, 80, 200 },
				{ { -100, 100 }, { -100, 100 } });
	} catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}

	auto [best_error, best_genome] = *tree.get_result();
	//	std::cout << "Best error: " << best_error <<  << std::endl;

	printvar(best_error);
	printvar(best_genome);
}

