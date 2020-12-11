import QtQuick 2.2
import QtQuick.Dialogs 1.0

FileDialog {
    id: fileDialog
    objectName: "fileDialog"
    title: qsTr("Выберите файл...")
    folder: shortcuts.home
    //selectMultiple: true
    signal choseFile(string dir)
    onAccepted: {
        choseFile(fileDialog.fileUrls)
        console.log("You chose: " + fileDialog.fileUrls)
    }
    onRejected: {
        console.log("Canceled File Dialog")
    }
    Component.onCompleted: visible = true
}
