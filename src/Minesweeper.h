#ifndef MINESWEEPER_H_INCLUDED
#define MINESWEEPER_H_INCLUDED

#include <SDL2/SDL.h>
#include <stdlib.h>
#include <sstream>
#include "Const.h"
#include "Grid.h"
#include "GameState.h"
#include "Button.h"

class Minesweeper : public GameState
{
    public:

    int row;
    int col;

    int mines;
    int flags;
    int unexploredTiles;
    int menu_bar_width;

    int **fieldVisible;
    int **fieldHidden;

    bool sweepEnable;
    bool flagEnable;
    bool pressed;
    bool inPlay;
    bool gameOver;

    SDL_Color textColor;

    Uint32 startTime;

    LButton resetButton;
    LButton newButton;

    LTexture gSpriteSheetTexture;
    LTexture ResetSpriteSheet;
    LTexture NewSpriteSheet;
    LTexture SmileySpriteSheet;

    LTexture timeTextTexture;
    std::stringstream timeText;

    LTexture flagTextTexture;
    std::stringstream flagText;

    Grid *playField;

    SDL_Rect gSpriteClips[ SPRITE_NUM ];
    SDL_Rect ResetSpriteClips[ BUTTON_SPRITE_NUM ];
    SDL_Rect NewSpriteClips[ BUTTON_SPRITE_NUM ];
    SDL_Rect SmileySpriteClips[ BUTTON_SPRITE_NUM ];

    int smileyMood;

    enum{SMILE,SAD,SURPRISE,WIN};

    ///Constructor Function
    Minesweeper(int w, int h, int m){

        //Load media
        if( !loadMedia() )
        {
            printf( "Failed to load media!\n" );
        }
        else
        {
            //Initialize playing field dimensions, difficulty, and appearance
            col = w;//GRID_WIDTH;
            row = h;//GRID_HEIGHT;
            mines = m;//TOTAL_MINES;
            startTime = 0;
            textColor = { 255, 0, 0 };

            playField = new Grid(0, MENU_BAR_HEIGHT, col, row);

            menu_bar_width = col * TILE_WIDTH;

            resetButton = LButton(menu_bar_width/2 + TILE_WIDTH, MENU_BAR_HEIGHT/4, TILE_WIDTH, TILE_HEIGHT);
            newButton = LButton(menu_bar_width/2 - (TILE_WIDTH*2), MENU_BAR_HEIGHT/4, TILE_WIDTH, TILE_HEIGHT);
            

            //Initialize visible playing field
            fieldVisible = new int *[col];
            for (int i = 0; i < col; i++)
                fieldVisible[i] = new int[row];

            //Initialize hidden playing field
            fieldHidden = new int *[col];
            for (int i = 0; i < col; i++)
                fieldHidden[i] = new int[row];

            newGame();

            //Initialize and display graphical interface
            SDL_SetWindowSize(gWindow,(col * TILE_WIDTH),(row * TILE_HEIGHT) + (TILE_HEIGHT * 2) );
            update();
        }

    }

    ///Deconstructor
    ~Minesweeper(){
        printf("Minesweeper Object Deconstructing...\n");

        for(int i = 0; i < col; ++i) {
            delete [] fieldVisible[i];
        }
        delete [] fieldVisible;

        for(int i = 0; i < col; ++i) {
            delete [] fieldHidden[i];
        }
        delete [] fieldHidden;

        delete playField;

        //Free loaded image
        gSpriteSheetTexture.free();
        ResetSpriteSheet.free();
        NewSpriteSheet.free();
        timeTextTexture.free();
        flagTextTexture.free();

    }

    void newGame()
    {
        //Hide all visible tiles
        for (int i = 0; i < col; i++) {
            for (int j = 0; j < row; j++)
                fieldVisible[i][j] = UNKNOWN;
        }

        //Clear playfield
        for (int i = 0; i < col; i++) {
            for (int j = 0; j < row; j++)
                fieldHidden[i][j] = EMPTY;
        }

        //Hide mines at random locations
        int n = 0;
        while (n < mines) {
            int i = rand() % col;
            int j = rand() % row;
            if (fieldHidden[i][j] != MINE) {
                fieldHidden[i][j] = MINE;
                n++;
            }
        }

        //Generate clues
        for (int i = 0; i < col; i++){
            for (int j = 0; j < row; j++){
                if (fieldHidden[i][j] != MINE)
                    fieldHidden[i][j] = setClue(i,j);
            }
        }

        //Initialize unexplored tile counter and event flags
        unexploredTiles = row * col;
        flags = mines;
        sweepEnable = false;
        flagEnable = false;
        inPlay = false;
        gameOver = false;
        startTime = SDL_GetTicks();
        smileyMood = SMILE;

        //Set text to be rendered
        flagText.str( "" );
        flagText << flags;

        //Render text
        if( !flagTextTexture.loadFromRenderedText( flagText.str().c_str(), textColor ) )
        {
            printf( "Unable to render flag texture!\n" );
        }

    }

