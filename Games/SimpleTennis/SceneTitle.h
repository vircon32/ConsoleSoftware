
int PreviousXDirection;
int PreviousYDirection;

// initialize all menu variables
// only once after starting the rom
void Title_Initialize()
{
  initialize_gameplay_values();
  PreviousXDirection = 0;
  PreviousYDirection = 0;
  StartGameText = "START GAME";
  OpponentText = "VS CPU";
  Difficulty = Normal;
  DifficultyText = "NORMAL";
  ScoreToWinText = "7";
  CursorLine = Menu_Start; 
  Opponent = Menu_VS_AI;
  SelectedDifficulty = Menu_Difficulty_Normal;
  ScoreToWin = Menu_Score_7;    
  GameState = Title_Menu;
}

// function to avoid starting a new game
// after finishing and pressing start
void Title_Wait()
{
  if (TimeGameFinished / 60 > SecondsUntilNewGame)
    GameState = Title_Menu;
  else
    ++TimeGameFinished;
}

// function to show the correct text for score to win
void ScoreToText()
{
  if (ScoreToWin == 5)
    ScoreToWinText = "5";
  else if (ScoreToWin == 6)
    ScoreToWinText = "6";
  else if (ScoreToWin == 7)
    ScoreToWinText = "7";
  else if (ScoreToWin == 8)
    ScoreToWinText = "8";
  else if (ScoreToWin == 9)
    ScoreToWinText = "9";
}

// menu navigation
void check_menu_movement()
{
  //only player 1 will control the menu
  int directionX, directionY;
  select_gamepad( 0 ); 
	gamepad_direction( &directionX, &directionY );
  int playMenuSound = 0;
  //navigate through menu options
  if (directionY == 1 && PreviousYDirection == 0)
  {
    if (CursorLine != Menu_Start)
    {
      CursorLine += 1;
      playMenuSound = 1;    
    }
  }
  else if (directionY == -1 && PreviousYDirection == 0)
  {
    if (CursorLine != Menu_Opponent)
    {
      CursorLine -= 1;
      playMenuSound = 1;
    } 
  }  

  //navigate through selected option
  if (directionX != 0 && PreviousXDirection == 0)
    if (CursorLine == Menu_Start)
      return;
    else
    {
      // Opponent
      if (CursorLine == Menu_Opponent)
        if (directionX == -1)
        {
          Opponent = Menu_VS_AI;
          OpponentText = "VS CPU";
          playMenuSound = 1; 
        }
        else if (directionX == 1)
        {
          Opponent = Menu_VS_P2;
          OpponentText = "VS P2";
          playMenuSound = 1; 
        }

      // Difficulty
      if (CursorLine == Menu_AIDifficulty)
        if (directionX == -1)
        {
          if (SelectedDifficulty != Menu_Difficulty_Easy)
          {
            SelectedDifficulty -= 1;
            playMenuSound = 1; 
          }  
        }
        else if (directionX == 1)
          if (SelectedDifficulty != Menu_Difficulty_Hard)
          {
            SelectedDifficulty += 1;
            playMenuSound = 1; 
          } 

      if (SelectedDifficulty == Menu_Difficulty_Easy)
      {
        DifficultyText = "EASY";
        Difficulty = Easy;
        playMenuSound = 1; 
      }  
      else if (SelectedDifficulty == Menu_Difficulty_Normal)
      {
        DifficultyText = "NORMAL";
        Difficulty = Normal;
        playMenuSound = 1; 
      }  
      else if (SelectedDifficulty == Menu_Difficulty_Hard)
      {
        DifficultyText = "HARD";
        Difficulty = Hard;
        playMenuSound = 1; 
      }  

      // Goals to win
      if (CursorLine == Menu_Score)
        if (directionX == -1)
        {
          if (ScoreToWin != Menu_Score_5)
          {
            ScoreToWin -= 1;
            ScoreToText();
            playMenuSound = 1; 
          }  
        }
        else if (directionX == 1)
          if (ScoreToWin != Menu_Score_9)
          {
            ScoreToWin += 1;
            ScoreToText();
            playMenuSound = 1; 
          }
    }

  if( playMenuSound )
    play_sound( SoundMenu );
  // to avoid going too fast through options
  // players are forced to return to neutral
  // their directions before advancing
  PreviousXDirection = directionX;
  PreviousYDirection = directionY;
}

// draw logo
void draw_logo()
{
  select_texture( TextureSimpleTennis );
  select_region( RegionLogo );
  draw_region_at( screen_width / 2, LogoYPlace );
}

// show the menu
void draw_menu()
{
  //number of players
  DrawText( Menu_Opponent_Option_X, Menu_Opponent_Y, "SELECT OPPONENT" );
  DrawText( Menu_Opponent_Select_X, Menu_Opponent_Y, OpponentText );

  //difficulty
  DrawText( Menu_AIDifficulty_Option_X, Menu_AIDifficulty_Y, "AI DIFFICULTY" );
  DrawText( Menu_AIDifficulty_Select_X, Menu_AIDifficulty_Y, DifficultyText );

  //score to win
  DrawText( Menu_Score_Option_X, Menu_Score_Y, "SCORE TO WIN" );
  DrawText( Menu_Score_Select_X, Menu_Score_Y, ScoreToWinText );

  //play button
  DrawText( Menu_Start_X, Menu_Start_Y, StartGameText );
}

// draw a rectangle around selected option
void draw_selected_menu_option(int* text, int displayX, int displayY)
{
  //get text length to draw selected option textures
  int TextLength = strlen(text);

  // draw left side
  select_region( RegionMenuSelectionLeft );
  draw_region_at( displayX - SelectionSideWidth + 1, displayY - 8 );

  // draw middle
  select_region( RegionMenuSelectionMiddle );
  for(int i=0; i<TextLength; i++)
    draw_region_at( displayX - SelectionSideWidth + SelectionSideWidth + (i * SelectionMiddleWidth) + 6 , displayY - 8 );

  // draw right side
  select_region( RegionMenuSelectionRight );
  draw_region_at( displayX - SelectionSideWidth + (TextLength * SelectionMiddleWidth) + 4, displayY - 8 );
}

// checks if player starts a new game
int game_start()
{
  select_gamepad( 0 );
  if (CursorLine == Menu_Start && gamepad_button_start() == 1)
    return 1;
  return 0;
}

void Title_Run()
{
  //black background
  clear_screen( make_color_rgb(0,0,0) );
  draw_logo();
  draw_menu();

  check_menu_movement();

  if (game_start())
  {
    Players = Opponent;
    GameScene = Scene_GamePlay;
    GameState = Gameplay_Initialize;
    return;
  }


  if (CursorLine == Menu_Opponent)
    draw_selected_menu_option(OpponentText, Menu_Opponent_Select_X, Menu_Opponent_Y );
  else if (CursorLine == Menu_AIDifficulty)
    draw_selected_menu_option(DifficultyText, Menu_AIDifficulty_Select_X, Menu_AIDifficulty_Y );
  else if (CursorLine == Menu_Score)
    draw_selected_menu_option(ScoreToWinText, Menu_Score_Select_X, Menu_Score_Y );
  else if (CursorLine == Menu_Start)
    draw_selected_menu_option(StartGameText, Menu_Start_X, Menu_Start_Y );
  
}

void Title_RunStateMachine()
{
  if( GameState == Title_Start )
    Title_Initialize();
  if( GameState == Title_Wait_New_Game )
    Title_Wait();
  else
    Title_Run();
}