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
class MaxHeap {
private:
	std::vector<int> array;
	int size;
	void heapify_pop(int i)
	{
		int max = i;
		if (2 * i + 1 < size && array[2 * i + 1] > array[i])
		{
			max = 2 * i + 1;
		}
		if (2 * i + 2 < size && array[2 * i + 2] > array[i])
		{
			max = 2 * i + 2;
		}
		if (max != i)
		{
			std::swap(array[i], array[max]);
			heapify_pop(max);
		}
	}
	void heapify_push(int i)
	{
		if (i > 0 && array[i] > array[(i - 1) / 2])
		{
			std::swap(array[i], array[(i - 1) / 2]);
			heapify_push((i - 1) / 2);
		}
	}
public:
	MaxHeap()
	{
		size = 0;
	}
	void push(int data)
	{
		array.push_back(data);
		size++;
		int index = size - 1;
		heapify_push(index);
	}

	int pop()
	{
		if (size == 0)
		{
			throw std::underflow_error::underflow_error("Heap underflow");
		}
		if (size == 1)
		{
			int data = array[0];
			array.pop_back();
			size--;
			return data;
		}
		int data = array[0];
		array[0] = array[size - 1];
		array.pop_back();
		size--;
		heapify_pop(0);
		return data;
	}

	int top()
	{
		if (size == 0)
		{
			throw std::underflow_error::underflow_error("Heap underflow");
		}
		return array[0];
	}
};


class MinHeap {
	private:
		std::vector<int> array;
		int size;
		void heapify_pop(int i)
		{
			int min = i;
			if (2 * i + 1 < size && array[2 * i + 1] < array[i])
			{
				min = 2 * i + 1;
			}
			if (2 * i + 2 < size && array[2 * i + 2] < array[i])
			{
				min = 2 * i + 2;
			}
			if (min != i)
			{
				std::swap(array[i], array[min]);
				heapify_pop(min);
			}
		}
		void heapify_push(int i)
		{
			if (i > 0 && array[i] < array[(i - 1) / 2])
			{
				std::swap(array[i], array[(i - 1) / 2]);
				heapify_push((i - 1) / 2);
			}
		}
	public:
		MinHeap()
		{
			size = 0;
		}
		void push(int data)
		{
			array.push_back(data);
			size++;
			int index = size-1;
			heapify_push(index);
		}

		int pop()
		{
			if (size == 0)
			{
				throw std::underflow_error::underflow_error("Heap underflow");
			}
			if (size == 1)
			{
				int data = array[0];
				array.pop_back();
				size--;
				return data;
			}
			int data = array[0];
			array[0] = array[size - 1];
			array.pop_back();
			size--;
			heapify_pop(0);
			return data;
		}
		
		int top()
		{
			if (size == 0)
			{
				throw std::underflow_error::underflow_error("Heap underflow");
			}
			return array[0];
		}
};