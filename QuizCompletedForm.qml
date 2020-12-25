import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.0
import QtQuick.Layouts 1.0
import QtQuick.Shapes 1.14

    PageBackground {
        id: compForm
        objectName: "quizCompPage"
        property string text: "Тест завершён"
        //property color backgroundColor: "#ffffff"
        signal formClosed();


                TextContainer {
                            id: endText
                            height: compForm.height/8
                            text: compForm.text
                            anchors.left: parent.left
                            anchors.right: parent.right
                            anchors.top: parent.top
                            anchors.leftMargin: 137
                            anchors.topMargin: compForm.height/7
                            anchors.rightMargin: 137
                            backgroundColor: "#ffffff"
                            minimumPointSize: 10
                            fontPointSize: 40
                                            }

                BarStack{
                    color: "white"
                    border.color: "black"
                    border.width: 3
                    //textPointSize: 20
                  //  width: 100

                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.top: endText.bottom
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin:  (count > 0 && parent.height/count*1.2 > 20) ? (parent.height/count*1.2 < parent.height/2 ? parent.height/count*1.2 : parent.height/2 ) : 20
                    anchors.topMargin: count < 10 ? endText.anchors.topMargin/3 : endText.anchors.topMargin/6
                    anchors.leftMargin: 200
                    anchors.rightMargin: 200
                    barColor: compForm.colorFirstAccent



                    //anchors.fill:parent
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


