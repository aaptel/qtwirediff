#include "diffview.h"
#include <QVBoxLayout>
#include <QLineEdit>

DiffView::DiffView(QWidget* w) : QFrame(w)
{
    this->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    this->setLineWidth(1);

    dtv_ = new DiffTreeView(this);
    txtFilter_ = new QLineEdit(this);
    txtFilter_->setPlaceholderText("comma-separated list of id to filter (prefix with ! to filter out)");

    auto layout = new QVBoxLayout();
    layout->addWidget(txtFilter_);
    layout->addWidget(dtv_);
    setLayout(layout);
    connect(txtFilter_, &QLineEdit::returnPressed, this, &DiffView::onFilterSubmitted);
}

QString DiffView::getFilter()
{
    return txtFilter_->text();
}

void DiffView::setFilter(QString f)
{
    txtFilter_->setText(f);
}

void DiffView::onFilterSubmitted()
{
    emit filterSubmitted(getFilter());
}
