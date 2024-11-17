#ifndef CMEMORY_VISUALIZER_H__
#define CMEMORY_VISUALIZER_H__

#include <QFrame>
#include <QWidget>
#include <QPaintEvent>
#include <QScrollBar>

#include "Helpers.h"

namespace VisualLinkerScript::Components::MemoryVisualizer::Models
{
	class CMemorySection;
}

class QLineEdit;
class QHBoxLayout;
class QVBoxLayout;
class QMemoryLayoutRender;
class QTextEdit;
class QPushButton;

using namespace VisualLinkerScript;
using namespace VisualLinkerScript::Components::MemoryVisualizer::Models;

/// @brief Linker Script Memory Layout Visualizer
class QMemoryVisualizer final : public QFrame
{
    Q_OBJECT

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

    SharedPtrVector<CMemorySection> m_memorySections;

    void BuildInterface();

public:
    /// @brief Default constructor
    QMemoryVisualizer(QWidget* parent = 0)
        : QFrame(parent)
    {
        this->BuildInterface();
    }

    /// @brief Update memory sections (i.e. the content of this component).
    void SetModel(SharedPtrVector<CMemorySection>&& memorySections);

private:
    void CalculateAndUpdateModelGeometry();
    void RequestRedraw() const;

signals:

private slots:
    void OnMouseWheel(int xSteps, int ySteps) const;

};


#endif // CMEMORY_VISUALIZER_H__

