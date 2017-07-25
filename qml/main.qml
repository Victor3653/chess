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
        id: buttonLoadGame

        Button {
            x: logic.boardSize * squareSize
            width: root.width - x
            y: 22
            text: "Load Game"
            onClicked: {
                console.log("Load Game");
                screen.push(loadGameScreen);
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
                screen.push(newGameScreen);
            }
        }
    }

    Component {
        id: buttonSaveGame

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
        id: buttonEndGame

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
        id: buttonPrevNext
        
        Item {
            Button {
                id: prev

                x: logic.boardSize * squareSize
                width: (root.width - x) / 2
                y: 44
                text: "Prev"
                onClicked: {
                    console.log("Prev");
                }
            }
            Button {
                id: next

                anchors.left: prev.right
                width: (root.width - logic.boardSize * squareSize) / 2
                y: 44
                text: "Next"
                onClicked: {
                    console.log("Next");
                }
            }
        }
    }

    Component {
        id: chessPlacement

        Item {
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

    Component {
        id: mainScreen
        Item {
            Loader {sourceComponent: gameBoard}
            Loader {sourceComponent: buttonNewGame}
            Loader {sourceComponent: buttonLoadGame}
        }
    }

    Component {
        id: newGameScreen

        Item {
            Loader {sourceComponent: gameBoard}
            Loader {sourceComponent: chessPlacement}
            Loader {sourceComponent: buttonSaveGame}
            Loader {sourceComponent: buttonEndGame}
        }
    }

    Component {
        id: loadGameScreen

        Item {
            Component {
                id: itemDelegate
                Text {
                    font.pixelSize: 24

                    text:  logic.gameName(index) 
                    MouseArea {
                        hoverEnabled: true
                        anchors.fill: parent
                        onClicked:  {
                            logic.newGame();
                            screen.replace(historyScreen);
                        }
                        onHoveredChanged: {
                            if (index > 0)
                                font.underline = font.underline == true ? false : true;
                        }
                    }
                }
            }
            ListView {
                height: 200
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.topMargin: 10
                model: logic.gamesSaved
                delegate: itemDelegate
            }
        }
    }

    Component {
        id: historyScreen

        Item {
            Loader {sourceComponent: gameBoard}
            Loader {sourceComponent: chessPlacement}
            Loader {sourceComponent: buttonNewGame}
            Loader {sourceComponent: buttonLoadGame}
            Loader {sourceComponent: buttonPrevNext}
        }
    }

    StackView {
        id: screen
        anchors.fill: parent
        initialItem: mainScreen
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
