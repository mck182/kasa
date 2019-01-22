import QtQuick 2.0
import QtQuick.Controls 2.3
import QtQuick.Controls.Material 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls.Universal 2.0

Item {
    width: 600
    height: 400

    Material.theme: Material.Light

    ColumnLayout {
        id: columnLayout
        anchors.fill: parent
        spacing: 0

        ToolBar {
            height: 48
            z: 8

            Layout.fillWidth: true
            Material.elevation: 4

            RowLayout {
                id: topRow
                anchors.fill: parent
                anchors.leftMargin: 8
                anchors.rightMargin: 8
                spacing: 8

                TextField {
                    id: textField
                    text: qsTr("")
                    placeholderText: "Filter by tags or name"
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignVCenter
                }

                Label {
                    text: qsTr("From")
                    leftPadding: 16
                    Layout.alignment: Qt.AlignVCenter
                    verticalAlignment: Text.AlignTop
                }

                ComboBox {
                    id: fromDatePicker
                    Layout.alignment: Qt.AlignVCenter
                }

                Label {
                    text: qsTr("To")
                    Layout.alignment: Qt.AlignVCenter
                }

                ComboBox {
                    id: toDatePicker
                    Layout.alignment: Qt.AlignVCenter
                }
            }
        }

        ListView {
            id: transactionsListView
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.leftMargin: 4
            Layout.rightMargin: 4

            spacing: 12
            model: transactionsModel

            delegate: TransactionDelegate {
                width: parent.width
                Material.elevation: 1

                onTransactionSelected: {
                    console.log(selectedIndex)
                }

            }

        }
    }

}
