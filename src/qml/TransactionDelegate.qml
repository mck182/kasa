import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Pane {
    width: 400

    signal transactionSelected(var selectedTransaction)
    signal transactionUnselected(var unselectedTransaction)

    GridLayout {
        id: gridLayout
        rows: 2
        columns: 5
        anchors.fill: parent

        CheckBox {
            id: selectionCheckBox

            Layout.rowSpan: 2

            onCheckedChanged: {
                if (checked) {
                    transactionSelected(model.transaction)
                } else {
                    transactionUnselected(model.transaction)
                }
            }
        }

        MouseArea {
            width: 80
            height: 48

            onClicked: {
                dateLabel.visible = false
                dateTextField.visible = true

                console.log("-----")
            }

//            Rectangle {
//                anchors.fill: parent
//                color: "#ff4444"
//            }

            Label {
                id: dateLabel
                anchors.fill: parent
                text: model.datePosted
            }

            TextField {
                id: dateTextField
                anchors.fill: parent
                visible: false

                onEditingFinished: {
                    dateLabel.visible = true
                    dateTextField.visible = false
                }
            }
        }

        MouseArea {
            id: nameMouseArea
            Layout.fillHeight: true
            Layout.fillWidth: true

            onClicked: {
                nameLabel.visible = false
                nameTextField.visible = true
                nameTextField.forceActiveFocus()
                nameMouseArea.enabled = false
            }

//            Rectangle {
//                anchors.fill: parent
//                color: "#ff4444"
//            }

            Label {
                id: nameLabel
                anchors.fill: parent
                text: model.name
                visible: true
            }

            TextField {
                id: nameTextField
                text: nameLabel.text
                placeholderText: "Transaction name..."
                anchors.fill: parent
                visible: false

                onEditingFinished: {
                    nameLabel.visible = true
                    nameTextField.visible = false
                    nameMouseArea.enabled = true
                }
            }
        }

        Label {
            id: label3
            text: model.memo
        }

        Label {
            id: label2
            text: model.amount
        }

        Label {
            id: label4
            Layout.columnSpan: 3
            text: model.tags
        }
    }
}
