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


#include "bitset.hpp"
#include <bitset>
#include <cassert>
#include <cstring>
#include <cstddef>
#include <cstdint>
#include <climits>

namespace memory {
namespace detail {

template <int N, class T>
constexpr T align_to(T value) {
	static_assert((N & (N - 1)) == 0, "invalid target rounding value");
	return ((value + N - 1) & ~(N - 1));
}

struct bitset_strategy {};
struct linked_strategy {};

template <size_t Size, size_t Count, class Strategy>
class arena_allocator;

template <size_t Size, size_t Count>
class arena_allocator<Size, Count, bitset_strategy> {
	// for very small objects, no alignment, you're on your own
	using T = struct { char bytes[Size]; };
public:
	arena_allocator() : storage_(reinterpret_cast<T*>(malloc(sizeof(T) * Count))) {
		freelist_.set();		
	}
	
	~arena_allocator() {
		free(storage_);
	}

public:	
	arena_allocator(arena_allocator &&other) : storage_(other.storage_), freelist_(other.freelist_) {
		other.storage_  = nullptr;
	}
	
	arena_allocator &operator=(arena_allocator &&rhs) noexcept {
		if(this != &rhs) {
			storage_      = rhs.storage_;
			freelist_     = rhs.freelist_;
			rhs.storage_  = nullptr;
			rhs.freelist_.clear();		
		}
		return *this;
	}	
	
private:
	arena_allocator(const arena_allocator &) = delete;
	arena_allocator &operator=(const arena_allocator &) = delete;
	
public:	
	void release(void *ptr) noexcept {
		if(ptr) {
			assert(ptr >= storage_ && "Attempting to release invalid pointer");
			assert(ptr < storage_ + Count && "Attempting to release invalid pointer");	

			const int index = (reinterpret_cast<T*>(ptr) - storage_);
			
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
		
		// this is the small object allocator, so it's pretty efficient to
		// always clear out the storage :-)
		memset(p, 0, sizeof(T));
		
		return p;
	}

private:	
	T*                 storage_;
	std::bitset<Count> freelist_;
};

template <size_t Size, size_t Count>
class arena_allocator<Size, Count, linked_strategy> {
	static_assert(Size >= sizeof(void *), "Linked strategy can only be used for objects larger than or equal to the size of a pointer");

private:
	using T = struct { char bytes[align_to<sizeof(void *)>(Size)]; };

private:
	struct node {
		node *next;
	};

public:
	arena_allocator() : storage_(reinterpret_cast<T*>(malloc(sizeof(T) * Count))), freelist_(nullptr) {
		for(size_t i = 0; i < Count; ++i) {
			release(&storage_[i]);
		}
	}
	
	~arena_allocator() {
		free(storage_);
	}	

public:	
	arena_allocator(arena_allocator &&other) : storage_(other.storage_), freelist_(other.freelist_) {
		other.storage_  = nullptr;
		other.freelist_ = nullptr;
	}
	
	arena_allocator &operator=(arena_allocator &&rhs) noexcept {
		if(this != &rhs) {
			storage_      = rhs.storage_;
			freelist_     = rhs.freelist_;
			rhs.storage_  = nullptr;
			rhs.freelist_ = nullptr;		
		}
		return *this;
	}
	
private:
	arena_allocator(const arena_allocator &) = delete;
	arena_allocator &operator=(const arena_allocator &) = delete;	
	
public:	
	void release(void *ptr) noexcept {
		if(ptr) {
			assert(ptr >= storage_ && "Attempting to release invalid pointer");
			assert(ptr < storage_ + Count && "Attempting to release invalid pointer");
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
		
		// avoid information disclosure bug
		p->next = nullptr;
		
		return reinterpret_cast<T *>(p);
	}

private:	
	T*    storage_;
	node* freelist_;
};
}

template <size_t Size, size_t Count>
detail::arena_allocator<Size, Count, detail::linked_strategy> make_arena(typename std::enable_if<Size >= sizeof(void*)>::type* = 0) {
	return detail::arena_allocator<Size, Count, detail::linked_strategy>();
}

template <size_t Size, size_t Count>
detail::arena_allocator<Size, Count, detail::bitset_strategy> make_arena(typename std::enable_if<Size < sizeof(void*)>::type* = 0) {
	return detail::arena_allocator<Size, Count, detail::bitset_strategy>();
}

}

#endif
