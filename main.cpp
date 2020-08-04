#include <iostream>
#include <ctime>
#include <random>
#include <stdlib.h>
#include <conio.h>

enum BlockStatus {
    HIDDEN,  //  *
    VISIBLE, //  Number or '.'
    FLAG,    //  !
    QMARK    //  ?
};

class Block {
private:
    int *x;
    int *y;
    int *surroundingBombs;


    BlockStatus *status;
public:


public:
    bool isBomb = false;

    Block(int x, int y, BlockStatus status) {
        this->x = new int(x);
        this->y = new int(y);
        this->status = new BlockStatus(status);
    }

    Block() {
        surroundingBombs = new int;
        *surroundingBombs = 0;
        status = new BlockStatus(HIDDEN);
    }

    ~Block() {
        delete (this->x);
        delete (this->y);
        delete (this->status);
        delete (surroundingBombs);
    }

    void setX(int x) {
        *this->x = x;
    }

    void setY(int y) {
        *this->y = y;
    }

    void init(int x, int y) {
        this->x = new int(x);
        this->y = new int(y);
    }

    void setXY(int givenX, int givenY) {
        *x = givenX;
        *y = givenY;
    }

    int getX() const {
        return *x;
    }

    int getY() const {
        return *y;
    }

    void incSurroundingBombs() {
        ++(*surroundingBombs);
    }

    int getSurroundingBombs() const {
        return *surroundingBombs;
    }

    void setSurroundingBombs(int bombs) {
        *surroundingBombs = bombs;
    }

    char getAsChar() {
        int bombs = getSurroundingBombs();
        switch (*(this->status)) {
            case HIDDEN:
                return '*';
            case VISIBLE:
                if (bombs == 0)
                    return '.';
            case FLAG:
                return '!';
            case QMARK:
                return '?';

            default:
                return 'x';
        }
    }

    BlockStatus getStatus() const {
        return *status;
    }

    void setStatus(BlockStatus givenStatus) {
        *Block::status = givenStatus;
    }


};

class GameBoard {
public:
    int ROW;
    int COL;
    int numberOfBombs;
    Block **blocks;

    GameBoard(int n, int m, int bomb) {
        this->ROW = n;
        this->COL = m;
        this->numberOfBombs = bomb;

        // allocating 2D Arrays of Block with given size
        blocks = new Block *[ROW];
        for (int i = 0; i < COL; ++i) {
            blocks[i] = new Block[COL];
        }

        // setting coordinates of each block according to array's index
        for (int i = 0; i < ROW; ++i) {
            for (int j = 0; j < COL; ++j) {
                blocks[i][j].init(i, j);
                blocks[i][j].setSurroundingBombs(0);


            }
        }
    }

    int getRow() const {
        return ROW;
    }

    int getCol() const {
        return COL;
    }

    void setBombs(int firstX, int firstY) {
        /*
         * first Y and X guarantees that
         * first choice is not a bomb
         */
        for (int i = 0; i < numberOfBombs; ++i) {
            int x = rand() % ROW;
            int y = rand() % COL;

            // conditions that random coordinates are not valid

            // same coordinates
            if (blocks[x][y].isBomb) {
                i--;
                continue;
            }

            //first choice of player should not be bomb
            if (x == firstX && y == firstY) {
                i--;
                continue;
            }


            blocks[x][y].isBomb = true;
        }

    }

    bool isValid(int x, int y) {
        /*
         *  returns true if given coordinates
         *  are in game board  :)
         */

        return x >= 0 &&
               x < getRow() &&
               y >= 0 &&
               y < getCol();
    }

