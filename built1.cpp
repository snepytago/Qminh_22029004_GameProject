#include <bits/stdc++.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <string>
#include <fstream>
using namespace std;

//**************************************************************
const int SCREEN_WIDTH = 338;
const int SCREEN_HEIGHT = 600;
const string WINDOW_TITLE = "Falling Bombs";
// khởi tạo các thành phần trong game
struct vitri{
    int x;
    int y;
    };
struct hinhdang{
    int h,w;
    };
struct maybay{
    vitri vt;
    hinhdang hd;
    int diem;
    int stepdiem;
    int mang;
    int highscore;
    };
struct bomb{
    vitri vt1;
    hinhdang hd1;
    int stepbomb;
    };
// Các hàm chung về khởi tạo và huỷ SDL
void logSDLError(std::ostream& os,
                 const std::string &msg, bool fatal = false);
void initSDL(SDL_Window* &window, SDL_Renderer* &renderer);
void quitSDL(SDL_Window* window, SDL_Renderer* renderer);
//**************************************************************
//Các hàm load ảnh
SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren);
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y);
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h);
void renderttf(SDL_Texture *tex, SDL_Renderer *ren, TTF_Font* font, string text, int x, int y, int w, int h);
//Các hàm chạy chương trình
int Loading(SDL_Window* &window, SDL_Renderer* &renderer);
int menu(SDL_Window* &window, SDL_Renderer* &renderer);
int level(SDL_Window* &window, SDL_Renderer* &renderer);
void HienThi(SDL_Window* &window, SDL_Renderer* &renderer,maybay &mb, bomb &b,bomb &b1,int &congtac1,int &congtac2);
int DieuKhien(maybay &mb);
void tangtocbomb(bomb &b,bomb &b1,maybay &mb,int &a);
int Xuly(maybay &mb, bomb &b, bomb &b1, int &congtac);
int choilai(SDL_Window* &window, SDL_Renderer* &renderer,maybay &mb);
void vuno(SDL_Window* &window, SDL_Renderer* &renderer, maybay &mb);
void writehighscore(int &highscore, int &score);
int readhighscore();
//***HÀM MAIN***
int main(int argc, char* argv[])
{
    SDL_Window* window;
    SDL_Renderer* renderer;
    initSDL(window, renderer);
    Loading(window, renderer);
    //khởi tạo bộ sinh số ngẫu nhiên
    srand (time (NULL));
    //khởi tạo máy bay
    maybay mb;
    mb.stepdiem = 50;
    //khởi tạo bomb
    bomb b, b1;
    b.stepbomb = 3;
    b1.stepbomb = 4;
    trolaimenu:
    int m = menu(window, renderer);
    if(m == 0)exit(0);
    if(m == 2) {
        int l = level(window, renderer);
        if (l == 1){
            b.stepbomb = 3;
            b1.stepbomb = 4;
            mb.stepdiem = 50;
            goto trolaimenu;
            }
        if (l == 2){
            b.stepbomb = 5;
            b1.stepbomb = 8;
            mb.stepdiem = 75;
            goto trolaimenu;
            }
        if (l == 3){
            b.stepbomb = 8;
            b1.stepbomb = 10;
            mb.stepdiem = 100;
            goto trolaimenu;
            }
        }
    playagain:
    if(m == 1){
        mb.highscore = readhighscore();
        mb.diem = 0;
        mb.vt.x = SCREEN_WIDTH / 2;
        mb.vt.y = 500;
        mb.hd.h = 70;
        mb.hd.w = 80;
        mb.mang = 3;
        int congtac1 = 1, congtac2 = 1;
        b.vt1.x = rand() % (SCREEN_WIDTH);
        b.vt1.y = 0;
        b.hd1.h = 50;
        b.hd1.w = 40;
        if (b.vt1.x > 200) b1.vt1.x = b.vt1.x- 150;
        else b1.vt1.x = b.vt1.x + 80;
        b1.vt1.y = 0;
        b1.hd1.h = 50;
        b1.hd1.w = 40;

    while (true) {
        // hiển thị
		HienThi (window, renderer,mb, b, b1,congtac1, congtac2);

        // điều khiển
		int x;
		x = DieuKhien (mb);
		if(x == -1)exit(0);

		//tăng tốc độ bomb
		int a;
        tangtocbomb(b, b1, mb,a);

        // xử lý
		int m1, m2;
		m = Xuly (mb,b,b1, congtac1);
		m1 = Xuly (mb,b1,b, congtac2);
        // game over
		if (m == -1||m1 == -1)
		{
		    vuno(window, renderer,mb);
			break;
		}
		writehighscore(mb.highscore, mb.diem);
	}
	int i;
    i = choilai(window, renderer,mb);
    if (i == 1){
        m=1;
        goto playagain;
        }
    if (i == 2)goto trolaimenu;
    if (i == 3) exit(0);

    //QuitSDL
    quitSDL(window, renderer);
    return 0;
    }
}

