import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.15
import QtQuick.Controls.Styles 1.4

PageBackground{
    color: app.colorMain
    colorSecondAccent: app.colorAccentRed
    id: page
    objectName: "homePage"
    colorFirstAccent: app.colorAccentBlue
    signal createRoomClicked();
//    signal loadTestClicked();
//    signal createTestClicked();
//    signal createPageClicked();
//    signal deleteRoomClicked();
//    signal deletePageClicked();
//    signal testEventClicked();
        ColumnLayout {
            id: columnLayout
            y: 190
            width: 254
            height: 322
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenterOffset: 0
            anchors.horizontalCenter: parent.horizontalCenter
            CustomButton {
                id: createRoomButton
                text: qsTr("Создать комнату")
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                onClicked: {page.createRoomClicked();
                    busyIndicator.visible = true;
                }
            }

        }

        CustomBusyIndicator {
            id: busyIndicator
            visible: false
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


    }

/*##^##
Designer {
    D{i:0;autoSize:true;formeditorZoom:1.3300000429153442;height:480;width:640}
}
##^##*/
