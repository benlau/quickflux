pragma Singleton
import QtQuick 2.0
import QuickFlux 1.0
import "./"

QtObject {

    // Add a new task
    function add(title) {
        AppDispatcher.dispatch(ActionTypes.todo_add,{title: title });
    }

    function setDone(uid,done) {
        AppDispatcher.dispatch(ActionTypes.todo_setDone,{uid: uid,done: done })
    }

    function showCompleted(value) {
        AppDispatcher.dispatch(ActionTypes.todo_showCompleted,{value: value })
    }

}

