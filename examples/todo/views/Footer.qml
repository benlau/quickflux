import QtQuick 2.1
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import org.kde.kirigami 2.7 as Kirigami
import "../actions"

RowLayout {
    id: row

    width: parent.width
    implicitHeight: row.implicitHeight

    spacing: Kirigami.Units.smallSpacing

    Label {
        text: i18n("To do:")

        leftPadding: Kirigami.Units.smallSpacing
        rightPadding: Kirigami.Units.smallSpacing
    }

    Kirigami.ActionTextField {
        id: textField
        Layout.fillWidth: true

        placeholderText: i18n("New task...")

        focus: true
        onAccepted: add();
        rightActions: Kirigami.Action {
            text: i18n("Add")
            // icon from Breeze (breeze-icons) package
            iconName: "list-add"
            visible: textField.text !== ""
            onTriggered: {
                textField.add();
            }
        }
        function add() {
            AppActions.addTask(textField.text);
            textField.text = "";
        }
    }
}
