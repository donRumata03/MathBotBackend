#include <pythonic.h>
#include "query_processor.h"

#include "optimizing/optimize_query_processor.h"
#include "plotting/plot_manager.h"
#include "solving/solver.h"


void process_user_input(const int argc, const char* argv[])
{
	// Test:
	// for (size_t index = 0; index < argc; index++) cout << std::endl << argv[index] << std::endl;
	
	// Get the input filename from command line arg:
	if (argc == 3) // 3 because there is the name of the file first, command name and the file path with parameters.
	{
		std::string command = argv[1];
		std::string filename = argv[2];

		std::cout << "Command: " << command << std::endl;
		std::cout << "Filename: " << filename << std::endl;
		
		if (command == "optimize") {
			auto query = load_optimization_query_from_json(filename);
			auto res = process_optimization_query(query);

			if (res.has_error_message)
			{
				std::string answer = "В результате обработки Вашего запроса возникла ошибка! Вот она: \"" + res.error_message + "\"\n";
				write_file<given_filename_encoding::utf8>(answer, query.target_path);
			}
			else {
				std::stringstream ss;
				ss << res.answer;
				std::string str_umap = ss.str();

				std::string answer = "Я смог найти минимум функции \"" + query.expression + "\" при этих значениях переменных: \n" +
						str_umap + "\nИтоговое значение функции в этой точке: " + std::to_string(res.best_value) + ".";
				write_file<given_filename_encoding::utf8>(answer, query.target_path);
			}
			std::string from_out_file = *read_file<given_filename_encoding::utf8>(query.target_path);
/*
			// recode::auto_recode(from_)

			// std::cout << recode::determine_encoding(from_out_file) << std::endl;

			// std::cout << recode::to_cp1251(from_out_file) << std::endl;
*/
			std::cout << "Output: " << "\"" << console_colors::bold << console_colors::blue <<
				from_out_file
			<< console_colors::remove_all_colors << "\"" << std::endl;
		}
		else if (command == "plot")
		{
			auto file_contents = *read_file<given_filename_encoding::utf8>(filename);
			json j = json::parse(file_contents);

			auto query = j.get<plotting_query>();

			auto response = process_plotting_query(query);

			if (response) {
				// Have some error!
				write_file<given_filename_encoding::utf8>(
						"Увы, я не могу за`plot`ить эту штуковину! Появилась ошибка: \""s + *response + "\"", query.text_path
				);

				std::cout << "There are some errors with plotting!" << std::endl;
				return;
			}

			write_file<given_filename_encoding::utf8>("", query.text_path);
		}
		else if (command == "solve")
		{
			// std::cout << "This command is being developed! :(..." << std::endl;

			auto file_contents = *read_file<given_filename_encoding::utf8>(filename);
			json j = json::parse(file_contents);

			auto query = j.get<equation_solving_query>();
			auto response = process_solving_query(query);


			if (std::holds_alternative<std::string>(response)) {
				/// There`s an error!
				std::string answer = "Не получилось решить уравнение! В ходе решения возникла ошбика: " + std::get<std::string>(response);
				write_file<given_filename_encoding::utf8>(answer, query.target_path);
			}
			else {
				/// Everything goes as expected:

				std::stringstream ss;
				ss << "Уравнение решено. Вот один из его корней: " << std::get<std::unordered_map<std::string, double>>(response);
				write_file<given_filename_encoding::utf8>(ss.str(), query.target_path);
			}

			// out_file.close();
			std::string from_out_file = *read_file<given_filename_encoding::utf8>(query.target_path);

			std::cout << "Output: " << "\"" << console_colors::bold << console_colors::blue <<
			          from_out_file
			          << console_colors::remove_all_colors << "\"" << std::endl;
		}
		else
		{
			std::cout << "Unknown command!" << std::endl;
		}
	}
	else
	{
		std::cout << "Usage: pass 2 argumentы: command name (i.e. \"optimize\") and the name of input json file with other parameters!" << std::endl;
	}
}




///////////////////////////////////////////////////////////////////////////////////
////////////////////////////		Loading queries 	 //////////////////////////
///////////////////////////////////////////////////////////////////////////////////

optimization_query load_optimization_query_from_json(const std::string& filename)
{
	optimization_query res;

	json file_data = json::parse(*read_file<given_filename_encoding::utf8>(filename));

	res.target_minimum = file_data["data"]["target_minimum"].get<double>();
	res.expression = recode::auto_recode(file_data["data"]["function"].get<std::string>(), encoding::cp1251);

	for (auto& var : file_data["data"]["variables"])
	{
		res.variables.insert(recode::to_cp1251(var.get<std::string>()));
	}
	for (auto& var: file_data["data"]["variable_ranges"].items())
	{
		res.variable_ranges[recode::to_cp1251(var.key())] = { var.value()[0].get<double>(), var.value()[1].get<double>() };
	}

	res.user_name = file_data["name"].get<std::string>();
	res.iterations = file_data["iterations"].get<size_t>();
	res.target_path = file_data["target_path"].get<std::string>();
	res.identifier = file_data["identifier"].get<std::string>();

	return res;
}


void to_json (json &j, const plotting_query &query)
{
	json common_json;

	to_json(common_json, *dynamic_cast<const base_query*>(&query));

	j = common_json;

	auto new_query = query;

	for (auto& function : new_query.functions) {
		function = recode::auto_recode(function, encoding::utf8);
	}

	j["data"] = {
			{ "functions", new_query.functions },

			{ "from", query.from },
			{ "to", query.to },
			{ "steps", query.steps },
			{ "text_path", query.text_path },
	};

}

void from_json (const json &j, plotting_query &query)
{
	auto& fake_base = *reinterpret_cast<base_query*>(&query);
	from_json(j, fake_base);

	query.to = j["data"]["to"].get<double>();
	query.from = j["data"]["from"].get<double>();
	query.steps = j["data"]["steps"].get<size_t>();
	query.functions = j["data"]["functions"].get<std::vector<std::string>>();

	for (auto &function : query.functions) {
		function = recode::auto_recode(function, encoding::cp1251);
	}

	query.text_path = j["text_path"].get<std::string>();
}

/// _____________________________________

void to_json (json &j, const base_query &query)
{
	j = {
			{ "identifier", query.identifier },
			{ "target_path", query.target_path },
			{ "name", query.user_name },
	};
}

void from_json (const json &j, base_query &query)
{
	query = {
			j["name"].get<std::string>(),
			j["target_path"].get<std::string>(),
			j["identifier"].get<std::string>(),
	};
}

/// _____________________________________

void from_json(const json& j, equation_solving_query& query){
	auto& fake_base = *reinterpret_cast<base_query*>(&query);
	from_json(j, fake_base);

	auto& data = j["data"];

	query.expression = recode::auto_recode(data["function"].get<std::string>(), encoding::cp1251);
	query.iterations = j["iterations"].get<size_t>();

	// Complicated things with strings, that need to be encoded:
	auto temp_variables = data["variables"].get<std::unordered_set<std::string>>();
	auto temp_variable_ranges = data["variable_ranges"].get<std::unordered_map<std::string, std::pair<double, double>>>();

	// Recode from utf8 to cp1251:
	for (auto& p : temp_variables) {
		query.variables.insert(recode::to_cp1251(p));
	}

	for (auto& p : temp_variable_ranges) {
		query.variable_ranges.emplace(recode::to_cp1251(p.first), p.second);
	}
}

