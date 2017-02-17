import QtQuick 2.0
import QtTest 1.0
import QuickFlux 1.1

TestCase {
    name : "StoreBridgeTests"

    property var seq: new Array;

    ActionCreator {
        id: defaultActionCreator

        signal test1();
    }

    ActionCreator {
        id: actionCreator

        dispatcher: Dispatcher {}

        signal test1
    }

    Store {
        id: store1

        property int test1: 0

        Filter {
            type: "test1"
            onDispatched: {
                store1.test1++;
                seq.push("store1");
            }
        }
    }

    Store {
        id: store2
        property int test1: 0

        Filter {
            type: "test1"
            onDispatched: {
                store2.test1++;
                seq.push("store2");
            }
        }
    }

    Store {
        id: rootStore
        property int test1: 0

        redispatchTargets: [
            store1,
            store2
        ]

        Filter {
            type: "test1"
            onDispatched: {
                rootStore.test1++;
                seq.push("store");
            }
        }
    }

    function test_dispatch() {
        compare(store1.bindSource, null);
        compare(store2.bindSource, null);

        rootStore.dispatch("test1");
        compare(rootStore.test1, 1);
        compare(store1.test1, 1);
        compare(store2.test1, 1);
        compare(seq,["store1", "store2", "store"]);

        rootStore.bind(actionCreator);

        defaultActionCreator.test1();

        compare(rootStore.test1, 1);
        compare(store1.test1, 1);
        compare(store2.test1, 1);
        compare(seq,["store1", "store2", "store"]);

        actionCreator.test1();

        compare(rootStore.test1, 2);
        compare(store1.test1, 2);
        compare(store2.test1, 2);
        compare(seq,["store1", "store2", "store", "store1", "store2", "store"]);

    }

}

