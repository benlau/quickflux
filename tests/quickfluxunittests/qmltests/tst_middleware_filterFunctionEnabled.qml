import QtQuick 2.0
import QtTest 1.0
import QuickFlux 1.1

TestCase {
    name : "Middleware_FilterFunctionEnabled"

    ActionCreator {
        id: actions

        signal test1();
    }

    MiddlewareList {
        id: middlewares

        Middleware  {
            id: middleware1

            property var actions : new Array

            filterFunctionEnabled: true

            function test1(message) {
                middleware1.actions.push("test1");
                next("test1", message);
            }

            function dispatch(type , message) {
            }
        }

        Middleware {
            id: middleware2

            function dispatch(type , message) {
                next(type , message);
                next(type , message);
            }
        }
    }

    AppListener {
        id: listener1
        property var actions : new Array

        onDispatched: {
            listener1.actions.push(type);
        }
    }

    function test_basic() {
        compare(middlewares.data.length, 2);
        compare(middleware1._nextCallback, undefined); // It won't set the next function until the middleware is applied

        middlewares.apply(actions);
        compare(typeof middleware1._nextCallback, "function");
        compare(typeof middleware2._nextCallback, "function");

        actions.test1();

        compare(middleware1.actions, ["test1"]);
        compare(listener1.actions, ["test1","test1"]);

    }

}
