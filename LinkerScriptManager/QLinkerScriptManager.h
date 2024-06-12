#ifndef QLINKERSCRIPT_MANAGER_H__
#define QLINKERSCRIPT_MANAGER_H__

#include <QtWidgets>
#include <QString>
#include <memory>
#include "../Helpers.h"

using namespace VisualLinkerScript;
class QLinkerScriptSession;
class CLinkerScriptSessionFileInfo;

/// @brief The QChromTab is composed of CChromeTabItem objects
class QLinkerScriptManager : public QObject
{
    Q_OBJECT

private:
    SharedPtrVector<QLinkerScriptSession> m_sessions;
    uint32_t m_sessionIdHolder = 10000;

public:
    /// @brief Default constructor
    explicit QLinkerScriptManager(QObject *parent = nullptr)
        : QObject(parent)
    {}

    /// @brief Default desctructor
    ~QLinkerScriptManager()
    {}

signals:
    void evNewSessionCreated(std::shared_ptr<QLinkerScriptSession> createdSession);

protected slots:
    void CloseButtonPressed(uint32_t tabId);

public:
    /// @brief Create a new session for an 'untitled' linker script.
    std::shared_ptr<QLinkerScriptSession> CreateSessionForUntitled(void);

    /// @brief Creates a new session based on an existing file on the disk
    std::shared_ptr<QLinkerScriptSession> CreateSessionForExistingFile(CLinkerScriptSessionFileInfo existingFileInfo);

protected:
};

#endif // end of "QLINKERSCRIPT_MANAGER_H__"
