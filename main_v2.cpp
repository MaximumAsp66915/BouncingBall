#include <bits/stdc++.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL2_gfx.h>
#include <stdio.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <fstream>

using namespace std;

#define n_rows 10 // !!!
#define n_columns 15 // !!!
#define n_rows_out 2000
#define time_freeze_ball_available 30 // It would take 30 seconds
#define freeze_ball_time_apply 10 // 10 is for normal
#define time_laser_ball_available 45 // It would take 45
#define laser_ball_time_apply 15 // 15 is for normal
#define time_mega_bomb_ball_available 15 // It would take 15 seconds //// its the time for each phrase to get activated
#define mega_bomb_time_explosion 15 // 15 is good amount and it will be multiplied by the ball phrase
#define mega_ball_rel_speed 0.5 // it will show the relative speed of the mega ball to a normal flying ball
#define vel_flying_ball 1 // amount should be with in range (0,1] , preferred .5 but will be so slow
#define number_of_colors 6
#define optimised_fps true // The non optimised version will use fps of 60
#define timer 60 // for the timer mode

int theme = 1;
int backgroundImg = 1;
string backgroundImgS = "../images/blue.jpg";
int n_rows_out_manual = 2000 ;
float vel_balls_go_down_manual = 2 ;
int vel_balls_go_down_initial_increase = 60 ;// preferred at around 60 for normal
float vel_balls_go_down_coll_reduction = 0.1 ;// preferred at around 0.2 for normal
float vel_balls_go_down_coll_recovery_time = 1 ;// preferred at around 1 for normal
float vel_balls_go_down_rate_increase = 0.1 ;// preferred at around 0.1 for normal
bool effects = true ;
bool multi_color_flying_ball = false ;
float fps = 60 ;// Preferred at around 60
float mean_fps = 60 ;
int color[number_of_colors] ;
float vel_balls_go_down = 0.5 * vel_balls_go_down_manual ;
int play_mode = 1 ; //// 0 -> developer / 1 -> infinity / 2 -> easy / 3 -> normal / 4 -> hard / 5 -> extreme
int color_selection[number_of_colors] = {}; //// for chosing theme
int amount_multi_color_flying_ball = 20; // 1 in every number that you put there will be a ball with multi colors
bool color_random_chosen = true;
bool gray_balls_exist = false;
float difficulty_gray_balls = 0.5; // !!! IT SHOULD BE IN THIS FORM AND BE FLOAT // the lower it is the less compacted will be the gray balls
bool multicolor_ball = false;
float amount_multicolor_ball = 0.5; // !!! IT SHOULD BE IN THIS FORM AND BE FLOAT // the lower it is the more compacted will be the multi_colored will be the balls
bool locked_ball = false;
float amount_locked_ball = 0.5;
float volume_of_balls = 1; // !!! IT SHOULD BE IN THIS FORM AND BE FLOAT // and for now it can not be 1 or 0 ...
int difficulty_of_colors = 2; // It should be bigger than 0 and its int , the bigger it is the less difficult the map would get for normal around 5
double ceil_y = 0 ;
int number_destroyed_balls = 0;
int number_fallen_balls = 0;
int total_score = 0 ;
int k_upbl = n_rows_out_manual -1; //iterator
bool u_b1 = true; //unknown_bool1
bool freeze_ball_allowed = false ;
bool laser_ball_allowed = false ;
bool mega_bomb_ball_allowed = false ;
bool timer_mode_allowed = false ;
bool timer_started = false;
clock_t timer_checker ;


struct User{
    string name = "";
    int score[3] = {0,0,0}; //normal 0 , time 1 , infinity 2
    int musicVolume = 409;
    int chunkVolume = 409;
    int music = 1;
    bool musicStop = false;
    int theme = 1;
};
Mix_Music *music;
int sec = 0;
int presec = 0;
string username = "";

struct ball2d {
    double x = 0;
    double y = 0;
    int rgb[3] = {0,0,0};
    int rgb1[3] = {0,0,0};
    bool is_exist = false;
    int eo = 1;
    bool connect = false;
    bool falling = false;
    bool multicolor = false;
    bool locked = false;
};
struct color2d {
    int r = 0 ;
    int g = 0 ;
    int b = 0 ;
    bool is_exist = false ;
    bool is_exist_temp = false ;

};
struct flying_color2d {
    int r = 0 ;
    int g = 0 ;
    int b = 0 ;
    bool multi_color = false ;
};
struct next_ball_color2d {
    int r = 0 ;
    int g = 0 ;
    int b = 0 ;
    bool multi_color = false ;
};

int initializing_play_mode_0(SDL_Renderer * renderer , int W , int H , ball2d balls[n_rows][n_columns] , ball2d balls_out[n_rows_out][n_columns] , color2d colors[9] , vector<array<int, 2>> &output , int color[number_of_colors]);
int initializing_play_mode_1(SDL_Renderer * renderer , int W , int H , ball2d balls[n_rows][n_columns] , ball2d balls_out[n_rows_out][n_columns] , color2d colors[9] , vector<array<int, 2>> &output , int color[number_of_colors]);
int initializing_play_mode_2(SDL_Renderer * renderer , int W , int H , ball2d balls[n_rows][n_columns] , ball2d balls_out[n_rows_out][n_columns] , color2d colors[9] , vector<array<int, 2>> &output , int color[number_of_colors]);
int initializing_play_mode_3(SDL_Renderer * renderer , int W , int H , ball2d balls[n_rows][n_columns] , ball2d balls_out[n_rows_out][n_columns] , color2d colors[9] , vector<array<int, 2>> &output , int color[number_of_colors]);
int initializing_play_mode_4(SDL_Renderer * renderer , int W , int H , ball2d balls[n_rows][n_columns] , ball2d balls_out[n_rows_out][n_columns] , color2d colors[9] , vector<array<int, 2>> &output , int color[number_of_colors]);
int initializing_play_mode_5(SDL_Renderer * renderer , int W , int H , ball2d balls[n_rows][n_columns] , ball2d balls_out[n_rows_out][n_columns] , color2d colors[9] , vector<array<int, 2>> &output , int color[number_of_colors]);
int initializing_play_mode_6(SDL_Renderer * renderer , int W , int H , ball2d balls[n_rows][n_columns] , ball2d balls_out[n_rows_out][n_columns] , color2d colors[9] , vector<array<int, 2>> &output , int color[number_of_colors]);
void menu(SDL_Renderer * renderer , int W , int H , int mouseX , int mouseY , bool & click , bool & started , bool & playing);
int checking_collision_ceil(SDL_Renderer * renderer , int W , int H , ball2d balls[n_rows][n_columns] , ball2d & flying_bullet , double &vel_x , double &vel_y , flying_color2d flying_color  , int &new_ball_row , int &new_ball_column , int R_collision);
void checkGameStatus(SDL_Renderer * renderer , int , int  , bool & , bool, ball2d balls[n_rows][n_columns] , double deadline_y);
void drawImg(SDL_Renderer* renderer , string imagePath , double , double);
int checkBallConnection(ball2d balls[n_rows][n_columns] , int p , int q);
int balls_go_down(SDL_Renderer * renderer , int W , int H , ball2d balls[n_rows][n_columns] , ball2d balls_out[n_rows_out][n_columns] , double deadline_y);
double invert(int n) ;
int find_sum_2_to_1_given(float input , int &output1 , int &output2) ;
double find_minimum_index(vector<double> arr) ;
double distance(double x1 , double y1 , double x2 , double y2);
int draw_balls2(SDL_Renderer * renderer , int W , int H , ball2d balls[n_rows][n_columns]);
int wanna_go_out(int x , int y , int W , int H , int R);
int initial_balls_coordinate(SDL_Renderer * renderer , int W , int H , ball2d balls[n_rows][n_columns]);
int initial_balls_color_0(SDL_Renderer * renderer , int W , int H , ball2d balls[n_rows][n_columns] , ball2d balls_out[n_rows_out][n_columns] , color2d colors[9] , vector<array<int, 2>> &output , int color[number_of_colors] );
int initial_balls_color_1(SDL_Renderer * renderer , int W , int H , ball2d balls[n_rows][n_columns] , ball2d balls_out[n_rows_out][n_columns] , color2d colors[9] , vector<array<int, 2>> &output , int color[number_of_colors] );
int draw_target(SDL_Renderer * renderer , int W , int H , int mouseX , int mouseY , flying_color2d flyingColor , color2d colors[8]);
int draw_beam(SDL_Renderer * renderer , int W , int H , int mouseX , int mouseY , ball2d & , double &vel_x , double &vel_y , flying_color2d flying_color , color2d colors[8]);
int draw_balls(SDL_Renderer * renderer , int W , int H , ball2d balls[n_rows][n_columns]);
int update_balls(SDL_Renderer * renderer , int W , int H , ball2d balls[n_rows][n_columns] , ball2d balls_out[n_rows_out][n_columns]);
int checking_collision(SDL_Renderer * renderer , int W , int H , ball2d balls[n_rows][n_columns] , ball2d & , double &vel_x , double &vel_y , flying_color2d flying_color , int &new_ball_row , int &new_ball_column , int R_collision);
int collision_type(ball2d balls[n_rows][n_columns] , int row1 , int columns1 , flying_color2d flying_color) ;
int game_CHECK(ball2d balls[n_rows_out][n_columns], ball2d balls_out[n_rows_out][n_columns] , color2d colors[9]) ;
int colors_exist(int r ,int g , int b , color2d colors[9] , int & counter) ;
int color_change_next_ball(flying_color2d &flyingColor , next_ball_color2d &next_Ball_Color) ;
int finding_neighbors_balls(int i_coll, int j_coll, ball2d balls[n_rows][n_columns], vector<array<int, 2>> &output ) ;
int finding_neighbors_balls_new(int i_coll, int j_coll, ball2d balls[n_rows][n_columns], vector<array<int, 2>> &output ) ;
int same_color (vector< array<int , 2> >  &output , ball2d balls[n_rows][n_columns] , int i_coll , int j_coll , int i_upper_ball , int j_upper_ball , vector<array<int , 2>> & changed_balls , int r , int g , int b );
int finding_balls_same_color (vector< array<int , 2> >  &output , ball2d balls[n_rows][n_columns] , int i_coll , int j_coll , vector<array<int , 2>> & changed_balls , int r ,int g , int b) ;
int laser_ball_beaming (SDL_Renderer * renderer , int W , int H , int mouseX , int mouseY , double vel_x , double vel_y , flying_color2d flying_color , color2d colors[8] , ball2d & , ball2d balls[n_rows][n_columns] , vector <array<int , 2> > &output);
int checking_collision_laser(SDL_Renderer * renderer , int W , int H , ball2d balls[n_rows][n_columns] , ball2d & , double &vel_x , double &vel_y , flying_color2d flying_color , int &new_ball_row , int &new_ball_column , int R_collision , vector <array<int , 2> > &output);
int mega_bomb_beaming(SDL_Renderer * renderer , int W , int H , int mouseX , int mouseY , ball2d & flying_bullet , double &vel_x , double &vel_y , flying_color2d flying_color , color2d colors[8] , int mega_phrase);
int checking_collision_mega_bomb(SDL_Renderer * renderer , int W , int H , ball2d balls[n_rows][n_columns] , ball2d & flying_bullet, flying_color2d flying_color , int &new_ball_row , int &new_ball_column , int mega_phrase , vector< array <int , 2 >> &destructed_balls);

void renderText(const char * text,int font_size,int font,SDL_Renderer *renderer,int x,int y,SDL_Color textColor)

{
    TTF_Init();

    const char * font_address_01="../fonts/OpenSans-Bold.ttf";

    const char * Font =NULL;

    if(font==1)
    {
        Font =font_address_01;
    }

    int mWidth = 0;
    int mHeight = 0;
    SDL_Rect* clip = NULL;
    TTF_Font *gFont = NULL;
    double angle = 0.0;
    SDL_Point* center = NULL;
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    SDL_Texture* mTexture = NULL;
    gFont = TTF_OpenFont(Font, font_size );

    //SDL_Surface* textSurface = TTF_RenderText_Solid( gFont,text, textColor );
    SDL_Surface* textSurface = TTF_RenderText_Blended(gFont,text,textColor);

    mWidth = textSurface->w;
    mHeight = textSurface->h;
    mTexture = SDL_CreateTextureFromSurface( renderer, textSurface );
    SDL_FreeSurface( textSurface );


    SDL_Rect renderQuad = { x , y , mWidth, mHeight };
    SDL_RenderCopyEx( renderer, mTexture, clip, &renderQuad, angle, center, flip );

    SDL_DestroyTexture( mTexture );
    //SDL_RenderPresent( renderer );

}
void renderImg(SDL_Renderer *renderer, SDL_Texture *img , double x, double y, double W, double H , bool renderB) {

    //SDL_QueryTexture(img, NULL, NULL, &w, &h);

    SDL_Rect texr;
    texr.x = x;
    texr.y = y;
    texr.w = W;
    texr.h = H;

    SDL_RenderCopy(renderer, img, NULL, &texr);
    SDL_SetRenderTarget(renderer, img);
    if(renderB){
        SDL_RenderPresent(renderer);
    }
    //SDL_DestroyTexture(texture);
    //SDL_FreeSurface(image);
    //IMG_Quit();
}

string join(const vector<string>& v, char delimiter) {
    ostringstream s;
    for (const auto& i : v) {
        if (&i != &v[0])
            s << delimiter;
        s << i;
    }
    return s.str();
}
vector<string> split(const string& s, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(s);
    while (getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}
bool compareDataS0(User a , User b){
    return a.score[0] > b.score[0];
}
bool compareDataS1(User a , User b){
    return a.score[1] > b.score[1];
}
bool compareDataS2(User a , User b){
    return a.score[2] > b.score[2];
}
bool compareDataA(User a , User b){
    return a.name > b.name;
}
bool updateFile(const string& path , vector<User>& data){
    ofstream file(path);
    if (file.is_open()) {
        for (const auto& user : data) {
            vector<string> vec = {user.name , to_string(user.score[0]), to_string(user.score[1]) , to_string(user.score[2]), to_string(user.music) , to_string(user.musicVolume) , to_string(user.chunkVolume) , to_string(user.musicStop) ,to_string(user.theme)};
            file << join(vec, ',') << "\n";
        }
        file.close();
        return true;
    }else{
        cout<<"can't open "<<path<<endl;
        return false;
    }
}
bool readData(const string& path , vector<User>& data){
    ifstream file(path);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            vector<string> vec;
            vec = split(line, ',');
            User user;
            user.name = vec[0];
            user.score[0] = stoi(vec[1]);
            user.score[1] = stoi(vec[2]);
            user.score[2] = stoi(vec[3]);
            user.music = stoi(vec[4]);
            user.musicVolume = stoi(vec[5]);
            user.chunkVolume = stoi(vec[6]);
            user.musicStop = stoi(vec[7]);
            user.theme = stoi(vec[8]);
            data.push_back(user);
        }
        sort(data.begin() , data.end() , compareDataA);
        file.close();
        return true;
    }else{
        cout<<"can't open "<<path<<endl;
        return false;
    }
}
bool buttonClicked(double W , double H , double mouseX, double mouseY, double x, double y, double r) {
    x = x*W/1920;
    y = y*H/1080;
    if (sqrt((mouseY - y) * (mouseY - y) + (mouseX - x) * (mouseX - x)) < r)
        return true;
    return false;
}

