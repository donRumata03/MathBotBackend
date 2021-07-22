#include "combi_optimizer.h"

#include "../query_processor.h"
#include "utils/rounding.h"



optimizer_return_struct process_optimization_query(optimization_query& q)
{
	// Firstly compile the expression:
	std::unique_ptr<expression_tree> tree;

	try {
		tree = make_expression_tree(q.expression);
	} catch (std::exception& e){
		return std::string(e.what());
	}


	// After that order the variables the way GA needs them
	std::pair<std::unordered_map<std::string, double>, double> res;
	try {
		res = combi_optimize(tree.get(), q.variable_ranges, q.variables, q.target_minimum, q.iterations);
	} catch(std::exception& e) {
		return std::string(e.what());
	}

	res.first = pretty_a_variable_pack(res.first);
	res.second = pretty_a_number(res.second);

	return res;
}


