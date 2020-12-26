import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.0
import QtQuick.Layouts 1.0
import QtQuick.Shapes 1.14


    PageBackground {
        id: pageBackground
        property int numOfRows: 1
        property int numOfCols: 1
        property int timer: 30
        property bool hasImage: false
        property string imagePath: ""
        property bool timerRunning: false
        property string question: "Sample text"
        property alias answers: gridview.model
        signal timerElapsed();
        //anchors.fill: parent
//        color: "#f4f4f6"
//        colorFirstAccent: "#234068"
//        colorSecondAccent: "#e63946"

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
                        timerElapsed();
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
            height: image.enabled ? pageBackground.height/5 : pageBackground.height/4
            text: pageBackground.question
            fontPointSize: 40
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.rightMargin: 197
            anchors.leftMargin: 198
            anchors.topMargin: pageBackground.height/15
        }

        Image {
            id: image
            enabled: pageBackground.hasImage
            fillMode: Image.PreserveAspectFit
            anchors.top: questionText.bottom
            sourceSize.width: pageBackground.width*.3
            anchors.topMargin: pageBackground.height/30
            anchors.horizontalCenter: parent.horizontalCenter
            Component.onCompleted: {
                if (pageBackground.hasImage){
                    try{
                        image.source = pageBackground.imagePath
                    }
                    catch(error){
                        image.enabled = false;
                        pageBackground.hasImage = false;
                    }
                }}
        }


        GridView {
            id: gridview
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: image.enabled ? image.bottom : questionText.bottom
            anchors.bottom: parent.bottom
            anchors.rightMargin: 150
            anchors.leftMargin: 150
            anchors.bottomMargin: pageBackground.height/20
            anchors.topMargin: pageBackground.height/20

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
                    fontPointSize: 30
                }



            }

        }


}

/*##^##
Designer {
    D{i:0;autoSize:true;formeditorZoom:1.100000023841858;height:480;width:640}
}
##^##*/


