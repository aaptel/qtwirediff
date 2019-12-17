// SPDX-License-Identifier: GPL-3.0
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "traceview.h"
#include "diff.h"
#include "difftreeview.h"
#include "diffview.h"
#include "session.h"

class Trace;
class QTableView;
class DiffView;
class DiffTreeView;

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
    DiffTreeView* getDiffTreeView() { return diffview->getDiffTreeView(); }
    void updateSession();
    void reloadSessionMenu();

public slots:
    void onPacketChange(TraceView* tv);

private slots:
    void on_actionAbout_triggered();
    void onSessionItemClick(bool checked);

private:
    Ui::MainWindow *ui;
    TraceView *traceLeft;
    TraceView *traceRight;
    DiffView *diffview;
    QVector<DiffNode> diff;
    SessionList sesList;
};

#endif // MAINWINDOW_H
