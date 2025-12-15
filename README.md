# 🎮 Memory Game 🧠

> A command-line memory card matching game written in C where players take turns finding matching pairs of objects on a grid board.

---

## 📋 Overview

This is a multi-player memory game where players try to find matching pairs of hidden objects on a board. Players take turns selecting two positions, and if they match, the player scores a point and continues their turn. The game ends when all pairs are found, and the player with the most matches wins.

## ✨ Features

- 🎲 Configurable board size (up to 30×30)
- 👥 Support for up to 10 players
- 🎯 Dynamic object placement
- 🔄 Turn-based gameplay
- 📊 Score tracking and leaderboard
- ✅ Input validation

---

## 🔨 Compilation

```bash
gcc -o memory_game ex_4.c
```

---

## 🚀 Usage

```bash
./memory_game <board_dimension> <num_players> <player_names> <object_names> <positions>
```

### 📝 Command Line Arguments

1. **board_dimension** 📐: Size of the square board (n×n)
2. **num_players** 👤: Number of players in the game
3. **player_names** 🏷️: Player names separated by `#` (e.g., `Alice#Bob#Charlie`)
4. **object_names** 🎴: Names of objects to match (n²/2 objects)
5. **positions** 📍: Positions for each object pair (two positions per object)

### 💡 Example

For a 4×4 board with 2 players:

```bash
./memory_game 4 2 "Alice#Bob" apple banana orange pear 0 5 1 8 2 11 3 15 4 9 6 13 7 14 10 12
```

This creates:
- 🎯 A 4×4 board (16 cells, 8 pairs)
- 👥 2 players: Alice and Bob
- 🍎 4 objects: apple, banana, orange, pear
- 📍 Each object appears at two positions on the board

---

## 📖 Game Rules

1. 🎯 Players take turns selecting two cards by entering their row and column coordinates
2. ✅ If the cards match, the player scores a point and gets another turn
3. ❌ If the cards don't match, the turn passes to the next player
4. 🔓 Matched cards remain revealed on the board
5. 🏁 The game ends when all pairs are found
6. 🏆 The player with the most matches wins

---

## ⌨️ Input Format

During gameplay, enter coordinates as:
```
row col
```

For example, to select position (1,2):
```
1 2
```

---

## ⚙️ Constraints

- 📏 Maximum board dimension: 30×30
- 👥 Maximum players: 10
- 📝 Maximum player name length: 20 characters
- 🎴 Maximum object name length: 7 characters
- 📍 Positions must be in range [0, n²-1]
- 🚫 No duplicate positions allowed

---

## ⚠️ Error Handling

The program validates:
- ✔️ Correct number of command line arguments
- ✔️ Board dimension and player count
- ✔️ Player names format
- ✔️ Position ranges and uniqueness
- ✔️ Move validity during gameplay

Invalid input results in an **"INVALID INPUT"** ⛔ message and program termination.

---

## 🎲 Game Flow

1. **🎬 Setup**: Display welcome message and board layout
2. **🎮 Gameplay**: Players take turns until all pairs are found
3. **📊 Scoring**: Track and display scores after each turn
4. **🏆 Winner**: Announce winner or tie at game end

---

## 🏗️ Code Structure

### 📦 Data Structures:
### 📦 Data Structures:
  - `Player` 👤: Stores player name and score
  - `Object` 🎴: Stores object name, positions, and found status
  - `Board` 🎯: Contains game configuration and state

### 🔧 Key Functions:
  - `createBoard()` 🏗️: Initializes game from command line arguments
  - `playGame()` 🎮: Main game loop
  - `playTurn()` 🎲: Handles individual player turns
  - `printCurrentBoard()` 🖨️: Displays current board state
  - `leaderboard()` 🏆: Determines and announces winner

---
---

<div align="center">

### 🎉 Happy Gaming! 🎉


</div>
