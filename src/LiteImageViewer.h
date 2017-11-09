#ifndef LITEIMAGEVIEWER_H
#define LITEIMAGEVIEWER_H

#include <QWidget>
#include <QScrollArea>
#include <QImage>

class ImageDispWidget;
class CustomScrollArea;

////////////////////////////////////////////////////////////////////////////////
/// 轻量级图片浏览器，特性：
/// 1、支持拖动
/// 2、支持缩放
/// 3、可支持较大图像（QImage上限）
class LiteImageViewer : public QWidget
{
    Q_OBJECT

public:
    explicit LiteImageViewer(QWidget *parent = 0);

public slots:
    void setImage(const QImage &);
    void setImage(const QString &);

protected:
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

private:
    void init();
    void initUI();
    void createConnects();
    void scaleImage(qreal);
    void adjustScrollBar(QScrollBar *, qreal factor);

private slots:


private:
    /**
     * @brief getMaxRatioSize
     *  获取某容器内保持比例的最大矩形
     * @param container
     * @param inner_rect
     * @return
     */
    QSize getMaxRatioSize(const QRect &container, const QRect &inner_rect);

private:
    CustomScrollArea    *m_scroll_area;
    ImageDispWidget     *m_disp_widget;

    qreal   m_scale_ratio;                        // 缩放比例

    // 拖拽相关
    bool    m_mouse_pressed;
    QPoint  m_strat_drag_mouse_pos;

private slots:
    void slotInitImageSize();
    void slotMouseWheel(qreal);

signals:
};


////////////////////////////////////////////////////////////////////////////////
/// 自定义滚动区
///
class CustomScrollArea : public QScrollArea
{
    Q_OBJECT

public:
    explicit CustomScrollArea(QWidget *parent = NULL);

protected:
    virtual void wheelEvent(QWheelEvent *e);

signals:
    void sigWheel(qreal);
};

#endif // LITEIMAGEVIEWER_H