//*****************************************************
// Các hàm chung về khởi tạo và huỷ SDL
void logSDLError(std::ostream& os,
                 const std::string &msg, bool fatal)
{
    os << msg << " Error: " << SDL_GetError() << std::endl;
    if (fatal) {
        SDL_Quit();
        exit(1);
    }
}

void initSDL(SDL_Window* &window, SDL_Renderer* &renderer)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        logSDLError(std::cout, "SDL_Init", true);
    if (TTF_Init() != 0)
        logSDLError(std::cout, "SDL_TTF", true);
    window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED,
       SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if (window == nullptr) logSDLError(std::cout, "CreateWindow", true);



    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |
                                              SDL_RENDERER_PRESENTVSYNC);

    if (renderer == nullptr) logSDLError(std::cout, "CreateRenderer", true);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void quitSDL(SDL_Window* window, SDL_Renderer* renderer)
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_Quit();
	SDL_Quit();
}

//*** CÁC HÀM LOAD ẢNH
SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren)
{
	//Khởi tạo là nullptr để tránh lỗi 'dangling pointer'
	SDL_Texture *texture = nullptr;
	//Nạp ảnh từ tên file (với đường dẫn)
	SDL_Surface *loadedImage = SDL_LoadBMP(file.c_str());
	//Nếu không có lỗi, chuyển đổi về dạng texture and và trả về
	if (loadedImage != nullptr){
		texture = SDL_CreateTextureFromSurface(ren, loadedImage);
		SDL_FreeSurface(loadedImage);
		//Đảm bảo việc chuyển đổi không có lỗi
		if (texture == nullptr){
			logSDLError(std::cout, "CreateTextureFromSurface");
		}
	}
	else {
		logSDLError(std::cout, "LoadBMP");
	}
	return texture;
}

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y)
{
	//Thiết lập hình chữ nhật đích mà chúng ta muốn vẽ ảnh vào trong
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	//Truy vẫn texture để lấy chiều rộng và cao (vào chiều rộng và cao tương ứng của hình chữ nhật đích)
	SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
    //Đưa toàn bộ ảnh trong texture vào hình chữ nhật đích
	SDL_RenderCopy(ren, tex, NULL, &dst);
}

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h)
{
	//Thiết lập hình chữ nhật đích mà chúng ta muốn vẽ ảnh vào trong
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
    dst.w = w;
    dst.h = h;
    //Đưa toàn bộ ảnh trong texture vào hình chữ nhật đích
    //(ảnh sẽ co dãn cho khớp với kích cỡ mới)
	SDL_RenderCopy(ren, tex, NULL, &dst);
}
void renderttf(SDL_Texture *tex, SDL_Renderer *ren, TTF_Font* font, string text,int x, int y, int w, int h)
{
	//Thiết lập hình chữ nhật đích mà chúng ta muốn viết chữ vào trong
	SDL_Rect rect;
	TTF_SizeText(font, text.c_str(), &rect.w, &rect.h);

	rect.x = x;
	rect.y = y;

	rect.w = w;
	rect.h = h;
    //Copy a portion of the texture to the current rendering target.
    SDL_RenderCopy(ren, tex, NULL, &rect);
}

