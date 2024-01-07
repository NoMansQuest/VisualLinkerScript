#include "QMemoryVisualizer.h"
#include <QPainter>

using namespace VisualLinkerScript::Components::MemoryVisualizer;

CMemoryVisualizer::CMemoryVisualizer(QWidget* parent)
    : QWidget(parent)
{
    // No particular action at this point in time
}

CMemoryVisualizer::~CMemoryVisualizer()
{
    // No particular action at this point in time
}

void CMemoryVisualizer::paintEvent(QPaintEvent* paintEvent)
{
    QPainter painter(this);
    //painter.tra
}
