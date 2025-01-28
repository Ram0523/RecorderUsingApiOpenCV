#ifndef VIDEORECORDER_H
#define VIDEORECORDER_H

#include <QObject>
#include <QTimer>
#include <opencv2/opencv.hpp>
#include <QDateTime>
#include <QDir>
#include <QImage>

class VideoRecorder : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isRecording READ isRecording NOTIFY isRecordingChanged)
    Q_PROPERTY(bool isPreviewEnabled READ isPreviewEnabled NOTIFY isPreviewEnabledChanged)

public:
    explicit VideoRecorder(QObject *parent = nullptr);

    Q_INVOKABLE void startRecording();
    Q_INVOKABLE void stopRecording();
    Q_INVOKABLE void enablePreview(bool enable);

    bool isRecording() const { return m_isRecording; }
    bool isPreviewEnabled() const;

signals:
    void isRecordingChanged();
    void isPreviewEnabledChanged();
    void previewUpdated(const QString &previewPath);


private slots:
    void captureFrame();
    void updatePreview();

private:
    cv::VideoCapture m_capture;
    cv::VideoWriter m_writer;
    QTimer m_timer;
    bool m_isRecording;
    QString m_outputPath;
    bool m_isPreviewEnabled;
    QTimer m_previewTimer;
};

#endif // VIDEORECORDER_H
