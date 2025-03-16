#pragma once
#include <stdlib.h>
#include <stdexcept>

class Graph {
	private:
		int size;
		double** array;
	public:

		//Constructor
		Graph(int n);

		//Adding edge between i->j
		void insertEdge(int i, int j, double weight);

		//Getting edge between i->j
		double getEdge(int i, int j);

		//Returns size of graph
		int getSize();
};

Graph::Graph(int n)
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

void Graph::insertEdge(int i, int j, double weight)
{
	if (i >= size || j >= size) throw std::range_error::range_error("Exceeded graph size");
	array[i][j] = weight;
}

double Graph::getEdge(int i, int j)
{
	if (i >= size || j >= size) throw std::range_error::range_error("Exceeded graph size");
	return array[i][j];
}

int Graph::getSize()
{
	return size;
}
