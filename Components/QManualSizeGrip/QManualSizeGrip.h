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
    void resized(QPointF globalPosition, int dx, int dy);

    /// @brief Triggered when resize is about to start  (i.e. mouse button pressed and held).
    void resizeStarted(QPointF globalPosition);

    /// @brief Triggered when resize is finished (i.e. mouse press was release).
    void resizeFinished();

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
