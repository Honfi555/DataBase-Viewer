#ifndef IODATABASE_H
#define IODATABASE_H

#include <QString>
#include <QMap>
#include <QSqlRelationalTableModel>

#include "types.h"


class IODataBase
{
public:
    IODataBase(const QString &save_path = "reports") : save_path_(save_path) {}

    bool write_table(const QString &table_name, QSqlRelationalTableModel *values, QFileType file_type) const;

    // bool read_table(const QString &table_name, QFileType file_type);

    bool write_table_params(const QString &table_name, const QVector<QVector<QString>> &table_params) const;

    bool read_table_params(const QString &table_name);

    // QMap<QString, QStandardItemModel*> get_models() const;
    // QStandardItemModel* get_model(const QString &table_name) const;

private:
    [[nodiscard]] QString collectData_to_string(QSqlRelationalTableModel *values) const;

    void print_error(const QString &table_name) const;

    // QMap<QString, QStandardItemModel*> models_;
    QString save_path_;
};

#endif // IODATABASE_H
