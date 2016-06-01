
/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

//Using SDL, standard IO, and, strings
#include <SDL.h>
#include <stdio.h>
#include <string>
#include <string.h>
#include <SDL_image.h>

#include <sstream>
#include <iostream>
#include <cmath>
std::string IntToString (int integer);

enum colour
{
    red=0,green, orange,purple,black
};


//The actual hardware texture
SDL_Texture* Intro[1586];

SDL_Texture* menu;
SDL_Texture* battleshipBoard;
SDL_Texture* ships [5][2];
SDL_Texture* StartGameBtn;
SDL_Texture* BattleShipStart;
SDL_Texture* MiniMap;
SDL_Texture* Miss;
SDL_Texture* Hit;

enum downUp {Down=0,Up};



int IntroW=0;
int IntroH=0;


class buttons
{

public:
    void render (void);
    bool ClickedPause (int xcoordinate, int ycoordinate);


private:




};
//Texture wrapper class
class LTexture
{
public:
    //Initializes variables
    LTexture();

    //Deallocates memory
    ~LTexture();



    bool loadMedia(void);
    SDL_Texture* loadFromFile( std::string path, int width, int height );


    //Deallocates texture
    void free();



    //Renders texture at given point
    void render( int x, int y, SDL_Texture* Texture,int width,int height,  SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );



    //Gets image dimensions
    int getWidth();
    int getHeight();




private:
    //The actual hardware texture
    SDL_Texture* mTexture;


};



//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;
SDL_Surface *screen =NULL;
LTexture gSplashTexture;

//Screen dimensions
SDL_Rect gScreenRect = { 0, 0, 320, 240 };






LTexture::LTexture()
{
    //Initialize
    mTexture = NULL;

}
bool IntroRender (int x, int y)
{
    //Event handler
    SDL_Event e;
    LTexture texture;
    std::string counter="";

    for (int i=0; i<1502; i+=2)
    {
        //Handle events on queue
        while( SDL_PollEvent( &e ) != 0 )
        {
            //Clear screen
            SDL_SetRenderDrawColor( gRenderer, 0, 0, 0, 0 );
            SDL_RenderClear( gRenderer );
            //User requests quit
            if( e.type == SDL_QUIT )
            {
                return false;
            }
        }
        bool quit=false;
        counter+="IntroPics/Intro";
        counter+=IntToString(i+1);
        counter+=".jpg";
        Intro [i]=texture.loadFromFile( counter.c_str(),gScreenRect.w, gScreenRect.h);
        gSplashTexture.render(x, y,Intro[i],IntroW, IntroH);
        //Update screen
        SDL_RenderPresent( gRenderer );
        if (Intro [i]!=NULL)

        {
            SDL_DestroyTexture( Intro [i] );
            Intro [i] = NULL;
        };

        printf(counter.c_str());
        counter="";
        SDL_Delay(10);

    }

//    //Render splash
//    for (int i=0 ; i<1410; i++)
//
//    {
//    if (Intro [i]!=NULL)
//
//        {
//            SDL_DestroyTexture( Intro [i] );
//           Intro [i] = NULL;
//        };
//    if (i<910)
//    Intro [i+500]=texture.loadFromFile( counter.c_str(),gScreenRect.w, gScreenRect.h);
//    }
    return true;
}
std::string IntToString (int integer)
{


    std::stringstream convert;   // stream used for the conversion

    convert << integer;      // insert the textual representation of 'Number' in the characters in the stream

    return convert.str();
}
char IntToChar (int integer)
{
    return (integer+48);
}


LTexture::~LTexture()
{
    //Deallocate
    free();
}

SDL_Texture* LTexture::loadFromFile( std::string path, int width, int height )
{


    //The final texture
    SDL_Texture* newTexture = IMG_LoadTexture(gRenderer,path.c_str());




    if( newTexture == NULL )
    {
        printf( "Unable to create texture from %s! SDL Error:%s\n", path.c_str(), SDL_GetError() );
    }




    return newTexture;
}




void LTexture::free()
{
    //Free texture if it exists
    if( mTexture != NULL )
    {
        SDL_DestroyTexture( mTexture );
        mTexture = NULL;


    }





}





