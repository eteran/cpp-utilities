#ifndef FLAT_SET_H_
#define FLAT_SET_H_

#include <algorithm>
#include <cstddef>
#include <functional>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>

template <class Key, class Compare = std::less<Key>, class Allocator = std::allocator<Key>>
class FlatSet {
public:
	using key_type               = Key;
	using value_type             = Key;
	using storage_type           = std::vector<value_type, Allocator>;
	using size_type              = typename storage_type::size_type;
	using difference_type        = typename storage_type::difference_type;
	using key_compare            = Compare;
	using allocator_type         = Allocator;
	using reference              = typename storage_type::reference;
	using const_reference        = typename storage_type::const_reference;
	using pointer                = typename storage_type::pointer;
	using const_pointer          = typename storage_type::const_pointer;
	using iterator               = typename storage_type::iterator;
	using const_iterator         = typename storage_type::const_iterator;
	using reverse_iterator       = typename storage_type::reverse_iterator;
	using const_reverse_iterator = typename storage_type::const_reverse_iterator;

public:
	class value_compare {
		friend class FlatSet;

	protected:
		value_compare(Compare c) : comp(c) {
		}

	public:
		bool operator()(const value_type &lhs, const value_type &rhs) const {
			return comp(lhs, rhs);
		}

	protected:
		Compare comp;
	};

public:
	FlatSet() : FlatSet(Compare()) {
	}

	explicit FlatSet(const Compare &comp, const Allocator &alloc = Allocator()) : comp_(comp), storage_(alloc) {
	}

	template <class In>
	FlatSet(In first, In last, const Compare &comp = Compare(), const Allocator &alloc = Allocator()) : comp_(comp), storage_(alloc) {
		insert(first, last);
	}

	template <class In>
	FlatSet(In first, In last, const Allocator &alloc) : storage_(alloc) {
		insert(first, last);
	}

	FlatSet(const FlatSet &other) : comp_(other.comp_), storage_(other.storage_) {
	}

	FlatSet(const FlatSet &other, const Allocator &alloc) : comp_(other.comp_), storage_(other.storage_, alloc) {
	}

	FlatSet(const FlatSet &&other) : comp_(std::move(other.comp_)), storage_(std::move(other.storage_)) {
	}

	FlatSet(const FlatSet &&other, const Allocator &alloc) : comp_(std::move(other.comp_)), storage_(std::move(other.storage_), alloc) {
	}

	FlatSet(std::initializer_list<value_type> init, const Compare &comp = Compare(), const Allocator &alloc = Allocator()) : comp_(comp), storage_(alloc) {
		insert(init);
	}

	FlatSet(std::initializer_list<value_type> init, const Allocator &alloc) : comp_(Compare()), storage_(alloc) {
		insert(init);
	}

	~FlatSet() {
	}

public:
	FlatSet &operator=(const FlatSet &other) {
		FlatSet(other).swap(*this);
		return *this;
	}

	FlatSet &operator=(FlatSet &&other) {
		FlatSet(std::move(other)).swap(*this);
		return *this;
	}

	FlatSet &operator=(std::initializer_list<value_type> ilist) {
		FlatSet(ilist).swap(*this);
		return *this;
	}

public:
	allocator_type get_allocator() const {
		return storage_.get_allocator();
	}

public:
	iterator begin() {
		using std::begin;
		return begin(storage_);
	}

	const_iterator begin() const {
		using std::begin;
		return begin(storage_);
	}

	const_iterator cbegin() const {
		using std::cbegin;
		return cbegin(storage_);
	}

	iterator end() {
		using std::end;
		return end(storage_);
	}

	const_iterator end() const {
		using std::end;
		return end(storage_);
	}

	const_iterator cend() const {
		using std::cend;
		return cend(storage_);
	}

	reverse_iterator rbegin() {
		using std::rbegin;
		return rbegin(storage_);
	}

	const_reverse_iterator rbegin() const {
		using std::rbegin;
		return rbegin(storage_);
	}

	const_reverse_iterator crbegin() const {
		using std::crbegin;
		return crbegin(storage_);
	}

	reverse_iterator rend() {
		using std::rend;
		return rend(storage_);
	}

	const_reverse_iterator rend() const {
		using std::rend;
		return rend(storage_);
	}

	const_reverse_iterator crend() const {
		using std::crend;
		return crend(storage_);
	}

public:
	bool empty() const {
		return storage_.empty();
	}

	size_type size() const {
		return storage_.size();
	}

