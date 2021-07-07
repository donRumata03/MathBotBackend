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
	void run(
			const std::function<double (const std::vector<double>&)>& error_function,
			const std::function<double (const std::vector<double>&)>& fitness_function,
			const std::function<std::vector<double> (const std::vector<double>&)>& first_gradient,
			const std::function<std::vector<double> (const std::vector<double>&)>& second_gradient
	);
};


