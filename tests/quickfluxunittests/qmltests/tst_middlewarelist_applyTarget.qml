import QtQuick 2.0
import QtTest 1.0
import QuickFlux 1.1

TestCase {
    name : "MiddlewaresTests_applyTarget"

    ActionCreator {
        id: actions1
        signal test1();
    }

    ActionCreator {
        id: actions2

        function test1() {
            dispatch("test1");
        }
    }

    MiddlewareList {
        id: middlewares
        applyTarget: actions1

        QtObject  {
            id: middleware1
            property var next

            property var actions : new Array

            function dispatch(type , message) {
                middleware1.actions.push(type);
                next(type , message);
            }
        }
    }

    function test_basic() {
        actions1.test1();
        compare(middleware1.actions, ["test1"]);
        actions2.test1();

        compare(middleware1.actions, ["test1" ,"test1"]); // Both of the actions use sample dispatcher

        middlewares.apply(null);
        actions1.test1();
        compare(middleware1.actions, ["test1" ,"test1"]);
    }

}
