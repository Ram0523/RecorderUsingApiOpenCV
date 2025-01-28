#ifndef AUDIORECORDER_H
#define AUDIORECORDER_H

#include <QObject>
#include <QTimer>
#include <QDateTime>
#include <QDir>
#include <portaudio.h>
#include <vector>
#include <fstream>

class AudioRecorder : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isRecording READ isRecording NOTIFY isRecordingChanged)
public:
    explicit AudioRecorder(QObject *parent = nullptr);
    ~AudioRecorder();

    Q_INVOKABLE void startRecording();
    Q_INVOKABLE void stopRecording();
    bool isRecording() const { return m_isRecording; }

signals:
    void isRecordingChanged();

private:
    static int recordCallback(const void *inputBuffer, void *outputBuffer,
                              unsigned long framesPerBuffer,
                              const PaStreamCallbackTimeInfo* timeInfo,
                              PaStreamCallbackFlags statusFlags,
                              void *userData);

    bool m_isRecording;
    QString m_outputPath;
    PaStream *m_stream;
    std::vector<float> m_recordedSamples;
    std::ofstream m_wavFile;

    void writeWavHeader();
    void finalizeWavFile();
};

#endif // AUDIORECORDER_H
