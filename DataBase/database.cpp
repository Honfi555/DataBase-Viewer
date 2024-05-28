#include "DataBase/database.h"

#include <QMessageBox>
#include <QSqlError>
#include <QSqlRecord>

#include "stylesheet.h"


DataBase::DataBase(const QString &user_name, const QString &user_password, const QString &sql_driver, const QString &ip)
{
#ifdef DEGUB_SQL_CONNECT
    qDebug() << QSqlDatabase::drivers();
#endif

    this->db_ = QSqlDatabase::addDatabase(sql_driver);
    this->db_.setUserName(user_name);
    this->db_.setPassword(user_password);
    this->db_.setHostName(ip);
    this->db_.open();

    if (this->db_.lastError().type() == QSqlError::ConnectionError) {
        QString error_text = "Db not open!";
#ifdef DEGUB_SQL_CONNECT
        error_text += "\n Sql error - " + this->db_.lastError().text();
#endif
        this->print_Error(error_text);
    }
}


/*Return all tables from current DB.*/
QVector<QString> DataBase::get_tables() const
{
    QVector<QString> tables;

    QSqlQuery query("SHOW TABLES");
    while(query.next()) {
        tables.push_back(query.value(0).toString());
    }

    this->check_queryError(query);
    return tables;
}


/*Return all DBs from connection.*/
QVector<QString> DataBase::get_dataBases() const
{
    QVector<QString> databases;

    QSqlQuery query("SHOW DATABASES");
    while(query.next()) {
        databases.push_back(query.value(0).toString());
    }

    this->check_queryError(query);
    return databases;
}


/*Return column params {column name, key, field type, extra}.*/
QVector<QVector<QString> > DataBase::get_replacment_columnParams(const QString &table_name) const
{
    QStringList id_columns;
    QStringList replacment_columns;

    for (const QString &table : this->get_tables()) {
        if (table.contains("справка")) {
            QSqlQuery column_params("SHOW COLUMNS FROM " + table);
            column_params.next();
            id_columns.append(column_params.value(0).toString());
            column_params.next();
            replacment_columns.append(column_params.value(0).toString());

            this->check_queryError(column_params);
        }
    }

    QSqlQuery column_params("SHOW COLUMNS FROM " + table_name);

    QVector<QString> column_name;
    QVector<QString> column_keys;
    QVector<QString> column_types;
    QVector<QString> column_extra_info;
    for (int i = 0; column_params.next(); ) {
        if (id_columns.contains(column_params.value(0).toString()) && !table_name.contains("справка")) {
            column_name.push_back(replacment_columns.at(i));
            column_keys.push_back(column_params.value(3).toString());
            column_types.push_back("varchar(45)");
            column_extra_info.push_back(column_params.value(5).toString());
            i++;
        }
        else {
            column_name.push_back(column_params.value(0).toString());
            column_keys.push_back(column_params.value(3).toString());
            column_types.push_back(column_params.value(1).toString());
            column_extra_info.push_back(column_params.value(5).toString());
        }
    }

    QVector<QVector<QString>> rezult;
    rezult.append(column_name);
    rezult.append(column_keys);
    rezult.append(column_types);
    rezult.append(column_extra_info);

    this->check_queryError(column_params);
    return rezult;
}


/*Return map with replaced column name and his replaced field type {replaced column name, replaced field type}.*/
QMap<QString, QString> DataBase::get_replacment_columnTypes(const QString &table_name) const
{
    QVector<QVector<QString>> column_params = this->get_replacment_columnParams(table_name);

    QMap<QString, QString> rezult;
    for (int i = 0; i < column_params.at(0).size(); i++) {
        rezult.insert(column_params.at(0).at(i), column_params.at(2).at(i));
    }

    return rezult;
}


/*Return map with column name and his values from table {column name, {vlues1, value2, ...}}.*/
QMap<QString, QVector<QString> > DataBase::get_dialogData() const
{
    QMap<QString, QVector<QString>> values_set;
    QVector<QString> tables(this->get_tables());

    for (const QString& table : tables) {
        QSqlQuery column_params = this->get_sql_columnParams(table);
        int i = 0;
        while(column_params.next()) {
            if (column_params.value(3).toString() == "PRI") {
                int j = i;
                if (table.contains("справка")) {
                    column_params.next();
                    j++;
                }
                values_set.insert(column_params.value(0).toString(), this->get_columnData(table, j));
            }
            i++;
        }
        this->check_queryError(column_params);
    }

    return values_set;
}


