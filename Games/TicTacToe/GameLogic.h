int NotTieLineCheck(int a, int b, int c) {
    //  Returns 1 if the line is not tied (there is a win condition for this line)

    int ThereIsX    = 0;
    int ThereIsO    = 0;

    for (int i = a; i<b; i=i+c) {
        if (Tile[i]==1) ThereIsX = 1;
        if (Tile[i]==2) ThereIsO = 1;
    }
    if ( ThereIsX & ThereIsO ) return 0; else return 1;

}

int CheckWinCondition(){
    // returns:
    //   0 -> if the game must continue
    //   1 -> if the P1 wins
    //   2 -> if the P2 wins
    //   3 -> tie game

    // Check if someone won
    for (int i = 1; i<3; i++) {
        if ( Tile[0]==i & Tile[4]==i & Tile[8]==i) return i;
        if ( Tile[2]==i & Tile[4]==i & Tile[6]==i) return i;
        if ( Tile[0]==i & Tile[1]==i & Tile[2]==i) return i;
        if ( Tile[3]==i & Tile[4]==i & Tile[5]==i) return i;
        if ( Tile[6]==i & Tile[7]==i & Tile[8]==i) return i;
        if ( Tile[0]==i & Tile[3]==i & Tile[6]==i) return i;
        if ( Tile[1]==i & Tile[4]==i & Tile[7]==i) return i;
        if ( Tile[2]==i & Tile[5]==i & Tile[8]==i) return i;
    }

    // Checking Tie Game    
    if ( NotTieLineCheck(0,3,1) ) return 0;
    if ( NotTieLineCheck(3,6,1) ) return 0;
    if ( NotTieLineCheck(6,9,1) ) return 0;
    if ( NotTieLineCheck(0,7,3) ) return 0;
    if ( NotTieLineCheck(1,8,3) ) return 0;
    if ( NotTieLineCheck(2,9,3) ) return 0;
    if ( NotTieLineCheck(0,9,4) ) return 0;
    if ( NotTieLineCheck(2,7,2) ) return 0;
    
    // The game is a Tie
    return 3;
}