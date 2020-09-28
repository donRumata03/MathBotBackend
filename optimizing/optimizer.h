#pragma once
#include "pch.h"
#include "query_processor.h"

struct optimizer_return_struct
{
	std::string error_message;
	std::unordered_map<std::string, double> answer;
	double best_value = -1;
	bool has_error_message = true;

	optimizer_return_struct() = default;
	optimizer_return_struct(std::string err) : error_message(std::move(err)), has_error_message(true) {}
	optimizer_return_struct(std::pair<std::unordered_map<std::string, double>, double> ans) : answer(std::move(ans.first)), best_value(ans.second), has_error_message(false) {}
};



optimizer_return_struct process_optimization_query(optimization_query& q);

std::pair<std::unordered_map<std::string, double>, double> combi_optimize(expression_tree* tree,
	const std::unordered_map<std::string, std::pair<double, double>> &variable_ranges, const std::unordered_set<std::string> &variables,
	double target_minimum, size_t iterations);