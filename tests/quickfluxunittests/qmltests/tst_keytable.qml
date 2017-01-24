// To prove that it could pass QImage via AppDispatcher
import QtQuick 2.0
import QtTest 1.0
import QuickFlux 1.0

TestCase {
    name : "KeyTableTests"

    KeyTable {
        id: table1

        property string value1;

        property int value2;

        property string value3;

        property string custom: "customValue";

        property real value4 : 5.5;
    }


    function test_stringTable() {
        compare(table1.objectName, "");
        compare(table1.value1 , "value1");
        compare(table1.value2 , 0);
        compare(table1.value3 , "value3");
        compare(table1.custom , "customValue");

    }

    function test_code_generator() {

        table1.genHeaderFile("ActionTypes");
        table1.genSourceFile("ActionTypes","actiontypes.h");
    }

}

