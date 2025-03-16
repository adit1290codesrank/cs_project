#include <crow.h>
#include <crow/middlewares/cors.h>
#include "min_cash_flow.h"
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

    CROW_ROUTE(app, "/test")([]() {
        auto page = crow::mustache::load("test.html");
        return page.render();
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
        std::string response = min_cash_flow(graph);
        return crow::response(200, response);
        });

    app.port(80).multithreaded().run();
}