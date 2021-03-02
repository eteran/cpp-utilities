/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2015 Evan Teran
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
 
#ifndef UTILITY_ARENA_HPP_
#define UTILITY_ARENA_HPP_

#include <bitset>
#include <cassert>
#include <cstring>
#include <cstddef>
#include <cstdint>
#include <climits>
#include <cpp-utilities/bitset.h>

#define ARENA_ALLOCATOR_PURIFY

namespace memory {
namespace detail {

struct bitset_strategy_tag {};
struct linked_strategy_tag {};

// simplify storage management
template <class T, size_t Count>
class malloc_storage {
public:
	malloc_storage() : p_(reinterpret_cast<T*>(malloc(sizeof(T) * Count))) {
	}
	
	~malloc_storage() {
		free(p_);
	}
	
	malloc_storage(const malloc_storage &)            = delete;
	malloc_storage &operator=(const malloc_storage &) = delete;
	
	malloc_storage(malloc_storage &&rhs) noexcept : p_(rhs.p_) {
		rhs.p_ = nullptr;
	}
	
	malloc_storage &operator=(malloc_storage &&rhs) noexcept {
		p_ = rhs.p_;
		rhs.p_ = nullptr;
		return *this;
	}
	
	T &operator[](size_t index) {
		return p_[index];
	}
	
private:
	T *p_;
};

template <class T, size_t Count, class Strategy>
class arena_allocator;

template <class T, size_t Count>
class arena_allocator<T, Count, bitset_strategy_tag> {
public:
	arena_allocator() {
		freelist_.set();		
	}
	
	arena_allocator(arena_allocator &&other) noexcept          = default;	
	arena_allocator &operator=(arena_allocator &&rhs) noexcept = default;
	arena_allocator(const arena_allocator &)                   = delete;
	arena_allocator &operator=(const arena_allocator &)        = delete;
	~arena_allocator()                                         = default;
	
public:	
	void release(void *ptr) noexcept {
		if(ptr) {
			assert(ptr >= &storage_[0]     && "Attempting to release invalid pointer");
			assert(ptr  < &storage_[Count] && "Attempting to release invalid pointer");	

			const int index = (reinterpret_cast<T*>(ptr) - &storage_[0]);
			
			assert(!freelist_[index] && "Double free detected");			
			freelist_.flip(index);
		}
	}
	
	void *allocate() noexcept {
		const int index = bitset::find_first(freelist_);
		if(index == Count) {
			return nullptr;
		}
		
		freelist_[index].flip();
		
		T *const p = &storage_[index];
		
#ifdef ARENA_ALLOCATOR_PURIFY
		// this is the small object allocator, so it's pretty efficient to
		// always clear out the storage :-)
		memset(p, 0, sizeof(T));
#endif
		return p;
	}
	
private:	
	malloc_storage<T, Count> storage_;
	std::bitset<Count>       freelist_;
};

template <class T, size_t Count>
class arena_allocator<T, Count, linked_strategy_tag> {
	static_assert(sizeof(T) >= sizeof(void *), "Linked strategy can only be used for objects larger than or equal to the size of a pointer");

private:
	struct node {
		node *next;
	};

public:
	arena_allocator() : freelist_(nullptr) {
		for(size_t i = 0; i < Count; ++i) {
			release(&storage_[i]);
		}
	}
	
	~arena_allocator() = default;

public:	
	arena_allocator(arena_allocator &&other) : storage_(std::move(other.storage_)), freelist_(other.freelist_) {
		other.freelist_ = nullptr;
	}
	
	arena_allocator &operator=(arena_allocator &&rhs) noexcept {
		if(this != &rhs) {
			storage_      = std::move(rhs.storage_);
			freelist_     = rhs.freelist_;
			rhs.freelist_ = nullptr;		
		}
		return *this;
	}
	
private:
	arena_allocator(const arena_allocator &)           = delete;
	arena_allocator &operator=(const arena_allocator &) = delete;	
	
public:	
	void release(void *ptr) noexcept {
		if(ptr) {
			assert(ptr >= &storage_[0]     && "Attempting to release invalid pointer");
			assert(ptr  < &storage_[Count] && "Attempting to release invalid pointer");
			assert((reinterpret_cast<uintptr_t>(ptr) & (sizeof(void *) - 1)) == 0 && "Attempting to release misaligned pointer");	

			node *const p = reinterpret_cast<node *>(ptr);

			// TODO: in debug mode, detect a double free... not sure how this can be
			//       done efficiently with a linked list.

			p->next = freelist_;
			freelist_ = p;
		}
	}
	
	void *allocate() noexcept {
		if(!freelist_) {
			return nullptr;
		}
		
		node *const p = freelist_;
		freelist_ = freelist_->next;		
#ifdef ARENA_ALLOCATOR_PURIFY
		// avoid information disclosure bug
		p->next = nullptr;
#endif
		return reinterpret_cast<T *>(p);
	}

private:	
	malloc_storage<T, Count> storage_;
	node*                    freelist_;
};
}

template <class T, size_t Count>
detail::arena_allocator<T, Count, detail::linked_strategy_tag> make_arena(typename std::enable_if<(sizeof(T) >= sizeof(void *)) && (Count > sizeof(size_t) * CHAR_BIT)>::type* = nullptr) {
	return detail::arena_allocator<T, Count, detail::linked_strategy_tag>();
}

template <class T, size_t Count>
detail::arena_allocator<T, Count, detail::bitset_strategy_tag> make_arena(typename std::enable_if<(sizeof(T) < sizeof(void *)) || (Count <= sizeof(size_t) * CHAR_BIT)>::type* = nullptr) {
	return detail::arena_allocator<T, Count, detail::bitset_strategy_tag>();
}

}

#endif
