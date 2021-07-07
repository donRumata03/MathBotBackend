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
