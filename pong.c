#include <SDL2/SDL.h>
#include <stdio.h>

#define SCALE 1.5

#define WINDOW_WIDTH 800 * SCALE
#define WINDOW_HEIGHT 600 * SCALE

#define PADDLE_WIDTH 20 * SCALE
#define PADDLE_HEIGHT 100 * SCALE

#define PADDLE_SPEED 10

#define BALL_SIZE 15 * SCALE

#define BALL_SPEED_X 7
#define BALL_SPEED_Y 7

// Step 6: Define the paddle struct 
typedef struct {
	SDL_Rect rect; // SDL_Rect {int x, int y, int w, int h} - holds info about rect (paddle)
	int velY; // Vertical velocity to move paddles
} Paddle;

// Step 12: Define struct for ball
typedef struct {
	SDL_Rect rect;
	int velX, velY; //Velocity for horizontal and vertical movement
} Ball;

// Step 9: move paddles
void move_paddle(Paddle* paddle){
	paddle->rect.y += paddle->velY;
	// Step 11: Prevent the paddle from going off the screen
	if(paddle->rect.y < 0) paddle->rect.y = 0; // top limit
	if(paddle->rect.y > WINDOW_HEIGHT - PADDLE_HEIGHT) paddle->rect.y = WINDOW_HEIGHT - PADDLE_HEIGHT; // bottom limit
}

// Step 15: Move ball
void move_ball(Ball* ball){
	ball->rect.x += ball->velX;
	ball->rect.y += ball->velY;

	//Ball collision with top and bottom walls
	if(ball->rect.y <= 0 || ball->rect.y >= WINDOW_HEIGHT-BALL_SIZE) {
		ball->velY = -ball->velY; //invert vertical velosity
	}
	////right and left collision 
	//if(ball->rect.x <=0 || ball->rect.x >= WINDOW_WIDTH-BALL_SIZE){
	//	ball->velX = -ball->velX; //invert
	//}
}

// Step 16: Ball-paddle collision
void ball_paddle_collision(Ball* ball, Paddle* leftPaddle, Paddle* rightPaddle){
	if(SDL_HasIntersection(&ball->rect, &leftPaddle->rect) || SDL_HasIntersection(&ball->rect, &rightPaddle->rect)) {
		ball->velX = -ball->velX; //invert horizontal velosity
	}
}

// Step 17: Reset the ball position
void reset_ball_position(Ball* ball){
	ball->rect.x = WINDOW_WIDTH/2 - BALL_SIZE/2;
	ball->rect.y = WINDOW_HEIGHT/2 - BALL_SIZE/2;
	
	// Randomize ball direction
    ball->velX = (rand() % 2 == 0 ? BALL_SPEED_X : -BALL_SPEED_X);
    ball->velY = (rand() % 2 == 0 ? BALL_SPEED_Y : -BALL_SPEED_Y);
}

// Step 10: Handle key events
void keyHandler(Paddle* leftPaddle, Paddle* rightPaddle) {
	// Get the current keyboard state
	const Uint8 *keyState = SDL_GetKeyboardState(NULL);
	
	// Stop movement if no key is pressed
	leftPaddle->velY = 0;  
    rightPaddle->velY = 0;
	
	// Player 1 movement (W and S keys)
	if (keyState[SDL_SCANCODE_W]) leftPaddle->velY = -PADDLE_SPEED;  // Move up
	if (keyState[SDL_SCANCODE_S]) leftPaddle->velY = PADDLE_SPEED;   // Move down
	
	// Player 2 movement (Up and Down arrow keys)
	if (keyState[SDL_SCANCODE_UP]) rightPaddle->velY = -PADDLE_SPEED; // Move up
	if (keyState[SDL_SCANCODE_DOWN]) rightPaddle->velY = PADDLE_SPEED;   // Move down
}

int main(){
	//Step 1: Initialize SDL library for video output
	SDL_Init(SDL_INIT_VIDEO);

	//Step 2: Create the game window
	// SDL_Window * SDL_CreateWindow(const char *title, //title
        //                     	         int x, int y,      //position
	//                               int w, int h,      //widht and height of the window
	//                               Uint32 flags);     //keyword
	SDL_Window* window = SDL_CreateWindow("Pong name",
					       SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
					       WINDOW_WIDTH, WINDOW_HEIGHT, 
                                               0);
	
	// Step 3: Create a rendering context that allow to draw things inside the window
	// SDL_Renderer * SDL_CreateRenderer(SDL_Window * window, int index, Uint32 flags);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	// Step 7: Initialize the paddles
	Paddle leftPaddle = {{20, WINDOW_HEIGHT/2 - PADDLE_HEIGHT/2,
			     PADDLE_WIDTH, PADDLE_HEIGHT}, // SDL_Rect rect 
			     0}; // int velY
	Paddle rightPaddle = {{WINDOW_WIDTH-20-PADDLE_WIDTH, WINDOW_HEIGHT/2 - PADDLE_HEIGHT/2,
			       PADDLE_WIDTH, PADDLE_HEIGHT}, 
			      0};

	// Step 13: Initialize ball
	Ball ball = {{WINDOW_WIDTH/2-BALL_SIZE/2, WINDOW_HEIGHT/2-BALL_SIZE/2,
		      BALL_SIZE, BALL_SIZE},
		     BALL_SPEED_X, BALL_SPEED_Y};
	
	// Step 4: Create a game loop
	int running = 1;
	SDL_Event event;
	while (running) {
		// Event handling
		while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = 0; //check window close event
        }
				
		// keyHandler
		keyHandler(&leftPaddle, &rightPaddle);
		
		// movePaddle
		move_paddle(&leftPaddle);
		move_paddle(&rightPaddle);
			
		// Step 15.1: move ball
		move_ball(&ball);		
		
		// Step 16.1: ball-paddle collision
		ball_paddle_collision(&ball, &leftPaddle, &rightPaddle);

		// Step 17.1: Reset ball position
		if(ball.rect.x <= 0 || ball.rect.x >= WINDOW_WIDTH - BALL_SIZE) {
			reset_ball_position(&ball);
		}

		// Clear the screen with a black color
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // set color to black
		SDL_RenderClear(renderer); //clear the screen
		
		// Step 8: Draw the paddles
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // set color to white
		SDL_RenderFillRect(renderer, &leftPaddle.rect); // draw the left paddle
		SDL_RenderFillRect(renderer, &rightPaddle.rect); // draw the right paddle

		// Step 14: Draw ball
		SDL_RenderFillRect(renderer, &ball.rect);
		
		// Update the screen
		SDL_RenderPresent(renderer);

		SDL_Delay(4); // Control the frame rate
	}

	// Step 5: Clean up resources before exiting
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
