/* StoreAdapter is a utility to initial Store singleton objects and setup dependence
 */
import QtQuick 2.0
import "../stores"

Item {

    Component.onCompleted: {
        TodoStore.waitFor = [UserPrefsStore.listenerId];
    }

}
