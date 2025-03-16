#pragma once
#include "graph.h"
#include "max_heap.h"
#include "min_heap.h"
#include <string>
#include <tuple>


//Function which caluclates the minimum cash flow 
std::string min_cash_flow(Graph graph);

//Initiliazes heaps from graph
std::tuple < MaxHeap<std::string>, MinHeap<std::string>> initializeHeap(Graph graph);

std::string min_cash_flow(Graph graph)
{
	MaxHeap<std::string> creditors;
	MinHeap<std::string> debitors;
    std::tie(creditors, debitors) = initializeHeap(graph);
    std::string response;
    while (!creditors.isEmpty() && !debitors.isEmpty())
    {
        std::pair<std::string, int> max_creditor = creditors.pop();
        std::pair<std::string, int> max_debitor = debitors.pop();
        int amount = max_creditor.second + max_debitor.second;
        if (amount > 0)
        {
            response += max_debitor.first + " owes " + max_creditor.first + " " + std::to_string(-max_debitor.second) + "</br>";
            creditors.push(max_creditor.first, amount);
        }
        else if (amount < 0)
        {
            response += max_debitor.first + " owes " + max_creditor.first + " " + std::to_string(max_creditor.second) + "</br>";
            debitors.push(max_debitor.first, amount);
        }
        else
        {
            response += max_debitor.first + " owes " + max_creditor.first + " " + std::to_string(max_creditor.second) + "</br>";
        }
    }
    return response;
}

std::tuple < MaxHeap<std::string>, MinHeap<std::string>> initializeHeap(Graph graph)
{
	MaxHeap<std::string> creditors;
	MinHeap<std::string> debitors;
    for (int i = 0; i < graph.getSize(); i++)
    {
        std::string name = "Person " + std::to_string(i + 1);
        int amount = 0;
        for (int j = 0; j < graph.getSize(); j++)
        {
            amount += graph.getEdge(j, i) - graph.getEdge(i, j);
        }
        if (amount < 0) debitors.push(name, amount);
        if (amount > 0) creditors.push(name, amount);
    }

    return std::make_tuple(creditors, debitors);
}

