import QtQuick
import QtQuick.Window 2.15
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import QtMultimedia
import "components"
import JapaneseLearner 1.0

Window {
    id: mainWindow
    width: 1280
    height: 720
    visible: true

    color: "#09090b"

    Connections {
        target: player
        function onPositionChanged() {
            var phrases = engine.getPhraseAtTime(player.getPosition())
            var newSubs = phrases[0] ?? ""
            if (newSubs !== subtitleText.text) {
                subtitleText.text = newSubs
                translationText.text = phrases[1] ?? ""
                wordList.model = engine.getWordsAtTime(player.getPosition())

            }
        }
    }

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

                ButtonStyled {
                    id: settingsButton 
                    text: "⚙"
                    height: 24 
                    width: 24
                }
            }

            Row {
                anchors.centerIn: parent
                spacing: 10

                ButtonStyled {
                    id: loadVideoButton
                    text: "Load Video"
                    onClicked: videoDialog.open()
                }
                ButtonStyled {
                    id: loadOriginalSubsButton
                    text: "Load Subs (Original)"
                    onClicked: subsDialog.open()
                }
                ButtonStyled {
                    id: loadTranslationSubsButton
                    text: "Load Subs (Translation)"
                    onClicked: translationSubsDialog.open()
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

        Item{
            Layout.fillWidth: true
            Layout.fillHeight: true

            VideoPlayer {
                id: player
                videoOutput: videoOutput
            }

            VideoOutput {
                id: videoOutput
                anchors.fill: parent
            }

            Rectangle {
                anchors.bottom: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                width: subtitleColumn.width + 40
                height: subtitleColumn.height + 20
                color: "#8018181b"
                visible: subtitleText.text !== "" || translationText.text !== ""

                Column {
                    id: subtitleColumn
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 10
                    spacing: 4

                    Text {
                        id: subtitleText
                        color: "white"
                        font.pixelSize: 24
                        horizontalAlignment: Text.AlignHCenter
                        text: ""
                    }

                    Text {
                        id: translationText
                        color: "white"
                        horizontalAlignment: Text.AlignHCenter
                        font.pixelSize: 18
                        text: ""
                    }
                }
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

                    Text {
                        id: videoDuration
                        Layout.leftMargin: 8
                        Layout.rightMargin: 8
                        text: "24:24"
                        color: "#a1a1aa"
                        font.pixelSize: 12
                    }
                }

                Slider {
                    id: videoPosition
                    Layout.fillWidth: true
                    Layout.leftMargin: 8
                    from: 0
                    to: 1
                    onMoved: player.seek(value)

                    Connections {
                        target: player
                        function onPositionChanged() {
                            if (!videoPosition.pressed)
                            videoPosition.value = player.getPosition()
                        }
                        function onDurationChanged() {
                            videoPosition.to = player.getDuration()

                        }
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

                Flickable {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    contentWidth: wordRow.width
                    flickableDirection: Flickable.HorizontalFlick
                    clip: true

                    Row {
                        id: wordRow
                        spacing: 8 

                        Repeater {
                            id: wordList
                            model: []

                            delegate: Rectangle {
                                height: 130
                                width: 120
                                color: "#2a2a2a"
                                radius: 8
                                clip: true

                                Flickable {

                                    anchors.fill: parent
                                    anchors.margins: 8
                                    contentHeight: wordColumn.height
                                    flickableDirection: Flickable.VerticalFlick

                                    Column {
                                        id: wordColumn
                                        spacing: 4
                                        width: parent.width

                                        Text {
                                            text: modelData.kanji
                                            color: "#aaaaaa"
                                            font.pixelSize: 12
                                            wrapMode: Text.Wrap
                                        }

                                        Text {
                                            text: modelData.reading
                                            color: "#aaaaaa"
                                            font.pixelSize: 12
                                            wrapMode: Text.Wrap
                                        }

                                        Text {
                                            text: modelData.meaning.replace(/\//g, " / ")
                                            color: "white"
                                            font.pixelSize: 12
                                            width: parent.width
                                            wrapMode: Text.Wrap

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
        }
    }

    FileDialog {
        id: videoDialog
        title: "Select Video"
        nameFilters: ["Video files (*.mkv *.mp4 *.avi *.mov)"]
        onAccepted: {
            var path = selectedFile.toString().replace("file://", "")
            player.loadVideo(path)
            engine.prepareVideo(selectedFile)
            subtitleDialog.show()            
        }
    }

    FileDialog {
        id: subsDialog
        title: "Select Subs"
        nameFilters: ["Subtitle files (*.srt *.ass)"]
        onAccepted: engine.loadOriginalSubtitles(selectedFile)
    }

    FileDialog {
        id: translationSubsDialog
        title: "Select Subs"
        nameFilters: ["Subtitle files (*.srt *.ass)"]
        onAccepted: engine.loadTranslationSubtitles(selectedFile)
    }

    SubtitleDialog {
        id: subtitleDialog

        transientParent: mainWindow
        modality: Qt.ApplicationModal

    }

}


