//
// Created by Vova on 06.07.2021.
//

#include "math_bot_config.h."

/**
 	Use this code to compare old and new results:

 	@code
		if (!std::isnan(new_error) && new_error < old_error)
		{
			output_seq = new_seq;
		}
		else
		{
			output_seq = old_seq;
		}
	@endcode

 Optimization Blocks:

 — GA:
 	— { fitness_function; ranges; params } --> { best_function_value; best_genome }
	— Needs informer
 	— Might end in each iteration
 	— Might throw useful exceptions!
 	— We should measure time

 — GD:
 	— { error_function, first_gradient, previous_approximation, learning_rate, iterations } --> { variable_sequence; best_error }
 	— We need to tune iterations depending on total iterations and tree size!
 	— Also catch exceptions
 	— // TODO: learn how to tune iteration number for GD, when to stop, try not once with different learning rates!
 	—
 — Newton:
 	— { error_function, first_gradient, second_gradient, initial_seq, learning_rate (= 1.), iterations } --> { best_error, variable_sequence }
 	— Take second derivative tree size into account when counting iterations for Newton's method
 	— Don't forget to catch errors

 */

#pragma once


class OptimizationBlock
{
public:
	double time_weight = -1;
	size_t min_it {};

	double computations_resource_in_units = -1;

public:
	enum class type
	{
		GA,
		Annealing,
		GD,
		Newton,

		// To be continued…
	};

	/// Functions to be implemented in derived class:

	virtual type get_type() = 0;

	virtual double iteration_cost_units() { return 1; }

	virtual void update_optimization_objective (
			const std::function<double (const std::vector<double>&)>& _error_function,
			const std::function<double (const std::vector<double>&)>& _fitness_function,
			const std::function<std::vector<double> (const std::vector<double>&)>& _first_gradient,
			const std::function<std::vector<double> (const std::vector<double>&)>& _second_gradient,
			const std::vector<double>& gradient_tree_sizes) = 0;

	virtual void run (const std::vector<double>& parent_genome, const std::vector<std::pair<double, double>>& point_ranges) = 0;

	virtual std::pair<double, std::vector<double>> get_result() = 0;

public:
/// Generated 2nd order functions:

	bool result_is_ready() {
		auto[err, g] = get_result();
		return g.empty();
	}

	bool can_use_initial_position() {
		auto t = get_type();
		return t != type::GA;
	}
	bool initial_position_recommended() {
		return is_in(get_type(), std::initializer_list<type>{ type::GD, type::Newton });
	}

	std::string get_type_name() {
		switch (get_type()) {
			case type::GA:
				return "GA";
			case type::Annealing:
				return "Annealing";
			case type::GD:
				return "GD";
			case type::Newton:
				return "Newton";
		}
		return "Unknown Algorithm!";
	}

	void collect_universal_hyperparameters(const json& params_json) {
		if (params_json.contains("time_weight")) this->time_weight = params_json["time_weight"].get<li>();
		if (params_json.contains("min_it")) this->min_it = params_json["min_it"].get<li>();
	}
};


