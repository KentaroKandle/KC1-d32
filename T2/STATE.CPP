/* state.cpp -- Ability to save and reload the current game's "state"
** similar to the snapshot or savestate for emulators and whatnot, 
** as opposed to a savefile or a record of the game (as in KCSN notation,
** similar to a Chess game's record). */


#include "state.hpp"

/*
struct SaveState {
	string fn = "snapfile.txt";
	
	int open(
		//string fn="snap0001.txt"
		);
	int close()	; 
	
	int dump();
	
	// private 
	
	int record_side();
	int record_turn();
	int record_state();
	int record_go(); // typically used, game still going.
	int record_end(); // only used if in end-state
	
};
*/

static ofstream* output_file = null;

int SaveState::open() {
	int nErr = 0;
	ofstream out(fn);
	
	// check for errors TODO 
	
	
	
	out << "REM save state, KCSN v0.0.0, blah blah," << endl
	<< "REM fuck C++ the Advanced Asshole of Asuran contra-human conspiracy...."
	<< endl;
	
	output_file = out;
	
	if out.bad() 
		nErr++;
	
	return nErr;
}

int SaveState::close() {
	output_file->close();
	return 0; // TODO: check .bad() and stuff
} 


