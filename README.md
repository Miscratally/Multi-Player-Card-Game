# Multi-Player-Card-Game

This project is a card game application developed using C++ and the Qt framework. The game is designed to be played by multiple players, and it involves a deck of cards, where each player draws a card, and the player with the highest-ranked card wins the game.

1. MainWindow:
The `MainWindow` class is derived from `QMainWindow`. It sets up the main user interface, handles user interactions, and manages the game's overall flow. The GUI layout consists of several elements, such as a start button, player widgets, a timer display, and labels for winner information. The class also interacts with the `QGameLogic` object to control the game.
- This class represents the main user interface of the card game application.
- It uses QPlayerWidget to display information about each player.
- The "initWidget" method sets up the UI components, including buttons, labels, and player widgets.
- The "initGame" method initializes the game with the specified number of players, allowing the user to input the player count using a QInputDlg.
- The "endGame" method cleans up resources and stops the game.
- The "slot_onPushbtnStartOnRoundClicked" method starts a new round when the "Start One Round" button is clicked.
- The "slot_onPlayerWidget_playerAddOrRemove" and "slot_onPlayerWidget_playerDrawCard" methods handle player-related actions when buttons are clicked.

2. QPlayerWidget:
The `QPlayerWidget` class is a custom widget representing a player's information. Each `QPlayerWidget` contains UI elements like a label to display the player's name, an avatar, and a button to add or remove the player from the game. The class provides slots to handle button clicks and emits signals to notify the main window about player-related events.
- Constructor: The constructor takes the player index (m_idx) as input and sets up the widget for the specific player.
- initWidget(): Initializes the widgets and layout for the player, including labels, buttons, and other UI components.
- setPlayerInfo(const QGamePlayer* gamePlayer): Updates the widget to display the information of the specified QGamePlayer object, such as name and avatar.
- setCardInfo(const QGameCard* gameCard): Updates the widget to display the information of the specified QGameCard object, such as card number and suit.
- sig_onPlayerAddOrRemove(int idx): Signal emitted when the add or remove player button is clicked, sending the player index as a parameter.
- sig_onPlayerDrawCard(int idx): Signal emitted when the draw card button is clicked, sending the player index as a parameter.
- slot_onAddOrRemovePushBtnClicked(): Slot to handle the click event of the add or remove player button.
- slot_onDrawCardPushBtnClicked(): Slot to handle the click event of the draw card button.

3. QInputDlg:
The `QInputDlg` class serves as a dialog box for user input to initialize a multi-player card game. The dialog allows the user to specify the number of players participating in the game. The main components of the dialog include a QLabel, a QLineEdit for user input, and a QPushButton to start the game. The dialog's appearance is customized using style sheets for a more visually appealing user experience.
- This class is a custom QDialog designed to get user input for the number of players.
- It contains a QLineEdit widget (m_lineEditNum) and a QPushButton widget (m_pushBtnStart).
- The constructor sets up the dialog's layout, including input labels and buttons.
- The onStartClicked slot is connected to the "clicked" signal of the "Start The Game" button and handles the start process when the button is clicked.
- The closeEvent method is overridden to handle the event when the dialog is closed. It calls QCoreApplication::quit() to exit the game.

4. QGameLogic:
The `QGameLogic` class is responsible for managing the game logic, including player initialization, drawing cards, and determining the winner. It runs on a separate thread to avoid blocking the main GUI thread. The class communicates with the main window through signals and slots to update the UI and receive user input.
- This class represents the game logic and manages player actions and card drawing.
- It maintains a vector of players and a list of game cards.
- The constructor initializes the player vector.
- The "initGame" method initializes the game with a given number of players and shuffles the deck of cards.
- The "startOneRound" method starts a new round of the game, clearing player cards and resetting the time countdown.
- The "addPlayer" and "removePlayer" methods allow adding and removing players from the game.
- The "findPlayerByName" method finds a player by their name.
- The "randomGameCards" method shuffles the deck of cards randomly.
- The "playerDrawCard" method allows a player to draw a card and calculates the winner if all players have drawn cards.
- The "calcWinner" method determines the winner based on the drawn cards.
- The "slot_onTimeout" method handles the game logic related to the time countdown.

5. QGameCard Class:
The `QGameCard` class encapsulates the properties of a playing card. It has two constructors: one default constructor and another that takes a card number and suit as arguments. The class has private member variables `m_number` and `m_suit`, which store the card's number and suit, respectively.
- The class has two constructors, a default constructor, and another constructor that takes a card number and suit as arguments to initialize the card.
- The "cardString" method returns a string representation of the card, combining the card's number and suit.
- The "compare" method allows comparing this card with another card. It returns an integer:
- The "number" method returns the number of the card (Ace, 2, 3, ..., 10, Jack, Queen, King).
- The "suit" method returns the suit of the card as an enumerated value (Spade, Heart, Diamond, Club).

6. QGamePlayer Class:
The `QGamePlayer` class, which represents a player in the card game application. Each player is associated with a name, an avatar, and a game card.
- This class represents a player in the card game application.
- It has a constructor that takes a player's name and avatar as input.
- The constructor initializes the player's name, avatar, and sets the game card pointer to nullptr.
- The "setGameCard" method is a setter function that allows setting the game card for the player.
- The "gameCard" method is a getter function that retrieves the pointer to the player's current game card.
- The "avator" method is a getter function that retrieves the player's avatar.
- The "name" method is a getter function that retrieves the player's name.
