#include "QLinkerScriptManager.h"
#include "../Components/QLinkerScriptSession/CLinkerScriptSessionFileInfo.h"
#include "../Components/QLinkerScriptSession/QLinkerScriptSession.h"

/// @brief Create a new session for an 'untitled' linker script.
std::shared_ptr<QLinkerScriptSession> QLinkerScriptManager::CreateSessionForUntitled()
{
    uint32_t proposedIndex = 0;
    std::string untitledFileNameTemplate = "Untitled-%d.lds";
    while (proposedIndex++)
    {
        bool matchFound = false;
        auto iterator = std::find_if(this->m_sessions.begin(), this->m_sessions.end(), [=](const std::shared_ptr<QLinkerScriptSession>& entry)
        {
            return StringEquals(StringFormat(untitledFileNameTemplate, proposedIndex), entry->SessionFileInfo().FileName());
        });
        if (iterator != this->m_sessions.end())
        {
            continue;
        }        
        break;
    }

    auto newSessionId = this->m_sessionIdHolder++;
    auto targetFileName = StringFormat(untitledFileNameTemplate, proposedIndex);
    auto newSession = std::make_shared<QLinkerScriptSession>(newSessionId, CLinkerScriptSessionFileInfo(false, "", targetFileName, "", ""));
    this->m_sessions.emplace_back(newSession);
    emit this->evNewSessionCreated(newSession);
    return newSession;
}

/// @brief Creates a new session based on an existing file on the disk
std::shared_ptr<QLinkerScriptSession> QLinkerScriptManager::CreateSessionForExistingFile(CLinkerScriptSessionFileInfo existingFileInfo)
{
    auto newSessionId = this->m_sessionIdHolder++;
    auto newSession = std::make_shared<QLinkerScriptSession>(newSessionId, existingFileInfo);
    this->m_sessions.emplace_back(newSession);
    emit this->evNewSessionCreated(newSession);
    return newSession;
}

void QLinkerScriptManager::CloseButtonPressed(uint32_t tabId)
{
    auto iterator = std::find_if(this->m_sessions.begin(), this->m_sessions.end(), [=](const std::shared_ptr<QLinkerScriptSession>& entry)
        {
            return entry->TabIndex() == tabId;
        });

    if (iterator == this->m_sessions.end())
    {
        // We have an error
        throw std::out_of_range("TabId did not correspond to any of the available sessions.");
    }

    // Now we have to prepare for closure.
    std::string stuff;
}