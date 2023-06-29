#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <chrono>

using namespace std;

map<int, vector<int>> graph;
map<int, vector<int>> nodes;
map<int, int> cost;
map<int, vector<int>> best_cost;




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


void addNode(int key, int value, int key2, int value2) {
    map<int, vector<int>>::iterator it = nodes.find(key);
    map<int, vector<int>>::iterator it2 = best_cost.find(key);

    if (it == nodes.end()) {
        vector<int> vec;
        vec.push_back(value);
        nodes.insert(make_pair(key, vec));

    } else {
        it->second.push_back(value);
    }


    if (it2 == best_cost.end()) {
        vector<int> vec;
        vec.push_back(value2);
        best_cost.insert(make_pair(key2, vec));

    } else {
        best_cost[key2].push_back(value2);
    }
}




void addConection(int key, int value) {
    map<int, vector<int>>::iterator it = graph.find(key);

    if (it == graph.end()) {
        vector<int> vec;
        vec.push_back(value);
        graph.insert(make_pair(key, vec));

    } else {
        it->second.push_back(value);
    }
}


void minCalc(int src, int par) {


    for (int x : graph[src]) {
        if (x != par) {
            minCalc(x, src);
        }
    }

    for (int x : graph[src]) {
        if (x != par) {

            nodes[src][0] += nodes[x][1];
            best_cost[src][0] += best_cost[x][1];

            if (nodes[x][1] < nodes[x][0]) {
                best_cost[src][1] += best_cost[x][1];
                nodes[src][1] += nodes[x][1];
                

            } else if (nodes[x][1] > nodes[x][0]) {
                best_cost[src][1] += best_cost[x][0];
                nodes[src][1] += nodes[x][0];

            } else {

                if (best_cost[x][0] > best_cost[x][1])
                    best_cost[src][1] += best_cost[x][0];

                else
                    best_cost[src][1] += best_cost[x][1];
                
                nodes[src][1] += nodes[x][0];
            }

        }
    }

}


void findMin() {
    
    if(graph.empty()){

        cout << "0 0" << endl;
        
        return;
    }
    if(graph.find(0) == graph.end()){
        cout << "0 0" << endl;
        return;
    }

    for (map<int, vector<int>>::iterator it = graph.begin(); it != graph.end(); ++it) {
        addNode(it->first, 0, it->first, 0);
        addNode(it->first, 1, it->first, cost[it->first]);
        

    }

    minCalc(0,-1);


    if (nodes[0][0] < nodes[0][1]) {
        outln(to_string(nodes[0][0]) + " " + to_string(best_cost[0][0]));

    } else if (nodes[0][0] > nodes[0][1]) {
        outln(to_string(nodes[0][1]) + " " + to_string(best_cost[0][1]));

    } else {
        outln(to_string(nodes[0][0]) + " " + to_string(max(best_cost[0][0], best_cost[0][1])));
    }

}


int main() {

    int number_lines = 0;
    int number_conections = 0;


    vector<int> line = readln();

    while(line.size() > 0) {

        if(line[0] == -1) {

            findMin();
            graph.clear();
            nodes.clear();
            cost.clear();
            best_cost.clear();
            number_lines = 0;
            number_conections = 0;
        
        } else {
            number_lines += 1;

            for (int i = 0; i < (int) line.size(); i++) {
                if (i == 0)
                    continue;

                else if (i == (int) line.size() - 1)
                    cost.insert(make_pair(line[0], line[i]));

                else {
                    number_conections += 1;
                    addConection(line[0], line[i]);
                    addConection(line[i], line[0]);
                }
            }


        }

        line = readln();
    }


    return 0;
}
