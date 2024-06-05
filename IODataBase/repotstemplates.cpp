#include "DataBase/repotstemplates.h"

#include <QFile>
#include <QDir>
#include <QSqlQuery>
#include <QJsonObject>


ReportTemplate::ReportTemplate(const QString &template_path)
{
    this->setTemplate(template_path);
}


ReportTemplate::ReportTemplate(const QString &name, bool standart)
    : standart_(standart)
{
    setName(name);
}


bool ReportTemplate::isStandart()
{
    return standart_;
}


bool ReportTemplate::setTemplate(const QString &template_path)
{
    standart_ = false;

    QFile file(template_path);
    if (file.open(QIODevice::ReadOnly)) {
        QByteArray data = file.readAll();
        this->report_template_ = QJsonDocument::fromJson(data);

        return true;
    }

    return false;
}


void ReportTemplate::setName(const QString &name)
{
    QJsonObject template_object = this->report_template_.object();
    template_object.insert("name", name);
    this->report_template_.setObject(template_object);
    qDebug() << report_template_["name"];
}


QSqlRelationalTableModel* ReportTemplate::model(const QSqlDatabase &database) const
{
    QSqlRelationalTableModel *query_model = new QSqlRelationalTableModel();
    query_model->setQuery(this->report_template_["query"].toString(), database);

    return query_model;
}


QString ReportTemplate::name() const
{
    return this->report_template_["name"].toString();
}


QString ReportTemplate::top() const
{
    return this->report_template_["top"].toString();
}


QString ReportTemplate::bottom() const
{
    return this->report_template_["bottom"].toString();
}


QString ReportTemplate::signature() const
{
    return this->report_template_["signature"].toString();
}


QVector<QString> reportsTemplatesPaths(const QString path)
{
    QVector<QString> rezult;
    QDir templates(path);

    for (const auto &file_info : templates.entryInfoList(QDir::Files)) {
        rezult.push_back(QString::fromStdString(file_info.filesystemAbsoluteFilePath().string()));
    }

    return rezult;
}
