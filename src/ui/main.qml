import QtQuick
import QtQuick.Window 2.15
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
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

                Button {
                    text: "Load Video"
                    onClicked: videoDialog.open()
                }
                Button {
                    text: "Load Subs (Original)"
                    onClicked: subsDialog.open()
                }
                Button { text: "Load Subs (Translation)" }

            }

            Row {
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                anchors.rightMargin: 10

                Text {
                    color: "white"
                    text: "JapaneseLearner"
                }
            }

        }

        MpvWidget {
            id: mpv
            Layout.fillWidth: true
            Layout.fillHeight: true
            x: 0
            y: 0

            Text {
                id: subtitleText
                anchors.bottom: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                color: "white"
                font.pixelSize: 24
                text: ""
            }

            Component.onCompleted: {
                player.setWidget(this)
            }
            onPositionChanged: function(time) {
                subtitleText.text = engine.getPhraseAtTime(time)
                wordList.model = engine.getWordsAtTime(time)
            }
        }
         


        Rectangle {
            id: controls
            Layout.fillWidth: true
            Layout.preferredHeight: 60

            ColumnLayout {
                anchors.fill: parent

                RowLayout {
                    Slider {
                        id: videoPosition

                        Layout.fillWidth: true
                        from: 0
                        value: player.position
                        to: player.duration

                        onMoved: player.seek(value)
                    }

                }

                RowLayout {
                    Button {
                        text: "play"
                        onClicked: player.togglePlayPause()
                    }

                    Slider {
                        id: volume
                        from: 0
                        value: 25
                        to: 100
                        onMoved: player.setVolume(value)
                    }

                }
            }


        }

        Rectangle {
            id: wordBreakdown
            Layout.fillWidth: true
            Layout.preferredHeight: 150

            Row {

                anchors.fill: parent
                spacing: 8 
                padding: 10

                Repeater {
                    id: wordList

                    model: []

                    delegate: Rectangle {
                        width: 120
                        height: 130
                        color: "#2a2a2a"
                        radius: 8

                        Column {
                            anchors.centerIn: parent
                            spacing: 4

                            Text {
                                text: modelData.kanji
                                color: "#aaaaaa"
                                font.pixelSize: 12
                            }

                            Text {
                                text: modelData.reading
                                color: "#aaaaaa"
                                font.pixelSize: 12
                            }

                            Text {
                                text: modelData.meaning
                                color: "white"
                                font.pixelSize: 12
                            }

                            Text {
                                text: "N" + modelData.jlpt
                                color: "white"
                                font.pixelSize: 11

                            }

                        }

                    }

                }


            }
        }
    }

    FileDialog {
        id: videoDialog
        title: "Select Video"
        nameFilters: ["Video files (*.mkv *.mp4 *.avi *.mov)"]
        onAccepted: player.loadVideo(selectedFile)
    }

    FileDialog {
        id: subsDialog
        title: "Select Subs"
        nameFilters: ["Subtitle files (*.srt)"]
        onAccepted: engine.loadSubtitles(selectedFile)
    }
}
