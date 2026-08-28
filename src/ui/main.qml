import QtQuick
import QtQuick.Window 2.15
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import QtMultimedia
import JapaneseLearner 1.0

Window {
    id: mainWindow
    width: 1280
    height: 720
    visible: true

    color: ThemeManager.current.bgApp

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
            color: ThemeManager.current.bgPanel

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
                    color: ThemeManager.current.textPrimary
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
                color: ThemeManager.current.bgSurface
                visible: subtitleText.text !== "" || translationText.text !== ""

                Column {
                    id: subtitleColumn
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 10
                    spacing: 4

                    Text {
                        id: subtitleText
                        color: ThemeManager.current.textPrimary
                        font.pixelSize: 24
                        horizontalAlignment: Text.AlignHCenter
                        text: ""
                    }

                    Text {
                        id: translationText
                        color: ThemeManager.current.textPrimary
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
            color: ThemeManager.current.bgSurface
            border.color: ThemeManager.current.border
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
                        color: ThemeManager.current.textSecondary
                        font.pixelSize: 12
                    }

                    Text {
                        id: videoDuration
                        Layout.leftMargin: 8
                        Layout.rightMargin: 8
                        text: "24:24"
                        color: ThemeManager.current.textSecondary
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
                            color: ThemeManager.current.textSecondary
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
            color: ThemeManager.current.bgSurface

            ColumnLayout {
                anchors.fill: parent
                spacing: 8

                Row {

                    Text {
                        text: "Word breakdown"
                        color: ThemeManager.current.textPrimary
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
                                color: ThemeManager.current.bgSurfaceAlt
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
                                            color: ThemeManager.current.textMuted
                                            font.pixelSize: 12
                                            wrapMode: Text.Wrap
                                        }

                                        Text {
                                            text: modelData.reading
                                            color: ThemeManager.current.textMuted
                                            font.pixelSize: 12
                                            wrapMode: Text.Wrap
                                        }

                                        Text {
                                            text: modelData.meaning.replace(/\//g, " / ")
                                            color: ThemeManager.current.textPrimary
                                            font.pixelSize: 12
                                            width: parent.width
                                            wrapMode: Text.Wrap

                                        }

                                        Text {
                                            text: "N" + modelData.jlpt
                                            color: ThemeManager.current.textPrimary
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
    }

    FileDialog {
        id: translationSubsDialog
        title: "Select Subs"
        nameFilters: ["Subtitle files (*.srt *.ass)"]
    }

    SubtitleDialog {
        id: subtitleDialog

        transientParent: mainWindow
        modality: Qt.ApplicationModal

    }

}


