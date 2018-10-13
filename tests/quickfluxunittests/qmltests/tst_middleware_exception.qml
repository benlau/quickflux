import QtQuick 2.0
import QtTest 1.0
import QuickFlux 1.1

TestCase {
    name : "Middleware_Exception"

    Loader {
        id: loader
        active: false
        sourceComponent: MiddlewareList {
            id: list
            applyTarget: AppDispatcher

            Middleware {
            }

            Middleware {
                function dispatch(type, message) {
                    i = 10;
                }
            }
        }
    }

    function init() {
        loader.active = true;
    }

    function cleanup() {
        loader.active = false;
    }

    function test_Middleware_dispatch_should_show_exception_error() {
        AppDispatcher.dispatch("test_exception");
    }

}
