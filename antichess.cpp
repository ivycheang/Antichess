/*
This was done for Game Theory class as a final coding project.
The coding is messy because I had 3 days to do a group project on my own in
C++, which I never touched before.
Hopefully I'll get to clean it up a bit sometime.

This AI plays a modified version of chess. Main idea is that if you can eat
a piece, then you'd have to. When I clean the code I'll dig up the actual
rules and write them down.
*/

#include <iostream>
#include <cstring>
#include <sstream>

using namespace std;

//nodes of game tree
struct node {
    char turn;
    string board [64];
    int next;
    string moves [220];
    node *next_list [220];
};

//white or black
string side;

//input
string input = "";

//strategy
string best_move;

//round counter
int round = 0;

//50-move rule
int fifty = 0;

//max_depth
int bravery = 6;

//values of pieces
int p_val = 1;
int r_val = 5;
int n_val = 3;
int b_val = 3;
int q_val = 10;
int k_val = 1000;

//variables for side
int one;
int two;
int three;
int four;
int five;
int six;
int seven;
int eight;
int A;
int B;
int C;
int D;
int E;
int F;
int G;
int H;

//blank box
string blank = "///";

//function to copy a board from one to two
void copy_board(string *one, string *two) {
    for (int j=0; j<64; j++) {
            two[j] = one[j];
    }
}

//converting number index for board to string position
string ind_to_pos (int ind) {
    int col = ind%8+1;
    int row = (ind-(col-1))/8+1;
    char letter;
    char num;

    if (col == 1) {letter=A;}
    else if (col == 2) {letter=B;}
    else if (col == 3) {letter=C;}
    else if (col == 4) {letter=D;}
    else if (col == 5) {letter=E;}
    else if (col == 6) {letter=F;}
    else if (col == 7) {letter=G;}
    else if (col == 8) {letter=H;}
    if (row == 1) {num = one;}
    else if (row == 2) {num = two;}
    else if (row == 3) {num = three;}
    else if (row == 4) {num = four;}
    else if (row == 5) {num = five;}
    else if (row == 6) {num = six;}
    else if (row == 7) {num = seven;}
    else if (row == 8) {num = eight;}

    stringstream ss;
    ss << letter << num;
    return ss.str();
}


//check if move is a capture move or not
int capture(string step, string *board) {
    int ind = 0;
    if (step[2]==A) {}
    else if (step[2]==B) {ind+=1;}
    else if (step[2]==C) {ind+=2;}
    else if (step[2]==D) {ind+=3;}
    else if (step[2]==E) {ind+=4;}
    else if (step[2]==F) {ind+=5;}
    else if (step[2]==G) {ind+=6;}
    else if (step[2]==H) {ind+=7;}
    if (step[2]==one) {}
    else if (step[2]==two) {ind+=1*8;}
    else if (step[2]==three) {ind+=2*8;}
    else if (step[2]==four) {ind+=3*8;}
    else if (step[2]==five) {ind+=4*8;}
    else if (step[2]==six) {ind+=5*8;}
    else if (step[2]==seven) {ind+=6*8;}
    else if (step[2]==eight) {ind+=7*8;}

    if (board[ind]==blank) {return 0;}
    else {return 1;}
}

