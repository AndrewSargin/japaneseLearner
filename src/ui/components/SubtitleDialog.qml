import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Dialogs
import JapaneseLearner

Window {

    id: subtitleDialog
    color: ThemeManager.current.bgPanel

    property alias originalSubs: originalSubs
    property alias translationSubs: translationSubs
    property var subtitleTracks: []

    ButtonGroup { id: originalGroup }
    ButtonGroup { id: translationGroup }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 48
            color: "transparent"
            border.color: ThemeManager.current.border
            border.width: 1


            Text {
                anchors.left: parent.left
                anchors.leftMargin: 8
                anchors.verticalCenter: parent.verticalCenter
                anchors.topMargin: 8
                text: "Select Subtitle Tracks"
                color: ThemeManager.current.textPrimary
            }

            Button {
                id: closeButton
                anchors.right: parent.right       
                anchors.rightMargin: 8
                anchors.verticalCenter: parent.verticalCenter

                background: Rectangle {
                    radius: 8
                    color: ThemeManager.current.bgApp
                    border.color: ThemeManager.current.border
                    border.width: 1
                }

                contentItem: Text {
                    text: "X"
                    color: ThemeManager.current.textSecondary
                    font.pixelSize: 12
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }

        Rectangle {

            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "transparent"

            RowLayout {
                anchors.margins: 8                
                anchors.fill: parent

                ColumnLayout {
                    Layout.preferredWidth: 1
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    Text {
                        Layout.alignment: Qt.AlignTop
                        text: "ORIGINAL LANGUAGE"
                        font.pixelSize: 12
                        color: ThemeManager.current.textSecondary
                    }

                    Flickable{
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        contentHeight: originalSubsColumn.height
                        flickableDirection: Flickable.VerticalFlick
                        clip: true

                        Column {
                            id: originalSubsColumn
                            width: parent.width
                            spacing: 4

                            Repeater {
                                id: originalSubs
                                model: engine.subtitleTracks

                                delegate: Rectangle {
                                    width: parent.width
                                    height: 55
                                    radius: 8
                                    color: originalTrackSelected.checked ? ThemeManager.current.bgPanelSelected : "transparent"
                                    border.width: 1
                                    border.color: originalTrackSelected.checked ? ThemeManager.current.accent : "transparent" 

                                    MouseArea {
                                        anchors.fill: parent
                                        onClicked: originalTrackSelected.checked = true
                                    }

                                    RadioButton {
                                        id: originalTrackSelected
                                        anchors.left: parent.left
                                        anchors.verticalCenter: parent.verticalCenter
                                        property int trackIndex: modelData.trackIndex
                                        ButtonGroup.group: originalGroup

                                        indicator: Rectangle {
                                            implicitWidth: 18
                                            implicitHeight: 18
                                            x: originalTrackSelected.leftPadding
                                            y: parent.height / 2 - height / 2
                                            radius: 9
                                            border.color: originalTrackSelected.checked ? ThemeManager.current.accent : ThemeManager.current.textMuted
                                            border.width: 2
                                            color: ThemeManager.current.bgPanel

                                            Rectangle {
                                                width: 10
                                                height: 10
                                                x: parent.width / 2 - width / 2
                                                y: parent.height / 2 - height / 2
                                                radius: 5
                                                color: originalTrackSelected.checked ? ThemeManager.current.accent : ThemeManager.current.textMuted
                                                visible: originalTrackSelected.checked
                                            }
                                        }
                                    }
                                    
                                    Column {

                                        anchors.left: originalTrackSelected.right
                                        anchors.leftMargin: 4
                                        anchors.verticalCenter: parent.verticalCenter

                                        Text {
                                            text: "Track: " + modelData.title
                                            color: ThemeManager.current.textPrimary
                                            font.pixelSize: 14
                                            font.bold: true
                                        }

                                        Row {

                                            Text {
                                                text: modelData.language
                                                color: ThemeManager.current.textMuted
                                                font.pixelSize: 10
                                            }

                                            Text {
                                                text: " • " + modelData.format
                                                color: ThemeManager.current.textMuted
                                                font.pixelSize: 10
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                Rectangle {
                    implicitWidth: 1
                    Layout.fillHeight: true
                    Layout.margins: 8

                    color: ThemeManager.current.border

                }

                ColumnLayout {
                    Layout.preferredWidth: 1
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    Text {
                        Layout.alignment: Qt.AlignTop
                        text: "TRANSLATION"
                        font.pixelSize: 12
                        color: ThemeManager.current.textSecondary
                    }

                    Flickable{
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        contentHeight: translationSubsColumn.height
                        flickableDirection: Flickable.VerticalFlick
                        clip: true

                        Column {
                            id: translationSubsColumn
                            width: parent.width
                            spacing: 4

                            Repeater {
                                id: translationSubs
                                model: engine.subtitleTracks

                                delegate: Rectangle {
                                    width: parent.width
                                    height: 55
                                    radius: 8
                                    color: translationTrackSelected.checked ? ThemeManager.current.bgPanelSelected : "transparent"
                                    border.width: 1
                                    border.color: translationTrackSelected.checked ? ThemeManager.current.accent : "transparent" 

                                    MouseArea {
                                        anchors.fill: parent
                                        onClicked: translationTrackSelected.checked = true
                                    }

                                    RadioButton {
                                        id: translationTrackSelected
                                        anchors.left: parent.left
                                        anchors.verticalCenter: parent.verticalCenter
                                        ButtonGroup.group: translationGroup
                                        property int trackIndex: modelData.trackIndex

                                        indicator: Rectangle {
                                            implicitWidth: 18
                                            implicitHeight: 18
                                            x: translationTrackSelected.leftPadding
                                            y: parent.height / 2 - height / 2
                                            radius: 9
                                            border.color: translationTrackSelected.checked ? ThemeManager.current.accent : ThemeManager.current.textMuted

                                            border.width: 2
                                            color: ThemeManager.current.bgPanel

                                            Rectangle {
                                                width: 10
                                                height: 10
                                                x: parent.width / 2 - width / 2
                                                y: parent.height / 2 - height / 2
                                                radius: 5
                                                color: translationTrackSelected.checked ? ThemeManager.current.accent : ThemeManager.current.textMuted

                                                visible: translationTrackSelected.checked
                                            }
                                        }

                                    }
                                    
                                    Column {

                                        anchors.left: translationTrackSelected.right
                                        anchors.leftMargin: 4
                                        anchors.verticalCenter: parent.verticalCenter

                                        Text {
                                            text: "Track: " + modelData.title
                                            color: ThemeManager.current.textPrimary
                                            font.pixelSize: 14
                                            font.bold: true
                                        }

                                        Row {

                                            Text {
                                                text: modelData.language
                                                color: ThemeManager.current.textMuted
                                                font.pixelSize: 10
                                            }

                                            Text {
                                                text: " • " + modelData.format
                                                color: ThemeManager.current.textMuted
                                                font.pixelSize: 10
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

        Button {
            id: loadExternalSubsButton
            
            Layout.fillWidth: true
            Layout.bottomMargin: 8
            Layout.rightMargin: 8
            Layout.leftMargin: 8

            onClicked: externalSubsDialog.open()

            background: Rectangle {
                radius: 4
                color: ThemeManager.current.border
            }

            contentItem: Text {
                text: "Load External File..."
                color: ThemeManager.current.accent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 48          
            color: ThemeManager.current.bgSurface
            border.color: ThemeManager.current.border
            border.width: 1

            Button {
                id: applyButton
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                anchors.rightMargin: 8
                
                onClicked: {
                    var origIndex = originalGroup.checkedButton ? originalGroup.checkedButton.trackIndex : -1
                    var transIndex = translationGroup.checkedButton ? translationGroup.checkedButton.trackIndex : -1
                    engine.loadSubtitles(origIndex, transIndex)
                    subtitleDialog.close()
                }


                background: Rectangle {
                    radius: 4
                    color: ThemeManager.current.accent
                }

                contentItem: Text {
                    text: "Apply changes"
                    color: ThemeManager.current.accentText
                    font.pixelSize: 12
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter 
                }
            }

            Button {
                id: cancelButton
                anchors.right: applyButton.left
                anchors.verticalCenter: parent.verticalCenter
                anchors.rightMargin: 8

                onClicked: subtitleDialog.close()

                background: Rectangle {
                    radius: 4
                    color: ThemeManager.current.border
                    border.width: 1
                    border.color: ThemeManager.current.textSecondary
                }

                contentItem: Text {
                    text: "Cancel"
                    color: ThemeManager.current.textSecondary
                    font.pixelSize: 12
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }
    }

    FileDialog {
        id: externalSubsDialog
        title: "Select subs"
        nameFilters: ["Subtitle Files (*.srt *.ass)"]
        onAccepted: engine.addExternalSubtitleToList(selectedFile)  //subtitleDialog.subtitleTracks.push(selectedFile)
    }
}
