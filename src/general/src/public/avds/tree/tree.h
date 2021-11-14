/*
	Tree - A type-generic tree

	STILL IN EARLY DEVELOPMENT

	Tree is a container for storing data in a tree structure. It represents a
	dynamic tree: tree nodes can have an arbitrary number of child nodes.

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

#include <stdexcept>
#include <type_traits>
#include <vector>

#include "../deep_ptr.h"

namespace avds::tree {

//==================================================================================================
// Forward declarations
//==================================================================================================

template<typename T>
class Tree;

template<typename T>
void swap(Tree<T>&, Tree<T>&) noexcept;

template<typename T>
bool operator==(const Tree<T>&, const Tree<T>&);

template<typename T>
bool operator!=(const Tree<T>&, const Tree<T>&);

template<typename T>
void swap(typename Tree<T>::Node&, typename Tree<T>::Node&) noexcept;

//==================================================================================================
// Tree class
//==================================================================================================

/**
 * A container for storing data in a tree structure
 */
template<typename T>
class Tree {
public:
	template<bool Const> class Traverser; // Forward declaration
	using traverser = Traverser<false>;
	using const_traverser = Traverser<true>;

	//----------------------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------------------

	/** Constructs an empty tree */
	Tree();

	/** Constructs a tree with the given value at its root */
	Tree(const T& root_value);

	/** Constructs a tree with the given value at its root */
	Tree(T&& root_value);

	/** Constructs a tree with a root element that is constructed in-place using [args] */
	template<typename... Args>
	Tree(Args&&... args);

	/** Constructs a copy of the tree pointed to by [t] */
	Tree(const const_traverser& t);

	//----------------------------------------------------------------------------------------------
	// Methods
	//----------------------------------------------------------------------------------------------

	/** Returns whether the tree is empty */
	bool empty() const noexcept;

	/** Returns the value at the root of the tree. If the tree is empty, the result is undefined. */
	T& root();
	/** Returns the value at the root of the tree. If the tree is empty, the result is undefined. */
	const T& root() const;

	/**
	 * Returns a new Tree object containing the subtree at the specified index.
	 * If the tree contains no subtree with the specified index, the result is undefined.
	 */
	Tree subtree(size_t child_index) const;

	/** Returns a traverser pointing to the root of the tree */
	traverser entrance() noexcept;
	/** Returns a traverser pointing to the root of the tree */
	const_traverser entrance() const noexcept;
	/** Returns a const traverser pointing to the root of the tree */
	const_traverser centrance() const noexcept;

	/**
	 * Ensures that the child capacity of the node pointed at by [t] is at least [n].
	 * Reallocates storage if the current capacity is less than [n].
	 */
	template<bool Const>
	void reserve_children(const Traverser<Const>& t, size_t n);

	/**
	 * Inserts a new node with the given value before the node pointed at by [t].
	 * [t] may not be a past-the-end traverser, except if the tree is empty (this limitation is for
	 * performance reasons).
	 * Returns a traverser pointing to the inserted node.
	 */
	template<bool Const>
	traverser insert(const Traverser<Const>& t, const T& value);

	/**
	 * Inserts a new node with the given value before the node pointed at by [t].
	 * [t] may not be a past-the-end traverser, except if the tree is empty (this limitation is for
	 * performance reasons).
	 * Returns a traverser pointing to the inserted node.
	 */
	template<bool Const>
	traverser insert(const Traverser<Const>& t, T&& value);

	template<bool Const, typename... Args>
	traverser emplace(const Traverser<Const>& t, Args&&... args);

	// TODO: range insert
	/*
	template<bool Const, typename InputIt>>
	traverser insert(const Traverser<Const>& t, InputIt begin, InputIt end);
	*/

	/**
	 * Copies and inserts the tree pointed at by [subtree] before the node pointed at by [t].
	 * [t] may not be a past-the-end traverser, except if the tree is empty (this limitation is for
	 * performance reasons).
	 * Returns a traverser pointing to the root of the inserted subtree.
	 */
	template<bool Const, typename Other_traverser>
	traverser insert_subtree(const Traverser<Const>& t, Other_traverser subtree);

	/**
	 * Copies and inserts [tree] before the node pointed at by [t].
	 * [t] may not be a past-the-end traverser, except if the tree is empty (this limitation is for
	 * performance reasons).
	 * Returns a traverser pointing to the root of the inserted subtree.
	 */
	template<bool Const>
	traverser insert_subtree(const Traverser<Const>& t, const Tree& tree);