void userNameMenu(SDL_Renderer *renderer, int W, int H, vector<User>& data) {
    SDL_Event *event = new SDL_Event();
    string textInput = " ";
    bool running = true;
    int flag2 = 0;
    double mouseX , mouseY;
    bool click = false;
    SDL_Texture *img = NULL;
    if(theme==1) {
        img = IMG_LoadTexture(renderer, "../images/3.png");
    }else{
        img = IMG_LoadTexture(renderer, "../images/g3.png");
    }
    while(running) {
        SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255 );
        SDL_RenderClear( renderer );
        renderImg(renderer, img , 0, 0, W, H , false);
        if (SDL_PollEvent(event)) {
            switch (event->type) {
                case SDL_TEXTINPUT:
                    textInput += event->text.text;
                    flag2 = 0;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (event->button.button == SDL_BUTTON_LEFT) {
                        mouseX = event->motion.x;
                        mouseY = event->motion.y;
                        click = true;
                    }
                    break;
                case SDL_KEYDOWN:
                    if(event->key.keysym.sym == SDLK_BACKSPACE && textInput.length()!=1 ){
                        textInput.pop_back();
                        flag2 = 0;
                    }
                    break;
            }
        }
        renderText(textInput.c_str(), 100, 1, renderer, W / 6, 0.48 * H, {255, 255, 255, 255});
        if (click && buttonClicked(W , H , mouseX, mouseY, 985, 785, 65)) {
            if (textInput.substr(1, textInput.length() - 1) == "") {
                //---
            }else{
                bool flag = true;
                for (auto user: data) {
                    if (user.name == textInput.substr(1, textInput.length() - 1)) {
                        flag = false;
                        flag2++;
                        break;
                    }
                }
                if (flag || flag2 == 2) {
                    if (flag2 != 2) {
                        User user;
                        user.name = textInput.substr(1, textInput.length() - 1);
                        //user.score = 0;
                        data.push_back(user);
                        //updateFile("E:\\Term1\\C++_mabani\\BB\\files\\data.csv", data);
                    }
                    username = textInput.substr(1, textInput.length() - 1);
                    sec = 1;
                    presec = 3;
                    running = false;

                    for (auto user: data) {
                        if (user.name == username) {
                            theme = user.theme;
                            switch (user.music) {
                                case 1:
                                    music = Mix_LoadMUS("../musics/mainmusic.mp3");
                                    break;
                                case 2:
                                    music = Mix_LoadMUS("../musics/gole.mp3");
                                    break;
                                case 3:
                                    music = Mix_LoadMUS("../musics/ow.mp3");
                                    break;
                            }
                            int maxVolume = 409 * H / 1080;
                            if (user.musicVolume == 409 && user.chunkVolume == 409) {
                                user.musicVolume == maxVolume;
                                user.chunkVolume == maxVolume;
                            }
                            Mix_PlayMusic(music, 1000);
                            Mix_Volume(-1, MIX_MAX_VOLUME * user.chunkVolume / maxVolume);
                            Mix_VolumeMusic(MIX_MAX_VOLUME * user.musicVolume / maxVolume);
                        }
                    }
                }
            }
            click = false;
        }
        if(flag2 == 1){
            string text1 = textInput.substr(1, textInput.length() - 1) + " is an old user if you aren't him/her please try new name else click again !";
            renderText(text1.c_str(), 20, 1, renderer, 430 * W / 1920, 680 * H / 1080,{255, 0, 0, 255});
        }
        SDL_RenderPresent(renderer);
    }
}
bool startMenuB = true;
void startMenu(SDL_Renderer *renderer, int W, int H, int mouseX, int mouseY, bool &click) {
    if (startMenuB) {
        SDL_Texture *img = NULL;
        if(theme==1) {
            img = IMG_LoadTexture(renderer, "../images/5.png");
        }else{
            img = IMG_LoadTexture(renderer, "../images/g1.png");
        }
        renderImg(renderer, img , 0, 0, W, H,true);
        startMenuB = false;
    } else if (click && buttonClicked(W , H , mouseX, mouseY, 956 , 540 , 256)) {
        sec = 8;
        presec = 1;
        startMenuB = true;
    }else if (click && buttonClicked(W , H , mouseX, mouseY, 479 , 762 , 222 )) {
        sec = 5;
        presec = 1;
        startMenuB = true;
    }else if (click && buttonClicked(W , H , mouseX, mouseY, 1439, 762, 222)) {
        sec = 4;
        presec = 1;
        startMenuB = true;
    }
}
bool rankShowB = true;
int page = 1;
int modeRankShow = 0;
void rankShow(SDL_Renderer *renderer, double W, double H , vector<User>& data ) {
    string text;
    SDL_Event *event = new SDL_Event();
    bool running = true;
    double mouseX , mouseY;
    bool click = false;
    SDL_Texture *img = NULL;
    while(running) {
        if (SDL_PollEvent(event)) {
            switch (event->type) {
                case SDL_MOUSEBUTTONDOWN:
                    if (event->button.button == SDL_BUTTON_LEFT) {
                        mouseX = event->motion.x;
                        mouseY = event->motion.y;
                        click = true;
                    }
                    break;
            }
        }
        if (rankShowB) {
            if (modeRankShow == 0) {
                sort(data.begin(), data.end(), compareDataS0);
            } else if (modeRankShow == 1) {
                sort(data.begin(), data.end(), compareDataS1);
            } else if (modeRankShow == 2) {
                sort(data.begin(), data.end(), compareDataS2);
            }

            rankShowB = false;
            SDL_Texture *img = NULL;
            if (page == 1) {
                if (modeRankShow == 0) {
                    img = IMG_LoadTexture(renderer, "../images/2.1.0.png");
                } else if (modeRankShow == 1) {
                    img = IMG_LoadTexture(renderer, "../images/2.1.1.png");
                } else if (modeRankShow == 2) {
                    img = IMG_LoadTexture(renderer, "../images/2.1.2.png");
                }
            } else {
                if (modeRankShow == 0) {
                    img = IMG_LoadTexture(renderer, "../images/2.2.0.png");
                } else if (modeRankShow == 1) {
                    img = IMG_LoadTexture(renderer, "../images/2.2.1.png");
                } else if (modeRankShow == 2) {
                    img = IMG_LoadTexture(renderer, "../images/2.2.2.png");
                }
            }

            SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255 );
            SDL_RenderClear( renderer );
            renderImg(renderer, img, 0, 0, W, H, false);
            renderText(to_string(page).c_str(), 30, 1, renderer, 355 * W / 1920, 155 * H / 1080, {255, 242, 0, 255});

            int j = 0;
            for (int i = (page - 1) * 7; i < (page) * 7; i++) {
                if (i < data.size() && data[i].name != "") {
                    text = to_string(i + 1) + ". " + data[i].name + " : " + to_string(data[i].score[modeRankShow]);
                    renderText(text.c_str(), 40, 1, renderer, 85 * W / 1920, 240 * H / 1080 + j * H / 10,
                               {255, 255, 255, 255});
                }
                j++;
            }
            for (auto user: data) {
                if (user.name == username && user.name != "") {
                    text = "your score : " + to_string(user.score[modeRankShow]);
                    renderText(text.c_str(), 40, 1, renderer, W / 2 + 200 * W / 1920, 327 * H / 1080,
                               {255, 242, 0, 255});
                }
            }
            int sum = 0;
            for (auto user: data) {
                if (user.name != "") {
                    sum += user.score[modeRankShow];
                }
            }
            sum = sum / data.size();
            text = "number of players : " + to_string(data.size());
            renderText(text.c_str(), 40, 1, renderer, W / 2 + 200 * W / 1920, 544 * H / 1080, {255, 242, 0, 255});
            text = "Average score: " + to_string(sum);
            renderText(text.c_str(), 40, 1, renderer, W / 2 + 200 * W / 1920, 756 * H / 1080, {255, 242, 0, 255});

            SDL_Delay(50);
        }

        if (click && 1861 * W / 1920 < mouseX && mouseY < 63 * H / 1080) {
            sec = 1;
            presec = 4;
            rankShowB = true;
            running = false;
        } else if (click && buttonClicked(W, H, mouseX, mouseY, 71, 175, 29)) {
            if (page != 1) {
                page--;
            } else {
                if (data.size() % 7 == 0) {
                    page = data.size() / 7;
                } else {
                    page = data.size() / 7 + 1;
                }
            }
            rankShowB = true;
        } else if (click && buttonClicked(W, H, mouseX, mouseY, 144, 178, 29)) {
            if (data.size() / 7 != 0 && !(page == data.size() / 7 && data.size() % 7 == 0) && page <= data.size() / 7) {
                page++;
            } else {
                page = 1;
            }
            rankShowB = true;
        } else if (click && buttonClicked(W, H, mouseX, mouseY, 81, 81, 42)) {
            if (modeRankShow != 2) {
                modeRankShow++;
            } else {
                modeRankShow = 0;
            }
            rankShowB = true;
        }

        SDL_RenderPresent(renderer);
        click = false;
    }
}
void settingShow(SDL_Renderer *renderer, double W, double H , vector<User>& data){
    for(auto& user : data) {
        if (user.name == username) {
            double maxVolume = 409 * H / 1080;
            if(user.musicVolume == 409 && user.chunkVolume == 409){
                user.musicVolume = maxVolume ;
                user.chunkVolume = maxVolume;
            }

            Mix_Chunk *chunk = Mix_LoadWAV("../chunks/sword.wav");
            SDL_Event *event = new SDL_Event();

            double whichThemeY;
            double whichMusicY;
            bool running = true;
            double mouseX, mouseY;
            bool click = false;

            double buttonMusicY = 965*H/1080 - user.musicVolume , buttonChunkY = 965*H/1080 - user.chunkVolume;
            int flagMusic = 0, flagChunk = 0;
            SDL_Texture *img = NULL;
            if(theme==1) {
                img = IMG_LoadTexture(renderer, "../images/6.png");
            }else{
                img = IMG_LoadTexture(renderer, "../images/g5.png");
            }
            bool clickedE = false;
            while (running) {
                SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255 );
                SDL_RenderClear( renderer );
                renderImg(renderer, img, 0, 0, W, H, false);
                if (SDL_PollEvent(event)) {
                    switch (event->type) {
                        case SDL_MOUSEBUTTONDOWN:
                            if (event->button.button == SDL_BUTTON_LEFT) {
                                click = true;
                            }
                            break;
                        case SDL_KEYDOWN:
                            switch (event->key.keysym.sym) {
                                case (SDLK_ESCAPE) :
                                    sec = 1;
                                    presec = 5;
                                    running = false;
                                    //updateFile("E:\\Term1\\C++_mabani\\BB\\files\\data.csv", data);
                                    break;
                            }
                        case SDL_MOUSEMOTION:
                            mouseX = event->motion.x;
                            mouseY = event->motion.y;
                            break;
                    }
                }
                if (!clickedE && click && buttonClicked(W,H,mouseX, mouseY, 614 , buttonMusicY * 1080 / H, 30)) {
                    flagMusic++;
                    clickedE = true;
                }
                if (flagMusic == 1) {
                    if (mouseY < 970 * H / 1080 && 565 * H / 1080 < mouseY) {
                        buttonMusicY = mouseY;
                        user.musicVolume = 970 * H / 1080 - buttonMusicY;
                        if (user.musicVolume <= 10) {
                            user.musicStop = 1;
                            Mix_PauseMusic();
                        }else {
                            Mix_ResumeMusic();
                            user.musicStop=0;
                            Mix_VolumeMusic(MIX_MAX_VOLUME * user.musicVolume / maxVolume);
                        }
                    }
                    filledEllipseRGBA(renderer, 614 * W / 1920, buttonMusicY, 30, 30, 255, 242, 0, 200);
                } else {
                    clickedE = false;
                    flagMusic = 0;
                    filledEllipseRGBA(renderer, 614 * W / 1920, buttonMusicY, 30, 30, 255, 242, 0, 255);
                }
                if (user.musicVolume <= 10) {
                    lineRGBA(renderer, 500 * W / 1920, 204 * H / 1080, 723 * W / 1920, 402 * H / 1080, 255, 0, 0, 255);
                }

                if (!clickedE && click && buttonClicked(W , H , mouseX, mouseY, 926 , buttonChunkY * 1080 / H, 30)) {
                    Mix_PlayChannel(1, chunk, 0);
                    flagChunk++;
                    clickedE = true;
                }
                if (flagChunk == 1) {
                    if (mouseY < 970 * H / 1080 && 565 * H / 1080 < mouseY) {
                        buttonChunkY = mouseY;
                        user.chunkVolume = 970 * H / 1080 - buttonChunkY;
                        if (user.chunkVolume <= 10) {
                            user.chunkVolume = 0;
                        }
                        Mix_Volume(-1, MIX_MAX_VOLUME * user.chunkVolume / maxVolume);
                    }
                    filledEllipseRGBA(renderer, 926 * W / 1920, buttonChunkY, 30, 30, 255, 242, 0, 200);
                } else {
                    clickedE = false;
                    flagChunk = 0;
                    filledEllipseRGBA(renderer, 926 * W / 1920, buttonChunkY, 30, 30, 255, 242, 0, 255);
                }
                if (user.chunkVolume <= 10) {
                    lineRGBA(renderer, 820 * W / 1920, 206 * H / 1080, 1021 * W / 1920, 425 * H / 1080, 255, 0, 0, 255);
                }
                for (auto &user: data) {
                    if (user.name == username) {
                        switch (user.music) {
                            case 1:
                                whichMusicY = 595 * H / 1080;
                                break;
                            case 2:
                                whichMusicY = 758 * H / 1080;
                                break;
                            case 3:
                                whichMusicY = 930 * H / 1080;
                                break;
                        }
                    }
                }
                for (auto &user: data) {
                    if (user.name == username) {
                        switch (user.music) {
                            case 1:
                                whichMusicY = 595 * H / 1080;
                                break;
                            case 2:
                                whichMusicY = 758 * H / 1080;
                                break;
                            case 3:
                                whichMusicY = 930 * H / 1080;
                                break;
                        }
                        switch (user.theme) {
                            case 1:
                                whichThemeY = 595 * H / 1080;
                                break;
                            case 2:
                                whichThemeY = 758 * H / 1080;
                                break;
                        }
                    }
                }
                if (!clickedE && click && buttonClicked(W , H , mouseX, mouseY, 1160 , 595 , 44)) {
                    music = Mix_LoadMUS("../musics/mainmusic.mp3");
                    for (auto &user: data) {
                        if (user.name == username) {
                            user.music = 1;
                        }
                    }
                    Mix_PlayMusic(music, 1000);
                    whichMusicY = 595 * H / 1080;
                } else if (!clickedE && click && buttonClicked(W , H , mouseX, mouseY, 1160 , 758 , 44)) {
                    music = Mix_LoadMUS("../musics/gole.mp3");
                    for (auto &user: data) {
                        if (user.name == username) {
                            user.music = 2;
                        }
                    }
                    Mix_PlayMusic(music, 1000);
                    whichMusicY = 758 * H / 1080;
                } else if (!clickedE && click && buttonClicked(W , H , mouseX, mouseY, 1160 , 930 , 44)) {
                    music = Mix_LoadMUS("../musics/ow.mp3");
                    for (auto &user: data) {
                        if (user.name == username) {
                            user.music = 3;
                        }
                    }
                    Mix_PlayMusic(music, 1000);
                    whichMusicY = 930 * H / 1080;
                }else if (!clickedE && click && 1861 * W / 1920 < mouseX && mouseY < 63 * H / 1080) {
                    sec = 1;
                    presec = 5;
                    running = false;
                }else if (user.musicVolume > 10 && !clickedE && click && buttonClicked(W , H , mouseX, mouseY, 614 , 301 , 149)) {
                    if(user.musicStop){
                        user.musicStop = 0;
                        Mix_ResumeMusic();
                    }else {
                        user.musicStop=1;
                        Mix_PauseMusic();
                    }
                }else if (!clickedE && click && buttonClicked(W , H , mouseX, mouseY, 295 , 595 , 44)) {
                    theme = user.theme = 1;
                    whichThemeY = 595 * H / 1080 ;
                    running = false;
                }else if (!clickedE && click && buttonClicked(W , H , mouseX, mouseY, 295 , 758 , 44)) {
                    theme = user.theme = 2;
                    whichThemeY = 758 * H / 1080 ;
                    running = false;
                }
                if (user.musicStop) {
                    lineRGBA(renderer, 500 * W / 1920, 204 * H / 1080, 723 * W / 1920, 402 * H / 1080, 255, 0, 0, 255);
                }
                filledEllipseRGBA(renderer, 1160 * W / 1920, whichMusicY, 20, 20, 255, 242, 0, 255);
                filledEllipseRGBA(renderer, 295 * W / 1920, whichThemeY, 20, 20, 255, 242, 0, 255);
                click = false;
                SDL_RenderPresent(renderer);
            }
        }
    }
}
void pauseMenu(SDL_Renderer *renderer, double W, double H , vector<User>& data){
    for(auto& user : data) {
        if (user.name == username) {
            double maxVolume = 409 * H / 1080;
            if(user.musicVolume == 409 && user.chunkVolume == 409){
                user.musicVolume = maxVolume ;
                user.chunkVolume = maxVolume;
            }
            Mix_Chunk *chunk = Mix_LoadWAV("../chunks/sword.wav");
            SDL_Event *event = new SDL_Event();
            double whichMusicY;
            bool running = true;
            double mouseX, mouseY;
            bool click = false;
            double buttonMusicY = 965*H/1080 - user.musicVolume , buttonChunkY = 965*H/1080 - user.chunkVolume;
            int flagMusic = 0, flagChunk = 0;
            SDL_Texture *img = NULL;
            if(theme==1) {
                img = IMG_LoadTexture(renderer, "../images/7.png");
            }else{
                img = IMG_LoadTexture(renderer, "../images/g6.png");
            }
            bool clickedE = false;
            while (running) {
                SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255 );
                SDL_RenderClear( renderer );
                renderImg(renderer, img, 0, 0, W, H, false);
                if (SDL_PollEvent(event)) {
                    switch (event->type) {
                        case SDL_MOUSEBUTTONDOWN:
                            if (event->button.button == SDL_BUTTON_LEFT) {
                                click = true;
                            }
                            break;
                        case SDL_KEYDOWN:
                            switch (event->key.keysym.sym) {
                                case (SDLK_ESCAPE) :
                                    sec = 1;
                                    presec = 5;
                                    running = false;
                                    //updateFile("E:\\Term1\\C++_mabani\\BB\\files\\data.csv", data);
                                    break;
                            }
                        case SDL_MOUSEMOTION:
                            mouseX = event->motion.x;
                            mouseY = event->motion.y;
                            break;
                    }
                }

                if (!clickedE && click && buttonClicked(W,H,mouseX, mouseY, 614 , buttonMusicY * 1080 / H, 30)) {
                    flagMusic++;
                    clickedE = true;
                }
                if (flagMusic == 1) {
                    if (mouseY < 970 * H / 1080 && 565 * H / 1080 < mouseY) {
                        buttonMusicY = mouseY;
                        user.musicVolume = 970 * H / 1080 - buttonMusicY;
                        if (user.musicVolume <= 10) {
                            user.musicStop = 1;
                            Mix_PauseMusic();
                        }else {
                            Mix_ResumeMusic();
                            user.musicStop=0;
                            Mix_VolumeMusic(MIX_MAX_VOLUME * user.musicVolume / 409);
                        }
                    }
                    filledEllipseRGBA(renderer, 614 * W / 1920, buttonMusicY, 30, 30, 255, 242, 0, 200);
                } else {
                    clickedE = false;
                    flagMusic = 0;
                    filledEllipseRGBA(renderer, 614 * W / 1920, buttonMusicY, 30, 30, 255, 242, 0, 255);
                }
                if (user.musicVolume <= 10) {
                    lineRGBA(renderer, 500 * W / 1920, 204 * H / 1080, 723 * W / 1920, 402 * H / 1080, 255, 0, 0, 255);
                }
                if (!clickedE && click && buttonClicked(W , H , mouseX, mouseY, 926 , buttonChunkY * 1080 / H, 30)) {
                    Mix_PlayChannel(1, chunk, 0);
                    flagChunk++;
                    clickedE = true;
                }
                if (flagChunk == 1) {
                    if (mouseY < 970 * H / 1080 && 565 * H / 1080 < mouseY) {
                        buttonChunkY = mouseY;
                        user.chunkVolume = 970 * H / 1080 - buttonChunkY;
                        if (user.chunkVolume <= 10) {
                            user.chunkVolume = 0;
                        }
                        Mix_Volume(-1, MIX_MAX_VOLUME * user.chunkVolume / 409);
                    }
                    filledEllipseRGBA(renderer, 926 * W / 1920, buttonChunkY, 30, 30, 255, 242, 0, 200);
                } else {
                    clickedE = false;
                    flagChunk = 0;
                    filledEllipseRGBA(renderer, 926 * W / 1920, buttonChunkY, 30, 30, 255, 242, 0, 255);
                }
                if (user.chunkVolume <= 10) {
                    lineRGBA(renderer, 820 * W / 1920, 206 * H / 1080, 1021 * W / 1920, 425 * H / 1080, 255, 0, 0, 255);
                }
                for (auto &user: data) {
                    if (user.name == username) {
                        switch (user.music) {
                            case 1:
                                whichMusicY = 595 * H / 1080;
                                break;
                            case 2:
                                whichMusicY = 758 * H / 1080;
                                break;
                            case 3:
                                whichMusicY = 930 * H / 1080;
                                break;
                        }
                    }
                }
                if (!clickedE && click && buttonClicked(W , H , mouseX, mouseY, 1160 , 595 , 44)) {
                    music = Mix_LoadMUS("../musics/mainmusic.mp3");
                    for (auto &user: data) {
                        if (user.name == username) {
                            user.music = 1;
                        }
                    }
                    Mix_PlayMusic(music, 1000);
                    whichMusicY = 595 * H / 1080;
                } else if (!clickedE && click && buttonClicked(W , H , mouseX, mouseY, 1160 , 758 , 44)) {
                    music = Mix_LoadMUS("../musics/gole.mp3");
                    for (auto &user: data) {
                        if (user.name == username) {
                            user.music = 2;
                        }
                    }
                    Mix_PlayMusic(music, 1000);
                    whichMusicY = 758 * H / 1080;
                } else if (!clickedE && click && buttonClicked(W , H , mouseX, mouseY, 1160 , 930 , 44)) {
                    music = Mix_LoadMUS("../musics/ow.mp3");
                    for (auto &user: data) {
                        if (user.name == username) {
                            user.music = 3;
                        }
                    }
                    Mix_PlayMusic(music, 1000);
                    whichMusicY = 930 * H / 1080;
                }else if (!clickedE && click && 1861 * W / 1920 < mouseX && mouseY < 63 * H / 1080) {
                    sec = 1;
                    presec = 5;
                    running = false;
                }else if (user.musicVolume > 10 && !clickedE && click && buttonClicked(W , H , mouseX, mouseY, 614 , 301 , 149)) {
                    if(user.musicStop){
                        user.musicStop = 0;
                        Mix_ResumeMusic();
                    }else {
                        user.musicStop=1;
                        Mix_PauseMusic();
                    }
                }else if (!clickedE && click && buttonClicked(W,H,mouseX, mouseY, 293 , 315 , 159)) {
                    sec = 50;
                    presec = 6;
                    running = false;
                } else if (!clickedE && click && buttonClicked(W,H,mouseX, mouseY, 1582, 315 , 159)) {
                    sec = 1;
                    presec = 6;
                    running = false;
                }
                if (user.musicStop) {
                    lineRGBA(renderer, 500 * W / 1920, 204 * H / 1080, 723 * W / 1920, 402 * H / 1080, 255, 0, 0, 255);
                }
                filledEllipseRGBA(renderer, 1160 * W / 1920, whichMusicY, 20, 20, 255, 242, 0, 255);
                click = false;
                SDL_RenderPresent(renderer);
            }
        }
    }
}
bool beforeGameMenuB = true;
double xBGS1 = 354 , yBGS1 = 214;
double xBGS21 = -100 , yBGS21 = -100;
double xBGS22 = -100 , yBGS22 = -100;
double xBGS23 = -100 , yBGS23 = -100;
double xBGS24 = -100 , yBGS24 = -100;
double xBGS25 = -100 , yBGS25 = -100;
double xBGS26 = -100 , yBGS26 = -100;
double xBGS27 = -100 , yBGS27 = -100;
double xBGS3 = 825 , yBGS3 = 582;
double xBGS4 = 565 , yBGS4 = 728;
bool normalModeGame = false;
void beforeGameMenu(SDL_Renderer *renderer, int W, int H) {

    SDL_Event *event = new SDL_Event();
    bool running = true;
    int flag2 = 0;
    double mouseX , mouseY;
    bool click = false;
    SDL_Texture *img = NULL;
    if(theme==1) {
        img = IMG_LoadTexture(renderer, "../images/10.png");
    }else{
        img = IMG_LoadTexture(renderer, "../images/g8.png");
    }
    while(running) {
        if (SDL_PollEvent(event)) {
            switch (event->type) {
                case SDL_MOUSEBUTTONDOWN:
                    if (event->button.button == SDL_BUTTON_LEFT) {
                        mouseX = event->motion.x;
                        mouseY = event->motion.y;
                        click = true;
                    }
                    break;
            }
        }

        renderImg(renderer, img, 0, 0, W, H, false);

        if (click && buttonClicked(W, H, mouseX, mouseY, 354, 214, 17)) { //m
            play_mode = 1;
            xBGS1 = 354;
            yBGS1 = 214;
            //beforeGameMenuB = true;
        } else if (click && buttonClicked(W, H, mouseX, mouseY, 585, 214, 17)) { //m
            play_mode = 6;
            xBGS1 = 585;
            yBGS1 = 214;
            //beforeGameMenuB = true;
        } else if (click && buttonClicked(W, H, mouseX, mouseY, 814, 214, 17)) {//m
            normalModeGame = true;
            xBGS1 = 814;
            yBGS1 = 214;
            //beforeGameMenuB = true;
        } else if (click && buttonClicked(W, H, mouseX, mouseY, 455, 353, 17)) {//f
            mega_bomb_ball_allowed = true;
            xBGS21 = 455;
            yBGS21 = 353;
            //beforeGameMenuB = true;
        } else if (click && buttonClicked(W, H, mouseX, mouseY, 814, 357, 17)) {//f
            laser_ball_allowed = true;
            xBGS22 = 814;
            yBGS22 = 357;
            //beforeGameMenuB = true;
        } else if (click && buttonClicked(W, H, mouseX, mouseY, 1046, 357, 17)) {//f
            freeze_ball_allowed = true;
            xBGS23 = 1046;
            yBGS23 = 357;
            //beforeGameMenuB = true;
        } else if (click && buttonClicked(W, H, mouseX, mouseY, 1426, 357, 17)) {//f
            multi_color_flying_ball = true;
            xBGS24 = 1426;
            yBGS24 = 357;
            //beforeGameMenuB = true;
        } else if (click && buttonClicked(W, H, mouseX, mouseY, 452, 438, 17)) {//f
            locked_ball = true;
            xBGS25 = 452;
            yBGS25 = 438;
            //beforeGameMenuB = true;
        } else if (click && buttonClicked(W, H, mouseX, mouseY, 854, 438, 17)) {//f
            multicolor_ball = true;
            xBGS26 = 854;
            yBGS26 = 438;
            //beforeGameMenuB = true;
        } else if (click && buttonClicked(W, H, mouseX, mouseY, 1426, 438, 17)) {//f
            gray_balls_exist = true;
            xBGS27 = 1426;
            yBGS27 = 438;
            //beforeGameMenuB = true;
        } else if (click && buttonClicked(W, H, mouseX, mouseY, 472, 582, 17)) {//d
            play_mode = 5;
            xBGS3 = 472;
            yBGS3 = 582;
            //beforeGameMenuB = true;
        } else if (click && buttonClicked(W, H, mouseX, mouseY, 825, 582, 17)) {//d
            play_mode = 4;
            xBGS3 = 825;
            yBGS3 = 582;
            //beforeGameMenuB = true;
        } else if (click && buttonClicked(W, H, mouseX, mouseY, 1076, 582, 17)) {//d
            play_mode = 3;
            xBGS3 = 1076;
            yBGS3 = 582;
            //beforeGameMenuB = true;
        } else if (click && buttonClicked(W, H, mouseX, mouseY, 1395, 582, 17)) {//d
            play_mode = 2;
            xBGS3 = 1395;
            yBGS3 = 582;
            //beforeGameMenuB = true;
        } else if (click && buttonClicked(W, H, mouseX, mouseY, 565, 728, 17)) {//b
            backgroundImgS = "../images/blue.jpg";
            xBGS4 = 565;
            yBGS4 = 728;
            //beforeGameMenuB = true;
        } else if (click && buttonClicked(W, H, mouseX, mouseY, 1038, 728, 17)) {//b
            backgroundImgS = "../images/orange.jpg";
            xBGS4 = 1038;
            yBGS4 = 728;
            //beforeGameMenuB = true;
        } else if (click && buttonClicked(W, H, mouseX, mouseY, 1494, 728, 17)) {//b
            backgroundImgS = "../images/pink.jpg";
            xBGS4 = 1494;
            yBGS4 = 728;
            //beforeGameMenuB = true;
        } else if (click && 1263 * W / 1920 < mouseX && mouseX < 1530 * W / 1920 && mouseY < 182 * H / 1080) {
            sec = 49;
            presec = 8;
            running = false;
            //beforeGameMenuB = true;
        } else if (click && 1861 * W / 1920 < mouseX && mouseY < 63 * H / 1080) {
            sec = 1;
            presec = 8;
            running = false;
            //beforeGameMenuB = true;
        }
        filledEllipseRGBA(renderer, xBGS1 * W / 1920, yBGS1 * H / 1080, 15 / 2, 15 / 2, 255, 242, 0, 255);
        filledEllipseRGBA(renderer, xBGS21 * W / 1920, yBGS21 * H / 1080, 15 / 2, 15 / 2, 255, 242, 0, 255);
        filledEllipseRGBA(renderer, xBGS22 * W / 1920, yBGS22 * H / 1080, 15 / 2, 15 / 2, 255, 242, 0, 255);
        filledEllipseRGBA(renderer, xBGS23 * W / 1920, yBGS23 * H / 1080, 15 / 2, 15 / 2, 255, 242, 0, 255);
        filledEllipseRGBA(renderer, xBGS24 * W / 1920, yBGS24 * H / 1080, 15 / 2, 15 / 2, 255, 242, 0, 255);
        filledEllipseRGBA(renderer, xBGS25 * W / 1920, yBGS25 * H / 1080, 15 / 2, 15 / 2, 255, 242, 0, 255);
        filledEllipseRGBA(renderer, xBGS26 * W / 1920, yBGS26 * H / 1080, 15 / 2, 15 / 2, 255, 242, 0, 255);
        filledEllipseRGBA(renderer, xBGS27 * W / 1920, yBGS27 * H / 1080, 15 / 2, 15 / 2, 255, 242, 0, 255);
        filledEllipseRGBA(renderer, xBGS3 * W / 1920, yBGS3 * H / 1080, 15 / 2, 15 / 2, 255, 242, 0, 255);
        filledEllipseRGBA(renderer, xBGS4 * W / 1920, yBGS4 * H / 1080, 15 / 2, 15 / 2, 255, 242, 0, 255);

        SDL_RenderPresent(renderer);
    }
}
bool goverMenuB = true;
void goverMenu(SDL_Renderer *renderer, int W, int H, bool &click,int mouseX, int mouseY , vector<User>& data) {
    if (goverMenuB) {
        SDL_Texture *img = NULL;
        if(theme==1) {
            img = IMG_LoadTexture(renderer, "../images/8.png");
        }else{
            img = IMG_LoadTexture(renderer, "../images/g2.png");
        }
        renderImg(renderer, img, 0, 0, W, H,true);
        for(auto& user : data){
            if(user.name == username){
                if(play_mode == 6){
                    if(total_score>user.score[1])
                        user.score[1] =   total_score;
                }else if(normalModeGame){
                    if(total_score>user.score[0])
                        user.score[0] =   total_score;
                }else if(play_mode == 1){
                    if(total_score>user.score[2])
                        user.score[2] =   total_score;
                }
                //updateFile("E:\\Term1\\C++_mabani\\BB\\files\\data.csv",data);
            }
        }
        string text = to_string(int(  total_score));
        Render_text(text.c_str(), 100, 1, renderer, 960 * W /1920, 300 * H / 1080, {255, 242, 0, 255});

        goverMenuB = false;
    } else if (click && buttonClicked(W,H,mouseX, mouseY, 641 , 720 , 180)) {
        sec = 49;
        presec = 2;
        goverMenuB = true;
    } else if (click && buttonClicked(W,H,mouseX, mouseY, 1273, 720, 180)) {
        sec = 1;
        presec = 2;
        goverMenuB = true;
        //Mix_CloseAudio();
    }
}
bool gwinMenuB = true;
void gwinMenu(SDL_Renderer *renderer, int W, int H, bool &click,int mouseX, int mouseY , vector<User>& data) {
    if (gwinMenuB) {
        SDL_Texture *img = NULL;
        if(theme==1) {
            img = IMG_LoadTexture(renderer, "../images/9.png");
        }else{
            img = IMG_LoadTexture(renderer, "../images/g7.png");
        }
        renderImg(renderer, img, 0, 0, W, H,true);
        for(auto& user : data){
            if(user.name == username){
                if(play_mode == 6){
                    if(total_score>user.score[1])
                        user.score[1] =   total_score;
                }else if(normalModeGame){
                    if(total_score>user.score[0])
                        user.score[0] =   total_score;
                }else if(play_mode == 1){
                    if(total_score>user.score[2])
                        user.score[2] =   total_score;
                }
            }
        }
        string text = to_string(int(  total_score));
        Render_text(text.c_str(), 100, 1, renderer, 960 * W /1920, 300 * H / 1080, {255, 242, 0, 255});

        gwinMenuB = false;
    } else if (click && buttonClicked(W,H,mouseX, mouseY, 641 , 720 , 180)) {
        sec = 49;
        presec = 7;
        gwinMenuB = true;
    } else if (click && buttonClicked(W,H,mouseX, mouseY, 1273, 720, 180)) {
        sec = 8;
        presec = 7;
        gwinMenuB = true;
    }
}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    IMG_Init(IMG_INIT_JPG);
    SDL_Texture *img = NULL;
    vector<User> data;
    readData("../files/data.csv" , data);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 1, 2048);
    Mix_Chunk* sound;
    clock_t NOW = clock();
    bool playing = false;
    bool started = false;
    bool click = false;
    bool flag_fps = true;
    bool collision_success = false ;
    bool falling = false;
    bool freeze_apply = false;
    bool laser_apply = false;
    bool laser_applied = false;
    bool mega_bomb_apply = false;
    bool mega_bomb_applied = false;
    bool freeze_ball_available = false;
    bool laser_ball_available = false;
    bool mega_bomb_ball_available = false;
    bool mega_bomb_explosion = false;
    bool flag_changed_vel_go_down = false;
    int collision_success_timer = 0;
    int mega_phrase = 0;
    int previous_mega_phrase = 0;
    int freeze_ball_counter = 0;
    int laser_ball_counter = 0;
    int mega_bomb_ball_counter = 0;
    int mega_bomb_after_explosion_time_counter = 0;
    int wgo_laser = 0 ;
    int freeze_time_counter = 0;
    int laser_time_counter = 0;
    int mega_bomb_time_counter = 0 ;
    int color_temp_r ;
    int color_temp_g ;
    int color_temp_b ;
    int fps_cycle_tracker = 0 ;
    int fps_cycle_counter = 0 ;
    float laser_foot_print_x ;
    float laser_foot_print_y ;
    float mega_bomb_print_x ;
    float mega_bomb_print_y ;
    double time_fps = 1000/double(fps) ;
    double t ;
    double mean_t = 0;
    double vel_x_laser ;
    double vel_y_laser ;
    double vel_x_mega_bomb ;
    double vel_y_mega_bomb ;
    double prev_vel_balls_go_down ;
    clock_t freeze_apply_t ;
    clock_t collision_success_t ;
    clock_t laser_applied_t ;
    clock_t freeze_ball_t = clock();
    clock_t laser_ball_t = clock();
    clock_t mega_ball_t = clock();
    clock_t mega_bomb_explosion_t ;
    int random_number_temp_color = rand() % 8 ;

    color2d colors[100];
    colors[0] = {230, 25, 76}; // red
    colors[1] = {60, 180, 75}; // green
    colors[2] = {255, 225, 25}; // yellow
    colors[3] = {67, 99, 216}; // blue
    colors[4] = {245, 130, 49}; // orange
    colors[5] = {145, 30, 180}; // purple
    colors[6] = {66, 212, 244}; // cyan
    colors[7] = {240, 50, 230}; // magenta
    colors[8] = {100, 100, 100};// gray
    colors[9] = {54, 139, 193}; //freeze ball
    colors[10] = {255 ,255 ,102}; // laser ball
    colors[11] = {255 ,0 ,0}; // mega bomb ball

    color2d mega_bomb_color[4];
    mega_bomb_color[0] = {255 , 75 , 78}; // phrase zer0
    mega_bomb_color[1] = {255 , 75 , 78}; // phrase one
    mega_bomb_color[2] = {120 , 6 , 255}; // phrase two
    mega_bomb_color[3] = {48 , 10 , 10}; // phrase three

    flying_color2d flying_color ;
    next_ball_color2d next_ball_color ;
    vector <array<int , 2> > output ;
    vector <array<int , 2> > output1 ;
    vector <array<int , 2> > changed_balls ;
    vector <array<float , 3> > falling_balls ; // i , j , vel
    vector <array<int , 2> > falling_balls_vel ;
    long long int cycle_counter = 0 ;
    int new_ball_row = 0 , new_ball_column = 0 ;

    ball2d balls_out[n_rows_out][n_columns];
    ball2d balls[n_rows][n_columns];
    ball2d balls_falling[n_rows][n_columns];
    int mouseX;
    int mouseY;
    bool running = true;
    bool bullet_is_flying = false  , flag1 = true , collided = false ;
    int color_counter1 ;

    //------------------------< configuration
    //Initialization of SDL windows
    Uint32 SDL_flags = SDL_INIT_EVERYTHING;//SDL_INIT_VIDEO | SDL_INIT_TIMER;
    Uint32 WND_flags = SDL_WINDOW_INPUT_FOCUS|SDL_WINDOW_FULLSCREEN_DESKTOP;
    SDL_Window *m_window;
    SDL_Renderer *m_renderer;
    SDL_Init(SDL_flags);
    SDL_CreateWindowAndRenderer(1920, 480, WND_flags, &m_window, &m_renderer);
    //Pass the focus to the drawing window
    SDL_RaiseWindow(m_window);
    //Get screen resolution
    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);
    int W = DM.w;
    int H = DM.h;
    int R = int(W / (n_columns * 2 + 1));
    //int W_min = (W-(R*(n_columns*2+1)))/2;
    //int W_max = (W-(R*(n_columns*2+1)))/2+((R*(n_columns*2+1)));
    //------------------------> configuration
    double deadline_y = (n_rows-2) * sqrt(3) * R ;
    bool wanna_beam = false;
    ball2d flying_bullet;
    ball2d laser_foot_print;
    flying_bullet.x = W/2;
    flying_bullet.y = H - R;

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ S
    SDL_Event *event = new SDL_Event();
    while (running) {
        clock_t now = clock();
        while(SDL_PollEvent(event)) {
            switch (event->type) {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_KEYDOWN:
                    switch (event->key.keysym.sym) {
                        case (SDLK_ESCAPE) :
                            if(sec == 4){
                                sec = 1;
                                presec =4;
                                rankShowB = true;
                            }else {
                                running = false;
                            }
                            break;
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if(event->button.button == SDL_BUTTON_LEFT && mouseY < H-2*R && !laser_apply && !mega_bomb_apply) {//normal left click
                        if (!bullet_is_flying) {
                            mouseX = event->motion.x;
                            mouseY = event->motion.y;
                            wanna_beam = true;
                        }
                        click = true;
                    }else if(event->button.button == SDL_BUTTON_LEFT && (mouseX -W * 0.95 )*(mouseX -W * 0.95 ) + ( mouseY - (H-R)) * ( mouseY - (H-R)) <= R * R){
                        sec = 6 ;
                        presec = 50 ;
                    }else if(event->button.button == SDL_BUTTON_LEFT && mouseY < H*0.6 && laser_apply){//laser beaming click
                        if (!bullet_is_flying) {
                            laser_foot_print.rgb[0] = flying_color.r ;
                            laser_foot_print.rgb[1] = flying_color.g ;
                            laser_foot_print.rgb[2] = flying_color.b ;
                            laser_foot_print_x = flying_bullet.x ;
                            laser_foot_print_y = flying_bullet.y ;
                            double u_h1 , u_l1 , u_ch1 , urx , ury;
                            u_l1 =  mouseX - flying_bullet.x; //u_length1
                            u_h1 =  mouseY - flying_bullet.y; //u_height1
                            u_ch1 = sqrt(pow(u_h1,2)+pow(u_l1,2)); //u_chord1
                            urx = (u_l1 / u_ch1); //u_ratio_x
                            ury = (u_h1 / u_ch1); //u_ratio_y
                            vel_x_laser = urx * vel_flying_ball * R;
                            vel_y_laser = ury * vel_flying_ball * R;
                            mouseX = event->motion.x;
                            mouseY = event->motion.y;
                            wgo_laser = 0 ;
                            laser_ball_beaming(m_renderer, W, H, mouseX, mouseY , vel_x_laser , vel_y_laser , flying_color , colors , flying_bullet , balls , output);
                            flying_color.r = color_temp_r;
                            flying_color.g = color_temp_g;
                            flying_color.b = color_temp_b;
                            laser_apply = false ;
                            laser_applied = true ;
                            //sound laser
                            sound = Mix_LoadWAV("../chunks/laser.mp3");
                            Mix_PlayChannel(-1, sound, 0);
                            laser_applied_t = clock() ;
                            laser_time_counter = 0 ;
                        }
                        click = true;
                    }else if(event->button.button == SDL_BUTTON_LEFT && mouseY < H - 2*R && mega_bomb_apply) {//mega bomb beaming click
                        if (!bullet_is_flying){
                            double u_h1 , u_l1 , u_ch1 , urx , ury;
                            u_l1 =  mouseX - flying_bullet.x; //u_length1
                            u_h1 =  mouseY - flying_bullet.y; //u_height1
                            u_ch1 = sqrt(pow(u_h1,2)+pow(u_l1,2)); //u_chord1
                            urx = (u_l1 / u_ch1); //u_ratio_x
                            ury = (u_h1 / u_ch1); //u_ratio_y
                            vel_x_mega_bomb = urx * vel_flying_ball * R * mega_ball_rel_speed;
                            vel_y_mega_bomb = ury * vel_flying_ball * R * mega_ball_rel_speed;
                            mouseX = event->motion.x;
                            mouseY = event->motion.y;
                            wgo_laser = 0 ;
                            mega_bomb_apply = false ;
                            mega_bomb_applied = true ;
                            mega_bomb_time_counter = 0 ;
                            mega_ball_t = clock() ;
                        }
                        click = true;
                    }else if(event->button.button == SDL_BUTTON_LEFT && !freeze_apply && freeze_ball_available && !bullet_is_flying && sqrt((mouseX-W*0.675)*(mouseX-W*0.675)+(mouseY-(H-R))*(mouseY-(H-R))) <= R){//freeze button
                        freeze_ball_available = false ;
                        freeze_ball_counter = 0 ;
                        freeze_time_counter = 0 ;
                        freeze_apply = true ;
                        //sound freeze
                        sound = Mix_LoadWAV("../chunks/freeze.mp3");
                        Mix_PlayChannel(-1, sound, 0);
                        freeze_apply_t = clock() ;
                        freeze_ball_t = clock() ;
                    }else if(event->button.button == SDL_BUTTON_LEFT && !laser_apply && !mega_bomb_apply && laser_ball_available && !bullet_is_flying && sqrt((mouseX-W*0.75)*(mouseX-W*0.75)+(mouseY-(H-R))*(mouseY-(H-R))) <= R){//laser button
                        laser_ball_available = false ;
                        laser_ball_counter = 0 ;
                        laser_apply = true ;
                        color_temp_r = flying_color.r ;
                        color_temp_g = flying_color.g ;
                        color_temp_b = flying_color.b ;
                        flying_color.r = 255 ;//colors[10].r ;
                        flying_color.g = 255 ;//colors[10].g ;
                        flying_color.b = 102 ;//colors[10].b ;
                        laser_ball_t = clock() ;
                    }else if(event->button.button == SDL_BUTTON_LEFT && !laser_apply && mega_bomb_ball_available && !bullet_is_flying && sqrt((mouseX-W*0.825)*(mouseX-W*0.825)+(mouseY-(H-R))*(mouseY-(H-R))) <= R){//mega bomb button
                        mega_bomb_ball_available = false ;
                        mega_bomb_apply  = true ;
                        mega_bomb_ball_counter = 0 ;
                        color_temp_r = flying_color.r ;
                        color_temp_g = flying_color.g ;
                        color_temp_b = flying_color.b ;
                        flying_color.r = mega_bomb_color[mega_phrase].r ;
                        flying_color.g = mega_bomb_color[mega_phrase].g ;
                        flying_color.b = mega_bomb_color[mega_phrase].b ;
                        //mega_ball_t = clock() ;
                    }else if(event->button.button == SDL_BUTTON_RIGHT && !mega_bomb_apply && !laser_apply) {//right click
                        if (!bullet_is_flying) {
                            if(next_ball_color.multi_color){
                                color_change_next_ball(flying_color, next_ball_color);
                                next_ball_color.multi_color = false ;
                                flying_color.multi_color = true ;
                            }else if(flying_color.multi_color){
                                color_change_next_ball(flying_color, next_ball_color);
                                next_ball_color.multi_color = true ;
                                flying_color.multi_color = false ;
                            }else
                                color_change_next_ball(flying_color, next_ball_color);
                        }
                    }
                    break;
                case SDL_MOUSEMOTION:
                    mouseX = event->motion.x;
                    mouseY = event->motion.y;
                    break;
            }
        }

        switch (sec) {
            case 0:
                img = IMG_LoadTexture(m_renderer, "../images/1.png");
                renderImg(m_renderer, img , 0, 0, W, H,true);
                SDL_Delay(2000);
                sec = 3;
                presec = 0;
                break;
            case 1:
                startMenu(m_renderer, W, H, mouseX, mouseY, click);
                break;
            case 2 :
                goverMenu(m_renderer, W, H, click, mouseX, mouseY ,data);
                break;
            case 3 :
                userNameMenu(m_renderer, W, H,data);
                break;
            case 4 :
                rankShow(m_renderer,W,H,data);
                break;
            case 5 :
                settingShow(m_renderer,W,H,data);
                break;
            case 6 :
                pauseMenu(m_renderer, W, H , data);
                break;
            case 7 :
                gwinMenu(m_renderer, W, H, click, mouseX, mouseY ,data);
                break;
            case 8 :
                normalModeGame = false;
                timer_mode_allowed = false ;
                gray_balls_exist = false ;
                multicolor_ball  = false ;
                locked_ball = false ;
                multi_color_flying_ball = false ;
                freeze_ball_allowed  = false ;
                laser_ball_allowed  = false ;
                mega_bomb_ball_allowed = false  ;
                xBGS21 = -100 , yBGS21 = -100;
                xBGS22 = -100 , yBGS22 = -100;
                xBGS23 = -100 , yBGS23 = -100;
                xBGS24 = -100 , yBGS24 = -100;
                xBGS25 = -100 , yBGS25 = -100;
                xBGS26 = -100 , yBGS26 = -100;
                xBGS27 = -100 , yBGS27 = -100;
                beforeGameMenu(m_renderer, W, H);
                break;
            case 49:
                NOW = clock();
                u_b1 = true; //unknown_bool1
                playing = false;
                started = false;
                click = false;
                flag_fps = true;
                collision_success = false ;
                falling = false;
                freeze_apply = false;
                laser_apply = false;
                laser_applied = false;
                mega_bomb_apply = false;
                mega_bomb_applied = false;
                freeze_ball_available = false;
                laser_ball_available = false;
                mega_bomb_ball_available = false;
                mega_bomb_explosion = false;
                flag_changed_vel_go_down = false;
                collision_success_timer = 0;
                mega_phrase = 0;
                previous_mega_phrase = 0;
                freeze_ball_counter = 0;
                laser_ball_counter = 0;
                mega_bomb_ball_counter = 0;
                mega_bomb_after_explosion_time_counter = 0;
                wgo_laser = 0 ;
                freeze_time_counter = 0;
                laser_time_counter = 0;
                mega_bomb_time_counter = 0 ;
                fps_cycle_tracker = 0 ;
                fps_cycle_counter = 0 ;
                time_fps = 1000/double(fps) ;
                mean_t = 0;
                freeze_ball_t = clock();
                laser_ball_t = clock();
                mega_ball_t = clock();
                cycle_counter = 0 ;
                new_ball_row = 0 , new_ball_column = 0 ;
                bullet_is_flying = false  , flag1 = true , collided = false ;
                total_score = 0;

                for (int i = 0; i < n_rows; ++i) {
                    for (int j = 0; j < n_columns; ++j) {
                        balls[i][j].x = 0 ;
                        balls[i][j].y = 0 ;
                        balls[i][j].rgb[0] = 0 ;
                        balls[i][j].rgb[1] = 0 ;
                        balls[i][j].rgb[2] = 0 ;
                        balls[i][j].rgb1[0] = 0 ;
                        balls[i][j].rgb1[1] = 0 ;
                        balls[i][j].rgb1[2] = 0 ;
                        balls[i][j].is_exist = false ;
                        balls[i][j].eo = 1 ;
                        balls[i][j].connect = false ;
                        balls[i][j].falling = false ;
                        balls[i][j].multicolor = false ;
                        balls[i][j].locked = false ;
                    }
                }
                for (int i = 0; i < n_rows_out; ++i) {
                    for (int j = 0; j < n_columns; ++j) {
                        balls_out[i][j].x = 0 ;
                        balls_out[i][j].y = 0 ;
                        balls_out[i][j].rgb[0] = 0 ;
                        balls_out[i][j].rgb[1] = 0 ;
                        balls_out[i][j].rgb[2] = 0 ;
                        balls_out[i][j].rgb1[0] = 0 ;
                        balls_out[i][j].rgb1[1] = 0 ;
                        balls_out[i][j].rgb1[2] = 0 ;
                        balls_out[i][j].is_exist = false ;
                        balls_out[i][j].eo = 1 ;
                        balls_out[i][j].connect = false ;
                        balls_out[i][j].falling = false ;
                        balls_out[i][j].multicolor = false ;
                        balls_out[i][j].locked = false ;
                    }
                }
                for (int i = 0; i < 11; ++i) {
                    colors[i].is_exist = false ;
                    colors[i].is_exist_temp = false ;
                }
                flying_color.multi_color = false ;
                next_ball_color.multi_color = false ;

                initial_balls_coordinate(m_renderer , W , H , balls);

                if(play_mode == 0) { // DEVELOPER
                    initializing_play_mode_0(m_renderer , W , H , balls , balls_out , colors , output , color);
                }else if(play_mode == 1){ // INFINITY
                    initializing_play_mode_1(m_renderer , W , H , balls , balls_out , colors , output , color);
                }else if(play_mode == 2){ // EASY
                    initializing_play_mode_2(m_renderer , W , H , balls , balls_out , colors , output , color);
                }else if(play_mode == 3){ // NORMAL
                    initializing_play_mode_3(m_renderer , W , H , balls , balls_out , colors , output , color);
                }else if(play_mode == 4){ // HARD
                    initializing_play_mode_4(m_renderer , W , H , balls , balls_out , colors , output , color);
                }else if(play_mode == 5){ // EXTREME
                    initializing_play_mode_5(m_renderer , W , H , balls , balls_out , colors , output , color);
                }else if(play_mode == 6){ // TIMER
                    initializing_play_mode_6(m_renderer , W , H , balls , balls_out , colors , output , color);
                }
                ceil_y = (n_rows_out_manual+1) * -sqrt(3) * R - R;
                k_upbl = n_rows_out_manual -1; //iterator
                vel_balls_go_down = 0.5 * vel_balls_go_down_manual ;

                for (int i = n_rows-1; i >= 0; i--) {
                    for (int j = 0; j < n_columns; ++j) {
                        if(balls[i][j].is_exist){
                            colors_exist(balls[i][j].rgb[0] , balls[i][j].rgb[1] , balls[i][j].rgb[2] , colors , color_counter1) ;
                            if(balls[i][j].multicolor){
                                colors_exist(balls[i][j].rgb1[0] , balls[i][j].rgb1[1] , balls[i][j].rgb1[2] , colors , color_counter1) ;
                            }
                        }
                    }
                }
                random_number_temp_color = rand() % 8 ;
                while(!colors[random_number_temp_color].is_exist_temp)
                    random_number_temp_color = rand() % 8 ;
                next_ball_color.r = colors[random_number_temp_color].r ;
                next_ball_color.g = colors[random_number_temp_color].g ;
                next_ball_color.b = colors[random_number_temp_color].b ;

                deadline_y = (n_rows-2) * sqrt(3) * R ;
                wanna_beam = false;
                flying_bullet.x = W/2;
                flying_bullet.y = H - R;

                img = IMG_LoadTexture(m_renderer, backgroundImgS.c_str());

                timer_started = false ;

                presec = 49;
                sec = 50;
                break;
            case 50:
                if(timer_mode_allowed){
                    timer_started = true ;
                    timer_mode_allowed = false ;
                    timer_checker = clock() ;
                }
                SDL_SetRenderDrawColor( m_renderer, 255, 255, 255, 255 );
                SDL_RenderClear( m_renderer );
                renderImg(m_renderer, img , 0, 0, W, H,false);

//                if(backgroundImg == 1){
//                    SDL_SetRenderDrawColor( m_renderer, 0, 0, 0, 255 );
//                }else if(backgroundImg == 2){
//                    SDL_SetRenderDrawColor( m_renderer, 255, 255, 255, 255 );
//                }else if(backgroundImg == 3){
//                    SDL_SetRenderDrawColor( m_renderer, 206, 196, 8, 255 );
//                }
//                SDL_RenderClear( m_renderer );

                string text = "your score: " + to_string(total_score);
                renderText(text.c_str(),50,1,m_renderer,50,H-100,{23,165,137,255});

                number_destroyed_balls = 0 ;
                number_fallen_balls = 0 ;
                ////speed balls go down and time loops and cycles
                cycle_counter++ ;
                freeze_ball_counter++ ;
                laser_ball_counter++ ;
                mega_bomb_ball_counter++ ;
                if(freeze_apply || collision_success){
                    if(freeze_apply){
                        if(freeze_time_counter == 0){
                            vel_balls_go_down *= 0.01 ;
                        }
                        if(effects)
                            boxRGBA(m_renderer , 0 , 0 , W , H , 54 , 139 , 193 , float(1-((clock() - freeze_apply_t)/float(freeze_ball_time_apply * CLOCKS_PER_SEC)))*150+10) ;
                        freeze_time_counter++ ;
                        if(clock() - freeze_apply_t >= freeze_ball_time_apply * CLOCKS_PER_SEC){
                            freeze_apply = false ;
                            vel_balls_go_down *= 100 ;
                        }
                    }else if(collision_success){
                        if(collision_success_timer == 0 && !flag_changed_vel_go_down){
                            prev_vel_balls_go_down = vel_balls_go_down ;
                            flag_changed_vel_go_down = true ;
                        }
                        vel_balls_go_down = prev_vel_balls_go_down * vel_balls_go_down_coll_reduction * ((1/vel_balls_go_down_coll_reduction)*(clock() - collision_success_t)/float(vel_balls_go_down_coll_recovery_time * CLOCKS_PER_SEC));
                        collision_success_timer++ ;
                        if(clock() - collision_success_t >= vel_balls_go_down_coll_recovery_time * CLOCKS_PER_SEC ){
                            collision_success = false ;
                            flag_changed_vel_go_down = false ;
                        }
                    }
                }else
                    vel_balls_go_down+=float(1/float(cycle_counter+vel_balls_go_down_initial_increase))*(vel_balls_go_down_rate_increase) ;
                if(laser_applied){
                    if(clock() - laser_applied_t >= laser_ball_time_apply * CLOCKS_PER_SEC){
                        laser_applied = false ;
                    }else if(effects){
                        float laser_transparency = float(1-((clock() - laser_applied_t)/float(laser_ball_time_apply*CLOCKS_PER_SEC)))*120 ;
                        float vel_x_laser_temp = vel_x_laser ;
                        float vel_y_laser_temp = vel_y_laser ;
                        laser_foot_print.x = laser_foot_print_x ;
                        laser_foot_print.y = laser_foot_print_y ;
                        while(laser_foot_print.y > 0){
                            wgo_laser =wanna_go_out(laser_foot_print.x , laser_foot_print.y , W , H , R);
                            if(wgo_laser == 2 || wgo_laser == 4){
                                vel_x_laser_temp *= -1;
                            }
                            laser_foot_print.x += vel_x_laser_temp;
                            laser_foot_print.y += vel_y_laser_temp;
                            filledEllipseRGBA(m_renderer, laser_foot_print.x, laser_foot_print.y, R , R , laser_foot_print.rgb[0], laser_foot_print.rgb[1], laser_foot_print.rgb[2], laser_transparency);
                        }
                        laser_time_counter++ ;
                    }
                }
                if(!freeze_ball_available && freeze_ball_allowed){
                    if(clock() - freeze_ball_t >= time_freeze_ball_available * CLOCKS_PER_SEC){
                        freeze_ball_available = true ;
                    }
                    for (int i = 0; i < (clock() - freeze_ball_t)/CLOCKS_PER_SEC; ++i)
                        filledPieRGBA(m_renderer , W*0.675 , H-R , R , i*(360/time_freeze_ball_available) , (i+1)*(360/time_freeze_ball_available) , 54 , 139 , 193 , 255 ) ;
                }else
                    filledEllipseRGBA(m_renderer , W*0.675 , H-R , R , R , 54 , 139 , 193 , 255 ); // freeze ball
                if(!laser_ball_available && laser_ball_allowed){
                    if(clock() - laser_ball_t >= time_laser_ball_available * CLOCKS_PER_SEC){
                        laser_ball_available = true ;
                        laser_ball_counter = 0 ;
                    }
                    for (int i = 0; i < (clock() - laser_ball_t)/CLOCKS_PER_SEC; ++i)
                        filledPieRGBA(m_renderer , W*0.75 , H-R , R , i*(360/time_laser_ball_available) , (i+1)*(360/time_laser_ball_available) , 255 , 255 , 102 , 255 ) ;
                }else
                    filledEllipseRGBA(m_renderer , W*0.75 , H-R , R , R , 255 , 255 , 102 , 255 ); // laser ball
                if(mega_phrase != 3 && mega_bomb_ball_allowed){
                    if(mega_phrase == 2){
                        filledEllipseRGBA(m_renderer , W*0.825 , H-R , R , R , mega_bomb_color[2].r , mega_bomb_color[2].g , mega_bomb_color[2].b , 255 ); // mega bomb ball phrase 3
                        for (int i = 0; i < (clock() - mega_ball_t)/CLOCKS_PER_SEC; ++i)
                            filledPieRGBA(m_renderer , W*0.825 , H-R , R , i*(360/time_mega_bomb_ball_available) , (i+1)*(360/time_mega_bomb_ball_available) , mega_bomb_color[3].r , mega_bomb_color[3].g , mega_bomb_color[3].b , 255 ) ;
                        if(clock() - mega_ball_t >= time_mega_bomb_ball_available * CLOCKS_PER_SEC){
                            mega_bomb_ball_available = true ;
                            mega_phrase = 3 ;
                            mega_bomb_ball_counter = 0 ;
                            mega_ball_t = clock() ;
                        }
                    }else if(mega_phrase == 1){
                        filledEllipseRGBA(m_renderer , W*0.825 , H-R , R , R , mega_bomb_color[1].r , mega_bomb_color[1].g , mega_bomb_color[1].b , 255 ); // mega bomb ball phrase 3
                        for (int i = 0; i < (clock() - mega_ball_t)/CLOCKS_PER_SEC; ++i)
                            filledPieRGBA(m_renderer , W*0.825 , H-R , R , i*(360/time_mega_bomb_ball_available) , (i+1)*(360/time_mega_bomb_ball_available) , mega_bomb_color[2].r , mega_bomb_color[2].g , mega_bomb_color[2].b , 255 ) ;
                        if(clock() - mega_ball_t >= time_mega_bomb_ball_available * CLOCKS_PER_SEC){
                            mega_bomb_ball_available = true ;
                            mega_phrase = 2 ;
                            mega_bomb_ball_counter = 0 ;
                            mega_ball_t = clock() ;
                        }
                    }else{
                        mega_phrase = 0 ;
                        filledEllipseRGBA(m_renderer , W*0.825 , H-R , R , R , mega_bomb_color[0].r , mega_bomb_color[0].g , mega_bomb_color[0].b  , 50 ); // mega bomb ball
                        for (int i = 0; i < (clock() - mega_ball_t)/CLOCKS_PER_SEC; ++i)
                            filledPieRGBA(m_renderer , W*0.825 , H-R , R , i*(360/time_mega_bomb_ball_available) , (i+1)*(360/time_mega_bomb_ball_available) , mega_bomb_color[1].r , mega_bomb_color[1].g , mega_bomb_color[1].b , 255 ) ;
                        if(clock() - mega_ball_t >= time_mega_bomb_ball_available * CLOCKS_PER_SEC){
                            mega_bomb_ball_available = true ;
                            mega_phrase = 1 ;
                            mega_bomb_ball_counter = 0 ;
                            mega_ball_t = clock() ;
                        }
                    }
                }else
                    filledEllipseRGBA(m_renderer , W*0.825 , H-R , R , R , mega_bomb_color[3].r , mega_bomb_color[3].g , mega_bomb_color[3].b , 255 ); // mega bomb ball phrase 3

                if(effects && mega_bomb_explosion){
                    int R_destruction ;
                    if(previous_mega_phrase == 1){
                        R_destruction = R * 4 ;
                    }else if(previous_mega_phrase == 2){
                        R_destruction = R * 7 ;
                    }else if(previous_mega_phrase == 3){
                        R_destruction = R * 10 ;
                    }
                    filledCircleRGBA(m_renderer , mega_bomb_print_x , mega_bomb_print_y , R_destruction , mega_bomb_color[previous_mega_phrase].r , mega_bomb_color[previous_mega_phrase].g , mega_bomb_color[previous_mega_phrase].b , float(1-((clock() - mega_bomb_explosion_t)/float(mega_bomb_time_explosion * previous_mega_phrase * CLOCKS_PER_SEC)))*120) ;
                    mega_bomb_after_explosion_time_counter++ ;
                    if(clock() - mega_bomb_explosion_t +1 >= mega_bomb_time_explosion * previous_mega_phrase * CLOCKS_PER_SEC){
                        mega_bomb_explosion = false ;
                        mega_bomb_after_explosion_time_counter = 0 ;
                    }
                }

                lineRGBA(m_renderer, 0, deadline_y-R, W, deadline_y-R, 255, 0, 0, 255); // dead line
                filledEllipseRGBA(m_renderer , W*0.95 , H-R , R , R , 255 , 255 , 255 , 255 ); // menu / puase button
                //boxRGBA(m_renderer, W*0.87 , H , W*0.63 , H-2*R , 255 , 255 , 255 , 20 ); // box for the specials
                filledEllipseRGBA(m_renderer , W*0.675 , H-R , R , R , 54 , 139 , 193 , 50 ); // freeze ball
                filledEllipseRGBA(m_renderer , W*0.75 , H-R , R , R , 255 , 255 , 102 , 50 ); // laser ball
                if( ceil_y > 0 && game_CHECK( balls , balls_out , colors ) == 0){
                    sec = 7 ;
                    presec = 50 ;
                    vel_balls_go_down = 0 ;
                }

                balls_go_down(m_renderer, W, H, balls, balls_out , deadline_y);
                ceil_y += vel_balls_go_down ;

                if(laser_apply || mega_bomb_apply || mega_bomb_applied){
                    if(mega_bomb_applied){
                        mega_bomb_beaming(m_renderer , W , H , mouseX , mouseY , flying_bullet , vel_x_mega_bomb , vel_y_mega_bomb , flying_color , colors , mega_phrase);
                        vector< array <int , 2 >> destructed_balls ;
                        if(checking_collision_mega_bomb(m_renderer , W , H , balls , flying_bullet, flying_color , new_ball_row , new_ball_column , mega_phrase , destructed_balls) == 1){
                            flying_color.r = color_temp_r;
                            flying_color.g = color_temp_g;
                            flying_color.b = color_temp_b;
                            mega_bomb_applied = false ;
                            mega_bomb_print_x = flying_bullet.x ;
                            mega_bomb_print_y = flying_bullet.y ;
                            flying_bullet.x = W/2 ;
                            flying_bullet.y = H - R ;
                            previous_mega_phrase = mega_phrase ;
                            mega_bomb_ball_counter = 0 ;
                            mega_bomb_after_explosion_time_counter = 0 ;
                            mega_bomb_time_counter = 0 ;
                            mega_bomb_explosion = true ;
                            //sound mega
                            if(mega_phrase == 1){
                                sound = Mix_LoadWAV("../chunks/mega_bomb_1.mp3");
                                Mix_PlayChannel(-1, sound, 0);
                            }else if(mega_phrase == 2){
                                sound = Mix_LoadWAV("../chunks/mega_bomb_2.mp3");
                                Mix_PlayChannel(-1, sound, 0);
                            }else if(mega_phrase == 3){
                                sound = Mix_LoadWAV("../chunks/mega_bomb_3.mp3");
                                Mix_PlayChannel(-1, sound, 0);
                            }
                            mega_phrase = 0 ;
                            mega_bomb_explosion_t = clock() ;
                            mega_ball_t = clock() ;
                        }
                    }
                }else{
                    if (collided || flag1) {
                        game_CHECK( balls , balls_out , colors);
                        if(flying_color.multi_color){
                            flying_color.multi_color = false ;
                            if(k_upbl > 1){
                                finding_neighbors_balls_new(new_ball_row, new_ball_column, balls , output) ;
                            }else
                                finding_neighbors_balls(new_ball_row, new_ball_column, balls , output) ;
                            bool flag2 = false ;
                            int new_new_ball_row = new_ball_row;
                            int new_new_ball_column = new_ball_column;
                            int largest_counter_neighbors = -1;
                            int new_r , new_g , new_b;
                            balls[new_new_ball_row][new_new_ball_column].is_exist = false;
                            for(auto i : output){
                                if (balls[i[0]][i[1]].is_exist){
                                    new_ball_row = i[0];
                                    new_ball_column = i[1];
                                    finding_balls_same_color(output1, balls, new_ball_row, new_ball_column,
                                                             changed_balls, balls[new_ball_row][new_ball_column].rgb[0],
                                                             balls[new_ball_row][new_ball_column].rgb[1],
                                                             balls[new_ball_row][new_ball_column].rgb[2]);
                                    vector<array<int, 2>> real_changed_balls;
                                    for (auto k: changed_balls) {
                                        bool flag = true;
                                        for (auto j: real_changed_balls) {
                                            if (k[0] == j[0] && k[1] == j[1])
                                                flag = false;
                                        }
                                        if (flag) {
                                            real_changed_balls.push_back({k[0], k[1]});
                                            balls[k[0]][k[1]].is_exist = true;
                                        }
                                    }
                                    if(int(real_changed_balls.size()) >= largest_counter_neighbors && (balls[new_ball_row][new_ball_column].rgb[0] != colors[8].r && balls[new_ball_row][new_ball_column].rgb[1] != colors[8].g && balls[new_ball_row][new_ball_column].rgb[2] != colors[8].b)){
                                        largest_counter_neighbors = int(real_changed_balls.size());
                                        new_r = balls[i[0]][i[1]].rgb[0] ;
                                        new_g = balls[i[0]][i[1]].rgb[1] ;
                                        new_b = balls[i[0]][i[1]].rgb[2] ;
                                    }
                                    real_changed_balls.clear();
                                    changed_balls.clear();
                                }
                            }
                            if(largest_counter_neighbors > -1){
                                balls[new_new_ball_row][new_new_ball_column].is_exist = true;
                                new_ball_row = new_new_ball_row ;
                                new_ball_column = new_new_ball_column ;
                                finding_balls_same_color ( output , balls , new_ball_row , new_ball_column ,
                                                           changed_balls , new_r , new_g , new_b) ;
                                vector <array<int,2>> prime_changed_balls ;
                                for(auto i:changed_balls){
                                    bool flag = true ;
                                    for(auto j :prime_changed_balls){
                                        if(i[0] == j[0] && i[1] == j[1])
                                            flag = false ;
                                    }
                                    if(flag)
                                        prime_changed_balls.push_back({i[0],i[1]}) ;
                                }
                                if(prime_changed_balls.size() <= 2){
                                    while(!changed_balls.empty()){
                                        balls[changed_balls.back().at(0)][changed_balls.back().at(1)].is_exist = true ;
                                        changed_balls.pop_back() ;
                                    }
                                }else{
                                    collision_success_t = clock() ;
                                    collision_success = true ;
                                    vector <array<int,2>> real_changed_balls ;
                                    for(auto i:changed_balls){
                                        bool flag = true ;
                                        for(auto j :real_changed_balls){
                                            if(i[0] == j[0] && i[1] == j[1])
                                                flag = false ;
                                        }
                                        if(flag){
                                            real_changed_balls.push_back({i[0],i[1]}) ;
                                            if(effects)
                                                filledEllipseRGBA(m_renderer , balls[i[0]][i[1]].x , balls[i[0]][i[1]].y , R*1.2 , R*1.2 , 0 , 255 , 255 , 100);
                                            balls[i[0]][i[1]].multicolor = false ;
                                            balls[i[0]][i[1]].locked = false ;
                                        }
                                    }
                                    number_destroyed_balls+=real_changed_balls.size() ;
                                    real_changed_balls.clear() ;
                                }
                                changed_balls.clear() ;
                                prime_changed_balls.clear() ;
                            }
                        }else{
                            if(next_ball_color.multi_color){
                                next_ball_color.multi_color = false ;
                                flying_color.multi_color = true ;
                                int random_number = rand() % 8 ;
                                while(!colors[random_number].is_exist_temp){
                                    random_number = rand() % 8 ;
                                }
                                next_ball_color.r = colors[random_number].r ;
                                next_ball_color.g = colors[random_number].g ;
                                next_ball_color.b = colors[random_number].b ;
                            }else if(multi_color_flying_ball && rand()%amount_multi_color_flying_ball == 0){
                                flying_color.r = next_ball_color.r ;
                                flying_color.g = next_ball_color.g ;
                                flying_color.b = next_ball_color.b ;
                                next_ball_color.multi_color = true ;
                            }else{
                                int random_number = rand() % 8 ;
                                while(!colors[random_number].is_exist_temp){
                                    random_number = rand() % 8 ;
                                }
                                flying_color.r = next_ball_color.r ;
                                flying_color.g = next_ball_color.g ;
                                flying_color.b = next_ball_color.b ;
                                next_ball_color.r = colors[random_number].r ;
                                next_ball_color.g = colors[random_number].g ;
                                next_ball_color.b = colors[random_number].b ;
                                for (int i = 0; i < 8; ++i) {
                                    if(flying_color.r == colors[i].r && flying_color.g == colors[i].g && flying_color.b == colors[i].b){
                                        if(!colors[i].is_exist_temp){
                                            random_number = rand() % 8 ;
                                            while(!colors[random_number].is_exist_temp){
                                                random_number = rand() % 8 ;
                                            }
                                            flying_color.r = next_ball_color.r ;
                                            flying_color.g = next_ball_color.g ;
                                            flying_color.b = next_ball_color.b ;
                                            next_ball_color.r = colors[random_number].r ;
                                            next_ball_color.g = colors[random_number].g ;
                                            next_ball_color.b = colors[random_number].b ;
                                        }
                                    }
                                }
                            }
                            for (int i = 0; i < 8; ++i) {
                                colors[i].is_exist_temp = false ;
                            }
                            flag1 = false ;
                            if(collided){
                                if(k_upbl > 1){
                                    finding_neighbors_balls_new(new_ball_row, new_ball_column, balls , output) ;
                                }else
                                    finding_neighbors_balls(new_ball_row, new_ball_column, balls , output) ;
                                bool flag2 = false ;
                                for(auto i : output){
                                    if (balls[i[0]][i[1]].is_exist && balls[i[0]][i[1]].locked && (balls[i[0]][i[1]].rgb[0] == balls[new_ball_row][new_ball_column].rgb[0] && balls[i[0]][i[1]].rgb[1] == balls[new_ball_row][new_ball_column].rgb[1] && balls[i[0]][i[1]].rgb[2] == balls[new_ball_row][new_ball_column].rgb[2])){
                                        flag2 = true ;
                                    }
                                }
                                if(flag2){
                                    int new_new_ball_row = new_ball_row;
                                    int new_new_ball_column = new_ball_column;
                                    for(auto i : output) {
                                        if (!balls[i[0]][i[1]].locked && balls[i[0]][i[1]].is_exist &&
                                            (balls[i[0]][i[1]].rgb[0] == balls[new_new_ball_row][new_new_ball_column].rgb[0] &&
                                             balls[i[0]][i[1]].rgb[1] == balls[new_new_ball_row][new_new_ball_column].rgb[1] &&
                                             balls[i[0]][i[1]].rgb[2] == balls[new_new_ball_row][new_new_ball_column].rgb[2])) {
                                            balls[new_new_ball_row][new_new_ball_column].is_exist = false;
                                            new_ball_row = i[0];
                                            new_ball_column = i[1];
                                            finding_balls_same_color(output1, balls, new_ball_row, new_ball_column,
                                                                     changed_balls, balls[new_ball_row][new_ball_column].rgb[0],
                                                                     balls[new_ball_row][new_ball_column].rgb[1],
                                                                     balls[new_ball_row][new_ball_column].rgb[2]);
                                            vector<array<int, 2>> prime_changed_balls;
                                            for (auto k: changed_balls) {
                                                bool flag = true;
                                                for (auto j: prime_changed_balls) {
                                                    if (k[0] == j[0] && k[1] == j[1])
                                                        flag = false;
                                                }
                                                if (flag)
                                                    prime_changed_balls.push_back({k[0], k[1]});
                                            }
                                            if (prime_changed_balls.size() <= 1) {
                                                while (!changed_balls.empty()) {
                                                    balls[changed_balls.back().at(0)][changed_balls.back().at(1)].is_exist = true;
                                                    changed_balls.pop_back();
                                                }
                                                balls[new_new_ball_row][new_new_ball_column].is_exist = true;
                                            } else {
                                                collision_success_t = clock() ;
                                                collision_success = true ;
                                                vector<array<int, 2>> real_changed_balls;
                                                for (auto k: changed_balls) {
                                                    bool flag = true;
                                                    for (auto j: real_changed_balls) {
                                                        if (k[0] == j[0] && k[1] == j[1])
                                                            flag = false;
                                                    }
                                                    if (flag) {
                                                        real_changed_balls.push_back({k[0], k[1]});
                                                        filledEllipseRGBA(m_renderer, balls[k[0]][k[1]].x, balls[k[0]][k[1]].y,
                                                                          R * 1.2, R * 1.2, 0, 255, 255, 100);
                                                        balls[k[0]][k[1]].multicolor = false;
                                                        balls[k[0]][k[1]].locked = false;
                                                    }
                                                }
                                                number_destroyed_balls+=real_changed_balls.size();
                                                real_changed_balls.clear();
                                            }
                                            changed_balls.clear();
                                            prime_changed_balls.clear();
                                        }else if(balls[i[0]][i[1]].multicolor && !balls[i[0]][i[1]].locked && balls[i[0]][i[1]].is_exist &&
                                                 (balls[i[0]][i[1]].rgb1[0] == balls[new_new_ball_row][new_new_ball_column].rgb[0] &&
                                                  balls[i[0]][i[1]].rgb1[1] == balls[new_new_ball_row][new_new_ball_column].rgb[1] &&
                                                  balls[i[0]][i[1]].rgb1[2] == balls[new_new_ball_row][new_new_ball_column].rgb[2])){
                                            balls[new_new_ball_row][new_new_ball_column].is_exist = false;
                                            new_ball_row = i[0];
                                            new_ball_column = i[1];
                                            finding_balls_same_color(output1, balls, new_ball_row, new_ball_column,
                                                                     changed_balls, balls[new_ball_row][new_ball_column].rgb1[0],
                                                                     balls[new_ball_row][new_ball_column].rgb1[1],
                                                                     balls[new_ball_row][new_ball_column].rgb1[2]);
                                            vector<array<int, 2>> prime_changed_balls;
                                            for (auto k: changed_balls) {
                                                bool flag = true;
                                                for (auto j: prime_changed_balls) {
                                                    if (k[0] == j[0] && k[1] == j[1])
                                                        flag = false;
                                                }
                                                if (flag)
                                                    prime_changed_balls.push_back({k[0], k[1]});
                                            }
                                            if (prime_changed_balls.size() <= 1) {
                                                while (!changed_balls.empty()) {
                                                    balls[changed_balls.back().at(0)][changed_balls.back().at(1)].is_exist = true;
                                                    changed_balls.pop_back();
                                                }
                                                balls[new_new_ball_row][new_new_ball_column].is_exist = true;
                                            } else {
                                                collision_success_t = clock() ;
                                                collision_success = true ;
                                                vector<array<int, 2>> real_changed_balls;
                                                for (auto k: changed_balls) {
                                                    bool flag = true;
                                                    for (auto j: real_changed_balls) {
                                                        if (k[0] == j[0] && k[1] == j[1])
                                                            flag = false;
                                                    }
                                                    if (flag) {
                                                        real_changed_balls.push_back({k[0], k[1]});
                                                        filledEllipseRGBA(m_renderer, balls[k[0]][k[1]].x, balls[k[0]][k[1]].y,
                                                                          R * 1.2, R * 1.2, 0, 255, 255, 100);
                                                        balls[k[0]][k[1]].multicolor = false;
                                                        balls[k[0]][k[1]].locked = false;
                                                    }
                                                }
                                                number_destroyed_balls+=real_changed_balls.size();
                                                real_changed_balls.clear();
                                            }
                                            changed_balls.clear();
                                            prime_changed_balls.clear();
                                        }
                                    }
                                    for(auto i : output) {
                                        if (balls[i[0]][i[1]].locked && balls[i[0]][i[1]].is_exist &&
                                            (balls[i[0]][i[1]].rgb[0] == balls[new_new_ball_row][new_new_ball_column].rgb[0] &&
                                             balls[i[0]][i[1]].rgb[1] == balls[new_new_ball_row][new_new_ball_column].rgb[1] &&
                                             balls[i[0]][i[1]].rgb[2] == balls[new_new_ball_row][new_new_ball_column].rgb[2])) {
                                            balls[new_new_ball_row][new_new_ball_column].is_exist = false;
                                            new_ball_row = i[0];
                                            new_ball_column = i[1];
                                            balls[new_ball_row][new_ball_column].locked = false;
                                            finding_balls_same_color(output1, balls, new_ball_row, new_ball_column,
                                                                     changed_balls, balls[new_ball_row][new_ball_column].rgb[0],
                                                                     balls[new_ball_row][new_ball_column].rgb[1],
                                                                     balls[new_ball_row][new_ball_column].rgb[2]);
                                            vector<array<int, 2>> prime_changed_balls;
                                            for (auto k: changed_balls) {
                                                bool flag = true;
                                                for (auto j: prime_changed_balls) {
                                                    if (k[0] == j[0] && k[1] == j[1])
                                                        flag = false;
                                                }
                                                if (flag)
                                                    prime_changed_balls.push_back({k[0], k[1]});
                                            }
                                            if (prime_changed_balls.size() <= 1) {
                                                while (!changed_balls.empty()) {
                                                    balls[changed_balls.back().at(0)][changed_balls.back().at(1)].is_exist = true;
                                                    balls[changed_balls.back().at(0)][changed_balls.back().at(1)].locked = false;
                                                    changed_balls.pop_back();
                                                }
                                            } else {
                                                vector<array<int, 2>> real_changed_balls;
                                                for (auto k: changed_balls) {
                                                    bool flag = true;
                                                    for (auto j: real_changed_balls) {
                                                        if (k[0] == j[0] && k[1] == j[1])
                                                            flag = false;
                                                    }
                                                    if (flag) {
                                                        real_changed_balls.push_back({k[0], k[1]});
                                                        if(effects)
                                                            filledEllipseRGBA(m_renderer, balls[k[0]][k[1]].x, balls[k[0]][k[1]].y,
                                                                              R * 1.2, R * 1.2, 255, 0, 255, 100);
                                                        balls[k[0]][k[1]].is_exist = true ;
                                                        balls[k[0]][k[1]].locked = false;
                                                    }
                                                }
                                                real_changed_balls.clear();
                                            }
                                            changed_balls.clear();
                                            prime_changed_balls.clear();
                                        }
                                    }
                                }else{
                                    finding_balls_same_color ( output , balls , new_ball_row , new_ball_column ,
                                                               changed_balls , balls[new_ball_row][new_ball_column].rgb[0] , balls[new_ball_row][new_ball_column].rgb[1] , balls[new_ball_row][new_ball_column].rgb[2]) ;
                                    vector <array<int,2>> prime_changed_balls ;
                                    for(auto i:changed_balls){
                                        bool flag = true ;
                                        for(auto j :prime_changed_balls){
                                            if(i[0] == j[0] && i[1] == j[1])
                                                flag = false ;
                                        }
                                        if(flag)
                                            prime_changed_balls.push_back({i[0],i[1]}) ;
                                    }
                                    if(prime_changed_balls.size() <= 2){
                                        while(!changed_balls.empty()){
                                            balls[changed_balls.back().at(0)][changed_balls.back().at(1)].is_exist = true ;
                                            changed_balls.pop_back() ;
                                        }
                                    }else{
                                        collision_success_t = clock() ;
                                        collision_success = true ;
                                        vector <array<int,2>> real_changed_balls ;
                                        for(auto i:changed_balls){
                                            bool flag = true ;
                                            for(auto j :real_changed_balls){
                                                if(i[0] == j[0] && i[1] == j[1])
                                                    flag = false ;
                                            }
                                            if(flag){
                                                real_changed_balls.push_back({i[0],i[1]}) ;
                                                if(effects)
                                                    filledEllipseRGBA(m_renderer , balls[i[0]][i[1]].x , balls[i[0]][i[1]].y , R*1.2 , R*1.2 , 0 , 255 , 255 , 100);
                                                balls[i[0]][i[1]].multicolor = false ;
                                                balls[i[0]][i[1]].locked = false ;
                                            }
                                        }
                                        number_destroyed_balls+=real_changed_balls.size();
                                        real_changed_balls.clear() ;
                                    }
                                    changed_balls.clear() ;
                                    prime_changed_balls.clear() ;
                                }
                            }
                        }
                        collided = false ;
                    }

                    if (wanna_beam) {
                        double vel_x, vel_y;
                        if (!bullet_is_flying) {
                            double u_h1, u_l1, u_ch1, urx, ury;
                            u_l1 = mouseX - W / 2; //u_length1
                            u_h1 = mouseY - H + R; //u_height1
                            u_ch1 = sqrt(pow(u_h1, 2) + pow(u_l1, 2)); //u_chord1
                            urx = (u_l1 / u_ch1); //u_ratio_x
                            ury = (u_h1 / u_ch1); //u_ratio_y
                            vel_x = urx * vel_flying_ball * R;
                            vel_y = ury * vel_flying_ball * R;
                        }
                        draw_beam(m_renderer, W, H, mouseX, mouseY, flying_bullet, vel_x, vel_y, flying_color , colors);
                        bullet_is_flying = true;
                        if (flying_bullet.y >= H) {
                            bullet_is_flying = false;
                            wanna_beam = false;
                            flying_bullet.x = W / 2;
                            flying_bullet.y = H - R;
                            flag1 = true ;
                        }
                        if (checking_collision(m_renderer, W, H, balls, flying_bullet, vel_x, vel_y, flying_color, new_ball_row,
                                               new_ball_column , 2.5*R*R) == 1) {
                            wanna_beam = false;
                            collided = true;
                            flying_bullet.x = W / 2;
                            flying_bullet.y = H - R;
                        }else if(flying_bullet.y <= ceil_y+R){
                            checking_collision_ceil(m_renderer, W, H, balls, flying_bullet, vel_x, vel_y, flying_color, new_ball_row,
                                                    new_ball_column , 4*R*R);
                            wanna_beam = false;
                            collided = true;
                            flying_bullet.x = W / 2;
                            flying_bullet.y = H - R;
                        }
                    } else {
                        bullet_is_flying = false;
                    }
                }
                if(flying_color.multi_color){
                    for (int i = 1; i < number_of_colors; ++i) {
                        filledPieRGBA(m_renderer, W / 2, H - R , R, (i-1)*(360/(number_of_colors-1)) , i*(360/(number_of_colors-1)) , colors[color[i]].r , colors[color[i]].g , colors[color[i]].b , 255);
                    }
                }else
                    filledEllipseRGBA(m_renderer, W / 2, H - R, R, R, flying_color.r, flying_color.g, flying_color.b, 255);
                if(next_ball_color.multi_color){
                    for (int i = 1; i < number_of_colors; ++i) {
                        filledPieRGBA(m_renderer, W / 2 - 3 * R, H - R, R / 2, (i-1)*(360/(number_of_colors-1)) , i*(360/(number_of_colors-1)) , colors[color[i]].r , colors[color[i]].g , colors[color[i]].b , 255);
                    }
                }else
                    filledEllipseRGBA(m_renderer, W / 2 - 3 * R, H - R, R / 2, R / 2, next_ball_color.r, next_ball_color.g,
                                      next_ball_color.b, 255);

                if (mouseY < H * 0.6 && laser_apply)
                    draw_target(m_renderer, W, H, mouseX, mouseY, flying_color , colors);
                else if (mouseY < H - 2 * R && !laser_apply)
                    draw_target(m_renderer, W, H, mouseX, mouseY, flying_color , colors);

                for (int i = 0; i < n_rows; i++) {
                    for (int j = 0; j < n_columns; j++) {
                        balls[i][j].connect = false;
                    }
                }
                for (int i = 0; i < n_rows; i++) {
                    if(ceil_y >= -2 * R) {
                        checkBallConnection(balls, 0, i);
                    }
                    else
                        checkBallConnection(balls, 1, i);
                }
                for (int i = 1; i < n_rows; i++) {
                    for (int j = 0; j < n_columns; j++) {
                        if (!balls[i][j].connect && balls[i][j].is_exist){
                            balls[i][j].is_exist = false;
                            balls[i][j].falling = true ;
                            balls_falling[i][j] = balls[i][j] ;
                            balls[i][j].multicolor = false;
                            balls[i][j].locked = false;
                            falling_balls.push_back({float(i),float(j),0}) ;
                            falling = true ;
                            number_fallen_balls++ ;
                        }
                    }
                }if(falling){
            int counter_still_falling = 0 ;
            for (auto & i : falling_balls) {
                if(balls_falling[int(i[0])][int(i[1])].y >= H+R || !balls_falling[int(i[0])][int(i[1])].falling){
                    balls_falling[int(i[0])][int(i[1])].falling = false ;
                    balls_falling[int(i[0])][int(i[1])].multicolor = false;
                    balls_falling[int(i[0])][int(i[1])].locked = false;
                    i[2] = 0 ;
                }else{
                    counter_still_falling++ ;
                    i[2]+= float(9.8 / float(mean_fps)) * 2 ;
                    balls_falling[int(i[0])][int(i[1])].y+=i[2] ;
                    filledEllipseRGBA(m_renderer , balls_falling[int(i[0])][int(i[1])].x , balls_falling[int(i[0])][int(i[1])].y , R , R , balls_falling[int(i[0])][int(i[1])].rgb[0] , balls_falling[int(i[0])][int(i[1])].rgb[1] , balls_falling[int(i[0])][int(i[1])].rgb[2] , 255) ;
                    if(balls_falling[int(i[0])][int(i[1])].multicolor)
                        filledEllipseRGBA(m_renderer , balls_falling[int(i[0])][int(i[1])].x , balls_falling[int(i[0])][int(i[1])].y , R/2 , R/2 , balls_falling[int(i[0])][int(i[1])].rgb1[0] , balls_falling[int(i[0])][int(i[1])].rgb1[1] , balls_falling[int(i[0])][int(i[1])].rgb1[2] , 255) ;
                    else if(balls_falling[int(i[0])][int(i[1])].locked){
                        float newR = (R/sqrt(2)) ;
                        thickLineRGBA(m_renderer , balls_falling[int(i[0])][int(i[1])].x-newR , balls_falling[int(i[0])][int(i[1])].y-newR , balls_falling[int(i[0])][int(i[1])].x+newR , balls_falling[int(i[0])][int(i[1])].y+newR , R/7 , 0 , 0 , 0 , 255 );
                        thickLineRGBA(m_renderer , balls_falling[int(i[0])][int(i[1])].x-newR , balls_falling[int(i[0])][int(i[1])].y+newR , balls_falling[int(i[0])][int(i[1])].x+newR , balls_falling[int(i[0])][int(i[1])].y-newR , R/7 , 0 , 0 , 0 , 255 );
                    }
                }
            }
            if(counter_still_falling == 0){
                falling = false ;
                falling_balls.clear() ;
            }
        }

                draw_balls(m_renderer, W, H, balls);
                if(ceil_y >= 0)
                    lineRGBA(m_renderer, 0, ceil_y, W, ceil_y, 255, 0, 255, 255); // ceil line
                //draw_balls2(m_renderer , W , H , balls);
                //------------------------------

                if(play_mode == 0) { // DEVELOPER
                    total_score+=((number_fallen_balls * 50) + (number_destroyed_balls * 20)) * 1;
                }else if(play_mode == 1){ // INFINITY
                    total_score+=((number_fallen_balls * 50) + (number_destroyed_balls * 20)) * 3;
                }else if(play_mode == 2){ // EASY
                    total_score+=((number_fallen_balls * 50) + (number_destroyed_balls * 20)) * 1;
                }else if(play_mode == 3){ // NORMAL
                    total_score+=((number_fallen_balls * 50) + (number_destroyed_balls * 20)) * 2;
                }else if(play_mode == 4){ // HARD
                    total_score+=((number_fallen_balls * 50) + (number_destroyed_balls * 20)) * 3;
                }else if(play_mode == 5){ // EXTREME
                    total_score+=((number_fallen_balls * 50) + (number_destroyed_balls * 20)) *5;
                }else if(play_mode == 6){ // TIME
                    total_score+=((number_fallen_balls * 50) + (number_destroyed_balls * 20)) *2;
                }

                checkGameStatus(m_renderer , W , H , playing , started , balls , deadline_y);

                if(timer_started){
                    filledCircleRGBA(m_renderer , W*0.05 , H-2.6*R , R*4/5 , 23 , 165 , 160 , 100 ) ;
                    for (int i = 0 ; i < (clock()-timer_checker)/CLOCKS_PER_SEC ; ++i) {
                        filledPieRGBA(m_renderer , W*0.05 , H-2.6*R , R*4/5 , i*(360/timer)+90 , (i+1)*(360/timer)+90 , 23 , 165 , 160 , 255 ) ;
                    }
                    if(clock()-timer_checker >= timer * CLOCKS_PER_SEC){
                        timer_started = false ;
                        sec = 7 ;
                        presec = 50 ;

                    }
                }

                t=double(clock()-now)/CLOCKS_PER_SEC;
                mean_t = (mean_t * float(fps_cycle_tracker) + t)/float(fps_cycle_tracker+1) ;
                if(optimised_fps && fps_cycle_tracker % int(mean_fps) == 0) {
                    fps_cycle_counter++ ;
                    fps_cycle_tracker = 0 ;
                    fps = float(1/double(mean_t * 0.6)) ;
                    mean_fps = (mean_fps * float(fps_cycle_counter-1) + fps)/float(fps_cycle_counter) ;
                    mean_t = 0 ;
                    if(abs(mean_fps - fps) >= 15)
                        mean_fps = fps ;
                    if(mean_fps > 60)
                        mean_fps = 60 ;
                    if(mean_fps < 30)
                        mean_fps = 30 ;
                    time_fps = 1000/double(mean_fps) ;
                }else{
                    if(time_fps>t){
                        SDL_Delay(int(time_fps-t)) ;
                    }
                }
                fps_cycle_tracker++ ;
                break;
        }
        click = false;
        SDL_RenderPresent(m_renderer);
    }
    //------------------------< Quit
    Mix_FreeChunk(sound);
    updateFile("../files/data.csv",data);
    SDL_StopTextInput();
    Mix_FreeMusic(music);
    Mix_CloseAudio();
    SDL_DestroyWindow(m_window);
    SDL_DestroyRenderer(m_renderer);
    IMG_Quit();
    SDL_Quit();
    //------------------------> Quit
    return 0;
}

