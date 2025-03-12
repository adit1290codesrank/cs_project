#include <crow.h>
#include <crow/middlewares/cors.h>
#include "priority_queue.h"
#include "heap.h"
#include <iostream>

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
        MaxHeap heap;
        heap.push(13);
        heap.push(8);
        heap.push(6);
        heap.push(10);
        std::string res = std::to_string(heap.pop());
        res += " " + std::to_string(heap.pop());
        res += " " + std::to_string(heap.pop());
        res += " " + std::to_string(heap.pop());
        return res;
        });

    CROW_ROUTE(app, "/wink/<int>/<int>")([](int a, int b) {
        std::string res = "Answer of " + std::to_string(a) + " modulo " + std::to_string(b) + " is " + std::to_string(a % b);
        return res;
        });

    CROW_ROUTE(app, "/get_data").methods(crow::HTTPMethod::POST)([]() {
        crow::response response("Tashya ek lodu hain.");
        return response;
        });
    app.port(8080).multithreaded().run();
}