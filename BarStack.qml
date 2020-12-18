import QtQuick 2.0
import QtQuick.Layouts 1.0
Item {
    id:root
property int height: 20
    RowLayout {
        id: row
        anchors.fill: parent
        spacing: 3
        ColumnLayout {
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            id: playerColumn

            TextContainer {
                id: name
                height: root.height
                backgroundColor: "#ffffff"
                fontPointSize: 30
                text: qsTr("text")
            }
            TextContainer {
                id: name1
                height: root.height
                text: qsTr("text")
                backgroundColor: "#ffffff"
                fontPointSize: 30
            }
            TextContainer {
                id: name2
                height: root.height
                backgroundColor: "#ffffff"
                fontPointSize: 30
                text: qsTr("text")
            }



        }


        ColumnLayout {
            id: chartColumn
            anchors.left: playerColumn.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            Rectangle { color: "red"; width: 50; height: 20 }
            Rectangle { color: "green"; width: 70; height: 20 }
            Rectangle { color: "blue"; width: 80; height: 20 }
        }

    }



}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
