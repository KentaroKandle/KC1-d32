
/* kc.cpp; Copyright -2015 ELI K LAN [r].BARBADOS. Trade Secret.
	Internal Use Only. Not for public vision.
	based on July 2001 "1.0.6" re-release with minor fixes (and same major flaws!)
	(see KC.HPP for current version, build_string, etc.)

	based on
	KC, a little project by
		Keinall H. Caddle.

	Please see "doc" directory, for some (somewhat outdated and incomplete) 
	developer-friendly (or, at least, common person unfriendly) info. 

		"If you put your mind to it,
		you can be good to yourself all of the time."
		- Former Overlord Gran(d)it(h)or
	
	BDSM: 
		For those who are submissive, in search of a master,
		you can also cum under the Fifth Pillearh Overlord....ship.
	
	Do you consider me a worthy potential owner? 
	Only part-time, though. 
	Mortgage or something to pay (midas reality bites).
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#include "allegro.h"
#include "sfchar.h" // for the data file.

#include "kc.hpp"
#include "kci.hpp"

DATAFILE *data;
bool disable_joystick = true;

int main(int argc, char *argv[])
{
   int c;

   for (c=1; c<argc; c++) {
	   if (stricmp(argv[c], "+joy") == 0) {
		   disable_joystick = false;
	   }; 
	   if (stricmp(argv[c], "-joy") == 0) {
		   disable_joystick = true;
	   };
	   
	if (stricmp(argv[c], "-debug") == 0) {
	debug = TRUE;

   doutput = fopen(doutfile, "w");
   if (doutput == NULL)
      {
      printf("\nFatal Error: File for dout() cannot be opened\n\tError # %d",errno);
      exit(errno);
      }

   dout("\nDebug mode enabled!\n");

   fprintf(doutput, "\nSourceFile : %s. Compiled : %s @ %s.\n",__FILE__,__DATE__,__TIME__);

   }
      if (stricmp(argv[c], "-dullstart") == 0)
   {
   dullstart = TRUE;
   dout("\nCheat mode enabled!\n");
   }
      if ((stricmp(argv[c], "-v") == 0) || (stricmp(argv[c], "-verbose") == 0))
   {
   verbose = TRUE;
      dout("\nVerbose mode enabled!\n");
   }

   }

   return nike();
}


int nike() {
	cell NowWinner = Empty;
  	Initialise(); // Setup the game machine
	
	int done = FALSE;
	int loop = 0;

//	for(int loop=0; loop < 10; loop++) {
	do  {
		loop++;

		if (AbortNow == TRUE) {
			ShutDown();
			return 0;
		}

	  switch(loop) {

		case 1:

			cx = 1; cy = 3;//init cursor position
			side = matrix[cx][cy];// right effect, bad logic. orange always first by game rules.

			FirstMove();
			break;

		case 2:
			SecondMove();
			break;

		case 3:  // "fencepost" state
		/* we would do some sorta system tasks here */ 
			break;

		case 4:

		  DisplayStats();
		   last_side = side;
		   NowWinner = GetPlay();
		   DisplayStats();
		   Toggle();

		   if (NowWinner != Empty) {
		   		Congrats(NowWinner);// may set AbortNow

		   		if (AbortNow == FALSE) {
		   			ResetGlobals();
		   			Reset();
		   			loop = 0;
				}
			}

		   break;

		case 5: // "bouncing fenceend" state
			loop = 3; break;
		}

	} while (!done); // --- should never reach here!

//fallthru, impossible

	ShutDown();
	printf("\n\n\tAck! Imp!\n");
	return 0;
}

void ShutDown()
{
	dout("\nShutting down Allegro...\n");

	set_gfx_mode(GFX_TEXT, 0,0,0,0);

	printf("\n\n\tBye!\n");
	printf("This was: %s\nUsing game library: %s\n", build_string, allegro_id);	
	
	destroy_bitmap(s); // destroy Screen buffer

	unload_datafile(data);
	dout("\nDatafile unloaded.");

	allegro_exit();

	//   cputs(data[END_TEXT].dat);

	if(debug) {
		dout("\nLast Debugging OUTput message... Closing all files\n");
		fclose(doutput);
		printf("\n%d dout() messages were logged.\n",dcount);
	}
}//ShutDown


