#ifndef DRCCOMMONS_H__
#define DRCCOMMONS_H__

#define REGISTER_DRC_RULE(drcRule) \
    namespace { \
    using namespace VisualLinkerScript::DrcEngine; \
    using namespace VisualLinkerScript::DrcEngine::Rules; \
    bool __registration__##drcRule = CDrcManager::Instance().RegisterRule(std::make_shared<drcRule>()); \
    }

#endif // DRCCOMMONS_H__
