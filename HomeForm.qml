import QtQuick 2.0
import QtQuick.Layouts 1.0

PageBackground{
    color: app.colorMain
    colorSecondAccent: app.colorAccentRed
    id: page
    objectName: "homePage"
    colorFirstAccent: app.colorAccentBlue
    signal loadTestClicked();
    signal createTestClicked();
    signal createRoomClicked();
    signal testEventClicked();
        ColumnLayout {
            id: columnLayout
            x: 138
            y: 190
            width: 254
            height: 100
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            CustomButton {
                id: createRoomButton
                text: qsTr("Создать комнату")
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                onClicked: page.createRoomClicked()
            }
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
                id: testEventButton
                text: qsTr("Отправить тестовое событие")
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                onClicked: page.testEventClicked()
            }

        }


    }
