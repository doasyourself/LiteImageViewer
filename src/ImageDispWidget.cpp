#include "ImageDispWidget.h"
#include <QPainter>

ImageDispWidget::ImageDispWidget(QWidget *parent)
    : QWidget(parent)
{

}

void ImageDispWidget::setImage(const QImage &image)
{
    m_dest_image = image;
}

const QImage &ImageDispWidget::getImage()
{
    return m_dest_image;
}

void ImageDispWidget::paintEvent(QPaintEvent *e)
{
    QWidget::paintEvent(e);
    QPainter pat(this);
    pat.drawImage(this->rect(), m_dest_image);
}
