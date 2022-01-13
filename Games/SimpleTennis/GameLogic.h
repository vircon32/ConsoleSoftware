// ---------------------------------------------------------
//   GAME SESSION DATA
// ---------------------------------------------------------

// Scores
int scoreP1;
int scoreP2;
	
// Paddles
int paddle1X;
int paddle1Y;
	
int paddle2X;
int paddle2Y;
	
// Ball
int ballX;
int ballY;
int previousBallX;
int previousBallY;
int ballSpeed;
int goingLeft;
int currentAngle;

// -----------------------------------------------
// GET RANDOM ANGLE FUNCTION
// Ball starts at a random angle each time a goal
// is scored or at the beginning of the game
// -----------------------------------------------
int get_random_angle()
{
  // initial angles can vary from 10 to MaxAngle
  int angle = rand() % (MaxAngle - 10);
  int sign = rand() % (1);

  if (sign == 0)
    return angle + 10;
  else
    return 0 - (angle + 10);
}

// -----------------------------------------------
// INITIALIZE GAMEPLAY VALUES FUNCTION
// sets all gameplay values to start a game
// -----------------------------------------------
void initialize_gameplay_values()
{
  scoreP1 = 0;
  scoreP2 = 0;
	
  // paddles start at the center vertically
  // and at a set px distance from the edge of the screen
  paddle1X = PaddleDistance + (PaddleSizeX / 2);
  paddle1Y = screen_height / 2;
	
  paddle2X = screen_width - PaddleDistance - (PaddleSizeX / 2);
  paddle2Y = screen_height / 2;
	
  // ball starts at the center
  ballX = screen_width / 2;
  ballY = screen_height / 2;
  previousBallX = ballX;
  previousBallY = ballY;
	
  // ball starts with base speed
  ballSpeed = BallBaseSpeed;
  goingLeft = rand() % 2;

  currentAngle = get_random_angle();
}

// -----------------------------------------------
// MOVE PADDLE IA FUNCTION
// moves paddle according to IA difficulty
// -----------------------------------------------
void move_paddle_IA()
{
  // AI tries to match ball height
  // tolerance will differ with
  // difficulty levels
 
  if (ballY < paddle2Y) //ball above paddle
  {
    int moveUp = 0;
    if( currentAngle > 0 )
      moveUp = paddle2Y - ballY > PaddleSizeY / Difficulty;
    else 
      if( paddle2Y - ballY > Difficulty )
        moveUp = paddle2Y > screen_height / 5;
  	
    if( moveUp )
      paddle2Y -= PaddleSpeed;
  } 
  else if( ballY > paddle2Y ) //ball below paddle
  {
    int moveDown = 0;
    if( currentAngle < 0 )
      moveDown = ballY - paddle2Y > PaddleSizeY / Difficulty;
    else
      if( ballY - paddle2Y > Difficulty )
	      moveDown =  paddle2Y < (screen_height / 5) * 4;
	      
    if( moveDown )
      paddle2Y += PaddleSpeed;
  }
}

// -----------------------------------------------
// CHECK PADDLE COLLISIONS FUNCTION
// Checks if the paddle reached top/bottom walls
// and avoids going through those walls
// -----------------------------------------------
void check_paddle_collisions( int* positionY )
{
  if(*positionY >= screen_height - WallWidth - (PaddleSizeY / 2) )
    *positionY = screen_height - WallWidth - (PaddleSizeY / 2);
  else if( *positionY <= 0 + WallWidth + (PaddleSizeY / 2))
    *positionY = 0 + WallWidth + (PaddleSizeY / 2);
}

// -----------------------------------------------
// GET NEW ANGLE FUNCTION
// Function only called if ball hits a paddle
// and gives a new angle depending on the part
// of the paddle hitting the ball
// -----------------------------------------------
void get_new_angle ( int paddleY )
{
  // Depending on the position the ball hits the paddle
  // the ball will bounce in different angles
	
  // get the point where the ball hits the paddle
  int hitPoint = abs(ballY - paddleY);

  // get the furthest hit point to calculate the angle 
  // the ball will bounce
  int furthestHitPoint = (BallSize / 2) + (PaddleSizeY / 2) - 1;
	
  // if paddle is above ball, ball bounces downwards
  if( paddleY < ballY )
    currentAngle = 0 - ((hitPoint * 100 / furthestHitPoint) * MaxAngle ) / 100;
  else
    currentAngle = ((hitPoint * 100 / furthestHitPoint) * MaxAngle) / 100;	
}

