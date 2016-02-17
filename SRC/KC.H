#define build_string "KC v1.0.6,\"Wibbly Phoenix!\""

// From here on, it is flange!

#define BoardSize 8

typedef enum cell{Empty,Orange,Green};
// each square on the gameboard is called a "cell"

// Inner  Court limits

const int BorderSize = 2;
int Xlo = BorderSize;
int Ylo = BorderSize;
int Xhi = BoardSize - (BorderSize +1);
int Yhi = BoardSize - (BorderSize +1);

// Players

int Active[3]={0,24,24};
// 24 players per side.

int Inside[3]={0,0,0};
//Inner court starts empty

/* And now for some useful stuff...
** position the board right in the middle of the 640x480 screen.
*/

int   BoardX = 192;
int   BoardY = 112;

int   LineColor = 93;
int   TestColor = 2;
int   BoardColor= 105;
int   BackColor[3] = { 205, 105, 215}; // Background colors??

#define   SquareSz 32
// each square, or cell, is 32x32 pixels.

/* move types */

typedef enum move{Imp,MvUp,MvDn,MvLf,MvRt,EatUp,EatDn,EatLf,EatRt};

const int   xe[9]={0,0,0,0,0, 0, 0,-1,+1};
const int   ye[9]={0,0,0,0,0,-1,+1, 0, 0};

typedef enum mvflag{ImpL,Mv,Eat};

//int nike();// just do it - main
