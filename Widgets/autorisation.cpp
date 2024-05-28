#include "autorisation.h"
#include "ui_autorisation.h"

#include "stylesheet.h"


Autorisation::Autorisation(QWidget *parent)
    : QWidget(parent)
    , ui_(new Ui::Autorisation)
{
    ui_->setupUi(this);

    this->apply_styleSheet();
    this->setWindowTitle("Autorisation");
    this->setWindowIcon(QIcon(":/settings/icons_switchUser.png"));
    this->ui_->lineEdit_password->setEchoMode(QLineEdit::Password);
}


Autorisation::~Autorisation()
{
    delete ui_;
}


/*Rerurn pair of credential (login, password).*/
QPair<QString, QString> Autorisation::get_credential() const
{
    return QPair<QString, QString>(ui_->lineEdit_login->text(), ui_->lineEdit_password->text());
}


/*Clear widget exactly login and password line edits.*/
void Autorisation::clear()
{
    this->ui_->lineEdit_login->clear();
    this->ui_->lineEdit_password->clear();
}


/*Trigger a signal that the credential entered and close autorisation window.*/
void Autorisation::on_pushButton_ok_clicked()
{
    emit this->credential_entered();
    this->close();
}


/*Close autorisation window.*/
void Autorisation::on_pushButton_cancel_clicked()
{
    this->close();
}


/*Set css styles to widgets.*/
void Autorisation::apply_styleSheet()
{
    StyleSheet style_sheet;

    this->setStyleSheet(style_sheet.window_styleSheet());

    ui_->label_login->setStyleSheet(style_sheet.label_styleSheet());
    ui_->label_password->setStyleSheet(style_sheet.label_styleSheet());
    ui_->label_autorisation->setStyleSheet(style_sheet.label_styleSheet(30));

    ui_->lineEdit_login->setStyleSheet(style_sheet.lineEdit_styleSheet());
    ui_->lineEdit_password->setStyleSheet(style_sheet.lineEdit_styleSheet());

    ui_->pushButton_ok->setStyleSheet(style_sheet.pushButton_styleSheet());
    ui_->pushButton_cancel->setStyleSheet(style_sheet.pushButton_styleSheet());
}

