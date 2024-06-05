#include "reportdialog.h"
#include "ui_reportdialog.h"

#include "stylesheet.h"


ReportDialog::ReportDialog(QWidget *parent)
    : QWidget(parent)
    , ui_(new Ui::ReportDialog)
{
    ui_->setupUi(this);

    this->setWindowTitle("Repots templates");
    this->setWindowIcon(QIcon(":/settings/icons_printer.png"));
    this->apply_styleSheet();
}


ReportDialog::~ReportDialog()
{
    delete ui_;
}


void ReportDialog::update_widget(const QVector<ReportTemplate> &reports)
{
    reports_.clear();
    ui_->listWidget_reports->clear();

    reports_.append(ReportTemplate("Standart report", true));
    reports_ += reports;

    for (const auto &report : reports_) ui_->listWidget_reports->addItem(report.name());
}


void ReportDialog::on_pushButton_enter_clicked()
{
    for (const auto &report : reports_) {
        if (report.name() != ui_->listWidget_reports->currentItem()->text()) continue;

        emit this->reportEntered(report);
        this->close();
        break;
    }
}


void ReportDialog::on_pushButton_cancel_clicked()
{
    this->close();
}


void ReportDialog::apply_styleSheet()
{
    StyleSheet style_sheet;

    this->setStyleSheet(style_sheet.window_styleSheet());

    ui_->label_tittle->setStyleSheet(style_sheet.label_styleSheet(25));

    ui_->listWidget_reports->setStyleSheet(style_sheet.listWidget_styleSheet());

    ui_->pushButton_enter->setStyleSheet(style_sheet.pushButton_styleSheet());
    ui_->pushButton_cancel->setStyleSheet(style_sheet.pushButton_styleSheet());
}

