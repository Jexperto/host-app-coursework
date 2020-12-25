import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.0


PageBackground{
    property string codeText: "----"
    property bool startTestButtonVisible: false
    property bool busyIndicatorVisible: false

    color: app.colorMain
    colorSecondAccent: app.colorAccentRed
    id: page
    objectName: "roomCreatedPage"
    colorFirstAccent: app.colorAccentBlue
    signal loadTestClicked();
    signal deleteRoomClicked();
    signal createTestClicked();
    signal startTestClicked();
    signal refreshClicked();
        ColumnLayout {
            id: columnLayout
            y: 190
            width: 254
            height: 156
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenterOffset: 0
            anchors.horizontalCenter: parent.horizontalCenter


            CustomButton {
                visible: startTestButtonVisible
                id: startTest
                text: qsTr("Начать тест")
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                onClicked: page.startTestClicked()
            }

            CustomButton {
                id: deleteRoomButton
                text: qsTr("Закрыть комнату")
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                onClicked: page.deleteRoomClicked()
            }
//            CustomButton {
//                id: createPageButton
//                text: qsTr("Создать страницу")
//                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
//                onClicked: page.createPageClicked()
//            }
//            CustomButton {
//                id: deletePageButton
//                text: qsTr("Удалить страницу")
//                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
//                onClicked: page.deletePageClicked()
//            }
//            CustomButton {
//                id: testEventButton
//                text: qsTr("Отправить тестовое событие")
//                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
//                onClicked: page.testEventClicked()
//            }

            CustomButton {
                id: loadTestButton
                text: qsTr("Загрузить тест")
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                onClicked: page.loadTestClicked()
            }

            CustomButton {
                id: createTestButton
                text: qsTr("Создать тест")
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                onClicked: page.createTestClicked()
            }
            CustomButton {
                id: refreshButton
                text: qsTr("Обновить")
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                onClicked: page.refreshClicked()
            }
        }


        CustomBusyIndicator {
            id: busyIndicator
            visible: busyIndicatorVisible
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.leftMargin: 40
            anchors.topMargin: 40

            bLength: 40
            bWidth: 30
            bRadius: 50

            bOpacity: 0.7
            bColor: page.color
            bHighlightColor: page.colorFirstAccent
            bBgColor: "transparent"
            bSpeed: 150
            width: 40
            height: 40

        }

        Item{
            id:codeItem
            width: 150
            height: 52
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.topMargin: 20
            anchors.rightMargin: 30

        ColumnLayout{
            anchors.fill: parent
            Label {
                width: contentWidth
                Layout.fillHeight: true
               // Layout.fillWidth: true
                font.pointSize: codeItem.height/4
                id: label
                text: qsTr("Код комнаты:")
                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
        }
        Label{
            width: contentWidth
            Layout.fillHeight: true
           // Layout.fillWidth: true
            font.pointSize: codeItem.height/4
            text: page.codeText
            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
        }
        }}



    }

/*##^##
Designer {
    D{i:0;autoSize:true;formeditorZoom:1.100000023841858;height:480;width:640}
}
##^##*/
