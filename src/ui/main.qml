import QtQuick
import QtQuick.Window 2.15
import JapaneseLearner 1.0

Window {
    width: 1280
    height: 720
    visible: true

    MpvWidget{
        id: mpv
        width: parent.width
        height: parent.height
        x: 0
        y: 0

        onPositionChanged: function(time) {
            console.log("time:", time)
            subtitleText.text = engine.getPhraseAtTime(time)
        }

    }

    Text {
        id: subtitleText
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottomMargin: 20
        color: "white"
        font.pixelSize: 24
        text: "s"
    }

    FrameAnimation {
        running: true
        onTriggered: {
            subtitleText.text = engine.getPhraseAtTime(mpv.getCurrentTime())
        }
    }
}
