
#ifndef LRU_CACHE_H_
#define LRU_CACHE_H_

#include <algorithm>
#include <list>
#include <mutex>
#include <optional>
#include <shared_mutex>
#include <unordered_map>
#include <vector>

template <class Key, class T>
class lru_cache {
public:
	using value_type    = std::pair<Key, T>;
	using list_type     = std::list<value_type>;
	using list_iterator = typename list_type::iterator;
	using map_type      = std::unordered_map<Key, list_iterator>;
	using map_iterator  = typename map_type::iterator;

public:
	lru_cache(size_t capacity)
		: capacity_(capacity) {}

	~lru_cache() {
		clear();
	}

	size_t size() const {
		return size_;
	}

	size_t capacity() const {
		return capacity_;
	}

	void clear() {
		std::unique_lock lock(mutex_);
		list_.clear();
		index_.clear();
	};

	bool exists(const Key &key) const {
		std::shared_lock lock(mutex_);
		return index_.find(key) != index_.end();
	}

	void remove(const Key &key) {
		std::unique_lock lock(mutex_);
		auto it = index_.find(key);
		if (it != index_.end()) {
			remove_entry(it);
		}
	}

	void touch(const Key &key) {
		std::unique_lock lock(mutex_);
		touch_entry(key);
	}

	std::optional<T> fetch(const Key &key, bool touch_data = true) {
		std::unique_lock lock(mutex_);
		auto it = index_.find(key);
		if (it == index_.end()) {
			return {};
		}

		T tmp = it->second->second;
		if (touch_data) {
			touch_entry(it);
		}

		return tmp;
	}

	std::optional<T> take(const Key &key) {
		std::unique_lock lock(mutex_);
		auto it = index_.find(key);
		if (it == index_.end()) {
			return {};
		}

		T tmp = std::move(it->second->second);
		remove_entry(it);
		return tmp;
	}

	template <class U>
	void insert(const Key &key, U &&data) {
		std::unique_lock lock(mutex_);
		auto it = touch_entry(key);
		if (it != index_.end()) {
			remove_entry(it);
		}

		list_.emplace_front(key, std::forward<U>(data));
		index_.emplace(key, list_.begin());
		++size_;

		flush_overflow();
	}

	std::vector<Key> keys() const {

		std::vector<Key> ret;
		ret.reserve(list_.size());
		{
			std::shared_lock lock(mutex_);
			std::transform(list_.begin(), list_.end(), std::back_inserter(ret), [](const value_type &entry) {
				return entry.first;
			});
		}

		return ret;
	}

private:
	void flush_overflow() {
		while (size_ > capacity_) {
			remove_entry(list_.back().first);
		}
	}

	map_iterator touch_entry(const Key &key) {
		auto it = index_.find(key);
		if (it == index_.end()) {
			return it;
		}

		return touch_entry(it);
	}

	template <class Iter>
	Iter touch_entry(Iter it) {
		list_.splice(list_.begin(), list_, it->second);
		return it;
	}

	template <class Iter>
	void remove_entry(Iter it) {
		--size_;
		list_.erase(it->second);
		index_.erase(it);
	}

	void remove_entry(const Key &key) {
		auto it = index_.find(key);
		if (it != index_.end()) {
			remove_entry(it);
		}
	}

private:
	list_type list_;
	map_type index_;
	size_t capacity_ = 0;
	size_t size_     = 0;

private:
	mutable std::shared_mutex mutex_;
};

#endif
