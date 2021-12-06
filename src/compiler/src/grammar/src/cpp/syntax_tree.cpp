#include "syntax_tree.h"

Syntax_tree::Syntax_tree() {}
Syntax_tree::Syntax_tree(const Construction& construction) : tree(construction) {}

Syntax_tree::const_traverser Syntax_tree::append_subtree(Syntax_tree&& subtree) {
	return tree.append_child_subtree(tree.entrance(), std::move(subtree.tree));
}

Syntax_tree::const_traverser Syntax_tree::entrance()  const noexcept { return tree.entrance(); }
Syntax_tree::const_traverser Syntax_tree::centrance() const noexcept { return tree.entrance(); }
