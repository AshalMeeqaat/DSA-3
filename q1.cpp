#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;
//Game class to have its members
class Game {
public:
    string Game_ID;
    string Name;
    string Developer;
    string Publisher;
    string File_Size_in_GBs;
    string Downloads;
    Game* left;
    Game* right;
    //Game constructor
    Game(string gameID, string name, string developer, string publisher, string fileSize, string downloads)
        : Game_ID(gameID), Name(name), Developer(developer), Publisher(publisher), 
          File_Size_in_GBs(fileSize), Downloads(downloads), left(nullptr), right(nullptr) {}
};


// To form search tree
class GameBinarySearchTree {
public:
    Game* root;

    GameBinarySearchTree() : root(nullptr) {}

    void insert(Game* newGame) {
        root = insertnode(root, newGame);
    }

    Game* insertnode(Game* node, Game* newGame) {
        if (node == nullptr) return newGame;
        if (newGame->Game_ID == node->Game_ID) {
            cout << "Game with ID " << newGame->Game_ID << " already exists \n";
            return node;  // Handles duplicate
        }
        //if ID less than root then shift left
        if (newGame->Game_ID < node->Game_ID)
            node->left = insertnode(node->left, newGame);
        else
            node->right = insertnode(node->right, newGame);  //if ID greater than root then shift right

        return node;
    }

    void Display(Game* node) {
        if (node == nullptr) return;
        Display(node->left);
        cout << "----------------------------------------------" << endl;
        cout << "Game ID: " << node->Game_ID << endl;
        cout << "Game name: " << node->Name << endl;
        cout << "Developer: " << node->Developer << endl;
        cout << "Publisher: " << node->Publisher << endl;
        cout << "File in GBs: " << node->File_Size_in_GBs << endl;
        cout << "Downloads: " << node->Downloads << endl;
        Display(node->right);
    }

    // Search for a game in BST by its id
    Game* search(string gameID) {
        return searchRec(root, gameID);
    }
    // Helper function for searching in BST
    Game* searchRec(Game* node, string gameID) {
        // If root null return 
        if (node == nullptr) return nullptr;
        // if matches ID return node
        if (node->Game_ID == gameID) return node;
        // Else iterate left or right
        if (gameID < node->Game_ID) return searchRec(node->left, gameID);
        return searchRec(node->right, gameID);
    }

    Game* deleteGame(Game* root, string gameID) {
        // Base condition
        if (root == nullptr) return root;

        // Iterate through BST 
        if (gameID < root->Game_ID) {
            root->left = deleteGame(root->left, gameID);
        } else if (gameID > root->Game_ID) {
            root->right = deleteGame(root->right, gameID);
        } else {
            // Node with the gameID found
            //If node has no child delete directly
            if (root->left == nullptr && root->right == nullptr) {
                delete root;
                return nullptr;
            }

            // if node has one child save it in temp
            if (root->left == nullptr) {
                Game* temp = root->right;
                delete root;
                return temp;
            } else if (root->right == nullptr) {
                Game* temp = root->left;
                delete root;
                return temp;
            }

            // If has two child then find minimum replace and then delete
            Game* temp = minValueNode(root->right);
            root->Game_ID = temp->Game_ID;
            root->right = deleteGame(root->right, temp->Game_ID);
        }
        return root;
    }

    // To find minimum value in right subtree to replace
    Game* minValueNode(Game* node) {
        Game* current = node;
        while (current && current->left != nullptr) {
            current = current->left;
        }
        return current;
    }

    // Delete game
    void deleteGame(string gameID) {
        root = deleteGame(root, gameID);
    }
    // Save data in csv file in preorder 
    void savePreorder(Game* node, ofstream& file) {
        // If null return 
        if (node == nullptr) return;
        // Store root first then left right
        file << node->Game_ID << "," << node->Name << "," << node->Developer << "," 
             << node->Publisher << "," << node->File_Size_in_GBs << "," << node->Downloads << endl;
        savePreorder(node->left, file);
        savePreorder(node->right, file);
    }

