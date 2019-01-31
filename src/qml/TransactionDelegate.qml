import QtQuick 2.11
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import QtQuick.Layouts 1.3

Pane {
    width: 400
    height: 90

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
            height: 40

            onClicked: {
                dateLabel.visible = false
                dateTextField.visible = true
            }

            Label {
                id: dateLabel
                anchors.verticalCenter: parent.verticalCenter
                text: model.datePosted
            }

//            TextField {
//                id: dateTextField
//                anchors.fill: parent
//                visible: false

//                onEditingFinished: {
//                    dateLabel.visible = true
//                    dateTextField.visible = false
//                }
//            }
        }

        EditableLabel {
            id: nameEditableLabel

            Layout.fillWidth: true

            placeholderText: "Transaction name..."
            text: model.name
        }

        EditableLabel {
            id: memoEditableLabel

            placeholderText: "Memo..."
            text: model.memo
        }

        EditableLabel {
            id: amountEditableLabel

            //FIXME: TextMetrics return wrong width for some reason
            Layout.minimumWidth: 68
            Layout.maximumWidth: 68

            horizontalAlignment: Text.AlignRight
            placeholderText: "Amount.."
            text: model.amount

            TextMetrics {
                id: amountTextMetrics

                font: amountEditableLabel.font
                text: "-88,888"
            }
        }

        MouseArea {
            id: tagsMouseArea

            property var transactionItem: model
            property var tagsModel: model.tagsList
            property var tags: model.tags

            Layout.columnSpan: 3
            Layout.fillWidth: true
            Layout.fillHeight: true

            onClicked: {
                if (textField.visible) {
                    transactionItem.tags = textField.text
                    textField.visible = false
                    tagsFlow.visible = true
                } else {
                    textField.visible = true
                    tagsFlow.visible = false
                }
            }

            TextField {
                id: textField

                anchors.fill: parent

                text: tagsMouseArea.tags
                visible: false

                onEditingFinished: {
                    tagsMouseArea.transactionItem.tags = textField.text
                    textField.visible = false
                    tagsFlow.visible = true
                }
            }

            Flow {
                id: tagsFlow

                anchors.fill: parent

                spacing: 4

                move: Transition {
                    NumberAnimation {
                        properties: "x,y"
                        duration: 150
                        easing.type: Easing.OutQuad
                    }
                }

                Repeater {
                    width: parent.width
                    height: parent.height

                    model: tagsMouseArea.tagsModel

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

                                    var tags = tagsMouseArea.tagsModel
                                    tags.splice(index, 1)
                                    tagsMouseArea.transactionItem.tagsList = tags
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
