#pragma once

#include "TestRegistry.h"

#define REGISTER_TEST(ClassName)                     \
    namespace {                                      \
        struct AutoRegister##ClassName {             \
            AutoRegister##ClassName() {              \
                TestRegistry::Instance().RegisterTest([](int argc, char* argv[]) { \
                    ClassName test;                  \
                    return QTest::qExec(&test, argc, argv); \
                });                                  \
            }                                        \
        };                                           \
        static AutoRegister##ClassName instance##ClassName; \
    }