 #include "audiorecorder.h"
#include <QDebug>

AudioRecorder::AudioRecorder(QObject *parent)
    : QObject{parent}, m_isRecording(false), m_stream(nullptr)
{
    Pa_Initialize();

    // Set the output directory to the desired path
    m_outputPath = "E:/workspace/RecorderUsingApiOpenCV/recordings";
    QDir dir(m_outputPath);
    if (!dir.exists()) {
        dir.mkpath(".");
    }
}

AudioRecorder::~AudioRecorder()
{
    if (m_stream) {
        Pa_CloseStream(m_stream);
    }
    Pa_Terminate();
}

void AudioRecorder::startRecording()
{
    if (m_isRecording) return;

    m_recordedSamples.clear();

    QString filename = m_outputPath + "/" + QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss") + ".wav";
    m_wavFile.open(filename.toStdString(), std::ios::binary);
    writeWavHeader();

    PaError err;
    err = Pa_OpenDefaultStream(&m_stream,
                               1,               // input channels
                               0,               // output channels
                               paFloat32,       // sample format
                               44100,           // sample rate
                               paFramesPerBufferUnspecified,
                               AudioRecorder::recordCallback,
                               this);

    if (err != paNoError) {
        qWarning() << "PortAudio error: " << Pa_GetErrorText(err);
        return;
    }

    err = Pa_StartStream(m_stream);
    if (err != paNoError) {
        qWarning() << "PortAudio error: " << Pa_GetErrorText(err);
        return;
    }

    m_isRecording = true;
    emit isRecordingChanged();
}

void AudioRecorder::stopRecording()
{
    if (!m_isRecording) return;

    Pa_StopStream(m_stream);
    Pa_CloseStream(m_stream);
    m_stream = nullptr;

    finalizeWavFile();

    m_isRecording = false;
    emit isRecordingChanged();
}

int AudioRecorder::recordCallback(const void *inputBuffer, void *outputBuffer,
                                  unsigned long framesPerBuffer,
                                  const PaStreamCallbackTimeInfo* timeInfo,
                                  PaStreamCallbackFlags statusFlags,
                                  void *userData)
{
    AudioRecorder* recorder = static_cast<AudioRecorder*>(userData);
    const float* input = static_cast<const float*>(inputBuffer);

    for (unsigned long i=0; i<framesPerBuffer; i++) {
        recorder->m_recordedSamples.push_back(input[i]);
        recorder->m_wavFile.write(reinterpret_cast<const char*>(&input[i]), sizeof(float));
    }

    return paContinue;
}

void AudioRecorder::writeWavHeader()
{
    // Write WAV header (to be filled with correct sizes later)
    m_wavFile.write("RIFF", 4);
    m_wavFile.write("\x00\x00\x00\x00", 4); // File size (to be filled later)
    m_wavFile.write("WAVE", 4);
    m_wavFile.write("fmt ", 4);
    uint32_t subchunk1Size = 16;
    m_wavFile.write(reinterpret_cast<const char*>(&subchunk1Size), 4);
    uint16_t audioFormat = 3; // IEEE float
    m_wavFile.write(reinterpret_cast<const char*>(&audioFormat), 2);
    uint16_t numChannels = 1;
    m_wavFile.write(reinterpret_cast<const char*>(&numChannels), 2);
    uint32_t sampleRate = 44100;
    m_wavFile.write(reinterpret_cast<const char*>(&sampleRate), 4);
    uint32_t byteRate = sampleRate * numChannels * sizeof(float);
    m_wavFile.write(reinterpret_cast<const char*>(&byteRate), 4);
    uint16_t blockAlign = numChannels * sizeof(float);
    m_wavFile.write(reinterpret_cast<const char*>(&blockAlign), 2);
    uint16_t bitsPerSample = 32;
    m_wavFile.write(reinterpret_cast<const char*>(&bitsPerSample), 2);
    m_wavFile.write("data", 4);
    m_wavFile.write("\x00\x00\x00\x00", 4); // Data size (to be filled later)
}

void AudioRecorder::finalizeWavFile()
{
    // Fill in the file size and data size in the WAV header
    uint32_t dataSize = m_recordedSamples.size() * sizeof(float);
    uint32_t fileSize = 36 + dataSize;
    m_wavFile.seekp(4);
    m_wavFile.write(reinterpret_cast<const char*>(&fileSize), 4);
    m_wavFile.seekp(40);
    m_wavFile.write(reinterpret_cast<const char*>(&dataSize), 4);
    m_wavFile.close();
}
