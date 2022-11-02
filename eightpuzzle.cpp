#include <iostream>
#include <vector>
#include <chrono>
#include <ratio>
#include <map>
#include <queue>


using namespace std;
using namespace std::chrono;

//Test cases from Project Documentation
vector<vector<int>> goal_st = {{1,2,3},{4,5,6},{7,8,0}};
vector<vector<int>> puzzle1 = {{1,2,3},{4,5,6},{7,8,0}};
vector<vector<int>> puzzle2 = {{1,2,3},{4,5,6},{0,7,8}};
vector<vector<int>> puzzle3 = {{1,2,3},{5,0,6},{4,7,8}};
vector<vector<int>> puzzle4 = {{1,3,6},{5,0,7},{4,8,2}};
vector<vector<int>> puzzle5 = {{1,3,6},{5,0,7},{4,8,2}};
vector<vector<int>> puzzle6 = {{1,6,7},{5,0,3},{4,8,2}};
vector<vector<int>> puzzle7 = {{7,1,2},{4,8,5},{6,3,0}};
vector<vector<int>> puzzle8 = {{0,7,2},{4,6,1},{3,5,8}};


/*node code to represent nodes/states
has 4 children representing expanded states (up, down, left, right)
*/
class node {

    public: 
        int depth = 0;
        int cost = 0;
        bool expanded = false;

        //4 children defaulted to null empty
        node* child1 = nullptr;
        node* child2 = nullptr;
        node* child3 = nullptr;
        node* child4 = nullptr;

        //node that holds its children
        node(vector<vector<int>> puzzle){
            this->state = puzzle;
        }

        //vector of states
        vector<vector<int>> state;   
        
        friend ostream & operator << (ostream &, const node&);
};

//print puzzle
//https://www.geeksforgeeks.org/operator-overloading-cpp-print-contents-vector-map-pair/
ostream & operator << (ostream &os, const node &n){
    for (int i = 0; i < n.state.size(); i++){
        for (int j = 0; j < n.state[i].size(); j++){
            os << n.state[i][j] << ' ';
        }
        os << endl;
    }
    return os;
}

//Referencing for priority queue: https://www.geeksforgeeks.org/stl-priority-queue-for-structure-or-class/
//operator to compare nodes for priority queue
struct Compare{
    bool operator()(node &n1, node &n2) const{
        return (n1.depth+n1.cost) > (n2.cost+n2.depth);
    }
};


int puzzlemenu();
node expand(node n, priority_queue<node>& pq);
bool goal(node n);
void generalsearch(vector<vector<int>>, int);
int misplacedTile(vector<vector<int>>);
int manhattanDist(vector<vector<int>>);


int main(){
     int choice;
    vector<vector<int>> puzzle;

    //setting up source puzzle
    puzzle = {{1,6,7},{5,0,3},{4,8,2}};

    choice = puzzlemenu();
    
    if(choice == 1){
        cout << "Uniform Cost Search" << endl;
        generalsearch(puzzle, choice);
    }
    else if (choice == 2){
        cout << "Misplaced Tile " << endl;
        generalsearch(puzzle, choice);
    }
    else if (choice == 3){
        cout << "Manhattan Distance" << endl;
        generalsearch(puzzle, choice);
    }
    return 0;
}

int puzzlemenu(){
    int choice;
    cout << "1. Uniform Cost Search\n2. A* w/ Misplaced Tile\n3. A* w/ Manhattan Distance\n4. Exit\n";
    cin >> choice;
    return choice;
}

// Expands a node (moves 0) in all legal ways
// using map:seen to track all visited states
// Referencing map: https://cplusplus.com/reference/map/map/
node expand(node n, map<vector<vector<int>>, int> seen){

    //tracks expanded directions
    vector<vector<int>> up;
    vector<vector<int>> down;
    vector<vector<int>> left;
    vector<vector<int>> right;
    vector<int> tempV;


    bool legal(true);
    int up0, down0, left0, right0;
    int pos0, tempSwap;
    int row(0), col(0);
    
    
    //check 0 position
    for (int i = 0; i < n.state.size(); i++){
        for (int j = 0; j < n.state[i].size(); j++){
            if (n.state[i][j] == 0){
                row = i;
                col = j;
            }
        }
    }
    //cout << "Expand row: " << row << "col: " << col << endl;

    //Copying vector to vector: https://www.geeksforgeeks.org/ways-copy-vector-c/
    //Checking Legal Moves
    //Up
    if (row != 0){
        //copy and push to up vector
        copy(n.state.begin(), n.state.end(), back_inserter(up));

        //swap 0 pos with up neighbor
        up0 = n.state[row-1][col];
        up[row-1][col] = 0;
        up[row][col] = up0;
    }

    //Down
    if (row != n.state.size()-1){
        //copy and push to down vector
        copy(n.state.begin(), n.state.end(), back_inserter(down));

        //swap 0 pos with down neighbor
        down0 = n.state[row+1][col];
        down[row+1][col] = 0;
        down[row][col] = down0;
    }

    //Left
    if (col != 0){
        //copy and push to up vector
        copy(n.state.begin(), n.state.end(), back_inserter(left));

        //swap 0 pos with up neighbor
        left0 = n.state[row][col-1];
        left[row][col-1] = 0;
        left[row][col] = left0;
    }

    //Right
    if (col != n.state[0].size()-1){
        //copy and push to up vector
        copy(n.state.begin(), n.state.end(), back_inserter(right));

        //swap 0 pos with up neighbor
        right0 = n.state[row][col+1];
        right[row][col+1] = 0;
        right[row][col] = right0;
    }

    //checking for empty & seen nodes
    if (up.size() != 0 && (seen.find(up) == seen.end())){
        n.child1 = new node(up);
    }
    if (down.size() != (0 && seen.find(down) == seen.end())){
        n.child1 = new node(down);
    }
    if (left.size() != (0 && seen.find(left) == seen.end())){
        n.child1 = new node(left);
    }
    if (right.size() != (0 && seen.find(right) == seen.end())){
        n.child1 = new node(right);
    }

    //returning orignal node, updated with new children (up to 4)
    return n;
}




