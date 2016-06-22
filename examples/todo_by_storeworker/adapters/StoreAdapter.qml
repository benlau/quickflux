/* StoreAdapter is a utility to initial Store singleton objects and setup dependence
 */
import QtQuick 2.0
import QuickFlux 1.0
import "../stores"
import "../storeworkers"

AppListener {

    Item {
        id: container

        TodoStoreWorker {
        }

        UserPrefsStoreWorker {
        }
    }

    onDispatched: {
        var workers = container.data;

        for (var i in workers) {
            workers[i].dispatched(type, message);
        }
    }

    Component.onCompleted: {
        TodoStore.waitFor = [UserPrefsStore.listenerId];
    }

}
