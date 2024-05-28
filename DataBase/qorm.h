#ifndef QORM_H
#define QORM_H

#include <QString>
#include <QVector>
#include <QMap>


class QORM
{
public:
    QORM(const QString& sql_driver = "QMYSQL",
         const QString& user_name = "root",
         const QString& user_password = "fopol90goh",
         const QString& ip = "127.0.0.1") {}

    void set_database(const QString &db_name) {};

    [[nodiscard]] QVector<QString> get_dataBases() const {return {};};
    [[nodiscard]] QVector<QString> get_tables() const {return {};};


};

#endif // QORM_H
