//
// Created by Vova on 07.07.2021.
//

#pragma once

#include "optimizing/OptimizationTree.h"

inline void test_parsing_optimization_tree() {
	std::string filename = "basic_tree.json";
//	std::string filename = "tree_with_custom_start.json";

	json j = *read_file<given_filename_encoding::utf8>(math_bot_base_dir / "data" / filename);
	OptimizationTree tree(j, OptimizationTree::type::par_container);

	int _ = 0;
}
