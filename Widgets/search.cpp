#include "search.h"
#include "ui_search.h"

#include <QMap>

#include "stylesheet.h"


Search::Search(QWidget *parent, const QSearchType &search_type, const QMap<QString, QString> &columns_types)
    : QWidget(parent)
    , ui_(new Ui::Search)
    , search_type_(search_type)
    , columns_types_(columns_types)
{
    ui_->setupUi(this);
    this->setWindowTitle("Окно поиска");
    this->setWindowIcon(QIcon(":/toolIcons/icons_search.png"));

    this->apply_styleSheet();
}

Search::~Search()
{
    delete ui_;
}


/*Set widget search type Simple/Complicated.*/
void Search::set_SearchType(const QSearchType &searchType)
{
    this->search_type_ = searchType;
}


/*Return widget search type Simple/Complicated.*/
QSearchType Search::get_SearchType() const
{
    return this->search_type_;
}


/*Fillng first-stage widget.*/
void Search::insert_data(const QString &table_name, const QVector<QString> &columns_names, const QMap<QString, QString> &columns_types)
{
    this->clear_widget();

    this->table_name_ = table_name;
    this->ui_->label_table->setText("Поиск по таблице - " + table_name);
    this->ui_->comboBox_columns->insertItems(0, columns_names);

    this->columns_types_ = columns_types;
    this->update_widget(columns_names.at(0), columns_types);
}


/*Update widget.*/
void Search::update_widget(const QString &column_name, const QMap<QString, QString> &columns_types)
{
    this->ui_->comboBox_condition1->clear();
    this->ui_->comboBox_condition2->clear();

    this->ui_->lineEdit_condition1->clear();
    this->ui_->lineEdit_condition2->clear();
    this->ui_->lineEdit_condition1->setValidator(this->get_validator(columns_types[column_name]));
    this->ui_->lineEdit_condition2->setValidator(this->get_validator(columns_types[column_name]));

    if (columns_types[this->ui_->comboBox_columns->currentText()].contains("varchar")) {
        this->ui_->comboBox_condition1->insertItems(0, {"=", "!="});

        this->search_type_ = QSearchType::Simple;
    }
    else {
        this->ui_->comboBox_condition1->insertItems(0, {"<", ">", "<=", ">=", "=", "!="});
        this->ui_->comboBox_condition2->insertItems(0, {"<", ">", "<=", ">=", "=", "!="});
    }

    if (this->search_type_ == QSearchType::Complicated) {
        this->ui_->comboBox_condition2->show();
        this->ui_->lineEdit_condition2->show();
    }
    else {
        this->ui_->comboBox_condition2->hide();
        this->ui_->lineEdit_condition2->hide();
    }
}


/*Return validator for control the input line edits data.*/
QRegularExpressionValidator *Search::get_validator(const QString &column_type) const
{
    QString number;
    QRegularExpression regex;

    if (column_type.contains("tinyint")) {
        regex = QRegularExpression("[0-1]{1}");
    }
    else if (column_type.contains("int")) {
        regex = QRegularExpression("\\d+");
    }
    else if (column_type.contains("date"))
        regex = QRegularExpression("\\d{4}[-][0-1]\\d[-][0-3]\\d");

    else if (column_type.contains("varchar")) {
        number = column_type.mid(column_type.indexOf('(') + 1, column_type.indexOf(')') - column_type.indexOf('(') - 1);
        regex = QRegularExpression(".{" + number + "}");
    }

    QRegularExpressionValidator* validator = new QRegularExpressionValidator(regex);

    return validator;
}


/*Return entered values and properties.*/
QMap<QString, QString> Search::get_searchCondition() const
{
    QMap<QString, QString> search_condition;

    search_condition.insert("table_name", this->table_name_);
    search_condition.insert("column_name", this->ui_->comboBox_columns->currentText());

    search_condition.insert("condition1_property", this->ui_->comboBox_condition1->currentText());
    search_condition.insert("condition1_value", this->ui_->lineEdit_condition1->text());

    search_condition.insert("condition2_property", this->ui_->comboBox_condition2->currentText());
    search_condition.insert("condition2_value", this->ui_->lineEdit_condition2->text());

    return search_condition;
}


/*Clear widget.*/
void Search::clear_widget()
{
    this->ui_->label_table->clear();
    this->ui_->comboBox_columns->clear();
    this->ui_->lineEdit_condition1->clear();
    this->ui_->lineEdit_condition2->clear();
    this->ui_->comboBox_condition1->clear();
    this->ui_->comboBox_condition2->clear();

    this->ui_->comboBox_condition2->hide();
    this->ui_->lineEdit_condition2->hide();
}


/*Emit sigbal of searching.*/
void Search::on_pushButton_search_clicked()
{
    emit this->searchEntered();
}


/*Switch search condition.*/
void Search::on_pushButton_switchCondition_clicked()
{
    this->search_type_ = (this->search_type_ == QSearchType::Complicated ||
                        this->columns_types_[this->ui_->comboBox_columns->currentText()] == "varchar(45)")
                        ? QSearchType::Simple : QSearchType::Complicated;

    this->update_widget(this->column_name_, this->columns_types_);
}


/*Close widget.*/
void Search::on_pushButton_cancel_clicked()
{
    this->close();
}


/*Update widget when combo box with columns is changed.*/
void Search::on_comboBox_columns_currentTextChanged(const QString &arg1)
{
    this->column_name_ = arg1;
    this->update_widget(arg1, this->columns_types_);
}


/*Set css styles to widgets.*/
void Search::apply_styleSheet()
{
    StyleSheet style_sheet;

    this->setStyleSheet(style_sheet.window_styleSheet());

    ui_->comboBox_columns->setStyleSheet(style_sheet.comboBox_styleSheet());
    ui_->comboBox_condition1->setStyleSheet(style_sheet.comboBox_styleSheet());
    ui_->comboBox_condition2->setStyleSheet(style_sheet.comboBox_styleSheet());

    ui_->lineEdit_condition1->setStyleSheet(style_sheet.lineEdit_styleSheet());
    ui_->lineEdit_condition2->setStyleSheet(style_sheet.lineEdit_styleSheet());

    ui_->label_x->setStyleSheet(style_sheet.label_styleSheet());
    ui_->label_columns->setStyleSheet(style_sheet.label_styleSheet());
    ui_->label_condition->setStyleSheet(style_sheet.label_styleSheet());
    ui_->label_table->setStyleSheet(style_sheet.label_styleSheet(15));

    ui_->pushButton_cancel->setStyleSheet(style_sheet.pushButton_styleSheet());
    ui_->pushButton_search->setStyleSheet(style_sheet.pushButton_styleSheet());
    ui_->pushButton_switchCondition->setStyleSheet(style_sheet.pushButton_styleSheet());
}

