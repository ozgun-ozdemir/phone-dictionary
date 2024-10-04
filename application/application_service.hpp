#ifndef APPLICATION_SERVER_HPP
#define APPLICATION_SERVER_HPP

#include "../domain/contact/domain_service.hpp"

using namespace std;

namespace ApplicationServer {
    inline void addContact(pqxx::connection& dbConnection, const string& name, const string& surname, const string& phone, const string& email, const string& notes) {
        DomainService::addContact(dbConnection, name, surname, phone, email, notes);
    }

    inline void updateContact(pqxx::connection& dbConnection, const int id, const string& name, const string& surname, const string& phone, const string& email, const string& notes) {
        DomainService::updateContact(dbConnection, id, name, surname, phone, email, notes);
    }

    inline void deleteContact(pqxx::connection& dbConnection, const int id) {
        DomainService::deleteContact(id, dbConnection);
    }

    inline crow::json::wvalue listContacts(pqxx::connection& dbConnection) {
        return DomainService::listContacts(dbConnection);
    }
}

#endif
