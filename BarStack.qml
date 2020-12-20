import QtQuick 2.0
import QtQuick.Layouts 1.0
Item{
    id:root
    property int textPointSize: 10
    property int minimumBarWidth: 100
    property int preferredBarWidth: 200
    property int itemHeight: 20
    property color barColor: "#70A5CA"
    width: 400
    height: 100
    RowLayout {
        id: row
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        spacing: 3
        ColumnLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true
           // Layout.preferredWidth: rec.width
            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
            id: playerColumn
            Repeater{
              //  model: QuestionFormModel{list: formList}
                model: chartModel
                Item {
                    id:rec
                    width: myText.contentWidth
                    height: myText.contentHeight
                    Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                    Text {
                        id: myText
                        font.family: "Helvetica"
                        font.pointSize: root.textPointSize
                        text:  model.name
                        anchors.fill: parent

                    }
                  }
    }

        }

        ColumnLayout {
            id: chartColumn
            Layout.fillHeight: true

            Repeater{
                model:chartModel
            Item{
                Layout.fillWidth: true
                height: root.itemHeight
                Layout.minimumWidth: root.minimumBarWidth
                Layout.preferredWidth: root.preferredBarWidth
                Rectangle { color: root.barColor; width: parent.width*model.result/100; height: root.itemHeight;}
            }
        }}


        ColumnLayout {
            Layout.fillHeight: true
            Layout.fillWidth: false
           // Layout.preferredWidth: rec.width
            Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
            id: pecentColumn
            Repeater{
                model: chartModel
            Item {
                width: pcText.contentWidth
                height: pcText.contentHeight
                Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter

                Text {
                    id: pcText
                    font.family: "Helvetica"
                    font.pointSize: root.textPointSize
                    text:  model.result.toString() + "%";
                    anchors.fill: parent

                }
              }}

        }
    }
}



/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