	size_type max_size() const {
		return storage_.max_size();
	}

public:
	void clear() {
		storage_.clear();
	}

public:
	std::pair<iterator, bool> insert(const value_type &value) {
		using std::begin;
		using std::end;
		auto it = std::lower_bound(begin(storage_), end(storage_), value, comp_);
		if (it != end(storage_) && !comp_(value, *it)) {
			// found!
			return std::make_pair(it, false);
		} else {
			// create it
			auto n = storage_.insert(it, value);
			return std::make_pair(n, true);
		}
	}

	template <class P>
	std::pair<iterator, bool> insert(P &&value) {
		using std::begin;
		using std::end;
		auto it = std::lower_bound(begin(storage_), end(storage_), value, comp_);
		if (it != end(storage_) && !comp_(value, *it)) {
			// found!
			return std::make_pair(it, false);
		} else {
			// create it
			auto n = storage_.insert(it, std::forward<P>(value));
			return std::make_pair(n, true);
		}
	}

	std::pair<iterator, bool> insert(value_type &&value) {
		using std::begin;
		using std::end;
		auto it = std::lower_bound(begin(storage_), end(storage_), value, comp_);
		if (it != end(storage_) && !comp_(value, *it)) {
			// found!
			return std::make_pair(it, false);
		} else {
			// create it
			auto n = storage_.insert(it, std::move(value));
			return std::make_pair(n, true);
		}
	}

	std::pair<iterator, bool> insert(const_iterator hint, const value_type &value) {
		(void)hint;
		return insert(value);
	}

	template <class P>
	std::pair<iterator, bool> insert(const_iterator hint, P &&value) {
		(void)hint;
		return insert(std::forward<P>(value));
	}

	std::pair<iterator, bool> insert(const_iterator hint, value_type &&value) {
		(void)hint;
		return insert(std::move(value));
	}

	template <class In>
	void insert(In first, In last) {
		while (first != last) {
			insert(*first++);
		}
	}

	void insert(std::initializer_list<value_type> ilist) {
		for (auto &&value : ilist) {
			insert(std::move(value));
		}
	}

public:
	template <class M>
	std::pair<iterator, bool> insert_or_assign(const key_type &key, M &&obj) {
		auto it = insert(key);
		if (it.second) {
			it.first->second = std::forward<M>(obj);
		}
		return it;
	}

	template <class M>
	std::pair<iterator, bool> insert_or_assign(key_type &&key, M &&obj) {
		auto it = insert(std::move(key));
		if (it.second) {
			it.first->second = std::forward<M>(obj);
		}
		return it;
	}

	template <class M>
	std::pair<iterator, bool> insert_or_assign(const_iterator hint, const key_type &key, M &&obj) {
		auto it = insert(hint, key);
		if (it.second) {
			it.first->second = std::forward<M>(obj);
		}
		return it;
	}

	template <class M>
	std::pair<iterator, bool> insert_or_assign(const_iterator hint, key_type &&key, M &&obj) {
		auto it = insert(hint, key);
		if (it.second) {
			it.first->second = std::forward<M>(obj);
		}
		return it;
	}

public:
	// TODO:  try_emplace
	template <class... Args>
	std::pair<iterator, bool> emplace(Args &&... args) {
		using std::begin;
		using std::end;

		value_type temp(std::forward<Args>(args)...);

		auto it = std::lower_bound(begin(storage_), end(storage_), temp, comp_);
		if (it != end(storage_) && !comp_(temp, *it)) {
			// found!
			return std::make_pair(it, false);
		} else {
			// create it
			auto n = storage_.emplace(it, std::move(temp));
			return std::make_pair(n, true);
		}
	}

	template <class... Args>
	std::pair<iterator, bool> emplace_hint(const_iterator hint, Args &&... args) {

		(void)hint;

		using std::begin;
		using std::end;

		value_type temp(std::forward<Args>(args)...);

		auto it = std::lower_bound(begin(storage_), end(storage_), temp, comp_);
		if (it != end(storage_) && !comp_(temp, *it)) {
			// found!
			return std::make_pair(it, false);
		} else {
			// create it
			auto n = storage_.emplace(it, std::move(temp));
			return std::make_pair(n, true);
		}
	}

public:
	iterator erase(const_iterator pos) {
		return storage_.erase(pos);
	}

	iterator erase(iterator pos) {
		return storage_.erase(pos);
	}

	iterator erase(const_iterator first, const_iterator last) {
		return storage_.erase(first, last);
	}

