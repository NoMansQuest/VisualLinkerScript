#include <QCoreApplication>
#include "TestRegistry.h"

/// @brief Automatically resolve and run all registered tests
int main(int argc, char* argv[]) {
    QCoreApplication app(argc, argv);
    return TestRegistry::instance().RunAllTests(argc, argv);
}