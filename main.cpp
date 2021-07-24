#include <pythonic.h>
#include <tests/optimization_tree_tests/raw_optimization_tree_test.h>
#include <tests/other_tests.h>

#include "query_processor.h"

#include "benchmark/length_benchmark.h"
#include "math_bot_config.h"


void test_query_processing(const std::string& process_name, const std::string& dir_name, const std::string& file_name) {
	auto path = (math_bot_query_path / dir_name / file_name).string();

	const char* emulated_argv[] = {
			"",
			process_name.c_str(),
			path.c_str()
	};

	std::cout << console_colors::green
		<< "Testing math bot query: " << process_name
	<< console_colors::remove_all_colors << std::endl;

	process_user_input(3, emulated_argv);
}


void test_process_plotting(){
	std::string dir_with_examples = "_examples";
	std::string example_filename = "plotting_example.json";

	const std::string& this_filename = example_filename;
	const std::string& this_dir = dir_with_examples;

	test_query_processing(
			"plot",
			this_dir,
			this_filename
			);
}

void test_process_optimizing(){
	std::string dir_with_examples = "_examples";
	std::string example_filename = "optimizing_example.json";

//	const std::string& this_dir = "215659697";
//	const std::string& this_filename = "125.json";

	const std::string& this_dir = dir_with_examples;
	const std::string& this_filename = example_filename;

	test_query_processing(
			"optimize",
			this_dir,
			this_filename
	);
}

void test_process_solving(){
	std::string dir_with_examples = "_examples";
	std::string example_filename = "solving_example.json";
	std::string non_solvable_filename = "non_solvable_example.json";

	// const std::string& this_dir = "447186473";
	// const std::string& this_filename = "1311.json";

	const std::string& this_dir = dir_with_examples;
	const std::string& this_filename = example_filename;
//	const std::string& this_filename = non_solvable_filename;

	test_query_processing(
			"solve",
			this_dir,
			this_filename
	);
}

int main (int argc, const char **argv)
{
	set_utf8_in_console();


	if (argc == 1) {
		/// Testing:

		// plot_expression_derivative_length_graph();
		// test_raw_optimization_tree();
		// rubbish_tests();

		// test_process_optimizing();
		// test_process_plotting();
		test_process_solving();
	}
	else {
		/// Actual work:
		process_user_input(argc, reinterpret_cast<const char **>(argv));
	}

	return 0;
}
