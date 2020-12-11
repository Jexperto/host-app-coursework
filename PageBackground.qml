import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Controls.Universal 2.0

Page{
    id: page
    property color color: "#f1faee"
    property color colorFirstAccent: "#1d3557"
    property color colorSecondAccent: "#e63946"
Rectangle {
    id: background
    Universal.theme: Universal.Light
    Universal.accent: page.colorFirstAccent
    anchors.fill: parent
    color: page.color
    clip: true

    Rectangle {
        id: blueBase
        color: page.colorFirstAccent
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.topMargin: 0
        anchors.bottomMargin: 0
        anchors.leftMargin: 290
        transformOrigin: Item.Top
        anchors.rightMargin: 290
        clip: false
        rotation: 0

        Rectangle {
            id: redStripe2
            x: 592
            width: blueBase.width/7
            color: page.colorSecondAccent
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.rightMargin: 0
            anchors.topMargin: 0
            anchors.bottomMargin: 0

        }

        Rectangle {
            id: redStripe1
            width: blueBase.width/7
            color: colorSecondAccent
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.leftMargin: 0
            anchors.topMargin: 0
            anchors.bottomMargin: 0

        }

    }
}
}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.75}D{i:2}D{i:3}
}
##^##*/
