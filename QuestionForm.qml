import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.0
import QtQuick.Layouts 1.0
import QtQuick.Shapes 1.14


//ApplicationWindow {
//    width: 1280
//    height: 720
//    visible: true
//    title: qsTr("Tabs")

    PageBackground {
        id: pageBackground
        property int numOfRows: 1
        property int numOfCols: 1
        property int timer: 30
        property bool timerRunning: false
        property string question: "Sample text"
        property alias answers: gridview.model
        //anchors.fill: parent
        color: "#f4f4f6"
        colorFirstAccent: "#234068"
        colorSecondAccent: "#e63946"

        Item {
            id: rectangle
            x: 1115
            y: 559
            width: 150
            height: 150
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 11
            anchors.rightMargin: 15

            Canvas {
                id: timerCanvas
                property double partition: 1
                anchors.fill: parent
                onPaint: {
                    var ctx = getContext("2d");
                         ctx.reset();
                          var centreX = width / 2;
                          var centreY = height / 2;
                          var radius = width/3;
                          var startAngle = -Math.PI/2;
                          var endAngle = Math.PI*2;
                          var counterClockwise = false;
                          var firstLineWidth = 1;
                          var secondLineWidth = 7;

                          ctx.beginPath();
                          ctx.arc(centreX, centreY, radius+secondLineWidth/2, startAngle, endAngle-Math.PI/2, counterClockwise);
                          ctx.lineWidth = firstLineWidth;
                          ctx.strokeStyle = pageBackground.colorFirstAccent;
                          ctx.stroke();

                          ctx.beginPath();
                          ctx.arc(centreX, centreY, radius, startAngle, endAngle*partition-Math.PI/2, counterClockwise);
                          ctx.lineWidth = secondLineWidth;
                          ctx.strokeStyle = pageBackground.colorFirstAccent;
                          ctx.stroke();


                }
            }
            Timer{
                signal finished()
                property double initialValue: pageBackground.timer
                property double value: pageBackground.timer
                running: pageBackground.timerRunning
                triggeredOnStart: true
                interval: 50
                repeat: true
                function startTimer(_initialValue){
                    initialValue = _initialValue;
                    value = initialValue;
                    start();
                }
                onTriggered: {
                    if (value<0){
                        stop();
                        text1.text = "0";
                        timerCanvas.partition = 0;
                        timerCanvas.requestPaint();
                        finished();
                        return;
                    }
                    timerCanvas.partition = value/initialValue;
                    text1.text = Math.ceil(value);
                    timerCanvas.requestPaint();
                    value-=interval/1000;
                }

            }
            Text{
                id: text1

                text: "0"
                anchors.verticalCenter: parent.verticalCenter
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.family: "MS Shell Dlg 2"
                font.pointSize: 20
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }


        TextContainer {
            id: questionText
            height: pageBackground.height/3.5
            text: pageBackground.question
            fontPointSize: height/7
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.rightMargin: 197
            anchors.leftMargin: 198
            anchors.topMargin: 68
        }



        GridView {
            id: gridview
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: questionText.bottom
            anchors.bottom: parent.bottom
            anchors.rightMargin: 129
            anchors.leftMargin: 130
            anchors.bottomMargin: 59
            anchors.topMargin: 20

            cellWidth:  width/pageBackground.numOfCols
            cellHeight: pageBackground.numOfCols==1 && pageBackground.numOfRows == 1 ? questionText.height*0.75:height/pageBackground.numOfRows
            focus: true
            //required model

            //highlight: Rectangle { width: gridview.cellWidth; height: gridview.cellHeight; color: "lightsteelblue" }

            delegate: Item {
                id: item1
                width: gridview.cellWidth; height: gridview.cellHeight;


                TextContainer {
                    anchors.topMargin: parent.height/10
                    anchors.bottomMargin: parent.height/10
                    anchors.leftMargin: parent.width/10
                    anchors.rightMargin: parent.width/10
                    text: modelData
                    anchors.fill: parent
                    backgroundColor: "#ffffff"
                    fontPointSize: parent.height/10
                }



            }
        }


}

//}
/*##^##
Designer {
    D{i:0;formeditorZoom:0.6600000262260437}
}
##^##*/


