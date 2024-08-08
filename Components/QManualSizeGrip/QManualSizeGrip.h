#ifndef QMANUALSIZEGIP_H__
#define QMANUALSIZEGIP_H__

#include <QtWidgets>

/// @brief QManualSizeGrip
class QManualSizeGrip : public QFrame {
    Q_OBJECT

public:
    /// @brief Default constructor.
    explicit QManualSizeGrip(QWidget* parent = nullptr);

signals:
    /// @brief Triggered when a resize is expected to take place.
    void resized(int dx, int dy);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    bool m_resizing;
    bool m_dragging = false;    
    QPointF m_mouseHoldStartPosition;
};


#endif