//**************************************************************
//CÁC HÀM THỰC HIỆN CHƯƠNG TRÌNH
int Loading(SDL_Window* &window, SDL_Renderer* &renderer){
    vector<SDL_Texture *> star;
    star.push_back(loadTexture("image/star/1.bmp", renderer));
    star.push_back(loadTexture("image/star/2.bmp", renderer));
    star.push_back(loadTexture("image/star/3.bmp", renderer));
    star.push_back(loadTexture("image/star/4.bmp", renderer));
    star.push_back(loadTexture("image/star/5.bmp", renderer));
    star.push_back(loadTexture("image/star/6.bmp", renderer));
    star.push_back(loadTexture("image/star/7.bmp", renderer));
    star.push_back(loadTexture("image/star/8.bmp", renderer));
    star.push_back(loadTexture("image/star/1.bmp", renderer));
    int imageNum = star.size();
    for (int i = 0; i < imageNum; ++i) {
        if (star[i] == nullptr) {
            for (int j = 0; j < imageNum; ++j) SDL_DestroyTexture(star[j]);
            quitSDL(window, renderer);
            return 1;
        }
    }
    int iSize = 60; //Kích cỡ ảnh
    int x = SCREEN_WIDTH / 2 - iSize / 2;
    int y = SCREEN_HEIGHT / 2 - iSize / 2;
    for (int j = 0; j < imageNum; ++j){
        // Delay 150 mili giây
        SDL_Delay(150);
        // Xoá màn hình
        SDL_RenderClear(renderer);
        //Vẽ ngôi sao số j
        renderTexture(star[j], renderer, x, y, iSize, iSize);
        // Dùng lệnh hiển thị (đưa) hình đã vẽ ra mành hình
        SDL_RenderPresent(renderer);
    }
    for (int j = 0; j < imageNum; ++j) SDL_DestroyTexture(star[j]);
    }
int menu(SDL_Window* &window, SDL_Renderer* &renderer){
    //Nạp ảnh nền vào một biến texture
    SDL_Texture *background = loadTexture("image/game-menu-1.bmp", renderer);
    //Nếu có lỗi thì giải phóng hai texture đã nạp, rồi kết thúc
    if (background == nullptr){
        SDL_DestroyTexture(background);
        quitSDL(window, renderer);
        }
    //Xoá màn hình
    SDL_RenderClear(renderer);
    //Vẽ ảnh nền vào toàn bộ cửa sổ
    renderTexture(background, renderer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    // Dùng lệnh hiển thị (đưa) hình đã vẽ ra mành hình
    SDL_RenderPresent(renderer);
    //Giải phóng biến texture đã dùng
    SDL_DestroyTexture(background);

    //khởi tạo biến event
    SDL_Event e;

    while (true) {
        // Đợi 10 mili giây
        SDL_Delay(10);

        while (SDL_PollEvent(&e)) {

            // Nếu sự kiện là kết thúc (như đóng cửa sổ) thì thoát khỏi vòng lặp
            if (e.type == SDL_QUIT) return 0;

            // Nếu nhấn phìm ESC thì thoát khỏi vòng lặp
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) return 0;

            // Nếu chuột (mouse) được nhấn (xuống)
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x = e.button.x; // Lấy hoành độ x của chuột
                int y = e.button.y; // Lấy tung độ y của chuột
                if(x<181&&x>23&&y<418&&y>380)return 1;
                if(x<306&&x>150&&y<578&&y>540)return 0;
                if(x<300&&x>100&&y<498&&y>460)return 2;
                }
            }
        }
    }
int level(SDL_Window* &window, SDL_Renderer* &renderer){
    //Nạp ảnh nền vào một biến texture
    SDL_Texture *background = loadTexture("image/game-level.bmp", renderer);
    //Nếu có lỗi thì giải phóng hai texture đã nạp, rồi kết thúc
    if (background == nullptr){
        SDL_DestroyTexture(background);
        quitSDL(window, renderer);
        }
    //Xoá màn hình
    SDL_RenderClear(renderer);
    //Vẽ ảnh nền vào toàn bộ cửa sổ
    renderTexture(background, renderer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    // Dùng lệnh hiển thị (đưa) hình đã vẽ ra mành hình
    SDL_RenderPresent(renderer);
    //Giải phóng 2 biến texture đã dùng
    SDL_DestroyTexture(background);

    //khởi tạo biến event
    SDL_Event e;

    while (true) {
        // Đợi 10 mili giây
        SDL_Delay(10);

        while (SDL_PollEvent(&e)) {

            // Nếu sự kiện là kết thúc (như đóng cửa sổ) thì thoát khỏi vòng lặp
            if (e.type == SDL_QUIT) return 0;

            // Nếu nhấn phìm ESC thì thoát khỏi vòng lặp
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) return 0;

            // Nếu chuột (mouse) được nhấn (xuống)
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x = e.button.x; // Lấy hoành độ x của chuột
                int y = e.button.y; // Lấy tung độ y của chuột
                if(x<181&&x>23&&y<418&&y>380)return 1;
                if(x<309&&x>63&&y<578&&y>540)return 3;
                if(x<307&&x>150&&y<498&&y>460)return 2;
                }
            }
        }
    }

