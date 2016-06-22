import QtQuick 2.0
import QuickFlux 1.0
import "../stores"
import "../actions"

StoreWorker {

    Filter {
        // Views do not write to showCompeletedTasks directly.
        // It asks AppActions.setShowCompletedTasks() to do so.
        type: ActionTypes.setShowCompletedTasks
        onDispatched: {
            UserPrefsStore.showCompletedTasks = message.value;
        }
    }


}
