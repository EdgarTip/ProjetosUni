#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;


struct Operation {
    int id;
    int cost;
    vector<int> dependenciesIds;
    vector<struct Operation*> childrens;
    vector<struct Operation*> parents;
};

vector<struct Operation*> operations;

int idInitOp;

vector<bool> nodesVisited;
vector<bool> nodesCalculated;
vector<bool> nodesChecked;

vector<int> dp;

int totalTime;
vector<int> orderExecution;



void outln(string str) {
    cout << str << endl;
}


vector<int> readln() {
    string str = "";
    getline(std::cin, str, '\n');

	vector<int> result;
	istringstream iss(str);
	for (string s; iss >> s; )
		result.push_back(stoi(s));
	return result;
}


bool checkForSingleRoot() {
    int starts = 0;
    for (Operation* op : operations) {
        if(op->parents.empty()) {
            starts++;
            idInitOp = op->id;
            if (starts > 1)
                return false;
        }
    }
    return true;
}


void dfsForward(int x) {
    nodesVisited[x] = true;
 
    for (auto i : operations[x]->childrens)
        if (!nodesVisited[i->id])
            dfsForward(i->id);
}


void dfsBack(int x) {
    nodesVisited[x] = true;
 
    for (auto i : operations[x]->parents)
        if (!nodesVisited[i->id])
            dfsBack(i->id);
}


bool checkGraphConnections(int idOp) {
    nodesVisited = vector<bool>(operations.size(), false);
    dfsForward(idOp);

    if (find(nodesVisited.begin(), nodesVisited.end(), false) == nodesVisited.end()) 
        return true;

    return false;
}


bool checkForBottleneck(int idOp) {
    nodesVisited = vector<bool>(operations.size(), false);
    dfsForward(idOp);
    dfsBack(idOp);

    if (find(nodesVisited.begin(), nodesVisited.end(), false) == nodesVisited.end()) 
        return true;

    return false;
}


vector<int> findBottlenecks(int id, vector<int> res) {
    nodesChecked[id] = true;

    if (id == idInitOp || operations[id]->childrens.size() == 0) {
        res.push_back(id + 1);
    

    } else if(checkForBottleneck(id)) {
        res.push_back(id + 1);
    }
    

    for (Operation* op : operations[id]->childrens) {
        if (nodesChecked[op->id] == false)
            res = findBottlenecks(op->id, res);
    }
    return res;
}


bool checkEndPoints() {
    int endPoints = 0;

    for (int i = 0; i < (int) operations.size(); i++)
        if (operations[i]->childrens.size() == 0) 
            endPoints += 1;
        
    if (endPoints == 1)
        return true;

    return false;
}


bool isCyclic(int currentID) {
    nodesVisited[currentID] = true;

    for (auto i : operations[currentID]->childrens) {
        if (nodesVisited[i->id] == true)
            return true;
        else
            if (nodesCalculated[i->id] == false && isCyclic(i->id) == true)
                return true;
    }
    nodesVisited[currentID] = false;
    nodesCalculated[currentID] = true;
    return false;
}


bool checkPipeline() {

    // check if there are multiple starting points
    if (!checkForSingleRoot())
        return false;

    // check if all nodes are connected
    if(!checkGraphConnections(idInitOp))
        return false;

    // check if there are multiple ending points
    if(!checkEndPoints())
        return false;

    // check if there are no cicles in the graph
    nodesVisited = vector<bool>(operations.size(), false);
    nodesCalculated = vector<bool>(operations.size(), false);
    if (isCyclic(idInitOp))
        return false;    

    return true;
}


void addConnection(int first_operation, int operation_dependent) {
    operations[first_operation]->childrens.push_back(operations[operation_dependent]);
    operations[operation_dependent]->parents.push_back(operations[first_operation]);
}


bool checkDependencies(Operation* operation) {
    for (Operation* op : operation->parents) {
        if (nodesVisited[op->id] == false)
            return false;
    }
    return true;
}


bool compareCostOperation(Operation* op1, Operation* op2) {
    if (op1->id > op2->id)
        return false;
    else
        return true;
}


void runPipelineSingleThreaded() {
    totalTime = 0;
    orderExecution = vector<int>();
    nodesVisited = vector<bool>(operations.size(), false);

    vector<Operation*> queue = vector<Operation*>();
    queue.push_back(operations[idInitOp]);

    while (!queue.empty()) {
        if (nodesVisited[queue[0]->id] == true) {
            queue.erase(queue.begin());
            continue;
        }
        sort(queue.begin(), queue.end(), &compareCostOperation);
        
        nodesVisited[queue[0]->id] = true;
        totalTime += queue[0]->cost;
        orderExecution.push_back(queue[0]->id + 1);

        vector<Operation*> childOps = vector<Operation*>();

        for (Operation* op : queue[0]->childrens) {
            if (nodesVisited[op->id] == false && checkDependencies(op))
                queue.push_back(op);
        }

        // remove elem
        queue.erase(queue.begin());
    }

}

void dfs(int id) {
    nodesVisited[id] = true;

    for (Operation* op: operations[id]->childrens) {
        if (nodesVisited[op->id] == false)
            dfs(op->id);

        dp[id] = max(dp[id], op->cost + dp[op->id]);
    }
}


void runPipelineMultiThreaded() {
    totalTime = 0;
    nodesVisited = vector<bool>(operations.size(), false);
    dp = vector<int>(operations.size(), 0);

    for (int i = 0; i < (int) nodesVisited.size(); i++) {
        if (nodesVisited[i] == false) {
            dfs(i);
        }
    }

    totalTime = *max_element(dp.begin(), dp.end());

    //add initial cost
    totalTime += operations[idInitOp]->cost;

}


int main() {

    int elems = readln()[0];

    for (int i = 0; i < elems; i++) {
        vector<int> line = readln();

        struct Operation* op = new Operation;
        op->id = i;
        op->cost = line[0];
        operations.push_back(op);
        vector<int> dep;

        if (line[1] == 0) {
        } else {
            for (int j = 2; j < line[1] + 2; j++) {
                dep.push_back(line[j]);
            }
        }
        op->dependenciesIds = dep;
        
    }

    // create connections
    for (auto elem : operations) {
        for (int id : elem->dependenciesIds) {
            addConnection(id - 1, elem->id);
        }
    }

    int option = readln()[0];

    bool isValid = checkPipeline();
  
    if(!isValid) {
        cout << "INVALID" << endl;
        return 0;
    }

    vector<int> res;
    string output = "";

    switch (option) {
        case 0:
            if(isValid)
                cout << "VALID" << endl;
            else
                cout << "INVALID" << endl;
            break;

        case 1:
            runPipelineSingleThreaded();
            cout << totalTime << endl;
            for (int elem : orderExecution)
                output += to_string(elem) + "\n";

            cout << output << endl;
            
            break;
        /*
        case 2:
            runPipelineMultiThreaded();
            cout << totalTime << endl;
            break;

        case 3:
            nodesChecked = vector<bool>(operations.size(), false);
            res = findBottlenecks(idInitOp, res);
            
            for (int elem : res)
                output += to_string(elem) + "\n";

            cout << output << endl;
            
            break;

        default:
            if(isValid)
                cout << "VALID" << endl;
            else
                cout << "INVALID" << endl;
            break;*/
    }

    
    return 0;
}