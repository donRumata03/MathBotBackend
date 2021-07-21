//
// Created by Vova on 01.11.2020.
//

#pragma once
#include <utils/sequence_plotter.h>


#include <optimizing/OptimizationBlock.h>
#include <optimizing/nodes/GA_OptimizationBlock.h>


class OptimizationTree
{
public:
	enum class type
	{
		opt_block,
		seq_container,
		par_container
	};

private:
	type m_type;

	std::vector<OptimizationTree> children;
	std::unique_ptr<OptimizationBlock> m_block = nullptr;

	double best_error = std::numeric_limits<double>::max();
	std::vector<double> best_sequence;

	bool has_set_iterations = false;

public:
	explicit OptimizationTree(const json& source, type parent_container_type = type::par_container);
	void run (const std::function<double (const std::vector<double>&)>& error_function,
	          const std::function<double (const std::vector<double>&)>& fitness_function,
	          const std::function<std::vector<double> (const std::vector<double>&)>& first_gradient,
	          const std::function<std::vector<double> (const std::vector<double>&)>& second_gradient,
	          const std::vector<std::pair<double, double>>& search_domain,
	          const std::optional<std::pair<double, std::vector<double>>>& parent_result = std::nullopt,
	          const std::vector<std::variant<BlockLinker, std::string>>& blocks_with_connections = {});

	std::pair<double, std::vector<double>> get_result() {
		return { best_error, best_sequence };
	}

	void push_iteration_plan(double target_iterations);
	void set_all_blocks_resource(double target_iterations, double total_weight);

private:
	[[nodiscard]] double count_total_time_weight() const;
};


