#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <vector>
#include <set>  
#include <random>
#include <cassert>
#include <numeric>


#include <chrono>


using namespace std;

typedef long long int ll;


std::chrono::steady_clock::time_point beginTime;
std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();


void setBeginTime(){
    beginTime = std::chrono::steady_clock::now();
}

ll getElapsedTimeMilli(){
    std::chrono::steady_clock::time_point endTime = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(endTime - beginTime).count();
}


#define MAX_N 42
#define MAX_LENGTH 100000

#define SEED 0

#ifndef SHUFFLE
#define SHUFFLE 1
#endif

enum Direction {
    UP = 0,
    DOWN,
    LEFT,
    RIGHT,
    MAX_DIRECTION
};

vector<Direction> defaultDirections = {UP, DOWN, LEFT, RIGHT};

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

// std::random_device rd;
std::mt19937 g(SEED);

class Storage {
private:
    int n;
    bool horizontal[MAX_N][MAX_N - 1];
    bool vertical[MAX_N - 1][MAX_N];
    ll dirt[MAX_N][MAX_N];
    // set<Position> notVisited;
    bool visited[MAX_N][MAX_N];
    vector<Direction> path;

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

    void dfs(Position& pos, vector<Direction> directions = defaultDirections) {
        #if SHUFFLE
        std::shuffle(directions.begin(), directions.end(), g);
        #endif
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

    void printPositions() {
        Position pos = Position(0, 0);
        for(auto direction: this->path) {
            cout << pos.i << " " << pos.j << endl;
            auto nextPos = pos.getPosDirection(direction);
            pos = nextPos;
        }
        cout << pos.i << " " << pos.j << endl;
    }

    auto getPath(){
        return this->path;
    }

    void printPath(vector<Direction> &path) {
        for(auto &direction: path) {
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
        this->path.clear();
    }

    void resetTotalDirt(){
        for(int i = 0; i < MAX_N; i++) {
            for(int j = 0; j < MAX_N; j++) {
                this->totalDirt[i][j] = 0;
            }
        }
    }

    int getPathLength() {
        return this->path.size();
    }


    void updateDirtiness(int index) {
        // Update the dirtiness for each square
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                totalDirt[i][j] += dirt[i][j];
            }
        }
    }


    ll calculateAverageDirtiness() {
        for (int t = 0; t < 2 * getPathLength(); ++t) {
            updateDirtiness(t % getPathLength());
            // Update the current position based on the direction at the current index of the path
            this->currentPos = this->currentPos.getPosDirection(this->path[t % getPathLength()]);
            totalDirt[currentPos.i][currentPos.j] = 0;
        }

        ll sum = 0;
        for (int i = 0; i < n; i++) {
            sum += std::accumulate(totalDirt[i], totalDirt[i] + n, 0);
        }

        return sum / getPathLength();
    }

    
};

int main()
{
    setBeginTime();

    int N;
    cin >> N;
    Storage storage(N);

    Position pos(0, 0);

    ll bestAvgDirtiness = INT64_MAX;
    vector<Direction> bestPath;

    int tried = 0;

    while(getElapsedTimeMilli() < 1900){
        tried++;

        storage.resetVisited();
        storage.resetPath();
        storage.resetTotalDirt();

        storage.dfs(pos);

        if(storage.getPathLength() > MAX_LENGTH){
            continue;
        }



        ll dirtiness = storage.calculateAverageDirtiness();
        if(dirtiness < bestAvgDirtiness){
            bestAvgDirtiness = dirtiness;
            bestPath = storage.getPath();
        }

        #if not SHUFFLE
        break;
        #endif
    } 
    

    // cout << "Done dfs" << endl;
    // storage.printPositions();

    #if DEVEL
    storage.visualize();
    cerr << "Tried: " << tried << "\t Map Size: " << N << "\t Path Length: " << bestPath.size() << "\t Avg Dirtiness: " << bestAvgDirtiness << endl;
    #endif

    storage.printPath(bestPath);

    return 0;
}

