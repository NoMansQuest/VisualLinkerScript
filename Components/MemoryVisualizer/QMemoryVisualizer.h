#ifndef CMEMORY_VISUALIZER_H__
#define CMEMORY_VISUALIZER_H__

#include <QFrame>
#include <QWidget>
#include <QPaintEvent>
#include <QScrollBar>

#include "Helpers.h"

class QPercentageLineEdit;

namespace VisualLinkerScript::Components::MemoryVisualizer::Composition
{
	class CFloorPlan;
	class CMemoryRegionBlock;
}

class QLineEdit;
class QHBoxLayout;
class QVBoxLayout;
class QMemoryLayoutRender;
class QTextEdit;
class QPushButton;

using namespace VisualLinkerScript;
using namespace VisualLinkerScript::Components::MemoryVisualizer::Composition;

/// @brief Linker Script Memory Layout Visualizer
class QMemoryVisualizer final : public QFrame
{
    Q_OBJECT

    QPushButton* m_zoomInButton;
    QPushButton* m_zoomOutButton;
    QPushButton* m_zoomToContent;
    QScrollBar*  m_horizontalScrollBar;
    QScrollBar*  m_verticalScrollBar;
    QPercentageLineEdit* m_currentZoomTextEdit;
    QMemoryLayoutRender* m_memoryLayoutRender;

    QVBoxLayout* m_masterLayout;
    QHBoxLayout* m_rendererAndVScrollHolderLayout;
    QHBoxLayout* m_hScrollAndButtonsHolderLayout;
    QVBoxLayout* m_hScrollHousingLayout;

    std::shared_ptr<CFloorPlan> m_model;

    void BuildInterface();

public:
    /// @brief Default constructor
    QMemoryVisualizer(QWidget* parent = 0)
        : QFrame(parent)
    {
        this->BuildInterface();
    }

    /// @brief Update the model (i.e. the content of this component).
    void SetModel(const std::shared_ptr<CFloorPlan>& floorPlan);

private:
    void CalculateAndUpdateModelGeometry() const;
    void RequestRedraw() const;

signals:

private slots:
    void OnScrollChangeByMouseWheel(int xSteps, int ySteps) const;
    void OnZoomChangedByMouseWheel(int zoomChangeInPercent) const;
    void OnScrollPositionChange() const;
    void OnZoomIncreaseClicked() const;
    void OnZoomDecreaseClicked() const;
    void OnZoomUpdated() const;
};


#endif // CMEMORY_VISUALIZER_H__

