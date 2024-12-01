#ifndef QMEMORY_LAYOUT_RENDER_H__
#define QMEMORY_LAYOUT_RENDER_H__

#include <QFrame>
#include <QWidget>
#include <QPaintEvent>

#include "Helpers.h"

namespace VisualLinkerScript::Components::MemoryVisualizer::Composition
{
	class CMemorySection;
}

class QTextEdit;
class QPushButton;

using namespace VisualLinkerScript;
using namespace VisualLinkerScript::Components::MemoryVisualizer::Composition;

/// @brief Linker Script Memory Layout Visualizer
class QMemoryLayoutRender : public QFrame
{
    Q_OBJECT

private:
    void BuildInterface();
    void RedrawDoubleBuffer();

    QPixmap m_doubleBuffer;
    SharedPtrVector<CMemorySection> m_model;


public:
    /// @brief Default constructor
    QMemoryLayoutRender(QWidget* parent = 0) :
        QFrame(parent)
    {
        this->BuildInterface();
    }

    /// @brief Return the hinted size of the object.
    [[nodiscard]] QSize sizeHint() const override {
        return {50, 50};  // Default size hint (width, height)
    }

    /// @brief Updates the model for this render.
    void SetModel(const SharedPtrVector<CMemorySection>& model);

protected:
    void resizeEvent(QResizeEvent* event) override;    
    void paintEvent(QPaintEvent* paintEvent) override;
    void wheelEvent(QWheelEvent* event) override;

signals:
    void evMouseWheel(int xSteps, int ySteps);
    void evMoveScreen(int xMovement, int yMovement);

public slots:    

};


#endif // QMEMORY_LAYOUT_RENDER_H__
