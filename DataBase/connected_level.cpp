#include "DataBase/connected_level.h"

#include <QSqlQuery>
#include <QSqlField>
#include <QSqlError>
#include <QSqlRecord>
#include <QMessageBox>


ConnectedLevel::~ConnectedLevel()
{
    this->db_.removeDatabase(this->db_.connectionName());
    this->db_.close();
}

/*Return all values from current table.*/
QSqlRelationalTableModel* ConnectedLevel::get_allData(const QString& table_name) const
{
    QSqlQuery query(this->get_allData_query_str(table_name));
    QSqlRelationalTableModel *model = new QSqlRelationalTableModel(nullptr, this->db_);
    model->setTable(table_name);
    model->setQuery(std::move(query));

    return model;
}


/*Return all values from searching.*/
QSqlRelationalTableModel* ConnectedLevel::get_searchData(QMap<QString, QString> search_condition, const QSearchType search_type) const
{
    QSqlRelationalTableModel *model = new QSqlRelationalTableModel(nullptr, this->db_);
    model->setTable(search_condition["table_name"]);

    if (this->contains_infoTable(search_condition["table_name"])) {
        QString search_query_str = this->get_allData_query_str(search_condition["table_name"]);

        search_query_str.append(+ " WHERE '" + search_condition["condition1_value"] + "'"
                                + " " + search_condition["condition1_property"]
                                + " " + search_condition["column_name"] + ";");

        QSqlQuery query(search_query_str);
        model->setQuery(std::move(query));
        return model;
    }

    QString search_query_str("SELECT * FROM " + search_condition["table_name"]
                             + " WHERE " + " '" + search_condition["condition1_value"] + "'"
                             + " " + search_condition["condition1_property"]
                             + " " + search_condition["column_name"]);

    if (search_type == QSearchType::Complicated) {
        search_query_str.append(" and " + search_condition["column_name"]
                                + " " + search_condition["condition2_property"]
                                + " '" + search_condition["condition2_value"] + "'");
    }

    QSqlQuery query(search_query_str + ";");
    model->setQuery(std::move(query));

    return model;
}


/*Delete row from table with sql query by pri column.*/
void ConnectedLevel::delete_row(const QString table_name, const QModelIndexList& indexes) const
{
    QSqlQuery column = this->get_sql_columnParams(table_name);
    QString key = this->get_pri(table_name);
    int key_index;
    for (key_index = 0; column.next(); key_index++) {
        qDebug() << column.value(0).toString();
        if (column.value(0).toString() == key) break;
    }

    QString pri_column_value;
    for(int i = 0; i< indexes.count(); i++)
    {
        QModelIndex index = indexes.at(i);
        if (index.column() == key_index) {
            pri_column_value = index.data().toString();
            break;
        }
    }

    QSqlQuery delete_row_query("DELETE FROM " + table_name + " WHERE (" + key + " = '" + pri_column_value + "')");

    this->check_queryError(column);
    this->check_queryError(delete_row_query);

    //Restore auro increment fields values
    QVector<QVector<QString>> table_params = this->get_replacment_columnParams(table_name);
    for (int i = 0; i < table_params.at(0).size(); i++) {
        if (table_params.at(3).at(i) == "auto_increment") {
            this->restore_autoInc_order(table_name, table_params.at(0).at(i));
        }
    }
}


/*Update row values with sql query.*/
void ConnectedLevel::update_row(const QString table_name
                       , const QVector<QString> &new_data
                       , const QModelIndexList& indexes) const
{
    QSqlQuery column_params = this->get_sql_columnParams(table_name);

    QString key;
    int key_number;
    for (key_number = 0; column_params.next(); key_number++) {

        if (column_params.value(3).toString() == "PRI") {
            key = column_params.value(0).toString();
            break;
        }
    }

    QString query_str = "UPDATE " + table_name + " SET";


    column_params = this->get_sql_columnParams(table_name);
    for (int i = 0; column_params.next(); i++) {
        if (column_params.value(5) != "auto_increment")
            query_str += " " + column_params.value(0).toString() + "='" + new_data[i] + "',";  //
    }
    query_str.erase(query_str.end() - 1, query_str.end());

    query_str += " WHERE (" + key + "='" + indexes.at(key_number).data().toString() + "')";

    QSqlQuery update_row_query(query_str);

    this->check_queryError(column_params);
    this->check_queryError(update_row_query);
}


