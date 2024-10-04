#ifndef DOMAIN_SERVICE_HPP
#define DOMAIN_SERVICE_HPP

#include "repository.hpp"
#include "contact.hpp"
#include "factory.hpp"

using namespace std;

namespace DomainService {
    inline void addContact(pqxx::connection& dbConnection, const string &name, const string &surname, const string &phone, const string &email, const string &notes) {
        const Contact contact = ContactFactory::create(name, surname, phone, email, notes);
        Repository::addContact(contact, dbConnection);
    }

    inline void updateContact(pqxx::connection& dbConnection, const int id, const string &name, const string &surname, const string &phone, const string &email, const string &notes) {
        Contact contact = ContactFactory::create(name, surname, phone, email, notes);
        contact.id = id;
        Repository::updateContact(contact, dbConnection);
    }

    inline void deleteContact(int id, pqxx::connection& dbConnection) {
        Repository::deleteContact(id, dbConnection);
    }

    inline crow::json::wvalue listContacts(pqxx::connection& dbConnection) {
        return Repository::listContacts(dbConnection);
    }
}

#endif
