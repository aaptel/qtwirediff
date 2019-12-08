#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "traceview.h"
#include "diff.h"
#include "diffview.h"

class Trace;
class QTableView;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    static MainWindow * instance;
    explicit MainWindow(QWidget *parent = 0);
    QVector<DiffNode>* getDiff() { return &diff; }
    void moveSelection(int dir);
    ~MainWindow();

public slots:
    void onPacketChange(TraceView* tv);

private:
    Ui::MainWindow *ui;
    TraceView *traceLeft;
    TraceView *traceRight;
    DiffView *diffview;
    QVector<DiffNode> diff;
};

#endif // MAINWINDOW_H
