#pragma once
#include <vector>
#include <stdexcept>

/*A heap is a binary tree where the children node are either greater or lesser than the parent node.
We can implement a priority queue using a MaxHeap/MinHeap.
We can implement tree as an array where for every i index
->2*i + 1 is the left node
->2*i + 2 is the right node
-> (i-1)/2 is the parent node

Insertion of an element is O(logn)
Deletion of an element is O(logn)
Accessing Max/Min element is O(1)

Space Complexity is O(n)
*/

template <typename type> class MaxHeap {
	private:
		int size;
		std::vector<std::pair<type, int>> array;

		//Functions to maintain the heap property of array
		void heapify_pop(int i);
		void heapify_push(int i);

	public:
		//Constructor
		MaxHeap();

		//To check if given heap is empty
		bool isEmpty();

		//Add element according to priority
		void push(type T, int priority);

		//Return element of highest priority
		std::pair<type, int> pop();
};

template <typename type>
void MaxHeap<type>::heapify_pop(int i)
{
	int max = i;
	if (2 * i + 1 < size && array[2 * i + 1].second > array[max].second)
	{
		max = 2 * i + 1;
	}
	if (2 * i + 2 < size && array[2 * i + 2].second > array[max].second)
	{
		max = 2 * i + 2;
	}
	if (max != i)
	{
		std::swap(array[i], array[max]);
		heapify_pop(max);
	}
}

template <typename type>
void MaxHeap<type>::heapify_push(int i)
{
	if (i > 0 && array[i].second > array[(i - 1) / 2].second)
	{
		std::swap(array[i], array[(i - 1) / 2]);
		heapify_push((i - 1) / 2);
	}
}

template <typename type>
MaxHeap<type>::MaxHeap()
{
	size = 0;
}

template <typename type>
bool MaxHeap<type>::isEmpty()
{
	if (size > 0) return false;
	return true;
}

template <typename type>
void MaxHeap<type>::push(type T, int priority)
{
	array.push_back({ T,priority });
	size++;
	int index = size - 1;
	heapify_push(index);
}

template <typename type>
std::pair<type, int> MaxHeap<type>::pop()
{
	if (size == 0)
	{
		throw std::underflow_error::underflow_error("Heap underflow");
	}
	if (size == 1)
	{
		std::pair<type, int> data = array[0];
		array.pop_back();
		size--;
		return data;
	}
	std::pair<type, int> data = array[0];
	array[0] = array[size - 1];
	array.pop_back();
	size--;
	heapify_pop(0);
	return data;
}