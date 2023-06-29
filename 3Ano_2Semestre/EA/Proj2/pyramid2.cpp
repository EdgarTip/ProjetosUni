
// C++ implementation for the above approach
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <chrono>
using namespace std;
 
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


class Node{
    

    public:
        vector<int> nodes;
        int cost = -1;
        int num = -1;
        int inc = 1;
        int exc = 0;
        int cost_inc = 0;
        int cost_exc = 0;

        Node(){}
        Node(int x, int y){
            num = x;
            cost = y;
        }

        void addNode(int a){
            nodes.push_back(a);
        }
        
};

map<int, Node> graph;

void printMap(){
    for (auto it = graph.cbegin(); it != graph.cend(); ++it) {
        std::cout << "{" << (*it).first << ": " << (*it).second.cost_exc << "}\n";
    }
}

void calc(Node &src, int par){
    
    for( int x : src.nodes){
        if( x != par ){
            calc(graph[x], src.num);
        }
    }

    for( int x : src.nodes){

        if (x != par){
            src.exc += graph[x].inc;
            src.cost_exc += graph[x].cost_inc;

            if(graph[x].inc > graph[x].exc){
                src.cost_inc += graph[x].cost_exc;
                src.inc += graph[x].exc;
            }
            else if(graph[x].inc < graph[x].exc){
                src.cost_inc += graph[x].cost_inc;
                src.inc += graph[x].inc;
            }
            else{
                if (graph[x].cost_inc > graph[x].cost_exc){
                    src.cost_inc += graph[x].cost_inc;
                }
                else{
                    src.cost_inc += graph[x].cost_exc;
                }
                src.inc += graph[x].exc;
            }
            
        }
    }



}

int main()
{  
    bool root_chosen = false;

    Node root = Node();
    Node new_node = Node();

    

    while(true) {
        vector<int> line = readln();

        if(line.size() < 1){
            return 0;
        }

        if(line[0] == -1) {
            
            if(!root_chosen) {
                cout << "0 0" << endl;
                continue;
            }

            calc(root, -1);

            if(root.inc < root.exc){
                cout << root.inc << " " << root.cost_inc << endl;
            }
            else if(root.inc > root.exc){
                cout << root.exc << " " << root.cost_exc << endl;
            }
            else{
                if(root.cost_inc > root.cost_exc){
                    cout << root.inc << " " << root.cost_inc << endl;
                }
                else{
                    cout << root.exc << " " << root.cost_exc << endl;
                }
            }
            graph.clear();
            Node root = Node();
            root_chosen = false;
        }
        else{
            Node new_node = Node();
            for (int i = 0; i < (int) line.size(); i++) {
                if (i == 0){
                    new_node.num = line[0];
                }

                else if (i == (int) line.size() - 1)
                    new_node.cost_inc = line[i];

                else {
                    new_node.addNode(line[i]);
                }
            }

            graph.insert(make_pair(line[0], new_node));
            if(line[0] == 0){
                root = new_node;
                root_chosen = true;
            }

        }
    }
 
    return 0;
}