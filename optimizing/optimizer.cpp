#include <pythonic.h>
#include "optimizer.h"
#include "GA.h"
#include "other_optimization/local_optimization.h"
#include "../query_processor.h"
#include "utils/rounding.h"

optimizer_return_struct process_optimization_query(optimization_query& q)
{
	// Firstly compile the expression:
/*
	std::cout << q.expression << std::endl;
	
	auto compiled_res = compile_expression(q.expression);

	if (compiled_res.has_error_message)
	{
		std::cout << "Error occured while compiling the expression: " << compiled_res.error_message << std::endl;
		return compiled_res.error_message;
	}
	q.parsed_expression = std::move(compiled_res.particle);
*/
	std::unique_ptr<expression_tree> tree = nullptr;

	try {
		tree = make_expression_tree(q.expression);
	} catch (std::exception& e){
		return std::string(e.what());
	}


	// After that order the variables the way GA needs them
	/*
	size_t number_of_variables = q.variables.size();

	std::vector < std::pair<double, double> > ranges;
	std::vector<std::string> var_names;
	ranges.reserve(number_of_variables);
	var_names.reserve(number_of_variables);

	for (auto& range : q.variable_ranges) {
		var_names.emplace_back(range.first);
		ranges.emplace_back(range.second);
	}

	// Generate function to convert variable sequence to umap:
	auto convert_variable_sequence = [&](const std::vector<double>& variable_values) -> std::unordered_map<std::string, double>
	{
		std::unordered_map<std::string, double> res;
		for (size_t var_index = 0; var_index < number_of_variables; var_index++) res[var_names[var_index]] = variable_values[var_index];

		return res;
	};
	// Generate the function, that gives the error by the ordered variable values by putting thm to unoredered map (frmat for expression_particle):
	auto generated_error_function = [&](const std::vector<double>& variable_values) -> double
	{
		std::unordered_map < std::string, double > vars = convert_variable_sequence(variable_values);
		
		try {
			return tree->compute(vars);
		}
		catch (std::exception& e)
		{
			std::cout << e.what();
			throw;
		}
	};

	// Generate fitness function!
	auto generated_fitness_function = [&](const std::vector<double>& variable_values)
	{
		double function_result = generated_error_function(variable_values);
		
		if (function_result - q.target_minimum < 1e-10) return 2e+15;
		
		double fitness = 1 / abs(function_result - q.target_minimum);
		return fitness;
	};

	// Construct GA_params:

	GA_params params;

	params.allow_multithreading = false;
	params.exiting_fitness_value = 1e+15;
	
	auto all_countings = double(q.iterations);

	params.population_size = size_t(std::round(square(cbrt(all_countings))));
	params.epoch_num = size_t(std::round(cbrt(all_countings)));

	// Interesting parameters:
	params.hazing_percent = 0.3;

	params.target_gene_mutation_number = 0.3 * number_of_variables;
	params.mutation_percent_sigma = 0.05;
	params.cut_mutations = true;
	
	params.best_genome_percent = 0.2;
	params.elite_fit_pow = 2;
	params.parent_fit_pow = 0.3;
	
	params.mode_of_matting = GA::matting_mode::low_variance_genetic;

	// params.allow_multithreading = true;

	std::cout << "Ready to launch GA!" << std::endl;

	std::cout << "__________________________________________________" << std::endl;
	auto GA_res = GA::ga_optimize(generated_fitness_function, ranges, params);
	std::cout << "__________________________________________________" << std::endl;

	auto best_genome = GA_res.second; // vector of variable values in the same order to "var_names"
	std::unordered_map<std::string, double> GA_best_variable_values = convert_variable_sequence(best_genome);
	double GA_best_error = tree->compute(GA_best_variable_values);
	
	// Now apply gradient descent:

	auto GD_res = gradient_optimize(generated_error_function, best_genome, 0.003, q.iterations / 3);

	auto GD_best_variable_sequence = GD_res.second;
	auto GD_best_error = GD_res.first;
	auto GD_best_variable_values = convert_variable_sequence(GD_best_variable_sequence);

	std::cout << std::setprecision(15) << std::endl;
	std::cout << "GA error: " << GD_best_error << std::endl;
	std::cout << "GD error: " << GA_best_error << std::endl;
	// cout << std::boolalpha << (GA_best_error == GD_best_error) << endl;
	
	if (!isnan(GD_best_error) && GD_best_error < GA_best_error)
	{
		std::pair<std::unordered_map<std::string, double>, double> total_res = { GD_best_variable_values, GD_best_error };
		return total_res;
	}
	else
	{
		std::pair<std::unordered_map<std::string, double>, double> total_res = { GA_best_variable_values, GA_best_error };
		return total_res;
	}
	*/

	std::pair<std::unordered_map<std::string, double>, double> res;
	try {
		res = combi_optimize(tree.get(), q.variable_ranges, q.variables, q.target_minimum, q.iterations);
	} catch(std::exception& e) {
		return std::string(e.what());
	}

	res.first = pretty_a_variable_pack(res.first);
	res.second = pretty_a_number(res.second);

	return res;
}