//create new board by stepping on existing board
void step_board(string step, string *cur, string *con) {
    //find start index
    int beg = 0;
    if (step[0] == A) {}
    else if (step[0] == B) {beg += 1;}
    else if (step[0] == C) {beg += 2;}
    else if (step[0] == D) {beg += 3;}
    else if (step[0] == E) {beg += 4;}
    else if (step[0] == F) {beg += 5;}
    else if (step[0] == G) {beg += 6;}
    else if (step[0] == H) {beg += 7;}
    if (step[1] == one) {}
    else if (step[1] == two) {beg += 8;}
    else if (step[1] == three) {beg += 16;}
    else if (step[1] == four) {beg += 24;}
    else if (step[1] == five) {beg += 32;}
    else if (step[1] == six) {beg += 40;}
    else if (step[1] == seven) {beg += 48;}
    else if (step[1] == eight) {beg += 56;}
    //find end index
    int stop = 0;
    if (step[2] == A) {}
    else if (step[2] == B) {stop += 1;}
    else if (step[2] == C) {stop += 2;}
    else if (step[2] == D) {stop += 3;}
    else if (step[2] == E) {stop += 4;}
    else if (step[2] == F) {stop += 5;}
    else if (step[2] == G) {stop += 6;}
    else if (step[2] == H) {stop += 7;}
    if (step[3] == one) {}
    else if (step[3] == two) {stop += 8;}
    else if (step[3] == three) {stop += 16;}
    else if (step[3] == four) {stop += 24;}
    else if (step[3] == five) {stop += 32;}
    else if (step[3] == six) {stop += 40;}
    else if (step[3] == seven) {stop += 48;}
    else if (step[3] == eight) {stop += 56;}

    copy_board(cur, con);

    if (step=="") {return;}

    con[beg] = blank;
    con[stop] = cur[beg];

    for(int i=0; i<63;i++){
        if (con[i][1]=='P' && i!=stop) {
            if (con[i][2]=='1') {con[i][2]='2';}
        }
    }

    //SPECIAL CASES
    //counters
    if (cur[beg][1] == 'R' && cur[beg][2] == '0') {
        con[stop][2] = '1';
    }
    if (cur[beg][1] == 'K' && cur[beg][2] == '0') {
        con[stop][2] = '1';
    }
    if (cur[beg][1] == 'P' && cur[beg][2] == '0') {
        con[stop][2] = '1';
    }
    if (cur[beg][1] == 'P' && cur[beg][2] == '1') {
        con[stop][2] = '2';
    }

    //en passant
    if (cur[beg][1]=='P' && step[0]!=step[2] && step[1]!=step[3] && cur[stop]==blank) {
        if (cur[beg-1] == "1P1") {
            con[beg-1] = blank;
        }
        if (cur[beg-1] == "2P1") {
            con[beg-1] = blank;
        }
        if (cur[beg+1] == "1P1") {
            con[beg+1] = blank;
        }
        if (cur[beg+1] == "2P1") {
            con[beg+1] = blank;
        }
    }

    //castling
    if (cur[beg][1] == 'K' && cur[beg][2] == '0') {
        if (step == "e1c1") {
            int beg1 = 56;
            int stop1 = 59;
            if (side == "white" && cur[beg1] == "1R0") {
                con[stop][2] = '1';
                con[beg1] = blank;
                con[stop1] = "1R1";
            }
            if (side == "black" && cur[63-beg1] == "2R0") {
                con[stop][2] = '1';
                con[63-beg1] = blank;
                con[63-stop1] = "1R1";
            }
        }
        else if (step == "e1g1") {
            int beg1 = 63;
            int stop1 = 61;
            if (side == "white" && cur[beg1] == "1R0") {
                con[stop][2] = '1';
                con[beg1] = blank;
                con[stop1] = "1R1";
            }
            if (side == "black" && cur[63-beg1] == "2R0") {
                con[stop][2] = '1';
                con[63-beg1] = blank;
                con[63-stop1] = "1R1";
            }
        }
        else if (step == "e8c8") {
            int beg1 = 0;
            int stop1 = 3;
            if (side == "white" && cur[beg1] == "1R0") {
                con[stop][2] = '1';
                con[beg1] = blank;
                con[stop1] = "1R1";
            }
            if (side == "black" && cur[63-beg1] == "2R0") {
                con[stop][2] = '1';
                con[63-beg1] = blank;
                con[63-stop1] = "1R1";
            }
        }
        else if (step == "e8g8") {
            int beg1 = 7;
            int stop1 = 5;
            if (side == "white" && cur[beg1] == "1R0") {
                con[stop][2] = '1';
                con[beg1] = blank;
                con[stop1] = "1R1";
            }
            if (side == "black" && cur[63-beg1] == "2R0") {
                con[stop][2] = '1';
                con[63-beg1] = blank;
                con[63-stop1] = "1R1";
            }
        }
    }

    //pawn promotion
    if (cur[beg] == "1P2" && step[3] == one) {
        if (step[4] == 'b') {con[stop][1] = 'B';}
        if (step[4] == 'n') {con[stop][1] = 'N';}
        if (step[4] == 'q') {con[stop][1] = 'Q';}
        if (step[4] == 'r') {con[stop][1] = 'R';}
    }
    else if (cur[beg] == "2P2" && step[3] == eight) {
        if (step[4] == 'b') {con[stop][1] = 'B';}
        if (step[4] == 'n') {con[stop][1] = 'N';}
        if (step[4] == 'q') {con[stop][1] = 'Q';}
        if (step[4] == 'r') {con[stop][1] = 'R';}
    }
}


//make a new node by steping now with s, uses step_board
node *step_node(node *now, string s) {
    node *temp;
    temp = new node;
    now->next_list[now->next] = temp;
    now->next++;
    temp->next = 0;
    temp->next_list[0] = NULL;
    if (now->turn=='1'){
        temp->turn='2';
    }
    else {
        temp->turn = '1';
    }
    step_board(s, now->board, temp->board);
    return temp;
}