int initializing_play_mode_0(SDL_Renderer * renderer , int W , int H , ball2d balls[n_rows][n_columns] , ball2d balls_out[n_rows_out][n_columns] , color2d colors[9] , vector<array<int, 2>> &output , int color[number_of_colors]){
    n_rows_out_manual = 1 ;
    vel_balls_go_down_manual = 3 ;
    vel_balls_go_down_initial_increase = 60 ;// preferred at around 60 for normal
    vel_balls_go_down_rate_increase = 0.1 ;// preferred at around 0.1 for normal
    vel_balls_go_down_coll_reduction = 0.1 ;// preferred at around 0.2 for normal
    vel_balls_go_down_coll_recovery_time = 3 ;// preferred at around 1 for normal
    multi_color_flying_ball = true ;
    amount_multi_color_flying_ball = 20; // 1 in every number that you put there will be a ball with multi colors
    gray_balls_exist = true;
    difficulty_gray_balls = 0.5; // !!! IT SHOULD BE IN THIS FORM AND BE FLOAT // the lower it is the less compacted will be the gray balls
    multicolor_ball = true;
    amount_multicolor_ball = 0.5; // !!! IT SHOULD BE IN THIS FORM AND BE FLOAT // the lower it is the less compacted will be the multi_colored will be the balls
    locked_ball = true;
    amount_locked_ball = 0.5;
    volume_of_balls = 1; // !!! IT SHOULD BE IN THIS FORM AND BE FLOAT // and for now it can not be 1 or 0 ...
    difficulty_of_colors = 6; // It should be bigger than 0 and its int , the bigger it is the less difficult the map would get for normal around 5
    if(color_random_chosen){
        for (int i = 0; i < number_of_colors; ++i) {
            if(i == 0 && gray_balls_exist){
                color[i] = 8 ;
            }else{
                bool flag2 = true ;
                while(flag2){
                    int temp_rand = rand()%8 ;
                    flag2 = false ;
                    for (int j = 0; j < i; ++j) {///////////////////////////////////changed =
                        if(color[j] == temp_rand)
                            flag2 = true ;
                    }
                    if(!flag2)
                        color[i]=temp_rand ;
                }
            }
        }
    }else{
        for (int j = 0; j < number_of_colors; ++j) {
            color[j] = color_selection[j] ;
        }
    }
    initial_balls_color_0(renderer , W , H , balls , balls_out , colors , output , color);
}

