//
// Created by Vova on 01.11.2020.
//

#pragma once

#include <pythonic.h>
#include "GA/GA_optimizer.h"
#include "expression_parsing.h"


constexpr const char* math_bot_pch_file_path = __FILE__;
inline std::filesystem::path math_bot_base_dir = safe_get_parent(std::string{math_bot_pch_file_path});
		// get_path_folder(math_bot_pch_file_path);

inline const auto math_bot_query_path = math_bot_base_dir.parent_path() / "queries";
inline const auto math_bot_result_path = math_bot_base_dir.parent_path() / "results";

