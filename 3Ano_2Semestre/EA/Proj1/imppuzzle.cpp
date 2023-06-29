#include <iostream>
#include <string>
#include <sstream>
#include <iterator>
#include <cstring>
#include <vector>
#include <list>
#include <algorithm>
#include <chrono>
#include <unordered_set>
#include <map>

using namespace std;
using namespace std::chrono;


int columns = 0;
int lines = 0;
struct Piece *root = NULL;  

struct Piece{
    vector<int> array;
    struct Piece *right;
    struct Piece *up;
    struct Piece *down;
    struct Piece *left;
    struct Piece *ini_line;

    int xcord;
    int ycord;
    int position_linked;
    bool placed;
    bool is_redundant;
    bool is_4_equal;
};

map<int, vector<Piece*>> pair_map = {};
map<int, vector<Piece*>> two_pairs_map = {};

//Split function
template <size_t N>
void splitString(string (&arr)[N], string str)
{
    int n = 0;
    istringstream iss(str);
    for (auto it = istream_iterator<string>(iss); it != istream_iterator<string>() && n < int(N); ++it, ++n)
        arr[n] = *it;
}

vector<int> rotate_array_n(vector<int> arr_rot, int repeat){
    if(repeat < 0) repeat += 4;
    repeat = repeat % 4;
    vector<int> new_arr;


    if (repeat == 0) {
        new_arr = arr_rot;
    } else if (repeat == 1) {
        new_arr = {arr_rot[1], arr_rot[2], arr_rot[3], arr_rot[0]};
    } else if (repeat == 2) {
        new_arr = {arr_rot[2], arr_rot[3], arr_rot[0], arr_rot[1]};
    } else if (repeat == 3) {
        new_arr = {arr_rot[3], arr_rot[0], arr_rot[1], arr_rot[2]};
    }
    
    return new_arr;
}


void printPuzzle() {
    if(root == NULL) return;
    struct Piece* aux = root;
    struct Piece* line =root;
    struct Piece* collumn = root;
    string str = "";
    for (int l = 0; l < lines; l++) {
        collumn = aux;
        for (int k = 0; k < 2; k++) {
            bool first = true;
            aux = collumn;
            for (int c = 0; c < columns; c++) {

                if (k == 0) {
                    if (first) {
                        str += to_string(aux->array[0]);
                        str += ' ';
                        str += to_string(aux->array[1]);
                        first = false;
                    }
                    else {
                        str += "  ";
                        str += to_string(aux->array[0]);
                        str += ' ';
                        str += to_string(aux->array[1]);
                    }
                }
                else {
                    if (first) {
                        str += to_string(aux->array[3]);
                        str += ' ';
                        str += to_string(aux->array[2]);
                        first = false;
                    }
                    else {
                        str += "  ";
                        str += to_string(aux->array[3]);
                        str += ' ';
                        str += to_string(aux->array[2]);
                    }
                }


                if(aux->right != NULL) aux = aux->right;
                

            }
            

            str += '\n';
        }

        line = line->down;
        aux = line;

        if (l != lines - 1) {
            str += '\n';
        }
        
        
    }
    cout << str;

}


// TODO: optimize if possible
vector<Piece*> intersection_vectors(vector<Piece*> v1, vector<Piece*> v2) {
    vector<Piece*> res;

    for (Piece* p1 : v1) {
        auto it = find(v2.begin(), v2.end(), p1);
        if (it != v2.end())
            res.push_back(p1);
        
    }

    return res;
}

vector<int> intersection(vector<int> arr1, vector<int> arr2){
    vector<int> v3;


    std::set_intersection(arr1.begin(),arr1.end(),
                          arr2.begin(),arr2.end(),
                          back_inserter(v3));
    return v3;
}

