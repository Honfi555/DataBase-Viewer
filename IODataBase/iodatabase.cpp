#include "iodatabase.h"

#include <QFile>
#include <QTextStream>
#include <QSqlTableModel>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QMessageBox>
#include <QSqlRecord>


/*Create report from table (.csv/.txt).*/
bool IODataBase::write_table(const QString &table_name, QSqlRelationalTableModel *values, QFileType file_type) const
{
    QString textData(this->collectData_to_string(values));

    // Save to file
    QString file_name = this->save_path_ + "\\" + table_name;

    switch(file_type) {
    case QFileType::csv:
        file_name += ".csv";
        break;
    case QFileType::txt:
        file_name +=".txt";
        break;
    }

    QFile csvFile(file_name);
    if(csvFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {

        QTextStream out(&csvFile);
        out << textData;

        csvFile.close();

        return true;
    }

    this->print_error(table_name);
    return false;
}

// bool IODataBase::read_table(const QString &table_name, QFileType file_type)
// {
//     QString file_name = this->save_path_ + "\\" + table_name;

//     switch(file_type) {
//     case QFileType::csv:
//         file_name += ".csv";
//         break;
//     case QFileType::txt:
//         file_name +=".txt";
//         break;
//     }

//     QFile file(file_name);
//     if (file.open(QIODevice::ReadOnly)) {

//         QTextStream in(&file);                 // read to text stream

//         QStandardItemModel *model = new QStandardItemModel;

//         for (int lineindex = 0; !in.atEnd(); lineindex++) {

//             // read one line from textstream(separated by "\n")
//             QString fileLine = in.readLine();

//             // parse the read line into separate pieces(tokens) with "," as the delimiter
//             QStringList lineToken = fileLine.split(",", Qt::SkipEmptyParts);
//             qDebug() << lineToken;

//             // load parsed data to model accordingly
//             for (int j = 0; j < lineToken.size(); j++) {
//                 QString value = lineToken.at(j);
//                 QStandardItem *item = new QStandardItem(value);
//                 model->setItem(lineindex, j, item);
//             }
//         }

//         file.close();
//         this->models_.insert(table_name, model);

//         return true;
//     }

//     this->print_error(table_name);
//     return false;
// }

// QMap<QString, QStandardItemModel *> IODataBase::get_models() const
// {
//     return this->models_;
// }

// QStandardItemModel *IODataBase::get_model(const QString &table_name) const
// {
//     return this->models_[table_name];
// }


/*Create json file with table params (aka sql query - show columns from [table name]).*/
bool IODataBase::write_table_params(const QString &table_name, const QVector<QVector<QString> > &table_params) const
{
    QJsonObject json_object;
    json_object.insert("table_name", table_name);
    QJsonObject params;
    params.insert("Feild", QJsonArray::fromStringList(table_params.at(0)));
    params.insert("Key", QJsonArray::fromStringList(table_params.at(1)));
    params.insert("Type", QJsonArray::fromStringList(table_params.at(2)));
    params.insert("Extra", QJsonArray::fromStringList(table_params.at(3)));

    json_object.insert("params", params);

    QJsonDocument json_document(json_object);
    QFile file(this->save_path_ + "\\" + table_name + ".json");
    if (file.open(QIODevice::WriteOnly)) {
        file.write(json_document.toJson());
        file.close();

        return true;
    }

    this->print_error(table_name);
    return false;
}


/*Read json file with table params.*/
bool IODataBase::read_table_params(const QString &table_name)
{
    QFile file(this->save_path_ + "\\" + table_name + ".json");
    if (file.open(QIODevice::ReadOnly)) {
        QByteArray data = file.readAll();
        QJsonDocument jsonDocument = QJsonDocument::fromJson(data);
        QJsonObject json_object = jsonDocument.object();

        return true;
    }

    this->print_error(table_name);
    return false;
}


/*Cast QSqlRelationalTableModel* to QString.*/
QString IODataBase::collectData_to_string(QSqlRelationalTableModel *values) const
{
    QString textData;

    for (int i = 0; i < values->rowCount(); i++) {
        for (int j = 0; j < values->columnCount(); j++) {

            textData += values->record(i).value(j).toString();
            textData += ", ";      // for .csv file format
        }
        textData.erase(textData.end() - 2, textData.end());
        textData += "\n";             // (for new line segmentation)
    }

    return textData;
}


/*Print file error.*/
void IODataBase::print_error(const QString &table_name) const
{
    QMessageBox::warning(new QWidget(), "File error", "File (for " + table_name + ") not open.");
}
