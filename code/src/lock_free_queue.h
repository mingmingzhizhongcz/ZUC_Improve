/************************************************************************
File name:	lock_free_queue.h
Date:	Oct 19,2019
Description: Definition file for unlocked loop queues
class List:LockFreeQueue//Circular queue buffer
The members of the class:1.LockFreeQueue//the construction function 
                         2.~LockFreeQueue//he destruction function 
						 3.push//enqueue the T-type data
						 4.pop//dequeue the T-type data
						 5.Node//Buffer node
						 6._head//Global head pointer
						 7._tail//Global tail pointer
						 8.CacheLinePad1,CacheLinePad2,CacheLinePad3//Used to solve false sharing  problem of _head and _tail
						 9._capacity:Actual buffer size，it's a power of two.
Other:Suppose the CacheLine size is 64 byte
**************************************************************************/
#pragma once
#include <atomic>
#include <cstddef>
template <typename T> class LockFreeQueue
{
public:
	explicit LockFreeQueue(size_t capacity)
	{
		_capacityMask = capacity - 1;
		for (size_t i = 1; i <= sizeof(void*) * 4; i <<= 1)
			_capacityMask |= _capacityMask >> i;
		_capacity = _capacityMask + 1;//将缓冲区容量扩充至2的整次方
		_queue = new Node[_capacity];
		for (size_t i = 0; i < _capacity; ++i)
		{
			_queue[i].tail.store(i, std::memory_order_relaxed);
			_queue[i].head.store(-1, std::memory_order_relaxed);
		}

		_tail.store(0, std::memory_order_relaxed);
		_head.store(0, std::memory_order_relaxed);
	}

	~LockFreeQueue()
	{
		delete[] _queue;
	}

	bool push(const T& data)
	{
		Node* node;
		size_t tail = _tail.load(std::memory_order_relaxed);
		for (;;)
		{
			node = &_queue[tail & _capacityMask];
			if (node->tail.load(std::memory_order_relaxed) != tail)
				return false;
			if ((_tail.compare_exchange_weak(tail, tail + 1, std::memory_order_relaxed)))
				break;
		}

		node->data = data;
		node->head.store(tail, std::memory_order_release);//内存屏障，保证资源释放一致性
		return true;
	}
	bool pop(T& result)
	{
		Node* node;
		size_t head = _head.load(std::memory_order_relaxed);
		for (;;)
		{
			node = &_queue[head & _capacityMask];
			if (node->head.load(std::memory_order_relaxed) != head)
				return false;
			if (_head.compare_exchange_weak(head, head + 1, std::memory_order_relaxed))
				break;
		}
		result = node->data;
		node->tail.store(head + _capacity, std::memory_order_release); //内存屏障，保证资源释放一致性
		return true;
	}

private:
	struct Node
	{
		T data;
		std::atomic<size_t> tail;
		std::atomic<size_t> head;
	};

private:
	size_t _capacityMask;
	Node* _queue;
	size_t _capacity;
	char cacheLinePad1[64];
	std::atomic<size_t> _tail;
	char cacheLinePad2[64];
	std::atomic<size_t> _head;
	char cacheLinePad3[64];
};