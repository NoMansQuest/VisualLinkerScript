#ifndef CMEMORY_VISUALIZER_H__
#define CMEMORY_VISUALIZER_H__

#include <QFrame>
#include <QWidget>
#include <QPaintEvent>
#include <QScrollBar>

#include "EInsertObjectType.h"
#include "SBackendResponse.h"

class QLineEdit;
class QHBoxLayout;
class QVBoxLayout;
class QMemoryLayoutRender;
class QTextEdit;
class QPushButton;

/// @brief Linker Script Memory Layout Visualizer
class QMemoryVisualizer final : public QFrame
{
    Q_OBJECT

private:
    QPushButton* m_zoomInButton;
    QPushButton* m_zoomOutButton;
    QPushButton* m_zoomToContent;
    QLineEdit* m_currentZoomTextEdit;
    QScrollBar* m_horizontalScrollBar;
    QScrollBar* m_verticalScrollBar;
    QMemoryLayoutRender* m_memoryLayoutRender;

    QVBoxLayout* m_masterLayout;
    QHBoxLayout* m_rendererAndVScrollHolderLayout;
    QHBoxLayout* m_hScrollAndButtonsHolderLayout;
    QVBoxLayout* m_hScrollHousingLayout;


    void BuildInterface();

public:
    /// @brief Default constructor
    QMemoryVisualizer(QWidget* parent = 0)
        : QFrame(parent)
    {
        this->BuildInterface();
    }

signals:

public slots:

};


#endif // CMEMORY_VISUALIZER_H__