	/**
	 * Moves in [tree] before the node pointed at by [t].
	 * [Tree] will be empty after the operation
	 * [t] may not be a past-the-end traverser, except if the tree is empty (this limitation is for
	 * performance reasons).
	 * Returns a traverser pointing to the root of the inserted subtree.
	 */
	template<bool Const>
	traverser insert_subtree(const Traverser<Const>& t, Tree&& tree);

	/**
	 * Appends a new node with the given value after the child nodes of the node pointed at by [t].
	 * Returns a traverser pointing to the appended node.
	 */
	template<bool Const>
	traverser append_child(const Traverser<Const>& t, const T& value);

	/**
	 * Appends a new node with the given value after the child nodes of the node pointed at by [t].
	 * Returns a traverser pointing to the appended node.
	 */
	template<bool Const>
	traverser append_child(const Traverser<Const>& t, T&& value);

	template<bool Const, typename... Args>
	traverser emplace_back_child(const Traverser<Const>& t, Args&&... args);

	/**
	 * Copies and appends the tree pointed at by [subtree] after the child nodes of the node pointed
	 * at by [t].
	 * Returns a traverser pointing to the root of the appended subtree.
	 */
	template<bool Const, typename Other_traverser>
	traverser append_child_subtree(const Traverser<Const>& t, Other_traverser subtree);

	/**
	 * Copies and appends [tree] after the child nodes of the node pointed at by [t].
	 * Returns a traverser pointing to the root of the appended subtree.
	 */
	template<bool Const>
	traverser append_child_subtree(const Traverser<Const>& t, const Tree& tree);

	/**
	 * Moves in [tree] after the child nodes of the node pointed at by [t].
	 * [tree] will be empty after the operation.
	 * Returns a traverser pointing to the root of the appended subtree.
	 */
	template<bool Const>
	traverser append_child_subtree(const Traverser<Const>& t, Tree&& tree);

	friend void swap<T>(Tree& l, Tree& r) noexcept;

	friend bool operator==<T>(const Tree& l, const Tree& r);
	friend bool operator!=<T>(const Tree& l, const Tree& r);

private:

	struct Node {
		// TODO: constructors taking T&& value and Args&&...

		Node(const T& value, Node* parent, std::vector<Node> children);
		Node(const T& value, Node* parent = nullptr);
		Node(const Node& other);
		Node(Node&& other) noexcept;

		Node& operator=(const Node& other);
		Node& operator=(Node&& other);

		void set_parent_pointers_of_children();

		bool operator==(const Node& r) const;
		bool operator!=(const Node& r) const;

		T value;
		Node* parent;
		std::vector<Node> children;
	};

	friend void swap<T>(Node& l, Node& r) noexcept;

	Tree(const Node& node);

	[[noreturn]]
	static void throw_insert_before_root_exception();

	[[noreturn]]
	static void throw_insert_before_past_the_end_traverser_exception();

	template<bool Const, typename... Args>
	traverser emplace_node(const Traverser<Const>& t, Args&&... args);

	template<bool Const, typename... Args>
	traverser emplace_back_child_node(const Traverser<Const>& t, Args&&... args);

	deep_ptr<Node> root_ptr;
};

//==================================================================================================
// Tree::Traverser class
//==================================================================================================

/**
 * Traverser class for Tree, templated over whether it only gives const access.
 * Important note: for performance reasons, the traverser cannot go back to its parent from a
 * past-the-end traverser.
 * Also acts as a sibling iterator. In this sense it is a random access iterator (hence the
 * enormous amount of boilerplate).
 */
template<typename T> template<bool Const>
class Tree<T>::Traverser {
	friend class Tree;
public:
	using difference_type = std::ptrdiff_t;
	using value_type = T;
	using reference = std::conditional_t<Const, T const&, T&>;
	using pointer = std::conditional_t<Const, T const*, T*>;
	using iterator_category = std::random_access_iterator_tag;

	friend void swap(Traverser& l, Traverser& r) noexcept {
		using std::swap;
		swap(l.node_ptr, r.node_ptr);
		swap(l.parent_ptr, r.parent_ptr);
	}

	reference operator*() const { return node_ptr->value; }
	pointer  operator->() const { return &(node_ptr->value); }