std::pair<std::unordered_map<std::string, double>, double>
combi_optimize (expression_tree *tree, const std::unordered_map<std::string, std::pair<double, double>> &variable_ranges,
                const std::unordered_set<std::string> &variables, double target_minimum, size_t iterations)
{
	size_t number_of_variables = variables.size();


	/// Firstly, order the variables the way GA needs them

	std::vector < std::pair<double, double> > ranges;
	std::vector<std::string> var_names;
	ranges.reserve(number_of_variables);
	var_names.reserve(number_of_variables);

	for (auto& range : variable_ranges) {
		var_names.emplace_back(range.first);
		ranges.emplace_back(range.second);
	}

	/// Count the derivatives:
	std::vector<std::unique_ptr<expression_tree>> first_derivative_trees;
	first_derivative_trees.reserve(number_of_variables);
	for (size_t dim_number = 0; dim_number < number_of_variables; ++dim_number) {
		auto this_tree = tree->generate_derivative_tree(var_names[dim_number]);
		first_derivative_trees.emplace_back(this_tree);
	}

	std::vector<std::unique_ptr<expression_tree>> second_derivative_trees;
	second_derivative_trees.reserve(number_of_variables);
	for (size_t dim_number = 0; dim_number < number_of_variables; ++dim_number) {
		auto this_tree = first_derivative_trees[dim_number]->generate_derivative_tree(var_names[dim_number]);
		second_derivative_trees.emplace_back(this_tree);
	}

	double function_tree_nodes = std::sqrt(double(tree->count_operation_number() * tree->count_node_number()));
	double first_derivative_tree_nodes = std::sqrt(double(first_derivative_trees[0]->count_operation_number() * first_derivative_trees[0]->count_node_number()));
	double second_derivative_tree_nodes = std::sqrt(double(second_derivative_trees[0]->count_operation_number() * second_derivative_trees[0]->count_node_number()));
	printvar(function_tree_nodes);
	printvar(first_derivative_tree_nodes);
	printvar(second_derivative_tree_nodes);

	/// Generate function to convert variable sequence to umap:
	auto convert_variable_sequence = [&](const std::vector<double>& variable_values) -> std::unordered_map<std::string, double>
	{
		std::unordered_map<std::string, double> res;
		for (size_t var_index = 0; var_index < number_of_variables; var_index++) res[var_names[var_index]] = variable_values[var_index];

		return res;
	};

	/// Generate the function, that gives the error by the ordered variable values by putting thm to unoredered map (frmat for expression_particle):
	auto generated_error_function = [&](const std::vector<double>& variable_values) -> double
	{
		std::unordered_map < std::string, double > vars = convert_variable_sequence(variable_values);

		try {
			return tree->compute(vars);
		}
		catch (std::exception& e)
		{
			std::cout << e.what();
			throw;
		}
	};

	/// Function, that gives us the first gradient:
	auto generated_first_gradient = [&first_derivative_trees, &convert_variable_sequence]
			(const std::vector<double>& variable_values) -> std::vector<double>
	{
		std::vector<double> res(variable_values.size());
		std::unordered_map < std::string, double > vars = convert_variable_sequence(variable_values);

		for (size_t dim_number = 0; dim_number < variable_values.size(); ++dim_number) {
			res[dim_number] = first_derivative_trees[dim_number]->compute(vars);
		}

		return res;
	};

	/// Function, that gives us the second gradient:
	auto generated_second_gradient = [&second_derivative_trees, &convert_variable_sequence]
			(const std::vector<double>& variable_values) -> std::vector<double>
	{
		std::vector<double> res(variable_values.size());
		std::unordered_map < std::string, double > vars = convert_variable_sequence(variable_values);

		for (size_t dim_number = 0; dim_number < variable_values.size(); ++dim_number) {
			res[dim_number] = second_derivative_trees[dim_number]->compute(vars);
		}

		return res;
	};

	/// Generate fitness function!
	auto generated_fitness_function = [&](const std::vector<double>& variable_values)
	{
		double function_result = generated_error_function(variable_values);

		if (function_result - target_minimum < 1e-10) return 2e+15;

		double fitness = 1 / abs(function_result - target_minimum);
		return fitness;
	};

	/// Construct GA_params:
	GA_params params;

	params.allow_multithreading = false;
	params.exiting_fitness_value = 1e+15;

	auto all_countings = double(iterations);

	params.population_size = size_t(std::round(square(cbrt(all_countings)) * 2));
	params.epoch_num = size_t(std::round(cbrt(all_countings) / 2));

	// Interesting parameters:
	params.hazing_percent = 0.3;

	params.target_gene_mutation_number = 0.3 * number_of_variables;
	params.mutation_percent_sigma = 0.05;
	params.cut_mutations = true;

	params.best_genome_percent = 0.2;
	params.elite_fit_pow = 2;
	params.parent_fit_pow = 0.3;

	params.mode_of_matting = GA::matting_mode::low_variance_genetic;

	// params.allow_multithreading = true;

	std::cout << "Ready to launch GA!" << std::endl;

	std::cout << "__________________________________________________" << std::endl;
	std::pair<double, GA::genome> GA_res;
	try {
		{
			Timer ga_timer("GA processing");
			GA_res = GA::ga_optimize(generated_fitness_function, ranges, params);
			double per_iteration = ga_timer.get_time() / double(params.population_size * params.epoch_num);
			std::cout << "(" << per_iteration << "ms per one function computation, " << params.population_size * params.epoch_num << " computations" << ")\n";
		}
	} catch(std::exception& e) {
		throw std::runtime_error("Error occurred while optimization: "s + e.what());
	}
	std::cout << "__________________________________________________" << std::endl;

	auto best_genome = GA_res.second; // vector of variable values in the same order to "var_names"
	std::unordered_map<std::string, double> GA_best_variable_values = convert_variable_sequence(best_genome);
	double GA_best_error = tree->compute(GA_best_variable_values);

	/// Now apply gradient descent:
	// TODO!| Automatic learning_rate tuning!!!


	std::pair < double, std::vector <double> > GD_res;

	// size_t GD_iterations = iterations / (6.25 + 1);
	double GD_coolness_coefficient = 0.5;
	size_t GD_iterations = std::round(GD_coolness_coefficient * iterations
			* function_tree_nodes / (function_tree_nodes + first_derivative_tree_nodes) / number_of_variables);

	try {
		{
			Timer gd_timer("GD processing");
			GD_res = gradient_optimize(generated_error_function, generated_first_gradient, best_genome, 0.003, GD_iterations);
			double per_iteration = gd_timer.get_time() / double(GD_iterations);
			std::cout << "(" << per_iteration << "ms per one iteration, " << GD_iterations << " iterations" << ")\n";
		}
	} catch(std::exception& e) {
		throw std::runtime_error("Error occurred while optimization: "s + e.what());
	}
	std::cout << "__________________________________________________" << std::endl;


	auto GD_best_variable_sequence = GD_res.second;
	auto GD_best_error = GD_res.first;
	auto GD_best_variable_values = convert_variable_sequence(GD_best_variable_sequence);

	std::cout << std::setprecision(15) << std::endl;
	std::cout << "GA error: " << GD_best_error << std::endl;
	std::cout << "GD error: " << GA_best_error << std::endl;

	std::vector<double> best_2nd_step_variable_sequence;

	/*
	if (!isnan(GD_best_error) && GD_best_error < GA_best_error)
	{
		std::pair<std::unordered_map<std::string, double>, double> total_res = { GD_best_variable_values, GD_best_error };
		return total_res;
	}
	else
	{
		std::pair<std::unordered_map<std::string, double>, double> total_res = { GA_best_variable_values, GA_best_error };
		return total_res;
	}
	*/

	if (!isnan(GD_best_error) && GD_best_error < GA_best_error)
	{
		best_2nd_step_variable_sequence = GD_best_variable_sequence;
	}
	else
	{
		best_2nd_step_variable_sequence = best_genome;
	}

	/// Improve best result by Newton method:
	std::pair < double, std::vector <double> > newton_output;

	// size_t newton_iterations = iterations / (40 + 6.25 + 1);
	double Newton_coolness_coefficient = 1.5;
	size_t newton_iterations = std::round(Newton_coolness_coefficient * iterations
			* function_tree_nodes / (function_tree_nodes + first_derivative_tree_nodes + second_derivative_tree_nodes)
				/ number_of_variables); // Not "/ square(number_of_variables)" yet.

	try {
		Timer gd_timer("Newton processing");

		newton_output = newton_optimize(
				generated_error_function, generated_first_gradient, generated_second_gradient,
				best_2nd_step_variable_sequence, 1., newton_iterations
		);

		double per_iteration = gd_timer.get_time() / double(newton_iterations);
		std::cout << "(" << per_iteration << "ms per one iteration, " << newton_iterations << " iterations" << ")\n";
	} catch(std::exception& e) {
		throw std::runtime_error("Error occurred while optimization: "s + e.what());
	}
	auto [newton_best_error, newton_best_variable_sequence] = newton_output;
	std::cout << "__________________________________________________" << std::endl;


	auto newton_best_variable_values = convert_variable_sequence(newton_best_variable_sequence);


	std::cout << console_colors::yellow << "__________________________________________________" << console_colors::simple << std::endl;
	std::cout << "GA output: " << GA_best_error << " " << GA_best_variable_values << std::endl;
	std::cout << "GD output: " << GD_best_error << " " << GD_best_variable_values << std::endl;
	std::cout << "Newton output: " << newton_best_error << " " << newton_best_variable_values << std::endl;
	std::cout << console_colors::yellow << "__________________________________________________" << console_colors::simple << std::endl;

	/// Choosing the best result:
	std::vector<double> best_resultive_variable_sequence;

	if (!isnan(GD_best_error) && GD_best_error < GA_best_error)
	{
		best_resultive_variable_sequence = std::move(newton_best_variable_sequence);
	}
	else
	{
		best_resultive_variable_sequence = std::move(best_2nd_step_variable_sequence);
	}

	auto best_resultive_variable_values = convert_variable_sequence(best_resultive_variable_sequence);
	auto best_resultive_error = generated_error_function(best_resultive_variable_sequence);

	return { best_resultive_variable_values, best_resultive_error };
	// return { newton_best_variable_values, newton_best_error };
}
