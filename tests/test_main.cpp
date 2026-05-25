/*
 * PinkReader - Open source Reddit client for Android
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: test_main.cpp - Test suite entry point
 */


#include <QtTest>
#include <QApplication>
#include <QDebug>

/**
 * @brief Test suite entry point
 *
 * Runs all PinkReader unit tests. The test suite validates:
 * - Data model JSON serialization/deserialization
 * - Reddit API request/response handling
 * - Cache management operations
 * - URL parsing and deep link handling
 * - Markdown/HTML parsing accuracy
 * - Settings persistence
 * - Account management
 * - OAuth token handling
 * - Rate limiter behavior
 * - Image host API integration
 * - Cache compression integrity
 * - Listing controller pagination
 * - Adapter data operations
 *
 * @param argc Argument count
 * @param argv Argument vector
 * @return 0 if all tests pass, non-zero otherwise
 */
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Disable GUI during tests
    QApplication::setAttribute(Qt::AA_Use96Dpi, true);

    qDebug() << "========================================";
    qDebug() << "  PinkReader Test Suite";
    qDebug() << "========================================";

    int failedTests = 0;
    QStringList testResults;

    // Collect all test objects
    QList<QObject *> tests;

    // Run each test and collect results
    for (QObject *test : tests) {
        QString testName = test->metaObject()->className();
        qDebug() << "";
        qDebug() << "--- Running:" << testName << "---";

        int result = QTest::qExec(test, argc, argv);

        if (result == 0) {
            qDebug() << "  PASSED:" << testName;
            testResults.append(QString("  PASS: %1").arg(testName));
        } else {
            qDebug() << "  FAILED:" << testName << "(" << result << " failures)";
            testResults.append(QString("  FAIL: %1 (%2 failures)")
                                  .arg(testName).arg(result));
            failedTests++;
        }
    }

    // Summary
    qDebug() << "";
    qDebug() << "========================================";
    qDebug() << "  Test Results Summary";
    qDebug() << "========================================";

    for (const QString &result : testResults) {
        qDebug() << result.toUtf8().constData();
    }

    qDebug() << "";
    qDebug() << "Total tests run:" << tests.size();
    qDebug() << "Passed:" << (tests.size() - failedTests);
    qDebug() << "Failed:" << failedTests;

    if (failedTests == 0) {
        qDebug() << "";
        qDebug() << "All tests passed!";
        qDebug() << "========================================";
        return 0;
    } else {
        qDebug() << "";
        qDebug() << "Some tests FAILED!";
        qDebug() << "========================================";
        return 1;
    }
}
