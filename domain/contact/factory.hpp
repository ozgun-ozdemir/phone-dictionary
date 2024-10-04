#ifndef FACTORY_HPP
#define FACTORY_HPP

#include "contact.hpp"
#include <pqxx/pqxx>
#include <crow.h>

using namespace std;

namespace ContactFactory {
    inline Contact create(const string& name, const string& surname, const string& phone, const string& email, const string& notes) {
        return Contact{-1, name, surname, phone, email, notes};
    }

    inline crow::json::wvalue createJsonFromResult(const pqxx::result& result) {
        crow::json::wvalue contacts;
        for (const auto& row : result) {
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
}

#endif