void HienThi(SDL_Window* &window, SDL_Renderer* &renderer,maybay &mb, bomb &b,bomb &b1,int &congtac1,int &congtac2){

    //nạp font
    TTF_Font* font = TTF_OpenFont("dlxfont.ttf", 30);
    //chọn màu chữ
    SDL_Color fg = { 243, 156, 18 };
    string text = "Score:";//Chuyển điểm int về string
    string scorestring, high;
    stringstream ss,hsc;
    ss<<mb.diem;
    hsc<<mb.highscore;
    scorestring = ss.str();
    high = hsc.str();
    text = text + scorestring + "/" + high;
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), fg);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

    //Nạp ảnh nền vào một biến texture
    SDL_Texture *background = loadTexture("image/bg600.bmp", renderer);
    //Nạp ảnh vào một biến texture
    SDL_Texture *image = loadTexture("image/maybay4.bmp", renderer);
    //Nạp ảnh vào một biến texture
    SDL_Texture *bombimage = loadTexture("image/bom1.bmp", renderer);
    //Nạp ảnh vào một biến texture
    SDL_Texture *healthybar1 = loadTexture("image/healthybar/healthybar1.bmp", renderer);
    //Nạp ảnh vào một biến texture
    SDL_Texture *healthybar2  = loadTexture("image/healthybar/healthybar2.bmp", renderer);
    //Nạp ảnh vào một biến texture
    SDL_Texture *healthybar3  = loadTexture("image/healthybar/healthybar3.bmp", renderer);
    //Nạp ảnh vào một biến texture
    SDL_Texture *maybaybomb = loadTexture("image/maybaybomb.bmp", renderer);
    //Nếu có lỗi thì giải phóng hai texture đã nạp, rồi kết thúc
    if (background == nullptr || image == nullptr){
        SDL_DestroyTexture(background);
        SDL_DestroyTexture(image);
        SDL_DestroyTexture(bombimage);
        SDL_DestroyTexture(healthybar1);
        SDL_DestroyTexture(healthybar2);
        SDL_DestroyTexture(healthybar3);
        SDL_DestroyTexture(maybaybomb);
        quitSDL(window, renderer);
    }
    //Xoá màn hình
    SDL_RenderClear(renderer);
    //Vẽ ảnh nền vào toàn bộ cửa sổ
    renderTexture(background, renderer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    //In điểm
    renderttf(texture, renderer, font, text,200,20,120,20);
    //In thanh sức khoẻ
    if (mb.mang == 1)renderTexture(healthybar1, renderer,15,15,47*2,14*2);
    if (mb.mang == 2)renderTexture(healthybar2, renderer,15,15,47*2,14*2);
    if (mb.mang == 3)renderTexture(healthybar3, renderer,15,15,47*2,14*2);
    //Hiển thị máy bay ở vị trí mới
    if (congtac1 == 0||congtac2 == 0)renderTexture(maybaybomb, renderer, mb.vt.x, mb.vt.y, mb.hd.w, mb.hd.h);
    else renderTexture(image, renderer, mb.vt.x, mb.vt.y, mb.hd.w, mb.hd.h);
    //Hiển thị bomb ở vị trí mới
    renderTexture(bombimage, renderer, b.vt1.x, b.vt1.y, b.hd1.w, b.hd1.h);
    renderTexture(bombimage, renderer, b1.vt1.x, b1.vt1.y, b1.hd1.w, b1.hd1.h);
    // Dùng lệnh hiển thị (đưa) hình đã vẽ ra mành hình
    SDL_RenderPresent(renderer);
    //Giải phóng 2 biến texture đã dùng
    SDL_DestroyTexture(background);
    SDL_DestroyTexture(image);
    SDL_DestroyTexture(bombimage);
    SDL_DestroyTexture(maybaybomb);
    SDL_DestroyTexture(texture);
    SDL_DestroyTexture(healthybar1);
    SDL_DestroyTexture(healthybar2);
    SDL_DestroyTexture(healthybar3);
    }
