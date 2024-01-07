#ifndef CMEMORY_VISUALIZER_H__
#define CMEMORY_VISUALIZER_H__

#include <QWidget>
#include <QPaintEvent>
#include "EInsertObjectType.h"
#include "SBackendResponse.h"

namespace VisualLinkerScript::Components::MemoryVisualizer
{
    /// @brief Linker Script Memory Layout Visualizer
    class QMemoryVisualizer : public QWidget
    {
        Q_OBJECT

    public:
        /// @brief Default constructor
        QMemoryVisualizer(QWidget* parent = 0);

        /// @brief Default destructor
        ~QMemoryVisualizer();

    protected:
        /// @brief Overriden Paint-Event, which is where our painting takes place.
        virtual void paintEvent(QPaintEvent* paintEvent) override;        
    
    signals:


    public slots:

    };

};

#endif // CMEMORY_VISUALIZER_H__

