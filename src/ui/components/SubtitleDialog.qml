import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Window {

    id: subtitleDialog
    color: "#121214"

    property alias originalSubs: originalSubs
    property alias translationSubs: translationSubs

    ButtonGroup { id: originalGroup }
    ButtonGroup { id: translationGroup }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 48
            color: "transparent"

            Text {
                anchors.left: parent.left
                anchors.leftMargin: 8
                anchors.verticalCenter: parent.verticalCenter
                anchors.topMargin: 8
                text: "Select Subtitle Tracks"
                color: "#f4f4f5"
            }

            Button {
                id: closeButton
                anchors.right: parent.right       
                anchors.rightMargin: 8
                anchors.verticalCenter: parent.verticalCenter

                background: Rectangle {
                    radius: 8
                    color: "#09090b"
                    border.color: "#27272a"
                    border.width: 1
                }

                contentItem: Text {
                    text: "X"
                    color: "#a1a1aa"
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
            border.color: "#27272a"
            border.width: 1

            RowLayout {
                anchors.fill: parent
                anchors.margins: 8

                ColumnLayout {
                    Layout.preferredWidth: 1
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    Text {
                        Layout.alignment: Qt.AlignTop
                        text: "ORIGINAL LANGUAGE"
                        font.pixelSize: 12
                        color: "#a1a1aa"
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
                                model: []

                                delegate: Rectangle {
                                    width: parent.width
                                    height: 55
                                    color: "transparent"

                                    MouseArea {
                                        anchors.fill: parent
                                        onClicked: originalTrackSelected.checked = true
                                    }

                                    RadioButton {
                                        id: originalTrackSelected
                                        anchors.left: parent.left
                                        anchors.verticalCenter: parent.verticalCenter
                                        property int trackIndex: index
                                        ButtonGroup.group: originalGroup

                                        indicator: Rectangle {
                                            implicitWidth: 18
                                            implicitHeight: 18
                                            x: originalTrackSelected.leftPadding
                                            y: parent.height / 2 - height / 2
                                            radius: 9
                                            border.color: originalTrackSelected.checked ? "#38bdf8" : "#71717a"
                                            border.width: 2
                                            color: "#121214"

                                            Rectangle {
                                                width: 10
                                                height: 10
                                                x: parent.width / 2 - width / 2
                                                y: parent.height / 2 - height / 2
                                                radius: 5
                                                color: originalTrackSelected.checked ? "#38bdf8" : "#71717a"
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
                                            color: "#f4f4f5"
                                            font.pixelSize: 14
                                            font.bold: true
                                        }

                                        Row {

                                            Text {
                                                text: modelData.language
                                                color: "#71717a"
                                                font.pixelSize: 10
                                            }

                                            Text {
                                                text: " • " + modelData.format
                                                color: "#71717a"
                                                font.pixelSize: 10
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    
                    Button {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 32
                        Layout.alignment: Qt.AlignBottom
                        
                        background: Rectangle {
                            radius: 4
                            color: "#27272a"
                        }
                        
                        contentItem: Text {
                            text: "Load External Files..."
                            color: "#38bdf8"
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                    }
                }

                Rectangle {
                    implicitWidth: 1
                    Layout.fillHeight: true
                    Layout.margins: 8
                    color: "#27272a"

                }

                ColumnLayout {
                    Layout.preferredWidth: 1
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    Text {
                        Layout.alignment: Qt.AlignTop
                        text: "TRANSLATION"
                        font.pixelSize: 12
                        color: "#a1a1aa"
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
                                model: []

                                delegate: Rectangle {
                                    width: parent.width
                                    height: 55
                                    color: "transparent"

                                    MouseArea {
                                        anchors.fill: parent
                                        onClicked: translationTrackSelected.checked = true
                                    }

                                    RadioButton {
                                        id: translationTrackSelected
                                        anchors.left: parent.left
                                        anchors.verticalCenter: parent.verticalCenter
                                        ButtonGroup.group: translationGroup
                                        property int trackIndex: index

                                        indicator: Rectangle {
                                            implicitWidth: 18
                                            implicitHeight: 18
                                            x: translationTrackSelected.leftPadding
                                            y: parent.height / 2 - height / 2
                                            radius: 9
                                            border.color: translationTrackSelected.checked ? "#38bdf8" : "#71717a"
                                            border.width: 2
                                            color: "#121214"

                                            Rectangle {
                                                width: 10
                                                height: 10
                                                x: parent.width / 2 - width / 2
                                                y: parent.height / 2 - height / 2
                                                radius: 5
                                                color: translationTrackSelected.checked ? "#38bdf8" : "#71717a"
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
                                            color: "#f4f4f5"
                                            font.pixelSize: 14
                                            font.bold: true
                                        }

                                        Row {

                                            Text {
                                                text: modelData.language
                                                color: "#71717a"
                                                font.pixelSize: 10
                                            }

                                            Text {
                                                text: " • " + modelData.format
                                                color: "#71717a"
                                                font.pixelSize: 10
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }


                    Button {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 32
                        Layout.alignment: Qt.AlignBottom
                        background: Rectangle {
                            radius: 4
                            color: "#27272a"
                        }

                        contentItem: Text {
                            text: "Load External File..."
                            color: "#38bdf8"
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                    }
                }
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 48          
            color: "#18181b"

            Button {
                id: applyButton
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                anchors.rightMargin: 8
                
                onClicked: {
                    var origIndex = originalGroup.checkedButton ? originalGroup.checkedButton.trackIndex : -1
                    var transIndex = translationGroup.checkedButton ? translationGroup.checkedButton.trackIndex : -1
                    //player.loadSubtitles(origIndex, transIndex)
                    subtitleDialog.close()
                }


                background: Rectangle {
                    radius: 4
                    color: "#38BDF8"
                }

                contentItem: Text {
                    text: "Apply changes"
                    color: "black"
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

                background: Rectangle {
                    radius: 4
                    color: "#27272a"
                    border.width: 1
                    border.color: "#a1a1aa"
                }

                contentItem: Text {
                    text: "Cancel"
                    color: "#a1a1aa"
                    font.pixelSize: 12
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }
    }
}
