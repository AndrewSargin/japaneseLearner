import QtQuick
import QtQuick.Window 2.15
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import QtMultimedia
import QtQuick.Controls.impl
import JapaneseLearner 1.0

Window {
    id: mainWindow
    width: 1280
    height: 720
    visible: true

    readonly property string iconPath: "icons/"

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

                spacing: 16

                RowLayout {
                    Layout.leftMargin: 16
                    Layout.rightMargin: 16
                    Layout.fillWidth: true

                    Text {
                        id: currentVideoPosition
                        text: "00:00"
                        color: ThemeManager.current.textSecondary
                        font.pixelSize: 12
                    }

                    Slider {
                        id: videoPosition
                        Layout.fillWidth: true
                        Layout.leftMargin: 8
                        Layout.rightMargin: 8
                        from: 0
                        to: 1
                        onMoved: player.seek(value)

                        background: Rectangle {
                            x: videoPosition.leftPadding
                            y: videoPosition.topPadding + videoPosition.availableHeight / 2 - height / 2
                            radius: 2
                            color: ThemeManager.current.bgApp
                            implicitHeight: 4
                            height: implicitHeight

                            Rectangle {
                                width: videoPosition.visualPosition * parent.width
                                height: parent.height
                                color: ThemeManager.current.accent
                                radius: 2
                            }
                        }

                        handle: Rectangle {
                            x: videoPosition.leftPadding + videoPosition.visualPosition * (videoPosition.availableWidth - width)
                            y: videoPosition.topPadding + videoPosition.availableHeight / 2 - height / 2
                            implicitWidth: 12
                            implicitHeight: 12
                            radius: 6
                        }

                        Connections {
                            target: player

                            function formatTime(totalSeconds) {
                                let total = totalSeconds | 0

                                let h = (total / 3600) | 0
                                let min = ((total % 3600) / 60) | 0
                                let sec = total % 60

                                let mm = String(min).padStart(2, '0')
                                let ss = String(sec).padStart(2, '0')

                                if (h > 0) {
                                    let hh = String(h).padStart(2, '0')
                                    return `${hh}:${mm}:${ss}`
                                }
                                return `${mm}:${ss}`
                            }

                            function onPositionChanged() {
                                var position = player.getPosition()
                                if (!videoPosition.pressed)
                                    videoPosition.value = position
                                currentVideoPosition.text = formatTime(position)

                            }
                            function onDurationChanged() {
                                var duration = player.getDuration()
                                videoPosition.to = duration
                                videoDuration.text = formatTime(duration)                           

                            }
                        }
                    }

                    Text {
                        id: videoDuration
                        text: "00:00"
                        color: ThemeManager.current.textSecondary
                        font.pixelSize: 12
                    }
                }

              
                RowLayout {
                    
                    Layout.leftMargin: 16
                    Layout.rightMargin: 16

                    Button {
                        id: prevButton
                        flat: true
                        background: null

                        icon.source: mainWindow.iconPath + "Prev_btn.svg"
                        icon.width: 12
                        icon.height: 12
                        icon.cache: true
                        icon.color: ThemeManager.current.textMuted
                    }

                    Button {
                        id: playButton
                        flat: true
                        background: null
                       
                        icon.source: mainWindow.iconPath + "Play_btn.svg"
                        icon.width: 36
                        icon.height: 36
                        icon.cache: true
                        icon.color: playButton.hovered ? ThemeManager.current.textMuted : ThemeManager.current.textPrimary

                        onClicked: player.togglePlayPause()
                    }

                    Button {
                        id: nextButton
                        flat: true
                        background: null

                        icon.source: mainWindow.iconPath + "Next_btn.svg"
                        icon.width: 12
                        icon.height: 12
                        icon.cache: true
                        icon.color: ThemeManager.current.textMuted

                    }

                    Rectangle {
                        implicitWidth: 1
                        Layout.fillHeight: true
                        Layout.margins: 8

                        color: ThemeManager.current.border

                    }

                    IconImage {
                        source: mainWindow.iconPath + "Volume_btn.svg"
                        Layout.preferredWidth: 12

                        fillMode: IconImage.PreserveAspectFit
                        color: ThemeManager.current.textMuted
                    }

                    Slider {
                        id: volume
                        Layout.margins: 8
                        from: 0
                        value: 25
                        to: 100
                        onMoved: player.setVolume(value)

                        background: Rectangle {
                            x: volume.leftPadding
                            y: volume.topPadding + volume.availableHeight / 2 - height / 2
                            radius: 2
                            color: ThemeManager.current.bgApp
                            implicitHeight: 4
                            height: implicitHeight
                            implicitWidth: 150
                            width: implicitWidth

                            Rectangle {
                                width: volume.visualPosition * parent.width
                                height: parent.height
                                color: ThemeManager.current.accent
                                radius: 2
                            }
                        }

                        handle: Rectangle {
                            x: volume.leftPadding + volume.visualPosition * (volume.availableWidth - width)
                            y: volume.topPadding + volume.availableHeight / 2 - height / 2
                            implicitWidth: 12
                            implicitHeight: 12
                            radius: 6
                        }
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


