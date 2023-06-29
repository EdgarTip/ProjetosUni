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

using namespace std;
using namespace std::chrono;

int columns = 0;
int lines = 0;
struct Piece *root = NULL;

struct Piece{
    std::vector<std::string> array;
    struct Piece *right;
    struct Piece *up;
    struct Piece *down;
    struct Piece *left;
    struct Piece *ini_line;

    int xcord;
    int ycord;
    int position_linked;
};

//Split function
template <size_t N>
void splitString(string (&arr)[N], string str)
{
    int n = 0;
    istringstream iss(str);
    for (auto it = istream_iterator<string>(iss); it != istream_iterator<string>() && n < int(N); ++it, ++n)
        arr[n] = *it;
}

struct Piece* rotate_array(vector<string> arr_rot){
    struct Piece *rot_piece = new Piece;

    rot_piece->array.push_back(arr_rot[1]);
    rot_piece->array.push_back(arr_rot[2]);
    rot_piece->array.push_back(arr_rot[3]);
    rot_piece->array.push_back(arr_rot[0]);
    
    return rot_piece;
}

struct Piece* rotate_array_n(vector<string> arr_rot, int repeat){
    struct Piece *rot_piece = new Piece;
    vector<string> res = arr_rot;
    vector<string> temp;
    for (int i = repeat; i--; ) {
        temp.push_back(res[1]);
        temp.push_back(res[2]);
        temp.push_back(res[3]);
        temp.push_back(res[0]);

        res = temp;
        if (i) temp.clear();
    
    }
    rot_piece->array = res;
    
