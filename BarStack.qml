import QtQuick 2.0
import QtQuick.Layouts 1.0
Rectangle{
    id:root
    property int minimumBarWidth: 100
    property int preferredBarWidth: 200

    property color barColor: "#70A5CA"
    property int count: 0
    width: 400
   // height: 100
    GridLayout {
        id: grid
        property int count: 1
        anchors.fill: parent
        columns: 3
        rowSpacing:  root.height/(count*5) > 5 ?  root.height/(count*5) : 5
        columnSpacing: 10
        anchors.margins: root.height/(count*2) > 5 ? root.height/(count*2) : 5
          //property double rowMulti : root

            Repeater{
                id: rep
                model: chartModel
                onCountChanged: if (count>0) {parent.count = count; root.count = count}
                Item {
                    id:rec
                    Layout.rowSpan: 1
                    Layout.preferredWidth:myText.contentWidth;
                    Layout.minimumHeight: root.height/(4*grid.count)
                    Layout.fillHeight: true
                    //Layout.preferredHeight : (grid.height-(grid.anchors.margins*2))/model.count//grid.prefHeight(this)
                    Layout.column: 0
                    Layout.row: index
                    Layout.alignment: Qt.AlignRight
//                    width: myText.contentWidth
//                    height: myText.contentHeight
                    Text {
                        id: myText

                        font.family: "Helvetica"
                        font.pixelSize: parent.height*0.8
                        text:  model.name
                        anchors.fill: parent
                        horizontalAlignment: Text.AlignRight
                        verticalAlignment: Text.AlignVCenter

                    }
                  }
    }


            Repeater{
                model:chartModel
            Item{
                Layout.fillHeight: true
                Layout.rowSpan: 1
                Layout.fillWidth: true
                Layout.minimumHeight: root.height/(4*grid.count)
              //  Layout.preferredHeight : (grid.height-(grid.anchors.margins*2))/model.count//grid.prefHeight(this)
                Layout.column: 1
                Layout.row: index
                //height: root.height/model.count
                Layout.minimumWidth: root.minimumBarWidth
                //Layout.preferredWidth: root.preferredBarWidth
                Rectangle {color: root.barColor; width: parent.width*model.result/100; height: parent.height}
            }
        }

            Repeater{
                model: chartModel
            Item {
                Layout.fillHeight: true
                Layout.rowSpan: 1
                Layout.preferredWidth:pcText.contentWidth;
                    Layout.minimumHeight: root.height/(4*grid.count)
               // Layout.preferredHeight : (grid.height-(grid.anchors.margins*2))/model.count//grid.prefHeight(this)
                Layout.column: 2
                Layout.row: index
//                width: pcText.contentWidth
//                height: pcText.contentHeight
                Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter

                Text {
                    id: pcText
                    font.family: "Helvetica"
                    font.pixelSize: parent.height*0.8
                    text:  model.result.toString() + "%";
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter

                }
              }

        }
    }
}




