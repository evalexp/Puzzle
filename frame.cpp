#include "frame.h"

Frame::Frame(QWidget *parent):QFrame(parent)
{
    setAcceptDrops(true);
    this->pixH = this->pixW = 0;
    this->level = 3;
}

/**
 * @brief Frame::setLevel
 * @param level
 */
void Frame::setLevel(int level){
    this->level = level;
    rects.clear();
    pixList.clear();
    pixSplit(level, level);
    update();
}

/**
 * @brief Frame::paintEvent
 * @param event
 */
void Frame::paintEvent(QPaintEvent *){
    QPainter painter(this);
    if(this->pixH == 0 || this->pixW == 0){
        painter.drawPixmap(0,0,this->width(), this->height(), pix);
    }else{
        for(auto i = 0; i < pixList.count(); i++){
            painter.drawPixmap(rects[i], pixList[i]);
        }
    }
}
/**
 * @brief Frame::loadImage
 * @param fileName
 */
void Frame::loadImage(QString fileName){
    bool success = pix.load(fileName);
    if(success){
        setLevel(this->level);
        update();
    }else{
        QMessageBox::warning(this, "ERROR", "文件打开失败");
    }
}

/**
 * @brief Frame::swap
 * @param a
 * @param b
 */
template <class T>
Frame* Frame::swap(T &a, T &b){
    T c = a;
    a = b;
    b = c;
    return this;
}

/**
 * @brief Frame::dragEnterEvent
 * @param event
 */
void Frame::dragEnterEvent(QDragEnterEvent *event){
    event->accept();
}

/**
 * @brief Frame::pixSplit
 * @param row
 * @param col
 */
void Frame::pixSplit(int row, int col){
    this->row = row;
    this->col = col;
    this->pixW = pix.size().width() / col;
    this->pixH = pix.size().height() / row;
    int w = this->width() / col;
    int h = this->height() / row;
    for(auto i = 0; i < row; i++){
        for(auto j = 0; j < col; j++){
            rects << QRect(j*w, i*h, w, h);
            pixList << pix.copy(j * pixW, i * pixH, pixW, pixH);
            nowIndex[i*row+j] = i*row+j;
        }
    }
    this->upsetOrder();
}

/**
 * @brief Frame::upsetOrder
 */
void Frame::upsetOrder(){
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    for(auto i = 0; i < this->row * this->col; i++){
        int randNum1 = qrand()%(this->row * this->col);
        int randNum2 = qrand()%(this->row * this->col);
        this->swap(pixList[randNum1], pixList[randNum2])
            ->swap(nowIndex[randNum1], nowIndex[randNum2]);
    }
    update();
}

/**
 * @brief Frame::dropEvent
 * @param event
 */
void Frame::dropEvent(QDropEvent *event){
    int index = targetIndex(event->pos());
    this->swap(pixList[index], pixList[srcIndex])
        ->swap(nowIndex[index], nowIndex[srcIndex]);
    for(auto i = 0; i < this->row * this->col; i++){
        if(nowIndex[i] != i){
            update();
            return ;
        }
    }
    update();
    emit this->sendSuccess();
}

/**
 * @brief Frame::mousePressEvent
 * @param event
 * set the drag picture
 */
void Frame::mousePressEvent(QMouseEvent *event){
    if(!pixList.count())
        return ;
    srcIndex = targetIndex(event->pos());
    QMimeData *data = new QMimeData;
    src = pixList[targetIndex(event->pos())];
    QDrag *drag = new QDrag(this);
    drag->setMimeData(data);
    drag->setPixmap(src.scaled(QSize(this->width() / col, this->height() / row)));
    drag->exec();
}

/**
 * @brief Frame::targetIndex
 * @param pos
 * @return index
 * index of the position
 */
int Frame::targetIndex(const QPoint &pos){
    int w = this->width() / col;
    int h = this->height() / row;
    return pos.y() / h * col + pos.x() / w;
}

/**
 * @brief Frame::recvLevel
 * @param level
 * reset level
 */
void Frame::recvLevel(int level){
    this->setLevel(level);
}