// side: true -> compares p1 right side with left side of p2
//       false -> compares p1 down side with up side of p2
vector<int> find_pairs(vector<int> piece, vector<int> piece2, bool side) {

    vector<int> res;
    if (side) {
        if (piece[0] == piece2[1] && piece[3] == piece2[2]) res.push_back(0);
        if (piece[1] == piece2[1] && piece[0] == piece2[2]) res.push_back(1);
        if (piece[2] == piece2[1] && piece[1] == piece2[2]) res.push_back(2);
        if (piece[3] == piece2[1] && piece[2] == piece2[2]) res.push_back(3);
    } else {

        if (piece[0] == piece2[3] && piece[1] == piece2[2]) res.push_back(0);
        if (piece[1] == piece2[3] && piece[2] == piece2[2]) res.push_back(1);
        if (piece[2] == piece2[3] && piece[3] == piece2[2]) res.push_back(2);
        if (piece[3] == piece2[3] && piece[0] == piece2[2]) res.push_back(3);
    }
    
    
    return res;
}


// side: true -> compares p2 right side with left side of p1
//       false -> compares p2 down side with up side of p1
int find_pair(vector<int> piece, vector<int> piece2, bool side) {

    vector<int> res;
    if (side) {
        if (piece[0] == piece2[1] && piece[3] == piece2[2]) return 0;
        if (piece[1] == piece2[1] && piece[0] == piece2[2]) return 1;
        if (piece[2] == piece2[1] && piece[1] == piece2[2]) return 2;
        if (piece[3] == piece2[1] && piece[2] == piece2[2]) return 3;
    } else {
        if (piece[0] == piece2[3] && piece[1] == piece2[2]) return 0;
        if (piece[1] == piece2[3] && piece[2] == piece2[2]) return 1;
        if (piece[2] == piece2[3] && piece[3] == piece2[2]) return 2;
        if (piece[3] == piece2[3] && piece[0] == piece2[2]) return 3;
    }
    
    
    return -1;
}

bool equalNumbs(vector<int> piece){
    
    if (count(piece.begin(), piece.end(), piece[0]) == 3) return true;
    if (count(piece.begin(), piece.end(), piece[1]) == 3) return true;
    return false;
}