    // To make a csv file and save data
    void saveGamesToCSV(const string& filename) {
        ofstream file(filename);
        savePreorder(root, file);
        file.close();
    }
    // Find depth to help with layers
    int getMaxDepth(Game* node) {
        if (node == nullptr) {
            return 0;  // If the node is null
        }

        // calculate the depth of both subtrees
        int leftDepth = getMaxDepth(node->left);
        int rightDepth = getMaxDepth(node->right);
        
        // Return the larger of the two depths + 1 
        if (leftDepth > rightDepth) {
            return leftDepth + 1;
        } else {
            return rightDepth + 1;
        }
    }
    // Helper function to print values at current layer
    void printAtLevel(Game* node, int level) {
        if (node == nullptr) return;
        if (level == 0) {
            cout << "Game ID: " << node->Game_ID << " | Name: " << node->Name << endl;
        } else {
            printAtLevel(node->left, level - 1);
            printAtLevel(node->right, level - 1);
        }
    }

    // display layers up to N
    void displayNLayer(int N) {
        if (root == nullptr) {
            cout << "Tree is empty." << endl;
            return;
        }

        // Calculate the maximum depth
        int maxDepth = getMaxDepth(root);

        // Base case if user enters more layer
        if (N > maxDepth) {
            cout << "Cannot display " << N << " layers. The tree only has " << maxDepth << " layers." << endl;
            return;
        }

        // Display layers up to N
        for (int currentLayer = 0; currentLayer < N; ++currentLayer) {
            cout << "Layer " << currentLayer << ":" << endl;
            printAtLevel(root, currentLayer);
            cout << "----------------------" << endl;
        }
    }
    // Helper function to find the layer number of the id
    int findLayer(Game* node, const string& gameID, int level) {
        if (node == nullptr) return -1;  // not found

        if (node->Game_ID == gameID) return level;  // Found at start

        // Iterate through both sides
        int leftLevel = findLayer(node->left, gameID, level + 1);
    
        if (leftLevel != -1) return leftLevel;
        // Iterate through right side
        return findLayer(node->right, gameID, level + 1);
    }

    // To find the current layer of ID
    int getGameLayer(const string& gameID) {
        return findLayer(root, gameID, 0);  
    }
    // SHow the path it takes to reach the user entered value
    void showPreorderPath(Game* node, string gameID) {
    if (node == nullptr) return;

    // The id which we looking for
    cout << "Visiting Game ID: " << node->Game_ID << endl;

    // Base case if found
    if (node->Game_ID == gameID) {
        cout << "Found Game ID: " << node->Game_ID << endl;
        return;
    }

    // Iterate left and right
    if (gameID < node->Game_ID)
        showPreorderPath(node->left, gameID);
    else
        showPreorderPath(node->right, gameID);
}
    // Function to show the path it takes
void showGamePreorderPath(string gameID) {
    cout << "Searching for Game ID: " << gameID << endl;
    showPreorderPath(root, gameID);
}
    
    
};

// Load into a BST 
void loadGames(const string& filename, GameBinarySearchTree& gameBST) {
    ifstream file(filename);
    string line, gameID, name, developer, publisher, fileSizeStr, downloadsStr;


    while (getline(file, line)) {  


        stringstream ss(line);
        getline(ss, gameID, ',');
        getline(ss, name, ',');
        getline(ss, developer, ',');
        getline(ss, publisher, ',');
        getline(ss, fileSizeStr, ',');
        getline(ss, downloadsStr, ',');


        Game* newGame = new Game(gameID, name, developer, publisher, fileSizeStr, downloadsStr);
        gameBST.insert(newGame);
    }
    file.close();
}
//Player class to have its members
class Player {
public:
    string Player_ID;
    string Name;
    string Phone_Number;
    string Email;
    string Password;
    Player* left;
    Player* right;
    //Player constructor
    Player(string id, string name, string phone, string email, string password)
        : Player_ID(id), Name(name), Phone_Number(phone), Email(email), 
          Password(password), left(nullptr), right(nullptr) {}
};

// To form search tree
class PlayerBinarySearchTree {
public:
    Player* root;

    PlayerBinarySearchTree() : root(nullptr) {}

    void insert(Player* newPlayer) {
        root = insertnode(root, newPlayer);
    }

    Player* insertnode(Player* node, Player* newPlayer) {
        if (node == nullptr) return newPlayer;
        if (newPlayer->Player_ID == node->Player_ID) {
            cout << "Player with ID " << newPlayer->Player_ID << " already exists\n";
            return node;  // Handles duplicate
        }

        //if ID less than root then shift left
        if (newPlayer->Player_ID < node->Player_ID)
            node->left = insertnode(node->left, newPlayer); 
        else
            node->right = insertnode(node->right, newPlayer);  //if ID greater than root then shift right

        return node;
    }

