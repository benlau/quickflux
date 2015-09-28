pragma Singleton
import QtQuick 2.0
import QuickFlux 1.0

QtObject {

    property string prefix : "TodoActions."

    property string onAdd : prefix + "add"

    // Add a new task
    function add(title) {
        AppDispatcher.dispatch(onAdd,{title: title });
    }

    property string onSetDone: prefix + "setDone"

    function setDone(uid,done) {
        AppDispatcher.dispatch(onSetDone,{uid: uid,done: done })
    }

}

