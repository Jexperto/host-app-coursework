import QtQuick 2.0

Item{
    property color backgroundColor: "#ffffff"
    property color borderColor: "#000000"
    property color textColor: "#000000"
    property string text//: "This is a test text that you should never read in your right mind"
    property int borderWidth: 7
    property int fontPointSize: 20
    height: 191
Rectangle {
    color: parent.backgroundColor
    border.color: parent.borderColor
    border.width: parent.borderWidth
    anchors.fill: parent
    Text {
        wrapMode: Text.Wrap
        font.pointSize: parent.parent.fontPointSize
        anchors.topMargin: 25
        anchors.bottomMargin: 25
        anchors.rightMargin: 42
        anchors.leftMargin: 42
        text: parent.parent.text
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }
  }
}


