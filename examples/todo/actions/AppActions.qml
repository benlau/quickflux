pragma Singleton
import QtQuick 2.0
import QuickFlux 1.1

ActionCreator {

    // Add a new task
    signal addTask(string title);

    // Set/unset done on a task
    signal setTaskDone(int uid, bool done)

    // Show/hide completed task
    signal setShowCompletedTasks(bool value, Item contextItem)

}
