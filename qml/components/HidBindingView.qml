import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1

Item {
  id: root
  property int side_margins : 30

  VisualDataModel {
    id: visu_group_model
    model: hidmanager_model
    delegate:
        ColumnLayout {
          id: group_column
          spacing: 0
          anchors.margins: side_margins

          Text { text: name; font.bold: true; font.underline: true}

          Repeater {
            Layout.fillWidth: true
            model: VisualDataModel {
              id: test_model
              model: hidmanager_model
              rootIndex: visu_group_model.modelIndex(index)

              Item {
                property bool show_description: false
                implicitHeight: col.height
                implicitWidth: view.width

                opacity: 0.8
                anchors.margins: 10

                Rectangle {
                  id: hvr_backg
                  anchors.fill: parent
                  visible: false
                  color: "gray"
                  opacity: 0.5

                  radius: 3
                }

                ColumnLayout {
                  id: col
                  spacing: 0
                  RowLayout {
                    Item { width: side_margins;}
                    Item {
                        height: text_name.height;
                        implicitWidth: view.width*2/3;
                        Text{ id: text_name; text: name ? name : "Name N/A" }
                    }
                    Item {
                        height: text_binding.height;
                        Layout.fillWidth: true;
                        Text{ id: text_binding; text: binding ? binding : "Binding N/A" }
                    }
                  }
                  RowLayout {
                    visible: show_description
                    Item { width: side_margins }
                    Item {
                        height: text_description.height;
                        Layout.fillWidth: true;
                        Text{ id: text_description; text: description ? description : "Description N/A";
                            font.italic: true; color: "gray" }
                    }
                  }
                  Item { height: 5 }
                }

                MouseArea {
                  anchors.fill: parent

                  hoverEnabled: true
                  onClicked: show_description = !show_description
                  onEntered: hvr_backg.visible = true
                  onExited: hvr_backg.visible = false
                }
              }
            }
          }
          Item { height: 10}
        }
  }

  Rectangle {
    anchors.fill: view
    opacity: 0.7
  }

  ListView {
    id: view
    anchors.fill: parent
    model: visu_group_model
    clip: true
  }

}
