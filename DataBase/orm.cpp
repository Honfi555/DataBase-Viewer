#include "DataBase/orm.h"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlIndex>
#include <QVariant>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlDriver>


ORMLevel::~ORMLevel()
{
    this->db_.removeDatabase(this->db_.connectionName());
    this->db_.close();
}


/*Return all values from current table.*/
QSqlRelationalTableModel *ORMLevel::get_allData(const QString &table_name) const
{
    return this->get_QSqlRelationalTableModel(table_name);
}


/*Return name of primary key column.*/
QString ORMLevel::get_pri(const QString &table_name) const
{
    QSqlTableModel model(nullptr, this->db_);
    model.setTable(table_name);

    return model.primaryKey().fieldName(0);
}


/*Return connection level name.*/
QString ORMLevel::get_level() const
{
    return "ORM";
}


/*Add new row to the table.*/
void ORMLevel::add_row(const QString &table_name, const QVector<QString> &new_row) const
{
    QSqlRelationalTableModel *model = this->get_QSqlRelationalTableModel(table_name);

    model->database().transaction();
    model->setTable(table_name);
    model->insertRow(model->rowCount());

    for (int column = 0; column < model->columnCount(); column++) {
        model->setData(model->index(model->rowCount() - 1, column), new_row.at(column));
    }

    if (model->submit()) model->database().commit();
    else {
        model->database().rollback();
        this->print_sql_error(model->database().lastError());
    }
}


/*Delete current row.*/
void ORMLevel::delete_row(const QString table_name, const QModelIndexList &indexes) const
{
    QSqlRelationalTableModel *model = this->get_QSqlRelationalTableModel(table_name);
    auto columns_params = this->get_replacment_columnParams(table_name);

    model->database().transaction();
    model->setTable(table_name);
    model->select();

    model->removeRow(indexes.at(0).row());

    for (int i = 0; i < columns_params.at(0).size(); i++) {
        if (columns_params.at(3).at(i) == "auto_increment") {
            this->restore_autoInc_order(table_name, columns_params.at(0).at(i));
        }
    }

    if (model->submit()) {
        model->database().commit();
    }
    else {
        model->database().rollback();
        this->print_sql_error(model->lastError());
    }
}


/*Update data at current row.*/
void ORMLevel::update_row(const QString table_name, const QVector<QString> &new_data, const QModelIndexList &indexes) const
{
    QSqlRelationalTableModel *model = this->get_QSqlRelationalTableModel(table_name);
    QVector<QString> columns_extra = this->get_replacment_columnParams(table_name).at(3);

    model->database().transaction();
    model->setTable(table_name);
    model->select();

    for (int column = 0; column < new_data.size(); column++) {
        QString insert_value = new_data.at(indexes.at(column).column());
        if (columns_extra.at(column) == "auto_increment") insert_value = QString::number(indexes.at(column).row());
        model->setData(model->index(indexes.at(column).row(), column), insert_value);
    }

    if (model->submit()) model->database().commit();
    else {
        model->database().rollback();
        qDebug() << model->lastError();
        this->print_sql_error(model->lastError());
    }
}


/*Return QSqlRelationalTableModel pointer with all relations.*/
QSqlRelationalTableModel *ORMLevel::get_QSqlRelationalTableModel(const QString &table_name) const
{
    QSqlRelationalTableModel *model = new QSqlRelationalTableModel(nullptr, this->db_);
    model->setTable(table_name);

    QVector<QString> columns_names = this->get_replacment_columnParams(table_name).at(0);
    QVector<QVector<QString>> info_columns_params = this->get_infoTables_params();

    if (info_columns_params.at(0).contains(table_name)) {
        model->select();

        return model;
    }

    for (const QString &column_name : columns_names) {
        if (info_columns_params.at(2).contains(column_name)) {
            int replacment_index = info_columns_params.at(2).indexOf(column_name);
            model->setRelation(columns_names.indexOf(column_name)
                               , QSqlRelation(info_columns_params.at(0).at(replacment_index)
                                              , info_columns_params.at(1).at(replacment_index)
                                              , info_columns_params.at(2).at(replacment_index)));
        }
    }
    model->select();

    return model;
}


/*Show user info about error.*/
void ORMLevel::print_sql_error(const QSqlError &error) const
{
    QString error_text = "Action not completed!";
#ifdef DEBUG_SQL_QUERY
    error_text += "\n Sql error - " + error.text();
#endif
    QMessageBox::warning(new QWidget(), "Error", error_text);
}


/*Return index if primary column.*/
int ORMLevel::get_pri_index(const QString &table_name) const
{
    QSqlTableModel model(nullptr, this->db_);
    model.setTable(table_name);

    return model.primaryKey().indexOf(this->get_pri(table_name));
}


/*Return all values from searching.*/
QSqlRelationalTableModel *ORMLevel::get_searchData(QMap<QString, QString> search_condition
                                                   , const QSearchType search_type) const
{
    QSqlRelationalTableModel *model = new QSqlRelationalTableModel(nullptr, this->db_);
    model->setTable(search_condition["table_name"]);

    QString search_filter("'" + search_condition["condition1_value"] + "'"
                             + " " + search_condition["condition1_property"]
                             + " " + search_condition["column_name"]);

    if (search_type == QSearchType::Complicated) {
        search_filter.append(" and " + search_condition["column_name"]
                                + " " + search_condition["condition2_property"]
                                + " '" + search_condition["condition2_value"] + "'");
    }

    model->setFilter(search_filter);
    model->select();
    return model;
}