    void countSurroundingBombs() {

        /*
         * counts number of bombs around the block
         * saves the number in block's field too
         */
        int count;

        int row = getRow();
        int col = getCol();

        for (int i = 0; i < row; ++i) {
            for (int j = 0; j < col; ++j) {

                count = 0;

                if (isValid(i + 1, j))
                    if (blocks[i + 1][j].isBomb)
                        count++;

                if (isValid(i, j + 1))
                    if (blocks[i][j + 1].isBomb)
                        count++;

                if (isValid(i - 1, j + 1))
                    if (blocks[i - 1][j + 1].isBomb)
                        count++;

                if (isValid(i + 1, j + 1))
                    if (blocks[i + 1][j + 1].isBomb)
                        count++;

                if (isValid(i - 1, j))
                    if (blocks[i - 1][j].isBomb)
                        count++;

                if (isValid(i, j - 1))
                    if (blocks[i][j - 1].isBomb)
                        count++;

                if (isValid(i - 1, j - 1))
                    if (blocks[i - 1][j - 1].isBomb)
                        count++;

                if (isValid(i + 1, j - 1))
                    if (blocks[i + 1][j - 1].isBomb)
                        count++;

                blocks[i][j].setSurroundingBombs(count);

            }
        }


    }

    void initBoard(int firstX, int firstY) {
        /*
         * randomizing bombs
         * then counting number of bombs
         * around the block
         */


        setBombs(firstX, firstY);
        countSurroundingBombs();
    }


};

void doNothing() {
    /*
     * this function does nothing
     * just here to thank you for 
     * reading this bullshit code
     * 
     * thanks
     */
}

class UI {

public:
    GameBoard *mainBoard;
    char **visibleBoard;
    int flagNumbers = 0;
    UI(GameBoard *givenBoard) {
        mainBoard = givenBoard;
        int row = mainBoard->getRow();
        int col = mainBoard->getCol();
        visibleBoard = new char *[row];
        for (int i = 0; i < row; ++i) {
            visibleBoard[i] = new char[col];
        }

        for (int i = 0; i < row; ++i) {
            for (int j = 0; j < col; ++j) {
                visibleBoard[i][j] = '*';
            }
        }
    }

    void RunUI() {

        bool userIsNotLost = true;
        bool isFirstChoice = true;
        int x, y;
        char userInput;
        while (userIsNotLost) {
            if(userWon()){
                printMainBoard();
                std::cout << "\n you won!";
                return;
            }
            printBoard();
            std::cout << std::endl <<
                      "Enter key ::\nr -> Reveal\nf -> !(Flag)\ns -> ?\nc -> Cheat\n";


            std::cin.ignore();
            std::cin >> userInput;

            switch (userInput) {
                case 'r':
                    std::cout << "Enter row then column\n";
                    std::cin >> x >> y;

                    --x;
                    --y;

                    if (isFirstChoice) {
                        mainBoard->initBoard(x, y);
                        isFirstChoice = false;
                        reveal(x, y);
                        break;
                    } else if (mainBoard->blocks[x][y].isBomb) {

                        visibleBoard[x][y] = '@';
                        printBoard();
                        std::cout << "\n!! You lost !!\n";
                        return;
                    } else if (!mainBoard->isValid(x, y)) {
                        std::cout << "Your input is not valid\n";
                        break;


                    } else if (mainBoard->blocks[x][y].getStatus() == VISIBLE) {

                        std::cout << "You can't choose same block\n";
                        break;
                    }

                    reveal(x, y);

                    break;

                case 'f': {
                    // flag menu
                    std::cout << "Enter row then column\n";
                    std::cin >> x >> y;

                    --x;
                    --y;
                    BlockStatus currentStatus = mainBoard->blocks[x][y].getStatus();
                    if (!mainBoard->isValid(x, y)) {
                        std::cout << "Your input is not valid\n";
                        break;
                    }


                    if (currentStatus == HIDDEN || currentStatus == FLAG) {
                        if (visibleBoard[x][y] != '!') {
                            visibleBoard[x][y] = '!';
                            mainBoard->blocks[x][y].setStatus(FLAG);
                            flagNumbers++;
                        } else {
                            visibleBoard[x][y] = '*';
                            mainBoard->blocks[x][y].setStatus(HIDDEN);
                            flagNumbers--;
                        }
                        break;
                    }

                    std::cout << "You can't mark that location\n";
                    break;
                }
                case 's': {
                    // question mark
                    std::cout << "Enter row then column\n";
                    std::cin >> x >> y;
                    --x;
                    --y;

                    if (!mainBoard->isValid(x, y)) {
                        std::cout << "Your input is not valid\n";

                    }

                    BlockStatus currentStatus = mainBoard->blocks[x][y].getStatus();
                    if (currentStatus == HIDDEN || currentStatus == QMARK) {
                        if (visibleBoard[x][y] != '?') {
                            visibleBoard[x][y] = '?';
                            mainBoard->blocks[x][y].setStatus(QMARK);
                        } else {
                            visibleBoard[x][y] = '*';
                            mainBoard->blocks[x][y].setStatus(HIDDEN);
                        }
                        break;
                    }

                    std::cout << "You can't mark that location\n";
                    break;
                }
                case 'c':
                    // Prints Main board
                    printMainBoard();
                    std::cout << '\n';

                    break;

                default:
                    std::cout << "wrong command! Enter again\n";
                    break;

            }
        }
    };

