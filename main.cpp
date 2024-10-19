#include <iostream>
#include <pqxx/pqxx>
#include <crow.h>

crow::SimpleApp app;

using namespace std;

pqxx::connection connect() {
    pqxx::connection C("dbname=postgres user=ozgun password=5432");
    if (!C.is_open()) {
        throw runtime_error("Can't open database");
    }
    return C;
}

void addContact(const string& name, const string& surname, const string& phone_number, const string& email, const string& notes) {
    pqxx::connection C = connect();
    pqxx::work W(C);

    C.prepare("insert_contact",
        "INSERT INTO contacts (name, surname, phone_number, email, notes) VALUES ($1, $2, $3, $4, $5)");

    W.exec_prepared("insert_contact", name, surname, phone_number, email, notes);
    W.commit();
}

void updateContact(int id, const string& name, const string& surname, const string& phone_number, const string& email, const string& notes) {
    pqxx::connection C = connect();
    pqxx::work W(C);

    C.prepare("update_contact",
        "UPDATE contacts SET name = $1, surname = $2, phone_number = $3, email = $4, notes = $5 WHERE id = $6");

    W.exec_prepared("update_contact", name, surname, phone_number, email, notes, id);
    W.commit();
}

void deleteContact(int id) {
    pqxx::connection C = connect();
    pqxx::work W(C);

    C.prepare("delete_contact", "DELETE FROM contacts WHERE id = $1");

    W.exec_prepared("delete_contact", id);
    W.commit();
}

crow::json::wvalue listContacts() {
    pqxx::connection C = connect();
    pqxx::nontransaction N(C);

    pqxx::result R = N.exec("SELECT id, name, surname, phone_number, email, notes FROM contacts");
    crow::json::wvalue contacts;

    for (const auto& row : R) {
        contacts[row[0].as<int>()] = {
            {"name", row[1].as<string>()},
            {"surname", row[2].as<string>()},
            {"phone_number", row[3].as<string>()},
            {"email", row[4].as<string>()},
            {"notes", row[5].as<string>()}
        };
    }

    return contacts;
}

int main() {
    CROW_ROUTE(app, "/contacts")
        .methods("GET"_method)([](const crow::request& req) {
            return listContacts();
        });

    CROW_ROUTE(app, "/contacts/add")
        .methods("POST"_method)([](const crow::request& req) {
            auto json = crow::json::load(req.body);
            if (!json) return crow::response(400, "Invalid JSON");

            string name = json["name"].s();
            string surname = json["surname"].s();
            string phone_number = json["phone_number"].s();
            string email = json["email"].s();
            string notes = json["notes"].s();

            try {
                addContact(name, surname, phone_number, email, notes);
                return crow::response(201, "Contact added successfully!");
            } catch (const exception& e) {
                return crow::response(500, e.what());
            }
        });

    CROW_ROUTE(app, "/contacts/update/<int>")
        .methods("POST"_method)([](const crow::request& req, int id) {
            auto json = crow::json::load(req.body);
            if (!json) return crow::response(400, "Invalid JSON");

            string name = json["name"].s();
            string surname = json["surname"].s();
            string phone_number = json["phone_number"].s();
            string email = json["email"].s();
            string notes = json["notes"].s();

            try {
                updateContact(id, name, surname, phone_number, email, notes);
                return crow::response(200, "Contact updated successfully!");
            } catch (const exception& e) {
                return crow::response(500, e.what());
            }
        });

    CROW_ROUTE(app, "/contacts/delete/<int>")
        .methods("DELETE"_method)([](const int id) {
            try {
                deleteContact(id);
                return crow::response(200, "Contact deleted successfully!");
            } catch (const exception& e) {
                return crow::response(500, e.what());
            }
        });

    app.port(18082).multithreaded().run();
    return 0;
}
