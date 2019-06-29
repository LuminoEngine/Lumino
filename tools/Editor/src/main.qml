import QtQuick 2.3
import QtQuick.Controls 1.4
import OpenGLUnderQML 1.0
import QtQuick.Layouts 1.1
import SceneGraphRendering 1.0

// main window
ApplicationWindow {
    id: root
    visible: true
    width: 800
    height: 600


    menuBar: MenuBar {
        Menu {
            title: "File"
            MenuItem { text: "Open..." }
            MenuItem {
                text: "Close"
                onTriggered: { vm.clicked(); }
            }
        }

        Menu {
            title: "Edit"
            MenuItem { text: "Cut" }
            MenuItem { text: "Copy" }
            MenuItem { text: "Paste" }
        }
    }

    ColumnLayout {

            anchors.fill: parent

            Rectangle {
                color: "#414141"
                Layout.fillWidth: true
                Layout.preferredHeight: 50
                //MyLabel {
                //    text: "Toolbar"
                //}
            }

            SplitView {

                Layout.fillHeight: true
                Layout.fillWidth: true
                orientation: Qt.Horizontal
                //handleDelegate: MyVSlider {}

                SplitView {

                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    orientation: Qt.Vertical
                    //handleDelegate: MyHSlider {}

                    SplitView {
                        //handleDelegate: MyVSlider {}
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        orientation: Qt.Horizontal

                        Rectangle {
                            color: "#565656"
                            Layout.fillHeight: true
                            Layout.preferredWidth: 200
                            Layout.minimumWidth: 200
                            //MyLabel {
                            //    text: "Tree view"
                            //}
                        }

                        TabView {
                            id: tabView
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            //onCurrentIndexChanged: tabChangeSound.play()

                            Tab{
                                Component.onCompleted:  { vm.onCompleted(); }
                                title: "Tab1"

                                Item{
                                    width: 320
                                    height: 480
                                    anchors.centerIn: parent

                                    Renderer {
                                        id: renderer
                                        anchors.fill: parent
                                        anchors.margins: 10

                                        // The transform is just to show something interesting..
                                        transform: [
                                            Rotation { id: rotation; axis.x: 0; axis.z: 0; axis.y: 1; angle: 0; origin.x: renderer.width / 2; origin.y: renderer.height / 2; },
                                            Translate { id: txOut; x: -renderer.width / 2; y: -renderer.height / 2 },
                                            Scale { id: scale; },
                                            Translate { id: txIn; x: renderer.width / 2; y: renderer.height / 2 }
                                        ]
                                    }
/*
                                    Squircle {
                                        SequentialAnimation on t {
                                            NumberAnimation { to: 1; duration: 2500; easing.type: Easing.InQuad }
                                            NumberAnimation { to: 0; duration: 2500; easing.type: Easing.OutQuad }
                                            loops: Animation.Infinite
                                            running: true
                                        }
                                    }
                                    */
                                }
                            }

                            Tab {
                                title: "Tab2"

                                Text {
                                    text: "text2"
                                }
                            }
                        }
                    }

                    Rectangle {
                        color: "#565656"
                        Layout.fillWidth: true
                        Layout.preferredHeight: 200
                        Layout.minimumHeight: 200
                        //MyLabel {
                        //    text: "Console output"
                        //}
                    }
                }

                Rectangle {
                    id: inspector
                    color: "#565656"
                    Layout.fillHeight: true
                    Layout.preferredWidth: 200
                    Layout.minimumWidth: 200
                    ///MyLabel {
                    //    text: "Properties inspector"
                    //}
                }


                ScrollView {
                    contentItem: inspector
                }

            }
        }

    /*
    Column {
        TabView {
            id: tabView
            //onCurrentIndexChanged: tabChangeSound.play()

            Tab{
                title: "Tab1"

                Text {
                    text: "text1"
                }
            }

            Tab {
                title: "Tab2"

                Text {
                    text: "text2"
                }
            }
        }
    }
    */

    /*
    Item{
        width: 320
        height: 480

        Squircle {
            SequentialAnimation on t {
                NumberAnimation { to: 1; duration: 2500; easing.type: Easing.InQuad }
                NumberAnimation { to: 0; duration: 2500; easing.type: Easing.OutQuad }
                loops: Animation.Infinite
                running: true
            }
        }
    }
    */

    /*
    MouseArea {
        anchors.fill: parent
        onClicked: {
                Qt.quit();
        }
    }

    Rectangle {
        anchors.fill: parent
        Text {
            anchors.centerIn: parent
            text: "Hello World"
        }
    }
    */
}


