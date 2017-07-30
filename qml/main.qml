import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.2

ApplicationWindow {
    id: root
    title: qsTr("Chess")
    visible: true
    minimumWidth: logic.boardSize * squareSize + squareSize * 4
    minimumHeight: logic.boardSize * squareSize

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
            y: squareSize
            width: root.width - x
            height: squareSize
            text: "Load Game"
            onClicked: {
                console.log("Load Game");
                screen.pop(null);
                screen.push(loadGameScreen);
            }
        }
    }

    Component {
        id: buttonNewGame

        Button {
            x: logic.boardSize * squareSize
            width: root.width - x
            height: squareSize

            text: "New Game"
            onClicked: {
                console.log("New Game");
                logic.clear();
                logic.newGame();
                screen.pop(null);
                screen.push(newGameScreen);
            }
        }
    }

    Component {
        id: buttonSaveGame

        Button {
            x: logic.boardSize * squareSize
            width: root.width - x
            height: squareSize
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
            y: squareSize
            width: root.width - x
            height: squareSize
            text: "End game"
            onClicked: {
                console.log("End game");
                logic.clear();
                screen.pop()
            }
        }
    }

    Component {
        id: buttonBack

        Button {
            x: logic.boardSize * squareSize
            width: root.width - x
            height: squareSize
            text: "Back"
            onClicked: {
                console.log("Back");
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
                y: squareSize * 2
                width: (root.width - x) / 2
                height: squareSize
                text: "Prev"
                onClicked: {
                    logic.prev();
                    console.log("Prev");
                }
            }
            Button {
                id: next

                anchors.left: prev.right
                anchors.verticalCenter: prev.verticalCenter
                width: (root.width - logic.boardSize * squareSize) / 2
                height: squareSize
                text: "Next"
                onClicked: {
                    logic.next();
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
                    text:  logic.gameName(index) 
                    MouseArea {
                        hoverEnabled: true
                        anchors.fill: parent
                        onClicked:  {
                                logic.clear();
                                logic.newGame();
                                logic.selectGame(index);
                                screen.replace(historyScreen);
                        }
                        onHoveredChanged: {
                                font.underline = font.underline == true ? false : true;
                                font.bold = font.bold == true ? false : true;
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

                header: Text {text: "Click to choose"}
                Loader {sourceComponent: buttonBack}
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
    Text {
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        text: qsTr("vpopovyc / 2017   ");
    }
}
