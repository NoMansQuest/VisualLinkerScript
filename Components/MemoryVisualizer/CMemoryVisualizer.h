#ifndef CMEMORY_VISUALIZER_H__
#define CMEMORY_VISUALIZER_H__

#include <QWidget>
#include <QPaintEvent>

namespace VisualLinkerScript::Components::MemoryVisualizer
{
    class CMemoryVisualizer : public QWidget
    {
        Q_OBJECT

    public:
        /// @brief Default constructor
        CMemoryVisualizer(QWidget* parent = 0);

        /// @brief Default destructor
        ~CMemoryVisualizer();

    protected:
        /// @brief Overriden Paint-Event, which is where our painting takes place.
        virtual void paintEvent(QPaintEvent* paintEvent) override;        

    signals:
        /// @brief Signal emitted before any change is requested. If model is in valid state, the control
        ///        will then resume with further requests
        void ModelUpdateRequest(bool& modelCanBeUpdated);

        /// @brief Requests
        void MemoryRegionCreationRequest(std::string proposedName, uint64_t propsedStartRegion, uint64_t proposedLength, bool& granted)

        void MemoryRegionSelection(std::string path);




    public slots:

    };

};

#endif // CMEMORY_VISUALIZER_H__