//static evaluation function, zero sum
int eval(string *board) {
    //score
    int score = 0;

    if (round == 0) {
        return score;
    }

    //points by piece values
    int piece1 = 0;
    int piece2 = 0;
    int king1 = 0;
    int king2 = 0;
    for (int i=0; i<64; i++) {
        if (board[i][0] == '1') {
            if (board[i][1] == 'P') {piece1+=p_val;}
            if (board[i][1] == 'R') {piece1+=r_val;}
            if (board[i][1] == 'N') {piece1+=n_val;}
            if (board[i][1] == 'B') {piece1+=b_val;}
            if (board[i][1] == 'Q') {piece1+=q_val;}
            if (board[i][1] == 'K') {king1=1;}
            }
        else if (board[i][0] == '2') {
            if (board[i][1] == 'P') {piece2+=p_val;}
            if (board[i][1] == 'R') {piece2+=r_val;}
            if (board[i][1] == 'N') {piece2+=n_val;}
            if (board[i][1] == 'B') {piece2+=b_val;}
            if (board[i][1] == 'Q') {piece2+=q_val;}
            if (board[i][1] == 'K') {king2=1;}
        }
    }

    if (king1==0) {
        return -1000;
    }
    if (king1==1 && king2==0) {
        return 1000;
    }

    score = piece1-piece2;

    return score;
}

