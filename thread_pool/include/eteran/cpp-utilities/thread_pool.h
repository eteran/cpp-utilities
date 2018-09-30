
#ifndef THREAD_POOL_H_
#define THREAD_POOL_H_

#include <cassert>
#include <condition_variable>
#include <cstddef>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class thread_pool {
public:
	using work_type = std::function<void()>;
	
public:
	/**
	 * Creates the thread pool with N threads where N is the value of
	 * std::thread::hardware_concurrency()
	 *
	 * @param count The number of threads in the pool
	 */
	thread_pool() : thread_pool(std::thread::hardware_concurrency()) {
	}

	/**
	 * Creates the thread pool with <count> threads
	 *
	 * @param count The number of threads in the pool
	 */
	thread_pool(std::size_t count) {
		// create all the threads
		for(std::size_t i = 0; i < count; ++i) {
			threads_.emplace_back([this]() {
				// keep looking for more tasks until we suicide
				while(true) {

					// get a new worker, this'll block while the queue is empty
					work_type worker = get_worker();

					// special case?
					if(!worker) {
						this->add_worker(nullptr);
						break;
					} else {
						// do the work				
						worker();
					}
				}
			});
		}	
	}
	
	/**
	 * Destroys the thread pool, waits still all outstanding work is complete
	 */
	~thread_pool() {
		// add special token which tells all the threads to suicide
		add_worker(nullptr);

		// wait till all outstanding tasks are done
		for(std::thread &thread : threads_) {
			thread.join();
		}	
	}

public:
	/**
	 * Adds a new work item to the pool for execution
	 *
	 * @param worker the work to do
	 */
	void add_worker(work_type worker) {
    	std::lock_guard<std::mutex> lock(queue_lock_);
    	work_queue_.push(std::move(worker));
    	queue_condition_.notify_one();
	}
	
	/**
	 * Waits until there is at least one work item to do, and then returns
	 * the work item after popping it off the queue
	 *
	 * @return
	 */
	work_type get_worker() {
		std::unique_lock<std::mutex> lock(queue_lock_);
		queue_condition_.wait(lock, [this]() {
			return !work_queue_.empty();
		});

		work_type val = std::move(work_queue_.front());
		work_queue_.pop();
		return val;
	}

private:
	std::vector<std::thread> threads_;
	std::queue<work_type>    work_queue_;
	std::mutex               queue_lock_;
	std::condition_variable  queue_condition_;
};

#endif
