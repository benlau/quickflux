import QtQuick 2.0
import QtTest 1.0
import QuickFlux 1.0

TestCase {
    name : "MiddlewaresTests"

    ActionCreator {
        id: actions

        signal test1();
    }

    Middlewares {
        id: middlewares
        QtObject  {
            id: middleware1
            property var next

            property var actions : new Array

            function dispatch(type , message) {
                actions.push(type);
                next(type , message);
            }
        }

        Item {
            id: middleware2
            property var next

            function dispatch(type , message) {
                next(type , message);
                next(type , message);
            }
        }
    }

    AppListener {
        id: listener1
    }

    function test_basic() {
        compare(middlewares.data.length, 2);
        compare(typeof middleware1.next, "function");
        compare(typeof middleware2.next, "function");
    }

}
