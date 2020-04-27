import QtQuick 2.0
import QtQuick.Controls 2.1
import QuickFlux 1.1
import "../actions"
import "../stores"

Middleware {

    property RootStore store: MainStore

    Dialog {
        id: dialog
        modal: true
        anchors.centerIn: parent

        title: i18n("Confirmation")
        Label {
            text: i18n("Are you sure want to show completed tasks?")
        }
        standardButtons: Dialog.Ok | Dialog.Cancel

        onAccepted: {
            _cleanup();
            next(ActionTypes.setShowCompletedTasks, {value: true});
        }

        onRejected: {
            _cleanup();
            /// Trigger the changed signal even it is unchanged. It forces the checkbox to be turned off.
            store.userPrefs.showCompletedTasksChanged();
        }

        function reparent(item) {
            // attach dialog to the root item, so that it stays centerred
            parent = _findRoot(item)
        }

        // Drills up to the top of component hierarchy.
        function _findRoot(/* Item */ item) {
            if (item === null) {
                return null;
            }
            while (item.parent !== null) {
                item = item.parent;
            }
            return item;
        }

        function _cleanup() {
            // free the parent
            reparent(null);
        }
    }

    function dispatch(type, message) {

        if (type === ActionTypes.setShowCompletedTasks &&
            message.value === true) {
            // If user want to show completed tasks, drop the action and show a dialog
            dialog.reparent(message.contextItem);
            dialog.open();
            return;
        }

        /// Pass the action to next middleware / store
        next(type, message);
    }
}
