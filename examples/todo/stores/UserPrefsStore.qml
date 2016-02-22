pragma Singleton
import QtQuick 2.0
import QuickFlux 1.0
import "../actions"

AppListener {

    property bool showCompletedTasks: false

    Filter {
        // Views do not write to showCompeletedTasks directly.
        // It asks AppActions.setShowCompletedTasks() to do so.
        type: ActionTypes.setShowCompletedTasks
        onDispatched: {
            showCompletedTasks = message.value;
        }
    }

}
