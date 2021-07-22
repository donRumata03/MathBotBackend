//
// Created by Vova on 01.11.2020.
//

#include <utils/genome_simple_utils.h>
#include "OptimizationTree.h"


OptimizationTree::OptimizationTree (const json& source, type parent_container_type)
{
	if (source.is_object()) {
		m_type = type::opt_block;

		// Initialize this block:
		auto params_json = source["parameters"];
		std::string block_type = source["type"];

		if (block_type == "Annealing") {
			throw std::logic_error("This type of block not yet implemented!");
		}
		else if (block_type == "GA") {
			auto params = GA::continuous_GA_params{};
			params.mutation_params.mutation_percent_sigma = 0.075;

			m_block = std::make_unique<GA_OptimizationBlock>(
				params
			);
		}
		else if (block_type == "GD") {
			m_block = std::make_unique<GD_OptimizationBlock>();
		}
		else if (block_type == "Newton") {
			m_block = std::make_unique<Newton_OptimizationBlock>();
		}
		else {
			throw std::logic_error("Bad optimization block name!");
		}

		// Universal parameters for blocks:
		m_block->collect_universal_hyperparameters(params_json);
		m_block->collect_specific_hyperparameters(params_json);

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
                            const std::vector<double>& gradient_tree_sizes,
                            const std::vector<std::pair<double, double>>& search_domain,
                            const std::optional<std::pair<double, std::vector<double>>>& parent_result,
                            const std::vector<std::variant<BlockLinker, std::string>>& blocks_with_connections)
{
	if (not has_set_iterations) throw std::logic_error("First " + strike_text_through("the kiss, then the cum!") + " — set iterations for tree, then — run");

	auto new_blocks_with_connections = blocks_with_connections;

	switch (m_type) {
		case type::opt_block: new_blocks_with_connections.emplace_back(m_block->get_type_name()); break;
		case type::seq_container: new_blocks_with_connections.emplace_back(BlockLinker::seq); break;
		case type::par_container: new_blocks_with_connections.emplace_back(BlockLinker::par); break;
	}
	std::cout << (m_type == type::opt_block ? console_colors::yellow : console_colors::blue)
		<< format_opt_block_sequence(new_blocks_with_connections) << "………"
	<< console_colors::remove_all_colors << std::endl;

	if (m_type == type::opt_block) {
		// Just use local optimizer and save the result:
		// …
		display_delimiter();
		std::cout << console_colors::purple
			<< "Optimizing with " << console_colors::underlined << std::get<std::string>(new_blocks_with_connections.back())
		<< console_colors::remove_all_colors << "" << std::endl;
		// ______________________________________________________________________

		m_block->update_optimization_objective(
				error_function,
				fitness_function,
				first_gradient,
				second_gradient,
				gradient_tree_sizes
				);

		/// Optionally, generate initial point, if recommended
		std::vector<double> starting_point_for_algorithm;
		if (not parent_result) {
			if (m_block->initial_position_recommended()) {
				std::cout << "[OptimizationTree::run] " << console_colors::yellow
					<< "WARNING: There's no parent result at the moment of running algorithm «" + m_block->get_type_name() + "», which needs it. "
					<< "The initial point will be generated, but it would impact the quality."
				<< console_colors::remove_all_colors << std::endl;
			}
			/// Generate:
			starting_point_for_algorithm = GA::generate_population(search_domain, 1)[0];
		}
		else{
			if (not m_block->can_use_initial_position()) {
				std::cout << "[OptimizationTree::run] " << console_colors::yellow
				          << "WARNING: Algorithm «" + m_block->get_type_name() + "», can't use initial position, but it had been pre-counted"
				          << "Parent's result will be discarded!"
				<< console_colors::remove_all_colors << std::endl;
			}
			else {
				starting_point_for_algorithm = parent_result->second;
			}
		}
		// m_block->can_use_initial_position()

		m_block->run(starting_point_for_algorithm, search_domain);

		std::tie(best_error, best_sequence) = m_block->get_result();
		// ______________________________________________________________________

		display_delimiter();
	}
	else {
		std::cout << console_colors::underlined << "Start " << (m_type == type::seq_container ? "sequence" : "parallel")
		<< " container" << console_colors::remove_all_colors << " {" << std::endl;

		auto inform_about_best_genome = [&] (const std::string& container_name) {
			std::cout << "[" + container_name + "]: best genome is";
			if (best_sequence.size() <= 7) {
				std::cout << ": " << best_sequence;
			}
			else {
				std::cout << " too long to print";
			}
			std::cout << std::endl;
		};

		if (m_type == type::seq_container) {
			/// Run children in sequence, choose best of children results
			auto current_optimal_result = parent_result;

			for (auto& child : children) {
				child.run(error_function, fitness_function, first_gradient, second_gradient, gradient_tree_sizes,
				          search_domain,
				          current_optimal_result,
				          new_blocks_with_connections
				);
				auto child_result = child.get_result();

				bool opt_updated_now = false;
				if (
						child_result
						and not std::isnan(child_result->first) and check_genome_non_nan(child_result->second)
						and (not current_optimal_result or child_result->first < current_optimal_result->first)
				) {
					current_optimal_result = child_result;
					opt_updated_now = true;
				}
				std::cout << "[SeqContainer]: best_error after block „" << child.m_block->get_type_name() << "”: " << current_optimal_result->first
				          << "(" << (opt_updated_now ? "updated" : "not updated") << ")" << std::endl;

				inform_about_best_genome("SeqContainer");

				new_blocks_with_connections.emplace_back(child.m_block->get_type_name());
			}

			if (not current_optimal_result) {
				std::cout << console_colors::yellow << "[SeqContainer] WARNING: "
					<< "container finished but there's still no result… Returning nothing"
				<< console_colors::remove_all_colors << std::endl;

				best_error = std::numeric_limits<double>::infinity();
				best_sequence.clear();
			}
			else{
				best_error = current_optimal_result->first;
				best_sequence = current_optimal_result->second;
			}
		}

		else{
			/// Run children in parallel
			/// can always leave empty result; leaves this result even if parent result is better: anyway, Parent will cope with it
			for (auto& child : children) {
				child.run(error_function, fitness_function, first_gradient, second_gradient, gradient_tree_sizes,
				          search_domain,
				          get_result(),
				          new_blocks_with_connections
				);
				auto child_result = child.get_result();

				bool renewed_now = false;
				if (
						child_result
						and not std::isnan(child_result->first) and check_genome_non_nan(child_result->second)
						and (not get_result() or best_error >= child_result->first)
				) {
					best_error = child_result->first;
					best_sequence = child_result->second;
					renewed_now = true;
				}
				std::cout << "[ParContainer]: next block processed (" << child.m_block->get_type_name() << "): best_error is "
				<< best_error << "(" << (renewed_now ? "updated" : "not updated") << ")" << std::endl;

				inform_about_best_genome("SeqContainer");
			}
		}

		std::cout << "} " << console_colors::underlined
			<< "(end " << (m_type == type::seq_container ? "sequence" : "parallel") << " container; best_error: " << best_error << ")"
		<< console_colors::remove_all_colors << std::endl;

	}
}

double OptimizationTree::count_total_time_weight () const
{
	switch (m_type) {
		case type::opt_block:
			return m_block->time_weight;
		case type::seq_container:
		case type::par_container:
		{
			std::vector<double> child_results(children.size());
			std::transform(children.begin(), children.end(), child_results.begin(), [](const OptimizationTree& tree){ return tree.count_total_time_weight(); });
			return std::accumulate(child_results.begin(), child_results.end(), 0.);
		}
	}

	return 0;
}

void OptimizationTree::push_iteration_plan (double target_iterations)
{
	set_all_blocks_resource(target_iterations, count_total_time_weight());
}

void OptimizationTree::set_all_blocks_resource (double target_iterations, double total_weight)
{
	has_set_iterations = true;

	if (m_type == type::opt_block) {
		m_block->computations_resource_in_units = target_iterations * (m_block->time_weight / total_weight);
	}
	else {
		for (auto& child : children) {
			child.set_all_blocks_resource(target_iterations, total_weight);
		}
	}
}
