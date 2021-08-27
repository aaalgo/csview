#include <iostream>
#include <vector>
#include <QApplication>
#include "csvlint.h"
#include "ui_csview.h"

int main(int argc, char **argv)
{
 std::cin >> std::noskipws;
 std::istream_iterator<char> it(std::cin);
 std::istream_iterator<char> end;
 std::string results(it, end);
 csvlint::Text text(&results);
 csvlint::Format fmt;
 fmt.train(text);
 int rows = text.size() - fmt.start_line;
 int cols = fmt.fields.size();

 QApplication app (argc, argv);
 QMainWindow window;
 Ui_MainWindow ui;
 ui.setupUi(&window);

 ui.tableWidget->setColumnCount(cols);
 ui.tableWidget->setRowCount(rows);
 ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

 if (fmt.has_header) {
     for (int col = 0; col < fmt.fields.size(); ++col) {
         QString qs = QString::fromStdString(fmt.fields[col].name);
         QTableWidgetItem *item = new QTableWidgetItem(qs);
         ui.tableWidget->setHorizontalHeaderItem(col, item);
     }
 }

 for (int row = 0; row < rows; ++row) {
     auto line = text[fmt.start_line + row];
     std::vector<csvlint::crange> out;
     fmt.parse(line, &out);
     for (int col = 0; col < out.size(); ++col) {
         auto &txt = out[col];
         QString qs = QString::fromUtf8(txt.begin(), txt.end()-txt.begin());
         QTableWidgetItem *item = new QTableWidgetItem(qs);
         ui.tableWidget->setItem(row, col, item);
     }
 }


 window.show();
 return app.exec();
}
