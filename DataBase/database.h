#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>
#include <QStandardItemModel>
#include <QSqlRelationalTableModel>

#include "types.h"

// #define DEBUG_SQL_QUERYS
// #define DEGUB_SQL_CONNECT


class DataBase
{
public:
    explicit DataBase(const QString& user_name = "reader",
                       const QString& user_password = "fopol91goh",
                       const QString& sql_driver = "QMYSQL",
                       const QString& ip = "127.0.0.1");

    [[nodiscard]] virtual QVector<QString> get_tables() const;
    [[nodiscard]] virtual QVector<QString> get_dataBases() const;

    [[nodiscard]] virtual QSqlRelationalTableModel* get_allData(const QString&) const
    {return new QSqlRelationalTableModel();};
    [[nodiscard]] virtual QSqlRelationalTableModel* get_searchData(QMap<QString, QString>, const QSearchType) const
    {return new QSqlRelationalTableModel();};

    [[nodiscard]] virtual QVector<QVector<QString>> get_replacment_columnParams(const QString&) const;
    [[nodiscard]] virtual QMap<QString, QString> get_replacment_columnTypes(const QString&) const;

    [[nodiscard]] virtual QString get_pri(const QString&) const {return "";};

    [[nodiscard]] virtual QMap<QString, QVector<QString>> get_dialogData() const;

    virtual void set_database(const QString&);

    [[nodiscard]] virtual QSqlDatabase get_database() const;

    [[nodiscard]] virtual QSqlQuery get_allData_query(const QString &) const {return QSqlQuery();};

    [[nodiscard]] virtual QString get_level() const {return "";};

    virtual void remuve_connection();

    virtual QPair<QString, QString> get_credential() const;

public slots:
    virtual void add_row(const QString&, const QVector<QString>&) const {};

    virtual void delete_row(const QString, const QModelIndexList&) const {};

    virtual void update_row(const QString
                            , const QVector<QString> &
                            , const QModelIndexList&) const {};

protected:
    virtual void check_queryError(QSqlQuery &query) const;

    void print_Error(const QString &error) const;

    void restore_autoInc_order(const QString &table_name, const QString &autoInc_column) const;

    [[nodiscard]] virtual QSqlQuery get_sql_columnParams(const QString& table_name) const;

    [[nodiscard]] virtual QVector<QVector<QString>> from_QSqlQuery(QSqlQuery &query) const;

    [[nodiscard]] virtual QVector<QString> get_columnData(const QString &table_name, const int column_number) const;

    [[nodiscard]] virtual QVector<QVector<QString>> get_infoTables_params() const;

private:
    QSqlDatabase db_;
};

#endif // DATABASE_H
