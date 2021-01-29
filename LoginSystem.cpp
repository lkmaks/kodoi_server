#include "LoginSystem.h"

#include <fstream>


LoginSystem::LoginSystem() : filename_("users.txt"), guest_id_(0) {}


bool LoginSystem::TryLogin(const QString &name, const QString &password) {
    bool exists;
    auto actual_password = GetPassword(name, exists);
    if (!exists) {
        AddUser(name, password);
        return true;
    }
    else if (actual_password == password) {
        return true;
    }

    return false;
}


QString LoginSystem::MakeGuestName() {
    QString name = "guest" + QString::number(guest_id_);
    guest_id_++;
    AddUser(name, "", true);
    return name;
}


/// private

QString LoginSystem::GetPassword(const QString &name, bool &exists) {
    std::ifstream fin;
    fin.open(filename_);

    while (!fin.eof()) {
        std::string line;
        getline(fin, line);
        auto vec = QString::fromUtf8(line.c_str()).split(';');
        if (name == vec[0]) {
            exists = true;
            return vec[1];
        }
    }
    exists = false;

    fin.close();

    return "";
}

void LoginSystem::AddUser(const QString &name, const QString &password, bool only_username) {
    std::ofstream fout;
    fout.open(filename_, std::ios_base::app);

    fout << name.toStdString();
    if (!only_username) {
        fout << ";" << password.toStdString();
    }
    fout << std::endl;

    fout.close();
}
