#include "QMemoryVisualizer.h"
#include <QPainter>

using namespace VisualLinkerScript::Components;

QMemoryVisualizer::QMemoryVisualizer(QWidget* parent)
    : QWidget(parent)
{
    // No particular action at this point in time
}

QMemoryVisualizer::~QMemoryVisualizer()
{
    // No particular action at this point in time
}

void QMemoryVisualizer::paintEvent(QPaintEvent* paintEvent)
{
    QPainter painter(this);
    //painter.tra
}
