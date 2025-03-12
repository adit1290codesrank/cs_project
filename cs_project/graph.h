#pragma once
#include <stdlib.h>
#include <stdexcept>

class Graph {
	private:
		int size;
		int** array;
	public:
		Graph(int n)
		{
			size = n;
			array = new int* [size];
			for (int i = 0; i < size; i++)
			{
				array[i] = new int[size];
			}
		}
		//i->j
		void insertEdge(int i, int j, int weight)
		{
			if (i >= size || j >= size) return;
			array[i][j] = weight;
		}

		int getEdge(int i, int j)
		{
			if (i >= size || j >= size) throw std::overflow_error::overflow_error("Exceeded graph size");
			return array[i][j];
		}
};