#include "QManualSizeGrip.h"
#include <QMouseEvent>

QManualSizeGrip::QManualSizeGrip(QWidget* parent)
    : QFrame(parent),
	  m_resizing(false)
{
    this->setMouseTracking(true);
    this->setFixedSize(20, 20);  // Set a fixed size for the size grip
    this->setCursor(Qt::SizeBDiagCursor);  // Set a diagonal resize cursor
}

void QManualSizeGrip::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    const auto squareSize = 2;
    const auto spacing = 1; // 1 pixel separation
    const auto squareColor = QColor(215, 215, 215, 255); 
    
    for (int i = 0; i < 3; ++i) 
    {
        for (int j = 0; j <= i; ++j) 
        {
            int x = width() - ((squareSize + spacing) * (i + 1));
            int y = height() - ((squareSize + spacing) * (j + 1));
            painter.fillRect(x, y, squareSize, squareSize, squareColor);
        }
    }
}

void QManualSizeGrip::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) 
    {
        this->m_resizing = true;
        this->m_mouseHoldStartPosition = event->globalPosition();
        emit this->resizeStarted(event->globalPosition());
        event->accept();
    }
}

void QManualSizeGrip::mouseMoveEvent(QMouseEvent* event)
{
    if (this->m_resizing)
    {
	    const auto currentPos = event->globalPosition();
        const auto dx = currentPos.x() - this->m_mouseHoldStartPosition.x();
        const auto dy = currentPos.y() - this->m_mouseHoldStartPosition.y();

        if (std::abs(dx) >= 1.0f || std::abs(dy) >= 1.0f) {
            qDebug() << "Size diff reported, dx: " << dx << " ,dy: " << dy << "\n";
            emit resized(
                currentPos,
                static_cast<int>(dx),
                static_cast<int>(dy));
        }        
        event->accept();
    }
}

void QManualSizeGrip::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) 
    {
        this->m_resizing = false;
        emit this->resizeFinished();
        event->accept();
    }
}