    //TODO: Can we streamline the sprite sheet creationg into a function?
    bool loadMedia()
    {
        //Loading success flag
        bool success = true;

        //Load sprite sheet texture
        if( !gSpriteSheetTexture.loadFromFile( "../assets/minesweeper_tiles_32.png") )
        {
            printf( "Failed to load minesweeper sprite sheet texture!\n" );
            success = false;
        }
        else
        {
            int n = 0;
            for (int i = 0; i < SS_COLUMNS; i++){
                for (int j = 0; j < SS_ROWS; j++){
                    gSpriteClips[ n ].x = SPRITE_WIDTH * i;
                    gSpriteClips[ n ].y = SPRITE_HEIGHT * j;
                    gSpriteClips[ n ].w = SPRITE_WIDTH;
                    gSpriteClips[ n ].h = SPRITE_HEIGHT;
                    n++;
                }
            }
        }

        //Load sprite sheet texture
        if( !ResetSpriteSheet.loadFromFile( "../assets/reset_SS.png") )
        {
            printf( "Failed to load reset button sprite sheet texture!\n" );
            success = false;
        }
        else
        {
            int n = 0;
            for (int i = 0; i < BUTTON_SS_COLUMNS; i++){
                for (int j = 0; j < BUTTON_SS_ROWS; j++){
                    ResetSpriteClips[ n ].x = SPRITE_WIDTH * i;
                    ResetSpriteClips[ n ].y = SPRITE_HEIGHT * j;
                    ResetSpriteClips[ n ].w = SPRITE_WIDTH;
                    ResetSpriteClips[ n ].h = SPRITE_HEIGHT;
                    n++;
                }
            }
        }

        //Load sprite sheet texture
        if( !NewSpriteSheet.loadFromFile( "../assets/new_SS.png") )
        {
            printf( "Failed to load new button sprite sheet texture!\n" );
            success = false;
        }
        else
        {
            int n = 0;
            for (int i = 0; i < BUTTON_SS_COLUMNS; i++){
                for (int j = 0; j < BUTTON_SS_ROWS; j++){
                    NewSpriteClips[ n ].x = SPRITE_WIDTH * i;
                    NewSpriteClips[ n ].y = SPRITE_HEIGHT * j;
                    NewSpriteClips[ n ].w = SPRITE_WIDTH;
                    NewSpriteClips[ n ].h = SPRITE_HEIGHT;
                    n++;
                }
            }
        }

        //Load sprite sheet texture
        if( !SmileySpriteSheet.loadFromFile( "../assets/smiley_SS.png") )
        {
            printf( "Failed to load smiley sprite sheet texture!\n" );
            success = false;
        }
        else
        {
            int n = 0;
            for (int i = 0; i < BUTTON_SS_COLUMNS; i++){
                for (int j = 0; j < BUTTON_SS_ROWS; j++){
                    SmileySpriteClips[ n ].x = SPRITE_WIDTH * i;
                    SmileySpriteClips[ n ].y = SPRITE_HEIGHT * j;
                    SmileySpriteClips[ n ].w = SPRITE_WIDTH;
                    SmileySpriteClips[ n ].h = SPRITE_HEIGHT;
                    n++;
                }
            }
        }
        return success;
    }

    ///Revert all graphical modifications to the playfield
    void update(void){
        for (int i = 0; i < col; i++){
            for (int j = 0; j < row; j++){
                playField->tiles[i][j] = fieldVisible[i][j];
            }
        }
    }

    ///Returns the number of mine tiles which are adjacent to the input coordinates
    int setClue(int x, int y){
        int xTemp = 0;
        int yTemp = 0;
        int clue = 0;
        DirectionVector dir;

        for (int d = 0; d < 9; d++){
            dir = DIRECTIONS[d];
            xTemp = x + dir.xDif;
            yTemp = y + dir.yDif;
            if ( xTemp >= 0 && xTemp < col && yTemp >= 0 && yTemp < row){
                if (fieldHidden[xTemp][yTemp] == MINE)
                    clue++;
            }
        }
        return clue;
    }

    ///Reveals the identity of the tile at the input coordinates
    ///If the revealed tile is empty, reveal all adjacent tiles
    ///Tiles marked with a flag cannot be revealed
    ///If a mine is revealed, the player dies and the game is over
    bool sweepTile(int x, int y){
        bool alive = true;
        int xTemp = 0;
        int yTemp = 0;
        DirectionVector dir;

        if ( fieldVisible[x][y] == UNKNOWN ){
            fieldVisible[x][y] = fieldHidden[x][y];
            unexploredTiles--;
            if ( fieldVisible[x][y] == EMPTY){
                for (int d = 0; d < 9; d++){
                    dir = DIRECTIONS[d];
                    xTemp = x + dir.xDif;
                    yTemp = y + dir.yDif;
                    if ( xTemp >= 0 && xTemp < col && yTemp >= 0 && yTemp < row){
                        if (fieldVisible[xTemp][yTemp] == UNKNOWN)
                            sweepTile( xTemp, yTemp );
                    }
                }
            }
            if ( fieldVisible[x][y] == MINE ){
                alive = false;
                revealAll();
            }
        }
        update();
        return alive;
    }

