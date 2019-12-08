#ifndef TRACEVIEW_H
#define TRACEVIEW_H


#include <QFrame>
#include "trace.h"

namespace Ui {
class TraceView;
}

class TraceView : public QFrame
{
    Q_OBJECT

public:
    explicit TraceView(QWidget *parent = 0);
    ~TraceView();
    Trace::Node* getPacket(int no = -1);
    Trace* getTrace() { return trace_; }
    void moveSelection(int dir);

signals:
    void packetChanged(TraceView *tv);

private:
    int getRow();

    Ui::TraceView *ui;
    Trace* trace_;
    Trace::Node* lastNode;

private slots:
    void onSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
    void onOpen(bool checked);
};

#endif // TRACEVIEW_H
