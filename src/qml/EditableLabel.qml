import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2

MouseArea {
    id: rootMouseArea

    property alias font: label.font
    property alias horizontalAlignment: label.horizontalAlignment
    property alias placeholderText: textField.placeholderText
    property alias text: label.text

    implicitWidth: label.implicitWidth
    implicitHeight: textField.visible ? textField.height : 40

    cursorShape: "IBeamCursor"

    onClicked: {
        label.visible = false
        textField.visible = true
        textField.forceActiveFocus()
        rootMouseArea.enabled = false
    }

//    Rectangle {
//        anchors.fill: parent
//        color: "#4444ff"
//        opacity: 0.2
//    }

    Label {
        id: label

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter

        visible: true
    }

    TextField {
        id: textField

        anchors.left: parent.left
        anchors.right: parent.right

        font.pointSize: label.font.pointSize
        text: label.text
        visible: false

        onEditingFinished: {
            label.text = text
            label.visible = true
            textField.visible = false
            rootMouseArea.enabled = true
        }

//                Rectangle {
//                    anchors.fill: parent
//                    color: "#4444ff44"
//                }
    }
}
