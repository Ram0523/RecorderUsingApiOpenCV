#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "VideoRecorder.h"
#include "AudioRecorder.h"
#include "filemanager.h"


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

     qmlRegisterType<VideoRecorder>("com.example", 1, 0, "VideoRecorder");
     qmlRegisterType<AudioRecorder>("com.example", 1, 0, "AudioRecorder");
     qmlRegisterType<FileManager>("FileManager", 1, 0, "FileManager");


    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("RecorderUsingApiOpenCV", "Main");

    return app.exec();
}


