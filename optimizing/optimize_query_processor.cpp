#include "combi_optimizer.h"

#include "../query_processor.h"
#include "utils/rounding.h"
#include "OptimizationTreeWrapper.h"


optimizer_return_struct process_optimization_query(optimization_query& q)
{
	// Firstly compile the expression:
	std::unique_ptr<expression_tree> tree;

	try {
		tree = make_expression_tree(q.expression);
	} catch (std::exception& e){
		return "Can't build expression tree (==> expression syntax is incorrect): " + std::string(e.what());
	}

	std::optional<OptimizationTreeWrapper> tree_wrapper;
	try {
		 tree_wrapper.emplace(math_bot_base_dir / "tree_schemas" / q.tree_name);
	} catch(std::exception& e) {
		return "Error while building tree (its structure might be incorrect): "s + e.what();
	}

	std::pair<std::unordered_map<std::string, double>, double> res;
	try {
//		res = combi_optimize(tree.get(), q.variable_ranges, q.variables, q.target_minimum, q.iterations); /// <<-- It's outdated!
		res =
	} catch(std::exception& e) {
		return "Error while optimizing: " + std::string(e.what());
	}

	res.first = pretty_a_variable_pack(res.first);
	res.second = pretty_a_number(res.second);

	return res;
}


