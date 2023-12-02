#include <iostream>
#include <vector>
#include <set>  

using namespace std;

#define MAX_N 40

enum Direction {
    UP = 0,
    DOWN,
    LEFT,
    RIGHT,
    MAX_DIRECTION
};

auto directions = {UP, DOWN, LEFT, RIGHT};

struct Position {
    int i;
    int j;

    Position(int i, int j): i(i), j(j) {}

    Position getPosDirection(Direction direction) {
        switch(direction) {
            case UP:
                return Position(this->i - 1, this->j);
            case DOWN:
                return Position(this->i + 1, this->j);
            case LEFT:
                return Position(this->i, this->j - 1);
            case RIGHT:
                return Position(this->i, this->j + 1);
            default:
                return Position(this->i, this->j);
        }
    }

    Direction getDirection(Position pos){
        if(this->i == pos.i) {
            if(this->j > pos.j) {
                return LEFT;
            } else {
                return RIGHT;
            }
        } else if (this->j == pos.j){
            if(this->i > pos.i) {
                return UP;
            } else {
                return DOWN;
            }
        } else {
            return MAX_DIRECTION;
        }
    }

    Direction getReverseDirection(Direction direction) {
        switch(direction) {
            case UP:
                return DOWN;
            case DOWN:
                return UP;
            case LEFT:
                return RIGHT;
            case RIGHT:
                return LEFT;
            default:
                return MAX_DIRECTION;
        }
    }
};

class Storage {
private:
    int n;
    bool horizontal[MAX_N][MAX_N - 1];
    bool vertical[MAX_N - 1][MAX_N];
    int dirt[MAX_N][MAX_N];
    // set<Position> notVisited;
    bool visited[MAX_N][MAX_N];
    vector<Direction> path;

    Position currentPos;
public:
    Storage(int n): n(n), currentPos(0, 0) {
        std::string s;
        
        for(int i = 0; i < n; i++) {
            cin >> s;
            for(int j = 0; j < n - 1; j++) {
                this->horizontal[i][j] = s[j] == '1' ? 1 : 0;
            }
        }
        for(int i = 0; i < n - 1; i++) {
            cin >> s;
            for(int j = 0; j < n; j++) {
                this->vertical[i][j] = s[j] == '1' ? 1 : 0;
            }
        }
        for(int i = 0; i < n; i++) {
            for(int j = 0; j < n; j++) {
                cin >> this->dirt[i][j];
            }
        }
        for (int i = 0; i < n; i++) {
            for(int j = 0; j < n; j++) {
                this->visited[i][j] = false;
            }
        }
    }

    bool isValid(Position pos1, Position pos2) {
        if(pos2.i < 0 || pos2.i >= this->n || pos2.j < 0 || pos2.j >= this->n) {
            return false;
        }
        Direction direction = pos1.getDirection(pos2);
        switch (direction) {
            case UP:
                return !this->horizontal[pos1.i - 1][pos1.j];
            case DOWN:
                return !this->horizontal[pos1.i][pos1.j];
            case LEFT:
                return !this->vertical[pos1.i][pos1.j - 1];
            case RIGHT:
                return !this->vertical[pos1.i][pos1.j];
            default:
                return false;  
        }
    }

    void dfs(Position pos) {
        this->visited[pos.i][pos.j] = true;
        for(auto direction: directions) {
            Position nextPos = pos.getPosDirection(direction);
            if(this->isValid(pos, nextPos) && !this->visited[nextPos.i][nextPos.j]) {
                this->path.push_back(direction);
                this->dfs(nextPos);
                this->path.push_back(pos.getReverseDirection(direction));
            }
        }
    }

    //visualize the map, horizontal and vertical walls
    void visualize() {
        for(int i = 0; i < this->n; i++) {
            for(int j = 0; j < this->n; j++) {
                cout << this->dirt[i][j] << (vertical[i][j] == 1 ? "X" : "") << "\t";
            }
            cout << endl;

            if(i == this->n - 1) {
                continue;
            }
            for (int j = 0; j < this->n; j++) {
                cout << (horizontal[i][j] == 1 ? "X" : "") << "\t";
            }
            cout << endl;
        }
    }

    void printPositions() {
        Position pos = Position(0, 0);
        for(auto direction: this->path) {
            cout << pos.i << " " << pos.j << endl;
            auto nextPos = pos.getPosDirection(direction);
            pos = nextPos;
        }
        cout << pos.i << " " << pos.j << endl;
    }

    void printPath() {
        for(auto direction: this->path) {
            switch(direction) {
                case UP:
                    cout << "U";
                    break;
                case DOWN:
                    cout << "D";
                    break;
                case LEFT:  
                    cout << "L";
                    break;  
                case RIGHT:
                    cout << "R";
                    break;
                default:    
                    cout << "X";
                    break;
            }
        }
        cout << endl;
    }
    
};

int main()
{
    int N;
    cin >> N;
    Storage storage(N);

    storage.dfs(Position(0, 0));

    // cout << "Done dfs" << endl;
    // storage.printPositions();
    // storage.visualize();

    storage.printPath();


    return 0;
}
