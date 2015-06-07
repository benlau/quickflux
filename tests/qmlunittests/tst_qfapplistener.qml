import QtQuick 2.0
import QtTest 1.0
import QuickFlux 1.0

TestCase {
    name : "QFAppListenerBase"
    Item {
        id: container

        QFAppListenerBase {
            id: listener
            target: AppDispatcher
        }
    }


    function test_on() {
        var test1, test2;
        listener.on("test_on_Test1",function(value) {
            test1 = value;
        }).on("test_on_Test2",function(value) {
            test2 = value;
        });

        AppDispatcher.dispatch("test_on_Test1","x");
        compare(test1,"x");
        compare(test2,undefined);

        AppDispatcher.dispatch("test_on_Test2",5676);
        compare(test1,"x");
        compare(test2,5676);
    }

    function test_enabled() {
        var count = 0;
        listener.on("test_enabled",function() {
            count++;
        });

        compare(count,0);
        AppDispatcher.dispatch("test_enabled",null);
        compare(count,1);

        container.enabled = false;
        AppDispatcher.dispatch("test_enabled",null);
        compare(count,1);

        container.enabled = true;
        AppDispatcher.dispatch("test_enabled",null);
        compare(count,2);
    }

}

