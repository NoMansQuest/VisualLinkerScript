#ifndef CMEMORY_VISUALIZER_H__
#define CMEMORY_VISUALIZER_H__

#include <QWidget>
#include <QPaintEvent>
#include "EInsertObjectType.h"
#include "SBackendResponse.h"

namespace VisualLinkerScript::Components::MemoryVisualizer
{
    /// @brief Linker Script Memory Layout Visualizer
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
        /// @brief Requests addition of a LMA->VMA link
        void AddLmaToVmaLink(
            const std::string& sourceObjectPath,
            const std::string& targetObjectPath,
            SBackendResponse& backendResponse);

        /// @brief Requests removal of LMA->VMA link
        void DeleteLmaToVmaLink(
            const std::string& sourceObjectPath,
            const std::string& targetObjectPath,
            SBackendResponse& backendResponse);

        /// @brief User asking an object to be inserted at a given location.
        void InsertContentInLocation(
            const std::string& parentPath,
            const std::string& precedingObjectPath,
            const std::string& succeedingObjectPath,
            EInsertObjectType objectType,
            SBackendResponse& backendResponse);

        /// @brief User asking an object to be inserted at a given location.
        void MoveContentToLocation(
            const std::string& objectToMove,
            const std::string& targetParentPath,
            const std::string& targetPrecedingObjectPath,
            const std::string& targetSucceedingObjectPath,
            SBackendResponse& backendResponse);

        /// @brief User asking an object to be removed at a given location.
        void RemoveContent(
            const std::string& targetObjecPath,
            SBackendResponse& backendResponse);

        /// @brief User asking to have a PHDR added to a section object
        void AddPhdrToSectionObject(
            const std::string& targetSectionObject, 
            const std::string& phdr,
            SBackendResponse& backendResponse);

        /// @brief User asking to have a PHDR removed from a section object
        void RemovePhdrFromSectionObject(
            const std::string& targetSectionObject, 
            const std::string& phdr,
            SBackendResponse& backendResponse);

        /// @brief User asking to have the fill expression of a section object
        void SetFillExpressionForSectionObject(
            const std::string& targetSectionObject, 
            uint32_t fillExpression,
            SBackendResponse& backendResponse);

        /// @brief User asking to have the name of a section object updated.
        void UpdateSectionObjectName(            
            const std::string& targetSectionObject, 
            const std::string& newName,
            SBackendResponse& backendResponse);

        void UpdateMemoryLocationIndicator(                        
            const std::string& indicatorPath, 
            uint64_t newValue,
            SBackendResponse& backendResponse);

        /// @brief When a user selects an object, this signal will be emitted. the object
        ///        in question could be content, LMA-to-VMA link, memory location indicator or 
        ///        size indicator
        void UserSelectedObject(
            const std::string& objectPath,
            SBackendResponse& backendResponse);

    public slots:

    };

};

#endif // CMEMORY_VISUALIZER_H__