int DieuKhien(maybay &mb){
    //Gán biến đọc sự kiện
    SDL_Event e;
    //Bước nhảy của máy bay
    int step=10;
    // Đợi 5 mili giây
    SDL_Delay(5);
    while (SDL_PollEvent(&e)) {

    // Nếu sự kiện là kết thúc (như đóng cửa sổ) thì thoát khỏi vòng lặp
    if (e.type == SDL_QUIT) return -1;

    // Nếu có một phím được nhấn, thì xét phím đó là gì để xử lý tiếp
    if (e.type == SDL_KEYDOWN) {
        // Nếu nhấn phìm ESC thì thoát khỏi vòng lặp
        if (e.key.keysym.sym == SDLK_ESCAPE) return -1;

        // Nếu phím mũi tên trái, dịch sang trái
        // (cộng chiều rộng, trừ bước, rồi lấy phần dư để giá trị luôn dương, và hiệu ứng quay vòng)
        if (e.key.keysym.sym == SDLK_LEFT&&(mb.vt.x  - step)>-20) mb.vt.x = (mb.vt.x  - step);
        // Tương tự với dịch phải, xuống và lên
        if (e.key.keysym.sym == SDLK_RIGHT && mb.vt.x + step < SCREEN_WIDTH - mb.hd.w + 20) mb.vt.x = (mb.vt.x + step);
        if (e.key.keysym.sym == SDLK_DOWN && mb.vt.y + step < SCREEN_HEIGHT - mb.hd.h +25) mb.vt.y = (mb.vt.y + step) ;
        if (e.key.keysym.sym == SDLK_UP && (mb.vt.y - step)> SCREEN_HEIGHT/2) mb.vt.y = (mb.vt.y - step);
    }}
    }
void tangtocbomb(bomb &b,bomb &b1,maybay &mb,int &a){

    if (mb.diem%500 == 0&& a == 1){
        b.stepbomb+=1;
        b1.stepbomb+=1;
        a=0;
        }
    if (mb.diem%500 != 0){
        a=1;
        }
    }
int Xuly(maybay &mb, bomb &b, bomb &b1, int &congtac){
    //vật cản đi xuống
    b.vt1.y+=b.stepbomb;
    //tăng điểm và đưa ra bomb tiếp theo
    if (b.vt1.y > SCREEN_HEIGHT){
		mb.diem+=mb.stepdiem;
		b.vt1.x = abs(rand() % (SCREEN_WIDTH)- b.hd1.w);
		if (abs (b.vt1.x -b1.vt1.x)<50)b.vt1.x-=100;
		b.vt1.y = 0;
        }
    int dX = abs (mb.vt.x+50 - b.vt1.x-25);
    int dY = abs (mb.vt.y+50 - b.vt1.y-25);

    if (dX < 50 && dY < 50 && congtac == 1){
        mb.mang-=1;
        //cout<<mb.mang;
        congtac = 0;
        }
    if ((dX < 50 && dY < 50) == false ){
        congtac = 1;
        }
    if (mb.mang == 0){
        return -1;
        }
    return 0;
        }
