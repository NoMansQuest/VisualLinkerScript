#pragma once

#include <QtTest/QtTest>
#include <functional>
#include <vector>

class TestRegistry {
public:
    static TestRegistry& Instance() {
        static TestRegistry registry;
        return registry;
    }

    void RegisterTest(const std::function<int(int, char* [])>& testFunction) {
        tests.push_back(testFunction);
    }

    int RunAllTests(int argc, char* argv[]) {
        int result = 0;
        for (const auto& test : tests) {
            result += test(argc, argv);
        }
        return result;
    }

private:
    std::vector<std::function<int(int, char* [])>> tests;
};