//Solves a puzzle. pieces is the linked list with all the remaining pieces, currX is current x coordinate of piece and currY is current y coordinate of piece
bool solveImpossiblePuzzle(struct Piece* last_piece, struct Piece* root_piece){   

    if(root_piece != NULL){
        root_piece->xcord = 0;
        root_piece->ycord = 0;
        // marks piece as placed
        root_piece->placed = true;
        root_piece->ini_line = root_piece;
        root = root_piece;
        root->position_linked = 0;

        //End condition
        
        if(root_piece->xcord == columns -1 && root_piece->ycord == lines -1) 
            return true;

        if (solveImpossiblePuzzle(root_piece, NULL)) 
            return true;
        
        // removes mark on piece
        root_piece->placed = false;
        

    }
    else{
        vector<Piece *> temp;
        //Line changes (only compare with top piece)
        if (last_piece->xcord == columns - 1) {

            int pair_top = last_piece->ini_line->array[3] << 8;
            pair_top += last_piece->ini_line->array[2];
            
            map<int, vector<Piece*>>::iterator it = pair_map.find(pair_top);

            if (it == pair_map.end()){
                // pair not found (probably never)
                return false;
            } else {
                temp = it->second;
                
            }


        } // first line (only compare with left piece)
        else if (last_piece->ycord == 0) {
            //cout << "YOO2" << endl;
            int pair_left = last_piece->array[2] << 8;
            pair_left += last_piece->array[1];

            map<int, vector<Piece*>>::iterator it = pair_map.find(pair_left);

            if (it == pair_map.end()){
                // pair not found (probably never)
                return false;
            } else {
                temp = it->second;
            }

        } // compare with both top and left pieces 
        else {
            int pair_left = last_piece->up->right->array[3] << 8;
            pair_left += last_piece->up->right->array[2];

            int pair_top = last_piece->array[2] << 8;
            pair_top += last_piece->array[1];

            int pair_comb = (pair_left << 16) + pair_top;
            auto it = two_pairs_map.find(pair_comb);

            if(it != two_pairs_map.end()) {
                temp = it->second;
            } else {

                map<int, vector<Piece*>>::iterator it_left = pair_map.find(pair_left);
                map<int, vector<Piece*>>::iterator it_top = pair_map.find(pair_top);

                if (it_left == pair_map.end() || it_top == pair_map.end()){
                    // pair not found (probably never)
                    return false;
                } else {
                    
                    vector<int> res;
                    vector<int> res2;
                    
                    temp = intersection_vectors(it_left->second, it_top->second);

                    for(int i = temp.size(); i--; ){
                        if(last_piece->up->right->array[3] == last_piece->up->right->array[2] && last_piece->up->right->array[2] == last_piece->array[2] && last_piece->up->right->array[2] == last_piece->array[1] && (!temp[i]->is_redundant && !temp[i]->is_4_equal)){
                            temp.erase(temp.begin()+i);
                            continue;
                        }

                        if(temp[i]->is_redundant) {
                            res = find_pairs(temp[i]->array,  last_piece->array, true);
                            res2 = find_pairs(temp[i]->array, last_piece->up->right->array, false);

                            if(res.empty() || res2.empty()){
                                temp.erase(temp.begin()+i);
                            }
                            else{
                                res = intersection(res2, res);

                                if(res.empty()){
                                    temp.erase(temp.begin()+i);
                                }
                                
                            }
                        } else {
                            int rot = find_pair(temp[i]->array,  last_piece->array, true);
                            int rot2 = find_pair(temp[i]->array, last_piece->up->right->array, false);

                            if(rot != rot2){
                                temp.erase(temp.begin()+i);
                            }   
                        }
                    }
                    two_pairs_map.insert({ pair_comb, temp});   
                }
            }            
        }

        int pos = 0;

        for (Piece *fit : temp) {
            // check if piece is already in place
            if (fit->placed)
                continue;

            
            //Line changes
            if(last_piece->xcord == columns-1){
                //second line
                if(last_piece->ycord == 0){
                    pos = 2;

                    root->down = fit;
                    if (!fit->is_redundant) fit->array = rotate_array_n(fit->array, find_pair(fit->array, last_piece->ini_line->array, false));
                    fit->up = root;
                    fit->xcord = 0;
                    fit->ycord = 1;
                }
                //Not second line
                else{
                    pos = 2;

                    if (!fit->is_redundant) fit->array = rotate_array_n(fit->array, find_pair(fit->array, last_piece->ini_line->array, false));
                    fit->up = last_piece->ini_line;
                    last_piece->ini_line->down = fit;
                    fit->xcord = 0;
                    fit->ycord = last_piece->ycord +1;
                }
                fit->ini_line = fit;
            }
            //No line change
            else{
                if (!fit->is_redundant) fit->array = rotate_array_n(fit->array, find_pair(fit->array, last_piece->array, true));
                fit->xcord = last_piece->xcord +1;
                fit->ycord = last_piece->ycord;
                
                if(last_piece->ycord != 0){
  
                    pos = 0;
                    last_piece->up->right->down = fit;
                    fit->up = last_piece->up->right;
                }
                else pos = 1;
                last_piece->right = fit;
                fit->left = last_piece;
                fit->ini_line = last_piece->ini_line;
            }
            

            // marks piece as placed
            fit->placed = true;

            if (fit->is_redundant) {
                vector<int> res;
                vector<int> res2;
                
                switch(pos){
                    case 0:
                        res = find_pairs(fit->array,  last_piece->array, true);
                        res2 = find_pairs(fit->array, last_piece->up->right->array, false);
                        
                        if(!res.empty() && !res2.empty()){
                            res = intersection(res2, res);
                        }
                        else{
                            res = {};
                        }
                        break;
                    
                    case 1:
                        res = find_pairs(fit->array, last_piece->array, true);
                        break;
                    
                    case 2:
                        res = find_pairs(fit->array, last_piece->ini_line->array, false);
                        break;
                    
                }       
                for (int i = 0; i < (int) res.size(); i++) {
                    fit->array = rotate_array_n(fit->array, res[i]);

                    //End condition
                    if(fit->xcord == columns -1 && fit->ycord == lines -1) 
                        return true;
                    
                    if (solveImpossiblePuzzle(fit, NULL)) 
                        return true;
                    
                    fit->array = rotate_array_n(fit->array, -res[i]);
                }


            } else {
                //End condition
                if(fit->xcord == columns -1 && fit->ycord == lines -1) 
                    return true;

                if (solveImpossiblePuzzle(fit, NULL)) 
                    return true;
            }
            
            // removes mark on piece
            fit->placed = false;

        }
    }

    return false;
}




