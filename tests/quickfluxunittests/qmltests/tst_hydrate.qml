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
            property var value4;
        }

        function test_hydrate_qtobject() {
            var input = {
                value1: 1,
                value2: 2.0,
                value3: "3",
                value4: {
                    value1: 41,
                    value2: [
                        {index: 0},
                        {index: 1},
                    ]
                }
            }

            Hydrate.rehydrate(target1, input);
            compare(target1.value1, 1);
            compare(target1.value2, 2.0);
            compare(target1.value3, "3");
            compare(target1.value4, {value1: 41, value2:[{index:0},{index:1}]});

            var data = Hydrate.dehydrate(target1);
            compare(JSON.stringify(data), JSON.stringify(input));
        }

        Store {
            id: target2

            bindSource: Dispatcher {
            }

            property int value1 : 0;
            property real value2 : 0;
            property string value3: "";

            property var value4 : QtObject {
                id: target2Child1
                property var value41
            }

            Item {
                id: dummy
                property int value1: 0
            }
        }

        function test_hydrate_store() {
            var input = {
                value1: 1,
                value2: 2.0,
                value3: "3",
                value4: {
                    value41: 4
                }
            }

            Hydrate.rehydrate(target2, input);
            compare(target2.value1, 1);
            compare(target2.value2, 2.0);
            compare(target2.value3, "3");
            compare(target2.value4.value41, 4);

            var data = Hydrate.dehydrate(target2);
            compare(JSON.stringify(data), JSON.stringify(input));
        }

    }
}
