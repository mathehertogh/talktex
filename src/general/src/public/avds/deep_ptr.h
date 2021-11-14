/*
	deep_ptr - An std::unique_ptr with a copy constructor

	Version 0.1.0 (IN DEVELOPMENT)

	deep_ptr is identical to std::unique_ptr in almost every way, but it
	additionally provides a copy constructor and a copying assignment operator.
	These methods perform a deep copy using the copy constructor of the
	underlying type T. T should therefore be copy-constructible.

	Currently, deep_ptr does not yet support the entire interface of
	std::unique_ptr.

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

#include <memory>

namespace avds {

/**
 * Equivalent to std::unique_ptr in almost every way, but additionally provides a copy constructor
 * and a copying assignment operator. These methods perform a deep copy using the copy constructor
 * of the underlying type T. T should therefore be copy-constructible.
 */
template<class T, class Deleter = std::default_delete<T> >
class deep_ptr {
public:
	using pointer      = typename std::unique_ptr<T, Deleter>::pointer;
	using element_type = typename std::unique_ptr<T, Deleter>::element_type;
	using deleter_type = typename std::unique_ptr<T, Deleter>::deleter_type;

	//==============================================================================================
	// Deep copy properties
	//==============================================================================================

	deep_ptr(const deep_ptr& other)
			: ptr(other ? new element_type{*(other.ptr)} : nullptr)
	{}

	deep_ptr& operator=(deep_ptr other) {
		swap(*this, other);
		return *this;
	}

	friend void swap(deep_ptr& l, deep_ptr& r) noexcept {
		using std::swap;
		swap(l.ptr, r.ptr);
	}

	//==============================================================================================
	// Conversions from std::unqiue_ptr
	//==============================================================================================

	explicit deep_ptr(const std::unique_ptr<T, Deleter>& uptr)
			: ptr(new element_type{*uptr})
	{}

	explicit deep_ptr(std::unique_ptr<T, Deleter>&& uptr) noexcept
			: ptr(std::move(uptr))
	{}

	deep_ptr& operator=(const std::unique_ptr<T, Deleter>& uptr) {
		return *this = deep_ptr(uptr);
	}

	deep_ptr& operator=(std::unique_ptr<T, Deleter>&& uptr) noexcept {
		return *this = deep_ptr(std::move(uptr));
	}

	//==============================================================================================
	// std::unique_ptr delegations
	//==============================================================================================

	constexpr deep_ptr() noexcept {}
	constexpr deep_ptr(std::nullptr_t) noexcept : ptr(nullptr) {}
	explicit deep_ptr(pointer p) noexcept : ptr(p) {}
	// Deleter constructors not implemented.

	deep_ptr(deep_ptr&& d) noexcept : ptr(std::move(d.ptr)) {}

	template<class U, class E>
	deep_ptr(deep_ptr<U, E>&& d) noexcept : ptr(std::move(d)) {}

	// Move assignment constructors not implemented.

	deep_ptr& operator=(std::nullptr_t) noexcept {
		ptr = nullptr;
		return *this;
	}

	pointer release() noexcept {
		return ptr.release();
	}

	void reset(pointer p = pointer()) noexcept {
		ptr.reset(p);
	}

	// Member swap not implemented.

	pointer get() const noexcept {
		return ptr.get();
	}

	Deleter& get_deleter() noexcept {
		return ptr.get_deleter();
	}

	const Deleter& get_deleter() const noexcept {
		return ptr.get_deleter();
	}

	explicit operator bool() const noexcept {
		return bool(ptr);
	}

	typename std::add_lvalue_reference<T>::type operator*() const {
		return *ptr;
	}

	pointer operator->() const noexcept {
		return ptr.get();
	}

private:
	std::unique_ptr<T, Deleter> ptr;
};

//==================================================================================================
// Helper functions
//==================================================================================================

template<class T, class... Args>
deep_ptr<T> make_deep(Args&&... args) {
	return deep_ptr<T>(new T(std::forward<Args>(args)...));
}

//==================================================================================================
// Comparison operators
//==================================================================================================

template<class T1, class D1, class T2, class D2>
inline bool operator==(const deep_ptr<T1, D1>& x, const deep_ptr<T2, D2> y) {
	return x.get() == y.get();
}

template<class T1, class D1, class T2, class D2>
inline bool operator!=(const deep_ptr<T1, D1>& x, const deep_ptr<T2, D2> y) {
	return x.get() != y.get();
}

template<class T, class D>
bool operator==(const deep_ptr<T, D>& x, std::nullptr_t) {
	return !x;
}

template<class T, class D>
bool operator==(std::nullptr_t, const deep_ptr<T, D>& x) {
	return !x;
}

template<class T, class D>
bool operator!=(const deep_ptr<T, D>& x, std::nullptr_t) {
	return (bool)x;
}

template<class T, class D>
bool operator!=(std::nullptr_t, const deep_ptr<T, D>& x) {
	return (bool)x;
}

// Other comparison operators not implemented.

} // namespace avds
