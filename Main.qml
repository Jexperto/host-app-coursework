import QtQuick 2.12
import QtQuick.Controls 2.5

ApplicationWindow {
    id:app
    visible: true
    width: 900
    height: 500
//    readonly property color colorMain: "#f4f4f6"
//    readonly property color colorAccentBlue: "#234068"
//    readonly property color colorAccentRed: "#e63946"
    title: qsTr("App")
    Item{
        objectName: "mainPage"
        property string source: "HomeForm.qml"
        anchors.fill: parent
        Loader
        {
            anchors.fill: parent
            id: pageLoader
            source: parent.source}
    }

}




