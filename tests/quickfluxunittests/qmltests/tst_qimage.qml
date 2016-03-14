// To prove that it could pass QImage via AppDispatcher
import QtQuick 2.0
import QtTest 1.0
import QuickFlux 1.0

TestCase {
    name : "QImageTests"

    function test_passby_appdispatcher() {
        var image = TestEnv.createQImage(100,100);
        compare(image !== null, true);

        var listener1Image,listener2Image;

        var listener1 = AppDispatcher.addListener(function(type,message) {
            listener1Image = message.image;
        });

        var listener2 = AppDispatcher.addListener(function(type,message) {
            listener2Image = message.image;
        });

        AppDispatcher.dispatch("any-message",{image: image});

        compare(image,listener1Image);
        compare(image,listener2Image);

        AppDispatcher.removeListener(listener1);
        AppDispatcher.removeListener(listener2);
    }

    AppListener {
        id: listener1
        property var image;
        enabled: false;
        onDispatched: {
            image = message.image;
        }
    }

    AppListener {
        id: listener2
        property var image;
        enabled: false;
        onDispatched: {
            image = message.image;
        }
    }

    function test_passby_applistener() {
        listener1.enabled = true;
        listener2.enabled = true;

        var image = TestEnv.createQImage(100,100);
        compare(image !== null, true);

        AppDispatcher.dispatch("any-message",{image: image});

        compare(image,listener1.image);
        compare(image,listener2.image);

        listener1.enabled = false;
        listener2.enabled = false;
    }

}

