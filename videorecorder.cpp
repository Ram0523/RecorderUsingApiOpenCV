#include "VideoRecorder.h"

VideoRecorder::VideoRecorder(QObject *parent)
    : QObject(parent), m_isRecording(false), m_isPreviewEnabled(false)
{
    connect(&m_timer, &QTimer::timeout, this, &VideoRecorder::captureFrame);
    connect(&m_previewTimer, &QTimer::timeout, this, &VideoRecorder::updatePreview);

    // Set the output directory to the desired path
    m_outputPath = "E:/workspace/RecorderUsingApiOpenCV/recordings";
    QDir dir(m_outputPath);
    if (!dir.exists()) {
        dir.mkpath(".");
    }
}

void VideoRecorder::startRecording()
{
    if (m_isRecording || !m_capture.isOpened()) return;

    QString filename = m_outputPath + "/" + QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss") + ".avi";
    int fourcc = cv::VideoWriter::fourcc('M', 'J', 'P', 'G');
    double fps = 30.0;
    cv::Size frameSize(static_cast<int>(m_capture.get(cv::CAP_PROP_FRAME_WIDTH)),
                       static_cast<int>(m_capture.get(cv::CAP_PROP_FRAME_HEIGHT)));

    m_writer.open(filename.toStdString(), fourcc, fps, frameSize);
    if (!m_writer.isOpened()) {
        qWarning() << "Failed to create video file";
        return;
    }

    m_isRecording = true;
    emit isRecordingChanged();
    m_timer.start(33); // ~30 fps
}

void VideoRecorder::stopRecording()
{
    if (!m_isRecording) return;

    m_timer.stop();
    m_writer.release();
    m_isRecording = false;
    emit isRecordingChanged();
}

void VideoRecorder::captureFrame()
{
    cv::Mat frame;
    if (m_capture.read(frame)) {
        m_writer.write(frame);
        if (m_isPreviewEnabled) {
            updatePreview();
        }
    }
}

void VideoRecorder::enablePreview(bool enable)
{
    m_isPreviewEnabled = enable;
    emit isPreviewEnabledChanged();

    if (enable) {
        if (!m_capture.isOpened()) {
            m_capture.open(0);  // Open default camera
            if (!m_capture.isOpened()) {
                qWarning() << "Failed to open camera";
                return;
            }
        }
        m_previewTimer.start(33); // ~30 fps
    } else {
        m_previewTimer.stop();
        m_capture.release();  // Close the camera when preview is disabled
    }
}

void VideoRecorder::updatePreview()
{
    cv::Mat frame;
    if (m_capture.read(frame)) {
        cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
        QImage qimg(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);

        // Save the image to a temporary file
        QString tempPath = QDir::temp().absoluteFilePath("preview.jpg");
        qimg.save(tempPath);

        // Emit the file path
        emit previewUpdated(tempPath);
    }
}

bool VideoRecorder::isPreviewEnabled() const
{
    return m_isPreviewEnabled;
}
