#ifndef QMEMORY_LAYOUT_RENDER_H__
#define QMEMORY_LAYOUT_RENDER_H__

#include <QFrame>
#include <QWidget>
#include <QPaintEvent>

class QTextEdit;
class QPushButton;

/// @brief Linker Script Memory Layout Visualizer
class QMemoryLayoutRender : public QFrame
{
    Q_OBJECT

private:
    void BuildInterface();

public:
    /// @brief Default constructor
    QMemoryLayoutRender(QWidget* parent = 0) :
        QFrame(parent)
    {
        this->BuildInterface();
    }

    QSize sizeHint() const override {
        return QSize(50, 50);  // Default size hint (width, height)
    }

protected:
    /// @brief Overridden Paint-Event, which is where our painting takes place.
    virtual void paintEvent(QPaintEvent* paintEvent) override;

signals:

public slots:

};


#endif // QMEMORY_LAYOUT_RENDER_H__
