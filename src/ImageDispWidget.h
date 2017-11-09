#ifndef IMAGEDISPWIDGET_H
#define IMAGEDISPWIDGET_H

/**************************************************
** 高效地显示图片到widget中，相对于QLabel的setPixmap，
** 可以显示更大的图片，而基本不会出现内存不足。
** 原理是使用drawImage绘制到Widget上。不会占用额外内存。
***************************************************/

#include <QWidget>
#include <QImage>

class ImageDispWidget : public QWidget
{
public:
    ImageDispWidget(QWidget *parent = NULL);

    void setImage(const QImage &);
    const QImage &getImage();

protected:
    void paintEvent(QPaintEvent *e);

private:
    QImage m_dest_image;
};

#endif // IMAGEDISPWIDGET_H
