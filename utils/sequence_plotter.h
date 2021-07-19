//
// Created by Vova on 10.07.2021.
//

#pragma once

#include "math_bot_config.h"

enum class BlockLinker
{
	seq,
	par
};

inline std::map<BlockLinker, std::string> block_linker_to_string = {
		{ BlockLinker::par, "⥤" },
		{ BlockLinker::seq, "->" },
};

//operator std::string (const BlockLinker& b) const {
//	if (b == BlockLinker::par) {
//		return "⥤";
//	}
//	return "->";
//}

inline std::string format_opt_block_sequence (const std::vector<std::variant<BlockLinker, std::string>>& blocks_and_connections)
{
	std::string res;
	bool is_start = true;
	for (const auto& block_or_connection : blocks_and_connections) {
		if (not is_start) res += " ";
		else is_start = false;


		if (std::holds_alternative<BlockLinker>(block_or_connection)) {
			res += block_linker_to_string[std::get<BlockLinker>(block_or_connection)];
		}
		else {
			res += std::get<std::string>(block_or_connection);
		}
	}

	return res;
}
