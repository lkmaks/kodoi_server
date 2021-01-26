#include "LoginSystem.h"

#include <QTextStream>


LoginSystem::LoginSystem() : file_("users.txt")
{
    file_.open(QIODevice::ReadWrite);
}

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
}


QString LoginSystem::GetPassword(const QString &name, bool &exists) {
    QTextStream in(&file_);
    while (!in.atEnd())
    {
        QString line = in.readLine();
        auto vec = line.split(';');
        if (name == vec[0]) {
            exists = true;
            return vec[1];
        }
    }
    exists = false;
    return "";
}

void LoginSystem::AddUser(const QString &name, const QString &password, bool only_username) {
    // need to append to file: name;password
}