// -----------------------------------------------
// INCREASE SPEED FUNCTION
// increases ball speed
// -----------------------------------------------
void increase_speed()
{
  if( ballSpeed < BallMaxSpeed )
 	ballSpeed += 2;
}

// -----------------------------------------------
// CHECK BALL WALL COLLISIONS FUNCTION
// detects if ball hits top/bottom walls
// -----------------------------------------------
void check_ball_wall_collisions()
{
  //Check if ball hits top or bottom walls
  //if so, then angle is reflected
  if( ballY >= screen_height - WallWidth - (BallSize / 2) )
  {
    ballY = screen_height - WallWidth - (BallSize / 2) - 1;
    currentAngle = abs( currentAngle);
    play_sound( SoundWallHit );
  } 
  else if( ballY <= 0 + WallWidth + (BallSize / 2) )
  {
    ballY = 0 + WallWidth + (BallSize / 2) + 1;
    currentAngle = 0 - currentAngle;
    play_sound( SoundWallHit );
  }  
}

// -----------------------------------------------
// CHECK BALL PADDLE TOP BOTTOM COLLISIONS FUNCTION
// detects if ball hits top or bottom sides of paddle
// returns 0 if no paddle was hit
// returns 1 if ball hits paddle
// -----------------------------------------------
int check_ball_paddle_top_bottom_collisions(int paddleX, int paddleY)
{
  // if ball hits after middle part of paddle
  // ball is not returned and only ball has 
  // angle reflected like hitting walls

   //check if ball hits paddle edges
  if( ballX - BallSize / 2 < paddleX + PaddleSizeX / 2 && ballX + BallSize / 2 > paddleX - PaddleSizeX / 2)
  {
	  int hit = 0;
	  //ball hits paddle bottom or top edges
    int topEdgeHit = ((ballY - BallSize / 2) - (paddleY + PaddleSizeY / 2) <= 0 && (previousBallY - BallSize / 2) - (paddleY + PaddleSizeY / 2) > 0);
    int bottomEdgeHit = ((paddleY - PaddleSizeY / 2) - (ballY + BallSize / 2) <= 0 && (paddleY + PaddleSizeY / 2) - (previousBallY + BallSize / 2) > 0);
	  if( topEdgeHit || bottomEdgeHit )
	  {
      if( ballY > paddleY )
	      currentAngle = MinAngle;
      else
        currentAngle = MaxAngle;	

	    //check if paddle is 1 or 2
	    if (goingLeft)
	    {
		    if( ballX > paddleX ) // ball is returned if hit before ball reaches the middle of paddle X size
		    hit = 1;
		  else
		    play_sound( SoundPaddleHit );
	    }
	    else
	    {
		    if( ballX < paddleX ) // ball is returned if hit before ball reaches the middle of paddle X size
		      hit = 1;
		    else
		      play_sound( SoundPaddleHit );
	    }
	  }  
	
	  if ( hit )
	  {
	    goingLeft = !goingLeft;
	    play_sound( SoundPaddleHit);
	    increase_speed();
	    return 1;
	  }
  }
  return 0;
}

// -----------------------------------------------
// CHECK BALL PADDLE COLLISIONS FUNCTION
// detects if ball hits paddle
// returns 0 if no paddle was hit
// returns 1 if ball hits any paddle
// -----------------------------------------------
int check_ball_paddle_collisions(int paddleXHitDetection, int ballXHitDetection, int previousBallXHitDetection, int paddleX, int paddleY)
{
  //Check if ball hits paddle
  if( ((ballXHitDetection <= paddleXHitDetection) && (previousBallXHitDetection >= paddleXHitDetection)) || ((ballXHitDetection >= paddleXHitDetection) && (previousBallXHitDetection <= paddleXHitDetection)))
  {
    // Compare if paddle position
    // is between ball position
    if( (paddleY - (PaddleSizeY / 2) <= (ballY + (BallSize / 2)) ) && (paddleY + (PaddleSizeY / 2) >= (ballY - (BallSize / 2)) ) )
    {
      increase_speed();
	    if( goingLeft )
	      ballX = paddleXHitDetection + (BallSize / 2);
	    else
  	    ballX = paddleXHitDetection - (BallSize / 2);
      goingLeft = !goingLeft;
      get_new_angle( paddleY );
      play_sound( SoundPaddleHit );
      return 1;
    }
  }
	  
  if (check_ball_paddle_top_bottom_collisions(paddleX, paddleY) == 1)
    return 1;
	
  return 0;
}

