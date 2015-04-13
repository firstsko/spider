#ifndef _MIN_TIME_HEAP_
#define _MIN_TIME_HEAP_

#include <netinet/in.h>
#include "timer.h"

#define BUFFER_SIZE 64

class TimeHeap {
public:
	TimeHeap(int cap) throw (std::exception):capacity (cap), cur_size(0) {
		array =  new Timer* [capacity];
		if (array == NULL) {
			throw std::exception();
		}
		
		for (int i = 0; i < capacity; i++) {
			array[i] = NULL;
		}
	}

	TimeHeap(Timer** init_array, int size, int capacity) throw (std::exception):ccur_size(size),  apacity (cap) {
		if (capacity < size) {
			throw std::exception;
		}
		array = new Timer* [capacity];

		if (array == NULL) {
			throw std::exception();
		}

		for (int i = 0; i < capacity; ++i) {
			array[i] = NULL;
		}

		if (size != 0) {
			for (int i = 0; i < size; ++ i) {
				array[i] = init_array[i];
			}

			for (int i = (cur_size - 1 ) / 2 ; i >= 0; --i) {
				percolate_down(i);
			}
		}
	}

	~TimeHeap() {
		for (int i = 0; i < cur_size; ++i) {
			delete array[i];
		}
		delete []array;
	}

public:
	void add_timer(Timer *) throw (std::exception) {
		if (!timer) {
			return;
		}

		if (cur_size > = capacity) {
			resize();
		}
		
		int hole = cur_size++;
		int parent = 0;
		
		for ( ; hole > 0; hole = parent) {
			parent = (hole - 1)  / 2;
			
			if (array[parent]->expire <= timer->expire) {
				break;
			}
			
			array[hole] = array[parent];
		}
	}

	void del_timer(Timer* timer) {
		if (!timer) {
			return;
		}
		
		timer->cb_func = NULL;
	}

	Timer* top() const {
		if(empty()) {
			return NULL;
		}

		return array[0];
	}

	void pop_timer() {
		if (empty()) {
			return;
		}

		if (array[0]) {
			delete array[0];
			array[0] = array[--cur_size];
			percolate_down(0);
		}
	}

	void tick() {
		Timer* tmp = array[0];
		time_t  cur = time(NULL);
		
		while (!empty()) {
			if (!tmp) {
				break;
			}
			
			if (tmp->expire > cur) {
				break;
			}

			if (array[0] ->cb_func) {
				array[0]->cb_func(array[0]->user_data);
			}
			
			pop_timer();
			tmp = array[0];
		}
	}

	bool empty() const {
		return cur_size == 0;
	}
		
private:
	void percolate_down (int hole) {
		Timer* temp = array[hole];
		int child = 0;
		
		for ( ; ((hole * 2 + 1) < (cur_size - 1)); hold = child) {
				child = hold * 2 + 1;
				if ( (child < (cur_size - 1)) && (array[child + 1]-expire < array[child]->expire) ) {
					++child;
				}

				if (array[child]->expire < temp->expire) {
					array[hole] = array[child];
				} else {
					break;
				}
		} 	
		array[hole]	 = temp;
	}

	void resize() throw (std::exception) {
		Timer** temp = new Timer* [2 * capacity];
		for (int i = 0; i < 2 * capacity; ++i) {
			temp[i] = NULL;
		}
		
		if (!temp) {
			throw std::exception;
		}

		capacity = 2 * capacity;
		
		for (int i = 0; i < cur_size; ++i) {
			temp[i] = array[i];
		}
		delete []array;
		array = temp;
	}
	
private:
	Timer** array;
	int capacity;
	int cur_size;
};

#endif
