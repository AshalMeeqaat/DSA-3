# Game and Player Binary Search Tree Project

This project demonstrates a **Binary Search Tree (BST)** system to manage both **Games** and **Players**. The system allows for inserting, searching, displaying, deleting, and exporting data to CSV files, utilizing separate BST structures for `Game` and `Player` records.

## Table of Contents

- [Features](#features)
- [Classes and Structure](#classes-and-structure)
- [Installation and Setup](#installation-and-setup)
- [Usage](#usage)
- [Code Overview](#code-overview)
- [License](#license)

## Features

- **Game and Player Management**: Insert, search, delete, and display `Game` and `Player` records.
- **File Operations**: Load data from CSV files and save BST data back into CSV files.
- **Layer Display**: Display tree layers up to a given depth.
- **Path Tracking**: Show the search path for a particular game or player in a preorder traversal.
- **Dynamic Depth Calculation**: Supports calculating maximum depth of the tree.

## Classes and Structure

### Game and Player Classes

- **Game Class**: Holds details about a game (ID, name, developer, publisher, file size, and downloads).
- **Player Class**: Stores information about a player (ID, name, phone number, email, and password).

### GameBinarySearchTree and PlayerBinarySearchTree Classes

- **GameBinarySearchTree**: A BST structure specifically for managing `Game` objects.
- **PlayerBinarySearchTree**: A BST structure for managing `Player` objects.

Both classes support the following functions:

1. `insert` - Insert a new record into the tree.
2. `search` - Search for a record by ID.
3. `delete` - Delete a record by ID.
4. `display` - Display all records in the tree.
5. `displayNLayer` - Display tree layers up to a specified depth.
6. `saveToCSV` - Save records into a CSV file.

## Installation and Setup

1. Clone the repository.
    ```bash
    git clone https://github.com/your-username/your-repo-name.git
    ```
2. Compile the project.
    ```bash
    g++ main.cpp -o bst_project
    ```
3. Run the compiled file.
    ```bash
    ./bst_project
    ```

## Usage

### Loading Data

To load game and player data from CSV files:

```cpp
GameBinarySearchTree gameBST;
loadGames("games.csv", gameBST);

PlayerBinarySearchTree playerBST;
loadPlayers("players.csv", playerBST);