// -----------------------------------------------
// CHECK GOALS FUNCTION
// detects if ball hits left or right screen edges
// -----------------------------------------------
void check_goals()
{
  if( (ballX >= screen_width + (BallSize / 2)) && !goingLeft )
  {
    ballX = screen_width / 2;
    ballY = screen_height / 2;
    ballSpeed = BallBaseSpeed;
    currentAngle = get_random_angle();
    goingLeft = 1;
    //Score can't go beyond 9
    if( scoreP1 < MaxScore )
      ++scoreP1;
    play_sound( SoundGoalScored );
    TimeAfterLastPoint = 0;
    GameState = Gameplay_Start_Point;
  }  
  else if( (ballX <= 0 - (BallSize / 2)) && goingLeft)
  {  
    ballX = screen_width / 2;
    ballY = screen_height / 2;
    ballSpeed = BallBaseSpeed;
    currentAngle = get_random_angle();
    goingLeft = 0;
    //Score can't go beyond 9
    if( scoreP2 < MaxScore )
      ++scoreP2;
    play_sound( SoundGoalScored );
    TimeAfterLastPoint = 0;
    GameState = Gameplay_Start_Point;
  }
}

// -----------------------------------------------
// CHECK BALL COLLISIONS FUNCTION
// Check every possible ball collision
// -----------------------------------------------
void check_ball_collisions()
{
  check_ball_wall_collisions();

  //only check goals if paddle wasn't hit
  if( goingLeft )
  {
	  int paddleXHitDetection = paddle1X + PaddleSizeX / 2;
	  int ballXHitDetection = ballX - BallSize / 2;
	  int previousBallXHitDetection = previousBallX - BallSize / 2;
	  if (!check_ball_paddle_collisions(paddleXHitDetection, ballXHitDetection, previousBallXHitDetection, paddle1X, paddle1Y))
      check_goals();
  }  
  else
  {
	  int paddleXHitDetection = paddle2X - PaddleSizeX / 2;
	  int ballXHitDetection = ballX + BallSize / 2;
	  int previousBallXHitDetection = previousBallX + BallSize / 2;
	  if (!check_ball_paddle_collisions(paddleXHitDetection, ballXHitDetection, previousBallXHitDetection, paddle2X, paddle2Y))
      check_goals();
  }
}


// -----------------------------------------------
// DRAW SCORE FUNCTION
// draws the ball in the next position
// depending on speed and angle
// -----------------------------------------------
void move_ball()
{
  previousBallX = ballX;
  previousBallY = ballY;
  int XMovement;
  int YMovement;
	
  // Get movement for each axis
  if( currentAngle == 0)
  {
    XMovement = ballSpeed;
    YMovement = 0;
  }  
  else
  {
    XMovement = ballSpeed * cos( ( abs( currentAngle ) * pi) / 180);
    YMovement = ballSpeed * sin( ( abs( currentAngle ) * pi) / 180);
  }
	
  // Apply movement in each axis
	
  // Y axis
  if( currentAngle > 0 )
    ballY = ballY - YMovement;
  else if( currentAngle < 0 )
    ballY = ballY + YMovement;
	
  // X axis
  if( goingLeft == 1 )
    ballX -= XMovement;
  else
    ballX += XMovement;
}

// -----------------------------------------------
// DRAW SCORE FUNCTION
// prints the score for each player
// -----------------------------------------------
void draw_score()
{
  //score for player 1
  select_region( RegionScore0 + scoreP1 );
  draw_region_at( (screen_width / 2) - ScoreDistanceToCenter - ScoreWidth, ScoreDistanceToTop );
	
  //score for player 2
  select_region( RegionScore0 + scoreP2 );
  draw_region_at( (screen_width / 2) + ScoreDistanceToCenter, ScoreDistanceToTop );
}

// -----------------------------------------------
// DRAW ANGLE FUNCTION
// only for testing, prints on the upper left corner
// the abs value of the angle the ball is moving
// -----------------------------------------------
void draw_angle()
{
  //first number
  select_region( RegionScore0 + abs(currentAngle / 10) );
  draw_region_at( ScoreDistanceToCenter + ScoreWidth, ScoreDistanceToTop );
	
  //second number
  select_region( RegionScore0 + abs(currentAngle % 10) );
  draw_region_at( ScoreDistanceToCenter + ScoreWidth + ScoreWidth + 5, ScoreDistanceToTop );
}