    ///Toggles flag marker (Unknown tiles only)
    void mineFlagToggle(int x, int y){
        if (fieldVisible[x][y] == FLAG){
            fieldVisible[x][y] = UNKNOWN;
            flags++;
        }
        else if (fieldVisible[x][y] == UNKNOWN){
            fieldVisible[x][y] = FLAG;
            flags--;
        }
        update();
    }

    ///Routine for losing the game
    ///Reveals all undiscovered mines and misplaced flags
    void revealAll(void){
        for (int i = 0; i < col; i++){
            for (int j = 0; j < row; j++){
                if ((fieldHidden[i][j] == MINE) && (fieldVisible[i][j] == UNKNOWN) )
                    fieldVisible[i][j] = MINE;
                if ((fieldVisible[i][j] == FLAG) && (fieldHidden[i][j] != MINE) )
                    fieldVisible[i][j] = MISPLACED;
            }
        }
        update();
    }

    ///Routine for winning the game
    ///Places flags on all known mines
    void victory(void){
        for (int i = 0; i < col; i++){
            for (int j = 0; j < row; j++){
                if ((fieldHidden[i][j] == MINE) && (fieldVisible[i][j] == UNKNOWN) ){
                    fieldVisible[i][j] = FLAG;
                    flags--;
                }
            }
        }

        //Set text to be rendered
        flagText.str( "" );
        flagText << flags;

        //Render text
        if( !flagTextTexture.loadFromRenderedText( flagText.str().c_str(), textColor ) )
        {
            printf( "Unable to render flag texture!\n" );
        }

        update();
    }

    ///Handles mouse event
    void handleEvent( SDL_Event* e){
        int x, y;

        //Get mouse position
        if( e->type == SDL_MOUSEMOTION ){
            SDL_GetMouseState( &x, &y );
            if (playField->isInside(x,y) == false){
                inPlay = false;
            }
            else {
                inPlay = true;
            }
        }

        //Handle grid events first
        playField->handleEvent(e);

        //Handle button events first
        resetButton.handleEvent(e);
        newButton.handleEvent(e);


        //Check Button Flags
        if (resetButton.isClicked)
            newGame();

        if (newButton.isClicked)
            set_next_state(STATE_MENU);

        //Refresh playing grid
        update();

        if (!gameOver){
            smileyMood = SMILE;
            if (inPlay){
                if( e->button.button == SDL_BUTTON_LEFT && e->type == SDL_MOUSEBUTTONUP ){
                    sweepEnable = true;
                }
                if( e->button.button == SDL_BUTTON_RIGHT && e->type == SDL_MOUSEBUTTONUP ){
                    flagEnable = true;
                }
                if( e->motion.state || e->button.state == SDL_PRESSED ){
                    smileyMood = SURPRISE;
                    if (playField->tiles[playField->col][playField->row] == UNKNOWN)
                        playField->tiles[playField->col][playField->row] = PRESS;
                }
            }
        }
    }

    void logic(){

        resetButton.logic();
        newButton.logic();

        if (sweepEnable){
            smileyMood = SMILE;
            if ( !sweepTile(playField->col, playField->row) ){
                smileyMood = SAD;
                printf("YOU LOSE...\n");
                gameOver = true;
            }
            else{
                if ( unexploredTiles == mines){
                    smileyMood = WIN;
                    printf("YOU WIN!!!\n");
                    victory();
                    gameOver = true;
                }
            }
            sweepEnable = false;
        }
        if (flagEnable){
            smileyMood = SMILE;
            mineFlagToggle(playField->col, playField->row);

            //Set text to be rendered
            flagText.str( "" );
            flagText << flags;

            //Render text
            if( !flagTextTexture.loadFromRenderedText( flagText.str().c_str(), textColor ) )
            {
                printf( "Unable to render flag text!\n" );
            }

            flagEnable = false;
        }

        if (!gameOver){
            //Set text to be rendered
            timeText.str( "" );
            timeText << (SDL_GetTicks() - startTime) / 1000;

            //Render text
            if( !timeTextTexture.loadFromRenderedText( timeText.str().c_str(), textColor ) )
            {
                printf( "Unable to render time text!\n" );
            }
        }
    }

    void render(){
        playField->render(gSpriteClips, &gSpriteSheetTexture);
        resetButton.render(ResetSpriteClips, &ResetSpriteSheet);
        newButton.render(ResetSpriteClips, &NewSpriteSheet);
        timeTextTexture.render( 5, MENU_BAR_HEIGHT/2 );
        flagTextTexture.render( menu_bar_width - ( flagTextTexture.getWidth() ), MENU_BAR_HEIGHT/2 );
        SmileySpriteSheet.render( menu_bar_width/2 - SPRITE_WIDTH/2, MENU_BAR_HEIGHT/4, &SmileySpriteClips[ smileyMood ] );
    }

};

#endif // MINESWEEPER_H_INCLUDED