/*
	Tree algorithms - Some standard algorithms for generic trees

	================================================================================

	MIT License

	Copyright (c) 2021 Arthur van der Staaij

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/

#pragma once

#include <cstddef>

namespace avds::tree {

namespace detail {

template<typename Traverser1, typename Traverser2>
bool equal_tree_recursive(const Traverser1& t1, const Traverser2& t2) {
	if (*t1 != *t2 || t1.child_count() != t2.child_count()) {
		return false;
	}
	auto ct1 = t1.begin();
	auto ct2 = t2.begin();
	while (ct1 != t1.end()) {
		if (!equal_subtree_recursive(ct1, ct2)) {
			return false;
		}
		++ct1;
		++ct2;
	}
	return true;
}

} // namespace detail

/**
 * Returns whether the trees pointed to by [t1] and [t2] are equal.
 * That is: whether they have the same tree structure and their elements compare as equal.
 */
template<typename Traverser1, typename Traverser2>
bool equal_tree(const Traverser1& t1, const Traverser2& t2) {
	if (!t1) {
		return !t2;
	}
	else {
		return (t2) && detail::equal_tree_recursive(t1, t2);
	}
}

/** Returns the amount of nodes in the tree that [t] points to */
template<typename Traverser>
size_t node_count(const Traverser& t) {
	if (!t) {
		return 0;
	}
	// TODO: Maybe there is an std algorithm for this.
	size_t result = 1;
	for (auto ct = t.begin(); ct != t.end(); ++ct) {
		result += node_count(ct);
	}
	return result;
}

/** Returns the amount of leaf nodes in the tree that [t] points to */
template<typename Traverser>
size_t leaf_count(const Traverser& t) {
	if (!t) {
		return 0;
	}
	if (t.is_leaf()) {
		return 1;
	}
	//TODO: Maybe there is an std algorithm for this.
	size_t result = 0;
	for (auto ct = t.begin(); ct != t.end(); ++ct) {
		result += leaf_count(ct);
	}
	return result;
}

/** Returns the depth of the tree that [t] points to */
template<typename Traverser>
size_t depth(const Traverser& t) {
	if (!t) {
		return 0;
	}
	size_t maxDepth = 0;
	for (auto ct = t.begin(); ct != t.end(); ++ct) {
		size_t d = depth(ct);
		if (d > maxDepth) {
			maxDepth = d;
		}
	}
	return maxDepth + 1;
}

} // namespace avds::tree