void generalsearch(vector<vector<int>> problem, int queuefunc){
    //cout << "in general search" << endl;

    //p-queue to compare value of states
    //map for seen states
    priority_queue<node, vector<node>, Compare> nodes;
    map<vector<vector<int>>, int> seen;
    int h;
    int count = 0;
    int queuesize = 0;
    bool run = true;

    //source node initalization
    node n = node(problem);
    n.depth = 0;
    //starting to fill seen map
    seen[problem] = 1;
    nodes.push(n);
    
    //setting up h(n) based on problem choice
    if(queuefunc == 2){
        h = misplacedTile(problem);
    }
    else if (queuefunc == 3){
        h = manhattanDist(problem);
    }
    else{
        //Uniform Cost Search
        h = 0;
    }

    //timer start algo
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    while(run){
        //cout << "in while" << endl;

        //if empty, exit
        if (nodes.empty()){
            cout << "Exiting" << endl;
            run = false;
        }

        //start first state search
        node currN = nodes.top();
        nodes.pop();

        //checking goal state match
        if (goal(currN)){
            //cout << "in goal" << endl;
            run = false;
            cout << "Goal reached at: " << count << " nodes\nMax nodes in a queue at once: " << queuesize << "\nFinal Depth: " << currN.depth << endl;
            break;
        }

        if (count != 0){
            //cout << "expanding " << endl;
            cout << "The best state to expand with a g(n) = " << currN.depth << " and h(n) = " << currN.cost << " is \n" << currN << endl;
        } else {
             cout << "Expand: " << endl << currN.state << endl;
        }

        //mark node off as expanded
        currN.expanded = true;
        count += 1;

        //expand node and fill seen map
        //filling vector of children of current node
        node tempN = expand(currN, seen);
        vector<node*> currChildren = {tempN.child1, tempN.child2, tempN.child3, tempN.child4};

        //expand through children nodes
        //add to seen and p-qeue
        for (int i = 0; i < currChildren.size(); i++){
            if (currChildren[i] != nullptr){
                if (queuefunc == 1){
                    currChildren[i]->cost = tempN.cost;
                    currChildren[i]->depth = tempN.depth + 1;
                }
                else if (queuefunc == 2){
                    currChildren[i]->cost = misplacedTile(currChildren[i]->state);
                    currChildren[i]->depth = tempN.depth + 1;
                }
                else{
                    currChildren[i]->cost = manhattanDist(currChildren[i]->state);
                    currChildren[i]->depth = tempN.depth + 1;
                }

                
                //add puzzle to map if not seen before
                nodes.push(*currChildren[i]);
                if (seen.find(currChildren[i]->state) == seen.end()){
                    seen.insert({currChildren[i]->state, 1});
                }
            }
        }
        //update queuesize
        if (nodes.size() > queuesize){
            queuesize = nodes.size();
        }
    }  

    //done with searching for goal state
    //Tracking execution time: https://cplusplus.com/reference/chrono/high_resolution_clock/now/
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(t2-t1);

    //cout << "Found in: " << time_span.count() << endl;
    if  (time_span.count() < 1){
        cout << "Found in less than 1 sec " << endl;
    }
    else{
        cout << "Found in: " << time_span.count() << " secs " << endl;
    }

}


//check if curr node is the goal state
bool goal(node n){
    return(n.state == goal_st);
}


//calculate h based on misplaced tiles
int misplacedTile(vector<vector<int>> puzzle){
    int h = 0;

    //iterating through vector to find # of misplaced tiles
    for (int i = 0; i < goal_st.size(); i++){
        for (int j = 0; j < goal_st.size(); j++){
            //tile not 0 and in wrong spot
            if (puzzle[i][j] != 0 && (puzzle[i][j] != goal_st[i][j])){
                h++;
            }
        }
    }
    return h;
}

//Referencing: https://www.geeksforgeeks.org/sum-manhattan-distances-pairs-points/
//calculate h based on misplaced tiles and distance from goal
int manhattanDist(vector<vector<int>> puzzle){
    int h = 0;
    int row=0, col=0;
    int goalr=0, goalc=0;

    //find number of tiles away from goal state
    for (int i = 0; i <= 8; i++){
        for (int j=0; j < goal_st.size(); j++){
            for (int k=0; k < goal_st.size(); k++){
                if (puzzle[j][k] == i+1){
                    row = j;
                    col = k;
                }
                if (goal_st[j][k] == i+1){
                    goalr = j;
                    goalc = k;
                }
            }
        }
        h += abs(goalr - row) + abs(goalc - col);
    }
    return h;
}