// pairs are made for comparasion with top, if you want to compare with left you need to invert the pair
vector<int> dividePairs(struct Piece* p) {
    vector<int> pairs;
    int pair1 = (p->array[0] << 8) + p->array[1];
    int pair2 = (p->array[1] << 8) + p->array[2];
    int pair3 = (p->array[2] << 8) + p->array[3];
    int pair4 = (p->array[3] << 8) + p->array[0];
    pairs.push_back(pair1);
    pairs.push_back(pair2);
    pairs.push_back(pair3);
    pairs.push_back(pair4);

    return pairs;
}


void calculatePairs(vector<struct Piece *> pieces) {
    map<string, vector<string>> temp = {};

    for(Piece *p : pieces) {
        for(int pair_c: dividePairs(p)) {
            map<int, vector<Piece*>>::iterator it = pair_map.find(pair_c); 
            if (it == pair_map.end()) {
                vector<Piece*> vec_pieces = {p};
                pair_map.insert( pair<int, vector<Piece*>>(pair_c, vec_pieces) );
            } else {
                if(find(it->second.begin(), it->second.end(), p) == it->second.end())
                    it->second.push_back(p);
            }
        }
    }

}

bool verifyIfPossible(vector<struct Piece *> pieces) {
    map<int, int> counter_map = {};
    for (Piece *p : pieces) {
        for (int elem: p->array) {
            map<int, int>::iterator it = counter_map.find(elem);
            if(it == counter_map.end()) {
                counter_map.insert( pair<int, int> (elem, 1));
            } else {
                it->second++;
            }
        }
    }

    map<int, int>::iterator it_loop;
    int counter = 0;

    for (it_loop = counter_map.begin(); it_loop != counter_map.end(); it_loop++) {
        if (it_loop->second % 2 == 1) {
            counter++;
            if (counter > 4) 
                return false;
        }
    }
    return true;
}


void printMap() {
    for (auto x : pair_map) {
        cout << x.first << " : ";
        vector<Piece*> v = x.second;
        for (int i = 0; i < (int) v.size(); i++) {
            cout << v[i]->array[0] << " " << v[i]->array[1] << " " << v[i]->array[2] << " " << v[i]->array[3] << "; ";
        }
        cout << endl;
    }
    for (auto x : two_pairs_map) {
        cout << x.first << " : ";
        vector<Piece*> v = x.second;
        for (int i = 0; i < (int) v.size(); i++) {
            cout << v[i]->array[0] << " " << v[i]->array[1] << " " << v[i]->array[2] << " " << v[i]->array[3] << "; ";
        }
        cout << endl;
    }
}


int main() {
    // We probably do not need this but it is faster
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    string number_tests;
    getline(cin, number_tests);

    
    
    for (int i = 0; i < stoi(number_tests); i++){

        vector<struct Piece *> pieces;
        

        string format;
        getline(cin, format);

        string arr[3];
        stringstream ssin(format);

        splitString(arr, format);
        lines = atoi(arr[1].c_str());
        columns = atoi(arr[2].c_str());

        for(int j = 0; j < atoi(arr[0].c_str()); j++){

            string piece_str;
            getline(cin, piece_str);

            string elems[4];
            stringstream ssin(piece_str);

            splitString(elems, piece_str);
            
            struct Piece* piece = new Piece;
            piece->array = {stoi(elems[0]), stoi(elems[1]), stoi(elems[2]), stoi(elems[3])};
            
            piece->xcord = 0;
            piece->ycord = 0;
            piece->placed = false;

            if(equalNumbs(piece->array)) piece->is_redundant = true;
            else piece->is_redundant = false;

            if(count(piece->array.begin(), piece->array.end(), piece->array[0]) == 4) piece->is_4_equal = true;
            else piece->is_4_equal = false;

            pieces.push_back(piece);


        }
        calculatePairs(pieces);
        //printMap();
        
        if(verifyIfPossible(pieces) && solveImpossiblePuzzle(NULL, pieces[0])){
            printPuzzle();
        }
        else{
            cout << "impossible puzzle!" << endl;
        }

        pair_map.clear();
        two_pairs_map.clear();
    }

    return 0;
}