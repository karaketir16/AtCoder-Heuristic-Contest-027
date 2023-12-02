#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <queue>
#include <vector>
#include <set>  
#include <random>
#include <cassert>
#include <numeric>
#include <chrono>


using namespace std;

typedef long long int ll;

#define MAX_N 42
#define MAX_LENGTH 100000

#define SEED 0


std::chrono::steady_clock::time_point beginTime;

void setBeginTime(){
    beginTime = std::chrono::steady_clock::now();
}

ll getElapsedTimeMilli(){
    std::chrono::steady_clock::time_point endTime = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(endTime - beginTime).count();
}



// std::random_device rd;
std::mt19937 g(SEED);


enum Direction {
    UP = 0,
    DOWN,
    LEFT,
    RIGHT,
    MAX_DIRECTION
};

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

vector<Direction> getReversePath(vector<Direction> path) {
    vector<Direction> reversePath;
    for(auto direction: path) {
        reversePath.push_back(getReverseDirection(direction));
    }
    reverse(reversePath.begin(), reversePath.end());
    return reversePath;
}

const vector<Direction> defaultDirections = {DOWN, RIGHT, UP, LEFT};

vector<Direction> someDirections = defaultDirections;

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

    bool operator==(const Position& pos) const {
        return this->i == pos.i && this->j == pos.j;
    }
    bool operator!=(const Position& pos) const {
        return this->i != pos.i || this->j != pos.j;
    }
};


class Storage {
private:
    int n;
    bool horizontal[MAX_N][MAX_N - 1];
    bool vertical[MAX_N - 1][MAX_N];
    ll dirt[MAX_N][MAX_N];
    ll currentDirt[MAX_N][MAX_N];
    // set<Position> notVisited;
    bool visited[MAX_N][MAX_N];
    vector<Direction> innerPath;

    Position currentPos;

