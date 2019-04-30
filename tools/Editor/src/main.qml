import QtQuick 2.3
import QtQuick.Controls 1.4

// main window
ApplicationWindow {
    visible: true

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

}


