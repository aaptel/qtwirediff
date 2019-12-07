#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "diff.h"

class Trace;
class QTableView;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void setTraceA(Trace* t);
    void setTraceB(Trace* t);
    void setFlatDiff(QVector<DiffNode>* diff);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void setTrace(Trace* t, QTableView *tv);
};

#endif // MAINWINDOW_H
