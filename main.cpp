#include <crow.h>
#include <pqxx/pqxx>
#include <iostream>
#include "../application/application_service.hpp"

using namespace std;

#include <pqxx/pqxx>
#include <stdexcept>
#include <memory>

crow::SimpleApp app;

inline pqxx::connection& getDatabaseConnection() {
    static pqxx::connection connection("dbname=postgres user=ozgun password=5432");
    if (!connection.is_open()) {
        throw ::runtime_error("Can't open database");
    }
    return connection;
}

int main() {
    CROW_ROUTE(app, "/contacts").methods("GET"_method)([]() {
        pqxx::connection& dbConnection = getDatabaseConnection();
        return ApplicationServer::listContacts(dbConnection);
    });

    CROW_ROUTE(app, "/contacts/add").methods("POST"_method)([](const crow::request& req) {
        pqxx::connection& dbConnection = getDatabaseConnection();
        auto json = crow::json::load(req.body);
        if (!json) {
            return crow::response(400, "Invalid JSON");
        }

        string name = json["name"].s();
        string surname = json["surname"].s();
        string phone_number = json["phone_number"].s();
        string email = json["email"].s();
        string notes = json["notes"].s();

        try {
            ApplicationServer::addContact(dbConnection, name, surname, phone_number, email, notes);
            return crow::response(201, "Contact added successfully!");
        } catch (const std::exception& e) {
            return crow::response(500, e.what());
        }
    });

    CROW_ROUTE(app, "/contacts/update/<int>").methods("POST"_method)([](const crow::request& req, int id) {
        pqxx::connection& dbConnection = getDatabaseConnection();
        auto json = crow::json::load(req.body);
        if (!json) {
            return crow::response(400, "Invalid JSON");
        }

        string name = json["name"].s();
        string surname = json["surname"].s();
        string phone_number = json["phone_number"].s();
        string email = json["email"].s();
        string notes = json["notes"].s();

        try {
            ApplicationServer::updateContact(dbConnection, id, name, surname, phone_number, email, notes);
            return crow::response(200, "Contact updated successfully!");
        } catch (const std::exception& e) {
            return crow::response(500, e.what());
        }
    });

    CROW_ROUTE(app, "/contacts/delete/<int>").methods("DELETE"_method)([](int id) {
        pqxx::connection& dbConnection = getDatabaseConnection();
        try {
            ApplicationServer::deleteContact(dbConnection, id);
            return crow::response(200, "Contact deleted successfully!");
        } catch (const std::exception& e) {
            return crow::response(500, e.what());
        }
    });

    app.port(18081).multithreaded().run();
}