    bool userWon(){
        int row = mainBoard->getRow();
        int col = mainBoard->getCol();

        for (int i = 0; i < row; ++i) {
            for (int j = 0; j < col; ++j) {
                if(visibleBoard[i][j] == '*' && !mainBoard->blocks[i][j].isBomb)
                    return false;
            }
        }
        return true;
    }
    void printMainBoard() {
        int row = mainBoard->getRow();
        int col = mainBoard->getCol();
        std::cout << ' ';
        for (int i = 0; i < col; ++i) {
            std::cout << " " << i + 1;
        }
        for (int i = 0; i < row; ++i) {
            std::cout << "\n" << (i + 1);
            for (int j = 0; j < col; ++j) {
                std::cout << " ";
                if (mainBoard->blocks[i][j].isBomb)
                    std::cout << '@';
                else
                    std::cout << mainBoard->blocks[i][j].getSurroundingBombs();
            }
        }


    }

    void printBoard() {

        int row = mainBoard->getRow();
        int col = mainBoard->getCol();
        std::cout << ' ';
        for (int i = 0; i < col; ++i) {
            std::cout << " " << i + 1;
        }
        for (int i = 0; i < row; ++i) {
            std::cout << "\n" << (i + 1);
            for (int j = 0; j < col; ++j) {
                std::cout << " " << visibleBoard[i][j];
            }
        }

        std::cout <<'\n' << mainBoard->numberOfBombs - flagNumbers << " bombs left from " << mainBoard->numberOfBombs;
    }

    void reveal(int x, int y) {

        if (mainBoard->blocks[x][y].getStatus() == VISIBLE)
            return;
        /*
         * recursion ending condition
         */

        mainBoard->blocks[x][y].setStatus(VISIBLE);
        visibleBoard[x][y] = mainBoard->blocks[x][y].getSurroundingBombs() + '0';

        // Kinda DFS ? idk man just give me the point :| 

        if (mainBoard->blocks[x][y].getSurroundingBombs() == 0) {
            if (mainBoard->isValid(x + 1, y))
                reveal(x + 1, y);

            if (mainBoard->isValid(x, y + 1))
                reveal(x, y + 1);

            if (mainBoard->isValid(x - 1, y + 1))
                reveal(x - 1, y + 1);

            if (mainBoard->isValid(x + 1, y + 1))
                reveal(x + 1, y + 1);

            if (mainBoard->isValid(x - 1, y))
                reveal(x - 1, y);

            if (mainBoard->isValid(x, y - 1))
                reveal(x, y - 1);

            if (mainBoard->isValid(x - 1, y - 1))
                reveal(x - 1, y - 1);

            if (mainBoard->isValid(x + 1, y - 1))
                reveal(x + 1, y - 1);
        }
    }

};


int main() {
    srand((unsigned)time(0));
    std::cout << "Welcome to the Mine Sweeper game!\nEnter n(row) then m(col) for size of map" << std::endl;
    int n, m;
    std::cin >> n >> m;
    int numberOfBombs;
    std::cout << "Please enter number of bombs\n";

    do {
        std::cin >> numberOfBombs;
        if (numberOfBombs >= n * m)
            std::cout << "Bombs must be less than " << n * m << "Enter again!\n";
    } while (numberOfBombs >= n * m);


    GameBoard mainBoard(n, m, numberOfBombs);
    UI gameUI(&mainBoard);
    gameUI.RunUI();


    return 0;
}


