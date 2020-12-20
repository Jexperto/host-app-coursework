import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.0
import QtQuick.Layouts 1.0
import QtQuick.Shapes 1.14

    PageBackground {
        id: compForm
        objectName: "quizCompPage"
        property string text: "Test completed"
        //property color backgroundColor: "#ffffff"
        signal formClosed();


                TextContainer {
                            id: endText
                            text: compForm.text
                            anchors.fill: parent
                            anchors.bottomMargin: 309
                            anchors.leftMargin: 137
                            anchors.topMargin: 106
                            anchors.rightMargin: 137
                            backgroundColor: "#ffffff"
                            minimumPointSize: 10
                            fontPointSize: 40
                                            }
                BarStack{
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.top: endText.bottom
                    anchors.horizontalCenterOffset: 0
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.topMargin: 40
                    anchors.leftMargin: 200
                    anchors.rightMargin: 200

                }

        Button {
            id: closeButton
            x: 0
            y: 0
            width: 81
            height: 49
            text: qsTr("Выйти")
            onClicked: compForm.formClosed();
        }





}

//}
/*##^##
Designer {
    D{i:0;autoSize:true;formeditorZoom:0.6600000262260437;height:480;width:640}
}
##^##*/


