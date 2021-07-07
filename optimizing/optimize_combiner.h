//
// Created by Vova on 01.11.2020.
//

#pragma once
#include <optimizing/OptimizationBlock.h>


class OptimizationTree
{
public:
	enum class type
	{
		opt_block,
		seq_container,
		par_container
	};

private:
	type m_type;
	std::vector<OptimizationTree> children;
	std::unique_ptr<OptimizationBlock> m_block = nullptr;

public:

	explicit OptimizationTree(const json& source, type parent_container_type);
};