	friend bool operator==(const Traverser& lhs, const Traverser& rhs) {
		return lhs.node_ptr == rhs.node_ptr;
	}
	friend bool operator!=(const Traverser& lhs, const Traverser& rhs) {
		return lhs.node_ptr != rhs.node_ptr;
	}

	Traverser& to_parent() {
		node_ptr = node_ptr->parent;
		return *this;
	}
	Traverser parent() const {
		return Traverser(*this).to_parent();
	}

	Traverser& to_child(size_t child_index) {
		node_ptr = node_ptr->children.data() + child_index;
		return *this;
	}
	Traverser child(size_t child_index) const {
		return Traverser(*this).to_child(child_index);
	}

	reference operator[](difference_type n) const {
		return &((node_ptr + n)->value);
	}

	difference_type friend operator-(const Traverser& l,const Traverser& r){
		return l.node_ptr - r.node_ptr;
	}

	friend bool operator<(const Traverser& l, const Traverser& r) {
		return l.node_ptr < r.node_ptr;
	}
	friend bool operator>(const Traverser& l, const Traverser& r) {
		return l.node_ptr > r.node_ptr;
	}
	friend bool operator<=(const Traverser& l, const Traverser& r) {
		return l.node_ptr <= r.node_ptr;
	}
	friend bool operator>=(const Traverser& l, const Traverser& r) {
		return l.node_ptr >= r.node_ptr;
	}

	Traverser& operator+=(difference_type n) {
		node_ptr += n;
		return *this;
	}
	Traverser& operator-=(difference_type n) {
		node_ptr -= n;
		return *this;
	}
	Traverser& operator++() {
		++node_ptr;
		return *this;
	}
	Traverser& operator--() {
		--node_ptr;
		return *this;
	}
	Traverser operator++(int) {
		auto ret = *this;
		++node_ptr;
		return ret;
	}
	Traverser operator--(int) {
		auto ret = *this;
		--node_ptr;
		return ret;
	}
	friend Traverser operator+(Traverser t, difference_type n) {
		return t += n;
	}
	friend Traverser operator+(difference_type n, Traverser t) {
		return t += n;
	}
	friend Traverser operator-(Traverser t, difference_type n) {
		return t -= n;
	}

	Traverser& to_begin() {
		node_ptr = node_ptr->children.data();
		return *this;
	}
	Traverser begin() const {
		return Traverser(*this).to_begin();
	}

	Traverser& to_end() {
		node_ptr = node_ptr->children.data() + node_ptr->children.size();
		return *this;
	}
	Traverser end() const {
		return Traverser(*this).to_end();
	}

	operator Traverser<true>() {
		return Traverser<true>(node_ptr);
	}

	bool is_leaf() const {
		return node_ptr->children.empty();
	}

	size_t child_count() const {
		return node_ptr->children.size();
	}

	bool is_valid() const noexcept {
		return node_ptr != nullptr;
	}

	operator bool() const noexcept {
		return is_valid();
	}

private:
	Traverser(Node* node_ptr) : node_ptr(node_ptr) {}

	Node* node_ptr;
};

//==================================================================================================
// Implementation public methods
//==================================================================================================

template<typename T>
Tree<T>::Tree() {}

template<typename T>
Tree<T>::Tree(const T& root_value) {
	root_ptr = make_deep<Node>(root_value);
}

template<typename T>
Tree<T>::Tree(T&& root_value) {
	root_ptr = make_deep<Node>(root_value);
}

template<typename T>
template<typename... Args>
Tree<T>::Tree(Args&&... args) {
	// Not quite true emplacement
	root_ptr = make_deep<Node>(T{args...});
}

template<typename T>
Tree<T>::Tree(const Tree<T>::const_traverser& t) : Tree(*(t.node_ptr)) {}

template<typename T>
bool Tree<T>::empty() const noexcept {
	return !root_ptr;
}

template<typename T>
T& Tree<T>::root() {
	return root_ptr->value;
}

template<typename T>
const T& Tree<T>::root() const {
	return root_ptr->value;
}

template<typename T>
Tree<T> Tree<T>::subtree(size_t child_index) const {
	return Tree(root_ptr->children[child_index]);
}