int initializing_play_mode_1(SDL_Renderer * renderer , int W , int H , ball2d balls[n_rows][n_columns] , ball2d balls_out[n_rows_out][n_columns] , color2d colors[9] , vector<array<int, 2>> &output , int color[number_of_colors]){
    n_rows_out_manual = 2000 ;
    vel_balls_go_down_manual = 1.5 ;
    vel_balls_go_down_initial_increase = 60 ;// preferred at around 60 for normal
    vel_balls_go_down_rate_increase = 0.3 ;// preferred at around 0.1 for normal
    vel_balls_go_down_coll_reduction = 0.15 ;// preferred at around 0.2 for normal
    vel_balls_go_down_coll_recovery_time = 3 ;// preferred at around 1 for normal
    amount_multi_color_flying_ball = 25; // 1 in every number that you put there will be a ball with multi colors
    difficulty_gray_balls = 0.1; // !!! IT SHOULD BE IN THIS FORM AND BE FLOAT // the lower it is the less compacted will be the gray balls
    amount_multicolor_ball = 0.9; // !!! IT SHOULD BE IN THIS FORM AND BE FLOAT // the lower it is the less compacted will be the multi_colored will be the balls
    amount_locked_ball = 0.1;
    volume_of_balls = 1; // !!! IT SHOULD BE IN THIS FORM AND BE FLOAT // and for now it can not be 1 or 0 ...
    difficulty_of_colors = 5; // It should be bigger than 0 and its int , the bigger it is the less difficult the map would get for normal around 5
    if(color_random_chosen){
        for (int i = 0; i < number_of_colors; ++i) {
            if(i == 0 && gray_balls_exist){
                color[i] = 8 ;
            }else{
                bool flag2 = true ;
                while(flag2){
                    int temp_rand = rand()%8 ;
                    flag2 = false ;
                    for (int j = 0; j < i; ++j) {///////////////////////////////////changed =
                        if(color[j] == temp_rand)
                            flag2 = true ;
                    }
                    if(!flag2)
                        color[i]=temp_rand ;
                }
            }
        }
    }else{
        for (int j = 0; j < number_of_colors; ++j) {
            color[j] = color_selection[j] ;
        }
    }
    initial_balls_color_1(renderer , W , H , balls , balls_out , colors , output , color);
}

