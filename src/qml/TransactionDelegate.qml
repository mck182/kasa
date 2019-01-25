import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
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
            }

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

        Flow {
            id: tagsFlow

            property var transactionItem: model
            property var tagsModel: model.tagsList

            Layout.columnSpan: 3

            spacing: 4

            move: Transition {
                NumberAnimation { properties: "x,y"; duration: 150; easing.type: Easing.OutQuad }
            }

            Repeater {
                width: parent.width
                height: parent.height

                model: tagsFlow.tagsModel

                Rectangle {
                    id: tagItem
                    width: tagLabel.width + closeButton.width + 16
                    height: tagLabel.height
                    radius: 10
                    clip: true
                    border.width: 1
                    border.color: Material.color(Material.primary)

                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true

                        onEntered: {
                            tagItem.color = Material.color(Material.primary)
                        }

                        onExited: {
                            tagItem.color = "transparent"
                        }
                    }

                    Label {
                        id: tagLabel

                        text: "#" + modelData
                        leftPadding: 8
                    }

                    Label {
                        id: closeButton
                        text: "x"
                        horizontalAlignment: Text.AlignHCenter
                        anchors.right: parent.right
                        anchors.rightMargin: 8

                        MouseArea {
                            anchors.fill: parent

                            onClicked: {
                                // FIXME: This is potentially stupid but it's so that
                                //        the Flow item can run the 'move' transition
                                tagItem.visible = false

                                var tags = tagsFlow.tagsModel
                                tags.splice(index, 1)
                                tagsFlow.transactionItem.tagsList = tags
                            }
                        }
                    }
                }
            }
        }
    }
}
