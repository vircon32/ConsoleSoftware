void Gameplay_RunState_Initialize()
{
  // set up all values to start a new game
  initialize_gameplay_values();
  TimeAfterLastPoint = 0;
  GameState = Gameplay_Start_Point;
}

void draw_field()
{
  select_texture( TextureSimpleTennis );
  // clear screen
  clear_screen( make_color_rgb(20,19,85) );
	
  // draw both goal lines
  select_region( RegionGoalLine );
  draw_region_at( GoalLineDistance, 0 );
	draw_region_at( screen_width - GoalLineWidth - GoalLineDistance, 0 );
		
  // draw middle line
  select_region( RegionMiddleLine );
  draw_region_at( screen_width / 2 - MiddleLineWidth / 2, 0 );

}

void check_players_movement()
{
  // Check players controllers
	// paddle will only move in Y axis
	// we will ignore directionX
		
	int directionX, directionY;

  // player 1 is human
  select_gamepad( 0 ); 
	gamepad_direction( &directionX, &directionY );
	paddle1Y += PaddleSpeed * directionY;
  if (Players ==  2)
  {
    // player 2 is human
  	select_gamepad( 1 ); 
	  gamepad_direction( &directionX, &directionY );
	  paddle2Y += PaddleSpeed * directionY;
  }
  else // player 2 is AI
    move_paddle_IA();

  //check if paddle would collide with walls
  check_paddle_collisions( &paddle1Y ); 
  check_paddle_collisions( &paddle2Y ); 
}

void draw_paddles()
{
  select_texture( TextureSimpleTennis );
  select_region( Regionpaddle );
  draw_region_at( paddle1X, paddle1Y );
	draw_region_at( paddle2X, paddle2Y );
}

void draw_ball()
{
  select_texture( TextureSimpleTennis );
  select_region( RegionBall );
  draw_region_at( ballX, ballY );
}

void draw_arrow()
{
  select_texture( TextureSimpleTennis );
  select_region( RegionArrow );
  if( goingLeft )
  {
    set_drawing_angle( pi ); // pi = 180º
    draw_region_rotated_at( screen_width / 2 - 50, screen_height / 2 );
  } 
  else
    draw_region_at( screen_width / 2 + 50, screen_height / 2 );
}

void check_pause()
{
  // check pause button for each player
  for (int i = 0; i < Players; i++)
  {
    select_gamepad( i ); 
    if( gamepad_button_start() == 1 )
    {
      PlayerPaused = i;
      GameState = Gameplay_Pause;
    }
  }
}

void check_score()
{
  if (scoreP1 == ScoreToWin || scoreP2 == ScoreToWin)
  {
    GameState = Gameplay_Game_Finished;
    play_sound( SoundGameEnd );
    return;
  }
}

void Gameplay_New_Point()
{
  // draw background and field lines
  draw_field();

  // move the paddles
	check_players_movement();
  draw_paddles();
		
	//
  draw_ball();

	// draw score above all
	draw_score();
  if( (TimeAfterLastPoint / 15) % 2 )
    draw_arrow();

  if (TimeAfterLastPoint / 60 > SecondsBeforeNewPoint)
    GameState = Gameplay_Playing; 
  else
    ++TimeAfterLastPoint;
  
}

void Gameplay_Finished()
{
  // Black background
  clear_screen( make_color_rgb(0,0,0) );

  if (scoreP1 == ScoreToWin)
    DrawText( screen_width/3 + 25, screen_height/2 - 20, "PLAYER 1 WINS!" );
  else
    if (Players == 2)
      DrawText( screen_width/3 + 25, screen_height/2 - 20, "PLAYER 2 WINS!" );
    else
      DrawText( screen_width/3 + 50, screen_height/2 - 20, "CPU WINS!" );
  
  DrawText( screen_width/5 + 25, screen_height/2 + 20, "PRESS START TO RETURN TO MENU" );

  // check pause button for each player
  for (int i = 0; i < Players; i++)
  {
    select_gamepad( i ); 
    if( gamepad_button_start() == 1 )
    {
      TimeGameFinished = 0;
      GameScene = Scene_Title;
      GameState = Title_Wait_New_Game;
    }
  }
}

void Gameplay_Paused()
{
  // draw all screen elements
  draw_field();
  draw_paddles();
  draw_ball();
  draw_score();

  // darken the screen and write a pause text
  clear_screen( make_color_rgba(0,0,0,190) );
  
  if (PlayerPaused == 0)
    DrawText( screen_width/4 + 25, screen_height/2, "GAME PAUSED BY PLAYER 1" );
  else
    DrawText( screen_width/4 + 25, screen_height/2, "GAME PAUSED BY PLAYER 2" );

  // The player who paused
  // needs to press start button
  // to resume the game
  select_gamepad ( PlayerPaused );
  if( gamepad_button_start() == 1 )
    GameState = Gameplay_Playing;
		
}

void Gameplay_Run()
{
  // draw background and field lines
  draw_field();

  // move the paddles
	check_players_movement();
  draw_paddles();
		
	// move the ball
	move_ball( );
	check_ball_collisions(); 
  draw_ball();

	// draw score above all
	draw_score();

  // check if any player paused the game
  check_pause();

  // check if any player won the game
  check_score();
  		
	// function to test the angles functions
	//draw_angle();
					        
}
        
void Gameplay_RunStateMachine()
{
  if( GameState == Gameplay_Initialize )
    Gameplay_RunState_Initialize();
  else if( GameState == Gameplay_Start_Point )
    Gameplay_New_Point();
  else if( GameState == Gameplay_Pause )
    Gameplay_Paused();
  else if( GameState == Gameplay_Game_Finished )
    Gameplay_Finished();
  else
    Gameplay_Run();
}