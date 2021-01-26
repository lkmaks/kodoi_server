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
    QFile file_;

    QString GetPassword(const QString &name, bool &exists);
    void AddUser(const QString &name, const QString &password, bool only_username = false);
};

#endif // LOGINSYSTEM_H
