import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.2

ApplicationWindow {
    id: root
    title: qsTr("Chess")
    visible: true
    width: 800
    height: 600
    color: "#e9f4ef"

    property int squareSize: 70

    property var images: [
        [
            {'imgPath' : "/images/white_pawn.svg"},
            {'imgPath' : "/images/white_rook.svg"},
            {'imgPath' : "/images/white_bishop.svg"}
        ],

        [
            {'imgPath' : "/images/black_pawn.svg"},
            {'imgPath' : "/images/black_rook.svg"},
            {'imgPath' : "/images/black_bishop.svg"}
        ]
    ]

//    Button {
//        id: loadGame
//        anchors.right: parent.right
//        anchors.top: newGame.bottom

//        text: "Load Game"
//        onClicked: {
//              console.log("Load Game");
//              screen.push(screenThree);
//        }
//    }
    Button {
        id: back
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        text: "Back"
        onClicked: {
            console.log("Pop view");
            screen.pop()
        }
}

    Button {
        id: clearButton
        anchors.bottom: parent.bottom

        text: "restart"
        anchors.right: back.left
        onClicked: {
            console.log("Restarted");
            logic.clear();
        }
    }

    Component {
        // First screen
        id: gameBoard
        Item {
            Image {
                id: board
                source: "/images/chess_board.jpg"
                height: logic.boardSize * squareSize
                width: logic.boardSize * squareSize
            }
        }
    }

    Component {
        id: mainScreen
        Item {
            Loader {
                id: backgroundLoader
                sourceComponent: gameBoard
            }
            Loader {
                id: buttonsLoader
                sourceComponent: buttonNewGame
            }
        }
    }

    Component {
        id: buttonNewGame
        Button {
            x: logic.boardSize * squareSize
            width: root.width - buttonNewGame.x
            text: "New Game"
            onClicked: {
                  console.log("New Game");
                  screen.push(chessPlacement);
            }
        }
    }

    Component {
        id: chessPlacement

        Item {

                Loader {sourceComponent: gameBoard}
                Repeater {
                    model: logic
                    Image {
                        height: squareSize
                        width : squareSize

                        x: squareSize * positionX
                        y: squareSize * positionY

                        source: images[(side == true) ? 0 : 1][type].imgPath

                        MouseArea {
                            anchors.fill: parent
                            drag.target: parent
                            property int startX: 0
                            property int startY: 0
                            onPressed: {
                                console.log("On press: ", parent.x, parent.y);
                                startX = parent.x;
                                startY = parent.y;
                            }
                            onReleased: {
                                var  fromX = startX / squareSize;
                                var fromY = startY / squareSize;
                                var toX   = (parent.x + mouseX) / squareSize;
                                var toY   = (parent.y + mouseY) / squareSize;
                                if (!logic.move(fromX, fromY, toX, toY, side, type))
                                {
                                    parent.x = startX;
                                    parent.y = startY;
                                }
                            }
                        }
                    }
                }


        }
    }

    StackView {
        id: screen
        anchors.fill: parent
        initialItem: mainScreen

        Item {
            id: screenThree
        }
    }

    Connections {
        target: clearButton
        onClicked: print("clicked")
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