void vuno(SDL_Window* &window, SDL_Renderer* &renderer, maybay &mb){
    int x = mb.vt.x + mb.hd.w /2,y = mb.vt.y + mb.hd.h/2;
    //Nạp ảnh nền vào một biến texture
    SDL_Texture *background = loadTexture("image/bg600.bmp", renderer);
    //Nạp ảnh vào một biến texture
    SDL_Texture *bam = loadTexture("image/bam.bmp", renderer);
    //Nạp ảnh vào một biến texture
    SDL_Texture *image = loadTexture("image/maybay4.bmp", renderer);
    //Nếu có lỗi thì giải phóng hai texture đã nạp, rồi kết thúc
    if (background == nullptr || bam == nullptr){
        SDL_DestroyTexture(background);
        SDL_DestroyTexture(bam);
        SDL_DestroyTexture(image);
        quitSDL(window, renderer);
        }
    //------VỤ NỔ-------
    for(int i=5; i<=80;i+=10){
        //Xoá màn hình
        SDL_RenderClear(renderer);
        //Vẽ ảnh nền vào toàn bộ cửa sổ
        renderTexture(background, renderer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        //Hiển thị máy bay ở vị trí mới
        renderTexture(image, renderer, mb.vt.x, mb.vt.y, mb.hd.w, mb.hd.h);
        //Vẽ ảnh bam vào toàn bộ cửa sổ
        renderTexture(bam, renderer, x, y,(7/6)*i ,i );
        // Dùng lệnh hiển thị (đưa) hình đã vẽ ra mành hình
        SDL_RenderPresent(renderer);
        x = x-35/6;
        y = y-5;
        }
        // Đợi 1000 mili giây
        SDL_Delay(1000);
        //Giải phóng 2 biến texture đã dùng
        SDL_DestroyTexture(bam);
        SDL_DestroyTexture(background);
        SDL_DestroyTexture(image);
    }
int choilai(SDL_Window* &window, SDL_Renderer* &renderer,maybay &mb){
        //Nạp ảnh nền vào một biến texture
        SDL_Texture *background = loadTexture("image/game over.bmp", renderer);
        //Nếu có lỗi thì giải phóng texture đã nạp, rồi kết thúc
        if (background == nullptr){
            SDL_DestroyTexture(background);
            quitSDL(window, renderer);
        }
        //nạp font
        TTF_Font* font = TTF_OpenFont("dlxfont.ttf", 30);
        //chọn màu chữ
        SDL_Color fg = { 243, 156, 18 };
        string text = "Score:";
        string scorestring, high;
        stringstream ss,hsc;
        ss<<mb.diem;
        hsc<<mb.highscore;
        scorestring = ss.str();
        high = hsc.str();
        text = text + scorestring + "/" + high;
        SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), fg);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        //-----MÀN HÌNH THUA CUOC------
        //Xoá màn hình
        SDL_RenderClear(renderer);
        //Vẽ ảnh nền vào toàn bộ cửa sổ
        renderTexture(background, renderer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        //In điểm ra màn hình với kích thước và vị trí có sẵn
        renderttf(texture, renderer, font, text,SCREEN_WIDTH/2 - 150,SCREEN_HEIGHT/2,200,50);
        // Dùng lệnh hiển thị (đưa) hình đã vẽ ra mành hình
        SDL_RenderPresent(renderer);
        //Giải phóng 2 biến texture đã dùng
        SDL_DestroyTexture(background);
        SDL_DestroyTexture(texture);
        //Gán biến đọc sự kiện
        SDL_Event e;
        // Đợi 100 mili giây
        SDL_Delay(100);
        while (1){
        while (SDL_PollEvent(&e)) {
            // Nếu sự kiện là kết thúc (như đóng cửa sổ) thì thoát khỏi vòng lặp
            if (e.type == SDL_QUIT) return 0;

            // Nếu nhấn phìm ESC thì thoát khỏi vòng lặp
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) return 0;

            // Nếu chuột (mouse) được nhấn (xuống)
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x = e.button.x; // Lấy hoành độ x của chuột
                int y = e.button.y; // Lấy tung độ y của chuột
                if(x<213&&x>27&&y<443&&y>364)return 1;
                if(x<245&&x>100&&y<510&&y>475)return 2;
                if(x<315&&x>171&&y<573&&y>537)return 3;
                }
            }
        }
    }
int readhighscore(){
    int highscore;
    ifstream file("HighScore.txt");
    if (file.is_open()) {
        while (file >> highscore) {
        //cout << highscore;
            }
        file.close();
    }
    else cout << "Error opening" ;
	return highscore;
}
void writehighscore(int &highscore, int &score){
    if(highscore<score){
        highscore = score;
        ofstream file ("HighScore.txt");
        if (file.is_open()) { // Kiểm tra việc mở tệp có thành công?
        file << highscore;
        file.close();
        }
        else cout << "Unable to open file";
            }
    }

