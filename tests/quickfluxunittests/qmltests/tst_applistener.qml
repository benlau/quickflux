import QtQuick 2.0
import QtTest 1.0
import QuickFlux 1.0

TestCase {

    name : "AppListener"

    Item {
        id: container

        AppListener {
            id: listener
            onDispatched: {
            }
        }
    }


    function test_on() {
        var test1, test2;
        compare(listener.listenerId > 0,true);
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

    function test_removeListener() {
        var count1 = 0,count2=0;

        var func1 = function() {
            count1++;
        }

        var func2 = function() {
            count2++;
        }

        listener.on("test_removeListener1",func1).on("test_removeListener2",func2);

        AppDispatcher.dispatch("test_removeListener1",null);
        AppDispatcher.dispatch("test_removeListener2",null);
        compare(count1,1);
        compare(count2,1);

        listener.removeListener("test_removeListener1",func1);
        AppDispatcher.dispatch("test_removeListener1",null);
        AppDispatcher.dispatch("test_removeListener2",null);

        compare(count1,1);
        compare(count2,2);

    }

    function test_removeAllListener() {
        var count1 = 0,count2=0;

        var func1 = function() {
            count1++;
        }

        var func2 = function() {
            count2++;
        }

        listener.on("test_removeAllListener",func1).on("test_removeAllListener",func2);

        AppDispatcher.dispatch("test_removeAllListener");
        AppDispatcher.dispatch("test_removeAllListener");
        compare(count1,2);
        compare(count2,2);

        listener.removeAllListener("test_removeAllListener");
        AppDispatcher.dispatch("test_removeAllListener",null);

        compare(count1,2);
        compare(count2,2);
    }

}