int initializing_play_mode_2(SDL_Renderer * renderer , int W , int H , ball2d balls[n_rows][n_columns] , ball2d balls_out[n_rows_out][n_columns] , color2d colors[9] , vector<array<int, 2>> &output , int color[number_of_colors]){
    n_rows_out_manual = 5 ;
    vel_balls_go_down_manual = 1 ;
    vel_balls_go_down_initial_increase = 60 ;// preferred at around 60 for normal
    vel_balls_go_down_rate_increase = 0.1 ;// preferred at around 0.1 for normal
    vel_balls_go_down_coll_reduction = 0.1 ;// preferred at around 0.2 for normal
    vel_balls_go_down_coll_recovery_time = 4 ;// preferred at around 1 for normal
    amount_multi_color_flying_ball = 10; // 1 in every number that you put there will be a ball with multi colors
    difficulty_gray_balls = 0.1; // !!! IT SHOULD BE IN THIS FORM AND BE FLOAT // the lower it is the less compacted will be the gray balls
    amount_multicolor_ball = 0.9; // !!! IT SHOULD BE IN THIS FORM AND BE FLOAT // the lower it is the less compacted will be the multi_colored will be the balls
    amount_locked_ball = 0.1;
    volume_of_balls = 1; // !!! IT SHOULD BE IN THIS FORM AND BE FLOAT // and for now it can not be 1 or 0 ...
    difficulty_of_colors = 5; // It should be bigger than 0 and its int , the bigger it is the less difficult the map would get for normal around 5
    if(color_random_chosen){
        for (int i = 0; i < number_of_colors; ++i) {
            if(i == 0 && gray_balls_exist){
                color[i] = 8 ;
            }else{
                bool flag2 = true ;
                while(flag2){
                    int temp_rand = rand()%8 ;
                    flag2 = false ;
                    for (int j = 0; j < i; ++j) {///////////////////////////////////changed =
                        if(color[j] == temp_rand)
                            flag2 = true ;
                    }
                    if(!flag2)
                        color[i]=temp_rand ;
                }
            }
        }
    }else{
        for (int j = 0; j < number_of_colors; ++j) {
            color[j] = color_selection[j] ;
        }
    }
    initial_balls_color_1(renderer , W , H , balls , balls_out , colors , output , color);
}

int initializing_play_mode_3(SDL_Renderer * renderer , int W , int H , ball2d balls[n_rows][n_columns] , ball2d balls_out[n_rows_out][n_columns] , color2d colors[9] , vector<array<int, 2>> &output , int color[number_of_colors]){
    n_rows_out_manual = 15 ;
    vel_balls_go_down_manual = 1.5 ;
    vel_balls_go_down_initial_increase = 60 ;// preferred at around 60 for normal
    vel_balls_go_down_rate_increase = 0.15 ;// preferred at around 0.1 for normal
    vel_balls_go_down_coll_reduction = 0.15 ;// preferred at around 0.2 for normal
    vel_balls_go_down_coll_recovery_time = 4 ;// preferred at around 1 for normal
    amount_multi_color_flying_ball = 15 ; // 1 in every number that you put there will be a ball with multi colors
    difficulty_gray_balls = 0.2; // !!! IT SHOULD BE IN THIS FORM AND BE FLOAT // the lower it is the less compacted will be the gray balls
    amount_multicolor_ball = 0.8; // !!! IT SHOULD BE IN THIS FORM AND BE FLOAT // the lower it is the less compacted will be the multi_colored will be the balls
    amount_locked_ball = 0.2;
    volume_of_balls = 1; // !!! IT SHOULD BE IN THIS FORM AND BE FLOAT // and for now it can not be 1 or 0 ...
    difficulty_of_colors = 4; // It should be bigger than 0 and its int , the bigger it is the less difficult the map would get for normal around 5
    if(color_random_chosen){
        for (int i = 0; i < number_of_colors; ++i) {
            if(i == 0 && gray_balls_exist){
                color[i] = 8 ;
            }else{
                bool flag2 = true ;
                while(flag2){
                    int temp_rand = rand()%8 ;
                    flag2 = false ;
                    for (int j = 0; j < i; ++j) {
                        if(color[j] == temp_rand)
                            flag2 = true ;
                    }
                    if(!flag2)
                        color[i]=temp_rand ;
                }
            }
        }
    }else{
        for (int j = 0; j < number_of_colors; ++j) {
            color[j] = color_selection[j] ;
        }
    }
    initial_balls_color_1(renderer , W , H , balls , balls_out , colors , output , color);
}

int initializing_play_mode_4(SDL_Renderer * renderer , int W , int H , ball2d balls[n_rows][n_columns] , ball2d balls_out[n_rows_out][n_columns] , color2d colors[9] , vector<array<int, 2>> &output , int color[number_of_colors]){
    n_rows_out_manual = 25 ;
    vel_balls_go_down_manual = 2 ;
    vel_balls_go_down_initial_increase = 60 ;// preferred at around 60 for normal
    vel_balls_go_down_rate_increase = 0.2 ;// preferred at around 0.1 for normal
    vel_balls_go_down_coll_reduction = 0.2 ;// preferred at around 0.2 for normal
    vel_balls_go_down_coll_recovery_time = 3 ;// preferred at around 1 for normal
    amount_multi_color_flying_ball = 20 ; // 1 in every number that you put there will be a ball with multi colors
    difficulty_gray_balls = 0.5; // !!! IT SHOULD BE IN THIS FORM AND BE FLOAT // the lower it is the less compacted will be the gray balls
    amount_multicolor_ball = 0.5; // !!! IT SHOULD BE IN THIS FORM AND BE FLOAT // the lower it is the less compacted will be the multi_colored will be the balls
    amount_locked_ball = 0.5;
    volume_of_balls = 1; // !!! IT SHOULD BE IN THIS FORM AND BE FLOAT // and for now it can not be 1 or 0 ...
    difficulty_of_colors = 3; // It should be bigger than 0 and its int , the bigger it is the less difficult the map would get for normal around 5
    if(color_random_chosen){
        for (int i = 0; i < number_of_colors; ++i) {
            if(i == 0 && gray_balls_exist){
                color[i] = 8 ;
            }else{
                bool flag2 = true ;
                while(flag2){
                    int temp_rand = rand()%8 ;
                    flag2 = false ;
                    for (int j = 0; j < i; ++j) {
                        if(color[j] == temp_rand)
                            flag2 = true ;
                    }
                    if(!flag2)
                        color[i]=temp_rand ;
                }
            }
        }
    }else{
        for (int j = 0; j < number_of_colors; ++j) {
            color[j] = color_selection[j] ;
        }
    }
    initial_balls_color_1(renderer , W , H , balls , balls_out , colors , output , color);
}

int initializing_play_mode_5(SDL_Renderer * renderer , int W , int H , ball2d balls[n_rows][n_columns] , ball2d balls_out[n_rows_out][n_columns] , color2d colors[9] , vector<array<int, 2>> &output , int color[number_of_colors]){
    n_rows_out_manual = 40 ;
    vel_balls_go_down_manual = 2.5 ;
    vel_balls_go_down_initial_increase = 60 ;// preferred at around 60 for normal
    vel_balls_go_down_rate_increase = 0.8 ;// preferred at around 0.1 for normal
    vel_balls_go_down_coll_reduction = 0.15 ;// preferred at around 0.2 for normal
    vel_balls_go_down_coll_recovery_time = 2 ;// preferred at around 1 for normal
    amount_multi_color_flying_ball = 30; // 1 in every number that you put there will be a ball with multi colors
    difficulty_gray_balls = 0.5; // !!! IT SHOULD BE IN THIS FORM AND BE FLOAT // the lower it is the less compacted will be the gray balls
    amount_multicolor_ball = 0.5; // !!! IT SHOULD BE IN THIS FORM AND BE FLOAT // the lower it is the less compacted will be the multi_colored will be the balls
    amount_locked_ball = 0.5;
    volume_of_balls = 1; // !!! IT SHOULD BE IN THIS FORM AND BE FLOAT // and for now it can not be 1 or 0 ...
    difficulty_of_colors = 3; // It should be bigger than 0 and its int , the bigger it is the less difficult the map would get for normal around 5
    if(color_random_chosen){
        for (int i = 0; i < number_of_colors; ++i) {
            if(i == 0 && gray_balls_exist){
                color[i] = 8 ;
            }else{
                bool flag2 = true ;
                while(flag2){
                    int temp_rand = rand()%8 ;
                    flag2 = false ;
                    for (int j = 0; j < i; ++j) {
                        if(color[j] == temp_rand)
                            flag2 = true ;
                    }
                    if(!flag2)
                        color[i]=temp_rand ;
                }
            }
        }
    }else{
        for (int j = 0; j < number_of_colors; ++j) {
            color[j] = color_selection[j] ;
        }
    }
    initial_balls_color_1(renderer , W , H , balls , balls_out , colors , output , color);
}

int initializing_play_mode_6(SDL_Renderer * renderer , int W , int H , ball2d balls[n_rows][n_columns] , ball2d balls_out[n_rows_out][n_columns] , color2d colors[9] , vector<array<int, 2>> &output , int color[number_of_colors]){
    timer_mode_allowed = true ;
    n_rows_out_manual = 2000 ;
    vel_balls_go_down_manual = 1.5 ;
    vel_balls_go_down_initial_increase = 60 ;// preferred at around 60 for normal
    vel_balls_go_down_rate_increase = 0.4 ;// preferred at around 0.1 for normal
    vel_balls_go_down_coll_reduction = 0.5 ;// preferred at around 0.2 for normal
    vel_balls_go_down_coll_recovery_time = 1 ;// preferred at around 1 for normal
    amount_multi_color_flying_ball = 15; // 1 in every number that you put there will be a ball with multi colors
    difficulty_gray_balls = 0.2; // !!! IT SHOULD BE IN THIS FORM AND BE FLOAT // the lower it is the less compacted will be the gray balls
    amount_multicolor_ball = 0.5; // !!! IT SHOULD BE IN THIS FORM AND BE FLOAT // the lower it is the less compacted will be the multi_colored will be the balls
    amount_locked_ball = 0.2;
    volume_of_balls = 1; // !!! IT SHOULD BE IN THIS FORM AND BE FLOAT // and for now it can not be 1 or 0 ...
    difficulty_of_colors = 6; // It should be bigger than 0 and its int , the bigger it is the less difficult the map would get for normal around 5
    if(color_random_chosen){
        for (int i = 0; i < number_of_colors; ++i) {
            if(i == 0 && gray_balls_exist){
                color[i] = 8 ;
            }else{
                bool flag2 = true ;
                while(flag2){
                    int temp_rand = rand()%8 ;
                    flag2 = false ;
                    for (int j = 0; j < i; ++j) {
                        if(color[j] == temp_rand)
                            flag2 = true ;
                    }
                    if(!flag2)
                        color[i]=temp_rand ;
                }
            }
        }
    }else{
        for (int j = 0; j < number_of_colors; ++j) {
            color[j] = color_selection[j] ;
        }
    }
    initial_balls_color_1(renderer , W , H , balls , balls_out , colors , output , color);
}

double find_minimum_index(vector<double> arr) {
    int min_index = min_element(arr.begin(), arr.end()) - arr.begin();
    return min_index;
}

double distance(double x1 , double y1 , double x2 , double y2)
{
    return sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)) ;
}

int draw_target(SDL_Renderer * renderer , int W , int H , int mouseX , int mouseY , flying_color2d flyingColor , color2d colors[8]){
    int R = int ( W/ (n_columns*2+1) );
    int W_min = (W-(R*(n_columns*2+1)))/2;
    int W_max = (W-(R*(n_columns*2+1)))/2+((R*(n_columns*2+1)));

    ball2d bullet;
    bullet.x = W/2;
    bullet.y = H - R;

    double u_h1 , u_l1 , u_ch1 , urx , ury;
    u_l1 =  mouseX - bullet.x; //u_length1
    u_h1 =  mouseY - bullet.y; //u_height1
    u_ch1 = sqrt(pow(u_h1,2)+pow(u_l1,2)); //u_chord1
    urx = (u_l1 / u_ch1); //u_ratio_x
    ury = (u_h1 / u_ch1); //u_ratio_y
    double vel_x = urx * vel_flying_ball * R;
    double vel_y = ury * vel_flying_ball * R;
    int wgo = 0; //out put of func: wanna_go_out
    double R_target = R*0.1;
    int i = 1;
    while(bullet.y > 0){
        wgo =wanna_go_out(bullet.x , bullet.y , W , H , R);
        if(wgo == 2 || wgo == 4){
            vel_x *= -1;
        }
        bullet.x += vel_x;
        bullet.y += vel_y;
        if(flyingColor.multi_color){
            if(i >= number_of_colors-1)
                i = 1 ;
            filledEllipseRGBA(renderer, bullet.x, bullet.y, R_target , R_target , colors[color[i]].r, colors[color[i]].g, colors[color[i]].b, 150);
            i++ ;
        }else
            filledEllipseRGBA(renderer, bullet.x, bullet.y, R_target , R_target , flyingColor.r, flyingColor.g, flyingColor.b, 150);
    }

    return 1;
}

int initial_balls_coordinate(SDL_Renderer * renderer , int W , int H , ball2d balls[n_rows][n_columns]){
    int R = int ( W/ (n_columns*2+1) );
    int W_min = (W-(R*(n_columns*2+1)))/2;
    int W_max = (W-(R*(n_columns*2+1)))/2+((R*(n_columns*2+1)));

    int x11 , y11 = -sqrt(2*R*2*R - R*R) , d11=sqrt(2*R*2*R - R*R);
    for (int j = 0; j < n_rows ; j++) {
        if(j%2==0)
        {
            x11=R;
            for (int i = 0; i < n_columns; i++) {
                balls[j][i].x = x11 + W_min;
                x11+=2*R ;
                balls[j][i].y = y11 ;
                balls[j][i].eo=-1;
            }
        }else{
            x11=2*R ;
            for (int i = 0; i < n_columns; i++) {
                balls[j][i].x = x11 + W_min;
                x11+=2*R ;
                balls[j][i].y = y11 ;
            }
        }
        y11+=d11 ;
    }

    return 1;
}

int update_balls(SDL_Renderer * renderer , int W , int H , ball2d balls[n_rows][n_columns] , ball2d balls_out[n_rows][n_columns]){
    int R = int ( W/ (n_columns*2+1) );
    int W_min = (W-(R*(n_columns*2+1)))/2;
    int W_max = (W-(R*(n_columns*2+1)))/2+((R*(n_columns*2+1)));

    if(k_upbl==-1)
        return 0;
    if (u_b1) {
        int x11 = W_min + 2 * R;
        for (int j = 0; j < n_columns; j++) {
            balls_out[k_upbl][j].x = x11;
            balls_out[k_upbl][j].y = balls[0][0].y - sqrt(3) * R;
            x11 += 2 * R;
        }
        u_b1 = !u_b1;
    } else {
        int x11 = W_min + R;
        for (int j = 0; j < n_columns; j++) {
            balls_out[k_upbl][j].x = x11;
            balls_out[k_upbl][j].y = balls[0][0].y - sqrt(3) * R;
            balls_out[k_upbl][j].eo = -1;
            x11 += 2 * R;
        }
        u_b1 = !u_b1;
    }
    for (int i = n_rows - 2; 0 <= i; i--) {
        for (int j = 0; j < n_columns; j++) {
            balls[i + 1][j] = balls[i][j];
        }
    }
    for (int j = 0; j < n_columns; j++) {
        balls[0][j] = balls_out[k_upbl][j];
    }
    k_upbl--;
    return 1;
}

int draw_balls2(SDL_Renderer * renderer , int W , int H , ball2d balls[n_rows][n_columns]){
    int R = int ( W/ (n_columns*2+1) );

    for(int i=1 ; i<n_rows ; i++){
        for(int j=0 ; j<n_columns ; j++){
            if(balls[i][j].eo == 1)
                filledEllipseRGBA(renderer, balls[i][j].x, balls[i][j].y , R/2, R/2, 255 , 255 , 255 , 255);
            else{
                filledEllipseRGBA(renderer, balls[i][j].x, balls[i][j].y , R/4, R/4, 255 , 255 , 255 , 255);
            }
        }
    }

    return 1;
}

int draw_balls(SDL_Renderer * renderer , int W , int H , ball2d balls[n_rows][n_columns]){
    int R = int ( W/ (n_columns*2+1) );
    if(ceil_y >= -2 * R) {
        for (int i = 0; i < n_rows; i++) {
            for (int j = 0; j < n_columns; j++) {
                if (balls[i][j].is_exist) {
                    filledEllipseRGBA(renderer, balls[i][j].x, balls[i][j].y, R, R, balls[i][j].rgb[0],
                                      balls[i][j].rgb[1], balls[i][j].rgb[2], 255);
                    if (balls[i][j].multicolor) {
                        filledEllipseRGBA(renderer, balls[i][j].x, balls[i][j].y, R / 2, R / 2, balls[i][j].rgb1[0],
                                          balls[i][j].rgb1[1], balls[i][j].rgb1[2], 255);
                    } else if (balls[i][j].locked) {
                        double newR = (R / sqrt(2));
                        //thickLineRGBA(renderer, balls[i][j].x - newR, balls[i][j].y - newR, balls[i][j].x + newR,balls[i][j].y + newR, R / 7, 0, 0, 0, 255);
                        thickLineRGBA(renderer, balls[i][j].x - newR, balls[i][j].y + newR, balls[i][j].x + newR,balls[i][j].y - newR, R / 7, 0, 0, 0, 255);
                    }
//                    if(balls[i][j].connect){
//                        filledEllipseRGBA(renderer, balls[i][j].x, balls[i][j].y, R/3, R/3, 255,255, 255, 255);
//                    }
                }
//                else
//                    ellipseRGBA(renderer, balls[i][j].x, balls[i][j].y , R, R, 0 , 255 , 0 , 255); ///// DEBUG MODE
            }
        }
}           else{
        for(int i=1 ; i<n_rows ; i++){
            for(int j=0 ; j<n_columns ; j++){
                if(balls[i][j].is_exist){
                    filledEllipseRGBA(renderer, balls[i][j].x, balls[i][j].y , R, R, balls[i][j].rgb[0] , balls[i][j].rgb[1] , balls[i][j].rgb[2] , 255);
                    if(balls[i][j].multicolor){
                        filledEllipseRGBA(renderer, balls[i][j].x, balls[i][j].y , R/2, R/2, balls[i][j].rgb1[0] , balls[i][j].rgb1[1] , balls[i][j].rgb1[2] , 255);
                    }else if(balls[i][j].locked){
                        double newR = (R/sqrt(2)) ;
                        //thickLineRGBA(renderer , balls[i][j].x-newR , balls[i][j].y-newR , balls[i][j].x+newR , balls[i][j].y+newR , R/7 , 0 , 0 , 0 , 255 );
                        thickLineRGBA(renderer , balls[i][j].x-newR , balls[i][j].y+newR , balls[i][j].x+newR , balls[i][j].y-newR , R/7 , 0 , 0 , 0 , 255 );
                    }
//                    if(balls[i][j].connect){
//                        filledEllipseRGBA(renderer, balls[i][j].x, balls[i][j].y, R/3, R/3, 255,255, 255, 255);
//                    }
                }
//                else
//                    ellipseRGBA(renderer, balls[i][j].x, balls[i][j].y , R, R, 0 , 255 , 0 , 255); ///// DEBUG MODE
            }
        }
    }
    return 1;
}

int wanna_go_out(int x , int y , int W , int H , int R){
    if(x-R < 0)
        return 4;
    if(W < x+R)
        return 2;
    if(H < y+R)
        return 3;
    if(y-R < 0)
        return 1;
    return 0;
}

int find_sum_2_to_1_given (float input , int &output1 , int &output2){
    if (input == 0) {           // If x is 0, print a special message and return
        return 0 ;
    }
    if (input == 1) {           // If x is 1, print another special message and return
        return 1 ;
    }
    double closest = 0;
    double diff = input;
    for (int i = 1; i <= 100; i++) {
        for (int j = i; j <= 100; j++) {
            double ii = invert(i);
            double ij = invert(j);
            double sum = ii + ij;
            if (abs(sum - input) < diff) {
                output1 = i;
                output2 = j;
                closest = sum;
                diff = abs(sum - input);
            }
        }
    }
    return -1 ;
}