/*Check table for contains replacment columns.*/
bool ConnectedLevel::contains_infoTable(const QString &table_name) const
{
    QVector<QVector<QString>> info_tables_params = this->get_infoTables_params();
    if (info_tables_params.at(0).contains(table_name)) return true;

    QSqlQuery column_params = this->get_sql_columnParams(table_name);
    while (column_params.next()) {
        if (info_tables_params.at(1).contains(column_params.value(0).toString())) {
            this->check_queryError(column_params);
            return true;
        }
    }

    this->check_queryError(column_params);
    return false;
}


/*Return string query of getting all table values.*/
QString ConnectedLevel::get_allData_query_str(const QString &table_name) const
{
    QString query_str("SELECT ");

    QStringList info_tables;
    QStringList id_columns;
    QStringList replacment_columns;
    for (const QString &table : this->get_tables()) {
        if (table.contains("справка")) {
            info_tables.append(table);
            QSqlQuery column_params = this->get_sql_columnParams(table);
            column_params.next();
            id_columns.append(column_params.value(0).toString());
            column_params.next();
            replacment_columns.append(column_params.value(0).toString());
        }
    }

    QSqlQuery main_column_params = this->get_sql_columnParams(table_name);
    bool empty_info = true;
    while(main_column_params.next()) {
        if (id_columns.contains(main_column_params.value(0).toString()) && !table_name.contains("справка")) empty_info = false;
    }

    if (!empty_info) {
        QSqlQuery column_params = this->get_sql_columnParams(table_name);
        for (int i = 0; column_params.next(); ) {
            if (!id_columns.contains(column_params.value(0).toString()))
                query_str.append(column_params.value(0).toString() + ", ");
            else  {
                query_str.append(info_tables.at(i) + "." + replacment_columns.at(i) + ", ");
                i++;
            }
        }
        query_str.erase(query_str.end() - 2);

        query_str.append(" from " + table_name);
        for (int i = 0; i < info_tables.count(); i++) {
            query_str.append(" left join " + info_tables.at(i));

            query_str.append(" on " + table_name + "." + id_columns.at(i) + " = " + info_tables.at(i) + "." + id_columns.at(i));
        }

        this->check_queryError(column_params);
    }
    else query_str.append("* from " + table_name + ";");

    this->check_queryError(main_column_params);
    return query_str;
}


/*Return column name with pri key.*/
QString ConnectedLevel::get_pri(const QString &table_name) const
{
    QSqlQuery column_info = this->get_sql_columnParams(table_name);

    while (column_info.next()) {
        if (column_info.value(3).toString() == "PRI") {
            QString rezult = column_info.value(0).toString();
            this->check_queryError(column_info);
            return rezult;
        }
    }

    this->check_queryError(column_info);
    return "";
}


/*Return connection level name.*/
QString ConnectedLevel::get_level() const
{
    return "Connected";
}


/*Add row to info table.*/
QVector<QString> ConnectedLevel::add_row_infoTable(const QString &table_name, QVector<QString> row_data) const
{
    QVector<QString> info_tables;
    for (const QString &table : this->get_tables()) {
        if (table.contains("справка")) info_tables.push_back(table);
    }

    QVector<QVector<QString>> columns_params = this->get_replacment_columnParams(table_name);
    for (int i = 0; i < columns_params.at(0).size(); i++) {
        if (columns_params.at(1).at(i) == "MUL") {
            for (const QString &info_table : info_tables) {
                QVector<QString> info_columns_params = this->get_replacment_columnParams(info_table).at(0);

                if (info_columns_params.at(1) == columns_params.at(0).at(i)) {
                    QSqlQuery select_id("SELECT " + info_columns_params.at(0) + " FROM " + info_table
                                           + " WHERE " + info_columns_params.at(1) + " = '" + row_data.at(i) + "';");

                    select_id.next();
                    row_data[i] = select_id.value(0).toString();

                    this->check_queryError(select_id);
                }
            }
        }
    }

    return row_data;
}


/*Add row to table.*/
void ConnectedLevel::add_row(const QString &table_name, const QVector<QString> &row_data) const
{
    QVector<QString> current_data = this->add_row_infoTable(table_name, row_data);

    QString query = "INSERT INTO " + table_name;

    query += " values(";

    for (int i = 0; i < current_data.size(); i++) {
        query += "'" + current_data.at(i) + "', ";

    }

    query.erase(query.end() - 2, query.end());

    QSqlQuery add_row_query(query + ");");

    this->check_queryError(add_row_query);
}
