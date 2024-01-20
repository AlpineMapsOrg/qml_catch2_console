/*****************************************************************************
 * QML Catch2 console
 * Copyright (C) 2023 Adam Celarek
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *****************************************************************************/

#include <QDirIterator>
#include <QQuickWindow>
#include <chrono>
#include <cstdio>
#include <iostream>
#include <limits>

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTimer>
#include <catch2/catch_session.hpp>
#include <catch2/catch_test_case_info.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/reporters/catch_reporter_event_listener.hpp>
#include <catch2/reporters/catch_reporter_registrars.hpp>
#include <fmt/core.h>

#include "StdBuffer.h"

namespace {
StdBuffer* g_std_buffer = nullptr;
}

class ProgressPrinter : public Catch::EventListenerBase
{
public:
    using EventListenerBase::EventListenerBase;

    static std::string getDescription() {
        return "Reporter that reports the progress after every test case.";
    }

    void testCaseEnded(const Catch::TestCaseStats& status) override
    {
        g_std_buffer->append(fmt::format("test case: {:<140}", status.testInfo->name));
        if (status.totals.testCases.allOk())
            g_std_buffer->append(fmt::format("passed {:>4} of {:>4}\n",
                                             status.totals.assertions.passed,
                                             status.totals.assertions.total()));
        else
            g_std_buffer->append(fmt::format("FAILED {:>4} of {:>4}\n",
                                             status.totals.assertions.failed,
                                             status.totals.assertions.total()));
        QGuiApplication::instance()->processEvents(QEventLoop::AllEvents, 1);
    }

    void assertionStarting(const Catch::AssertionInfo&) override
    {
        QGuiApplication::instance()->processEvents(QEventLoop::AllEvents, 0);
    }

    void assertionEnded(const Catch::AssertionStats&) override
    {
        QGuiApplication::instance()->processEvents(QEventLoop::AllEvents, 0);
    }
};

CATCH_REGISTER_LISTENER(ProgressPrinter)

int main( int argc, char* argv[] ) {
    int argc_qt = 1;
    QGuiApplication app = {argc_qt, argv};
    StdBuffer std_buffer(&app);
    g_std_buffer = &std_buffer;
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("_std_buffer", &std_buffer);
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [](QObject* obj, const QUrl& objUrl) {
            if (!obj) {
                qDebug() << "Creating QML object from " << objUrl << " failed!";
                QCoreApplication::exit(-1);
            }
        },
        Qt::QueuedConnection);


    engine.load(QUrl("qrc:/qt/qml/src/qml_catch2_console/console.qml"));
    QQuickWindow* root_window = dynamic_cast<QQuickWindow*>(engine.rootObjects().first());
    if (root_window == nullptr) {
        qDebug() << "root window not created!";
        return 1;
    }

#if (defined(__linux) && !defined(__ANDROID__)) || defined(_WIN32) || defined(_WIN64)
    root_window->showMaximized();
#endif

    QTimer::singleShot(1, [=]() {
        std::vector<char*> argv_2;
        for (int i = 0; i < argc; ++i) {
            argv_2.push_back(argv[i]);
        }
        std::array<char, 20> console_colour_switch = {"--colour-mode none"};
        argv_2.push_back(console_colour_switch.data());
        const int argc_2 = argc + 1;
        const auto retval = Catch::Session().run(argc_2, argv_2.data());
        std::cout << "Catch::Session finished with exit code " << retval << std::endl;
#if defined(__ANDROID__) && (defined(__i386__) || defined(__x86_64__))
        // stop automatically in the android emulator
        QTimer::singleShot(1000, []() { QCoreApplication::quit(); });
#endif
    });
    return app.exec();
}
