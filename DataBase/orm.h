#ifndef ORM_H
#define ORM_H

#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QVector>
#include <QString>
#include <QMap>
#include <QTableWidgetItem>

#include "types.h"
#include "DataBase/database.h"


class ORMLevel : public DataBase
{
public:
    explicit ORMLevel(const QString& user_name = "reader",
                      const QString& user_password = "fopol91goh",
                      const QString& sql_driver = "QMYSQL",
                      const QString& ip = "127.0.0.1") : DataBase(user_name, user_password, sql_driver, ip) {}
    ~ORMLevel();

    [[nodiscard]] QSqlRelationalTableModel* get_allData(const QString& table_name) const override;
    [[nodiscard]] QSqlRelationalTableModel *get_searchData(QMap<QString, QString> search_condition
                                                           , const QSearchType search_type) const override;

    [[nodiscard]] QString get_pri(const QString &table_name) const override;

    [[nodiscard]] QString get_level() const override;

public slots:
    void add_row(const QString &table_name, const QVector<QString>& row_data) const override;

    void delete_row(const QString table_name, const QModelIndexList& indexes) const override;

    void update_row(const QString table_name, const QVector<QString> &new_data
                    , const QModelIndexList& indexes) const override;

private:
    [[nodiscard]] QSqlRelationalTableModel *get_QSqlRelationalTableModel(const QString &table_name) const;

    [[nodiscard]] int get_pri_index(const QString &table_name) const;

    void print_sql_error(const QSqlError &error) const;


    QSqlDatabase db_;
};

#endif // ORM_H
