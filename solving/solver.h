//
// Created by Vova on 16.06.2020.
//

#pragma once

#include "expression_parsing.h"

#include "query_processor.h"
#include "optimizing/optimizer.h"
#include <algorithm>
#include <utils/rounding.h>

inline std::variant<std::string, std::unordered_map<std::string, double>> process_solving_query(const equation_solving_query& query)
{
	std::string function = query.expression;
	size_t eq_signs = std::count_if(function.begin(), function.end(), [](const auto & c) -> bool { return c == '='; });

	if (eq_signs != 1) {
		throw std::logic_error("The number of equality signs (\"=\") in your expression should me equal to 1, otherwise it isn`t a proper equation");
	}

	size_t eq_sign_pos = function.find('=');

	std::string left_part = Slice(function, 0, eq_sign_pos);
	std::string right_part = Slice(function, eq_sign_pos + 1, noslice);

	// std::cout << left_part << " | " << right_part << std::endl;

	std::unique_ptr<expression_tree> left_tree;
	std::unique_ptr<expression_tree> right_tree;
	try {
		left_tree = make_expression_tree(left_part);
		right_tree = make_expression_tree(right_part);
	} catch (std::exception& e) {
		return "Can`t even build your expression for solving! This is the error: "s + e.what();
	}

	auto left_ptr = left_tree.release();
	auto right_ptr = right_tree.release();

	auto res_tree = std::make_unique<expression_tree>(operation_char::absation,
			new expression_tree(operation_char::substraction, left_ptr, right_ptr), nullptr);

	std::unordered_map<std::string, double> for_res;
	double function_value;

	try {
		auto temp_optimizing_res = combi_optimize(res_tree.get(), query.variable_ranges, query.variables, 0., query.iterations);
		for_res = temp_optimizing_res.first;
		function_value = temp_optimizing_res.second;
	} catch (std::exception& exception) {
		std::cerr << "Problem with optimizing function during equation solving: \"" << exception.what() << "\"" << std::endl;

		return { std::string(exception.what()) };
	}

	// TODO: use Newton method for finding solution of an equation which requires only the first derivative!

	double edge = 1e-5;
	if (std::abs(function_value) < edge) {
		// Function has a solution:
		for_res = pretty_a_variable_pack(for_res);
		std::cout << "I`ve found a solution: " << for_res << std::endl;
		return for_res;
	}

	// Can`t find the solution!
	return "Unfortunately, I couldn`t find the solution for the equation!";
}
