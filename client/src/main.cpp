#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "Application.h"

using namespace gs::calc;

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    qputenv("QT_QUICK_CONTROLS_STYLE", "Material");

    QGuiApplication app(argc, argv);

    app.setOrganizationName("ExperimentalConnectivitySolutions");
    app.setApplicationName("SvistCalc");

    Application calcApp;
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("calcApp", &calcApp);

    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    return app.exec();
}
