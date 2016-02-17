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


}

