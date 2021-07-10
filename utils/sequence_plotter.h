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

std::string format_opt_block_sequence (const std::vector<std::pair<BlockLinker, std::string>>& blocks_with_connections)
{
	std::string res;
	for (const auto& block_with_connection : blocks_with_connections) {
		res += std::string(block_with_connection.first);
		res += " " + block_with_connection.second + " ";
	}

	return res;
}
