// To prove that it could pass QImage via AppDispatcher
import QtQuick 2.0
import QtTest 1.0
import QuickFlux 1.0

TestCase {
    name : "FilterTests"

    AppListener {
        id: listener1
        property int count : 0
        property var lastMessage : null;

        Filter {
            type: "testFilter"
            onDispatched: {
                listener1.count++;
                listener1.lastMessage = message;
            }
        }
    }

    function test_filter() {
        compare(listener1.count, 0);
        compare(listener1.lastMessage, null);
        AppDispatcher.dispatch("non-related-action");
        compare(listener1.count, 0);
        compare(listener1.lastMessage, null);

        var obj = {}
        AppDispatcher.dispatch("testFilter", obj);
        compare(listener1.count, 1);
        compare(listener1.lastMessage, obj);

        AppDispatcher.dispatch("testFilter", obj);
        compare(listener1.count, 2);
        compare(listener1.lastMessage, obj);
    }

    Item {
        id: listener2

        signal dispatched(string type, var message);

        property int count : 0
        property string lastType: ""
        property var lastMessage : null;

        Filter {
            type: "testFilterWithItem"
            onDispatched: {
                listener2.count++;
                listener2.lastType = type;
                listener2.lastMessage = message;
            }
        }
    }

    function test_filter_with_item() {
        listener2.dispatched("testFilterWithItem", { a:1, b:"2"});
        compare(listener2.count, 1);
        compare(listener2.lastType,"testFilterWithItem");
        compare(listener2.lastMessage, {a:1, b:"2"});

    }

    AppListener {

        id: listener3
        property int count: 0

        property var messages: ([]);

        Filter {
            id: filter3
            types: ["action1" , "action2"]
            onDispatched: {
                listener3.count++;
                listener3.messages.push(type);
            }
        }
    }

    function test_types() {
        listener3.count = 0;
        AppDispatcher.dispatch("action1");
        compare(listener3.count, 1);
        AppDispatcher.dispatch("action2");
        compare(listener3.count, 2);
        AppDispatcher.dispatch("action3");
        compare(listener3.count, 2);
        compare(listener3.messages, ["action1", "action2"]);
    }

    AppListener {
        id: listener4

        Filter {
            id: filter4

            Connections {
                // Test can a filter object hold children
            }
        }
    }


}

