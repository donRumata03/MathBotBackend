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
	enum class type
	{
		GA,
		Annealing,
		GD,
		Newton,

		// To be continued…
	};

	virtual void run() = 0;
	virtual std::pair<double, std::vector<double>> get_result() = 0;
	virtual type get_type() = 0;

	bool can_use_initial_position() {
		auto t = get_type();
		return t != type::GA;
	}
};


