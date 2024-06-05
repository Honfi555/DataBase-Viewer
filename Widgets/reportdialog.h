#ifndef REPORTDIALOG_H
#define REPORTDIALOG_H

#include <QWidget>

#include "IODataBase/repotstemplates.h"


namespace Ui {
class ReportDialog;
}

class ReportDialog : public QWidget
{
    Q_OBJECT

public:
    explicit ReportDialog(QWidget *parent = nullptr);
    ~ReportDialog();

    void update_widget(const QVector<ReportTemplate> &reports);

signals:
    void reportEntered(ReportTemplate);

private slots:
    void on_pushButton_enter_clicked();

    void on_pushButton_cancel_clicked();

private:
    void apply_styleSheet();

    Ui::ReportDialog *ui_;
    QVector<ReportTemplate> reports_;
};

#endif // REPORTDIALOG_H
