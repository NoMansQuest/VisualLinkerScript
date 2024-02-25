#ifndef QLINKERSCRIPT_MANAGER_H__
#define QLINKERSCRIPT_MANAGER_H__


#include <QtWidgets>
#include <QString>
#include <QtWidgets>
#include <QLabel>
#include <QIcon>
#include <memory>

namespace VisualLinkerScript::Components
{
    /// @brief The QChromTab is composed of CChromeTabItem objects
    class QLinkerScriptManager : public QObject
    {
        Q_OBJECT

    private:


    public:
        /// @brief Default constructor
        explicit QLinkerScriptManager(QObject *parent = nullptr)
            : QObject(parent)
        {}

        /// @brief Default desctructor
        ~QLinkerScriptManager()
        {}

    signals:


    protected slots:
        void CloseButtonPressed();

    public:


    protected:
    };
};

#endif // end of "QLINKERSCRIPT_MANAGER_H__"
