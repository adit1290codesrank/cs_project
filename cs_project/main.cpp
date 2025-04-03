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

    CROW_ROUTE(app, "/add_group_post").methods(crow::HTTPMethod::POST)([](const crow::request& req) {
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
        boost::mysql::results result_select;
        const std::string statement_select = "SELECT username FROM login_id WHERE login_id=\"" + login_id + "\"";
        conn.execute(statement_select, result_select);
        conn.close();
        std::string owner = result_select.rows().at(0).at(0).as_string();
        std::string name = data[1].s();
        std::string desc = data[2].s();
        std::string image_base_url;
        if (data[3].b()) image_base_url = data[4].s();
        else image_base_url = "null";
        boost::asio::io_context ctx2;
        boost::mysql::any_connection conn_group(ctx2);
        boost::mysql::connect_params params_group;
        params_group.server_address.emplace_host_and_port(env["HOST"], std::stod(env["PORT"]));
        params_group.username = env["USER"];
        params_group.password = env["PWD"];
        params_group.database = env["DB_GROUP"];
        conn_group.connect(params_group);
        boost::mysql::results result_insert;
        Encryption sha256(owner + name);
        std::string group_id = sha256.encrypt_sha256();
        std::string statement_insert;
        if (data[3].b())
        {
            statement_insert = "INSERT INTO _groups VALUES(\"" + group_id +"\", \""+owner+"\", \"["+owner+",]\", \""+name+"\", \""+desc+"\", \"" + image_base_url+"\")";
        }
        else
        {
            statement_insert = "INSERT INTO _groups VALUES(\"" + group_id + "\", \"" + owner + "\", \"["+owner+"]\", \"" + name + "\", \"" + desc + "\")";
        }
        conn_group.execute(statement_insert, result_insert);
        conn_group.close();
        return crow::response(200);
    });

    CROW_ROUTE(app, "/find_groups").methods(crow::HTTPMethod::POST)([](const crow::request& req) {
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
        boost::mysql::results result_select;
        const std::string statement_select = "SELECT username FROM login_id WHERE login_id=\"" + login_id + "\"";
        conn.execute(statement_select, result_select);
        conn.close();
        std::string user = result_select.rows().at(0).at(0).as_string();
        boost::asio::io_context ctx2;
        boost::mysql::any_connection conn2(ctx2);
        boost::mysql::connect_params params2;
        params2.server_address.emplace_host_and_port(env["HOST"], std::stod(env["PORT"]));
        params2.username = env["USER"];
        params2.password = env["PWD"];
        params2.database = env["DB_GROUP"];
        conn2.connect(params2);
        boost::mysql::results result_select2;
        const std::string statement_select2 = "SELECT * FROM _groups WHERE members LIKE \"%" + user + ",%\"";
        conn2.execute(statement_select2, result_select2);
        conn2.close();
        if (!result_select2.rows().empty())
        {
            std::string data = "[";
            for (int i = 0; i < result_select2.rows().size(); i++)
            {
                data =  data + "[";
                for (int j = 0; j < 6; j++)
                {
                    std::string to_add = result_select2.rows().at(i).at(j).as_string();
                    data = data + "\""+to_add +"\""+ ",";
                }
                data.pop_back();
                data = data + "],";
            }
            data.pop_back();
            data = data + "]";
            return crow::response(200, data);
        }
        return crow::response(200, "[false]");
    });

    CROW_ROUTE(app, "/find_trans").methods(crow::HTTPMethod::POST)([](const crow::request& req) {
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
        boost::mysql::results result_select;
        const std::string statement_select = "SELECT username FROM login_id WHERE login_id=\"" + login_id + "\"";
        conn.execute(statement_select, result_select);
        conn.close();
        std::string user = result_select.rows().at(0).at(0).as_string();
        boost::asio::io_context ctx2;
        boost::mysql::any_connection conn2(ctx2);
        boost::mysql::connect_params params2;
        params2.server_address.emplace_host_and_port(env["HOST"], std::stod(env["PORT"]));
        params2.username = env["USER"];
        params2.password = env["PWD"];
        params2.database = env["DB_GROUP"];
        conn2.connect(params2);
        boost::mysql::results result_select2;
        const std::string statement_select2 = "SELECT * FROM _groups WHERE members LIKE \"%" + user + ",%\"";
        conn2.execute(statement_select2, result_select2);
        conn2.close();
        if (!result_select2.rows().empty())
        {
            std::string data = "[";
            for (int i = 0; i < result_select2.rows().size(); i++)
            {
                data = data + "[";
                for (int j = 0; j < 6; j++)
                {
                    std::string to_add = result_select2.rows().at(i).at(j).as_string();
                    data = data + "\"" + to_add + "\"" + ",";
                }
                data.pop_back();
                data = data + "],";
            }
            data.pop_back();
            data = data + "]";
            return crow::response(200, data);
        }
        return crow::response(200, "[false]");
        });

    CROW_ROUTE(app, "/add_member/<string>")([](const std::string& group_id) {
        auto page = crow::mustache::load("add_member.html");
        return page.render();
    });


    CROW_ROUTE(app, "/group/<string>")([](const std::string& group_id) {
        auto page = crow::mustache::load("group_page.html");
        std::map<std::string, std::string> env = readEnv("./env.txt");
        boost::asio::io_context ctx;
        boost::mysql::any_connection conn(ctx);
        boost::mysql::connect_params params;
        params.server_address.emplace_host_and_port(env["HOST"], std::stod(env["PORT"]));
        params.username = env["USER"];
        params.password = env["PWD"];
        params.database = env["DB_GROUP"];
        conn.connect(params);
        std::string statement = "SELECT * from _groups WHERE group_id=\"" + group_id + "\"";
        boost::mysql::results result;
        conn.execute(statement, result);
        conn.close();
        std::string group_owner = result.rows().at(0).at(1).as_string();
        std::string group_members = result.rows().at(0).at(2).as_string();
        std::string group_name = result.rows().at(0).at(3).as_string();
        std::string group_desc = result.rows().at(0).at(4).as_string();
        std::string group_image = result.rows().at(0).at(5).as_string();
        group_members.pop_back();
        group_members.erase(0, 1);
        group_members.pop_back();
        std::vector<std::string> members;
        std::stringstream ss(group_members);
        while (ss.good()) {
            std::string substr;
            std::getline(ss, substr, ',');
            if (substr != group_owner)
            {
                members.push_back(substr);
            }
        }
        std::string members_html = "";
        for (int i = 0; i < members.size(); i++)
        {
            members_html = members_html + "<li class=\"list-group-item\">" + members[i] + "</li>";
        }
        crow::mustache::context ctx_page({ {"group_name", group_name}, {"group_owner", group_owner}, {"group_desc", group_desc}, {"img_src", group_image}, {"group_members", members_html} });
        return page.render(ctx_page);
    });
    
    CROW_ROUTE(app, "/add_member_post").methods(crow::HTTPMethod::POST)([](const crow::request& req) {
        std::map<std::string, std::string> env = readEnv("./env.txt");
        crow::json::rvalue data = crow::json::load(req.body);
        std::string group_id = data[1].s();
        std::string login_id = data[0].s();
        boost::asio::io_context ctx;
        boost::mysql::any_connection conn(ctx);
        boost::mysql::connect_params params;
        params.server_address.emplace_host_and_port(env["HOST"], std::stod(env["PORT"]));
        params.username = env["USER"];
        params.password = env["PWD"];
        params.database = env["DB_USER"];
        conn.connect(params);
        std::string statement = "SELECT username from login_id WHERE login_id=\"" + login_id + "\"";
        boost::mysql::results result;
        conn.execute(statement, result);
        conn.close();
        std::string user = result.rows().at(0).at(0).as_string();
        boost::mysql::any_connection conn2(ctx);
        boost::mysql::connect_params params2;
        params2.server_address.emplace_host_and_port(env["HOST"], std::stod(env["PORT"]));
        params2.username = env["USER"];
        params2.password = env["PWD"];
        params2.database = env["DB_GROUP"];
        conn2.connect(params2);
        boost::mysql::results result2;
        std::string statement_select = "SELECT members FROM _groups WHERE group_id=\"" + group_id + "\"";
        conn2.execute(statement_select, result2);
        std::string members = result2.rows().at(0).at(0).as_string();
        if (members.find(user+",") != std::string::npos)
        {
            return crow::response("[false]");
        }
        members.pop_back();
        members = members +user + ",]";
        std::string statement_update = "UPDATE _groups SET members = \"" + members + "\" WHERE group_id=\"" + group_id + "\"";
        conn2.execute(statement_update, result2);
        conn2.close();
        return crow::response("[true]");
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
        conn.close();
        return crow::response("[true]");
    });

    CROW_ROUTE(app, "/login")([]() {
        auto page = crow::mustache::load("login.html");
        return page.render();
    });
    
    CROW_ROUTE(app, "/home")([]() {
        auto page = crow::mustache::load("home.html");
        return page.render();
    });
    
    CROW_ROUTE(app, "/add_group")([]() {
        auto page = crow::mustache::load("add_group.html");
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