import QtQuick 2.0
import QtTest 1.0
import QuickFlux 1.1

TestCase {
    name : "StoreTests"

    ActionCreator {
        id: defaultActionCreator
        signal test1();
    }

    ActionCreator {
        id: actionCreator1
        dispatcher: Dispatcher {
            id: dispatcher1
        }

        signal test1();
    }

    Store {
        id: store1
        property int test1: 0

        Filter {
            type: "test1"
            onDispatched: {
                store1.test1++;
            }
        }
    }

    function test_dispatch() {

        // Unlike AppListener, Store component do not from AppDispatcher directly.
        compare(store1.bindSource, null);

        store1.dispatch("test1");
        compare(store1.test1, 1);

        defaultActionCreator.test1();
        compare(store1.test1, 1);

        actionCreator1.test1();
        compare(store1.test1, 1); // It is not changed

        store1.bind(actionCreator1);
        compare(store1.bindSource, actionCreator1);

        actionCreator1.test1();
        compare(store1.test1, 2);

        defaultActionCreator.test1();
        compare(store1.test1, 2);

    }


    Store {
        id: store2
        property int test1Count: 0
        property int test2Count: 0
        property var messages: [];

        function test1(message) {
            store2.test1Count++;
            messages.push(message);
        }

        function test2() {
            store2.test2Count++;
        }
    }

    function test_filterFunctionEnabled() {
        var m = {value: 1};
        store2.dispatch("test1", m);
        compare(store2.test1Count, 0);
        store2.filterFunctionEnabled = true;
        store2.dispatch("test1", m);
        compare(store2.test1Count, 1);
        compare(store2.messages[0], m);

        store2.dispatch("test2");
        compare(store2.test2Count, 1);
    }

}

