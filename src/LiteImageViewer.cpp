#include "LiteImageViewer.h"
#include "ImageDispWidget.h"

#include <QWheelEvent>
#include <QVBoxLayout>
#include <QTimer>
#include <qmath.h>
#include <QScrollBar>

LiteImageViewer::LiteImageViewer(QWidget *parent)
    : QWidget(parent),
      m_mouse_pressed(false)
{
    init();
}

void LiteImageViewer::setImage(const QImage &dest_img)
{
    m_disp_widget->setImage(dest_img);

    // 初始化大小，延时设置，才会生效
    QTimer::singleShot(0, this, SLOT(slotInitImageSize()));
}

void LiteImageViewer::setImage(const QString &img_path)
{
    QImage dest_img(img_path);
    if (!dest_img.isNull())
    {
        setImage(dest_img);
    }
}

void LiteImageViewer::mouseMoveEvent(QMouseEvent *e)
{
    if (!m_mouse_pressed) return;
    QPoint diff = e->pos() - m_strat_drag_mouse_pos;
    m_strat_drag_mouse_pos = e->pos();

    if (!m_scroll_area->horizontalScrollBar()->isVisible())
    {
        diff.setX(0);
    }
    else if (!m_scroll_area->verticalScrollBar()->isVisible())
    {
        diff.setY(0);
    }

    QScrollBar *h_bar = m_scroll_area->horizontalScrollBar();
    h_bar->setValue(h_bar->value() - diff.rx());

    QScrollBar *v_bar = m_scroll_area->verticalScrollBar();
    v_bar->setValue(v_bar->value() - diff.ry());
}

void LiteImageViewer::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {
        QWidget *dest_widget = m_scroll_area->widget();
        Q_ASSERT(dest_widget);
        dest_widget->setCursor(Qt::ClosedHandCursor);
        m_mouse_pressed = true;
        m_strat_drag_mouse_pos = e->pos();
    }
}

void LiteImageViewer::mouseReleaseEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {
        QWidget *dest_widget = m_scroll_area->widget();
        Q_ASSERT(dest_widget);
        dest_widget->setCursor(Qt::OpenHandCursor);
        m_mouse_pressed = false;
    }
}

void LiteImageViewer::init()
{
    initUI();
    createConnects();
}

void LiteImageViewer::initUI()
{
    QVBoxLayout *lay_root = new QVBoxLayout;
    lay_root->setContentsMargins(0, 0, 0, 0);
    setLayout(lay_root);

    m_scroll_area = new CustomScrollArea;
    m_scroll_area->setAlignment(Qt::AlignCenter);
    lay_root->addWidget(m_scroll_area);

    m_disp_widget = new ImageDispWidget;
    m_disp_widget->setCursor(Qt::OpenHandCursor);
    m_scroll_area->setWidget(m_disp_widget);
}

void LiteImageViewer::createConnects()
{
    connect(m_scroll_area, SIGNAL(sigWheel(qreal)),
            SLOT(slotMouseWheel(qreal)));

}

void LiteImageViewer::scaleImage(qreal scale_factor)
{
    QSize new_size = m_disp_widget->size() * scale_factor;

    // 最小长宽限制
    if (new_size.width() < 100 || new_size.height() < 100)
    {
        return;
    }

    // 放大比例限制
    QSize raw_size = m_disp_widget->getImage().size();
    if (new_size.width() / raw_size.width() > 5)
        return;

    m_disp_widget->setFixedSize(new_size);
    adjustScrollBar(m_scroll_area->horizontalScrollBar(), scale_factor);
    adjustScrollBar(m_scroll_area->verticalScrollBar(), scale_factor);
}

void LiteImageViewer::adjustScrollBar(QScrollBar *bar, qreal factor)
{
    int value = factor * bar->value() + (factor - 1) * bar->pageStep() / 2;
    bar->setValue(value);
}

QSize LiteImageViewer::getMaxRatioSize(const QRect &container,
                                       const QRect &inner_rect)
{
    if (container.width() <= 0 || container.height() <=0)
        return inner_rect.size();
    if (inner_rect.width() <= 0 || inner_rect.height() <=0)
        return inner_rect.size();

    QSize dest_size;
    qreal ratio_container = container.width() * 1.0 / container.height();
    qreal ratio_inner_rect = inner_rect.width()  * 1.0 / inner_rect.height();

    // 比谁细长，也可以比谁细高
    if (ratio_container < ratio_inner_rect) // 里面的矩形比较长
    {
        int container_width = container.width();
        dest_size.setWidth(container_width);
        dest_size.setHeight(container_width / ratio_inner_rect);
    }
    else if (ratio_container == ratio_inner_rect)
    {
        dest_size = container.size();
    }
    else // ratio_container > ratio_inner_rect
    {
        int container_height = container.height();
        dest_size.setHeight(container_height);
        dest_size.setWidth(container_height * ratio_inner_rect);
    }

    return dest_size;
}

void LiteImageViewer::slotInitImageSize()
{
    QRect container = this->rect();
    container.adjust(0, 0, -2, -2);
    QSize size = getMaxRatioSize(container , m_disp_widget->getImage().rect());
    m_disp_widget->setFixedSize(size);
}

void LiteImageViewer::slotMouseWheel(qreal delta)
{
    int num_steps = delta / 8 / 15;
    if (num_steps == 0) return;

    qreal scale_factor = qPow(1.08, num_steps);
    this->scaleImage(scale_factor);
}

CustomScrollArea::CustomScrollArea(QWidget *parent)
    : QScrollArea(parent)
{

}

void CustomScrollArea::wheelEvent(QWheelEvent *e)
{
    int delta = e->delta();
    emit sigWheel(delta);
}