int initial_balls_color_0(SDL_Renderer * renderer , int W , int H , ball2d balls[n_rows][n_columns] , ball2d balls_out[n_rows_out][n_columns] , color2d colors[9] , vector<array<int, 2>> &output , int color[number_of_colors]){
    int output1 = 1000000000 , output2 = 1000000000 , output11 = 1000000000 , output21 = 1000000000 , output12 = 1000000000 , output22 = 1000000000 , output13 = 1000000000 , output23 = 1000000000;
    float input = 1 - volume_of_balls;
    float input1 = 1 - difficulty_gray_balls;
    float input2 = amount_multicolor_ball;
    float input3 = amount_locked_ball;
    int result = find_sum_2_to_1_given ( input , output1 , output2) ;
    int result1 = find_sum_2_to_1_given ( input1 , output11 , output21) ;
    int result2 = find_sum_2_to_1_given ( input2 , output12 , output22) ;
    int result3 = find_sum_2_to_1_given ( input3 , output13 , output23) ;
    if(result == 0) {
        output1 = 1000000000;
        output2 = 1000000000;
    }else if(result == 1){
        cout << "# - #define volume_of_balls // CAN NOT BE (0) " << endl ;
        return -1 ;
    }
    if(result1 == 0) {
        output11 = 1000000000;
        output21 = 1000000000;
    }else if(result1 == 1){
        cout << "# - #define volume_of_balls // CAN NOT BE (0) " << endl ;
        return -1 ;
    }
    if(result2 == 0) {
        output12 = 1000000000;
        output22 = 1000000000;
    }else if(result2 == 1){
        cout << "# - #define volume_of_balls // CAN NOT BE (0) " << endl ;
        return -1 ;
    }
    if(result3 == 0) {
        output13 = 1000000000;
        output23 = 1000000000;
    }else if(result3 == 1){
        cout << "# - #define volume_of_balls // CAN NOT BE (0) " << endl ;
        return -1 ;
    }
    int random_number , random_number_color ;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < n_columns ; ++j) {
            random_number = rand() ;
            if(random_number%output1==0 || random_number%output2==0){
                balls[i][j].is_exist = false;
            }else{
                vector<int> colors_that_exist ;
                int counter_of_colors = 0 ;
                if(k_upbl > 1){
                    finding_neighbors_balls_new(i, j, balls , output) ;
                }else
                    finding_neighbors_balls(i, j, balls , output) ;
                for (auto & p : output) {
                    if(balls[p[0]][p[1]].is_exist){
                        for (int k = 0; k < number_of_colors; ++k) {
                            if(balls[p[0]][p[1]].rgb[0] == colors[color[k]].r && balls[p[0]][p[1]].rgb[1] == colors[color[k]].g && balls[p[0]][p[1]].rgb[2] == colors[color[k]].b){
                                counter_of_colors++ ;
                                colors_that_exist.push_back(k) ;
                            }
                        }
                    }
                }
                if(rand()%difficulty_of_colors == 0 || counter_of_colors == 0){
                    random_number_color = rand() % number_of_colors ;
                    if((rand() % output12 == 0 || rand() % output22 == 0) && multicolor_ball && color[random_number_color]!=8){
                        balls[i][j].multicolor = true ;
                        colors[color[random_number_color]].is_exist = true ;
                        balls[i][j].rgb[0] = colors[color[random_number_color]].r ;
                        balls[i][j].rgb[1] = colors[color[random_number_color]].g ;
                        balls[i][j].rgb[2] = colors[color[random_number_color]].b ;
                        random_number_color = rand() % number_of_colors ;
                        while(color[random_number_color]==8)
                            random_number_color = rand() % number_of_colors ;
                        colors[color[random_number_color]].is_exist = true ;
                        balls[i][j].rgb1[0] = colors[color[random_number_color]].r ;
                        balls[i][j].rgb1[1] = colors[color[random_number_color]].g ;
                        balls[i][j].rgb1[2] = colors[color[random_number_color]].b ;
                        balls[i][j].is_exist = true;
                    }else{
                        random_number_color = rand() % number_of_colors ;
                        if(locked_ball && (rand() % output13 == 0 || rand() % output23 == 0) && color[random_number_color] != 8){
                            colors[color[random_number_color]].is_exist = true ;
                            balls[i][j].rgb[0] = colors[color[random_number_color]].r ;
                            balls[i][j].rgb[1] = colors[color[random_number_color]].g ;
                            balls[i][j].rgb[2] = colors[color[random_number_color]].b ;
                            balls[i][j].is_exist = true;
                            balls[i][j].locked = true;
                        }else{
                            colors[color[random_number_color]].is_exist = true ;
                            balls[i][j].rgb[0] = colors[color[random_number_color]].r ;
                            balls[i][j].rgb[1] = colors[color[random_number_color]].g ;
                            balls[i][j].rgb[2] = colors[color[random_number_color]].b ;
                            balls[i][j].is_exist = true;
                        }
                    }
                }else{
                    int found_color = rand()%counter_of_colors ;
                    if(color[colors_that_exist[found_color]] == 8 && gray_balls_exist && (rand()%output11 == 0 || rand()%output21 == 0)){
                        random_number_color = rand() % number_of_colors ;
                        if((rand() % output12 == 0 || rand() % output22 == 0) && multicolor_ball && color[random_number_color]!=8){
                            balls[i][j].multicolor = true ;
                            colors[color[random_number_color]].is_exist = true ;
                            balls[i][j].rgb[0] = colors[color[random_number_color]].r ;
                            balls[i][j].rgb[1] = colors[color[random_number_color]].g ;
                            balls[i][j].rgb[2] = colors[color[random_number_color]].b ;
                            random_number_color = rand() % number_of_colors ;
                            while(color[random_number_color]==8)
                                random_number_color = rand() % number_of_colors ;
                            colors[color[random_number_color]].is_exist = true ;
                            balls[i][j].rgb1[0] = colors[color[random_number_color]].r ;
                            balls[i][j].rgb1[1] = colors[color[random_number_color]].g ;
                            balls[i][j].rgb1[2] = colors[color[random_number_color]].b ;
                            balls[i][j].is_exist = true;
                        }else{
                            random_number_color = rand() % number_of_colors ;
                            if(locked_ball && (rand() % output13 == 0 || rand() % output23 == 0) && color[random_number_color] != 8){
                                colors[color[random_number_color]].is_exist = true ;
                                balls[i][j].rgb[0] = colors[color[random_number_color]].r ;
                                balls[i][j].rgb[1] = colors[color[random_number_color]].g ;
                                balls[i][j].rgb[2] = colors[color[random_number_color]].b ;
                                balls[i][j].is_exist = true;
                                balls[i][j].locked = true;
                            }else{
                                colors[color[random_number_color]].is_exist = true ;
                                balls[i][j].rgb[0] = colors[color[random_number_color]].r ;
                                balls[i][j].rgb[1] = colors[color[random_number_color]].g ;
                                balls[i][j].rgb[2] = colors[color[random_number_color]].b ;
                                balls[i][j].is_exist = true;
                            }
                        }
                    }else{
                        if((rand() % output12 == 0 || rand() % output22 == 0) && multicolor_ball && color[colors_that_exist[found_color]] != 8 ){
                            balls[i][j].multicolor = true ;
                            colors[color[colors_that_exist[found_color]]].is_exist = true ;
                            balls[i][j].rgb[0] = colors[color[colors_that_exist[found_color]]].r ;
                            balls[i][j].rgb[1] = colors[color[colors_that_exist[found_color]]].g ;
                            balls[i][j].rgb[2] = colors[color[colors_that_exist[found_color]]].b ;
                            while(color[colors_that_exist[found_color]]==8)
                                found_color = rand()%counter_of_colors ;
                            colors[color[colors_that_exist[found_color]]].is_exist = true ;
                            balls[i][j].rgb1[0] = colors[color[colors_that_exist[found_color]]].r ;
                            balls[i][j].rgb1[1] = colors[color[colors_that_exist[found_color]]].g ;
                            balls[i][j].rgb1[2] = colors[color[colors_that_exist[found_color]]].b ;
                            balls[i][j].is_exist = true;
                        }else{
                            if(locked_ball && (rand() % output13 == 0 || rand() % output23 == 0) && color[colors_that_exist[found_color]] != 8){
                                colors[color[colors_that_exist[found_color]]].is_exist = true ;
                                balls[i][j].rgb[0] = colors[color[colors_that_exist[found_color]]].r ;
                                balls[i][j].rgb[1] = colors[color[colors_that_exist[found_color]]].g ;
                                balls[i][j].rgb[2] = colors[color[colors_that_exist[found_color]]].b ;
                                balls[i][j].is_exist = true;
                                balls[i][j].locked = true;
                            }else{
                                colors[color[colors_that_exist[found_color]]].is_exist = true ;
                                balls[i][j].rgb[0] = colors[color[colors_that_exist[found_color]]].r ;
                                balls[i][j].rgb[1] = colors[color[colors_that_exist[found_color]]].g ;
                                balls[i][j].rgb[2] = colors[color[colors_that_exist[found_color]]].b ;
                                balls[i][j].is_exist = true;
                            }
                        }
                    }
                }
                colors_that_exist.clear() ;
            }
        }
    }
    for (int i = n_rows_out_manual-1 ; i >= 0; i--) {
        for (int j = 0; j < n_columns ; ++j) {
            bool found = true ;
            random_number = rand() ;
            if(random_number%output1==0 || random_number%output2==0){
                balls_out[i][j].is_exist = false;
            }else{
                vector<int> colors_that_exist ;
                int counter_of_colors = 0 ;
                if(k_upbl > 1){
                    finding_neighbors_balls_new(i, j, balls , output) ;
                }else
                    finding_neighbors_balls(i, j, balls_out , output) ;
                for (auto & p : output) {
                    if(balls_out[p[0]][p[1]].is_exist){
                        for (int k = 0; k < number_of_colors; ++k) {
                            if(balls_out[p[0]][p[1]].rgb[0] == colors[color[k]].r && balls_out[p[0]][p[1]].rgb[1] == colors[color[k]].g && balls_out[p[0]][p[1]].rgb[2] == colors[color[k]].b){
                                counter_of_colors++ ;
                                colors_that_exist.push_back(k) ;
                            }
                        }
                    }
                }
                if(rand()%difficulty_of_colors == 0 || counter_of_colors == 0){
                    random_number_color = rand() % number_of_colors ;
                    if((rand() % output12 == 0|| rand() % output22 == 0) && multicolor_ball && color[random_number_color]!=8){
                        balls_out[i][j].multicolor = true ;
                        colors[color[random_number_color]].is_exist = true ;
                        balls_out[i][j].rgb[0] = colors[color[random_number_color]].r ;
                        balls_out[i][j].rgb[1] = colors[color[random_number_color]].g ;
                        balls_out[i][j].rgb[2] = colors[color[random_number_color]].b ;
                        random_number_color = rand() % number_of_colors ;
                        while(color[random_number_color]==8)
                            random_number_color = rand() % number_of_colors ;
                        colors[color[random_number_color]].is_exist = true ;
                        balls_out[i][j].rgb1[0] = colors[color[random_number_color]].r ;
                        balls_out[i][j].rgb1[1] = colors[color[random_number_color]].g ;
                        balls_out[i][j].rgb1[2] = colors[color[random_number_color]].b ;
                        balls_out[i][j].is_exist = true;
                    }else{
                        random_number_color = rand() % number_of_colors ;
                        if(locked_ball && (rand() % output13 == 0 || rand() % output23 == 0) && color[random_number_color] != 8){
                            colors[color[random_number_color]].is_exist = true ;
                            balls_out[i][j].rgb[0] = colors[color[random_number_color]].r ;
                            balls_out[i][j].rgb[1] = colors[color[random_number_color]].g ;
                            balls_out[i][j].rgb[2] = colors[color[random_number_color]].b ;
                            balls_out[i][j].is_exist = true;
                            balls_out[i][j].locked = true;
                        }else{
                            colors[color[random_number_color]].is_exist = true ;
                            balls_out[i][j].rgb[0] = colors[color[random_number_color]].r ;
                            balls_out[i][j].rgb[1] = colors[color[random_number_color]].g ;
                            balls_out[i][j].rgb[2] = colors[color[random_number_color]].b ;
                            balls_out[i][j].is_exist = true;
                        }
                    }
                }else{
                    int found_color = rand()%counter_of_colors ;
                    if(color[colors_that_exist[found_color]] == 8 && gray_balls_exist && (rand()%output11 == 0 || rand()%output21 == 0)){
                        random_number_color = rand() % number_of_colors ;
                        if((rand() % output12 == 0 || rand() % output22 == 0) && multicolor_ball && color[random_number_color]!=8){
                            balls_out[i][j].multicolor = true ;
                            colors[color[random_number_color]].is_exist = true ;
                            balls_out[i][j].rgb[0] = colors[color[random_number_color]].r ;
                            balls_out[i][j].rgb[1] = colors[color[random_number_color]].g ;
                            balls_out[i][j].rgb[2] = colors[color[random_number_color]].b ;
                            random_number_color = rand() % number_of_colors ;
                            while(color[random_number_color]==8)
                                random_number_color = rand() % number_of_colors ;
                            colors[color[random_number_color]].is_exist = true ;
                            balls_out[i][j].rgb1[0] = colors[color[random_number_color]].r ;
                            balls_out[i][j].rgb1[1] = colors[color[random_number_color]].g ;
                            balls_out[i][j].rgb1[2] = colors[color[random_number_color]].b ;
                            balls_out[i][j].is_exist = true;
                        }else{
                            random_number_color = rand() % number_of_colors ;
                            if(locked_ball && (rand() % output13 == 0 || rand() % output23 == 0) && color[random_number_color] != 8){
                                colors[color[random_number_color]].is_exist = true ;
                                balls_out[i][j].rgb[0] = colors[color[random_number_color]].r ;
                                balls_out[i][j].rgb[1] = colors[color[random_number_color]].g ;
                                balls_out[i][j].rgb[2] = colors[color[random_number_color]].b ;
                                balls_out[i][j].is_exist = true;
                                balls_out[i][j].locked = true;
                            }else{
                                colors[color[random_number_color]].is_exist = true ;
                                balls_out[i][j].rgb[0] = colors[color[random_number_color]].r ;
                                balls_out[i][j].rgb[1] = colors[color[random_number_color]].g ;
                                balls_out[i][j].rgb[2] = colors[color[random_number_color]].b ;
                                balls_out[i][j].is_exist = true;
                            }
                        }
                    }else{
                        if((rand() % output12 == 0 || rand() % output22 == 0) && multicolor_ball && color[colors_that_exist[found_color]] != 8 ){
                            balls_out[i][j].multicolor = true ;
                            colors[color[colors_that_exist[found_color]]].is_exist = true ;
                            balls_out[i][j].rgb[0] = colors[color[colors_that_exist[found_color]]].r ;
                            balls_out[i][j].rgb[1] = colors[color[colors_that_exist[found_color]]].g ;
                            balls_out[i][j].rgb[2] = colors[color[colors_that_exist[found_color]]].b ;
                            while(color[colors_that_exist[found_color]]==8)
                                found_color = rand()%counter_of_colors ;
                            colors[color[colors_that_exist[found_color]]].is_exist = true ;
                            balls_out[i][j].rgb1[0] = colors[color[colors_that_exist[found_color]]].r ;
                            balls_out[i][j].rgb1[1] = colors[color[colors_that_exist[found_color]]].g ;
                            balls_out[i][j].rgb1[2] = colors[color[colors_that_exist[found_color]]].b ;
                            balls_out[i][j].is_exist = true;
                        }else{
                            if(locked_ball && (rand() % output13 == 0 || rand() % output23 == 0) && color[colors_that_exist[found_color]] != 8){
                                colors[color[colors_that_exist[found_color]]].is_exist = true ;
                                balls_out[i][j].rgb[0] = colors[color[colors_that_exist[found_color]]].r ;
                                balls_out[i][j].rgb[1] = colors[color[colors_that_exist[found_color]]].g ;
                                balls_out[i][j].rgb[2] = colors[color[colors_that_exist[found_color]]].b ;
                                balls_out[i][j].is_exist = true;
                                balls_out[i][j].locked = true;
                            }else{
                                colors[color[colors_that_exist[found_color]]].is_exist = true ;
                                balls_out[i][j].rgb[0] = colors[color[colors_that_exist[found_color]]].r ;
                                balls_out[i][j].rgb[1] = colors[color[colors_that_exist[found_color]]].g ;
                                balls_out[i][j].rgb[2] = colors[color[colors_that_exist[found_color]]].b ;
                                balls_out[i][j].is_exist = true;
                            }
                        }
                    }
                }
                colors_that_exist.clear() ;
            }
        }
//        if(i % 10 == 0 && difficulty_of_colors > 1){
//            difficulty_of_colors-- ;
//
//        }
//        if(i % 10 == 0){
//            output12++ ;
//            if(i % 20 == 0 ){
//                output22++ ;
//            }
//        }
//        if(i % 10 == 0 && output13 > 2){
//            output13-- ;
//            if(i % 20 == 0){
//                output23-- ;
//            }
//        }
//        if(i % 10 == 0 && output11 > 2){
//            output11-- ;
//            if(i % 20 == 0){
//                output21-- ;
//            }
//        }
//      //cout << difficulty_of_colors << "   " << output11 << " " << output21 << "   " << output12 << " " << output22 << "   " << output13 << " " << output23 <<endl ;
    }
    return 1;
}

int initial_balls_color_1(SDL_Renderer * renderer , int W , int H , ball2d balls[n_rows][n_columns] , ball2d balls_out[n_rows_out][n_columns] , color2d colors[9] , vector<array<int, 2>> &output , int color[number_of_colors]){
    int output1 = 1000000000 , output2 = 1000000000 , output11 = 1000000000 , output21 = 1000000000 , output12 = 1000000000 , output22 = 1000000000 , output13 = 1000000000 , output23 = 1000000000;
    float input = 1 - volume_of_balls;
    float input1 = 1 - difficulty_gray_balls;
    float input2 = amount_multicolor_ball;
    float input3 = amount_locked_ball;
    int result = find_sum_2_to_1_given ( input , output1 , output2) ;
    int result1 = find_sum_2_to_1_given ( input1 , output11 , output21) ;
    int result2 = find_sum_2_to_1_given ( input2 , output12 , output22) ;
    int result3 = find_sum_2_to_1_given ( input3 , output13 , output23) ;
    if(result == 0) {
        output1 = 1000000000;
        output2 = 1000000000;
    }else if(result == 1){
        cout << "# - #define volume_of_balls // CAN NOT BE (0) " << endl ;
        return -1 ;
    }
    if(result1 == 0) {
        output11 = 1000000000;
        output21 = 1000000000;
    }else if(result1 == 1){
        cout << "# - #define volume_of_balls // CAN NOT BE (0) " << endl ;
        return -1 ;
    }
    if(result2 == 0) {
        output12 = 1000000000;
        output22 = 1000000000;
    }else if(result2 == 1){
        cout << "# - #define volume_of_balls // CAN NOT BE (0) " << endl ;
        return -1 ;
    }
    if(result3 == 0) {
        output13 = 1000000000;
        output23 = 1000000000;
    }else if(result3 == 1){
        cout << "# - #define volume_of_balls // CAN NOT BE (0) " << endl ;
        return -1 ;
    }
    int random_number , random_number_color ;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < n_columns ; ++j) {
            random_number = rand() ;
            if((random_number%output1==0 || random_number%output2==0 )&& false){
                balls[i][j].is_exist = false;
            }else{
                vector<int> colors_that_exist ;
                int counter_of_colors = 0 ;
                if(k_upbl > 1){
                    finding_neighbors_balls_new(i, j, balls , output) ;
                }else
                    finding_neighbors_balls(i, j, balls , output) ;
                for (auto & p : output) {
                    if(balls[p[0]][p[1]].is_exist){
                        for (int k = 0; k < number_of_colors; ++k) {
                            if(balls[p[0]][p[1]].rgb[0] == colors[color[k]].r && balls[p[0]][p[1]].rgb[1] == colors[color[k]].g && balls[p[0]][p[1]].rgb[2] == colors[color[k]].b){
                                counter_of_colors++ ;
                                colors_that_exist.push_back(k) ;
                            }
                        }
                    }
                }
                if(rand()%difficulty_of_colors == 0 || counter_of_colors == 0){
                    random_number_color = rand() % number_of_colors ;
                    if((rand() % output12 == 0 || rand() % output22 == 0) && multicolor_ball && color[random_number_color]!=8){
                        balls[i][j].multicolor = true ;
                        colors[color[random_number_color]].is_exist = true ;
                        balls[i][j].rgb[0] = colors[color[random_number_color]].r ;
                        balls[i][j].rgb[1] = colors[color[random_number_color]].g ;
                        balls[i][j].rgb[2] = colors[color[random_number_color]].b ;
                        random_number_color = rand() % number_of_colors ;
                        while(color[random_number_color]==8)
                            random_number_color = rand() % number_of_colors ;
                        colors[color[random_number_color]].is_exist = true ;
                        balls[i][j].rgb1[0] = colors[color[random_number_color]].r ;
                        balls[i][j].rgb1[1] = colors[color[random_number_color]].g ;
                        balls[i][j].rgb1[2] = colors[color[random_number_color]].b ;
                        balls[i][j].is_exist = true;
                    }else{
                        random_number_color = rand() % number_of_colors ;
                        if(locked_ball && (rand() % output13 == 0 || rand() % output23 == 0) && color[random_number_color] != 8){
                            colors[color[random_number_color]].is_exist = true ;
                            balls[i][j].rgb[0] = colors[color[random_number_color]].r ;
                            balls[i][j].rgb[1] = colors[color[random_number_color]].g ;
                            balls[i][j].rgb[2] = colors[color[random_number_color]].b ;
                            balls[i][j].is_exist = true;
                            balls[i][j].locked = true;
                        }else{
                            colors[color[random_number_color]].is_exist = true ;
                            balls[i][j].rgb[0] = colors[color[random_number_color]].r ;
                            balls[i][j].rgb[1] = colors[color[random_number_color]].g ;
                            balls[i][j].rgb[2] = colors[color[random_number_color]].b ;
                            balls[i][j].is_exist = true;
                        }
                    }
                }else{
                    int found_color = rand()%counter_of_colors ;
                    if(color[colors_that_exist[found_color]] == 8 && gray_balls_exist && (rand()%output11 == 0 || rand()%output21 == 0)){
                        random_number_color = rand() % number_of_colors ;
                        if((rand() % output12 == 0 || rand() % output22 == 0) && multicolor_ball && color[random_number_color]!=8){
                            balls[i][j].multicolor = true ;
                            colors[color[random_number_color]].is_exist = true ;
                            balls[i][j].rgb[0] = colors[color[random_number_color]].r ;
                            balls[i][j].rgb[1] = colors[color[random_number_color]].g ;
                            balls[i][j].rgb[2] = colors[color[random_number_color]].b ;
                            random_number_color = rand() % number_of_colors ;
                            while(color[random_number_color]==8)
                                random_number_color = rand() % number_of_colors ;
                            colors[color[random_number_color]].is_exist = true ;
                            balls[i][j].rgb1[0] = colors[color[random_number_color]].r ;
                            balls[i][j].rgb1[1] = colors[color[random_number_color]].g ;
                            balls[i][j].rgb1[2] = colors[color[random_number_color]].b ;
                            balls[i][j].is_exist = true;
                        }else{
                            random_number_color = rand() % number_of_colors ;
                            if(locked_ball && (rand() % output13 == 0 || rand() % output23 == 0) && color[random_number_color] != 8){
                                colors[color[random_number_color]].is_exist = true ;
                                balls[i][j].rgb[0] = colors[color[random_number_color]].r ;
                                balls[i][j].rgb[1] = colors[color[random_number_color]].g ;
                                balls[i][j].rgb[2] = colors[color[random_number_color]].b ;
                                balls[i][j].is_exist = true;
                                balls[i][j].locked = true;
                            }else{
                                colors[color[random_number_color]].is_exist = true ;
                                balls[i][j].rgb[0] = colors[color[random_number_color]].r ;
                                balls[i][j].rgb[1] = colors[color[random_number_color]].g ;
                                balls[i][j].rgb[2] = colors[color[random_number_color]].b ;
                                balls[i][j].is_exist = true;
                            }
                        }
                    }else{
                        if((rand() % output12 == 0 || rand() % output22 == 0) && multicolor_ball && color[colors_that_exist[found_color]] != 8 ){
                            balls[i][j].multicolor = true ;
                            colors[color[colors_that_exist[found_color]]].is_exist = true ;
                            balls[i][j].rgb[0] = colors[color[colors_that_exist[found_color]]].r ;
                            balls[i][j].rgb[1] = colors[color[colors_that_exist[found_color]]].g ;
                            balls[i][j].rgb[2] = colors[color[colors_that_exist[found_color]]].b ;
                            while(color[colors_that_exist[found_color]]==8)
                                found_color = rand()%counter_of_colors ;
                            colors[color[colors_that_exist[found_color]]].is_exist = true ;
                            balls[i][j].rgb1[0] = colors[color[colors_that_exist[found_color]]].r ;
                            balls[i][j].rgb1[1] = colors[color[colors_that_exist[found_color]]].g ;
                            balls[i][j].rgb1[2] = colors[color[colors_that_exist[found_color]]].b ;
                            balls[i][j].is_exist = true;
                        }else{
                            if(locked_ball && (rand() % output13 == 0 || rand() % output23 == 0) && color[colors_that_exist[found_color]] != 8){
                                colors[color[colors_that_exist[found_color]]].is_exist = true ;
                                balls[i][j].rgb[0] = colors[color[colors_that_exist[found_color]]].r ;
                                balls[i][j].rgb[1] = colors[color[colors_that_exist[found_color]]].g ;
                                balls[i][j].rgb[2] = colors[color[colors_that_exist[found_color]]].b ;
                                balls[i][j].is_exist = true;
                                balls[i][j].locked = true;
                            }else{
                                colors[color[colors_that_exist[found_color]]].is_exist = true ;
                                balls[i][j].rgb[0] = colors[color[colors_that_exist[found_color]]].r ;
                                balls[i][j].rgb[1] = colors[color[colors_that_exist[found_color]]].g ;
                                balls[i][j].rgb[2] = colors[color[colors_that_exist[found_color]]].b ;
                                balls[i][j].is_exist = true;
                            }
                        }
                    }
                }
                colors_that_exist.clear() ;
            }
        }
    }
    for (int i = n_rows_out_manual-1 ; i >= 0; i--) {
        for (int j = 0; j < n_columns ; ++j) {
            bool found = true ;
            random_number = rand() ;
            if((random_number%output1==0 || random_number%output2==0)&& false ){
                balls_out[i][j].is_exist = false;
            }else{
                vector<int> colors_that_exist ;
                int counter_of_colors = 0 ;
                if(k_upbl > 1){
                    finding_neighbors_balls_new(i, j, balls , output) ;
                }else
                    finding_neighbors_balls(i, j, balls_out , output) ;
                for (auto & p : output) {
                    if(balls_out[p[0]][p[1]].is_exist){
                        for (int k = 0; k < number_of_colors; ++k) {
                            if(balls_out[p[0]][p[1]].rgb[0] == colors[color[k]].r && balls_out[p[0]][p[1]].rgb[1] == colors[color[k]].g && balls_out[p[0]][p[1]].rgb[2] == colors[color[k]].b){
                                counter_of_colors++ ;
                                colors_that_exist.push_back(k) ;
                            }
                        }
                    }
                }
                if(rand()%difficulty_of_colors == 0 || counter_of_colors == 0){
                    random_number_color = rand() % number_of_colors ;
                    if((rand() % output12 == 0|| rand() % output22 == 0) && multicolor_ball && color[random_number_color]!=8){
                        balls_out[i][j].multicolor = true ;
                        colors[color[random_number_color]].is_exist = true ;
                        balls_out[i][j].rgb[0] = colors[color[random_number_color]].r ;
                        balls_out[i][j].rgb[1] = colors[color[random_number_color]].g ;
                        balls_out[i][j].rgb[2] = colors[color[random_number_color]].b ;
                        random_number_color = rand() % number_of_colors ;
                        while(color[random_number_color]==8)
                            random_number_color = rand() % number_of_colors ;
                        colors[color[random_number_color]].is_exist = true ;
                        balls_out[i][j].rgb1[0] = colors[color[random_number_color]].r ;
                        balls_out[i][j].rgb1[1] = colors[color[random_number_color]].g ;
                        balls_out[i][j].rgb1[2] = colors[color[random_number_color]].b ;
                        balls_out[i][j].is_exist = true;
                    }else{
                        random_number_color = rand() % number_of_colors ;
                        if(locked_ball && (rand() % output13 == 0 || rand() % output23 == 0) && color[random_number_color] != 8){
                            colors[color[random_number_color]].is_exist = true ;
                            balls_out[i][j].rgb[0] = colors[color[random_number_color]].r ;
                            balls_out[i][j].rgb[1] = colors[color[random_number_color]].g ;
                            balls_out[i][j].rgb[2] = colors[color[random_number_color]].b ;
                            balls_out[i][j].is_exist = true;
                            balls_out[i][j].locked = true;
                        }else{
                            colors[color[random_number_color]].is_exist = true ;
                            balls_out[i][j].rgb[0] = colors[color[random_number_color]].r ;
                            balls_out[i][j].rgb[1] = colors[color[random_number_color]].g ;
                            balls_out[i][j].rgb[2] = colors[color[random_number_color]].b ;
                            balls_out[i][j].is_exist = true;
                        }
                    }
                }else{
                    int found_color = rand()%counter_of_colors ;
                    if(color[colors_that_exist[found_color]] == 8 && gray_balls_exist && (rand()%output11 == 0 || rand()%output21 == 0)){
                        random_number_color = rand() % number_of_colors ;
                        if((rand() % output12 == 0 || rand() % output22 == 0) && multicolor_ball && color[random_number_color]!=8){
                            balls_out[i][j].multicolor = true ;
                            colors[color[random_number_color]].is_exist = true ;
                            balls_out[i][j].rgb[0] = colors[color[random_number_color]].r ;
                            balls_out[i][j].rgb[1] = colors[color[random_number_color]].g ;
                            balls_out[i][j].rgb[2] = colors[color[random_number_color]].b ;
                            random_number_color = rand() % number_of_colors ;
                            while(color[random_number_color]==8)
                                random_number_color = rand() % number_of_colors ;
                            colors[color[random_number_color]].is_exist = true ;
                            balls_out[i][j].rgb1[0] = colors[color[random_number_color]].r ;
                            balls_out[i][j].rgb1[1] = colors[color[random_number_color]].g ;
                            balls_out[i][j].rgb1[2] = colors[color[random_number_color]].b ;
                            balls_out[i][j].is_exist = true;
                        }else{
                            random_number_color = rand() % number_of_colors ;
                            if(locked_ball && (rand() % output13 == 0 || rand() % output23 == 0) && color[random_number_color] != 8){
                                colors[color[random_number_color]].is_exist = true ;
                                balls_out[i][j].rgb[0] = colors[color[random_number_color]].r ;
                                balls_out[i][j].rgb[1] = colors[color[random_number_color]].g ;
                                balls_out[i][j].rgb[2] = colors[color[random_number_color]].b ;
                                balls_out[i][j].is_exist = true;
                                balls_out[i][j].locked = true;
                            }else{
                                colors[color[random_number_color]].is_exist = true ;
                                balls_out[i][j].rgb[0] = colors[color[random_number_color]].r ;
                                balls_out[i][j].rgb[1] = colors[color[random_number_color]].g ;
                                balls_out[i][j].rgb[2] = colors[color[random_number_color]].b ;
                                balls_out[i][j].is_exist = true;
                            }
                        }
                    }else{
                        if((rand() % output12 == 0 || rand() % output22 == 0) && multicolor_ball && color[colors_that_exist[found_color]] != 8 ){
                            balls_out[i][j].multicolor = true ;
                            colors[color[colors_that_exist[found_color]]].is_exist = true ;
                            balls_out[i][j].rgb[0] = colors[color[colors_that_exist[found_color]]].r ;
                            balls_out[i][j].rgb[1] = colors[color[colors_that_exist[found_color]]].g ;
                            balls_out[i][j].rgb[2] = colors[color[colors_that_exist[found_color]]].b ;
                            while(color[colors_that_exist[found_color]]==8)
                                found_color = rand()%counter_of_colors ;
                            colors[color[colors_that_exist[found_color]]].is_exist = true ;
                            balls_out[i][j].rgb1[0] = colors[color[colors_that_exist[found_color]]].r ;
                            balls_out[i][j].rgb1[1] = colors[color[colors_that_exist[found_color]]].g ;
                            balls_out[i][j].rgb1[2] = colors[color[colors_that_exist[found_color]]].b ;
                            balls_out[i][j].is_exist = true;
                        }else{
                            if(locked_ball && (rand() % output13 == 0 || rand() % output23 == 0) && color[colors_that_exist[found_color]] != 8){
                                colors[color[colors_that_exist[found_color]]].is_exist = true ;
                                balls_out[i][j].rgb[0] = colors[color[colors_that_exist[found_color]]].r ;
                                balls_out[i][j].rgb[1] = colors[color[colors_that_exist[found_color]]].g ;
                                balls_out[i][j].rgb[2] = colors[color[colors_that_exist[found_color]]].b ;
                                balls_out[i][j].is_exist = true;
                                balls_out[i][j].locked = true;
                            }else{
                                colors[color[colors_that_exist[found_color]]].is_exist = true ;
                                balls_out[i][j].rgb[0] = colors[color[colors_that_exist[found_color]]].r ;
                                balls_out[i][j].rgb[1] = colors[color[colors_that_exist[found_color]]].g ;
                                balls_out[i][j].rgb[2] = colors[color[colors_that_exist[found_color]]].b ;
                                balls_out[i][j].is_exist = true;
                            }
                        }
                    }
                }
                colors_that_exist.clear() ;
            }
        }
        if(i % 10 == 0 && difficulty_of_colors > 1){
            difficulty_of_colors-- ;

        }
        if(i % 10 == 0){
            output12++ ;
            if(i % 20 == 0 ){
                output22++ ;
            }
        }
        if(i % 10 == 0 && output13 > 2){
            output13-- ;
            if(i % 20 == 0){
                output23-- ;
            }
        }
        if(i % 10 == 0 && output11 > 2){
            output11-- ;
            if(i % 20 == 0){
                output21-- ;
            }
        }
        //cout << difficulty_of_colors << "   " << output11 << " " << output21 << "   " << output12 << " " << output22 << "   " << output13 << " " << output23 <<endl ;
    }
    return 1;
}

int draw_beam(SDL_Renderer * renderer , int W , int H , int mouseX , int mouseY , ball2d & flying_bullet , double &vel_x , double &vel_y , flying_color2d flying_color , color2d colors[8]){
    int R = int ( W/ (n_columns*2+1) );
    int W_min = (W-(R*(n_columns*2+1)))/2;
    int W_max = (W-(R*(n_columns*2+1)))/2+((R*(n_columns*2+1)));
    int wgo = 0; //out put of func: wanna_go_out
    wgo = wanna_go_out(flying_bullet.x, flying_bullet.y, W, H, R);
    if (wgo == 2 || wgo == 4) {
        vel_x *= -1;
    }
    if(wgo == 1){
        vel_y *= -1;
    }
    flying_bullet.x += vel_x;
    flying_bullet.y += vel_y;
    if(flying_color.multi_color){
        for (int i = 1; i < number_of_colors; ++i) {
            filledPieRGBA(renderer, flying_bullet.x, flying_bullet.y, R, (i-1)*(360/(number_of_colors-1)) , i*(360/(number_of_colors-1)) , colors[color[i]].r , colors[color[i]].g , colors[color[i]].b , 255);
        }
    }else
        filledEllipseRGBA(renderer, flying_bullet.x, flying_bullet.y, R, R, flying_color.r , flying_color.g , flying_color.b , 255);
    return 1;
}