void dout(char *bufd)
{
	if(debug) {
	   dcount++;
	   fprintf(doutput, "%s\n", bufd);
	}
}

void ResetGlobals()
{
	cx = 0;	cy = 0;

	AbortNow = FALSE;
	Combo = 0;
	MaxCombo = 0;

	side = Empty;
	last_side = side;
	MaxComboSide = side;
	Winner = side;

	//move last_move_type = Imp;
	dcount = 0;

	status_line = TRUE;

	Active[1]=24; Active[2]=24;

	for (int c = 0 ; c<3 ; c++ ) {
	  Inside[c] = 0;
	}
}//ResetGlobals

void Initialise(void)
{
   int w=640; int h=480;
   char buf[80];

	printf("\nStarting up...\n");

	ResetGlobals();

dout("Initialising...\n");

   allegro_init();
   install_keyboard();
   install_timer();

   initialise_joystick();
//   load_joystick_data(NULL); //restore config

//We don't use sound, but if we did, we'd init it here;

  dout("Primitive Initialisation Complete...\n");

	printf("Now loading...\n");

  strcpy(buf, "SFCHAR.DAT");
  if (exists(buf)) {
      data = load_datafile(buf);
  } else {
    strcpy(buf, "/BUILT/SFCHAR.DAT");
    if (exists(buf)) {
      data = load_datafile(buf);
  } else {
  
	  /* another option : try the internal data file.
	  Not Yet Implemented (tm) */
      dout("\n\t\tAck!!!\n\tDid not find required datafile!\n\n");
      // non-fatal here, but falls thru to spikes...
  }

  if (!data) {// no data, no game.
      allegro_exit();
      dout("Error loading required datafile.\nFatality!\n\n");
      printf("\nError loading required datafile.\n");
      printf("\n\tFatality!\n\n\n\tno data, no game.\n\n");
      exit(1);
   } else
      dout("Data loaded successfully!\n");

	printf("Done loading!\n");

	#ifdef ALLEGRO_CONSOLE_OK
		fade_out(4);
   	#endif
   set_gfx_mode(GFX_AUTODETECT, w, h, 0, 0);
	if (!dullstart) { // a nice logo would be neater here, maybe.
		clear(screen);
		int offset = 16;
	   	rectfill(screen, offset, offset, w-offset, h-offset, 33);
	}

   text_mode(0);

   cur = (BITMAP*)(data[SELECT].dat);
   cstamp = create_bitmap(SquareSz,SquareSz);

   s = create_bitmap(SCREEN_W, SCREEN_H);

   //set_pallete((const PALETTE)data[PAL_GAME].dat);// old code, does not work for C++/C99, I guess.
//   PALLETE pall =  (PALETTE)data[PAL_GAME].dat ; // this does not work either.
   set_pallete( (RGB*)data[PAL_GAME].dat );// just followed the compiler. 

   p1 = (BITMAP*)(data[ORG].dat);
   p2 = (BITMAP*)(data[GRN].dat);

   Reset();
  dout("\nInitialisation complete.\n");
}//Initalise


void Reset()
{
  char buf[120];

  clear(s);

  auto_blit = FALSE;
  DrawBoard();// actually resets and redraws the board

  auto_blit = TRUE;
  sprintf(buf, "%s : Built on %s @ %s.",build_string, __DATE__,__TIME__);
  textprintf(screen, font,  0, 471, TestColor, buf); // footer is the timestamp of creation

  game_count++;// quick hak, temporary diagnostic
  sprintf(buf, "Now Playing Game # %d", game_count);
  textprintf(screen, font, 16, 463, BoardColor, buf);

  UpdateScreen();
}//Reset

void DrawBoard(void)
{ // also resets the board to the initial layout.
   for(int x=0; x<BoardSize; x++) {
      for(int y=0; y<BoardSize; y++) {
			  DrawSquare(x,y);

			  if ( IsInside(x,y) )
				 matrix[x][y]=Empty;
			  else {
				 if ( IsAlt(x,y) )
					matrix[x][y]=Green;
				 else
					matrix[x][y]=Orange;
	      }
	      ShowCell(x,y);
      }
   }
   dout("Board Drawn.");
   clear(screen); UpdateScreen();
   dout("Board Blitted to cleaned screen.");
}

void DrawSquare(int x, int y)
{
	int Tox; int Toy; // Temporary Origins of current square
	int Tlx; int Tly;

	int TmpColor;

	Tox = BoardX + (x*SquareSz) ;
	Toy = BoardY + (y*SquareSz);
	Tlx = (Tox+SquareSz-1);
	Tly = (Toy+SquareSz-1);

	rect(s, Tox, Toy, Tlx, Tly, LineColor);

	TmpColor = BoardColor;
	if (IsInside(x,y)) {
		 TmpColor = BackColor[2];
	}
	rectfill(s, Tox+1, Toy+1, Tlx-1, Tly-1, TmpColor);
}

int	IsAlt(int x, int y)
{
   if ( ( (x & 1) ^ (y & 1) ) == 1) {
      return TRUE; }
   else  return FALSE;
}

int	IsInside(int x, int y)
{
   if ( (x >= Xlo) && (x <= Xhi) && (y >=Ylo) && (y<=Yhi) )
      return TRUE;
   else
      return   FALSE;
}

void ShowCell(int x,int y)
{   // Displays the contents of a cell to the screen by blitting.
   int Tox = BoardX + (x*SquareSz);
   int Toy = BoardY + (y*SquareSz);

   PlotSide(Tox,Toy,matrix[x][y]);
}

int SelectCell(int x,int y)
{
      int tick_wait = 185; // MINIMUM delay between movements of the cursor
      int blink_wait = 25; // let cursour persist for 15 ms.

      int px, py;
      int st = 0;

      do {

      px = x; py = y;
      CStamp(x,y);
      UpdateScreen();
      rest(tick_wait);

	  // TODO: so much refactoring! This is a quick hak to avoid noisy joystick
      if (!disable_joystick) 
		  poll_joystick(); // To counter bounce-back.

do {
      if (!disable_joystick) 
			poll_joystick(); // TODO: write up why not to refactor this.

      if ((key[KEY_RIGHT]) || (joy_right)) x++; if (x>=BoardSize) x = 0;
      if ((key[KEY_LEFT]) || (joy_left)) x--; if (x < 0) x = (BoardSize-1);
      if ((key[KEY_UP]) || (joy_up))y--; if (y < 0) y = (BoardSize-1);
      if ((key[KEY_DOWN]) || (joy_down))y++;if (y>=BoardSize) y=0;
      if ((key[KEY_ESC]))
         { st = -1; AbortNow = TRUE; }
      if ((key[KEY_SPACE]) || (joy_b1))st = 1;
      if ((joy_b2) || (key[KEY_ENTER]))st = 2;

      if (sshot_rq()) SaveScreen();

      if ((key[KEY_C]) && (debug)) {
		dout("\nPreclosing due to user request.");
		dout("\nLast Debugging OUTput message... Closing all files\n");
		fclose(doutput);
		//printf("%d dout() messages were logged.",dcount);
		debug = FALSE;
	  }

} while((px==x) && (py==y) && (st==0));

      CUnStamp(px,py);

      UpdateScreen();
      rest(blink_wait); // rest for x ms.

      } while (st == 0);

      cx = x;
      cy = y;

return st;
}

void CStamp(int x,int y)
{
   int Tox = BoardX + (x*SquareSz);
   int Toy = BoardY + (y*SquareSz);

   blit(s, cstamp, Tox, Toy, 0, 0, SquareSz, SquareSz);
   masked_blit(cur, s, 0, 0, Tox, Toy, SquareSz, SquareSz);
}

void CUnStamp(int x,int y)
{
   int Tox = BoardX + (x*SquareSz);
   int Toy = BoardY + (y*SquareSz);

   blit(cstamp, s, 0, 0, Tox, Toy, SquareSz, SquareSz);
}


void UpdateScreen(void)
{
  if (status_line) {
    rectfill(s, BoardX, 32, SCREEN_W, 41, 0); // clear status bar
    textprintf (s, font,  BoardX, 32, LineColor, status);
  }

  blit(s, screen, 0, 0, 0, 0, SCREEN_W, (SCREEN_H-18));
}

void Toggle()
{

	switch(side) {
		case Green :
			side = Orange; break;
		case Orange :
			side = Green; break;
		default:
			dout("\n\tEgads!\n\tToggle BROKE when called!\n");
			break; // really broken, by the way!
	}

    dout("\nToggle called.\n");
}

cell other(cell side_to_reverse)
{
  if (side_to_reverse == Green) side_to_reverse = Orange;
    else {
       if (side_to_reverse == Orange) side_to_reverse = Green;
       }
  dout("\nother called.\n");
  return side_to_reverse;
}


void UnToggle()
{
   side = last_side;
   dout("\nUnToggle Called");
}


void Move(int x,int y,int dx,int dy)
{
   move type;

   if( (IsInside(dx,dy) && !IsInside(x,y)) ) Inside[side]++;

   if( (IsInside(x,y) && !IsInside(dx,dy)) ) Inside[side]--;

   type = ValidateMove(x,y,dx,dy);

   if(VerifyEat(x, y,type) == Eat) DoEat(x,y,type);

   matrix[dx][dy] = matrix[x][y];
   matrix[x][y] = Empty;

   DrawSquare(x,y);
   ShowCell(dx,dy);

   UpdateScreen();
}

int SuicideCheck(int x, int y, int dx, int dy)
{
  dout("\nsuicide check.");
   cell vside = matrix[x][y];
   if( (IsInside(x,y) && !IsInside(dx,dy)) && (Inside[vside]==1) )
    return TRUE;
   else {
    return FALSE;
   }
  dout("done.\n");
}

cell GetPlay()
{
//   cell winner = Empty;
   int sel_st, xd, yd, xs, ys;
   int lx=cx; int ly=cy;
   int done = FALSE;
   int suicide = FALSE;
   move type;
do
   {

   sprintf(status, "Select piece to move.");

   if(verbose)textprintf (s, font,  (BoardX+64), 0, LineColor, "%d -lmt",last_move_type);

   ShowSide();
   UpdateScreen();

   sel_st = SelectCell(cx,cy);
// sel_st : 1=select, 2= chain (continue combo/multiple jump)
   if ((sel_st == 2) && (matrix[cx][cy]==Empty) && (last_move_type >= EatUp)) {
// do chain move
   xs = lx; ys = ly;
   xd = cx; yd = cy;

  if(verbose)textprintf(s, font,  BoardX, 52, LineColor, "Chained Move: (%d, %d) -> (%d, %d)", xs, ys, xd, yd);
  Toggle();
  DisplayStats();

  type = ValidateMove(xs,ys,xd,yd);

  suicide = SuicideCheck(xs,ys,xd,yd);
  if ((type != Imp) && (VerifyEat(xs,ys,type)!=ImpL)&&(suicide==FALSE)) {
  Move(xs,ys,xd,yd);

  done = TRUE;
  last_move_type = type;
  }
    else {

    if(verbose)textprintf(s, font,  16, 452, LineColor, "UnToggle Called due to invalid chain move attempt. ");
    if(verbose)textprintf(s, font,  BoardX, 462, LineColor, "Attempted Chained Move: (%d, %d) -> (%d, %d)", xs, ys, xd, yd);
    if(debug)dout("\nUnToggle Called due to invalid chain move attempt.");
    UnToggle();
    ShowSide();
            }
   } else   {
// do move for current side...

if(verbose) textprintf (s, font,  BoardX, 42, TestColor, "Move from:( %d x , %d y ) %d st", cx, cy, sel_st);
        UpdateScreen();

        xs = cx;
        ys = cy;

        if ((matrix[xs][ys]) == Empty ) sel_st=-3;
        if ((matrix[xs][ys]) != side ) sel_st=-4;

        if ( sel_st > 0) {
        sprintf (status, "Move Piece to where ?");

        sel_st = SelectCell(cx,cy);
if(verbose)textprintf (s, font,  BoardX, 42, TestColor, "Move to  :( %d x , %d y ) %d st", cx, cy, sel_st);
        UpdateScreen();
        xd = cx;
        yd = cy;
        dout("\nValidating normal move...");
        suicide = SuicideCheck(xs,ys,xd,yd);
        type = ValidateMove(xs,ys,xd,yd);
        last_move_type = type;
        if(verbose)textprintf (s, font,  BoardX, 0, LineColor, "cmt:%d",type);
        if ((type!=Imp)&& (suicide==FALSE)) {
            Combo = 0;
       Move(xs,ys,xd,yd);

       done = TRUE;
       dout("done validation.\n");
       }
        }
      }

} while ((AbortNow == FALSE) && (done == FALSE));

        return Winner;

}

move ValidateMove(int x, int y, int dx, int dy)
{
        cell start_cell, end_cell;
        int deltax, deltay;
        move movetype = Imp;

        start_cell = matrix[x][y];
        end_cell = matrix[dx][dy];

        deltax = dx -x;
        deltay = dy -y;

        if (deltax == 0) {
                switch(deltay)
                {
                case 1: movetype = MvDn; break;
                case -1: movetype = MvUp; break;
                case 2: movetype = EatDn; break;
                case -2: movetype = EatUp; break;
                default: movetype = Imp;
                }
       }

        if (deltay == 0) {
                switch(deltax)
                {
                case 1: movetype = MvRt; break;
                case -1: movetype = MvLf; break;
                case 2: movetype = EatRt; break;
                case -2: movetype = EatLf; break;
                default: movetype = Imp;
                }
        }

    if ( (movetype >= EatUp) && (VerifyEat(x, y, movetype) == ImpL) )movetype = Imp;

        if (movetype == Imp) {
   if(verbose) textprintf(s, font,  BoardX, 24, LineColor, "Invalid Move! (%d, %d) -> (%d, %d)  ", x, y, dx, dy);
        } else {
   if(verbose) textprintf(s, font,  BoardX, 24, LineColor, "Correct Move: (%d, %d) -> (%d, %d) %d", x, y, dx, dy, movetype);

        }

        if ( (x==dx) && (y==dy) ) movetype = Imp;  //redundant

        if (end_cell != Empty)  movetype = Imp;

        return movetype;
}


mvflag VerifyEat(int x,int y,move mvtyp)
{
   mvflag can_eat = ImpL;

   if (mvtyp >= 5) {
         cell eaten_cell = matrix[x+(xe[mvtyp])][y+(ye[mvtyp])];

      if ( (eaten_cell !=Empty) && (eaten_cell !=side) ) can_eat = Eat;

      if ( (eaten_cell !=Empty) && (eaten_cell ==side) ) can_eat = Mv;

   }

if (verbose) textprintf(s, font,  BoardX, 400, LineColor, "victim is @( %d , %d ) %d : %d", (x+xe[mvtyp]),(y+ye[mvtyp]), mvtyp, can_eat);

   return can_eat;
}


void DoEat(int x,int y,move mvtyp)
{
   int vx = (x+xe[mvtyp]);
   int vy = (y+ye[mvtyp]);
   cell vside;

   vside=matrix[vx][vy];

if(verbose) textprintf(s, font,  BoardX, 410, LineColor, "Last eat was ( %d , %d ) %d", vx,vy, mvtyp);

   matrix[vx][vy]=Empty;

   DrawSquare(vx,vy);

   Active[vside]--;

   if (IsInside(vx,vy)) Inside[vside]--;
   Combo++;
   if (Combo > MaxCombo) {
     MaxCombo=Combo; MaxComboSide=side;
     }

   UpdateScreen();

   if((Inside[other(side)]<=0)) Winner=side;

}


void DisplayStats()
{
  for(int count=1; count<=2;count++)
  {
    if(verbose) textprintf(s, font,  BoardX, 416+(10*count), LineColor, "Side %d has %d inside, %d active", count,Inside[count],Active[count]);
  }
  if (Combo > 1) {
    textprintf(s, font,  16, 24, LineColor, "%d Hit", Combo);
    textprintf(s, font,  16, 34, LineColor, " Combo");
  } else {
    rectfill(s, 16, 24, 16+(8*7), 43, 0); //clear space
  }

  ShowSide();
  UpdateScreen();
}

void ShowSide()
{
  int Tox = 16;
  int Toy = 72;

  int Tlx = Tox+SquareSz-1;
  int Tly = Toy+SquareSz-1;

  rect(s, Tox, Toy, Tlx, Tly, LineColor);

  rectfill(s, Tox+1, Toy+1, Tlx-1, Tly-1, BackColor[2]);

  PlotSide(Tox,Toy,side);

}

void Congrats(cell Winner)
{
  // Display congratulations message for the Winner
  int done = FALSE;

  if(debug) dout("\nYou win!\n");

  status_line = FALSE;
  clear(s); // abruptly clear the screen.
  textprintf (s, font,  BoardX, 272, LineColor, "Y O U   W I N ! ! !");
  PlotSide(BoardX+(SquareSz*2),192,Winner);

  if(MaxCombo >= 2) {
    textprintf (s, font,  BoardX, 302, LineColor, "Max. Combo : %d", MaxCombo);
    if(verbose)textprintf (s, font,  BoardX, 322, LineColor, "done by side : %d", MaxComboSide);
    PlotSide(BoardX+(SquareSz*4)+4,304,MaxComboSide); // don't ask, random formula.
  }

  if(Combo >= 2) {
      textprintf (s, font,  BoardX, 102, LineColor, "%d Hit Combo Finish", Combo);
      // PlotSide(BoardX+(SquareSz*4)+4,104,Winner);
  }

  rest(1000); //  give uses time to read the screen.
  //we could also play a sound or something...

  textprintf_centre (s, font,  319, 42, LineColor, "Press ESC to exit.");
  textprintf_centre (s, font,  319, 242, LineColor, "Press SPACE, ENTER, or a BUTTON to play again.");
  UpdateScreen();

  done = WaitOnInput();

  if (done)
    dout("\nScruffy Abort Requested!\n");

}

void PlotSide(int Tox,int Toy,cell side_to_plot)
{
   switch(side_to_plot)
   {
   case Empty: break;

   case Orange:{
      masked_blit(p1, s, 0, 0, Tox, Toy, SquareSz, SquareSz);
      break;
      }
   case Green: {
      masked_blit(p2, s, 0, 0, Tox, Toy, SquareSz, SquareSz);
      break;
      }
   }
   if(auto_blit)UpdateScreen();
}

void FirstMove()
{
  int sel_st, xd, yd, xs, ys;
  int done = FALSE;
  int suicide = FALSE;

  move type;

  do
  {
    sprintf(status, "Select Piece for first move.");
    ShowSide();
    UpdateScreen();

    sel_st = SelectCell(cx,cy);

    // do move for current side...
    if(verbose) textprintf (s, font,  BoardX, 42, TestColor, "Move from:( %d x , %d y ) %d st", cx, cy, sel_st);
    UpdateScreen();

    xs = cx;
    ys = cy;

    if ((matrix[xs][ys]) == Empty ) sel_st=-3;
    if ((matrix[xs][ys]) != side ) sel_st=-4;

    if (( sel_st > 0)&& (AbortNow == FALSE)) {
      sprintf(status,  "Move 1st Piece into Inner Court.");

      sel_st = SelectCell(cx,cy);
      if(verbose)textprintf (s, font,  BoardX, 42, TestColor, "Move to  :( %d x , %d y ) %d st", cx, cy, sel_st);
      UpdateScreen();
      xd = cx;
      yd = cy;
      dout("\nFirst move...\n");
      suicide = SuicideCheck(xs,ys,xd,yd);
      type = ValidateMove(xs,ys,xd,yd);

      if ((type!=Imp)&& (suicide==FALSE) &&(type < EatUp)) {
        Move(xs,ys,xd,yd);
        x1 = xs; y1 = ys; // save origin

        done = TRUE;
        dout("done validation.\n");
      }
      else {
        dout("Rejected first move.");
      }
    }
  } while ((AbortNow == FALSE) && (done == FALSE));

  Toggle();
}

void SecondMove()
{
  int sel_st, xd, yd, xs, ys;
  int done = FALSE;  int suicide = FALSE;

  move type;  int valid = FALSE;

  do  {
    sprintf(status, "Select Piece for second move.");
    ShowSide();
    UpdateScreen();

    sel_st = SelectCell(cx,cy);
    x2 = cx ; y2 = cy;
    valid = ValidateSecondOrigin(x1,y1,x2,y2); // make sure the rules r followed.
    if (!valid) sel_st = -5; // don't approve invalid player.

    if(verbose) textprintf (s, font,  BoardX, 42, TestColor, "Move from:( %d x , %d y ) %d st", cx, cy, sel_st);
    UpdateScreen();

    xs = cx;
    ys = cy;

    if ((matrix[xs][ys]) == Empty ) sel_st=-3;
    if ((matrix[xs][ys]) != side ) sel_st=-4;

    if (( sel_st > 0)&& (AbortNow == FALSE)) {
      sprintf(status,  "Move 2nd Piece into Inner Court.");

      sel_st = SelectCell(cx,cy);
      if(verbose)textprintf (s, font,  BoardX, 42, TestColor, "Move to  :( %d x , %d y ) %d st", cx, cy, sel_st);
      UpdateScreen();
      xd = cx;
      yd = cy;
      dout("\nSecond move...\n");
      suicide = SuicideCheck(xs,ys,xd,yd);
      type = ValidateMove(xs,ys,xd,yd);
      x2 = xs; y2 = ys; // second origin.
      valid = ValidateSecondOrigin(x1,y1,x2,y2); // make sure the rules r followed.

      if ((type!=Imp)&& (suicide==FALSE) &&(type < EatUp) && (valid=TRUE)) {
        Move(xs,ys,xd,yd);

        done = TRUE;
        dout("done validation.\n");
      }
      else {
        dout("Rejected second move.\n");
      }
    }
  } while ((AbortNow == FALSE) && (done == FALSE));

  Toggle();
}

int WaitOnInput()
{
	int done = FALSE;

	if (!disable_joystick) 
	  poll_joystick(); // unnecessary, but perhaps if one's using analog switches...

	do
	{
	  if (!disable_joystick) {
		  
		poll_joystick();
		if((joy_b1) || (joy_b2)) done = TRUE;
	  };

	  if(key[KEY_SPACE] || key[KEY_ENTER]) done = TRUE;
	  if(key[KEY_ESC])
		{ AbortNow = TRUE; done = TRUE; }
	} while (done == FALSE);

	return AbortNow;
}

int ValidateSecondOrigin(int x, int y, int dx, int dy)
{
  int validate = FALSE;

  if((x == 1) && (dx == 6) && (2<=dy) && (dy<=5)) validate = TRUE;
  if((x == 6) && (dx == 1) && (2<=dy) && (dy<=5)) validate = TRUE;
  if((y == 1) && (dy == 6) && (2<=dx) && (dx<=5)) validate = TRUE;
  if((y == 6) && (dy == 1) && (2<=dx) && (dx<=5)) validate = TRUE;
    // if xs or ys are opposites then it's correct.
  return validate;
}

void SaveScreen()
{
	BITMAP* sshot;
	PALETTE pal;

	sshot = create_bitmap(SCREEN_W, SCREEN_H);
	clear(sshot);
	draw_sprite(sshot, s, 0,0 );
	get_palette(pal);

	save_bitmap("dump.bmp", sshot, pal);
	// saving will fail silently if file exists

	// we save to the lowest available file number
	char fn[20] = "sshot0000.bmp";
	int i = 0; int irange = 10000;
	for (i = 0; i < irange ; i++)
	{
		sprintf(fn, "sshot%4d.bmp", i);
		if (!exists(fn)) {
			save_bitmap(fn, sshot, pal);
//			char sstr[100] = "[Screenshot Message!]";
		    sprintf(status, "Screenshot: %s!", fn);
			UpdateScreen();
			i = irange; break; // break for (lawyer needed!)
		}
	}

	destroy_bitmap(sshot);

	while (sshot_rq()) ;	{
		; // do nothing... waits until the key is released.
		//timeout should be here, though
	}
}// SaveScreen - dump a screenshot!

int sshot_rq()
{
	return ((key[KEY_PRTSCR]) || (key[KEY_P]));
}
