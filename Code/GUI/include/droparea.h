#ifndef DROPAREA_H
#define DROPAREA_H
#include <QLabel>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QStackedWidget>
#include <QFrame>
#include <QVBoxLayout>
#include <QMimeData>

enum STATUS {EMPTY, FULL};

class DropArea : public QWidget
{
    Q_OBJECT
public:
    explicit DropArea(QWidget *parent = nullptr);
    QString getText();
    int getStatus();
    void setStatus(STATUS newStatus);
    void setText(QString newSentence);
    void setPixmap(QPixmap newPixmap);
    void setStack(QStackedWidget *newStack);

public slots:
    void clear();

signals:
    void changed(const QMimeData *mimeData = nullptr);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void paintEvent(QPaintEvent *);

private:
    QStackedWidget *stack;
    QLabel *pixmap;
    QLabel *sentence;
    STATUS full = EMPTY;

};

#endif // DROPAREA_H
