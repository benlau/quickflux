import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import org.kde.kirigami 2.0 as Kirigami
import "../actions"
import "../stores"

// It fills width of parent, and uses as much height as needed.
RowLayout {
    width: parent.width

    Label {
        text: i18n("Items:")
        leftPadding: Kirigami.Units.smallSpacing
        rightPadding: Kirigami.Units.smallSpacing
    }

    CheckBox {
        id: checkBox
        Layout.alignment: Qt.AlignRight | Qt.AlignVCenter

        text: i18n("Show Completed");
        checked: MainStore.userPrefs.showCompletedTasks
        onCheckedChanged: {
            AppActions.setShowCompletedTasks(checked, this);
        }
    }
}
