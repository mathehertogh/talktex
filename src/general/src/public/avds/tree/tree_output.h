/*
	Tree output - Additional functions for pretty-printing generic trees

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

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>
#include "tree_algorithms.h"

namespace avds::tree {

namespace detail {

template<typename T>
inline std::string default_print_func(const T& value) {
	std::stringstream ss;
	ss << value;
	return ss.str();
}

template<typename Traverser, typename String_from_tree_value_func>
void print_simple_recursive(
	std::ostream& out,
	const Traverser& t,
	String_from_tree_value_func print_func,
	unsigned int tab_chars_per_depth,
	size_t depth
) {
	out << std::setw(depth*tab_chars_per_depth) << "" << print_func(*t) << '\n';
	depth += 1;
	for (auto ct = t.begin(); ct != t.end(); ++ct) {
		print_simple_recursive(out, ct, print_func, tab_chars_per_depth, depth);
	}
}

template<typename Traverser, typename String_from_tree_value_func>
unsigned int width(
	const Traverser& t,
	String_from_tree_value_func print_func,
	unsigned int sep_width
) {
	if (t.is_leaf()) {
		return print_func(*t).length();
	}
	unsigned int length = 0;
	for (auto ct = t.begin(); ct != t.end(); ++ct) {
		length += width(ct, print_func, sep_width);
		if (ct != t.end()-static_cast<typename Traverser::difference_type>(1)) {
			length += sep_width;
		}
	}
	return std::max<unsigned int>(print_func(*t).length(), length);
}


template<typename Traverser, typename String_from_tree_value_func>
void prerender_horizontal_recursive(
	const Traverser& t,
	String_from_tree_value_func print_func,
	unsigned int sep_width,
	char char_l_branch,
	char char_r_branch,
	char char_t_branch,
	char char_b_branch,
	char char_p_branch,
	char char_hor,
	char char_vert,
	char char_sep,
	std::vector<std::string>& prerender_rows,
	size_t depth,
	size_t total_depth,
	unsigned int padding_left,
	unsigned int padding_right
) {
	// Don't even look at this

	std::string print = print_func(*t);

	if (t.is_leaf()) {
		prerender_rows[depth*4] +=
			  std::string(padding_left, char_sep)
			+ print
			+ std::string(sep_width+padding_right, char_sep);
		//Fill empty space below leaf.
		for (size_t d = depth + 1; d != total_depth; ++d) {
			for (int i = 0; i <= 3; ++i) {
				prerender_rows[d*4 - i] += std::string(
					padding_left + print.length() + padding_right + sep_width,
					char_sep
				);
			}
		}
		return;
	}
	// From here we are not at a leaf

	// Doing this for every recursion step is very inefficient, but whatever
	std::vector<unsigned int> mid_points;
	mid_points.reserve(t.child_count());
	unsigned int t_width = 0;
	for (auto ct = t.begin(); ct != t.end(); ++ct) {
		unsigned int tree_width = width(ct, print_func, sep_width);
		mid_points.emplace_back(t_width + (tree_width+1)/2); // Round up here
		t_width += tree_width;
		if (ct != t.end()-static_cast<typename Traverser::difference_type>(1)) {
			t_width += sep_width;
		}
	}

	unsigned int new_left_padding  = padding_left;
	unsigned int new_right_padding = padding_right;
	int diff = print.length() - t_width;
	if (diff > 0) { // If the root is wider than the children combined
		t_width = print.length();
		new_left_padding  += diff/2;
		new_right_padding += diff - diff/2;
	}
	for (auto& midPoint : mid_points) {
		midPoint += new_left_padding;
	}


	// I don't even understand it anymore

	unsigned int left_width  = ((t_width - print.length())/2) + padding_left;
	unsigned int right_width = (t_width - print.length() - left_width + sep_width) + padding_right;

	prerender_rows[depth*4] +=
		  std::string(left_width, char_sep)
		+ print
		+ std::string(right_width, char_sep);


	left_width = (t_width-1)/2 + padding_left;
	right_width = t_width-1 - left_width + sep_width + padding_right;

	prerender_rows[depth*4 + 1] +=
		  std::string(left_width, char_sep)
		+ char_vert
		+ std::string(right_width, char_sep);

	unsigned int step = mid_points.front()-1;
	prerender_rows[depth*4+2] += std::string(step, char_sep) + char_l_branch;
	prerender_rows[depth*4+3] += std::string(step, char_sep) + char_vert;
	for (auto it = mid_points.begin() + 1; it < mid_points.end(); ++it) {
		step = *it - *(it-1) - 1;
		prerender_rows[depth*4+2] += std::string(step, char_hor) + char_t_branch;
		prerender_rows[depth*4+3] += std::string(step, char_sep) + char_vert;
	}
	prerender_rows[depth*4+2].back() = char_r_branch;
	prerender_rows[depth*4+2] +=
		std::string(t_width - mid_points.back() + sep_width, char_sep);
	prerender_rows[depth*4+3] +=
		std::string(t_width - mid_points.back() + sep_width, char_sep);
	char& b = *(prerender_rows[depth*4+2].end() - right_width - 1);
	b = (
		t.child_count()==1? char_vert : (b != char_vert? char_p_branch : char_t_branch)
	);

	// Recursive calls
	for (auto ct = t.begin(); ct != t.end(); ++ct) {
		prerender_horizontal_recursive(
			ct, print_func, sep_width,
			char_l_branch, char_r_branch, char_t_branch, char_b_branch, char_p_branch,
			char_hor, char_vert, char_sep,
			prerender_rows, depth+1, total_depth,
			(ct == t.begin() ? new_left_padding : 0),
			(
				ct==t.end()-static_cast<typename Traverser::difference_type>(1)
				? new_right_padding : 0
			)
		);
	}
}

} // namespace detail

template<
	typename Traverser,
	typename String_from_tree_value_func
		= std::string (*)(const typename Traverser::value_type &)
>
void print_simple(
	std::ostream& out,
	const Traverser& entrance,
	String_from_tree_value_func print_func
		= detail::default_print_func<typename Traverser::value_type>,
	unsigned int tab_chars_per_depth = 2,
	char tab_char = ' '
) {
	if (!entrance.is_valid()) {
		return;
	}

	char oldFill = out.fill();
	out.fill(tab_char);
	detail::print_simple_recursive(
		out, entrance, print_func, tab_chars_per_depth, 0
	);
	out.fill(oldFill);
}

template<
	typename Traverser,
	typename String_from_tree_value_func
		= std::string (*)(const typename Traverser::value_type &)
>
void print_horizontal(
	std::ostream& out,
	const Traverser& entrance,
	String_from_tree_value_func print_func
		= detail::default_print_func<typename Traverser::value_type>,
	unsigned int sep_width = 3,
	char char_l_branch = '.',
	char char_r_branch = '.',
	char char_t_branch = '.',
	char char_b_branch = ':',
	char char_p_branch = ':',
	char char_hor      = '.',
	char char_vert     = ':',
	char char_sep      = ' '
) {
	if (!entrance.is_valid()) {
		return;
	}

	size_t tree_depth = depth(entrance);
	std::vector<std::string> prerender_rows(tree_depth*4 - 3);

	detail::prerender_horizontal_recursive(
		entrance, print_func, sep_width,
		char_l_branch, char_r_branch, char_t_branch, char_b_branch, char_p_branch,
		char_hor, char_vert, char_sep,
		prerender_rows, 0, tree_depth, 0, 0
	);

	for (const auto& row : prerender_rows) {
		out << row << '\n';
	}
}

} // namespace avds::tree
