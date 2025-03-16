#include <crow.h>
#include <crow/middlewares/cors.h>
#include "graph.h"
#include "heap.h"
#include <iostream>
#include <sql.h>

int main() {
    crow::App<crow::CORSHandler> app;

    auto& cors = app.get_middleware<crow::CORSHandler>();

    cors
        .global()
        .headers("X-Custom-Header", "Upgrade-Insecure-Requests")
        .methods("POST"_method, "GET"_method)
        .prefix("/cors")
        .origin("example.com")
        .prefix("/nocors")
        .ignore();

    CROW_ROUTE(app, "/test_queue")([]() {
        MaxHeap<std::string> heap;
        heap.push("string 1", 10);
        heap.push("string 2", 6);
        heap.push("string 3", 9);
        heap.push("string 4", 12);
        std::string res = std::to_string(heap.pop().second);
        res += " " + std::to_string(heap.pop().second);
        res += " " + std::to_string(heap.pop().second);
        res += " " + std::to_string(heap.pop().second);
        return res;
        });

    CROW_ROUTE(app, "/test")([]() {
        auto page = crow::mustache::load("test.html");
        return page.render();
        });

    CROW_ROUTE(app, "/name/<string>")([](std::string name) {
        auto page = crow::mustache::load("fancypage.html");
        crow::mustache::context ctx({ {"person", name} });
        return page.render(ctx);
        });

    CROW_ROUTE(app, "/wink/<int>/<int>")([](int a, int b) {
        std::string res = "Answer of " + std::to_string(a) + " modulo " + std::to_string(b) + " is " + std::to_string(a % b);
        return res;
        });

    CROW_ROUTE(app, "/get_data").methods(crow::HTTPMethod::POST)([]() {
        crow::response response("Tashya ek lodu hain.");
        return response;
        });
    
    CROW_ROUTE(app, "/calculate").methods(crow::HTTPMethod::POST)([](const crow::request& req) {
        crow::json::rvalue data = crow::json::load(req.body);
        int size = data.size();
        Graph graph(size);
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                graph.insertEdge(i, j, std::stod(data[i][j].s()));
            }
        }
        MaxHeap<std::string> creditors;
        MinHeap<std::string> debitors;
        for (int i = 0; i < size; i++)
        {
            std::string name = "Person " + std::to_string(i + 1);
            int amount = 0;
            for (int j = 0; j < size; j++)
            {
                amount += graph.getEdge(j, i) - graph.getEdge(i, j);
            }
            if (amount < 0) debitors.push(name, amount);
            if (amount > 0) creditors.push(name, amount);
        }
        std::string response = "";
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
        return crow::response(200, response);
        });

    app.port(80).multithreaded().run();
}