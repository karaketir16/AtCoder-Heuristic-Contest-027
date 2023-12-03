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

const vector<Direction> circleDirections = {RIGHT, DOWN, LEFT, LEFT, UP, UP, RIGHT, RIGHT};

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

    bool visited[MAX_N][MAX_N];
    vector<Direction> innerPath;

    int visitedCount;
    int cellCount;

    Position currentPos;
public:
    Storage(int n): n(n), currentPos(0, 0) {
        cellCount = n*n;

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
        resetVisited();
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

    void dfs(Position& pos, Position dest = Position(-1, -1), bool stopAllVisited = false) {
        this->setVisited(pos);
        if(pos == dest) {
            throw "Found the destination";
        } 

        if(stopAllVisited && this->isAllVisited()) {
            throw "All visited";
        }
        
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
        visitedCount = 0;
    }

    void resetPath() {
        this->innerPath.clear();
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
        return this->currentDirt[pos.i][pos.j];
    }

    void cleanCurrentDirt(Position pos) {
        this->currentDirt[pos.i][pos.j] = 0;
    }

    void setVisited(Position pos) {
        if(!this->visited[pos.i][pos.j]){
            this->visitedCount++;
        }
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

    void setPath(vector<Direction> path) {
        this->innerPath = path;
    }

    bool isAllVisited(){
        return this->visitedCount == this->cellCount;
    }

    Position maxCurrentDirtPos(){
        Position maxPos = Position(0, 0);
        ll maxDirt = currentDirt[0][0];
        for(int i = 0; i < this->n; i++) {
            for(int j = 0; j < this->n; j++) {
                if(currentDirt[i][j] > maxDirt) {
                    maxDirt = currentDirt[i][j];
                    maxPos = Position(i, j);
                }
            }
        }
        return maxPos;
    }

    void makeCircle(){
        for(auto direction: circleDirections){
            Position nextPos = this->currentPos.getPosDirection(direction);
            if(this->isValid(this->currentPos, nextPos)) {
                this->goDirection(direction);
                this->updateDirtiness();
                this->cleanCurrentDirt(this->getCurrentPos());
            }
        }
    }
};


Storage *storage;
int N;

struct Tree {
    Position pos;
    Tree* parent;
    vector<Tree*> children;
    ll maxDirtInTree;
    Tree *maxDirtInTreePtr;
    Tree *wayToMaxDirt;

    Tree(Position pos, Tree* parent = nullptr): pos(pos), parent(parent) {
        this->maxDirtInTree = storage->getCurrentDirt(pos);
        this->maxDirtInTreePtr = this;
        this->wayToMaxDirt = nullptr;
    }

    void addChild(Tree* child) {
        this->children.push_back(child);
    }

    static Tree* zeroPtr;

    
    // generate tree using bfs
    void generateTree() {

        queue<pair<int,Tree*>> nodeQueue;

        nodeQueue.push({0,this});
        storage->setVisited(this->pos);

        while(!nodeQueue.empty()) {
            auto [depth, currentNode] = nodeQueue.front();
            nodeQueue.pop();

            if(currentNode->pos == Position(0, 0)) {
                zeroPtr = currentNode;
            }

            vector<Position> positions;
            for(auto direction: someDirections) {
                positions.push_back(currentNode->pos.getPosDirection(direction));
            }
            
            for(int i = 0; i < MAX_DIRECTION; i++) {
                if(storage->isValid(currentNode->pos, positions[i]) && !storage->isVisited(positions[i])) {
                    Tree* child = new Tree(positions[i], currentNode);
                    currentNode->addChild(child);
                    storage->setVisited(positions[i]);
                    nodeQueue.push({depth + 1, child});
                }
            }

            if(currentNode->children.size() == 0) {
                Tree* currentTree = currentNode;
                currentTree->maxDirtInTree += storage->getDirt(currentTree->pos) * depth;
                while(currentTree->parent != nullptr) {
                    if(currentTree->maxDirtInTree > currentTree->parent->maxDirtInTree) {
                        currentTree->parent->maxDirtInTree = currentTree->maxDirtInTree;
                        currentTree->parent->maxDirtInTreePtr = currentTree->maxDirtInTreePtr;
                        currentTree->parent->wayToMaxDirt = currentTree;
                    }
                    currentTree = currentTree->parent;
                }
            }
        }
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
    try {
        storage->dfs(pos, Position(-1,-1), true);
    } catch (const char* msg) {

    }
    

    auto myLimit = storage->getPathLength() + N * N;

    bool circle = false;

    while(storage->getPathLength() < myLimit && getElapsedTimeMilli() < 1900){

        if(circle){
            storage->makeCircle();
            circle = false;
        }

        storage->resetVisited();

        Tree* root = new Tree(storage->getCurrentPos());        
        
        root->generateTree();

        // auto realMax = storage->maxCurrentDirtPos();
        // cerr << "Max dirtiness from real: " << storage->getCurrentDirt(realMax) << " at " << realMax.i << " " << realMax.j << endl;

        auto direction = root->pos.getDirection(root->wayToMaxDirt->pos);
        if(direction == MAX_DIRECTION) {
            direction = root->pos.getDirection(root->children[0]->pos);
        }

        if(root->wayToMaxDirt == root->maxDirtInTreePtr){
            circle = true;
            cerr << "Circle" << endl;
        }

        storage->goDirection(direction);
        storage->updateDirtiness();
        storage->cleanCurrentDirt(storage->getCurrentPos());
    }

    //parent from zeroPtr to root, then reverse the path
    storage->resetVisited();
    Tree *root = new Tree(storage->getCurrentPos());
    root->generateTree();

    vector<Direction> returnPath;
    Tree *currentTree = Tree::zeroPtr;
    while(currentTree->parent != nullptr) {
        auto dir = currentTree->pos.getDirection(currentTree->parent->pos);
        returnPath.push_back(dir);
        currentTree = currentTree->parent;
    }
    returnPath = getReversePath(returnPath);

    auto path = storage->getPath();
    path.insert(path.end(), returnPath.begin(), returnPath.end());

    storage->setPath(path);

    storage->printPath();

    
    return 0;
}

