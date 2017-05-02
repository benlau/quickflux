import QtQuick 2.3
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import App.constants 1.0
import App.adapters 1.0
import App.actions 1.0
import App.stores 1.0

Window {
    id: mainWindow
    visible: false
    width: 480
    height: 640

    StoreAdapter {
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            AppActions.quitApp();
        }
    }

    Text {
        text: qsTr(MainStore.text)
        anchors.centerIn: parent
    }

}
