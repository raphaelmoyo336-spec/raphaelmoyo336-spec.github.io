#pragma once

#include <QString>

struct TestWorkload
{
    int repetitions;
    int documentBytes;
    int dockWidgets;
};

inline TestWorkload testWorkload()
{
    const QString profile =
        qEnvironmentVariable("KOGENCE_TEST_PROFILE", QStringLiteral("small")).toLower();
    if (profile == QStringLiteral("medium"))
        return {30, 256 * 1024, 16};
    if (profile == QStringLiteral("large"))
        return {60, 1024 * 1024, 32};
    return {12, 64 * 1024, 8};
}
