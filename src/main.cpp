#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QIcon>
#include <QFont>

#include "ui/app_controller.hpp"
#include "core/image_cache.hpp"

int main(int argc, char* argv[]) {
    QGuiApplication app(argc, argv);
    app.setApplicationName("PinkReader");
    app.setOrganizationName("PinkReader");
    app.setApplicationVersion("0.1.0");

    // Register types
    qmlRegisterUncreatableType<PinkReader::Post>(
        "PinkReader", 1, 0, "Post", "Cannot create Post from QML");
    qmlRegisterUncreatableType<PinkReader::Comment>(
        "PinkReader", 1, 0, "Comment", "Cannot create Comment from QML");

    // Create controller
    auto* controller = new PinkReader::AppController(&app);

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("app", controller);

    // Register image provider
    engine.addImageProvider("pink",
        new PinkReader::PinkImageProvider(controller->imageCache()));

    // Load from QRC
    const QUrl url("qrc:/qt/qml/PinkReader/qml/main.qml");

    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreationFailed, &app,
        []() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);

    engine.load(url);

    return app.exec();
}
