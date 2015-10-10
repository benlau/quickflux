import QtQuick 2.0
import QtTest 1.0
import QuickFlux 1.0

TestCase {

    name : "AppListener_alwaysOn"

    property int count1 : 0
    property int count2 : 0

    Item {
        enabled: false
        AppListener {
            id: listener1
            alwaysOn: true
            onDispatched: count1++;
        }
    }

    Item {
        enabled: false
        AppListener {
            id: listener2
            onDispatched: count2++;
        }
    }

    function test_alwayson() {
        var name = "test_filter"
        compare(listener1.enabled,false);
        compare(listener2.enabled,false);
        AppDispatcher.dispatch(name);
        AppDispatcher.dispatch(name);

        compare(count1,2);
        compare(count2,0);

    }


}