template<typename T>
typename Tree<T>::traverser Tree<T>::entrance() noexcept {
	return traverser(root_ptr.get());
}
template<typename T>
typename Tree<T>::const_traverser Tree<T>::entrance() const noexcept {
	return const_traverser(root_ptr.get());
}
template<typename T>
typename Tree<T>::const_traverser Tree<T>::centrance() const noexcept {
	return const_traverser(root_ptr.get());
}

template<typename T>
template<bool Const>
void Tree<T>::reserve_children(const Traverser<Const>& t, size_t size) {
	t.node_ptr->children.reserve(size);
}

template<typename T>
template<bool Const>
typename Tree<T>::traverser Tree<T>::insert(const Traverser<Const>& t, const T& value) {
	return emplace_node(t, value);
}

template<typename T>
template<bool Const>
typename Tree<T>::traverser Tree<T>::insert(const Traverser<Const>& t, T&& value) {
	return emplace_node(t, value);
}

template<typename T>
template<bool Const, typename... Args>
typename Tree<T>::traverser Tree<T>::emplace(const Traverser<Const>& t, Args&&... args) {
	// Not quite true emplacement
	return emplace_node(t, T{std::forward<Args>(args)...});
}

template<typename T>
template<bool Const, typename Other_traverser>
typename Tree<T>::traverser Tree<T>::insert_subtree(const Traverser<Const>& t, Other_traverser t2) {
	return emplace_node(t, *(t2.node_ptr));
}

template<typename T>
template<bool Const>
typename Tree<T>::traverser Tree<T>::insert_subtree(const Traverser<Const>& t, const Tree& tree) {
	return insert_subtree(t, tree.entrance());
}


template<typename T>
template<bool Const>
typename Tree<T>::traverser Tree<T>::insert_subtree(const Traverser<Const>& t, Tree&& tree) {
	return emplace_node(t, std::move(*tree.root_ptr.release()));
}

template<typename T>
template<bool Const>
typename Tree<T>::traverser Tree<T>::append_child(const Traverser<Const>& t, const T& value) {
	return emplace_back_child_node(t, value);
}

template<typename T>
template<bool Const>
typename Tree<T>::traverser Tree<T>::append_child(const Traverser<Const>& t, T&& value) {
	return emplace_back_child_node(t, value);
}

template<typename T>
template<bool Const, typename... Args>
typename Tree<T>::traverser Tree<T>::emplace_back_child(const Traverser<Const>& t, Args&&... args) {
	// Not quite true emplacement
	return emplace_back_child_node(t, T{std::forward<Args>(args)...});
}

template<typename T>
template<bool Const, typename Other_traverser>
typename Tree<T>::traverser Tree<T>::append_child_subtree(
	const Traverser<Const>& t, Other_traverser t2
) {
	return emplace_back_child_node(t, *(t2.node_ptr));
}

template<typename T>
template<bool Const>
typename Tree<T>::traverser Tree<T>::append_child_subtree(
	const Traverser<Const>& t, const Tree& tree
) {
	return append_child_subtree(t, tree.entrance());
}

template<typename T>
template<bool Const>
typename Tree<T>::traverser Tree<T>::append_child_subtree(
	const Traverser<Const>& t, Tree&& tree
) {
	return emplace_back_child_node(t, std::move(*tree.root_ptr.release()));
}

template<typename T>
void swap(Tree<T>& l, Tree<T>& r) noexcept {
	using std::swap;
	swap(l.root_ptr, r.root_ptr);
}

template<typename T>
bool operator==(const Tree<T>& l, const Tree<T>& r) {
	if (l.root_ptr == nullptr) {
		return r.root_ptr == nullptr;
	}
	return r.root_ptr != nullptr && *(l.root_ptr) == *(r.root_ptr);
}

template<typename T>
bool operator!=(const Tree<T>& l, const Tree<T>& r) {
	if (l.root_ptr == nullptr) {
		return r.root_ptr != nullptr;
	}
	return r.root_ptr == nullptr || *(l.root_ptr) != *(r.root_ptr);
}

//==================================================================================================
// Implementation private methods
//==================================================================================================

//--------------------------------------------------------------------------------------------------
// Tree::Node methods
//--------------------------------------------------------------------------------------------------

template<typename T>
Tree<T>::Node::Node(const T& value, Node* parent, std::vector<Node> children)
		: value   (value)
		, parent  (parent)
		, children(children)
{}

