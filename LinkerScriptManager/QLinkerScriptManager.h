#ifndef QLINKERSCRIPT_MANAGER_H__
#define QLINKERSCRIPT_MANAGER_H__

#include <QtWidgets>
#include <QString>
#include <memory>
#include "../Helpers.h"

namespace VisualLinkerScript::Models
{
	class CLinkerScriptFile;
}

using namespace VisualLinkerScript;
class QLinkerScriptSession;
class CLinkerScriptSessionFileInfo;


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

    /// @brief Default destructor
    ~QLinkerScriptManager()
    {}

signals:
    void evNewSessionCreated(std::shared_ptr<QLinkerScriptSession> createdSession);
    void evSessionClosureRequested(std::shared_ptr<QLinkerScriptSession> origin);

protected slots:
    void CloseButtonPressed(uint32_t tabId);

public:
    /// @brief Returns the number of sessions.
    uint32_t GetSessionsCount() const { return this->m_sessions.size(); }

    /// @brief Returns the session based on the given index.
    std::shared_ptr<QLinkerScriptSession> GetSession(uint32_t sessionIndex) const;

    /// @brief Create a new session for an 'untitled' linker script.
    std::shared_ptr<QLinkerScriptSession> CreateSessionForUntitled(void);

    /// @brief Creates a new session based on an existing file on the disk
    std::shared_ptr<QLinkerScriptSession> CreateSessionForExistingFile(std::shared_ptr<Models::CLinkerScriptFile> linkerScriptFile);
};

#endif // end of "QLINKERSCRIPT_MANAGER_H__"
