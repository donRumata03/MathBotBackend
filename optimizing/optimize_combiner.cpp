//
// Created by Vova on 01.11.2020.
//

#include "optimize_combiner.h"

OptimizationTree::OptimizationTree (const json& source, type parent_container_type)
{
	if (source.type() == json::value_t::object) {
		m_type = type::opt_block;
		// Set parameters:
		auto params_json = source["parameters"];
		std::string block_type = source["type"];

		if (block_type == "Annealing") {
			throw std::logic_error("Function not yet implemented!");
		}
		else if (block_type == "GA") {
			// TODO
		}
		else if (block_type == "GD") {
			// TODO
		}
		else if (block_type == "Newton") {
			// TODO
		}
		else {
			throw std::logic_error("Bad optimization block name!");
		}

		return;
	}
	assert(source.is_array());

	if (parent_container_type == type::seq_container) {
		// This one is parallel:
		m_type = type::par_container;
	}
	else m_type = type::seq_container;

	for (const auto& json_partition : source) {
		children.emplace_back(json_partition, m_type);
	}
}

void OptimizationTree::run (const std::function<double (const std::vector<double>&)>& error_function,
                            const std::function<double (const std::vector<double>&)>& fitness_function,
                            const std::function<std::vector<double> (const std::vector<double>&)>& first_gradient,
                            const std::function<std::vector<double> (const std::vector<double>&)>& second_gradient,
                            const std::optional<std::pair<double, std::vector<double>>>& parent_result)
{
	if (m_type == type::opt_block) {
		// Just use local optimizer and save the result:
		// …
	}
	else if (m_type == type::seq_container) {
		/// Run children in sequence, choose best of children results
		auto current_optimal_result = parent_result;

		for (auto& child : children) {
			child.run(error_function, fitness_function, first_gradient, second_gradient, current_optimal_result);
			auto child_result = child.get_result();

			if (child_result.first < current_optimal_result->first) {
				current_optimal_result = child_result;
			}
		}
	}
	else{
		/// Run children in parallel

	}
}