import QtQuick 2.15
import QtQuick.Controls 2.15
import com.example 1.0

Rectangle {
    id: videoRec
    width: parent.width
    height: parent.height

    VideoRecorder {
        id: videoRecorder
        onPreviewUpdated: previewImage.source = "file:///" + previewPath
    }

    Rectangle {
        id: videoBox
        width: videoRec.width
        height: videoRec.height * 0.7
        anchors.top: parent.top
        color: "black"

        Image {
            id: previewImage
            anchors.fill: parent
            fillMode: Image.PreserveAspectFit
            visible: videoRecorder.isPreviewEnabled
        }

        Text {
            anchors.centerIn: parent
            text: "Video Preview Not Available"
            color: "white"
            font.pixelSize: 20
            visible: !videoRecorder.isPreviewEnabled
        }

        Rectangle {
            id: recordingIndicator
            width: 20
            height: 20
            color: "red"
            radius: 10
            anchors {
                top: parent.top
                right: parent.right
                topMargin: 10
                rightMargin: 10
            }
            visible: videoRecorder.isRecording
            Timer {
                interval: 500
                running: videoRecorder.isRecording
                repeat: true
                onTriggered: parent.visible = !parent.visible
            }
        }
    }

    Rectangle {
        id: buttonBox
        width: videoRec.width
        height: videoRec.height * 0.3
        anchors.top: videoBox.bottom

        Button {
            id: startButton
            text: videoRecorder.isRecording ? "Recording..." : "Start Recording"
            width: buttonBox.width * 0.4
            height: buttonBox.height * 0.6
            anchors {
                left: buttonBox.left
                leftMargin: buttonBox.width * 0.05
                verticalCenter: buttonBox.verticalCenter
            }
            font.bold: true
            font.pixelSize: 16
            background: Rectangle {
                color: "#90EE90"
                radius: 10
            }
            enabled: !videoRecorder.isRecording
            onClicked: videoRecorder.startRecording()
        }

        Button {
            id: stopButton
            text: "Stop Recording"
            width: buttonBox.width * 0.4
            height: buttonBox.height * 0.6
            anchors {
                right: buttonBox.right
                rightMargin: buttonBox.width * 0.05
                verticalCenter: buttonBox.verticalCenter
            }
            font.bold: true
            font.pixelSize: 16
            background: Rectangle {
                color: "#FF7F7F"
                radius: 10
            }
            enabled: videoRecorder.isRecording
            onClicked: videoRecorder.stopRecording()
        }
    }

    Component.onCompleted: {
        videoRecorder.enablePreview(true)
    }

    Component.onDestruction: {
        videoRecorder.enablePreview(false)
    }
}
