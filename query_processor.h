#pragma once
#include <pythonic.h>
#include <expression_parsing.h>



struct base_query
{
	std::string user_name = "Not Vova";
	std::string target_path;
	std::string identifier = "None";

	std::string tree_name = "GDs_with_learning_rates"; // TODO: add ability to change this from UI!
};

void to_json(json& j, const base_query& query);
void from_json(const json& j, base_query& query);



struct optimization_query : base_query
{
	std::string expression;

	std::unordered_set<std::string> variables;
	std::unordered_map<std::string, std::pair<double, double>> variable_ranges;
	double target_minimum = 0;

	size_t iterations = 1000000;
};

struct equation_solving_query : base_query // TODO
{
	std::string expression;

	std::unordered_set<std::string> variables;
	std::unordered_map<std::string, std::pair<double, double>> variable_ranges;

	size_t iterations = 1000000;
};

void from_json(const json& j, equation_solving_query& query);



struct plotting_query : base_query
{
	std::vector<std::string> functions;

	double from = 0;
	double to = 50;

	size_t steps = 1;
	std::string text_path;
};

void to_json(json& j, const plotting_query& query);
void from_json(const json& j, plotting_query& query);

optimization_query load_optimization_query_from_json(const std::string& filename);


void process_user_input(int argc, const char* argv[]);
