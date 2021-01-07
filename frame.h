#ifndef FRAME_H
#define FRAME_H
#include "QFrame"
#include "QPainter"
#include "QMessageBox"
#include "QDragEnterEvent"
#include "QDropEvent"
#include "QMouseEvent"
#include "QDrag"
#include "QMimeData"
#include "QDebug"
#include "QTime"

class Frame : public QFrame
{
    Q_OBJECT
public:
    Frame(QWidget *parent = 0);

    void loadImage(QString fileName);


    void upsetOrder();

private:
    QList<QPixmap> pixList;
    QList<QRect> rects;
    int level;
    QPixmap pix;
    int pixW, pixH;
    int row, col;
    QPixmap src;
    int srcIndex;

    int nowIndex[100];

    template <class T>
    Frame* swap(T& a, T& b);

    void setLevel(int level);
    void pixSplit(int row, int col);

    void paintEvent(QPaintEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

    int targetIndex(const QPoint &pos);


public slots:
    void recvLevel(int level);

signals:
    void sendSuccess();
};

#endif // FRAME_H
