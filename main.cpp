#include <pythonic.h>

#include "query_processor.h"

#include "benchmark/length_benchmark.h"

void test_process_plotting(){
	// std::string test_json = *read_file(R"(D:\Projects\Math_bot\queries\_examples\plot_example.json)");
	// std::string test_json = *read_file(R"(D:\Projects\Math_Bot\queries\1\1.json)");

	// json j = json::parse(test_json);
	// auto q = j.get<plotting_query>();

	// std::cout << json(q).dump(4) << std::endl;

	// const char* argv[] = {"", "plot", R"(D:\Projects\Math_bot\queries\_examples\plot_example.json)"};
	const char* argv[] = {"", "plot", R"(D:\Projects\Math_Bot\queries\1\1.json)"};

	process_user_input(3, argv);
}

void test_process_optimizing(){
	const char* argv[] = {"", "optimize", R"(D:\Projects\Math_Bot\queries\215659697\2553.json)"};
	// const char* argv[] = {"", "optimize", R"(D:\Projects\Math_Bot\queries\_examples\optimizing_example.json)"};
	process_user_input(3, argv);
}

void test_process_solving(){
	// const char* argv[] = {"", "solve", R"(D:\Projects\Math_bot\queries\_examples\solving_example.json)"};
	// process_user_input(3, argv);

	std::cout << console_colors::green << "______________________________________________" << console_colors::simple << std::endl;

	const char* argv2[] = {"", "solve", R"(D:\Projects\Math_Bot\queries\447186473\1311.json)"};
	process_user_input(3, argv2);
}

int main (int argc, const char **argv)
{
	// setlocale(LC_ALL, "Russian");

	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);

	system("chcp 65001");



	if (argc == 1) {
		// plot_expression_derivative_length_graph();

		// test_process_optimizing();
		test_process_plotting();
		// test_process_solving();
	}
	else {
		process_user_input(argc, reinterpret_cast<const char **>(argv));
	}

	return 0;
}
