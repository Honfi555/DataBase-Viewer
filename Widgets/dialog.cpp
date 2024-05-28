#include "dialog.h"
#include "ui_dialog.h"

#include <QPushButton>
#include <QFileDialog>
#include <QRegularExpressionValidator>

#include "stylesheet.h"


Dialog::Dialog(const QVector<QString> &columns_names
               , const QMap<QString, QVector<QString>> &column_values
               , QWidget *parent)
    : QDialog(parent)
    , ui_(new Ui::Dialog)
{
    ui_->setupUi(this);

    this->apply_styleSheet();

    if (!columns_names.empty())
        this->update_widget(columns_names, column_values, "", {""}, "", {});

    connect(ui_->pushButton_exit, SIGNAL(clicked()), this, SLOT(on_pushButton_exit_clicked()));
}


Dialog::~Dialog()
{
    delete ui_;
}


/*Return entered values from widget.*/
QVector<QString> Dialog::get_dialog_values() const
{
    QVector<QString> dialog_values;
    int box = 0, line = 0;

    for (int i : this->array_numbers_) {
        switch (i) {
        case 0:
            dialog_values.push_back("0");
            break;
        case 1:
            dialog_values.push_back(this->boxes_.at(box)->currentText());
            box++;
            break;
        case 2:
            dialog_values.push_back(this->lines_.at(line)->text());
            line++;
            break;
        }
    }

    return dialog_values;
}


/*Update widget with new params.*/
void Dialog::update_widget(const QVector<QString> &columns_names
                           , const QMap<QString, QVector<QString>> &columns_values
                           , const QString &priColumn
                           , const QVector<QString> &autoInc_columns
                           , const QString &table_name
                           , const QMap<QString, QString> &columns_types)
{
    this->clear_widget();
    StyleSheet style_sheet;

    for (int i = 0, box = 0, line = 0, label = 0, btn = 0; i < columns_names.size(); i++) {
        if (autoInc_columns.at(i) == "auto_increment") {
            array_numbers_.push_back(0);
            continue;
        }

        this->labels_.push_back(new QLabel(columns_names.at(i)));
        this->ui_->frame->layout()->addWidget(this->labels_.at(label));
        label++;

        if(columns_values.contains(columns_names.at(i))
                   && columns_names.at(i) != priColumn
                   && columns_values[columns_names[i]] != QVector<QString>()
                   && !table_name.contains("справка")) {
            this->boxes_.push_back(new QComboBox());
            this->array_numbers_.push_back(1);

            this->boxes_.at(box)->addItems(columns_values[columns_names[i]]);

            this->ui_->frame->layout()->addWidget(this->boxes_.at(box));
            this->boxes_.at(box)->setStyleSheet(style_sheet.comboBox_styleSheet());

            box++;
        } else {
            this->lines_.push_back(new QLineEdit());
            this->array_numbers_.push_back(2);
            this->lines_.at(line)->setValidator(this->get_validator(columns_types[columns_names.at(i)]));

            this->ui_->frame->layout()->addWidget(this->lines_.at(line));
            this->lines_.at(line)->setStyleSheet(style_sheet.lineEdit_styleSheet());

            line++;

            if (columns_names.at(i).contains("Фотография")) {
                this->btns_.push_back(new QPushButton("Browse photo"));
                array_numbers_.push_back(3);

                this->btns_.at(btn)->connect(this->btns_.at(btn), SIGNAL(clicked()), this, SLOT(on_pushButton_browse_clicked()));

                this->ui_->frame->layout()->addWidget(this->btns_.at(btn));
                this->btns_.at(btn)->setStyleSheet(style_sheet.pushButton_styleSheet());

                btn++;
            }
        }
    }
}


/*Set dialog widget type Add/Update.*/
void Dialog::set_dialogType(const QDialogType& dialog_type)
{
    this->dialog_type_ = dialog_type;

    switch (this->dialog_type_) {
    case QDialogType::Add:
        this->setWindowTitle("Add row");
        this->setWindowIcon(QIcon(":/toolIcons/icon_plus.png"));
        ui_->label->setText("Add row");
        break;
    case QDialogType::Update:
        this->setWindowTitle("Update row");
        this->setWindowIcon(QIcon(":/toolIcons/icons_edit.png"));
        ui_->label->setText("Upadte row");
        break;
    }
}


/*Clear widget from created filling.*/
void Dialog::clear_widget()
{
    for (auto &i : this->boxes_) {
        i->deleteLater();
    }
    this->boxes_.clear();

    for (auto &i : this->lines_) {
        i->deleteLater();
    }
    this->lines_.clear();

    for (auto &i : this->labels_) {
        i->deleteLater();
    }
    this->labels_.clear();

    for (auto &i : this->btns_) {
        i->deleteLater();
    }
    this->btns_.clear();

    this->array_numbers_.clear();
}


/*Set css styles to widgets.*/
void Dialog::apply_styleSheet()
{
    StyleSheet style_sheet;

    this->setStyleSheet(style_sheet.window_styleSheet());

    ui_->label->setStyleSheet(style_sheet.label_styleSheet(25));

    ui_->pushButton_enter->setStyleSheet(style_sheet.pushButton_styleSheet());
    ui_->pushButton_exit->setStyleSheet(style_sheet.pushButton_styleSheet());
}


/*Return validator by entered column type.*/
QRegularExpressionValidator *Dialog::get_validator(const QString &column_type) const
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


/*Close widget.*/
void Dialog::on_pushButton_exit_clicked()
{
    this->close();
}


/*Switch widget typr Add/Update.*/
void Dialog::on_pushButton_enter_clicked()
{
    if (this->dialog_type_ == QDialogType::Add) {
        emit this->dialogAddEntered();
    }
    else  if (this->dialog_type_ == QDialogType::Update) {
        emit this->dialogUpdateEntered();
    }
    this->close();
}


/*Browsing for select path to photo.*/
void Dialog::on_pushButton_browse_clicked()
{
    const QString FILE_PATH = QFileDialog::getOpenFileName(
        this,
        tr("Open File"),
            "C://",
            "Photo (*.png | *.jpg) ;; All Files (*.*)"
    );

    int sender_index = this->btns_.indexOf(QObject::sender());
    for (int i = 0, line = 0, btn = 0; i < this->array_numbers_.size(); i++) {
        if (this->array_numbers_.at(i) == 2) line++;
        else if (this->array_numbers_.at(i) == 3) {
            if (btn == sender_index) this->lines_.at(line - 1)->setText(FILE_PATH);

            btn++;
        }
    }
}

