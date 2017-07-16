import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.2

ApplicationWindow {
    title: qsTr("Chess")
    visible: true
    width: 800
    height: 600

    property int squareSize: 70

    property var images: [
        {'imgPath' : "/images/white_pawn.svg"},
        {'imgPath' : "/images/black_pawn.svg"},
    ]

    StackView {
        id: screen
        anchors.fill: parent
        initialItem: screenOne
// Why Component doesnt work right, wtf...
        Item {
            id: screenOne
            Item {
                id: gameBoard
                x: 0
                y: 0

                width : logic.boardSize * squareSize
                height: logic.boardSize * squareSize

                Image {
                    source: "/images/chess_board.jpg"
                    height: gameBoard.height
                    width: gameBoard.width
                }
            }
            Button {
                id: newGame
                anchors.left: gameBoard.right
                anchors.right: parent.right
                anchors.top: parent.top

                text: "New Game"
                onClicked: {
                      console.log("New game");
                }
            }
            Button {
                id: loadGame
                anchors.left: gameBoard.right
                anchors.right: parent.right
                anchors.top: newGame.bottom

                text: "Load Game"
                onClicked: {
                      console.log("Load Game");
                }
            }
        }
    }
}
//ApplicationWindow {
//    title: qsTr("Chess")
//    visible: true
//    width: 800
//    height: 600
    
//    property int squareSize: 70

//    property var images: [
//      {'imgPath' : "/images/white_pawn.svg"},
//      {'imgPath' : "/images/black_pawn.svg"},
//    ]

//    Item {
//      id: gameBoard
//      x: 0
//      y: 0
//      width : logic.boardSize * squareSize
//      height: logic.boardSize * squareSize
      
//      Image {
//        source: "/images/chess_board.jpg"
//        height: gameBoard.height
//        width: gameBoard.width
//      }
      
//      Repeater {
//        model: logic

//        Image {
//          height: squareSize
//          width : squareSize

//          x: squareSize * positionX
//          y: squareSize * positionY

//          source: images[type].imgPath
          
//          MouseArea {
//            anchors.fill: parent
//            drag.target: parent

//            property int startX: 0
//            property int startY: 0

//            onPressed: {
//              console.log("On press: ", parent.x, parent.y);
//              startX = parent.x;
//              startY = parent.y;
//            }

//            onReleased: {
//              var fromX = startX / squareSize;
//              var fromY = startY / squareSize;
//              var toX   = (parent.x + mouseX) / squareSize;
//              var toY   = (parent.y + mouseY) / squareSize;

//              if (!logic.move(fromX, fromY, toX, toY))
//              {
//                    parent.x = startX;
//                    parent.y = startY;
//              }
//            }
//          }
//        }
//      }
//    }

//    Button {
//      id: clearButton
//      anchors.left: gameBoard.right
//      anchors.right: parent.right
//      anchors.leftMargin: 10
//      anchors.rightMargin: 10
      
//      text: "Clear"

//      onClicked: {
//        logic.clear();
//      }
//    }
//    Button {
//        id: testButton
//        anchors.left: gameBoard.right
//        anchors.top: clearButton.bottom
//        anchors.right: parent.right
//        anchors.leftMargin: 10
//        anchors.rightMargin: 10

//        text: "Test"
//        onClicked: {
//          console.log("Test");
//        }
//    }
//}
