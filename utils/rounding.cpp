//
// Created by Vova on 02.07.2020.
//

#include "rounding.h"

std::unordered_map<std::string, double> pretty_a_variable_pack (const std::unordered_map<std::string, double>& variable_pack)
{
	auto res = variable_pack;

	for (auto& pair : res) {
		pair.second = pretty_a_number(pair.second);
	}

	return res;
}

double pretty_a_number (double number)
{
	double exp = 1e+5;
	auto answer = std::round(number * exp) / exp;
	if (answer == 0) answer = 0;

	return answer;
}
