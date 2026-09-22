import QtQuick
import QtQuick.Layouts
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Dialogs
import JapaneseLearner

Popup {
    id: dropdown

    property var subtitleTracks: ["Track 1", "Track 2"]

    ButtonGroup { id: originalGroup }
    ButtonGroup { id: translationGroup }

    padding: 0
    width: 320

    background: Rectangle {
        radius: 8
        color: ThemeManager.current.bgPanel
    }

    contentItem: Item {
        implicitHeight: mainLayout.implicitHeight

        ColumnLayout {
            id: mainLayout
            anchors.fill: parent
            spacing: 0

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 14
                Layout.topMargin: 8
                Layout.bottomMargin: 8
                color: "transparent"

                RowLayout {
                    anchors.verticalCenter: parent.verticalCenter
                    width: parent.width
                    spacing: 0

                    Text {
                        Layout.leftMargin: 16
                        text: "Select playback and translation tracks"
                        color: ThemeManager.current.textMuted
                        font.pixelSize: 10
                    }

                    Item {
                        Layout.fillWidth: true
                    }

                    Rectangle {
                        Layout.preferredHeight: 8
                        Layout.preferredWidth: 8
                        Layout.rightMargin: 4
                        color: ThemeManager.current.accent
                        radius: 4
                    }

                    Text {
                        Layout.rightMargin: 8
                        text: "Orig."
                        color: ThemeManager.current.textSecondary
                        font.pixelSize: 10
                    }

                    Rectangle {
                        Layout.preferredHeight: 8
                        Layout.preferredWidth: 8
                        Layout.rightMargin: 4
                        color: ThemeManager.current.success
                        radius: 4
                    }

                    Text {
                        Layout.rightMargin: 16
                        text: "Trans."
                        color: ThemeManager.current.textSecondary
                        font.pixelSize: 10
                    }
                }   
            }

            Repeater {
                id: subTracks
                model: engine.subtitleTracks

                delegate: Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: contentRow.implicitHeight
                    Layout.leftMargin: 12
                    Layout.rightMargin: 12
                    Layout.bottomMargin: 4
                    radius: 8
                    border.width: 1
                    border.color: originalTrackSelected.checked ? ThemeManager.current.accent : translationTrackSelected.checked ? ThemeManager.current.success : ThemeManager.current.border
                    color: "transparent"

                    RowLayout {

                        id: contentRow
                        anchors.fill: parent
                        spacing: 0

                        ColumnLayout {

                            Layout.fillHeight: true
                            Layout.margins: 8

                            Text {
                                text: modelData.title + " (" + modelData.format + ")"
                                color: ThemeManager.current.textPrimary
                                wrapMode: Text.Wrap
                                Layout.preferredWidth: contentRow.width - 80
                            }
                            Row {

                                spacing: 8
                                Layout.preferredWidth: langText.implicitWidth + 8 + embextText.implicitWidth

                                Rectangle {
                                    implicitWidth: embextText.width + 8
                                    implicitHeight: embextText.height + 4
                                    color: "transparent"
                                    border.width: 1
                                    border.color: ThemeManager.current.border
                                    radius: 4

                                    Text {
                                        id: embextText
                                        anchors.verticalCenter: parent.verticalCenter
                                        anchors.horizontalCenter: parent.horizontalCenter
                                        anchors.margins: 4
                                        text: modelData.isEmbeded ? "Emb" : "Ext"
                                        font.pixelSize: 10
                                        color: ThemeManager.current.textMuted
                                    }
                                }

                                Rectangle {
                                    implicitWidth: langText.width + 8
                                    implicitHeight: langText.height + 4
                                    color: "transparent"
                                    border.width: 1
                                    border.color: ThemeManager.current.border
                                    radius: 4
                                    visible: langText.text == "" ? false : true

                                    Text {
                                        id: langText
                                        anchors.verticalCenter: parent.verticalCenter
                                        anchors.horizontalCenter: parent.horizontalCenter
                                        anchors.margins: 4
                                        text: modelData.language
                                        font.pixelSize: 10
                                        color: ThemeManager.current.textMuted
                                    }
                                }

                            }
                        }
                        
                        Item {
                            Layout.fillWidth: true 
                        }
                        
                        RadioButton {
                            id: originalTrackSelected

                            implicitHeight: 24
                            implicitWidth: 24

                            Layout.rightMargin: 8

                            ButtonGroup.group: originalGroup
                            property int trackIndex: modelData.trackIndex

                            indicator: Rectangle {
                                id: indicatorOrig
                                implicitWidth: 24
                                implicitHeight: 24
                                anchors.centerIn: parent
                                radius: 12
                                border.color: originalTrackSelected.checked ? ThemeManager.current.accent : ThemeManager.current.border
                                border.width: 1
                                color: originalTrackSelected.checked ? ThemeManager.current.accent : ThemeManager.current.bgSurface
                            }

                            contentItem: Text {
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                                anchors.centerIn: indicatorOrig
                                text: "O"
                                font.pixelSize: 14
                                color: originalTrackSelected.checked ? ThemeManager.current.accentText : ThemeManager.current.textMuted
                            }

                            onClicked: {
                                var origIndex = originalGroup.checkedButton ? originalGroup.checkedButton.trackIndex : -1
                                var transIndex = translationGroup.checkedButton ? translationGroup.checkedButton.trackIndex : -1
                                engine.loadSubtitles(origIndex, transIndex)
                            }
                        }

                        RadioButton {
                            id: translationTrackSelected

                            implicitHeight: 24
                            implicitWidth: 24

                            Layout.rightMargin: 8

                            ButtonGroup.group: translationGroup
                            property int trackIndex: modelData.trackIndex

                            indicator: Rectangle {
                                id: indicatorTrans
                                implicitWidth: 24
                                implicitHeight: 24
                                anchors.centerIn: parent
                                radius: 12
                                border.color: translationTrackSelected.checked ? ThemeManager.current.success : ThemeManager.current.border
                                border.width: 1
                                color: translationTrackSelected.checked ? ThemeManager.current.success : ThemeManager.current.bgSurface
                            }

                            contentItem: Text {
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                                anchors.centerIn: indicatorTrans
                                text: "T"
                                font.pixelSize: 14
                                color: translationTrackSelected.checked ? ThemeManager.current.accentText : ThemeManager.current.textMuted
                            }
                            onClicked: {
                                var origIndex = originalGroup.checkedButton ? originalGroup.checkedButton.trackIndex : -1
                                var transIndex = translationGroup.checkedButton ? translationGroup.checkedButton.trackIndex : -1
                                engine.loadSubtitles(origIndex, transIndex)
                            }
                        }

                    }
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 1
                color: ThemeManager.current.border
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: loadExternal.height + 9
                color: ThemeManager.current.bgSurface
                bottomLeftRadius: 8
                bottomRightRadius: 8

                ButtonStyled {
                    id: loadExternal
                    text: "Load External..."
                    font.pixelSize: 24
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter

                    onClicked: externalSubsDialog.open()
                }
            }
        }

        Rectangle {
            anchors.fill: parent
            color: "transparent"
            border.color: ThemeManager.current.border
            border.width: 1
            radius: 8
        }
    }

        FileDialog {
        id: externalSubsDialog
        title: "Select subs"
        nameFilters: ["Subtitle Files (*.srt *.ass)"]
        onAccepted: engine.addExternalSubtitleToList(selectedFile)  //subtitleDialog.subtitleTracks.push(selectedFile)
    }
}
