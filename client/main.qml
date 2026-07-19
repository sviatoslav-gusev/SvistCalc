import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQml 2.2
import QtQuick.Controls.Material 2.0
import Qt.labs.settings 1.0

ApplicationWindow {
    id: window

    Material.theme: Material.Dark
    Material.accent: Material.Teal

    visible: true
    width: 650
    height: 500
    title: "СвистCalc - Community Edition 1.0"

    // Dark bg
    background: Rectangle {
        color: "#1c1c1e"

        MouseArea {
            anchors.fill: parent
            onClicked: exprInput.forceActiveFocus()
        }
    }

    Settings {
        category: "MainWindow"
        property alias x: window.x
        property alias y: window.y
        property alias width: window.width
        property alias height: window.height
    }

    // Statusbar stat vars
    property int queued: 0
    property int processed: 0
    property int total: 0
    property int errors: 0

    Connections {
        target: calcApp

        readonly property string logBlue:  "#4DA6FF"
        readonly property string logGreen: "#00D084"
        readonly property string logRed:   "#FF4A4A"

        onRequestAdded: {
            // Добавляем строку с HTML-форматированием цвета
            logArea.append("<font color=\"" + logBlue + "\">" + msg + "</font>")
            window.queued++
            window.total++
        }
        onResultSuccess: {
            logArea.append("<font color=\"" + logGreen + "\">" + msg + "</font>")
            window.queued--
            window.processed++
        }
        onResultError: {
            logArea.append("<font color=\"" + logRed + "\">" + msg + "</font>")
            window.queued--
            window.processed++
            window.errors++
        }
        onParseError: {
            logArea.append("<font color=\"" + logRed + "\">" + msg + "</font>")
            window.errors++
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 15
        spacing: 15

        focus: true   //intercept default focus

        // Focus on digit or operator input
        Keys.onPressed: {
            // Start calc by enter and focus does not matter
            if (event.key === Qt.Key_Enter || event.key === Qt.Key_Return || event.key === Qt.Key_Equal)
            {
                if (exprInput.text !== "") {
                    calcApp.calculate(exprInput.text,
                                      delayInput.value,
                                      engineInput.currentIndex)
                    exprInput.text = ""
                }
                event.accepted = true  // Notify system about acceptance
                return
            }

            var charTyped = event.text

            // By pressing digit, sign
            if (/^[0-9\.\,\+\-\*\/]$/.test(charTyped)) {

                // focus teleportation to the input
                exprInput.forceActiveFocus()

                exprInput.insert(exprInput.cursorPosition, charTyped)

                event.accepted = true  // Notify system about acceptance
            }
        }

        // 1. INPUT FIELD
        Rectangle {
            Layout.fillWidth: true
            height: 50
            color: "#2c2c2e"
            radius: 6
            border.color: "#3c3c3e"

            TextInput {
                id: exprInput
                anchors.fill: parent
                anchors.margins: 10
                color: "#ffffff"
                font.pixelSize: 22
                font.family: "Consolas"
                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignVCenter
                clip: true
                selectByMouse: true
                focus: true

                // Anti "tupoe tyrkanie" pt.1
                validator: RegExpValidator { regExp: /^[0-9\.\,\+\-\*\/]+$/ }

                // intercept '=' and start calculation
                Keys.onPressed: {
                    if (event.key === Qt.Key_Equal)
                    {
                        if (exprInput.text !== "") {
                            calcApp.calculate(exprInput.text,
                                              engineInput.currentIndex,
                                              delayInput.value)
                            exprInput.text = ""
                        }
                        event.accepted = true
                    }
                }

                // Anti "tupoe tyrkanie" pt.2
                onTextChanged: {
                    var modifiedText = text;

                    // 1. Search 2 operators. Found: just 2nd will alive
                    if (/[\+\-\*\/]{2}/.test(modifiedText)) {
                        modifiedText = modifiedText.replace(/[\+\-\*\/]([\+\-\*\/])/g, "$1");
                    }

                    // 2. Erase starting operator (except -) if exists
                    if (/^[\+\*\/]/.test(modifiedText)) {
                        modifiedText = modifiedText.substring(1);
                    }

                    // 3. Recursion breaker
                    // Reassign only if changed
                    if (modifiedText !== text) {
                        var cursor = cursorPosition;
                        text = modifiedText;

                        // Return cursor to the tail
                        cursorPosition = Math.max(0, cursor - 1);
                    }
                }

                onAccepted: {
                    if (exprInput.text === "") { return }

                    calcApp.calculate(exprInput.text,
                                      engineInput.currentIndex,
                                      delayInput.value)
                    exprInput.text = ""
                }
            }
        }

        // 2. MID: Settings (Left) + Keyboard (Right)
        RowLayout {
            Layout.fillWidth: true
            spacing: 20

            // SETTINGS
            ColumnLayout {
                Layout.alignment: Qt.AlignCenter
                spacing: 15

                RowLayout {
                    Text { text: "Engine";
                           color: "#a0a0a5";
                           font.pixelSize: 14;
                           Layout.preferredWidth: 60
                    }
                    ComboBox {
                        id: engineInput
                        model: ["Inner", "OuterLib"]
                        Layout.preferredWidth: 130
                    }
                }
                RowLayout {
                    Text { text: "Delay, s";
                           color: "#a0a0a5";
                           font.pixelSize: 14;
                           Layout.preferredWidth: 60
                    }
                    SpinBox {
                        id: delayInput
                        from: 0; to: 10; value: 1
                        Layout.preferredWidth: 130
                    }
                }
            }

            // KEYBOARD
            GridLayout {
                Layout.fillWidth: true
                columns: 6
                rowSpacing: 8
                columnSpacing: 8

                // Button models
                Repeater {
                    model: ListModel {
                        ListElement { label: "5"; type: "num" }
                        ListElement { label: "6"; type: "num" }
                        ListElement { label: "7"; type: "num" }
                        ListElement { label: "8"; type: "num" }
                        ListElement { label: "9"; type: "num" }
                        ListElement { label: "<-"; type: "del" }

                        ListElement { label: "0"; type: "num" }
                        ListElement { label: "1"; type: "num" }
                        ListElement { label: "2"; type: "num" }
                        ListElement { label: "3"; type: "num" }
                        ListElement { label: "4"; type: "num" }
                        ListElement { label: "C"; type: "clear" }

                        ListElement { label: "."; type: "num" }
                        ListElement { label: "+"; type: "op" }
                        ListElement { label: "-"; type: "op" }
                        ListElement { label: "*"; type: "op" }
                        ListElement { label: "/"; type: "op" }
                        ListElement { label: "="; type: "eq" }
                    }

                    Button {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 45
                        text: label
                        font.pixelSize: 18
                        font.bold: true

                        // Custom design for different types
                        contentItem: Text {
                            text: parent.text
                            font: parent.font
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter

                            // Ops: blue, Del: red, Nums: white
                            color: (type === "op")
                                   ? "#4DA6FF"
                                   : ((type === "del" || type === "clear")
                                      ? "#FF4A4A"
                                      : "#FFFFFF")
                        }

                        background: Rectangle {
                            radius: 6
                            // Beautiful "=", grey - anothers
                            color: type === "eq"
                                   ? (parent.pressed
                                      ? "#005bb5"
                                      : "#0078D7")
                                   : (parent.pressed
                                      ? "#4c4c4e"
                                      : "#3c3c3e")
                        }

                        // Click logic
                        onClicked: {
                            if (type === "eq") {
                                if (exprInput.text !== "") {
                                    calcApp.calculate(exprInput.text,
                                                      engineInput.currentIndex,
                                                      delayInput.value)
                                    exprInput.text = "" // Clean after task creation
                                }
                            } else if (type === "clear") {
                                exprInput.text = ""
                            } else if (type === "del") {
                                exprInput.text = exprInput.text.slice(0, -1)
                            } else {
                                exprInput.text += label
                            }
                        }
                    }
                }
            }
        }

        // 3. LOGS
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#0a0a0c"
            border.color: "#3c3c3e"
            radius: 6

            Flickable {
                id: logScrollView
                anchors.fill: parent
                anchors.margins: 10
                clip: true

                contentWidth: width
                contentHeight: logArea.height

                // Native scrolling behaviour
                boundsBehavior: Flickable.StopAtBounds

                TextEdit {
                    id: logArea
                    width: logScrollView.width

                    readOnly: true
                    selectByMouse: true
                    textFormat: TextEdit.RichText

                    font.pixelSize: 13
                    font.family: "Consolas"
                    wrapMode: TextEdit.Wrap

                    selectionColor: "#0078D7"
                    selectedTextColor: "#ffffff"

                    // Autoscroll
                    onHeightChanged: {
                        logScrollView.contentY = Math.max(0, logArea.height - logScrollView.height)
                    }
                }
            }
        }

        // 4. STATUSBAR
        Rectangle {
            Layout.fillWidth: true
            height: 1
            color: "#3c3c3e"
        }

        RowLayout {
            Layout.fillWidth: true
            Text {
                text: "TaskQueue: " + window.queued +
                      ".   Processed: " + window.processed + "/" + window.total +
                      ".   Errors: " + window.errors + "."
                color: "#a0a0a5"
                font.pixelSize: 12
            }
        }
    }
}