    ll totalDirt[MAX_N][MAX_N];

public:
    Storage(int n): n(n), currentPos(0, 0) {
        std::string s;
        
        for(int i = 0; i < n - 1; i++) {
            cin >> s;
            #if DEVEL
            cerr << "horizontal: "<< s << endl;
            #endif
            for(int j = 0; j < n; j++) {
                #if DEVEL_ASSERT
                assert(s[j] == '0' || s[j] == '1' && "Invalid input");
                #endif
                this->horizontal[i][j] = s[j] == '1' ? 1 : 0;
            }
        }
        for(int i = 0; i < n; i++) {
            cin >> s;
            #if DEVEL
            cerr << "vertical: "<< s << endl;
            #endif
            for(int j = 0; j < n - 1; j++) {
                #if DEVEL_ASSERT
                assert(s[j] == '0' || s[j] == '1' && "Invalid input");
                #endif
                this->vertical[i][j] = s[j] == '1' ? 1 : 0;
            }
        }
        for(int i = 0; i < n; i++) {
            for(int j = 0; j < n; j++) {
                cin >> this->dirt[i][j];
                this->currentDirt[i][j] = 0;
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

    void dfs(Position& pos, Position dest = Position(-1, -1)) {
        if(pos == dest) {
            throw "Found the destination";
        } 

        this->visited[pos.i][pos.j] = true;
        for(auto direction: defaultDirections) {
            Position nextPos = pos.getPosDirection(direction);
            if(this->isValid(pos, nextPos) && !this->visited[nextPos.i][nextPos.j]) {

                updateDirtiness();
                this->currentDirt[nextPos.i][nextPos.j] = 0;
                this->innerPath.push_back(direction);

                this->dfs(nextPos, dest);
                this->innerPath.push_back(getReverseDirection(direction));
                updateDirtiness();
                this->currentDirt[pos.i][pos.j] = 0;   
            } 
        }
    }

    //visualize the map, horizontal and vertical walls
    void visualize() {
        for(int i = 0; i < this->n; i++) {
            for(int j = 0; j < this->n; j++) {
                cout << this->dirt[i][j] << (vertical[i][j] == 1 ? "X" : "I") << "\t";
            }
            cout << endl;

            if(i == this->n - 1) {
                continue;
            }
            for (int j = 0; j < this->n; j++) {
                cout << (horizontal[i][j] == 1 ? "X" : "I") << "\t";
            }
            cout << endl;
        }
    }

    void visualizeVisited() {
        for(int i = 0; i < this->n; i++) {
            for(int j = 0; j < this->n; j++) {
                cout << this->visited[i][j] << "\t";
            }
            cout << endl;
        }
    }

    void printPositions() {
        Position pos = Position(0, 0);
        for(auto direction: this->innerPath) {
            cout << pos.i << " " << pos.j << endl;
            auto nextPos = pos.getPosDirection(direction);
            pos = nextPos;
        }
        cout << pos.i << " " << pos.j << endl;
    }

    auto getPath(){
        return this->innerPath;
    }

    void printPath() {
        for(auto &direction: this->innerPath) {
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

    void resetVisited() {
        for (int i = 0; i < MAX_N; i++) {
            for(int j = 0; j < MAX_N; j++) {
                this->visited[i][j] = false;
            }
        }
    }

    void resetPath() {
        this->innerPath.clear();
    }

    void resetTotalDirt(){
        for(int i = 0; i < MAX_N; i++) {
            for(int j = 0; j < MAX_N; j++) {
                this->totalDirt[i][j] = 0;
            }
        }
    }

    int getPathLength() {
        return this->innerPath.size();
    }


    void updateDirtiness() {
        // Update the dirtiness for each square
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                currentDirt[i][j] += dirt[i][j];
            }
        }
    }

    ll getDirt(Position pos) {
        return this->dirt[pos.i][pos.j];
    }

    ll getCurrentDirt(Position pos) {
        return this->dirt[pos.i][pos.j];
    }

    void setVisited(Position pos) {
        this->visited[pos.i][pos.j] = true;
    }

    bool isVisited(Position pos) {
        return this->visited[pos.i][pos.j];
    }
    
    auto getCurrentPos() {
        return this->currentPos;
    }

    void goDirection(Direction direction) {
        this->innerPath.push_back(direction);
        this->currentPos = this->currentPos.getPosDirection(direction);
    }

};


Storage *storage;
int N;

struct Tree {
    Position pos;
    Tree* parent;
    vector<Tree*> children;
    ll maxDirtInTree;

    Tree(Position pos, Tree* parent = nullptr): pos(pos), parent(parent), totalDirt(0) {}

    void addChild(Tree* child) {
        this->children.push_back(child);
    }

    static Tree* zeroPtr;

    
    // generate tree using bfs
    void generateTree() {

        if(this->pos == Position(0, 0)) {
            zeroPtr = this;
        } 

        vector<Direction> directions = {DOWN, RIGHT, UP, LEFT};
        vector<Position> positions = {this->pos.getPosDirection(DOWN), this->pos.getPosDirection(RIGHT), this->pos.getPosDirection(UP), this->pos.getPosDirection(LEFT)};
        for(int i = 0; i < MAX_DIRECTION; i++) {
            if(storage->isValid(this->pos, positions[i]) && !storage->isVisited(positions[i])) {
                Tree* child = new Tree(positions[i], this);
                this->addChild(child);
                storage->setVisited(positions[i]);
                child->generateTree();
            }
        }

        for(auto child: this->children) {
            if(child->maxDirtInTree > this->maxDirtInTree) {
                this->maxDirtInTree = child->maxDirtInTree;
            }
        }
    }

    pair<Direction, Tree*> maxValDirection() {
        ll maxVal = -1;
        Direction maxDirection = MAX_DIRECTION;
        Tree *maxChild = nullptr;
        for(auto child: this->children) {
            if(child->maxDirtInTree > maxVal) {
                maxVal = child->maxDirtInTree;
                maxDirection = this->pos.getDirection(child->pos);
                maxChild = child;
            }
        }
        return make_pair(maxDirection, maxChild);
    }
};

Tree* Tree::zeroPtr = nullptr;


ll bestAvgDirtiness = INT64_MAX;
vector<Direction> bestPath;


int main()
{
    setBeginTime();

    cin >> N;

    storage = new Storage(N);

    Position pos(0, 0);
    storage->dfs(pos);

    while(storage->getPathLength() < (MAX_LENGTH - 2000) && getElapsedTimeMilli() < 1900){

        storage->resetVisited();


        Tree* root = new Tree(storage->getCurrentPos());
        storage->setVisited(storage->getCurrentPos());
        storage->updateDirtiness();
        root->generateTree();
            
        auto [direction, child] = root->maxValDirection();

        storage->goDirection(direction);
    }

    //TODO run parent parent from zeroPtr to root, then reverse the path

    try {
        storage->resetVisited();
        auto pos = storage->getCurrentPos();
        storage->dfs(pos, Position(0, 0));
    } catch (const char* msg) {
        // cerr << msg << endl;
    }

    storage->printPath();

    
    return 0;
}

