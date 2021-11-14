#pragma once

#include <utility>
#include "avds/tree/tree.h"
#include "construction.h"

class Syntax_tree {
public:
	using const_traverser = avds::tree::Tree<Construction>::const_traverser;

	Syntax_tree();
	Syntax_tree(const Construction& construction);

	/** Constructs the root Construction object in-place using [args] */
	template<typename... Args>
	Syntax_tree(Args&&... args);

	/** The tree may not be empty */
	const_traverser append_subtree(Syntax_tree&& subtree);

	const_traverser entrance()  const noexcept;
	const_traverser centrance() const noexcept;

private:
	avds::tree::Tree<Construction> tree;
};

template<typename... Args>
Syntax_tree::Syntax_tree(Args&&... args) : tree(std::forward<Args>(args)...) {}