//make all valid moves function
void make_move(node *now) {
    int capture = 0;
    int ind = 0;
    int eat_ind = 0;
    int starve_ind = 0;
    char me = now->turn;

    string eat [220];
    string starve [220];

    for (int i = 0; i<64; i++) {
        if (now->board[i][0]!=me) {continue;}
        char type = now->board[i][1];
        string pos = ind_to_pos(i);
        string post;

        //PAWN
        if (type == 'P') {
            //pawn 1
            if (now->board[i][0] == '1') {
                //can eat
                //up right
                if ((now->board[i-8+1][0]=='2') && (i%8!=7)) {
                    post = ind_to_pos(i-8+1);
                    //promotion
                    if (post[1] == one) {
                        eat[eat_ind]=pos+post+"q";
                        eat_ind++;
                        ind++;
                        eat[eat_ind]=pos+post+"n";
                        eat_ind++;
                        ind++;
                        eat[eat_ind]=pos+post+"b";
                        eat_ind++;
                        ind++;
                        eat[eat_ind]=pos+post+"r";
                        eat_ind++;
                        ind++;
                    }
                    eat[eat_ind]=pos+post;
                    eat_ind++;
                    ind++;
                    capture = 1;
                }
                //up left
                if ((now->board[i-8-1][0]=='2') && (i%8!=0)) {
                    post = ind_to_pos(i-8-1);
                    //promotion
                    if (post[1] == one) {
                        eat[eat_ind]=pos+post+"q";
                        eat_ind++;
                        ind++;
                        eat[eat_ind]=pos+post+"n";
                        eat_ind++;
                        ind++;
                        eat[eat_ind]=pos+post+"b";
                        eat_ind++;
                        ind++;
                        eat[eat_ind]=pos+post+"r";
                        eat_ind++;
                        ind++;
                    }
                    eat[eat_ind]=pos+post;
                    eat_ind++;
                    ind++;
                    capture = 1;
                }
                //en passant
                if (i/8==3) {
                    //up right
                    if ((now->board[i+1]=="2P1") && i%8!=7) {
                        post = ind_to_pos(i-8+1);
                        eat[eat_ind]=pos+post;
                        eat_ind++;
                        ind++;
                        capture = 1;
                    }
                    //up left
                    if ((now->board[i-1]=="2P1") && i%8!=0) {
                        post = ind_to_pos(i-8-1);
                        eat[eat_ind]=pos+post;
                        eat_ind++;
                        ind++;
                        capture = 1;
                    }
                }
                //can't eat
                else {
                    //double walk
                    if (now->board[i] == "1P0" && now->board[i-8]==blank && now->board[i-16]==blank) {
                        post = ind_to_pos(i-16);
                        starve[starve_ind]=pos+post;
                        starve_ind++;
                        ind++;
                    }
                    post = ind_to_pos(i-8);
                    //normal walk
                    if (post[1] != one && now->board[i-8]==blank) {
                        starve[starve_ind]=pos+post;
                        starve_ind++;
                        ind++;
                    }
                    //promotion
                    if (post[1] == one && now->board[i-8]==blank) {
                        starve[starve_ind]=pos+post+"q";
                        starve_ind++;
                        ind++;
                        starve[starve_ind]=pos+post+"n";
                        starve_ind++;
                        ind++;
                        starve[starve_ind]=pos+post+"b";
                        starve_ind++;
                        ind++;
                        starve[starve_ind]=pos+post+"r";
                        starve_ind++;
                        ind++;
                    }
                }
            }
            //pawn 2
            else {
                //can eat
                //down right
                if ((now->board[i+8+1][0]=='1') && (i%8!=7)) {
                    post = ind_to_pos(i+8+1);
                    //promotion
                    if (post[1] == eight) {
                        eat[eat_ind]=pos+post+"q";
                        eat_ind++;
                        ind++;
                        eat[eat_ind]=pos+post+"n";
                        eat_ind++;
                        ind++;
                        eat[eat_ind]=pos+post+"b";
                        eat_ind++;
                        ind++;
                        eat[eat_ind]=pos+post+"r";
                        eat_ind++;
                        ind++;
                    }
                    else{
                        eat[eat_ind]=pos+post;
                        eat_ind++;
                        capture = 1;
                    }
                }
                //down left
                //can eat
                if ((now->board[i+8-1][0]=='1') && (i%8!=0)) {
                    post = ind_to_pos(i+8-1);
                    //promotion
                    if (post[1] == eight) {
                        eat[eat_ind]=pos+post+"q";
                        eat_ind++;
                        ind++;
                        eat[eat_ind]=pos+post+"n";
                        eat_ind++;
                        ind++;
                        eat[eat_ind]=pos+post+"b";
                        eat_ind++;
                        ind++;
                        eat[eat_ind]=pos+post+"r";
                        eat_ind++;
                        ind++;
                    }
                    eat[eat_ind]=pos+post;
                    eat_ind++;
                    ind++;
                    capture = 1;
                }
                //en passant
                if (i/8==4) {
                    //down right
                    if ((now->board[i+1]=="1P1") && i%8!=7) {
                        post = ind_to_pos(i+8+1);
                        eat[eat_ind]=pos+post;
                        eat_ind++;
                        ind++;
                        capture = 1;
                    }
                    //down left
                    if ((now->board[i-1]=="1P1") && i%8!=0) {
                        post = ind_to_pos(i+8-1);
                        eat[eat_ind]=pos+post;
                        eat_ind++;
                        ind++;
                        capture = 1;
                    }
                }
                //can't eat
                else {
                    //double walk
                    if (now->board[i] == "2P0" && now->board[i+8]==blank && now->board[i+16]==blank) {
                        post = ind_to_pos(i+16);
                        starve[starve_ind]=pos+post;
                        starve_ind++;
                        ind++;
                    }
                    post = ind_to_pos(i+8);
                    //normal walk
                    if (post[1] != eight && now->board[i+8]==blank) {
                        starve[starve_ind]=pos+post;
                        starve_ind++;
                        ind++;
                    }
                    //promotion
                    if (post[1] == eight && now->board[i+8]==blank) {
                        starve[starve_ind]=pos+post+"q";
                        starve_ind++;
                        ind++;
                        starve[starve_ind]=pos+post+"n";
                        starve_ind++;
                        ind++;
                        starve[starve_ind]=pos+post+"b";
                        starve_ind++;
                        ind++;
                        starve[starve_ind]=pos+post+"r";
                        starve_ind++;
                        ind++;
                    }
                }
            }
        }

        //ROOK
        if (type == 'R' || type =='Q') {
            //left
            for (int j=i-1; j>=i/8*8; j--) {
                if (capture == 0 && now->board[j] == blank) {
                    post = ind_to_pos(j);
                    starve[starve_ind] = pos+post;
                    starve_ind++;
                    ind++;
                }
                else if (now->board[j][0]!= now->turn && now->board[j] != blank) {
                    post = ind_to_pos(j);
                    eat[eat_ind] = pos+post;
                    eat_ind++;
                    ind++;
                    capture = 1;
                    break;
                }
                if (now->board[j]!=blank) {break;}
            }
            //right
            for (int j=i+1; j<(i+8)/8*8; j++) {
                if (capture == 0 && now->board[j] == blank) {
                    post = ind_to_pos(j);
                    starve[starve_ind] = pos+post;
                    starve_ind++;
                    ind++;
                }
                else if (now->board[j][0]!= now->turn && now->board[j] != blank) {
                    post = ind_to_pos(j);
                    eat[eat_ind] = pos+post;
                    eat_ind++;
                    ind++;
                    capture = 1;
                    break;
                }
                if (now->board[j]!=blank) {break;}
            }
            //up
            for (int j=i-8; j>=0; j-=8) {
                if (capture == 0 && now->board[j] == blank) {
                    post = ind_to_pos(j);
                    starve[starve_ind] = pos+post;
                    starve_ind++;
                    ind++;
                }
                else if (now->board[j][0]!= now->turn && now->board[j] != blank) {
                    post = ind_to_pos(j);
                    eat[eat_ind] = pos+post;
                    eat_ind++;
                    ind++;
                    capture = 1;
                    break;
                }
                if (now->board[j]!=blank) {break;}
            }
            //down
            for (int j=i+8; j<64; j+=8) {
                if (capture == 0 && now->board[j] == blank) {
                    post = ind_to_pos(j);
                    starve[starve_ind] = pos+post;
                    starve_ind++;
                    ind++;
                }
                else if (now->board[j][0]!= now->turn && now->board[j] != blank) {
                    post = ind_to_pos(j);
                    eat[eat_ind] = pos+post;
                    eat_ind++;
                    ind++;
                    capture = 1;
                    break;
                }
                if (now->board[j]!=blank) {break;}
            }
        }

        //BISHOP
        if (type == 'B' || type =='Q') {
            int row = i/8-1;
            int col = i%8-1;
            int j;
            //up left
            while (row>=0 && col>=0) {
                j = row*8+col;
                if ((capture==0) && (now->board[j]==blank)) {
                    post = ind_to_pos(j);
                    starve[starve_ind] = pos+post;
                    starve_ind++;
                    ind++;
                }
                else if (now->board[j][0]!=now->turn && now->board[j]!=blank) {
                    post = ind_to_pos(j);
                    eat[eat_ind] = pos+post;
                    eat_ind++;
                    ind++;
                    capture = 1;
                    break;
                }
                if (now->board[j]!=blank) {break;}
                row--;
                col--;
            }
            row = i/8+1;
            col = i%8-1;
            //down left
            while (row<=7 && col>=0) {
                j = row*8+col;
                if ((capture==0) && (now->board[j]==blank)) {
                    post = ind_to_pos(j);
                    starve[starve_ind] = pos+post;
                    starve_ind++;
                    ind++;
                }
                else if (now->board[j][0]!=now->turn && now->board[j]!=blank) {
                    post = ind_to_pos(j);
                    eat[eat_ind] = pos+post;
                    eat_ind++;
                    ind++;
                    capture = 1;
                    break;
                }
                if (now->board[j]!=blank) {break;}
                row++;
                col--;
            }
            row = i/8-1;
            col = i%8+1;
            //up right
            while (row>=0 && col<=7) {
                j = row*8+col;
                if ((capture==0) && (now->board[j]==blank)) {
                    post = ind_to_pos(j);
                    starve[starve_ind] = pos+post;
                    starve_ind++;
                    ind++;
                }
                else if (now->board[j][0]!=now->turn && now->board[j]!=blank) {
                    post = ind_to_pos(j);
                    eat[eat_ind] = pos+post;
                    eat_ind++;
                    ind++;
                    capture = 1;
                    break;
                }
                if (now->board[j]!=blank) {break;}
                row--;
                col++;
            }
            row = i/8+1;
            col = i%8+1;
            //down right
            while (row<=7 && col<=7) {
                j = (row)*8+col;
                if ((capture==0) && (now->board[j]==blank)) {
                    post = ind_to_pos(j);
                    starve[starve_ind] = pos+post;
                    starve_ind++;
                    ind++;
                }
                else if (now->board[j][0]!=now->turn && now->board[j]!=blank) {
                    post = ind_to_pos(j);
                    eat[eat_ind] = pos+post;
                    eat_ind++;
                    ind++;
                    capture = 1;
                    break;
                }
                if (now->board[j]!=blank) {break;}
                row++;
                col++;
            }
        }

        //KNIGHT
        else if (type == 'N') {
            int row = i/8;
            int col = i%8;
            int j;
            //up up
            if (row >= 2) {
                //up up left
                if (col>=1) {
                    j = (row-2)*8+col-1;
                    if ((capture==0) && (now->board[j]==blank)) {
                        post = ind_to_pos(j);
                        starve[starve_ind] = pos+post;
                        starve_ind++;
                        ind++;
                    }
                    else if (now->board[j]!=blank && now->board[j][0]!=now->turn) {
                        post = ind_to_pos(j);
                        eat[eat_ind] = pos+post;
                        eat_ind++;
                        ind++;
                        capture = 1;
                    }
                }
                //up up right
                if (col<=6) {
                    j = (row-2)*8+col+1;
                    if ((capture==0) && (now->board[j]==blank)) {
                        post = ind_to_pos(j);
                        starve[starve_ind] = pos+post;
                        starve_ind++;
                        ind++;
                    }
                    else if (now->board[j]!=blank && now->board[j][0]!=now->turn) {
                        post = ind_to_pos(j);
                        eat[eat_ind] = pos+post;
                        eat_ind++;
                        ind++;
                        capture = 1;
                    }
                }
            }
            //down down
            if (row <= 5) {
                //down down left
                if (col>=1) {
                    j = (row+2)*8+col-1;
                    if ((capture==0) && (now->board[j]==blank)) {
                        post = ind_to_pos(j);
                        starve[starve_ind] = pos+post;
                        starve_ind++;
                        ind++;
                    }
                    else if (now->board[j]!=blank && now->board[j][0]!=now->turn) {
                        post = ind_to_pos(j);
                        eat[eat_ind] = pos+post;
                        eat_ind++;
                        ind++;
                        capture = 1;
                    }
                }
                //down down right
                if (col<=6) {
                    j = (row+2)*8+col+1;
                    if ((capture==0) && (now->board[j]==blank)) {
                        post = ind_to_pos(j);
                        starve[starve_ind] = pos+post;
                        starve_ind++;
                        ind++;
                    }
                    else if (now->board[j]!=blank && now->board[j][0]!=now->turn) {
                        post = ind_to_pos(j);
                        eat[eat_ind] = pos+post;
                        eat_ind++;
                        ind++;
                        capture = 1;
                    }
                }
            }
            //right right
            if (col <= 5) {
                //right right up
                if (row>=1) {
                    j = (row-1)*8+col+2;
                    if ((capture==0) && (now->board[j]==blank)) {
                        post = ind_to_pos(j);
                        starve[starve_ind] = pos+post;
                        starve_ind++;
                        ind++;
                    }
                    else if (now->board[j]!=blank && now->board[j][0]!=now->turn) {
                        post = ind_to_pos(j);
                        eat[eat_ind] = pos+post;
                        eat_ind++;
                        ind++;
                        capture = 1;
                    }
                }
                //right right down
                if (row<=6) {
                    j = (row+1)*8+col+2;
                    if ((capture==0) && (now->board[j]==blank)) {
                        post = ind_to_pos(j);
                        starve[starve_ind] = pos+post;
                        starve_ind++;
                        ind++;
                    }
                    else if (now->board[j]!=blank && now->board[j][0]!=now->turn) {
                        post = ind_to_pos(j);
                        eat[eat_ind] = pos+post;
                        eat_ind++;
                        ind++;
                        capture = 1;
                    }
                }
            }
            //left left
            if (col >= 2) {
                //left left up
                if (row>=1) {
                    j = (row-1)*8+col-2;
                    if ((capture==0) && (now->board[j]==blank)) {
                        post = ind_to_pos(j);
                        starve[starve_ind] = pos+post;
                        starve_ind++;
                        ind++;
                    }
                    else if (now->board[j]!=blank && now->board[j][0]!=now->turn) {
                        post = ind_to_pos(j);
                        eat[eat_ind] = pos+post;
                        eat_ind++;
                        ind++;
                        capture = 1;
                    }
                }
                //left left down
                if (row<=6) {
                    j = (row+1)*8+col-2;
                    if ((capture==0) && (now->board[j]==blank)) {
                        post = ind_to_pos(j);
                        starve[starve_ind] = pos+post;
                        starve_ind++;
                        ind++;
                    }
                    else if (now->board[j]!=blank && now->board[j][0]!=now->turn) {
                        post = ind_to_pos(j);
                        eat[eat_ind] = pos+post;
                        eat_ind++;
                        ind++;
                        capture = 1;
                    }
                }
            }
        }

        //KING
        else if (type == 'K') {
            //castling
            //assigned white
            if (capture==0 && side=="white") {
                //self castle left
                if (now->board[i]=="1K0") {
                    if (now->board[56]=="1R0" &&
                        now->board[57]==blank &&
                        now->board[58]==blank &&
                        now->board[59]==blank) {
                        starve[starve_ind] = "e1c1";
                        starve_ind++;
                        ind++;
                    }
                }
                //self castle right
                if (now->board[i]=="1K0") {
                    if (now->board[63]=="1R0" &&
                        now->board[62]==blank &&
                        now->board[61]==blank) {
                        starve[starve_ind] = "e1g1";
                        starve_ind++;
                        ind++;
                    }
                }
                //opp castle left
                if (now->board[i]=="2K0") {
                    if (now->board[0]=="2R0" &&
                        now->board[1]==blank &&
                        now->board[2]==blank &&
                        now->board[3]==blank) {
                        starve[starve_ind] = "e8c8";
                        starve_ind++;
                        ind++;
                    }
                }
                //opp castle right
                if (now->board[i]=="2K0") {
                    if (now->board[7]=="2R0" &&
                        now->board[6]==blank &&
                        now->board[5]==blank) {
                        starve[starve_ind] = "e8g8";
                        starve_ind++;
                        ind++;
                    }
                }
            }
            //assigned black
            if (capture==0 && side=="black") {
                //self castle right
                if (now->board[i]=="1K0") {
                    if (now->board[63]=="1R0" &&
                        now->board[62]==blank &&
                        now->board[61]==blank &&
                        now->board[60]==blank) {
                        starve[starve_ind] = "e8g8";
                        starve_ind++;
                        ind++;
                    }
                }
                //self castle left
                if (now->board[i]=="1K0") {
                    if (now->board[56]=="1R0" &&
                        now->board[57]==blank &&
                        now->board[58]==blank) {
                        starve[starve_ind] = "e8c8";
                        starve_ind++;
                        ind++;
                    }
                }
                //opp castle right
                if (now->board[i]=="2K0") {
                    if (now->board[7]=="2R0" &&
                        now->board[6]==blank &&
                        now->board[5]==blank &&
                        now->board[4]==blank) {
                        starve[starve_ind] = "e1c1";
                        starve_ind++;
                        ind++;
                    }
                }
                //opp castle left
                if (now->board[i]=="2K0") {
                    if (now->board[0]=="2R0" &&
                        now->board[1]==blank &&
                        now->board[2]==blank) {
                        starve[starve_ind] = "e1g1";
                        starve_ind++;
                        ind++;
                    }
                }
            }
            int row = i/8;
            int col = i%8;
            int j;
            //up
            //up
            if (row>=1) {
                //up left
                if (col>=1) {
                    j = (row-1)*8+col-1;
                    if ((capture==0) && (now->board[j]==blank)) {
                        post = ind_to_pos(j);
                        starve[starve_ind] = pos+post;
                        starve_ind++;
                        ind++;
                    }
                    else if (now->board[j][0]!=now->turn && now->board[j]!=blank) {
                        post = ind_to_pos(j);
                        eat[eat_ind] = pos+post;
                        eat_ind++;
                        ind++;
                        capture = 1;
                    }
                }
                //up right
                if (col<=6) {
                    j = (row-1)*8+col+1;
                    if ((capture==0) && (now->board[j]==blank)) {
                        post = ind_to_pos(j);
                        starve[starve_ind] = pos+post;
                        starve_ind++;
                        ind++;
                    }
                    else if (now->board[j][0]!=now->turn && now->board[j]!=blank) {
                        post = ind_to_pos(j);
                        eat[eat_ind] = pos+post;
                        eat_ind++;
                        ind++;
                        capture = 1;
                    }
                }
                j = (row-1)*8+col;
                //plain up
                if ((capture==0) && (now->board[j]==blank)) {
                    post = ind_to_pos(j);
                    starve[starve_ind] = pos+post;
                    starve_ind++;
                    ind++;
                }
                else if (now->board[j][0]!=now->turn && now->board[j]!=blank) {
                    post = ind_to_pos(j);
                    eat[eat_ind] = pos+post;
                    eat_ind++;
                    ind++;
                    capture = 1;
                }
            }
            //down
            //down
            if (row<=6) {
                //down left
                if (col>=1) {
                    j = (row+1)*8+col-1;
                    if ((capture==0) && (now->board[j]==blank)) {
                        post = ind_to_pos(j);
                        starve[starve_ind] = pos+post;
                        starve_ind++;
                        ind++;
                    }
                    else if (now->board[j][0]!=now->turn && now->board[j]!=blank) {
                        post = ind_to_pos(j);
                        eat[eat_ind] = pos+post;
                        eat_ind++;
                        ind++;
                        capture = 1;
                    }
                }
                //down right
                if (col<=6) {
                    j = (row+1)*8+col+1;
                    if ((capture==0) && (now->board[j]==blank)) {
                        post = ind_to_pos(j);
                        starve[starve_ind] = pos+post;
                        starve_ind++;
                        ind++;
                    }
                    else if (now->board[j][0]!=now->turn && now->board[j]!=blank) {
                        post = ind_to_pos(j);
                        eat[eat_ind] = pos+post;
                        eat_ind++;
                        ind++;
                        capture = 1;
                    }
                }
                j = (row+1)*8+col;
                //plain down
                if ((capture==0) && (now->board[j]==blank)) {
                    post = ind_to_pos(j);
                    starve[starve_ind] = pos+post;
                    starve_ind++;
                    ind++;
                }
                else if (now->board[j][0]!=now->turn && now->board[j]!=blank) {
                    post = ind_to_pos(j);
                    eat[eat_ind] = pos+post;
                    eat_ind++;
                    ind++;
                    capture = 1;
                }
            }
            //plain left
            //left
            if (col>=1) {
                j = i-1;
                if ((capture==0) && (now->board[j]==blank)) {
                    post = ind_to_pos(j);
                    starve[starve_ind] = pos+post;
                    starve_ind++;
                    ind++;
                }
                else if (now->board[j][0]!=now->turn && now->board[j]!=blank) {
                    post = ind_to_pos(j);
                    eat[eat_ind] = pos+post;
                    eat_ind++;
                    ind++;
                    capture = 1;
                }
            }
            //right
            //plain right
            if (col<=6) {
                j = i+1;
                if ((capture==0) && (now->board[j]==blank)) {
                    post = ind_to_pos(j);
                    starve[starve_ind] = pos+post;
                    starve_ind++;
                    ind++;
                }
                else if (now->board[j][0]!=now->turn  && now->board[j]!=blank) {
                    post = ind_to_pos(j);
                    eat[eat_ind] = pos+post;
                    eat_ind++;
                    ind++;
                    capture = 1;
                }
            }
        }

    }

    //copy moves over
    int moves_ind = 0;
    if (capture==0) {
        while (starve_ind>0) {
            now->moves[moves_ind] = starve[starve_ind-1];
            moves_ind++;
            starve_ind--;
        }
    }
    while (eat_ind>0) {
        now->moves[moves_ind] = eat[eat_ind-1];
        moves_ind++;
        eat_ind--;
    }
    now->moves[moves_ind] = blank;
}