int checking_collision(SDL_Renderer * renderer , int W , int H , ball2d balls[n_rows][n_columns] , ball2d & flying_bullet , double &vel_x , double &vel_y , flying_color2d flying_color  , int &new_ball_row , int &new_ball_column , int R_collision){
    double R = int ( W/ (n_columns*2+1) );
    int W_min = (W-(R*(n_columns*2+1)))/2;
    int W_max = (W-(R*(n_columns*2+1)))/2+((R*(n_columns*2+1)));
    int i_empty =0 , j_empty=0 ;
    double distance_temp=10000 ;
    bool checked = false;
    double min_dist = 10000;
    double dist1;
    double dist2;
    int i_coll , j_coll;
    for (int i = n_rows-1; i >=0 ; i--) {
        if(i == n_rows-1){
            for (int j = 0; j < n_columns; ++j) {
                dist1 = (flying_bullet.x-balls[i-1][j].x)*(flying_bullet.x-balls[i-1][j].x)+(flying_bullet.y-balls[i-1][j].y)*(flying_bullet.y-balls[i-1][j].y);
                dist2 = (flying_bullet.x-balls[i][j].x)*(flying_bullet.x-balls[i][j].x)+(flying_bullet.y-balls[i][j].y)*(flying_bullet.y-balls[i][j].y);
                if(balls[i-1][j].is_exist && dist1<=R_collision){
                    if(dist1 < min_dist){
                        min_dist = dist1;
                        i_coll = i-1;
                        j_coll = j;
                    }
                    checked = true;
                }else if(balls[i][j].is_exist && dist2<=R_collision){
                    if(dist2 < min_dist){
                        min_dist = dist2;
                        i_coll = i;
                        j_coll = j;
                    }
                    checked = true;
                }
            }
        }else if(flying_bullet.y<=balls[i][0].y){
            for (int j = 0; j < n_columns; ++j) {
                dist1 = (flying_bullet.x-balls[i-1][j].x)*(flying_bullet.x-balls[i-1][j].x)+(flying_bullet.y-balls[i-1][j].y)*(flying_bullet.y-balls[i-1][j].y);
                dist2 = (flying_bullet.x-balls[i][j].x)*(flying_bullet.x-balls[i][j].x)+(flying_bullet.y-balls[i][j].y)*(flying_bullet.y-balls[i][j].y);
                if(balls[i-1][j].is_exist && dist1<=R_collision){
                    if(dist1 < min_dist){
                        min_dist = dist1;
                        i_coll = i-1;
                        j_coll = j;
                    }
                    checked = true;
                }else if(balls[i][j].is_exist && dist2<=R_collision){
                    if(dist2 < min_dist){
                        min_dist = dist2;
                        i_coll = i;
                        j_coll = j;
                    }
                    checked = true;
                }
            }
        }
        if(checked){
            double collision_x = (flying_bullet.x+balls[i_coll][j_coll].x)/2 , collision_y = (flying_bullet.y+balls[i_coll][j_coll].y)/2 ;
            if(effects)
                filledEllipseRGBA(renderer , collision_x , collision_y , R*1.2 , R*1.2 , 0 ,255 ,255 ,255);
            vector<double> my_array = {10000,10000,10000,10000,10000,10000};
            double dist11=10000 , dist22=10000 , dist33=10000 , dist44=10000 , dist55=10000 , dist66=10000 ;
            if(i_coll == n_rows-1 && balls[n_rows-1][n_columns].eo == 1){
                if(j_coll == 0){
                    if(!balls[i_coll][j_coll+1].is_exist)
                        dist11 = distance(collision_x , collision_y , balls[i_coll][j_coll+1].x , balls[i_coll][j_coll+1].y) ;
                    if(!balls[i_coll-1][j_coll+1].is_exist)
                        dist44 = distance(collision_x , collision_y , balls[i_coll-1][j_coll+1].x , balls[i_coll-1][j_coll+1].y) ;
                    if(!balls[i_coll-1][j_coll].is_exist)
                        dist55 = distance(collision_x , collision_y , balls[i_coll-1][j_coll].x , balls[i_coll-1][j_coll].y) ;
                    dist33 = distance(collision_x , collision_y , collision_x , balls[i_coll][j_coll].y+2*R) ;
                    my_array[0]= dist11 ;
                    my_array[2]= dist22 ;
                    my_array[3]= dist33 ;
                    my_array[4]= dist44 ;
                    my_array[5]= dist55 ;
                }else if(j_coll == n_columns-1){
                    if(!balls[i_coll][j_coll-1].is_exist)
                        dist11 = distance(collision_x , collision_y , balls[i_coll][j_coll-1].x , balls[i_coll][j_coll-1].y) ;
                    if(!balls[i_coll-1][j_coll].is_exist)
                        dist55 = distance(collision_x , collision_y , balls[i_coll-1][j_coll].x , balls[i_coll-1][j_coll].y) ;
                    dist33 = distance(collision_x , collision_y , collision_x , balls[i_coll][j_coll].y+2*R) ;
                    my_array[1]= dist11 ;
                    my_array[3]= dist33 ;
                    my_array[5]= dist55 ;
                }else{
                    if(!balls[i_coll][j_coll+1].is_exist)
                        dist11 = distance(collision_x , collision_y , balls[i_coll][j_coll+1].x , balls[i_coll][j_coll+1].y) ;
                    if(!balls[i_coll][j_coll-1].is_exist)
                        dist22 = distance(collision_x , collision_y , balls[i_coll][j_coll-1].x , balls[i_coll][j_coll-1].y) ;
                    if(!balls[i_coll-1][j_coll+1].is_exist)
                        dist55 = distance(collision_x , collision_y , balls[i_coll-1][j_coll+1].x , balls[i_coll-1][j_coll+1].y) ;
                    if(!balls[i_coll-1][j_coll].is_exist)
                        dist66 = distance(collision_x , collision_y , balls[i_coll-1][j_coll].x , balls[i_coll-1][j_coll].y) ;
                    dist33 = distance(collision_x , collision_y , collision_x , balls[i_coll][j_coll].y+2*R) ;
                    my_array[0]= dist11 ;
                    my_array[1]= dist22 ;
                    my_array[2]= dist33 ;
                    my_array[3]= dist44 ;
                    my_array[4]= dist55 ;
                    my_array[5]= dist66 ;
                }
                int minimum_index = find_minimum_index(my_array);
                if(minimum_index == 0){
                    balls[i_coll][j_coll+1].rgb[0] = flying_color.r ;
                    balls[i_coll][j_coll+1].rgb[1] = flying_color.g ;
                    balls[i_coll][j_coll+1].rgb[2] = flying_color.b ;
                    balls[i_coll][j_coll+1].is_exist = true ;
                    collision_type( balls ,i_coll , j_coll+1 , flying_color) ;
                    new_ball_row = i_coll ;
                    new_ball_column = j_coll+1 ;
                }else if(minimum_index ==1){
                    balls[i_coll][j_coll-1].rgb[0] = flying_color.r ;
                    balls[i_coll][j_coll-1].rgb[1] = flying_color.g ;
                    balls[i_coll][j_coll-1].rgb[2] = flying_color.b ;
                    balls[i_coll][j_coll-1].is_exist = true ;
                    collision_type( balls ,i_coll , j_coll-1 , flying_color) ;
                    new_ball_row = i_coll ;
                    new_ball_column = j_coll-1 ;
                }else if(minimum_index ==4){
                    balls[i_coll-1][j_coll+1].rgb[0] = flying_color.r ;
                    balls[i_coll-1][j_coll+1].rgb[1] = flying_color.g ;
                    balls[i_coll-1][j_coll+1].rgb[2] = flying_color.b ;
                    balls[i_coll-1][j_coll+1].is_exist = true ;
                    collision_type( balls ,i_coll-1 , j_coll+1 , flying_color) ;
                    new_ball_row = i_coll-1 ;
                    new_ball_column = j_coll+1 ;
                }else if(minimum_index ==5){
                    balls[i_coll-1][j_coll].rgb[0] = flying_color.r ;
                    balls[i_coll-1][j_coll].rgb[1] = flying_color.g ;
                    balls[i_coll-1][j_coll].rgb[2] = flying_color.b ;
                    balls[i_coll-1][j_coll].is_exist = true ;
                    collision_type( balls ,i_coll-1 , j_coll , flying_color) ;
                    new_ball_row = i_coll-1 ;
                    new_ball_column = j_coll ;
                }
                return 1;
            }else if(i_coll == n_rows-1 && balls[n_rows-1][n_columns].eo == -1){
                if(j_coll == 0){
                    if(!balls[i_coll][j_coll+1].is_exist)
                        dist11 = distance(collision_x , collision_y , balls[i_coll][j_coll+1].x , balls[i_coll][j_coll+1].y) ;
                    if(!balls[i_coll-1][j_coll].is_exist)
                        dist55 = distance(collision_x , collision_y , balls[i_coll-1][j_coll].x , balls[i_coll-1][j_coll].y) ;
                    dist33 = distance(collision_x , collision_y , collision_x , balls[i_coll][j_coll].y+2*R) ;
                    my_array[0]= dist11 ;
                    my_array[4]= dist55 ;
                    my_array[2]= dist33 ;
                }else if(j_coll == n_columns-1){
                    if(!balls[i_coll][j_coll-1].is_exist)
                        dist11 = distance(collision_x , collision_y , balls[i_coll][j_coll-1].x , balls[i_coll][j_coll-1].y) ;
                    if(!balls[i_coll-1][j_coll-1].is_exist)
                        dist44 = distance(collision_x , collision_y , balls[i_coll-1][j_coll-1].x , balls[i_coll-1][j_coll-1].y) ;
                    if(!balls[i_coll-1][j_coll].is_exist)
                        dist55 = distance(collision_x , collision_y , balls[i_coll-1][j_coll].x , balls[i_coll-1][j_coll].y) ;
                    dist33 = distance(collision_x , collision_y , collision_x , balls[i_coll][j_coll].y+2*R) ;
                    my_array[1]= dist11 ;
                    my_array[3]= dist22 ;
                    my_array[2]= dist33 ;
                    my_array[5]= dist44 ;
                    my_array[4]= dist55 ;
                }else{
                    if(!balls[i_coll][j_coll+1].is_exist)
                        dist11 = distance(collision_x , collision_y , balls[i_coll][j_coll+1].x , balls[i_coll][j_coll+1].y) ;
                    if(!balls[i_coll][j_coll-1].is_exist)
                        dist22 = distance(collision_x , collision_y , balls[i_coll][j_coll-1].x , balls[i_coll][j_coll-1].y) ;
                    if(!balls[i_coll-1][j_coll].is_exist)
                        dist55 = distance(collision_x , collision_y , balls[i_coll-1][j_coll].x , balls[i_coll-1][j_coll].y) ;
                    if(!balls[i_coll-1][j_coll-1].is_exist)
                        dist66 = distance(collision_x , collision_y , balls[i_coll-1][j_coll-1].x , balls[i_coll-1][j_coll-1].y) ;
                    dist33 = distance(collision_x , collision_y , collision_x , balls[i_coll][j_coll].y+2*R) ;
                    my_array[0]= dist11 ;
                    my_array[1]= dist22 ;
                    my_array[2]= dist33 ;
                    my_array[3]= dist44 ;
                    my_array[4]= dist55 ;
                    my_array[5]= dist66 ;
                }
                int minimum_index = find_minimum_index(my_array);
                if(minimum_index == 0){
                    balls[i_coll][j_coll+1].rgb[0] = flying_color.r ;
                    balls[i_coll][j_coll+1].rgb[1] = flying_color.g ;
                    balls[i_coll][j_coll+1].rgb[2] = flying_color.b ;
                    balls[i_coll][j_coll+1].is_exist = true ;
                    collision_type( balls ,i_coll , j_coll+1 , flying_color) ;
                    new_ball_row = i_coll ;
                    new_ball_column = j_coll+1 ;
                }else if(minimum_index ==1){
                    balls[i_coll][j_coll-1].rgb[0] = flying_color.r ;
                    balls[i_coll][j_coll-1].rgb[1] = flying_color.g ;
                    balls[i_coll][j_coll-1].rgb[2] = flying_color.b ;
                    balls[i_coll][j_coll-1].is_exist = true ;
                    collision_type( balls ,i_coll , j_coll-1 , flying_color) ;
                    new_ball_row = i_coll ;
                    new_ball_column = j_coll-1 ;
                }else if(minimum_index ==4){
                    balls[i_coll-1][j_coll].rgb[0] = flying_color.r ;
                    balls[i_coll-1][j_coll].rgb[1] = flying_color.g ;
                    balls[i_coll-1][j_coll].rgb[2] = flying_color.b ;
                    balls[i_coll-1][j_coll].is_exist = true ;
                    collision_type( balls ,i_coll-1 , j_coll , flying_color) ;
                    new_ball_row = i_coll-1 ;
                    new_ball_column = j_coll ;
                }else if(minimum_index ==5){
                    balls[i_coll-1][j_coll-1].rgb[0] = flying_color.r ;
                    balls[i_coll-1][j_coll-1].rgb[1] = flying_color.g ;
                    balls[i_coll-1][j_coll-1].rgb[2] = flying_color.b ;
                    balls[i_coll-1][j_coll-1].is_exist = true ;
                    collision_type( balls ,i_coll-1 , j_coll-1 , flying_color) ;
                    new_ball_row = i_coll-1 ;
                    new_ball_column = j_coll-1 ;
                }
                return 1;
            }else if(balls[i_coll][j_coll].eo == 1){// 5
                if(j_coll == 0){
                    if(!balls[i_coll][j_coll+1].is_exist)
                        dist11 = distance(collision_x , collision_y , balls[i_coll][j_coll+1].x , balls[i_coll][j_coll+1].y) ;
                    if(!balls[i_coll+1][j_coll+1].is_exist)
                        dist22 = distance(collision_x , collision_y , balls[i_coll+1][j_coll+1].x , balls[i_coll+1][j_coll+1].y) ;
                    if(!balls[i_coll+1][j_coll].is_exist)
                        dist33 = distance(collision_x , collision_y , balls[i_coll+1][j_coll].x , balls[i_coll+1][j_coll].y) ;
                    if(!balls[i_coll-1][j_coll+1].is_exist)
                        dist44 = distance(collision_x , collision_y , balls[i_coll-1][j_coll+1].x , balls[i_coll-1][j_coll+1].y) ;
                    if(!balls[i_coll-1][j_coll].is_exist)
                        dist55 = distance(collision_x , collision_y , balls[i_coll-1][j_coll].x , balls[i_coll-1][j_coll].y) ;
                    my_array[0]= dist11 ;
                    my_array[2]= dist22 ;
                    my_array[3]= dist33 ;
                    my_array[4]= dist44 ;
                    my_array[5]= dist55 ;
                }else if(j_coll == n_columns-1){
                    if(!balls[i_coll][j_coll-1].is_exist)
                        dist11 = distance(collision_x , collision_y , balls[i_coll][j_coll-1].x , balls[i_coll][j_coll-1].y) ;
                    if(!balls[i_coll+1][j_coll].is_exist)
                        dist33 = distance(collision_x , collision_y , balls[i_coll+1][j_coll].x , balls[i_coll+1][j_coll].y) ;
                    if(!balls[i_coll-1][j_coll].is_exist)
                        dist55 = distance(collision_x , collision_y , balls[i_coll-1][j_coll].x , balls[i_coll-1][j_coll].y) ;
                    my_array[1]= dist11 ;
                    my_array[3]= dist33 ;
                    my_array[5]= dist55 ;
                }else{
                    if(!balls[i_coll][j_coll+1].is_exist)
                        dist11 = distance(collision_x , collision_y , balls[i_coll][j_coll+1].x , balls[i_coll][j_coll+1].y) ;
                    if(!balls[i_coll][j_coll-1].is_exist)
                        dist22 = distance(collision_x , collision_y , balls[i_coll][j_coll-1].x , balls[i_coll][j_coll-1].y) ;
                    if(!balls[i_coll+1][j_coll+1].is_exist)
                        dist33 = distance(collision_x , collision_y , balls[i_coll+1][j_coll+1].x , balls[i_coll+1][j_coll+1].y) ;
                    if(!balls[i_coll+1][j_coll].is_exist)
                        dist44 = distance(collision_x , collision_y , balls[i_coll+1][j_coll].x , balls[i_coll+1][j_coll].y) ;
                    if(!balls[i_coll-1][j_coll+1].is_exist)
                        dist55 = distance(collision_x , collision_y , balls[i_coll-1][j_coll+1].x , balls[i_coll-1][j_coll+1].y) ;
                    if(!balls[i_coll-1][j_coll].is_exist)
                        dist66 = distance(collision_x , collision_y , balls[i_coll-1][j_coll].x , balls[i_coll-1][j_coll].y) ;
                    my_array[0]= dist11 ;
                    my_array[1]= dist22 ;
                    my_array[2]= dist33 ;
                    my_array[3]= dist44 ;
                    my_array[4]= dist55 ;
                    my_array[5]= dist66 ;
                }
                int minimum_index = find_minimum_index(my_array);
                if(minimum_index == 0){
                    balls[i_coll][j_coll+1].rgb[0] = flying_color.r ;
                    balls[i_coll][j_coll+1].rgb[1] = flying_color.g ;
                    balls[i_coll][j_coll+1].rgb[2] = flying_color.b ;
                    balls[i_coll][j_coll+1].is_exist = true ;
                    collision_type( balls ,i_coll , j_coll+1 , flying_color) ;
                    new_ball_row = i_coll ;
                    new_ball_column = j_coll+1 ;
                }else if(minimum_index ==1){
                    balls[i_coll][j_coll-1].rgb[0] = flying_color.r ;
                    balls[i_coll][j_coll-1].rgb[1] = flying_color.g ;
                    balls[i_coll][j_coll-1].rgb[2] = flying_color.b ;
                    balls[i_coll][j_coll-1].is_exist = true ;
                    collision_type( balls ,i_coll , j_coll-1 , flying_color) ;
                    new_ball_row = i_coll ;
                    new_ball_column = j_coll-1 ;
                }else if(minimum_index ==2){
                    balls[i_coll+1][j_coll+1].rgb[0] = flying_color.r ;
                    balls[i_coll+1][j_coll+1].rgb[1] = flying_color.g ;
                    balls[i_coll+1][j_coll+1].rgb[2] = flying_color.b ;
                    balls[i_coll+1][j_coll+1].is_exist = true ;
                    collision_type( balls ,i_coll+1 , j_coll+1 , flying_color) ;
                    new_ball_row = i_coll+1 ;
                    new_ball_column = j_coll+1 ;
                }else if(minimum_index ==3){
                    balls[i_coll+1][j_coll].rgb[0] = flying_color.r ;
                    balls[i_coll+1][j_coll].rgb[1] = flying_color.g ;
                    balls[i_coll+1][j_coll].rgb[2] = flying_color.b ;
                    balls[i_coll+1][j_coll].is_exist = true ;
                    collision_type( balls ,i_coll+1 , j_coll , flying_color) ;
                    new_ball_row = i_coll+1 ;
                    new_ball_column = j_coll ;
                }else if(minimum_index ==4){
                    balls[i_coll-1][j_coll+1].rgb[0] = flying_color.r ;
                    balls[i_coll-1][j_coll+1].rgb[1] = flying_color.g ;
                    balls[i_coll-1][j_coll+1].rgb[2] = flying_color.b ;
                    balls[i_coll-1][j_coll+1].is_exist = true ;
                    collision_type( balls ,i_coll-1 , j_coll+1 , flying_color) ;
                    new_ball_row = i_coll-1 ;
                    new_ball_column = j_coll+1 ;
                }else if(minimum_index ==5){
                    balls[i_coll-1][j_coll].rgb[0] = flying_color.r ;
                    balls[i_coll-1][j_coll].rgb[1] = flying_color.g ;
                    balls[i_coll-1][j_coll].rgb[2] = flying_color.b ;
                    balls[i_coll-1][j_coll].is_exist = true ;
                    collision_type( balls ,i_coll-1 , j_coll , flying_color) ;
                    new_ball_row = i_coll-1 ;
                    new_ball_column = j_coll ;
                }
            }else if(balls[i_coll][j_coll].eo == -1){ //3
                if(j_coll == 0){
                    if(!balls[i_coll][j_coll+1].is_exist)
                        dist11 = distance(collision_x , collision_y , balls[i_coll][j_coll+1].x , balls[i_coll][j_coll+1].y) ;
                    if(!balls[i_coll+1][j_coll].is_exist)
                        dist22 = distance(collision_x , collision_y , balls[i_coll+1][j_coll].x , balls[i_coll+1][j_coll].y) ;
                    if(!balls[i_coll-1][j_coll].is_exist)
                        dist33 = distance(collision_x , collision_y , balls[i_coll-1][j_coll].x , balls[i_coll-1][j_coll].y) ;
                    my_array[0]= dist11 ;
                    my_array[2]= dist22 ;
                    my_array[4]= dist33 ;
                }else if(j_coll == n_columns-1){
                    if(!balls[i_coll][j_coll-1].is_exist)
                        dist11 = distance(collision_x , collision_y , balls[i_coll][j_coll-1].x , balls[i_coll][j_coll-1].y) ;
                    if(!balls[i_coll+1][j_coll-1].is_exist)
                        dist22 = distance(collision_x , collision_y , balls[i_coll+1][j_coll-1].x , balls[i_coll+1][j_coll-1].y) ;
                    if(!balls[i_coll+1][j_coll].is_exist)
                        dist33 = distance(collision_x , collision_y , balls[i_coll+1][j_coll].x , balls[i_coll+1][j_coll].y) ;
                    if(!balls[i_coll-1][j_coll-1].is_exist)
                        dist44 = distance(collision_x , collision_y , balls[i_coll-1][j_coll-1].x , balls[i_coll-1][j_coll-1].y) ;
                    if(!balls[i_coll-1][j_coll].is_exist)
                        dist55 = distance(collision_x , collision_y , balls[i_coll-1][j_coll].x , balls[i_coll-1][j_coll].y) ;
                    my_array[1]= dist11 ;
                    my_array[3]= dist22 ;
                    my_array[2]= dist33 ;
                    my_array[5]= dist44 ;
                    my_array[4]= dist55 ;
                }else{
                    if(!balls[i_coll][j_coll+1].is_exist)
                        dist11 = distance(collision_x , collision_y , balls[i_coll][j_coll+1].x , balls[i_coll][j_coll+1].y) ;
                    if(!balls[i_coll][j_coll-1].is_exist)
                        dist22 = distance(collision_x , collision_y , balls[i_coll][j_coll-1].x , balls[i_coll][j_coll-1].y) ;
                    if(!balls[i_coll+1][j_coll].is_exist)
                        dist33 = distance(collision_x , collision_y , balls[i_coll+1][j_coll].x , balls[i_coll+1][j_coll].y) ;
                    if(!balls[i_coll+1][j_coll-1].is_exist)
                        dist44 = distance(collision_x , collision_y , balls[i_coll+1][j_coll-1].x , balls[i_coll+1][j_coll-1].y) ;
                    if(!balls[i_coll-1][j_coll].is_exist)
                        dist55 = distance(collision_x , collision_y , balls[i_coll-1][j_coll].x , balls[i_coll-1][j_coll].y) ;
                    if(!balls[i_coll-1][j_coll-1].is_exist)
                        dist66 = distance(collision_x , collision_y , balls[i_coll-1][j_coll-1].x , balls[i_coll-1][j_coll-1].y) ;
                    my_array[0]= dist11 ;
                    my_array[1]= dist22 ;
                    my_array[2]= dist33 ;
                    my_array[3]= dist44 ;
                    my_array[4]= dist55 ;
                    my_array[5]= dist66 ;
                }
                int minimum_index = find_minimum_index(my_array);
                if(minimum_index == 0){
                    balls[i_coll][j_coll+1].rgb[0] = flying_color.r ;
                    balls[i_coll][j_coll+1].rgb[1] = flying_color.g ;
                    balls[i_coll][j_coll+1].rgb[2] = flying_color.b ;
                    balls[i_coll][j_coll+1].is_exist = true ;
                    collision_type( balls ,i_coll , j_coll+1 , flying_color) ;
                    new_ball_row = i_coll ;
                    new_ball_column = j_coll+1 ;
                }else if(minimum_index ==1){
                    balls[i_coll][j_coll-1].rgb[0] = flying_color.r ;
                    balls[i_coll][j_coll-1].rgb[1] = flying_color.g ;
                    balls[i_coll][j_coll-1].rgb[2] = flying_color.b ;
                    balls[i_coll][j_coll-1].is_exist = true ;
                    collision_type( balls ,i_coll , j_coll-1 , flying_color) ;
                    new_ball_row = i_coll ;
                    new_ball_column = j_coll-1 ;
                }else if(minimum_index ==2){
                    balls[i_coll+1][j_coll].rgb[0] = flying_color.r ;
                    balls[i_coll+1][j_coll].rgb[1] = flying_color.g ;
                    balls[i_coll+1][j_coll].rgb[2] = flying_color.b ;
                    balls[i_coll+1][j_coll].is_exist = true ;
                    collision_type( balls ,i_coll+1 , j_coll , flying_color) ;
                    new_ball_row = i_coll+1 ;
                    new_ball_column = j_coll ;
                }else if(minimum_index ==3){
                    balls[i_coll+1][j_coll-1].rgb[0] = flying_color.r ;
                    balls[i_coll+1][j_coll-1].rgb[1] = flying_color.g ;
                    balls[i_coll+1][j_coll-1].rgb[2] = flying_color.b ;
                    balls[i_coll+1][j_coll-1].is_exist = true ;
                    collision_type( balls ,i_coll+1 , j_coll-1 , flying_color) ;
                    new_ball_row = i_coll+1 ;
                    new_ball_column = j_coll-1 ;
                }else if(minimum_index ==4){
                    balls[i_coll-1][j_coll].rgb[0] = flying_color.r ;
                    balls[i_coll-1][j_coll].rgb[1] = flying_color.g ;
                    balls[i_coll-1][j_coll].rgb[2] = flying_color.b ;
                    balls[i_coll-1][j_coll].is_exist = true ;
                    collision_type( balls ,i_coll-1 , j_coll , flying_color) ;
                    new_ball_row = i_coll-1 ;
                    new_ball_column = j_coll ;
                }else if(minimum_index ==5){
                    balls[i_coll-1][j_coll-1].rgb[0] = flying_color.r ;
                    balls[i_coll-1][j_coll-1].rgb[1] = flying_color.g ;
                    balls[i_coll-1][j_coll-1].rgb[2] = flying_color.b ;
                    balls[i_coll-1][j_coll-1].is_exist = true ;
                    collision_type( balls ,i_coll-1 , j_coll-1 , flying_color) ;
                    new_ball_row = i_coll-1 ;
                    new_ball_column = j_coll-1 ;
                }
            }
            return 1;
        }
    }
    return 0;
}

int game_CHECK(ball2d balls[n_rows_out][n_columns], ball2d balls_out[n_rows_out][n_columns] , color2d colors[9]){
    bool stop = false ;
    int counter = 0 ;
    for (int i = n_rows-1; i >= 0; i--) {
        for (int j = 0; j < n_columns; ++j) {
            if(counter>number_of_colors) {
                return 1 ;
            }
            if(balls[i][j].is_exist && !(balls[i][j].rgb[0] == colors[8].r && balls[i][j].rgb[1] == colors[8].g && balls[i][j].rgb[2] == colors[8].b)){
                stop = true ;
                colors_exist(balls[i][j].rgb[0] , balls[i][j].rgb[1] , balls[i][j].rgb[2] , colors ,counter ) ;
                if(balls[i][j].multicolor){
                    colors_exist(balls[i][j].rgb1[0] , balls[i][j].rgb1[1] , balls[i][j].rgb1[2] , colors ,counter ) ;
                }
            }
        }
    }
    if(stop){
        return 1 ; // game is not finished
    }
    if(!stop){
        return 0 ; // game is finished
    }
}

int colors_exist(int r , int g , int b , color2d colors[9] ,int & counter ){
    bool found = false ;
    for (int i = 0; i < 8; ++i) {
        if(colors[i].r == r && colors[i].g == g && colors[i].b == b){
            colors[i].is_exist_temp = true ;
            counter++ ;
            break ;
        }
    }
    return 0 ;
}

int collision_type(ball2d balls[n_rows][n_columns] , int row1 , int columns1 , flying_color2d flying_color){

}

