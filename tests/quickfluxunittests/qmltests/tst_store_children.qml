import QtQuick 2.0
import QtTest 1.0
import QuickFlux 1.1

TestCase {
    name : "StoreChildrenTests"

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
        id: store
        property int test1: 0

        Store {
            id: child1

            property int test1: 0

            Filter {
                type: "test1"
                onDispatched: {
                    child1.test1++;
                    seq.push("child1");
                }
            }
        }

        Store {
            id: child2
            property int test1: 0

            Filter {
                type: "test1"
                onDispatched: {
                    child2.test1++;
                    seq.push("child2");
                }
            }
        }

        Filter {
            type: "test1"
            onDispatched: {
                store.test1++;
                seq.push("store");
            }
        }
    }

    function test_dispatch() {
        compare(child1.bindSource, null);
        compare(child2.bindSource, null);

        store.dispatch("test1");
        compare(store.test1, 1);
        compare(child1.test1, 1);
        compare(child2.test1, 1);
        compare(seq,["child1", "child2", "store"]);

        store.bind(actionCreator);

        defaultActionCreator.test1();

        compare(store.test1, 1);
        compare(child1.test1, 1);
        compare(child2.test1, 1);
        compare(seq,["child1", "child2", "store"]);

        actionCreator.test1();

        compare(store.test1, 2);
        compare(child1.test1, 2);
        compare(child2.test1, 2);
        compare(seq,["child1", "child2", "store", "child1", "child2", "store"]);

    }

}

