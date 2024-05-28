#ifndef PRINTER_H
#define PRINTER_H


#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#include <QPainter>
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

#endif // PRINTER_H