void LTexture::render( int x, int y, SDL_Texture* Texture,int width,int height, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { x, y, width, height };

    //Set clip rendering dimensions
    if( clip != NULL )
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    //Render to screen
    SDL_RenderCopyEx( gRenderer, Texture, clip, &renderQuad, angle, center, flip );
}






bool init()
{
    //Initialization flag
    bool success = true;




    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        SDL_Log( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {

        //Set texture filtering to linear
        if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
        {
            SDL_Log( "Warning: Linear texture filtering not enabled!" );
        }

        //Get device display mode
        SDL_DisplayMode displayMode;
        if( SDL_GetCurrentDisplayMode( 0, &displayMode ) == 0 )
        {
            if (displayMode.w-100>displayMode.h-100)
            {
                gScreenRect.w = displayMode.h-100;
                gScreenRect.h = displayMode.h-100;
            }
            else
            {
                gScreenRect.w = displayMode.w-100;
                gScreenRect.h = displayMode.w-100;
            }



        }

        //Create window
        gWindow = SDL_CreateWindow( "Battle Ship", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, gScreenRect.w, gScreenRect.h, SDL_WINDOW_SHOWN);
        if( gWindow == NULL )
        {
            SDL_Log( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            //Create renderer for window
            gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
            if( gRenderer == NULL )
            {
                SDL_Log( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
                success = false;
            }
            else
            {
                //Initialize renderer color
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
            }
        }
    }

    return success;
}

bool LTexture::loadMedia(void)
{
//Get rid of preexisting texture
    free();
    std::string counter="";




    IntroW=gScreenRect.w;
    IntroH=gScreenRect.h;

    menu=loadFromFile( "Menu.png", gScreenRect.w, gScreenRect.w);
    battleshipBoard=loadFromFile( "BattleShipBoard.png", gScreenRect.w, gScreenRect.w);
    ships [red][Up]= loadFromFile( "redShipUp.png", gScreenRect.w, gScreenRect.w);
    ships [red][Down]= loadFromFile( "redShipDown.png", gScreenRect.w, gScreenRect.w);
    ships [green][Up]= loadFromFile( "greenShipUp.png", gScreenRect.w, gScreenRect.w);
    ships [green][Down]= loadFromFile( "greenShipDown.png", gScreenRect.w, gScreenRect.w);
    ships [orange][Up]= loadFromFile( "orangeShipUp.png", gScreenRect.w, gScreenRect.w);
    ships [orange][Down]= loadFromFile( "orangeShipDown.png", gScreenRect.w, gScreenRect.w);
    ships [purple][Up]= loadFromFile( "purpleShipUp.png", gScreenRect.w, gScreenRect.w);
    ships [purple][Down]= loadFromFile( "purpleShipDown.png", gScreenRect.w, gScreenRect.w);
    ships [black][Up]= loadFromFile( "blackShipUp.png", gScreenRect.w, gScreenRect.w);
    ships [black][Down]= loadFromFile( "blackShipDown.png", gScreenRect.w, gScreenRect.w);
    StartGameBtn= loadFromFile( "StartGameButton.png", gScreenRect.w, gScreenRect.w);
    BattleShipStart= loadFromFile( "BattleShipStart.png", gScreenRect.w, gScreenRect.w);
    MiniMap= loadFromFile( "Map.png", gScreenRect.w, gScreenRect.w);
    Hit= loadFromFile( "explosion.png", gScreenRect.w, gScreenRect.w);
    Miss= loadFromFile( "miss.png", gScreenRect.w, gScreenRect.w);



    return true;




}


void close()
{
    //Free loaded images
    gSplashTexture.free();

    //Destroy window
    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    gRenderer = NULL;

    //Quit SDL subsystems
    SDL_Quit();
}

bool overlapFunc (int colour, float motionx, float motiony,  float ShipSize[5][2],float ShipsPotions[5][2])
{
    enum coordinates {x=0,y};


    for(int i=0; i<5; i++)
    {
        if (((motionx>ShipsPotions[i][x] or motionx + ShipSize[colour][x]>ShipsPotions[i][x]) and (motionx<ShipsPotions[i][x]+ShipSize[i][x] or motionx + ShipSize[colour][x]<ShipsPotions[i][x]+ShipSize[i][x]))
                and ((motiony>ShipsPotions[i][y] or motiony + ShipSize[colour][y]>ShipsPotions[i][y]) and (motiony<ShipsPotions[i][y]+ShipSize[i][y] )))
        {
            if (i!=colour)
            {
                return true;
            }
        }

    }

}



void PlayersMapSetter (float ShipSize[5][2],float ShipsPotions[5][2], char map[10][10])
{

//check to see if in the on the grid properly

    enum colour
    {
        red=0,green, orange,purple,black
    };
    int xchange = ceil(gScreenRect.w/23.9473684211);
    int ychange = ceil(gScreenRect.h/11.9512195122);
    enum xy {x=0,y};

    int sizes [5]= {5,2,5,5,2};
    char markers [5]= {'R','G','O','P','B'};

//if (ShipsPotions[colour][x]<644)
    // return false;
    for (int i=0; i<5; i++)
    {
        int rows=0;

        for (int ys=gScreenRect.h/5.76470588235; ys<gScreenRect.h/1.00926879506; ys+=ychange)
        {
            int cols=0;
            for (int xs=gScreenRect.w/2.82608695652; xs<gScreenRect.w/1.2962962963; xs+=xchange)
            {

                if (((ShipsPotions[i][x]>xs or ShipsPotions[i][x] + ShipSize[i][x]>xs) and (ShipsPotions[i][x]<xs+xchange or ShipsPotions[i][x] + ShipSize[i][x]<xs+xchange))
                        and ((ShipsPotions[i][y]>ys or ShipsPotions[i][y] + ShipSize[i][y]>ys) and (ShipsPotions[i][y]<ys+ychange)))
                {
                    map[cols][rows]=markers[i];
                }
                cols++;
            }
            rows++;

        }

    }
//code for printting array to test
    /* int rows=0;


         for (int ys=gScreenRect.h/5.76470588235; ys<gScreenRect.h/1.00926879506; ys+=ychange)
         {
    int cols=0;

             for (int xs=gScreenRect.w/2.82608695652; xs<gScreenRect.w/1.2962962963; xs+=xchange)
             {


                     std::cout<<std::endl<<map[cols][rows];
                     //count++;

                 cols++;
             }
             rows++;

         }
         */
    // std::cout<<std::endl<<count;





}
void BlankMap (char map[10][10])
{
    for (int i=0; i<10; i++)
    {
        for (int t=0; t<10; t++)
        {
            map[t][i]='0';
        }
    }
}

void DisplayPlayersMap (char map[10][10], float ShipsPotions[5][2], float ShipSize[5][2])
{
    //check to see if in the on the grid properly


    int xchange = gScreenRect.w/56.875;
    int ychange = gScreenRect.h/30.625;
    enum xy {x=0,y};
    int counter=0;
    int sizes [5]= {5,2,5,5,2};
    bool colours [5]= {false,false, false,false, false};


    int rows=0;
    for (int ys=gScreenRect.h/1.73451327434; ys<gScreenRect.h/1.11111111111; ys+=ychange)
    {
        int cols=0;

        for (int xs=gScreenRect.w/1.22972972973; xs<gScreenRect.w/1.01449275362; xs+=xchange)
        {

            if (map[cols][rows]=='R' and colours[red]==false)
            {
                ShipSize[red][x]/=2.5;
                ShipSize[red][y]/=2.5;
                ShipsPotions[red][x]=xs+2;
                ShipsPotions[red][y]=ys+5;

                colours[red]=true;
            }
            else if (map[cols][rows]=='G' and colours[green]==false)
            {
                ShipSize[green][x]/=2.5;
                ShipSize[green][y]/=2.5;
                ShipsPotions[green][x]=xs+2;
                ShipsPotions[green][y]=ys+2;
                colours[green]=true;
            }
            else if (map[cols][rows]=='B' and colours[black]==false)
            {
                ShipSize[black][x]/=2.5;
                ShipSize[black][y]/=2.5;
                ShipsPotions[black][x]=xs+2;
                ShipsPotions[black][y]=ys+2;
                colours[black]=true;
            }
            else if (map[cols][rows]=='O' and colours[orange]==false)
            {
                ShipSize[orange][x]/=2.5;
                ShipSize[orange][y]/=2.5;
                ShipsPotions[orange][x]=xs+2;
                ShipsPotions[orange][y]=ys+2;
                colours[orange]=true;
            }
            else if (map[cols][rows]=='P' and colours[purple]==false)
            {
                ShipSize[purple][x]/=2.5;
                ShipSize[purple][y]/=2.5;
                ShipsPotions[purple][x]=xs+2;
                ShipsPotions[purple][y]=ys+2;
                colours[purple]=true;
            }



            cols++;
        }
        rows++;


    }




}


bool OnGrid (float ShipSize[5][2],float ShipsPotions[5][2])
{
//check to see if in the on the grid properly

    int numberOfSquares=0;
    int xchange = ceil(gScreenRect.w/23.9473684211);
    int ychange = ceil(gScreenRect.h/11.9512195122);
    enum xy {x=0,y};
    int counter=0;
    int sizes [5]= {5,2,5,5,2};
    for (int i=0; i<5; i++)
    {
        if (ShipsPotions[i][x]+ShipSize[i][x]>gScreenRect.w/1.2962962963 or ShipsPotions[i][y]<gScreenRect.h/5.76470588235 or ShipsPotions[i][x]<gScreenRect.w/2.82608695652)
        {
            return false;
        }

    }
//if (ShipsPotions[colour][x]<644)
    // return false;
    for (int i=0; i<5; i++)
    {
        numberOfSquares=0;
        for (int ys=gScreenRect.h/5.76470588235; ys<gScreenRect.h/1.00926879506; ys+=ychange)
        {
            for (int xs=gScreenRect.w/2.82608695652; xs<gScreenRect.w/1.2962962963+2; xs+=xchange)
            {

                if (((ShipsPotions[i][x]>xs or ShipsPotions[i][x] + ShipSize[i][x]>xs) and (ShipsPotions[i][x]<xs+xchange or ShipsPotions[i][x] + ShipSize[i][x]<xs+xchange))
                        and ((ShipsPotions[i][y]>ys or ShipsPotions[i][y] + ShipSize[i][y]>ys) and (ShipsPotions[i][y]<ys+ychange)))
                {
                    numberOfSquares+=1;
                }
            }

        }
        if (numberOfSquares==sizes[i]) counter++;

    }

    if (counter==5)return true;
    else return false;


}

void PlayerMarkingClick (int Clickx, int Clicky, char AImap [10][10])
{
    int xchange = ceil(gScreenRect.w/23.9473684211);
    int ychange = ceil(gScreenRect.h/11.9512195122);
    int rows=0;

    for (int ys=gScreenRect.h/5.76470588235; ys<gScreenRect.h/1.00926879506; ys+=ychange)
    {
        int cols=0;
        for (int xs=gScreenRect.w/2.82608695652; xs<gScreenRect.w/1.2962962963; xs+=xchange)
        {

            if (Clickx>xs and Clickx<xs+xchange and Clicky>ys and Clicky<ys+ychange)
            {
                AImap[cols][rows]='X';
            }
            cols++;
        }
        rows++;

    }


}
void PlayerDisplayingHitsAndMiss (char AImap [10][10])
{
    int xchange = gScreenRect.w/23.9473684211;
    int ychange = ceil(gScreenRect.h/11.9512195122);
    int rows=0;

    for (int ys=gScreenRect.h/5.76470588235; ys<gScreenRect.h/1.00926879506; ys+=ychange)
    {
        int cols=0;
        for (int xs=ceil(gScreenRect.w/2.82608695652)+10; xs<gScreenRect.w/1.2962962963; xs+=xchange)
        {

            if (AImap[cols][rows]=='X')
            {
                gSplashTexture.render(xs,ys,Miss,xchange, ychange);
            }
            cols++;
        }
        rows++;

    }


}




int main( int argc, char* args[] )
{
    bool Menu=true;
    int  Clickx=0;
    int Clicky=0;
    bool trackingGeneral = false;
    int motionx = 0;
    int motiony = 0;
    bool tracking[5]= {false,false,false,false,false};
    bool overlap =false;
    enum coordinates {x=0,y};
    int directions[5]= {Down,Down,Down,Down,Down};
    bool LockShips=false;
    bool StartGame=false;
    char PlayerMap [10][10];
    char AIMap [10][10];
    BlankMap(PlayerMap);
    BlankMap(AIMap);



    //Start up SDL and create window
    if( !init())
    {
        SDL_Log( "Failed to initialize!\n" );
    }
    else
    {

        //Main loop flag
        bool quit = false;

        //Event handler
        SDL_Event e;

        gSplashTexture.loadMedia();

        //While application is running
        while( !quit )
        {

            //Handle events on queue
            while( SDL_PollEvent( &e ) != 0 )
            {
                //User requests quit
                if( e.type == SDL_QUIT )
                {
                    quit = true;
                }

                else if( e.type == SDL_WINDOWEVENT )
                {
                    //Window resize/orientation change
                    if( e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED )
                    {

                        //Get screen dimensions
                        gScreenRect.w = e.window.data1;
                        gScreenRect.h = e.window.data2;

                        gSplashTexture.loadMedia();





                        //Update screenRocketY-gScreenRect.h/5
                        SDL_RenderPresent( gRenderer );
                    }

                }

            }


            //Clear screen
            SDL_SetRenderDrawColor( gRenderer, 0, 0, 0, 0 );
            SDL_RenderClear( gRenderer );

            float ShipsPotions[5][2] = {{gScreenRect.w/20,gScreenRect.h/10},{gScreenRect.w/20,gScreenRect.h/5},{gScreenRect.w/20,gScreenRect.h/4},{gScreenRect.w/20,gScreenRect.h/3},{gScreenRect.w/20,gScreenRect.h/2.5}};
            float ShipSize [5][2]= {{gScreenRect.w/5,gScreenRect.h/20},{gScreenRect.w/14,gScreenRect.h/20},{gScreenRect.w/5,gScreenRect.h/20},{gScreenRect.w/5,gScreenRect.h/20},{gScreenRect.w/14,gScreenRect.h/20}};



            //Maps.collision();



            //Maps.ScrollingMaps();




            while (true)
            {
                Clickx=0;
                Clicky=0;
                //Handle events on queue
                while( SDL_PollEvent( &e ) != 0 )
                {
                    //User requests quit
                    if( e.type == SDL_QUIT )
                    {
                        //Free resources and close SDL
                        close();

                        return 0;
                    }
                    else if(e.type == SDL_MOUSEBUTTONDOWN)
                    {
                        //If the left mouse button was pressed
                        if(e.button.button == SDL_BUTTON_LEFT )
                        {
                            Clickx=e.button.x;
                            Clicky=e.button.y;

                            for (int i=0; i<5; i++)
                            {

                                if (tracking[i]==true and trackingGeneral==true and !overlapFunc(i,motionx,motiony,ShipSize,ShipsPotions))
                                {
                                    tracking[i]=false;
                                    trackingGeneral=false;
                                }




                            }
                        }
                        else
                        {
                            if (trackingGeneral==true)
                            {


                                for (int i=0; i<5; i++)
                                {
                                    if (tracking[i]==true)
                                    {
                                        int temp=0;
                                        temp=ShipSize[i][x];
                                        ShipSize[i][x]=ShipSize[i][y];
                                        ShipSize[i][y]=temp;
                                        temp=ShipsPotions[i][x];
                                        ShipsPotions[i][x]=ShipsPotions[i][y];
                                        ShipsPotions[i][y]=temp;
                                        if (directions[i]==Up)
                                            directions[i]=Down;
                                        else
                                            directions[i]=Up;

                                    }


                                }
                            }


                        }
                    }

                    else if( e.type == SDL_MOUSEMOTION )
                    {
                        if (trackingGeneral==true)
                        {

                            //Get the mouse offsets

                            SDL_GetMouseState( &motionx, &motiony );

                        }



                    }


                }

                if (trackingGeneral==true)
                {
                    for (int i=0; i<5; i++)
                    {
                        if (tracking[i]==true)
                        {


                            ShipsPotions[i][x]=motionx;
                            ShipsPotions[i][y]=motiony;


                        }
                    }


                }
                if (Clickx!=0 or Clicky!=0)
                {
                    std::cout<<std::endl<<"x="<<Clickx;
                    std::cout<<std::endl<<"y="<<Clicky;
                }

//std::cout<<gScreenRect.w/20+IntroW/5<<std::endl;
                if (trackingGeneral==false and LockShips==false)
                {


                    for (int i=0; i<5; i++)
                    {

                        if (Menu==false and Clicky<ShipsPotions[i][y]+ShipSize[i][y] and Clicky>ShipsPotions[i][y] and Clickx>ShipsPotions[i][x] and Clickx<ShipsPotions[i][x]+ShipSize[i][x] )
                        {
                            trackingGeneral=true;
                            tracking[i]=true;
                            motionx=ShipsPotions[i][x];
                            motiony=ShipsPotions[i][y];
                        }
                    }
                }

                if (Menu==true)
                {

                    gSplashTexture.render(( gScreenRect.w - gScreenRect.w/2 - gScreenRect.w/3 - gScreenRect.w/5) / 2 +gScreenRect.w/60.6666666667,0,menu,IntroW, IntroH);
                    //Update screen
                    SDL_RenderPresent( gRenderer );
                    //Menu=true;
                    if (Clickx>gScreenRect.w/6.97318007663 and Clickx<gScreenRect.w/1.15262824573 and Clicky<gScreenRect.h/1.18932038835 and Clicky>gScreenRect.h/1.48036253776)
                    {
                        Menu=false;
                    }
                }
                else if (StartGame==false and Menu==false)
                {
                    //Clear screen
                    //SDL_SetRenderDrawColor( gRenderer, 0, 0, 0, 0 );
                    //SDL_RenderClear( gRenderer );
                    gSplashTexture.render(( gScreenRect.w - gScreenRect.w/2 - gScreenRect.w/3 - gScreenRect.w/5) / 2 +gScreenRect.w/60.6666666667,0,battleshipBoard,IntroW, IntroH);
                    int temp=10;
                    for (int i=0; i<5; i++)
                    {

                        gSplashTexture.render(ShipsPotions[i][x],ShipsPotions[i][y],ships[i][directions[i]],ShipSize[i][x], ShipSize[i][y]);
                        if (tracking[i]==true)
                        {
                            temp=i;
                        }
                    }



                    if (temp!=10)
                        gSplashTexture.render(ShipsPotions[temp][x],ShipsPotions[temp][y],ships[temp][directions[temp]],ShipSize[temp][x], ShipSize[temp][y]);

                    if (trackingGeneral==false)
                    {
                        if (OnGrid(ShipSize,ShipsPotions))
                        {
                            gSplashTexture.render(gScreenRect.w/1.27,gScreenRect.h/1.7,StartGameBtn,gScreenRect.w/5, gScreenRect.h/10);
                            if (Clickx>gScreenRect.w/1.27 and Clickx<gScreenRect.w/1.27+gScreenRect.w/5 and Clicky>gScreenRect.h/1.7 and Clicky<gScreenRect.h/1.7+gScreenRect.h/10)
                            {

                                LockShips=true;
                                StartGame=true;
                                PlayersMapSetter(ShipSize,ShipsPotions,PlayerMap);
                                DisplayPlayersMap(PlayerMap,ShipsPotions,ShipSize);
                            }
                        }


                    }



                }
                else if (StartGame==true )
                {
                    PlayerMarkingClick(Clickx,Clicky,AIMap);

                    gSplashTexture.render(( gScreenRect.w - gScreenRect.w/2 - gScreenRect.w/3 - gScreenRect.w/5) / 2 +30,0,BattleShipStart,IntroW, IntroH);

                    gSplashTexture.render(gScreenRect.w/1.25517241379,gScreenRect.h/1.83177570093,MiniMap,gScreenRect.w/5.2, gScreenRect.h/2.8);

                    for (int i=0; i<5; i++)
                    {

                        gSplashTexture.render(ShipsPotions[i][x],ShipsPotions[i][y],ships[i][directions[i]],ShipSize[i][x], ShipSize[i][y]);

                    }
                    PlayerDisplayingHitsAndMiss(AIMap);


                    //Update screen
                    // SDL_RenderPresent( gRenderer );
                }




                SDL_RenderPresent( gRenderer );


            }


        }
    }





//Free resources and close SDL
    close();

    return 0;
}
