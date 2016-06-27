Todo List Example (Rewritten with using storeworker)
=================

There has a problem in writing QML in an Flux way. That is the store to store dependence and data binding. Due to the [QTBUG-49370](https://bugreports.qt.io/browse/QTBUG-49370), access data from a singleton object to another singleton object in the same package may hang.

A workaround solution is to use an StoreAdapter out of stores package to initialize and setup stores :

```
StoreAdapter {
 Binding {
   target: BStore
   property: "aStoreValue1"
   value: AStore.value1
 }

 Binding {
   target: BStore
   property: "aStoreValue2"
   value: AStore.value2
 }

 Component.onCompleted : {
    BStore.waitFor = [AStore.listenerId];
 }
}
```

Obviously the StoreAdapter is not an ideal solution for this problem. As you could see it need to declare extra properties to hold a value from depended store. And the value is not initialized until the construction of StoreAdapter is completed.

In this example, it will introduce another concept called StoreWorker to solve this problem.

StoreWorker
============

The core concept of Quick Flux application is the separation of queries and updates. Although view components read from a store and render the content. It won’t update it directly. Instead, it asks Action to do so.

In order to follow the rule, store component in Quick Flux listens actions from AppDispatcher and update itself. An typical store component may look like this:

```
pragma Singleton
import QtQuick 2.0
import QuickFlux 1.0

AppListener {
  id : store

  property int value1 : 0
  property int value2 : 0

  Filter {
    type: ActionTypes.addValue1
    onDispatched: value1++;
  }
}
```

Since the current problem is the update logic can not access other store in the same package. A simple and direct solution will be : Move the update logic to another package. And that is how StoreWorker works.

StoreWorker is a kind of component that allow to query and update stores. As it is located in another package, it has no problem to access multiple stores.

File Structure

```
  /stores/AStore.qml
  /stores/BStore.qml
  /stores/qmldir # Register singleton component
  /storeworkers/AStoreWorker.qml
  /storeworkers/BStoreWorker.qml
  /storeworkers/StoreWorker.qml
```

StoreWorker.qml

```
import QtQuick 2.0

QtObject {
    id: worker

    signal dispatched(string type, var message);

    default property alias children: worker.__children

    property list<QtObject> __children: [QtObject {}]

}
```

AStoreWorker.qml:

```
StorkWorker {
  Filter {
    type: ActionTypes.addValue1
    onDispatched: AStore.value1++;
  }
}
```

AStore.qml - Pure data class

```
QtObject: {
  // Update logic has been moved to worker.
  property int value1 : 0
  property int value2 : 0
}
```

However, you still need a StoreAdapter to initialize store and store workers:

```
StoreAdapter {

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
}
```

Conclusion
==========

The benefit of using StoreWorker

 * A solution for QTBUG-49370
 * Update logic is centralized in a place and it could update multiple store in a single process.
 * You could control the order of message delivery to StoreWorker by an sequential array

In fact, StoreWorker is inspired by the Redux framework. It modify the original Flux application structure by reducing the no. of stores into one. And it takes out update logic from store to a set of component called reducer.

StoreWorker is somehow similar to the role of reducer. Both of them hold the update logic. The different is a reducer take a state object (data model in store) as input and return a new state object. It don’t update store directly. Multiple reducer is usually chained and only the final result will be taken to update store. Therefore, StoreWorker is not same as reducer.
