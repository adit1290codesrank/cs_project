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
	std::vector<std::pair<std::string, int>> array;
	int size;
	void heapify_pop(int i)
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
	void heapify_push(int i)
	{
		if (i > 0 && array[i].second > array[(i - 1) / 2].second)
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

	bool isEmpty()
	{
		if (size > 0) return false;
		return true;
	}

	void push(std::pair<std::string, int> data)
	{
		array.push_back(data);
		size++;
		int index = size - 1;
		heapify_push(index);
	}

	std::pair<std::string, int> pop()
	{
		if (size == 0)
		{
			throw std::underflow_error::underflow_error("Heap underflow");
		}
		if (size == 1)
		{
			std::pair<std::string, int> data = array[0];
			array.pop_back();
			size--;
			return data;
		}
		std::pair<std::string, int> data = array[0];
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
		return array[0].second;
	}
};


class MinHeap {
	private:
		std::vector<std::pair<std::string, int>> array;
		int size;
		void heapify_pop(int i)
		{
			int min = i;
			if (2 * i + 1 < size && array[2 * i + 1].second < array[min].second)
			{
				min = 2 * i + 1;
			}
			if (2 * i + 2 < size && array[2 * i + 2].second < array[min].second)
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
			if (i > 0 && array[i].second < array[(i - 1) / 2].second)
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

		bool isEmpty()
		{
			if (size > 0) return false;
			return true;
		}

		void push(std::pair<std::string, int> data)
		{
			array.push_back(data);
			size++;
			int index = size-1;
			heapify_push(index);
		}

		std::pair<std::string, int> pop()
		{
			if (size == 0)
			{
				throw std::underflow_error::underflow_error("Heap underflow");
			}
			if (size == 1)
			{
				std::pair<std::string, int> data = array[0];
				array.pop_back();
				size--;
				return data;
			}
			std::pair<std::string, int> data = array[0];
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
			return array[0].second;
		}
};