#ifndef PRINTER_H
#define PRINTER_H


#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#include <QPainter>
#include <QPdfWriter>
#include <QSqlRelationalTableModel>
#include <QModelIndex>
#include <QSqlRecord>


void print_table(QSqlRelationalTableModel *model)
{
    QPrinter printer;
    QPrintDialog dialog(&printer);
    dialog.setWindowIcon(QIcon(":/settings/icons_printer.png"));

    if (dialog.exec() == QDialog::Accepted) {
        QPainter painter(&printer);
        int rows = model->rowCount();
        int columns = model->columnCount();
        int margin = 30;  // Отступ
        int x = margin, y = margin;  // Начальная позиция для рисования

        // Печать заголовков
        QString header;
        for (int column = 0; column < columns; ++column) {
            QString header_text = model->record().fieldName(column);
            header += header_text + " ";
            painter.drawText(x, y, 200, 50, Qt::AlignLeft, header_text);
            x += 151;  // Смещение вправо для следующей колонки
        }
        qDebug() << header;
        y += 30;  // Смещение вниз для строк таблицы

        // Печать данных таблицы
        for (int row = 0; row < rows; ++row) {
            x = margin;  // Сброс x на начальное значение
            QString field;
            for (int col = 0; col < columns; ++col) {
                QString field_text = model->data(model->index(row, col)).toString();
                field += field_text + " ";
                painter.drawText(x, y, 200, 50, Qt::AlignLeft, field_text);
                x += 151;  // Смещение вправо для следующей колонки
            }
            qDebug() << field;
            y += 30;  // Смещение вниз для следующей строки
        }

        painter.end();  // Завершаем печать
    }
}


void print_report(const QString &name
                  , const QString &top
                  , const QString &bottom
                  , const QString &signature
                  , QSqlRelationalTableModel *model)
{
    QPrinter printer;
    QPrintDialog dialog(&printer);
    dialog.setWindowIcon(QIcon(":/settings/icons_printer.png"));

    if (dialog.exec() != QDialog::Accepted) return;

    QPainter painter(&printer);
    int margin = 30;  // Отступ
    int x = margin + 20, y = margin;  // Начальная позиция для рисования
    painter.drawImage(20, 20, QImage(":/windowIcons/icons_window.png").scaled(QSize(48, 48)));

    painter.drawText(0, 20, 1000, 50,  Qt::AlignCenter | Qt::AlignBottom, top);
    painter.drawText(x, 1050, 1000, 100, Qt::AlignLeft, signature);
    painter.drawText(0, 1250, 1000, 30,  Qt::AlignCenter | Qt::AlignBottom, bottom);
    y += 300;

    painter.drawText(0, y, 1000, 50, Qt::AlignCenter, "Отчёт " + name);
    y += 60;

    int rows = model->rowCount();
    int columns = model->columnCount();

    // Печать заголовков
    QString header;
    for (int column = 0; column < columns; ++column) {
        QString header_text = model->record().fieldName(column);
        header += header_text + " ";
        painter.drawText(x, y, 200, 50, Qt::AlignLeft, header_text);
        x += 151;  // Смещение вправо для следующей колонки
    }
    qDebug() << header;
    y += 30;  // Смещение вниз для строк таблицы

    // Печать данных таблицы
    for (int row = 0; row < rows; ++row) {
        x = margin + 20;  // Сброс x на начальное значение
        QString field;
        for (int col = 0; col < columns; ++col) {
            QString field_text = model->data(model->index(row, col)).toString();
            field += field_text + " ";
            painter.drawText(x, y, 200, 50, Qt::AlignLeft, field_text);
            x += 151;  // Смещение вправо для следующей колонки
        }
        qDebug() << field;
        y += 30;  // Смещение вниз для следующей строки
    }

    painter.end();  // Завершаем печать
}

#endif // PRINTER_H