//new future
int future (node* cur, int depth, int bot, int a, int b) {
    if (depth==bot) {
        int score=eval(cur->board);
        delete cur;
        return score;
    }
    make_move(cur);
    int best_score = -1000;

    for (int i=0;i<220;i++) {
        string step = cur->moves[i];
        if (step==blank) {
            cur->next_list[cur->next]=NULL;
            break;
        }
        node *temp = new node;
        if (cur->turn=='1') {temp->turn='2';}
        if (cur->turn=='2') {temp->turn='1';}
        step_board(step,cur->board,temp->board);
        temp->next = 0;
        temp->next_list[temp->next]=NULL;

        int score = future(temp,depth+1,bot,-b, -max(a,best_score));
        score=-score;

        if (score>best_score) {
            if (depth==0) {best_move=step;}
            best_score=score;

            if (best_score>=b) {
                if (depth!=0) {delete cur;}
                return best_score;
            }
        }

    }
    if (depth!=0) {delete cur;}
    return best_score;
}

int main(int argc, char *argv[])
{
    //store side assigned
    string side = argv[1];

    //depending on side, assign variables
    if (side == "white"){
        one = '8';
        two = '7';
        three = '6';
        four = '5';
        five = '4';
        six = '3';
        seven = '2';
        eight = '1';
        A = 'a';
        B = 'b';
        C = 'c';
        D = 'd';
        E = 'e';
        F = 'f';
        G = 'g';
        H = 'h';
    };
    if (side == "black") {
        one = '1';
        two = '2';
        three = '3';
        four = '4';
        five = '5';
        six = '6';
        seven = '7';
        eight = '8';
        A = 'h';
        B = 'g';
        C = 'f';
        D = 'e';
        E = 'd';
        F = 'c';
        G = 'b';
        H = 'a';
    };

    //set up initial board
    node *now = new node;
    now->next = 0;
    for (int i = 0; i<220;i++) {now->next_list[0]=NULL;}
    //putting blanks on board
    for (int i = 0; i<64; i++) {
        now->board[i] = blank;
    }
    //putting pieces onto initial board
    if (side == "white") {
        now->turn = '1';
        now->board[0] = "2R0";
        now->board[1] = "2N0";
        now->board[2] = "2B0";
        now->board[3] = "2Q0";
        now->board[4] = "2K0";
        now->board[5] = "2B0";
        now->board[6] = "2N0";
        now->board[7] = "2R0";
        for (int i = 8; i<16; i++) {
            now->board[i] = "2P0";
        };
        now->board[56] = "1R0";
        now->board[57] = "1N0";
        now->board[58] = "1B0";
        now->board[59] = "1Q0";
        now->board[60] = "1K0";
        now->board[61] = "1B0";
        now->board[62] = "1N0";
        now->board[63] = "1R0";
        for (int i = 48; i<56 ;i++) {
            now->board[i] = "1P0";
        };
    };
    if (side == "black") {
        now->turn = '2';
        now->board[0] = "2R0";
        now->board[1] = "2N0";
        now->board[2] = "2B0";
        now->board[3] = "2K0";
        now->board[4] = "2Q0";
        now->board[5] = "2B0";
        now->board[6] = "2N0";
        now->board[7] = "2R0";
        for (int i=8; i<16; i++) {
            now->board[i] = "2P0";
        };
        now->board[56] = "1R0";
        now->board[57] = "1N0";
        now->board[58] = "1B0";
        now->board[59] = "1K0";
        now->board[60] = "1Q0";
        now->board[61] = "1B0";
        now->board[62] = "1N0";
        now->board[63] = "1R0";
        for (int i=48; i<56 ;i++) {
            now->board[i] = "1P0";
        };
    }

    round++;

    //if white, make first move
    if (side=="white") {
            //decide best move with min-max and alpha-beta pruning
            future(now, 0, bravery,-1000, 1000);

            //50-move counter
            if (capture(best_move, now->board)==0) {fifty++;}
            else {fifty=0;}

            //output move and advance board
            cout<<best_move<<endl;
            node *temp = now;
            now = step_node(now, best_move);
            delete temp;
    }

    //reaction loop
    while (1){
        using namespace std;

        //store opponent's move
        getline(cin,input);

        //if endgame, AI terminates properly
        if (input == "1-0"){break;}
        if (input == "0-1"){break;}
        if (input == "1/2-1/2"){break;}

        //50-move counter
        if (capture(input, now->board)==0) {fifty++;}
        else {fifty=0;}

        //advance board according to input
        node *temp = now;
        now = step_node(now, input);
        delete temp;

        //FOR DEBUG: PRINTING BOARD
        for (int i=0; i<64; i++){
            cout << now->board[i] << "|";
            if (i%8 == 7) {
                cout << "\n";
            }
        }


        round++;

        //50-move rule check
        if (fifty>=50) {
            cout<<"1/2-1/2"<<endl;
            break;
        }

        //make list of possible moves
        make_move(now);

        //decide on move
        future(now, 0, bravery, -1000, 1000);

        //50-move counter
        if (capture(best_move, now->board)==0) {fifty++;}
        else {fifty=0;}

        //output move and advance board
        cout<<best_move<<endl;
        temp = now;
        now = step_node(now, best_move);
        delete temp;

        //FOR DEBUG: PRINTING BOARD
        for (int i=0; i<64; i++){
            cout << now->board[i] << "|";
            if (i%8 == 7) {
                cout << "\n";
            }
        }

    }
    return 0;
}
