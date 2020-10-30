#pragma once

#include "query_processor.h"

#include <expression_parsing.h>


inline std::optional<std::string> process_plotting_query (const plotting_query& query) {
	auto[from, to] = std::tuple{ query.from, query.to };

	size_t steps = query.steps;

	for (auto& function : query.functions) {
/*

		auto compiled_expression = compile_expression(function);

		if (compiled_expression.has_error_message) {
			return compiled_expression.error_message;
		}

		const auto &particle = compiled_expression.particle;

		auto raw_tokens = *make_tokens(function);
		auto variables = get_variables(raw_tokens);
*/
		std::unique_ptr<expression_tree> tree = nullptr;

		try {
			tree = make_expression_tree(function);
		} catch (std::exception& e){
			return e.what();
		}

		auto variables = tree->get_variables();

		if (variables.size() != 1) {
			return "Plotter fatal error: The number of variables in your function should be equal to one for this command!";
		}

		// std::cout << "" << std::endl;

		std::string variable_name = *variables.begin();


		auto plot_function = [&] (double x) {
			return tree->compute({{ variable_name, x }});
		};

		auto range_params = plot_range_params(from, to, steps);

		try {
			add_function_to_plot(plot_function, { .name = recode::to_utf8(function) }, range_params, true);
		} catch (std::exception &e) {
			return "There is an error while computing the function: "s + e.what();
		}

	}

	show_plot({ .output_filename = query.target_path });

	// std::cout << std::endl << "Plotted successfully!" << std::endl;
	std::cout << std::endl << console_colors::green << "Plotted successfully!" << console_colors::remove_all_colors << std::endl;

	return {};
}