/*Set DB by his name.*/
void DataBase::set_database(const QString &db_name)
{
    if (db_name.isEmpty()) return;
    QSqlQuery query("USE " + db_name);
    this->check_queryError(query);
}


/*Return database*/
QSqlDatabase DataBase::get_database() const
{
    return this->db_;
}



/*Remuve database coonection.*/
void DataBase::remuve_connection()
{
    this->db_.removeDatabase(this->db_.connectionName());
    this->db_.close();
}


/*Return login and password.*/
QPair<QString, QString> DataBase::get_credential() const
{
    return QPair<QString, QString>(this->db_.userName(), this->db_.password());
}


/*Check query executing for errors and show them.*/
void DataBase::check_queryError(QSqlQuery &query) const
{
    if (query.lastError().type() != QSqlError::NoError)
    {
        QString error_text = "Query not completed!";
#ifdef DEBUG_SQL_QUERY
        error_text += "\n Sql error - " + query.lastError().text();

#endif
        if (query.lastError().text().contains("command denied to user")) error_text = "Sorry but you don't have enough rights.";

        this->print_Error(error_text + " Query - " + query.lastQuery());
    }

#ifdef DEBUG_SQL_QUERY
    qDebug() << "Query - " << query.lastQuery();
#endif

    query.finish();
}


void DataBase::print_Error(const QString &error) const
{
    StyleSheet style_sheet;
    QPixmap icon(":/windowIcons/icons_error.png");
    QMessageBox message_box;

    message_box.setWindowIcon(QIcon(":/windowIcons/icons_error.png"));
    message_box.setIconPixmap(icon.scaled(QSize(48, 48)));
    message_box.setWindowTitle("Error");
    message_box.setText(error);
    message_box.setStandardButtons(QMessageBox::Ok);
    message_box.setStyleSheet(style_sheet.messageBox_styleSheet());
    message_box.exec();
}


/*Restore auto increment fields values.*/
void DataBase::restore_autoInc_order(const QString &table_name, const QString &autoInc_column) const
{
    QSqlQuery query("SET @num = 0; "
                    "UPDATE " + table_name + " "
                                   "SET " + autoInc_column + " = @num := @num + 1 "
                                       "WHERE " + autoInc_column + " > 0 "
                                       "ORDER BY " + autoInc_column +"; "
                                       "ALTER TABLE " + table_name + " auto_increment = 1;");

    this->check_queryError(query);
}


/*Convert QSqlQuery to QVector<QVector<QString>>.*/
QVector<QVector<QString>> DataBase::from_QSqlQuery(QSqlQuery &query) const
{
    QVector<QVector<QString>> data;
    while(query.next()) {
        QVector<QString> row;
        for (int i = 0; i < query.record().count(); i++) {
            row.push_back(query.value(i).toString());
        }
        data.push_back(row);
    }

    this->check_queryError(query);
    return data;
}


/*Return rezult of sql query with columns params {column name, field type, is null, key, default value, exrta}.*/
QSqlQuery DataBase::get_sql_columnParams(const QString &table_name) const
{
    return QSqlQuery("SHOW COLUMNS FROM " + table_name);
}


/*Return replacment values from one column.*/
QVector<QString> DataBase::get_columnData(const QString &table_name, const int column_number) const
{
    QVector<QString> column_values;
    QSqlQuery query = this->get_allData_query(table_name);

    for (const auto& i : this->from_QSqlQuery(query)) {
        column_values.push_back(i.at(column_number));
    }

    return column_values;
}


/*Return info tabeles params {table names, id fields names, replacment fields}.*/
QVector<QVector<QString> > DataBase::get_infoTables_params() const
{
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

            this->check_queryError(column_params);
        }
    }
    QVector<QVector<QString>> info_tables_params;
    info_tables_params.append(info_tables);
    info_tables_params.append(id_columns);
    info_tables_params.append(replacment_columns);

    return info_tables_params;
}