    return rot_piece;
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
                        str += aux->array[0];
                        str += ' ';
                        str += aux->array[1];
                        first = false;
                    }
                    else {
                        str += "  ";
                        str += aux->array[0];
                        str += ' ';
                        str += aux->array[1];
                    }
                }
                else {
                    if (first) {
                        str += aux->array[3];
                        str += ' ';
                        str += aux->array[2];
                        first = false;
                    }
                    else {
                        str += "  ";
                        str += aux->array[3];
                        str += ' ';
                        str += aux->array[2];
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


int intersection_str(vector<string> arr1, vector<string> arr2){
    vector<string> v3;


    std::set_intersection(arr1.begin(),arr1.end(),
                          arr2.begin(),arr2.end(),
                          back_inserter(v3));
    return v3.size();
}


vector<int> intersection(vector<int> arr1, vector<int> arr2){
    vector<int> v3;


    std::set_intersection(arr1.begin(),arr1.end(),
                          arr2.begin(),arr2.end(),
                          back_inserter(v3));
    return v3;
}



// side: true -> right side of the piece
//       false -> down side of the piece
vector<int> find_pairs(vector<string> piece, vector<string> piece2, bool side) {

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

// side: true -> right side of the piece
//       false -> down side of the piece
int find_pair(vector<string> piece, vector<string> piece2, bool side) {

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


//Finds possible pieces to fit in the puzzle. 
vector<struct Piece *> findPossiblePieces(vector<struct Piece *> v, struct Piece* last_piece){

    vector<struct Piece*> pieces_fit = {};
    
    if(last_piece == NULL){
        
        struct Piece *front_piece = v.front();

 
        struct Piece *new_piece;
        new_piece = front_piece;

        pieces_fit.push_back(new_piece);

        /* for(int i = 0; i < 3; i++){
            new_piece = rotate_array(new_piece->array);
            new_piece->position_linked = 0;
            pieces_fit.push_back(new_piece);
        } */


        return pieces_fit;

    }
    
    //Piece *p: v
    int count = 0;
    for(Piece *p: v){

        if(last_piece->xcord < columns -1  && (find(p->array.begin(), p->array.end(), last_piece->array[1]) == p->array.end() || find(p->array.begin(), p->array.end(), last_piece->array[2]) == p->array.end())){
            count++;
            continue;
        }

        //Condicoes depois de ver que corre meter
        struct Piece *new_piece = p;
        vector<string> origin_piece = new_piece->array;
        
        // good
        if (last_piece->xcord < columns - 1 && last_piece->ycord > 0) {
            vector<int> res1 = find_pairs(new_piece->array, last_piece->array, true);
            vector<int> res2 = find_pairs(new_piece->array, last_piece->up->right->array, false);
            
            if (!res1.empty() && !res2.empty()) {
                vector<int> res3 = intersection(res1, res2);
                bool is_first = true;
                for(int id : res3) {
                    new_piece = rotate_array_n(origin_piece, id);
                    if (is_first) new_piece->position_linked = count;
                    else new_piece->position_linked = 0;
                    pieces_fit.push_back(new_piece); 
                }

                count++; 
                continue;           
            }
        }
            
        if (last_piece->xcord < columns - 1 && last_piece->ycord == 0) {

            vector<int> res1 = find_pairs(new_piece->array, last_piece->array, true);
            if (!res1.empty()) {
                bool is_first = true;
                for(int id : res1) {
                    new_piece = rotate_array_n(origin_piece, id);
                    if (is_first) new_piece->position_linked = count;
                    else new_piece->position_linked = 0;
                    pieces_fit.push_back(new_piece); 
                }

                count++; 
                continue; 
                
            }

            
            
        }
        if (last_piece->xcord == columns - 1) {
            vector<int> res1 = find_pairs(new_piece->array, last_piece->ini_line->array, false);
            if (!res1.empty()) {
                bool is_first = true;
                for(int id : res1) {
                    new_piece = rotate_array_n(origin_piece, id);
                    if (is_first) new_piece->position_linked = count;
                    else new_piece->position_linked = 0;
                    pieces_fit.push_back(new_piece); 
                }

                count++; 
                continue; 
                
            }
            
            
            
        }

        count++;
    }

    return pieces_fit;
}



//Solves a puzzle. pieces is the linked list with all the remaining pieces, currX is current x coordinate of piece and currY is current y coordinate of piece
bool solveImpossiblePuzzle(vector<struct Piece *> pieces, struct Piece* last_piece, bool is_root){   
    
    vector<struct Piece *> temp =  findPossiblePieces(pieces, last_piece);

    if(!temp.empty()){

        for (Piece* fit: temp){

            if(is_root){
                fit->xcord = 0;
                fit->ycord = 0;
                fit->ini_line = fit;
                root = fit;
                root->position_linked = 0;
                

            }
            else{
                //Line changes
                if(last_piece->xcord == columns-1){
                    //second line
                    if(last_piece->ycord == 0){
                        root->down = fit;
                        fit->up = root;
                        fit->xcord = 0;
                        fit->ycord = 1;
                    }
                    //Not second line
                    else{
                        fit->up = last_piece->ini_line;
                        last_piece->ini_line->down = fit;
                        fit->xcord = 0;
                        fit->ycord = last_piece->ycord +1;
                    }
                    fit->ini_line = fit;
                }
                //No line change
                else{

                    fit->xcord = last_piece->xcord +1;
                    fit->ycord = last_piece->ycord;

                    if(last_piece->ycord != 0){
                        last_piece->up->right->down = fit;
                        fit->up = last_piece->up->right;
                    }
                    last_piece->right = fit;
                    fit->left = last_piece;
                    fit->ini_line = last_piece->ini_line;
                }
                
            }

            //Take the element out of the array 
            pieces.erase(pieces.begin() + fit->position_linked);
        
            //End condition
            
            if(fit->xcord == columns -1 && fit->ycord == lines -1) 
                return true;

            if (solveImpossiblePuzzle(pieces, fit, false)) 
                return true;


            //Go back in the recursion
            
            
            struct Piece* not_fit = fit;
            
            pieces.insert(pieces.begin() + fit->position_linked, not_fit);
            //not_fit->position_linked = tem;       //pieces.size() - 1
            //pieces.push_back(not_fit);
            //cout << "FIT count: " + to_string(not_fit->position_linked) << endl;
            fit = NULL;

            //cout << "NOT FIT" + not_fit->array[0] << endl;
            
            

            
        }
    }

    //cout << "back" << endl;
    return false;
}


int main() {
    // We probably do not need this but it is faster
    //ios_base::sync_with_stdio(0);
    //cin.tie(0);

    //auto init_time = high_resolution_clock::now();

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
            piece->array = {elems[0], elems[1], elems[2], elems[3]};
            
            piece->xcord = 0;
            piece->ycord = 0;

            pieces.push_back(piece);


        }
        
        if(solveImpossiblePuzzle(pieces, NULL, true)){
            printPuzzle();
        }
        else{
            cout << "impossible puzzle!" << endl;
        }

    }
    

    //auto end_time = high_resolution_clock::now();

    //auto duration = end_time - init_time;
    //cout << "Time solution ms: " << duration / milliseconds(1) << endl;


    return 0;
}
