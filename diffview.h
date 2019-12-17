#ifndef DIFFVIEW_H
#define DIFFVIEW_H

#include <QFrame>
#include "difftreeview.h"
#include "diff.h"

class DiffView : public QFrame
{
    Q_OBJECT
public:
    DiffView(QWidget* w);
    DiffTreeView* getDiffTreeView() { return dtv_; }
    void updateDiff() { dtv_->updateDiff(); }
    QString getFilter();
    void setFilter(QString f);

signals:
    void filterSubmitted(QString filt);

private:
    DiffTreeView* dtv_;
    QLineEdit* txtFilter_;

private slots:
    void onFilterSubmitted();

};

#endif // DIFFVIEW_H