    void Display(Player* node) {
        if (node == nullptr) return;
        Display(node->left);
        cout << "-----------------------------------------"<< endl;
        cout << "Player ID: " << node->Player_ID << endl;
        cout << "Player name: " << node->Name << endl;
        cout << "Player phone number: " << node->Phone_Number << endl;
        cout << "Player email: " << node->Email << endl;
        cout << "Player Password: " << node->Password << endl;
        cout << "-----------------------------------------"<< endl;
        Display(node->right);
    }
// Search for a player in BST by its id
    Player* search(string playerID) {
        return searchRec(root, playerID);
    }

    // Helper function for searching in BST
    Player* searchRec(Player* node, string gameID) {
        // If root null return 
        if (node == nullptr) return nullptr;
        // if matches ID return node
        if (node->Player_ID == gameID) return node;
        // Else iterate left or right
        if (gameID < node->Player_ID) return searchRec(node->left, gameID);
        return searchRec(node->right, gameID);
    }
    // To delete a node
    Player* deletePlayer(Player* root, string playerID) {
        if (root == nullptr) return root;

        // Find the node to be deleted
        if (playerID < root->Player_ID) {
            root->left = deletePlayer(root->left, playerID);
        } else if (playerID > root->Player_ID) {
            root->right = deletePlayer(root->right, playerID);
        } else {
            // Node with the playerID found
            // if has no child delete it directly
            if (root->left == nullptr && root->right == nullptr) {
                delete root;
                return nullptr;
            }
            // If has one child save it
            if (root->left == nullptr) {
                Player* temp = root->right;
                delete root;
                return temp;
            } else if (root->right == nullptr) {
                Player* temp = root->left;
                delete root;
                return temp;
            }

            // if has two child find minimum to replace
            Player* temp = minValueNode(root->right);
            root->Player_ID = temp->Player_ID;
            root->right = deletePlayer(root->right, temp->Player_ID);
        }
        return root;
    }

    // To find minimum value in right subtree
    Player* minValueNode(Player* node) {
        Player* current = node;
        while (current && current->left != nullptr) {
            current = current->left;
        }
        return current;
    }

    // Main function to call deletion by id
    void deletePlayer(string playerID) {
        root = deletePlayer(root, playerID);
    }
    // function to save data into csv in preorder
    void savePreorder(Player* node, ofstream& file) {
        if (node == nullptr) return; // if null return 
        // else start from root then left then right
        file << node->Player_ID << "," << node->Name << "," << node->Phone_Number << "," 
             << node->Email << "," << node->Password << endl;
        savePreorder(node->left, file);
        savePreorder(node->right, file);
    }

    // Main function to make a csv file 
    void savePlayersToCSV(const string& filename) {
        ofstream file(filename);
        savePreorder(root, file);
        file.close();
    }
    // Find max or depth to help with the layers
    int getMaxDepth(Player* node) {
        if (node == nullptr) {
            return 0;  // If the node is null depth is 0
        }

        // Calculate depth of both subtrees
        int leftDepth = getMaxDepth(node->left);
        int rightDepth = getMaxDepth(node->right);
        
        // Return the larger side and add one for depth
        if (leftDepth > rightDepth) {
            return leftDepth + 1;
        } else {
            return rightDepth + 1;
        }
    }

    // Helper Function to display the current values of node in layer
    void printAtLevel(Player* node, int level) {
        if (node == nullptr) return;
        if (level == 0) {
            cout << "Player ID: " << node->Player_ID << " | Name: " << node->Name << endl;
        } else {
            printAtLevel(node->left, level - 1);
            printAtLevel(node->right, level - 1);
        }
    }

    // To display N layers
    void displayNLayer(int N) {
        if (root == nullptr) {
            cout << "Tree is empty" << endl;
            return;
        }

        // Find depth
        int maxDepth = getMaxDepth(root);

        // Base case if user enters invalid number 
        if (N > maxDepth || N < 0) {
            cout << "Error: Cannot display " << N << " layers. The tree only has " << maxDepth << " layers." << endl;
            return;
        }

        for (int currentLayer = 0; currentLayer < N; ++currentLayer) {
            cout << "Layer " << currentLayer << ":" << endl;
            printAtLevel(root, currentLayer);
            cout << "----------------------" << endl;
        }
    }
    // To find layer of a specific node
    int findLayer(Player* node, const string& playerID, int level) {
        if (node == nullptr) return -1;  // Player not found

        if (node->Player_ID == playerID) return level;  // Player found at base level

        // Iterate through both sides
        int leftLevel = findLayer(node->left, playerID, level + 1);
        if (leftLevel != -1) return leftLevel;

        return findLayer(node->right, playerID, level + 1);
    }

