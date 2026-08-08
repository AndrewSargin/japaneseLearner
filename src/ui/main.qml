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

    color: "#09090b"

    ColumnLayout {

        anchors.fill: parent
        spacing: 0

        Rectangle {
            id: topBar
            Layout.fillWidth: true
            Layout.preferredHeight: 40
            color: "#121214"

            Row {
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                anchors.leftMargin: 10
                spacing: 8
                            
                Button {
                    id: settingsButton 
                    text: "⚙"
                    height: 24 
                    width: 24
                    
                    contentItem: Text {                       
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        text: settingsButton.text
                        color: "#a1a1aa"
                        font.pixelSize: 12
                    }

                    background: Rectangle {
                        radius: 4
                        color: "#18181B"
                        border.color: "#27272a"
                        border.width: 1
                    }
                }
            }

            Row {
                anchors.centerIn: parent
                spacing: 10

                Button {
                    id: loadVideoButton
                    text: "Load Video"

                    contentItem: Text {
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        text: loadVideoButton.text
                        color: "#f4f4f5"
                        font.pixelSize: 12
                    }

                    background: Rectangle {
                        radius: 4
                        color: "#18181b"
                        border.color: "#27272a"
                        border.width: 1

                    }
                    onClicked: videoDialog.open()
                }
                Button {
                    id: loadOriginalSubsButton
                    text: "Load Subs (Original)"

                    contentItem: Text {
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        text: loadOriginalSubsButton.text
                        color: "#f4f4f5"
                        font.pixelSize: 12
                    }

                    background: Rectangle {
                        radius: 4
                        color: "#18181b"
                        border.color: "#27272a"
                        border.width:1
                    }
                    onClicked: subsDialog.open()
                }
                Button {
                    id: loadTranslationSubsButton
                    text: "Load Subs (Translation)"

                    contentItem: Text {
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        text: loadTranslationSubsButton.text
                        color: "#f4f4f5"
                        font.pixelSize: 12
                    }

                    background: Rectangle {
                        radius: 4
                        color: "#18181b"
                        border.color: "#27272a"
                        border.width: 1
                    }
                }

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
            Layout.preferredHeight: 100
            Layout.topMargin: 16
            Layout.leftMargin: 16
            Layout.rightMargin: 16
            Layout.bottomMargin: 16
            radius: 4
            color: "#18181b"
            border.color: "#27272a"
            border.width: 1

            ColumnLayout {
                anchors.fill: parent
                anchors.topMargin: 16
                anchors.bottomMargin: 16

                spacing: 8

                RowLayout {

                    Text {
                        id: currentVideoPosition
                        Layout.leftMargin: 8
                        text: "00:00"
                        color: "#a1a1aa"
                        font.pixelSize: 12
                    }

                    Slider {
                        id: videoPosition

                        Layout.fillWidth: true
                        Layout.leftMargin: 8
                        from: 0
                        value: player.position
                        to: player.duration

                        onMoved: player.seek(value)
                    }

                    Text {
                        id: videoDuration
                        Layout.leftMargin: 8
                        Layout.rightMargin: 8
                        text: "24:24"
                        color: "#a1a1aa"
                        font.pixelSize: 12
                    }
                }

                RowLayout {

                    Button {
                        id: playButton
                        text: "▶"
                        font.pixelSize: 24
                        implicitWidth: 36
                        implicitHeight: 36

                        background: Rectangle {
                            radius: parent.implicitWidth / 2
                            color: "#a1a1aa"
                        }
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
            color: "#18181b"

            ColumnLayout {
                anchors.fill: parent
                spacing: 8

                Row {
                    
                    Text {
                        text: "Word breakdown"
                        color: "#f4f4f5"
                    }

                }

                Row {

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
