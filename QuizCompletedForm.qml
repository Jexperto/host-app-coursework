import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.0
import QtQuick.Layouts 1.0
import QtQuick.Shapes 1.14
import QtCharts 2.0

    PageBackground {
        id: compForm
        objectName: "quizCompPage"
        property string text: "Test completed"
        //property color backgroundColor: "#ffffff"
        signal formClosed();


        ChartView {
            id: bar
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.leftMargin: 170
            anchors.rightMargin: 170
            anchors.bottomMargin: 81
            anchors.topMargin: 99
            HorizontalBarSeries {
                name: "BarSeries"
                BarSet {

                    values: [2, 3, 4, 5, 6]
                    label: "Results"
                }

            }
        }
//                TextContainer {
        //                    text: compForm.text
        //                    anchors.fill: parent
        //                    anchors.bottomMargin: 185
        //                    anchors.leftMargin: 48
        //                    anchors.topMargin: 127
        //                    anchors.rightMargin: 48
        //                    backgroundColor: "#ffffff"
        //                    minimumPointSize: 10
        //                    fontPointSize: 40
        //                                    }

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
    D{i:0;autoSize:true;formeditorZoom:0.6600000262260437;height:480;width:640}D{i:1}
}
##^##*/