template<typename T>
Tree<T>::Node::Node(const T& value, Node* parent)
		: value (value)
		, parent(parent)
{}

template<typename T>
Tree<T>::Node::Node(const Node& other)
		: value   (other.value)
		, parent  (other.parent)
		, children(other.children)
{
	set_parent_pointers_of_children();
}

template<typename T>
Tree<T>::Node::Node(Node&& other) noexcept
		: value   (std::move(other.value))
		, parent  (std::move(other.parent))
		, children(std::move(other.children))
{
	set_parent_pointers_of_children();
}

template<typename T>
typename Tree<T>::Node& Tree<T>::Node::operator=(const Node& other) {
	value    = other.value;
	parent   = other.parent;
	children = other.children;
	set_parent_pointers_of_children();
	return *this;
}

template<typename T>
typename Tree<T>::Node& Tree<T>::Node::operator=(Node&& other) {
	value    = std::move(other.value);
	parent   = std::move(other.parent);
	children = std::move(other.children);
	set_parent_pointers_of_children();
	return *this;
}

template<typename T>
void Tree<T>::Node::set_parent_pointers_of_children() {
	for (auto& child : children) {
		child.parent = this;
	}
}

template<typename T>
bool Tree<T>::Node::operator==(const Node& r) const {
	return value == r.value && children == r.children;
}

template<typename T>
bool Tree<T>::Node::operator!=(const Node& r) const {
	return value != r.value || children != r.children;
}

//--------------------------------------------------------------------------------------------------
// Tree methods
//--------------------------------------------------------------------------------------------------

template<typename T>
void swap(typename Tree<T>::Node& l, typename Tree<T>::Node& r) noexcept {
	using std::swap;

	swap(l.value,    r.value);
	swap(l.parent,   r.parent);
	swap(l.children, r.children);

	l.set_parent_pointers_of_children();
	r.set_parent_pointers_of_children();
}

template<typename T>
Tree<T>::Tree(const Node& node) {
	root_ptr = make_deep<Node>(node);
}

// These throws are wrapped in functions to keep the error messages in a single place.

template<typename T>
[[noreturn]] void Tree<T>::throw_insert_before_root_exception() {
	throw std::logic_error(
		"Attempted to insert an element before the root of an avds::tree::Tree."
	);
}

template<typename T>
[[noreturn]] void Tree<T>::throw_insert_before_past_the_end_traverser_exception() {
	throw std::logic_error(
		"Attempted to insert an element before a past-the-end traversr of an avds::tree::Tree. "
		"This is not supported for performance reasons."
	);
}

template<typename T>
template<bool Const, typename... Args>
typename Tree<T>::traverser Tree<T>::emplace_node(const Traverser<Const>& t, Args&&... args) {
	Node* new_node_ptr;

	if (!t.is_valid()) {
		if (root_ptr != nullptr) {
			throw_insert_before_past_the_end_traverser_exception();
		}
		root_ptr = make_deep<Node>(std::forward<Args>(args)...);
		root_ptr->parent = nullptr;
		new_node_ptr = root_ptr.get();
	}
	else {
		// This is why inserting before a past-the-end traverser is not supported. The dereferencing
		// of t.node_ptr would cause a segfault.
		// The problem here is that we need to know the parent of a traverser even if it's
		// past-the-end. If we make traversers store and maintain a parent_ptr in addition to their
		// node_ptr, this will have significant performance costs while traversing.
		Node* parent_ptr = t.node_ptr->parent;

		if (parent_ptr == nullptr) {
			throw_insert_before_root_exception();
		}
		auto& vec = parent_ptr->children;
		auto new_it = vec.emplace(
			vec.begin()+(t.node_ptr-vec.data()), std::forward<Args>(args)...
		);
		new_node_ptr = vec.data() + (new_it - vec.begin());
		new_node_ptr->parent = parent_ptr;
	}

	return traverser(new_node_ptr);
}

template<typename T>
template<bool Const, typename... Args>
typename Tree<T>::traverser Tree<T>::emplace_back_child_node(
	const Traverser<Const>& t, Args&&... args
) {
	t.node_ptr->children.emplace_back(std::forward<Args>(args)...);
	auto new_node_ptr = &(t.node_ptr->children.back());
	new_node_ptr->parent = t.node_ptr;
	return traverser(new_node_ptr);
}

} //namespace avds::tree
