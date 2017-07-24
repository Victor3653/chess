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
            {'imgPath' : "/images/white_bishop.svg"},
            {'imgPath' : "/images/white_knight.svg"},
            {'imgPath' : "/images/white_queen.svg"},
            {'imgPath' : "/images/white_king.svg"}
        ],

        [
            {'imgPath' : "/images/black_pawn.svg"},
            {'imgPath' : "/images/black_rook.svg"},
            {'imgPath' : "/images/black_bishop.svg"},
            {'imgPath' : "/images/black_knight.svg"},
            {'imgPath' : "/images/black_queen.svg"},
            {'imgPath' : "/images/black_king.svg"}
        ]
    ]

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

        text: "Clear"
        anchors.right: back.left
        onClicked: {
            console.log("Clear");
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
            width: root.width - x
            text: "New Game"
            onClicked: {
                console.log("New Game");
                logic.newGame();
                screen.push(chessPlacement);
            }
        }
    }

    Component {
        id: saveGame

        Button {
            x: logic.boardSize * squareSize
            width: root.width - x
            text: "Save game"
            onClicked: {
                console.log("Save game");
                logic.saveGame();
            }
        }
    }

    Component {
        id: endGame

        Button {
            x: logic.boardSize * squareSize
            width: root.width - x
            y: 22
            text: "End game"
            onClicked: {
                console.log("End game");
                logic.clear();
                screen.pop()
            }
        }
    }

    Component {
        id: chessPlacement

        Item {
            Loader {sourceComponent: gameBoard}
            Loader {sourceComponent: saveGame}
            Loader {sourceComponent: endGame}
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
                            if (!logic.move(fromX, fromY, toX, toY))
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
