import QtQuick 2.0
import QtTest 1.0
import QuickFlux 1.1

Item {
    width: 640
    height: 480

    TestCase {
        name: "HydrateTests"

        QtObject {
            id: target1
            property int value1 : 0;
            property real value2 : 0;
            property string value3: "";
        }

        function test_assign() {
            var input = {
                value1: 1,
                value2: 2.0,
                value3: "3"
            }

            Hydrate.rehydrate(target1, input);
            compare(target1.value1, 1);
            compare(target1.value2, 2.0);
            compare(target1.value3, "3");

            var data = Hydrate.dehydrate(target1);
            compare(data, input);
        }

    }
}
