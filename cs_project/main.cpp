#include <crow.h>
#include <crow/middlewares/cors.h>
#include "min_cash_flow.h"
#include "utils.h"
#include <iostream>
#include <boost/mysql/any_connection.hpp>
#include <boost/mysql/connect_params.hpp>
#include <boost/mysql/error_with_diagnostics.hpp>
#include <boost/mysql/results.hpp>
#include <boost/asio/io_context.hpp>

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

    CROW_ROUTE(app, "/login_user").methods(crow::HTTPMethod::POST)([](const crow::request& req) {
        std::map<std::string, std::string> env = readEnv("./env.txt");
        crow::json::rvalue data = crow::json::load(req.body);
        std::string username = data[0].s();
        std::string password = data[1].s();
        boost::asio::io_context ctx;
        boost::mysql::any_connection conn(ctx);
        boost::mysql::connect_params params;
        params.server_address.emplace_host_and_port(env["HOST"], std::stod(env["PORT"]));
        params.username = env["USER"];
        params.password = env["PWD"];
        params.database = env["DB_USER"];
        conn.connect(params);
        boost::mysql::results result;
        const std::string statement = "SELECT * FROM users WHERE username=\"" + username + "\" AND pwd=\"" + password + "\"";
        conn.execute(statement, result);
        if (result.rows().empty()) return crow::response(200, "[false]");
        Encryption sha256(username);
        std::string encrypted = sha256.encrypt_sha256();
        std::string statement_insert = "INSERT INTO login_id VALUES(\"" + encrypted + "\", \"" + username + "\")";
        conn.execute(statement_insert, result);
        conn.close();
        return crow::response(200, "[true, \"" + encrypted +"\"]");
    });

    CROW_ROUTE(app, "/validate_login").methods(crow::HTTPMethod::POST)([](const crow::request& req) {
        std::map<std::string, std::string> env = readEnv("./env.txt");
        crow::json::rvalue data = crow::json::load(req.body);
        std::string login_id = data[0].s();
        boost::asio::io_context ctx;
        boost::mysql::any_connection conn(ctx);
        boost::mysql::connect_params params;
        params.server_address.emplace_host_and_port(env["HOST"], std::stod(env["PORT"]));
        params.username = env["USER"];
        params.password = env["PWD"];
        params.database = env["DB_USER"];
        conn.connect(params);
        boost::mysql::results result_insert;
        const std::string statement_insert = "SELECT username FROM login_id WHERE login_id=\"" + login_id + "\"";
        conn.execute(statement_insert, result_insert);
        conn.close();
        if (result_insert.rows().empty()) return crow::response(200, "[false]");
        std::string result_string = result_insert.rows().at(0).at(0).as_string();
        return crow::response(200, "[true,\"" + result_string + "\" ]");
    });

    CROW_ROUTE(app, "/logout").methods(crow::HTTPMethod::POST)([](const crow::request& req) {
        std::map<std::string, std::string> env = readEnv("./env.txt");
        crow::json::rvalue data = crow::json::load(req.body);
        std::string login_id = data[0].s();
        boost::asio::io_context ctx;
        boost::mysql::any_connection conn(ctx);
        boost::mysql::connect_params params;
        params.server_address.emplace_host_and_port(env["HOST"], std::stod(env["PORT"]));
        params.username = env["USER"];
        params.password = env["PWD"];
        params.database = env["DB_USER"];
        conn.connect(params);
        std::string statement = "DELETE FROM login_id WHERE login_id=\"" + login_id + "\"";
        boost::mysql::results result;
        conn.execute(statement, result);
        return crow::response("[true]");
    });

    CROW_ROUTE(app, "/login")([]() {
        auto page = crow::mustache::load("login.html");
        return page.render();
    });

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