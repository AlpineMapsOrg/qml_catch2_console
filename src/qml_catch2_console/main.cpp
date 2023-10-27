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
#include <QTimer>
#include <catch2/catch_session.hpp>
#include <catch2/catch_test_case_info.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/reporters/catch_reporter_event_listener.hpp>
#include <catch2/reporters/catch_reporter_registrars.hpp>
#include <fmt/core.h>

class ProgressPrinter : public Catch::EventListenerBase
{
public:
    using EventListenerBase::EventListenerBase;

    static std::string getDescription() {
        return "Reporter that reports the progress after every test case.";
    }

    void testCaseEnded(const Catch::TestCaseStats& status) override {
        fmt::print("test case: {:<140}", status.testInfo->name);
        if (status.totals.testCases.allOk())
            fmt::println("\033[0;32mpassed {:>4} of {:>4}\033[0m", status.totals.assertions.passed, status.totals.assertions.total());
        else
            fmt::println("\033[0;31mfailed {:>4} of {:>4}\033[0m", status.totals.assertions.failed, status.totals.assertions.total());
        std::fflush(stdout);
        QGuiApplication::instance()->processEvents();
    }
};

CATCH_REGISTER_LISTENER(ProgressPrinter)

int main( int argc, char* argv[] ) {
    int argc_qt = 1;
    QGuiApplication app = {argc_qt, argv};
    QQmlApplicationEngine engine;
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

    //    QDirIterator it(":", QDirIterator::Subdirectories);
    //    while (it.hasNext()) {
    //        qDebug() << it.next();
    //    }

    engine.load(QUrl("qrc:/qt/qml/src/qml_catch2_console/console.qml"));
    QQuickWindow* root_window = dynamic_cast<QQuickWindow*>(engine.rootObjects().first());
    if (root_window == nullptr) {
        qDebug() << "root window not created!";
        return 1;
    }

#if (defined(__linux) && !defined(__ANDROID__)) || defined(_WIN32) || defined(_WIN64)
    root_window->showMaximized();
#endif

    QTimer::singleShot(1, [=]() { int result = Catch::Session().run(argc, argv); });
    return app.exec();
}
