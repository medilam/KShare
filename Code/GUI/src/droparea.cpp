#include "droparea.h"
#include <QStyleOption>
#include <QPainter>

DropArea::DropArea(QWidget *parent)
    : QWidget(parent)
{
    pixmap = new QLabel;
    sentence = new QLabel;
    pixmap->setMinimumHeight(120);
    pixmap->setStyleSheet("background-color:rgb(186, 189, 182);");
    sentence->setStyleSheet("QLabel{color: rgb(84,84,84);background-color:rgb(186, 189, 182);}");
    setAcceptDrops(true);
    clear();

    QVBoxLayout *dragAndDropLayout = new QVBoxLayout;
    dragAndDropLayout->addWidget(pixmap);
    dragAndDropLayout->addWidget(sentence);
    setLayout(dragAndDropLayout);
}

void DropArea::dragEnterEvent(QDragEnterEvent *event)
{
    pixmap->setPixmap(QPixmap());
    sentence->setText("");

    event->acceptProposedAction();
    emit changed();
}

void DropArea::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
}

void DropArea::dropEvent(QDropEvent *event)
{
    setStatus(FULL);
    const QMimeData *mimeData = event->mimeData();
    QPixmap pixmapLogo(":/icons/file");
    if (mimeData->hasText()) {
        pixmap->setPixmap(pixmapLogo);
        sentence->setText(mimeData->text());
        sentence->setTextFormat(Qt::PlainText);
    } else {
        sentence->setText(tr("Cannot display data"));
    }
    event->acceptProposedAction();
    emit changed();
}

void DropArea::dragLeaveEvent(QDragLeaveEvent *event)
{
    clear();
    event->accept();
}

void DropArea::clear()
{
    setStatus(EMPTY);
    QPixmap dragAndDropLogo(":/icons/upload");
    pixmap->setPixmap(dragAndDropLogo);
    pixmap->setAlignment(Qt::AlignCenter);

    sentence->setText(tr("Please drop the file to send or use the browse button"));
    sentence->setAlignment(Qt::AlignCenter);

    emit changed();
}

void DropArea::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void DropArea::setText(QString newSentence)
{
    sentence->setText(newSentence);
}

void DropArea::setPixmap(QPixmap newPixmap)
{
    pixmap->setPixmap(newPixmap);
}

QString DropArea::getText()
{
    return sentence->text();
}

int DropArea::getStatus()
{
    return full;
}

void DropArea::setStatus(STATUS newStatus)
{
    full = newStatus;
}

