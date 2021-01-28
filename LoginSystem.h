#ifndef LOGINSYSTEM_H
#define LOGINSYSTEM_H

#include <QString>
#include <QFile>

class LoginSystem
{
public:
    LoginSystem();
    bool TryLogin(const QString &name, const QString &password);
    QString MakeGuestName();
private:
    std::string filename_;
    unsigned long long guest_id_;

    QString GetPassword(const QString &name, bool &exists);
    void AddUser(const QString &name, const QString &password, bool only_username = false);
};

#endif // LOGINSYSTEM_H