    // Main function to find current layer of ID
    int getPlayerLayer(const string& playerID) {

        return findLayer(root, playerID, 0);  
    }
    // Helper function to find the preorder path
    void showPreorderPath(Player* node, string playerID) {
    if (node == nullptr) return;

    cout << "Visiting Player ID: " << node->Player_ID << endl;

    // Base case if it matched then return
    if (node->Player_ID == playerID) {
        cout << "Found Player ID: " << node->Player_ID << endl;
        return;
    }

    // Iterate through both sides
    if (playerID < node->Player_ID)
        showPreorderPath(node->left, playerID);
    else
        showPreorderPath(node->right, playerID);
}
// Main function to find preorder paths both sides
void showPlayerPreorderPath(string playerID) {
    cout << "Searching for Player ID: " << playerID << endl;
    showPreorderPath(root, playerID);
}
};
// Function to load players from the txt file to BST
void loadPlayers(const string& filename, PlayerBinarySearchTree& playerBST, int skipThreshold) {
    ifstream file(filename);
    string line, playerID, name, phone, email, password;

    srand(232586);  // Seed for random number generation

    while (getline(file, line)) {
        int randomNum = rand() % 1001;  // Random number from 0 to 1000

        // If its below then skip that current line
        if (randomNum < skipThreshold) continue;

        stringstream ss(line);
        getline(ss, playerID, ',');
        getline(ss, name, ',');
        getline(ss, phone, ',');
        getline(ss, email, ',');
        getline(ss, password, ',');

        Player* newPlayer = new Player(playerID, name, phone, email, password);
        playerBST.insert(newPlayer);
    }
    file.close();
}
int main() {
    // Create both Binary Trees
    PlayerBinarySearchTree playerBST;
    GameBinarySearchTree gameBST;
    // 232586
    int skipThreshold = (86 * 10 + 100);  

    //Load data and that makes BST
    loadPlayers("Players.txt", playerBST, skipThreshold);
    loadGames("Games.txt", gameBST);

    // SEARCHING 
    string searchPlayerID ;  
    string searchGameID ; 
    cout<<"Enter the ID of player to search: ";
    cin>>searchPlayerID;
    cout<<"Enter the ID of Game to search: ";
    cin>>searchGameID;

    Player* foundPlayer = playerBST.search(searchPlayerID);
    if (foundPlayer) {
        cout << "\nFound Player: " << foundPlayer->Name << endl;
    } else {
        cout << "Player not found"<<endl;
    }

    Game* foundGame = gameBST.search(searchGameID);
    if (foundGame) {
        cout << "\nFound Game: " << foundGame->Name << endl;
    } else {
        cout << "Game not found"<<endl;
    }
    
    // DELETION
    string deletePlayerID ;
    string deleteGameID ;

    cout<<"Enter the ID of player to delete: ";
    cin>>deletePlayerID;
    cout<<"Enter the ID of Game to delete: ";
    cin>>deleteGameID;

    cout << "\nDeleting Player with ID: " << deletePlayerID << endl;
    playerBST.deletePlayer(deletePlayerID);

    cout << "\nDeleting Game with ID: " << deleteGameID << endl;
    gameBST.deleteGame(deleteGameID);

    // MAKE CSV FILES
    gameBST.saveGamesToCSV("games_output.csv");
    playerBST.savePlayersToCSV("players_output.csv");

    
    //gameBST.displayNLayer(1);
    playerBST.displayNLayer(10);
    string playerID = "9063655097";
    playerBST.showPlayerPreorderPath("9063655097");
    int layer = playerBST.getPlayerLayer(playerID);
    if (layer == -1) {
        cout << "Player with ID " << playerID << " not found in the tree.\n";
    } else {
        cout << "Player with ID " << playerID << " is at layer " << layer << ".\n";
    }
    // Display the players and games in order
    // cout << "Players in order:\n";
    // playerBST.Display(playerBST.root);
    // cout << "\nGames in order:\n";
    // gameBST.Display(gameBST.root);

    // Example: Searching for a Player and Game by ID
       // Replace with actual ID for testing

    

    

    return 0;
}
