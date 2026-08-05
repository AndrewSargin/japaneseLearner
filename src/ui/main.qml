import QtQuick
import QtQuick.Window 2.15
import QtQuick.Controls
import QtQuick.Layouts
import JapaneseLearner 1.0

Window {
    width: 1280
    height: 720
    visible: true

    ColumnLayout {

        anchors.fill: parent
        spacing: 0

        Rectangle {
            id: topBar
            Layout.fillWidth: true
            Layout.preferredHeight: 40
            color: "#1a1a1a"

            Row {
               anchors.left: parent.left
               anchors.verticalCenter: parent.verticalCenter
               anchors.leftMargin: 10
               spacing: 8
                
               Button {text: "⚙"}
            }

            Row {
                anchors.centerIn: parent
                spacing: 10

                Button { text: "Load Video" }
                Button { text: "Load Subs (Original)" }
                Button { text: "Load Subs (Translation)" }

            }

            Row {
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                anchors.rightMargin: 10

                Text { color: "white"
                text: "JapaneseLearner" }
            }

        }

        MpvWidget {
            id: mpv
            Layout.fillWidth: true
            Layout.fillHeight: true
            x: 0
            y: 0

            FrameAnimation {
                running: true
                onTriggered: {
                subtitleText.text = engine.getPhraseAtTime(mpv.getCurrentTime())
                }
            }

            Text {
                id: subtitleText
                anchors.bottom: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                color: "white"
                font.pixelSize: 24
                text: ""
            }

            onPositionChanged: function(time) {
                console.log("time:", time)
                subtitleText.text = engine.getPhraseAtTime(time)
            }
        }
         


        Rectangle {
            id: controls
            Layout.fillWidth: true
            Layout.preferredHeight: 60
        }

        Rectangle {
            id: wordBreakdown
            Layout.fillWidth: true
            Layout.preferredHeight: 150
        }
    }
}
