static cell matrix[BoardSize][BoardSize];

void 	ShutDown();
void    Initialise(void);
void 	Reset();
void	ResetGlobals();
void    FirstMove(void); // Orange opens
void    SecondMove(void); // Green replies with one of two choices

cell    GetPlay();

void    DrawBoard(void);
void    DisplayCombo(void);
void    DisplayStats();
void    Toggle();
void    UnToggle();
void    Congrats(cell Winner);

int IsAlt(int x, int y);
int IsInside(int x, int y);

void ShowCell(int x,int y);
void DrawSquare(int x,int y);
int SelectCell(int x,int y);

/* Cursor movementation */
void CStamp(int x, int y);
void CUnStamp(int x, int y);

void UpdateScreen(void);
void ShowSide();
cell other(cell side_to_reverse);
void PlotSide(int Tox,int Toy,cell side_to_plot);

int WaitOnInput();

void Move(int x, int y, int dx, int dy);
move ValidateMove(int x, int y, int dx, int dy);
mvflag VerifyEat(int x,int y,move mvtyp);
void DoEat(int x,int y,move mvtyp);
int SuicideCheck(int x, int y, int dx, int dy);
int ValidateSecondOrigin(int x, int y, int dx, int dy);

void SaveScreen();//hak 2001-july-8 to save a screnshot

#define doutfile "dout.log" // - Debug OUTput

int dullstart = FALSE;
int verbose = FALSE;
int debug = FALSE;

int game_count = 0;

//DATAFILE *data = 0; // ensure NULL status

BITMAP *s, *cur, *cstamp;
BITMAP *p1, *p2;

int cx = 0;
int cy = 0;

int x1,x2,y1,y2;

int AbortNow = FALSE;
int Combo = 0;
int MaxCombo = 0;
int status_line = TRUE;

cell side = Empty;
cell last_side = side;
cell MaxComboSide = side;
cell Winner = side;

int use_retrace_proc = FALSE;
int auto_blit = TRUE;

char status[80];

move last_move_type = Imp;

static FILE* doutput;
int dcount = 0;
void dout(char *bufd);

int nike();// just do it - main
//int clinit(int argc, char  *argv);

int sshot_rq(); // TRUE if screen shot requested (by user)