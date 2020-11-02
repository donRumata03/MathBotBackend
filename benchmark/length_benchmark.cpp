//
// Created by Vova on 29.06.2020.
//

#include "length_benchmark.h"

#include <pythonic.h>
#include "expression_parsing.h"

void plot_expression_derivative_length_graph ()
{
	std::vector<std::string> expressions = {
			"0.5 + (cos(sin(abs(x^2 - y^2)))^2 - 0.5) / (1 + 0.001(x^2 + y^2))^2",
			"-20e^((0-0.2)sqrt(0.5(x^2 + y^2))) - e^(0.5(cos(2pi*x) + cos(2pi*y))) + e + 20",
			"-cos(x)cos(y)(exp(-((x-pi)^2 + (y-pi)^2)))",
			"-(y + 47) * sin(sqrt(abs(x / 2 + (y + 47)))) - x * sin(sqrt(abs(x - (y + 47))))",
			"arctg(sin(cos(x))) + arccos(10x * sin(2x^2)) - tg(cos(x))",
			"arctg(x)",
            "sin(x)"
	};

	// std::string example_expression = "0.5 + (cos(sin(abs(x^2 - y^2)))^2 - 0.5) / (1 + 0.001(x^2 + y^2))^2";
	// auto example_tree = make_expression_tree(example_expression);

	size_t diff_steps_number = 4;

	std::vector<std::unique_ptr<expression_tree>> trees(expressions.size());
	for (size_t tree_index = 0; tree_index < expressions.size(); ++tree_index) {
		trees[tree_index] = make_expression_tree(expressions[tree_index]);
	}

	for (size_t tree_index = 0; tree_index < expressions.size(); ++tree_index) {
		std::string function_text = expressions[tree_index];
		auto& initial_tree = trees[tree_index];

		pms to_plot;
		to_plot.reserve(diff_steps_number);
		to_plot.emplace_back(0., double(initial_tree->count_operation_number()));

		auto this_derivative_tree = std::unique_ptr(std::move(initial_tree));

		for (size_t derivative_index = 1; derivative_index <= diff_steps_number; derivative_index++) {
			auto new_tree = std::unique_ptr<expression_tree>(this_derivative_tree->generate_derivative_tree("x"));
			this_derivative_tree = std::move(new_tree);
			to_plot.emplace_back(double(derivative_index), double(this_derivative_tree->count_operation_number()));
		}

		std::cout << function_text << " : " << to_plot << std::endl;
		add_pairs_to_plot(to_plot, { .name = function_text });
	}

	show_plot(
			{
				.log_y = true
			}
	);
}

void length_correlation_test ()
{

}