int color_change_next_ball(flying_color2d  &flyingColor , next_ball_color2d &next_Ball_Color){
    int temp_r = flyingColor.r ;
    int temp_g = flyingColor.g ;
    int temp_b = flyingColor.b ;
    flyingColor.r = next_Ball_Color.r ;
    flyingColor.g = next_Ball_Color.g ;
    flyingColor.b = next_Ball_Color.b ;
    next_Ball_Color.r = temp_r ;
    next_Ball_Color.g = temp_g ;
    next_Ball_Color.b = temp_b ;
}

double invert(int n) {
    if (n == 0) {
        return 1e9;
    }
    return floor(1.0 / n * 100) / 100;
}

int finding_neighbors_balls(int i_coll, int j_coll, ball2d balls[n_rows][n_columns], vector<array<int, 2>> &output ) {
    output.clear() ;
    if(i_coll == n_rows-1 && balls[i_coll][j_coll].eo == 1){
        if(j_coll == 0){
            output.push_back({i_coll , j_coll+1}) ;
            output.push_back({i_coll-1 , j_coll+1}) ;
            output.push_back({i_coll-1 , j_coll}) ;
            return 1 ;
        }else if(j_coll == n_columns-1){
            output.push_back({i_coll , j_coll-1}) ;
            output.push_back({i_coll-1 , j_coll}) ;
            return 1 ;
        }else{
            output.push_back({i_coll , j_coll+1}) ;
            output.push_back({i_coll , j_coll-1}) ;
            output.push_back({i_coll-1 , j_coll+1}) ;
            output.push_back({i_coll-1 , j_coll}) ;
            return 1 ;
        }
    }else if(i_coll == n_rows-1 && balls[i_coll][j_coll].eo == -1){
        if(j_coll == 0){
            output.push_back({i_coll , j_coll+1}) ;
            output.push_back({i_coll-1 , j_coll}) ;
            return 1 ;
        }else if(j_coll == n_columns-1){
            output.push_back({i_coll , j_coll-1}) ;
            output.push_back({i_coll-1 , j_coll-1}) ;
            output.push_back({i_coll-1 , j_coll}) ;
            return 1 ;
        }else{
            output.push_back({i_coll , j_coll+1}) ;
            output.push_back({i_coll , j_coll-1}) ;
            output.push_back({i_coll-1 , j_coll}) ;
            output.push_back({i_coll-1 , j_coll-1}) ;//
            return 1 ;
        }
    }else if(balls[i_coll][j_coll].eo == 1){// 5
        if(j_coll == 0){
            output.push_back({i_coll , j_coll+1}) ;
            output.push_back({i_coll+1 , j_coll+1}) ;
            output.push_back({i_coll+1 , j_coll}) ;
            output.push_back({i_coll-1 , j_coll+1}) ;
            output.push_back({i_coll-1 , j_coll}) ;
            return 1 ;
        }else if(j_coll == n_columns-1){
            output.push_back({i_coll , j_coll-1}) ;
            output.push_back({i_coll+1 , j_coll}) ;
            output.push_back({i_coll-1 , j_coll}) ;
            return 1 ;
        }else{
            output.push_back({i_coll , j_coll+1}) ;
            output.push_back({i_coll , j_coll-1}) ;
            output.push_back({i_coll+1 , j_coll+1}) ;
            output.push_back({i_coll+1 , j_coll}) ;
            output.push_back({i_coll-1 , j_coll+1}) ;//
            output.push_back({i_coll-1 , j_coll}) ;//
            return 1 ;
        }
    }else if(balls[i_coll][j_coll].eo == -1){ //3
        if(j_coll == 0){
            output.push_back({i_coll , j_coll+1}) ;
            output.push_back({i_coll+1 , j_coll}) ;
            output.push_back({i_coll-1 , j_coll}) ;
            return 1 ;
        }else if(j_coll == n_columns-1){
            output.push_back({i_coll , j_coll-1}) ;
            output.push_back({i_coll+1 , j_coll}) ;
            output.push_back({i_coll+1 , j_coll-1}) ;
            output.push_back({i_coll-1 , j_coll}) ;
            output.push_back({i_coll-1 , j_coll-1}) ;

            return 1 ;
        }else{
            output.push_back({i_coll , j_coll+1}) ;
            output.push_back({i_coll , j_coll-1}) ;
            output.push_back({i_coll+1 , j_coll}) ;
            output.push_back({i_coll+1 , j_coll-1}) ;
            output.push_back({i_coll-1 , j_coll}) ;//
            output.push_back({i_coll-1 , j_coll-1}) ;//
            return 1 ;
        }
    }
}

int finding_neighbors_balls_new(int i_coll, int j_coll, ball2d balls[n_rows][n_columns], vector<array<int, 2>> &output ) {
    output.clear() ;
    if(i_coll == n_rows-1 && balls[i_coll][j_coll].eo == 1){
        if(j_coll == 0){
            output.push_back({i_coll , j_coll+1}) ;
            output.push_back({i_coll-1 , j_coll+1}) ;
            output.push_back({i_coll-1 , j_coll}) ;
            return 1 ;
        }else if(j_coll == n_columns-1){
            output.push_back({i_coll , j_coll-1}) ;
            output.push_back({i_coll-1 , j_coll}) ;
            return 1 ;
        }else{
            output.push_back({i_coll , j_coll+1}) ;
            output.push_back({i_coll , j_coll-1}) ;
            output.push_back({i_coll-1 , j_coll+1}) ;
            output.push_back({i_coll-1 , j_coll}) ;
            return 1 ;
        }
    }else if(i_coll == n_rows-1 && balls[i_coll][j_coll].eo == -1){
        if(j_coll == 0){
            output.push_back({i_coll , j_coll+1}) ;
            output.push_back({i_coll-1 , j_coll}) ;
            return 1 ;
        }else if(j_coll == n_columns-1){
            output.push_back({i_coll , j_coll-1}) ;
            output.push_back({i_coll-1 , j_coll-1}) ;
            output.push_back({i_coll-1 , j_coll}) ;
            return 1 ;
        }else{
            output.push_back({i_coll , j_coll+1}) ;
            output.push_back({i_coll , j_coll-1}) ;
            output.push_back({i_coll-1 , j_coll}) ;
            output.push_back({i_coll-1 , j_coll-1}) ;//
            return 1 ;
        }
    }else if(i_coll == 1 && balls[i_coll][j_coll].eo == 1){// 5
        if(j_coll == 0){
            output.push_back({i_coll , j_coll+1}) ;
            output.push_back({i_coll+1 , j_coll+1}) ;
            output.push_back({i_coll+1 , j_coll}) ;
            return 1 ;
        }else if(j_coll == n_columns-1){
            output.push_back({i_coll , j_coll-1}) ;
            output.push_back({i_coll+1 , j_coll}) ;
            return 1 ;
        }else{
            output.push_back({i_coll , j_coll+1}) ;
            output.push_back({i_coll , j_coll-1}) ;
            output.push_back({i_coll+1 , j_coll+1}) ;
            output.push_back({i_coll+1 , j_coll}) ;
            return 1 ;
        }
    }else if(i_coll == 1 && balls[i_coll][j_coll].eo == -1){ //3
        if(j_coll == 0){
            output.push_back({i_coll , j_coll+1}) ;
            output.push_back({i_coll+1 , j_coll}) ;
            return 1 ;
        }else if(j_coll == n_columns-1){
            output.push_back({i_coll , j_coll-1}) ;
            output.push_back({i_coll+1 , j_coll}) ;
            output.push_back({i_coll+1 , j_coll-1}) ;
            return 1 ;
        }else{
            output.push_back({i_coll , j_coll+1}) ;
            output.push_back({i_coll , j_coll-1}) ;
            output.push_back({i_coll+1 , j_coll}) ;
            output.push_back({i_coll+1 , j_coll-1}) ;
            return 1 ;
        }
    }else if(balls[i_coll][j_coll].eo == 1){// 5
        if(j_coll == 0){
            output.push_back({i_coll , j_coll+1}) ;
            output.push_back({i_coll+1 , j_coll+1}) ;
            output.push_back({i_coll+1 , j_coll}) ;
            output.push_back({i_coll-1 , j_coll+1}) ;
            output.push_back({i_coll-1 , j_coll}) ;
            return 1 ;
        }else if(j_coll == n_columns-1){
            output.push_back({i_coll , j_coll-1}) ;
            output.push_back({i_coll+1 , j_coll}) ;
            output.push_back({i_coll-1 , j_coll}) ;
            return 1 ;
        }else{
            output.push_back({i_coll , j_coll+1}) ;
            output.push_back({i_coll , j_coll-1}) ;
            output.push_back({i_coll+1 , j_coll+1}) ;
            output.push_back({i_coll+1 , j_coll}) ;
            output.push_back({i_coll-1 , j_coll+1}) ;//
            output.push_back({i_coll-1 , j_coll}) ;//
            return 1 ;
        }
    }else if(balls[i_coll][j_coll].eo == -1){ //3
        if(j_coll == 0){
            output.push_back({i_coll , j_coll+1}) ;
            output.push_back({i_coll+1 , j_coll}) ;
            output.push_back({i_coll-1 , j_coll}) ;
            return 1 ;
        }else if(j_coll == n_columns-1){
            output.push_back({i_coll , j_coll-1}) ;
            output.push_back({i_coll+1 , j_coll}) ;
            output.push_back({i_coll+1 , j_coll-1}) ;
            output.push_back({i_coll-1 , j_coll}) ;
            output.push_back({i_coll-1 , j_coll-1}) ;

            return 1 ;
        }else{
            output.push_back({i_coll , j_coll+1}) ;
            output.push_back({i_coll , j_coll-1}) ;
            output.push_back({i_coll+1 , j_coll}) ;
            output.push_back({i_coll+1 , j_coll-1}) ;
            output.push_back({i_coll-1 , j_coll}) ;//
            output.push_back({i_coll-1 , j_coll-1}) ;//
            return 1 ;
        }
    }
}

int same_color (vector< array<int , 2> >  &output , ball2d balls[n_rows][n_columns] , int i_coll , int j_coll , int i_upper_ball , int j_upper_ball , vector<array<int , 2>> & changed_balls , int r , int g , int b){
    bool flag1 = false ;
    vector <array <int , 2> > neighbors ;
    for (auto & i : output) {
        if(balls[i[0]][i[1]].is_exist && balls[i[0]][i[1]].rgb[0] == r && balls[i[0]][i[1]].rgb[1] == g && balls[i[0]][i[1]].rgb[2] == b){
            neighbors.push_back({i[0],i[1]}) ;
            flag1 = true ;
        }
        if(balls[i[0]][i[1]].is_exist && balls[i[0]][i[1]].multicolor && balls[i[0]][i[1]].rgb1[0] == r && balls[i[0]][i[1]].rgb1[1] == g && balls[i[0]][i[1]].rgb1[2] == b){
            neighbors.push_back({i[0],i[1]}) ;
            flag1 = true ;
        }
    }
    if(flag1){
        changed_balls.push_back({i_coll , j_coll}) ;
        balls[i_coll][j_coll].is_exist = false ;
    }
    while(!neighbors.empty()){
        i_upper_ball = neighbors.back().at(0) ;
        j_upper_ball = neighbors.back().at(1) ;
        neighbors.pop_back() ;
        finding_balls_same_color(output , balls , i_upper_ball , j_upper_ball , changed_balls , r , g , b) ;
        changed_balls.push_back({i_upper_ball , j_upper_ball}) ;
        balls[i_upper_ball][j_upper_ball].is_exist = false ;
    }
}

int finding_balls_same_color (vector< array<int , 2> >  &output , ball2d balls[n_rows][n_columns] , int i_coll , int j_coll , vector<array<int , 2>> & changed_balls , int r , int g , int b) {
    if(k_upbl > 1){
        finding_neighbors_balls_new(i_coll,j_coll, balls , output) ;
    }else
        finding_neighbors_balls(i_coll, j_coll, balls , output) ;
//    cout << i_coll << "   " << j_coll <<"      "<< k_upbl << "               ";
//    for (auto i : output) {
//        cout << i[0] << " " ;
//    }cout << endl ;
    same_color (output , balls , i_coll , j_coll , i_coll , j_coll , changed_balls , r , g , b) ;
}

int checkBallConnection(ball2d balls[n_rows][n_columns] , int p , int q){
    if(balls[p][q].is_exist){
        balls[p][q].connect = true;
        if(balls[p][q].eo == 1){
            if(p-1>=0)
                if(!balls[p-1][q].connect)
                    checkBallConnection(balls , p-1 , q);
            if(p-1>=0 && n_columns>q+1)
                if(!balls[p-1][q+1].connect)
                    checkBallConnection(balls , p-1 , q+1);
            if(n_rows>p+1)
                if(!balls[p+1][q].connect)
                    checkBallConnection(balls , p+1 , q);
            if(n_rows>p+1 && n_columns>q+1)
                if(!balls[p+1][q+1].connect)
                    checkBallConnection(balls , p+1 , q+1);
            if(n_columns>q+1)
                if(!balls[p][q+1].connect)
                    checkBallConnection(balls , p , q+1);
            if(q-1>=0)
                if(!balls[p][q-1].connect)
                    checkBallConnection(balls , p , q-1);
        }else{
            if(p-1>=0)
                if(!balls[p-1][q].connect)
                    checkBallConnection(balls , p-1 , q);
            if(p-1>=0 && q-1>=0)
                if(!balls[p-1][q-1].connect)
                    checkBallConnection(balls , p-1 , q-1);
            if(n_rows>p+1)
                if(!balls[p+1][q].connect)
                    checkBallConnection(balls , p+1 , q);
            if(n_rows>p+1 && q-1>=0)
                if(!balls[p+1][q-1].connect)
                    checkBallConnection(balls , p+1 , q-1);
            if(n_columns>q+1)
                if(!balls[p][q+1].connect)
                    checkBallConnection(balls , p , q+1);
            if(q-1>=0)
                if(!balls[p][q-1].connect)
                    checkBallConnection(balls , p , q-1);
        }
    }
}

void checkGameStatus(SDL_Renderer * renderer ,  int W  , int H , bool & playing , bool started , ball2d balls[n_rows][n_columns] , double deadline_y){
    int R = int(W / (n_columns * 2 + 1));
    if(ceil_y >= -2 * R){
        for(int y = 0 ; y < n_rows ; y++){
            for (int j = 0; j < n_columns; j++) {
                if (balls[y][j].is_exist && balls[y][j].y > deadline_y){
                    sec = 2 ;
                    presec = 50 ;
                    //cout << "game over " << endl;
                }
            }
        }
    }else{
        for (int j = 0; j < n_columns; j++) {
            if (balls[n_rows - 1][j].is_exist && balls[n_rows - 1][j].y > deadline_y){
                sec = 2 ;
                presec = 50 ;
                //cout << "game over " << endl;
            }
        }
    }
}

int balls_go_down(SDL_Renderer * renderer , int W , int H , ball2d balls[n_rows][n_columns] , ball2d balls_out[n_rows][n_columns] , double deadline_y){
    int R = int ( W/ (n_columns*2+1) );
    int W_min = (W-(R*(n_columns*2+1)))/2;
    int W_max = (W-(R*(n_columns*2+1)))/2+((R*(n_columns*2+1)));
    if(balls[n_rows-1][n_columns-1].y < deadline_y) {
        for (int i = 0; i < n_rows; i++) {
            for (int j = 0; j < n_columns; j++) {
                balls[i][j].y += vel_balls_go_down ;
            }
        }
    }else {
        if (!update_balls(renderer, W, H, balls, balls_out)) {
            for (int i = 0; i < n_rows; i++) {
                for (int j = 0; j < n_columns; j++) {
                    balls[i][j].y += vel_balls_go_down;
                }
            }
        }
    }
    return 1;
}

int laser_ball_beaming (SDL_Renderer * renderer , int W , int H , int mouseX , int mouseY , double vel_x , double vel_y , flying_color2d flying_color , color2d colors[8] , ball2d & flying_bullet , ball2d balls[n_rows][n_columns] , vector <array<int , 2> > &output){
    int R = int ( W/ (n_columns*2+1) );
    int W_min = (W-(R*(n_columns*2+1)))/2;
    int W_max = (W-(R*(n_columns*2+1)))/2+((R*(n_columns*2+1)));

    ball2d bullet;
    bullet.x = W/2;
    bullet.y = H - R;

    int wgo = 0; //out put of func: wanna_go_out
    int new_ball_row = 0 ;
    int new_ball_column = 0 ;
    while(flying_bullet.y > 0){
        wgo =wanna_go_out(flying_bullet.x , flying_bullet.y , W , H , R);
        if(wgo == 2 || wgo == 4){
            vel_x *= -1;
        }
        flying_bullet.x += vel_x;
        flying_bullet.y += vel_y;
        filledEllipseRGBA(renderer, flying_bullet.x, flying_bullet.y, R , R , flying_color.r, flying_color.g, flying_color.b, 150);
        checking_collision_laser(renderer, W, H, balls, flying_bullet, vel_x, vel_y, flying_color, new_ball_row,
                                 new_ball_column , R*R , output) ;

        SDL_RenderPresent(renderer) ;
    }
    flying_bullet.x = W / 2;
    flying_bullet.y = H - R;


    return 1;
}

int checking_collision_laser(SDL_Renderer * renderer , int W , int H , ball2d balls[n_rows][n_columns] , ball2d & flying_bullet, double &vel_x , double &vel_y , flying_color2d flying_color , int &new_ball_row , int &new_ball_column , int R_collision , vector <array<int , 2> > &output){
    double R = int ( W/ (n_columns*2+1) );
    int W_min = (W-(R*(n_columns*2+1)))/2;
    int W_max = (W-(R*(n_columns*2+1)))/2+((R*(n_columns*2+1)));
    int i_empty =0 , j_empty=0 ;
    double distance_temp=10000 ;
    bool checked = false;
    double min_dist = 10000;
    double dist1;
    double dist2;
    int i_coll , j_coll;
    for (int i = n_rows-1; i >=0 ; i--) {
        if(i == n_rows-1){
            for (int j = 0; j < n_columns; ++j) {
                dist1 = (flying_bullet.x-balls[i-1][j].x)*(flying_bullet.x-balls[i-1][j].x)+(flying_bullet.y-balls[i-1][j].y)*(flying_bullet.y-balls[i-1][j].y);
                dist2 = (flying_bullet.x-balls[i][j].x)*(flying_bullet.x-balls[i][j].x)+(flying_bullet.y-balls[i][j].y)*(flying_bullet.y-balls[i][j].y);
                if(balls[i-1][j].is_exist && dist1<=R_collision){
                    if(dist1 < min_dist){
                        min_dist = dist1;
                        i_coll = i-1;
                        j_coll = j;
                    }
                    checked = true;
                }else if(balls[i][j].is_exist && dist2<=R_collision){
                    if(dist2 < min_dist){
                        min_dist = dist2;
                        i_coll = i;
                        j_coll = j;
                    }
                    checked = true;
                }
            }
        }else if(flying_bullet.y<=balls[i][0].y){
            for (int j = 0; j < n_columns; ++j) {
                dist1 = (flying_bullet.x-balls[i-1][j].x)*(flying_bullet.x-balls[i-1][j].x)+(flying_bullet.y-balls[i-1][j].y)*(flying_bullet.y-balls[i-1][j].y);
                dist2 = (flying_bullet.x-balls[i][j].x)*(flying_bullet.x-balls[i][j].x)+(flying_bullet.y-balls[i][j].y)*(flying_bullet.y-balls[i][j].y);
                if(balls[i-1][j].is_exist && dist1<=R_collision){
                    if(dist1 < min_dist){
                        min_dist = dist1;
                        i_coll = i-1;
                        j_coll = j;
                    }
                    checked = true;
                }else if(balls[i][j].is_exist && dist2<=R_collision){
                    if(dist2 < min_dist){
                        min_dist = dist2;
                        i_coll = i;
                        j_coll = j;
                    }
                    checked = true;
                }
            }
        }
        if(checked){
            float collision_x = float(balls[i_coll][j_coll].x+flying_bullet.x)/2 ;
            float collision_y = float(balls[i_coll][j_coll].y+flying_bullet.y)/2 ;
            if(effects)
                filledEllipseRGBA(renderer , collision_x , collision_y , R*1.2 , R*1.2 , 0 ,255 ,255 ,255);
            balls[i_coll][j_coll].is_exist = false ;
            balls[i_coll][j_coll].locked = false ;
            balls[i_coll][j_coll].multicolor = false ;
            if(k_upbl > 1){
                finding_neighbors_balls_new(i_coll, j_coll, balls , output) ;
            }else
                finding_neighbors_balls(i_coll , j_coll , balls , output) ;
            for(auto k:output){
                if(balls[k[0]][k[1]].is_exist && distance(collision_x , collision_y , balls[k[0]][k[1]].x , balls[k[0]][k[1]].y) <= 2.2*R){
                    balls[k[0]][k[1]].is_exist = false ;
                    balls[k[0]][k[1]].locked = false ;
                    balls[k[0]][k[1]].multicolor = false ;
                }
            }
            output.clear() ;
            return 1 ;
        }
    }
    return 0 ;
}

int mega_bomb_beaming(SDL_Renderer * renderer , int W , int H , int mouseX , int mouseY , ball2d & flying_bullet , double &vel_x , double &vel_y , flying_color2d flying_color , color2d colors[8] , int mega_phrase){
    int R = int ( W/ (n_columns*2+1) );
    int W_min = (W-(R*(n_columns*2+1)))/2;
    int W_max = (W-(R*(n_columns*2+1)))/2+((R*(n_columns*2+1)));
    int wgo = 0; //out put of func: wanna_go_out
    int newR ;
    if(mega_phrase == 1){
        newR = R ;
    }else if(mega_phrase == 2){
        newR = R * 1.5 ;
    }else if(mega_phrase == 3){
        newR = R * 2 ;
    }
    wgo = wanna_go_out(flying_bullet.x, flying_bullet.y, W, H, R);
    if (wgo == 2 || wgo == 4) {
        vel_x *= -1;
    }
    if(wgo == 1){
        vel_y *= -1;
    }
    flying_bullet.x += vel_x;
    flying_bullet.y += vel_y;
    filledEllipseRGBA(renderer, flying_bullet.x, flying_bullet.y, newR, newR, flying_color.r , flying_color.g , flying_color.b , 255);
    return 1;
}

int checking_collision_mega_bomb(SDL_Renderer * renderer , int W , int H , ball2d balls[n_rows][n_columns] , ball2d & flying_bullet , flying_color2d flying_color , int &new_ball_row , int &new_ball_column , int mega_phrase, vector< array <int , 2 >> &destructed_balls) {
    double R = int ( W/ (n_columns*2+1) );
    int W_min = (W-(R*(n_columns*2+1)))/2;
    int W_max = (W-(R*(n_columns*2+1)))/2+((R*(n_columns*2+1)));
    int i_empty =0 , j_empty=0 ;
    double distance_temp=10000 ;
    bool checked = false;
    double min_dist = 10000;
    int R_collision ;
    int R_destruction ;
    int delay ;
    if(mega_phrase == 1){
        R_collision = R * 2 ;
        R_destruction = R * 4 ;
        delay = 100 ;
    }else if(mega_phrase == 2){
        R_collision = R * 2.5 ;
        R_destruction = R * 7 ;
        delay = 150 ;
    }else if(mega_phrase == 3){
        R_collision = R * 3 ;
        R_destruction = R * 10 ;
        delay = 200 ;
    }
    double dist1;
    double dist2;
    int i_coll , j_coll;
    for (int i = n_rows-1; i >=0 ; i--) {
        if(i == n_rows-1){
            for (int j = 0; j < n_columns; ++j) {
                dist1 = (flying_bullet.x-balls[i-1][j].x)*(flying_bullet.x-balls[i-1][j].x)+(flying_bullet.y-balls[i-1][j].y)*(flying_bullet.y-balls[i-1][j].y);
                dist2 = (flying_bullet.x-balls[i][j].x)*(flying_bullet.x-balls[i][j].x)+(flying_bullet.y-balls[i][j].y)*(flying_bullet.y-balls[i][j].y);
                if(balls[i-1][j].is_exist && dist1<=R_collision*R_collision){
                    if(dist1 < min_dist){
                        min_dist = dist1;
                        i_coll = i-1;
                        j_coll = j;
                    }
                    checked = true;
                }else if(balls[i][j].is_exist && dist2<=R_collision*R_collision){
                    if(dist2 < min_dist){
                        min_dist = dist2;
                        i_coll = i;
                        j_coll = j;
                    }
                    checked = true;
                }
            }
        }else if(flying_bullet.y<=balls[i][0].y){
            for (int j = 0; j < n_columns; ++j) {
                dist1 = (flying_bullet.x-balls[i-1][j].x)*(flying_bullet.x-balls[i-1][j].x)+(flying_bullet.y-balls[i-1][j].y)*(flying_bullet.y-balls[i-1][j].y);
                dist2 = (flying_bullet.x-balls[i][j].x)*(flying_bullet.x-balls[i][j].x)+(flying_bullet.y-balls[i][j].y)*(flying_bullet.y-balls[i][j].y);
                if(balls[i-1][j].is_exist && dist1<=R_collision*R_collision){
                    if(dist1 < min_dist){
                        min_dist = dist1;
                        i_coll = i-1;
                        j_coll = j;
                    }
                    checked = true;
                }else if(balls[i][j].is_exist && dist2<=R_collision*R_collision){
                    if(dist2 < min_dist){
                        min_dist = dist2;
                        i_coll = i;
                        j_coll = j;
                    }
                    checked = true;
                }
            }
        }
        if(checked){
            destructed_balls.clear() ;
            float collision_x = float(balls[i_coll][j_coll].x+flying_bullet.x)/2 ;
            float collision_y = float(balls[i_coll][j_coll].y+flying_bullet.y)/2 ;
            for (int R_destruction_temp = (2 * R) ; R_destruction_temp <= R_destruction; R_destruction_temp+=R) {
                if(effects){
                    filledCircleRGBA(renderer , flying_bullet.x , flying_bullet.y , R_destruction_temp , flying_color.r , flying_color.g , flying_color.b , 70) ;
                    SDL_RenderPresent(renderer) ;
                }
                for (int j = 0; j < n_rows; ++j) {
                    for (int k = 0; k < n_columns; ++k) {
                        if(balls[j][k].is_exist && distance(flying_bullet.x , flying_bullet.y , balls[j][k].x , balls[j][k].y) <= R_destruction_temp){
                            destructed_balls.push_back({j,k}) ;
                            balls[j][k].is_exist = false ;
                            balls[j][k].locked = false ;
                            balls[j][k].multicolor = false ;
                            if(effects)
                                filledCircleRGBA(renderer , balls[j][k].x , balls[j][k].y , R * 0.8 , flying_color.r , flying_color.g , flying_color.b , 255) ;
                        }
                    }
                }
                if(effects)
                    SDL_RenderPresent(renderer) ;
                SDL_Delay(delay) ;
            }

            return 1 ;
        }
    }
    return 0 ;
}

int checking_collision_ceil(SDL_Renderer * renderer , int W , int H , ball2d balls[n_rows][n_columns] , ball2d & flying_bullet , double &vel_x , double &vel_y , flying_color2d flying_color  , int &new_ball_row , int &new_ball_column , int R_collision){
    double R = int ( W/ (n_columns*2+1) );
    int W_min = (W-(R*(n_columns*2+1)))/2;
    int W_max = (W-(R*(n_columns*2+1)))/2+((R*(n_columns*2+1)));
    int i_empty =0 , j_empty=0 ;
    double distance_temp=10000 ;
    bool checked = false;
    double min_dist = 10000;
    double dist1;
    double dist2;
    int i_coll , j_coll;
    int i = 0 ;
    for (int j = 0; j < n_columns; ++j) {
        dist2 = abs(flying_bullet.x-balls[i][j].x);
        if(!balls[i][j].is_exist){
            if(dist2 < min_dist){
                min_dist = dist2;
                i_coll = i;
                j_coll = j;
            }
            checked = true;
        }
    }
    if(checked){
        balls[i_coll][j_coll].rgb[0] = flying_color.r ;
        balls[i_coll][j_coll].rgb[1] = flying_color.g ;
        balls[i_coll][j_coll].rgb[2] = flying_color.b ;
        balls[i_coll][j_coll].is_exist = true ;
        new_ball_row = i_coll ;
        new_ball_column = j_coll ;
    }
    return 0;
}