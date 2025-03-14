#pragma once
#include <stdlib.h>
#include <stdexcept>

class Graph {
	private:
		int size;
		double** array;
	public:
		Graph(int n)
		{
			size = n;
			array = new double* [size];
			for (int i = 0; i < size; i++)
			{
				array[i] = new double[size];
				for (int j = 0; j < size; j++)
				{
					array[i][j] = 0;
				}
			}
		}
		//Adding edge between i->j
		void insertEdge(int i, int j, double weight)
		{
			if (i >= size || j >= size) return;
			array[i][j] = weight;
		}

		double getEdge(int i, int j)
		{
			if (i >= size || j >= size) throw std::overflow_error::overflow_error("Exceeded graph size");
			return array[i][j];
		}
};