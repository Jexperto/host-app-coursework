import QtQuick 2.0
import QtQuick.Controls 2.0

Button{
    background: Rectangle {
                implicitWidth: 100
                implicitHeight: 40
                color: down ? "#d6d6d6" : "#f6f6f6"
                border.color: "#26282a"
                border.width: 1
                radius: 1
            }
}

