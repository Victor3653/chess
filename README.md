# Chess

It's a local multiplayer chess game created using QT framework. Technologies: QT+QML, C++11 and SQLite.

 #### Features
 - Start new game
 - Save game played game in SQLite database
 - Load previously played games
 - Explore step-by-step previously played game

##### Screen_1
Once the application starts the **screen_1** appears. There should be at least the following elements on the screen: 
>Start button – starts a new game. Leads to the **screen_2**
>Load button – allows user to load saved game. Leads to the **screen_3**
>Empty chess board

![screen_1](http://i.imgur.com/2hVINYx.jpg)
##### Screen_2
Chess pieces are on the board (in initial position). User is able to move the pieces with a mouse according to the standard chess rules. Turn by turn rule is followed as well (white turn-black turn- white-…). The following buttons is accessible:
>Stop – application stops the game and leads to the **screen_1**
>Save – saves the game history to a file

![screen_2](http://i.imgur.com/SbnP9uX.jpg)
##### Screen_3
Loads a game history and allows to play it step by step using two buttons: “prev” and “next”. The following items are available on the screen:
>Start button – starts a new game. Leads to the **screen_2**
>Load button – allows user to load saved game. Leads to the **screen_3**
>Buttons “prev” and “next”
>Chess board with pieces
![screen_3](http://i.imgur.com/Vy1ffpP.jpg)
##### How to build

>Install QT 5.6
 - go to QT official site : https://info.qt.io/download-qt-for-application-development
 - press "Get your open source package" button. On the opened page, press "Download Now" button. (it will download qt online installer. Don't forget to choose QT.5.6 when the installer asks which QT version should be installed).

>Run Qt creator

>open Chess.pro file

>build and run the project