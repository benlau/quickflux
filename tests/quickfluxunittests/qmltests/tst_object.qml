import QtQuick 2.0
import QtTest 1.0
import QuickFlux 1.1

TestCase {
    name : "ObjectTests"

    Object {
        id: object
        property int count: 0
        AppListener {
            onDispatched: {
                object.count++;
            }
        }
    }

    function test_object() {
        compare(object.count, 0);
        AppDispatcher.dispatch("test",{});
        compare(object.count, 1);
    }


}