	size_type erase(const key_type &key) {
		size_type c = 0;
		auto r = equal_range(key);
		auto first = r.first;
		auto second = r.second;
		while (first != second) {
			erase(first++);
			++c;
		}
		return c;
	}

public:
	void swap(FlatSet &other) {
		using std::swap;
		swap(comp_, other.comp_);
		swap(storage_, other.storage_);
	}

public:
	size_type count(const Key &key) const {
		size_type c = 0;
		using std::begin;
		using std::end;

		auto it = std::lower_bound(begin(storage_), end(storage_), key, comp_);
		while (it != end(storage_) && !comp_(key, *it)) {
			++c;
		}

		return c;
	}

	template <class K>
	size_type count(const K &key) const {
		size_type c = 0;
		using std::begin;
		using std::end;
		auto it = std::lower_bound(begin(storage_), end(storage_), key, comp_);
		while (it != end(storage_) && !comp_(key, *it)) {
			++c;
		}

		return c;
	}

public:
	iterator find(const Key &key) {
		using std::begin;
		using std::end;
		auto it = std::lower_bound(begin(storage_), end(storage_), key, comp_);
		if (it != end(storage_) && !comp_(key, *it)) {
			return it;
		}

		return end(storage_);
	}

	const_iterator find(const Key &key) const {
		using std::begin;
		using std::end;
		auto it = std::lower_bound(begin(storage_), end(storage_), key, comp_);
		if (it != end(storage_) && !comp_(key, *it)) {
			return it;
		}

		return end(storage_);
	}

	template <class K>
	iterator find(const K &key) {
		using std::begin;
		using std::end;
		auto it = std::lower_bound(begin(storage_), end(storage_), key, comp_);
		if (it != end(storage_) && !comp_(key, *it)) {
			return it;
		}

		return end(storage_);
	}

	template <class K>
	const_iterator find(const K &key) const {
		using std::begin;
		using std::end;
		auto it = std::lower_bound(begin(storage_), end(storage_), key, comp_);
		if (it != end(storage_) && !comp_(key, *it)) {
			return it;
		}

		return end(storage_);
	}

public:
	std::pair<iterator, iterator> equal_range(const Key &key) {
		using std::begin;
		using std::end;
		return std::equal_range(begin(storage_), end(storage_), key, comp_);
	}

	std::pair<const_iterator, const_iterator> equal_range(const Key &key) const {
		using std::begin;
		using std::end;
		return std::equal_range(begin(storage_), end(storage_), key, comp_);
	}

	template <class K>
	std::pair<iterator, iterator> equal_range(const K &key) {
		using std::begin;
		using std::end;
		return std::equal_range(begin(storage_), end(storage_), key, comp_);
	}

	template <class K>
	std::pair<const_iterator, const_iterator> equal_range(const K &key) const {
		using std::begin;
		using std::end;
		return std::equal_range(begin(storage_), end(storage_), key, comp_);
	}

public:
	iterator lower_bound(const Key &key) {
		using std::begin;
		using std::end;
		return std::lower_bound(begin(storage_), end(storage_), key, comp_);
	}

	const_iterator lower_bound(const Key &key) const {
		using std::begin;
		using std::end;
		return std::lower_bound(begin(storage_), end(storage_), key, comp_);
	}

	template <class K>
	iterator lower_bound(const K &key) {
		using std::begin;
		using std::end;
		return std::lower_bound(begin(storage_), end(storage_), key, comp_);
	}

	template <class K>
	const_iterator lower_bound(const K &key) const {
		using std::begin;
		using std::end;
		return std::lower_bound(begin(storage_), end(storage_), key, comp_);
	}

public:
	iterator upper_bound(const Key &key) {
		using std::begin;
		using std::end;
		return std::upper_bound(begin(storage_), end(storage_), key, comp_);
	}

	const_iterator upper_bound(const Key &key) const {
		using std::begin;
		using std::end;
		return std::upper_bound(begin(storage_), end(storage_), key, comp_);
	}

	template <class K>
	iterator upper_bound(const K &key) {
		using std::begin;
		using std::end;
		return std::upper_bound(begin(storage_), end(storage_), key, comp_);
	}

	template <class K>
	const_iterator upper_bound(const K &key) const {
		using std::begin;
		using std::end;
		return std::upper_bound(begin(storage_), end(storage_), key, comp_);
	}

public:
	key_compare key_comp() const {
		return comp_.comp;
	}

	value_compare value_comp() const {
		return comp_;
	}

private:
	value_compare comp_;
	storage_type  storage_;
};

#endif
