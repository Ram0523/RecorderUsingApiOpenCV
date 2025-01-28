import QtQuick
import QtQuick.Controls
import com.example 1.0

Rectangle {
    id: audioRec
    width: parent.width
    height: parent.height

    AudioRecorder {
        id: audioRecorder
    }

    Rectangle {
        id: imageBox
        width: audioRec.width
        height: audioRec.height * 0.7
        anchors.top: parent.top

        Image {
            id: imgW
            source: "file:///E:/workspace/Recorder/Images/Sound-Wave-Transparent.png"
            anchors.fill: parent
            fillMode: Image.PreserveAspectFit
        }

        // Blinking indicator
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
            visible: audioRecorder.isRecording
            Timer {
                interval: 500
                running: audioRecorder.isRecording
                repeat: true
                onTriggered: parent.visible = !parent.visible
            }
        }
    }

    Rectangle {
        id: buttonBox
        width: audioRec.width
        height: audioRec.height * 0.3
        anchors.top: imageBox.bottom

        Button {
            id: startA
            text: audioRecorder.isRecording ? "Recording..." : "Start Recording"
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
            enabled: !audioRecorder.isRecording
            onClicked: audioRecorder.startRecording()
        }

        Button {
            id: stopA
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
            enabled: audioRecorder.isRecording
            onClicked: audioRecorder.stopRecording()
        }
    }
}
