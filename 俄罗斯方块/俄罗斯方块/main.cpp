/*
创建人：妙啊
创建时间：2020/12/26
项目名称：俄罗斯方块
图形库：SFML图形库
修改日志：



项目进度：
--->>一天完成
--->>小问题，两天

*/
#include <SFML/Graphics.hpp>	//SFML图像库
#include <SFML/Audio.hpp>		//音乐图形库
#include <time.h>
#include <iostream>
#include <Windows.h>
using namespace std;
using namespace sf;
//#pragma comment(lib,"sfml-audio-d.lib")
//#pragma comment(lib,"sfml-graphics-d.lib")
//#pragma comment(lib,"sfml-window-d.lib")
//#pragma comment(lib,"sfml-system-d.lib")

#define SIZE		29		//方块素材大小29*29
#define GAMESPACE_X 11		//游戏空间大小11*17
#define GAMESPACE_Y 17
#define DOWN_SPEED	1200		//方块下降速度

int Game_Space[GAMESPACE_X][GAMESPACE_Y] = { 0 };//游戏空间大小，高30宽10，0空白，！0有方块
												//值为n，表示是第n中方块，且为第n中颜色
//int Old_Space[GAMESPACE_X * GAMESPACE_Y] = { 0 };//纪录落下方块的图标号 1-7

int KY = 0;//移动速度
int KL = 0;
int space = 0;

int Block_Kinds = 0;//为不重复，表示当前方块类型
int New = 0;//新方块使用的纹理

const int Blocks[7][4] = {//7种方块
	{1,3,5,7},//I字形
	{2,4,5,7},//倒Z字形
	{3,4,5,6},//正Z字形
	{3,4,5,7},//T字形
	{2,3,5,7},//倒L型
	{3,5,6,7},//正L型
	{2,3,4,5}//方形
};
struct position {//每个方块都由4个小方块组成，Pos_Block[4]表示4个小方块坐标位置，用于变换位置及方向
	int x, y;
};
struct position New_Block[4];//新生成方块的每个小方块位置
int Create_Flag = 1;//创建新方块标志，同一时间只有一个正在下降的方块
sf::Texture texture_block[8];//加载方块纹理
sf::Sprite sprite_bg;//创建背景精灵

sf::RenderWindow window;//创建主窗口

void Updata(float *Down_Speed);
void Get_Key();
void Create_Block();//生成新方快
void Draw_Blocks();//绘制方块

sf::Sprite sprite_old_block;//创建旧方块精灵，静止落地的
sf::Sprite sprite_new_block;//创建新方块精灵，正在下降的
sf::Music getbgm;


int main(void)
{
	ShowWindow(GetForegroundWindow(), 0);//隐藏黑窗口
	int Display_Count = 0;//显示计数器
	float Down_Count = 1;//下降计数器
	int Sys_Count = 0;//系统计数器
	/*加载游戏资源创建创建窗口*/
	window.create(sf::VideoMode(320, 520), "MiaoA");//初始化主窗口，视频模式，大小320*520，标题MiaoA
	sf::Texture texture_bg;//加载背景纹理
	if (!texture_bg.loadFromFile("bg.png")) {//加载背景图片纹理
		cout << "image bg.png load flase" << endl;
	}
	if (!texture_block[1].loadFromFile("1.png")) {
		cout << "image 1.png load flase" << endl;
	}
	if (!texture_block[2].loadFromFile("2.png")) {
		cout << "image 2.png load flase" << endl;
	}
	if (!texture_block[3].loadFromFile("3.png")) {
		cout << "image 3.png load flase" << endl;
	}
	if (!texture_block[4].loadFromFile("4.png")) {
		cout << "image 4.png load flase" << endl;
	}
	if (!texture_block[5].loadFromFile("5.png")) {
		cout << "image 5.png load flase" << endl;
	}
	if (!texture_block[6].loadFromFile("6.png")) {
		cout << "image 6.png load flase" << endl;
	}
	if (!texture_block[7].loadFromFile("7.png")) {
		cout << "image 7.png load flase" << endl;
	}

	sprite_bg.setTexture(texture_bg);//生成背景精灵
	window.draw(sprite_bg);//显示精灵背景图片
	window.display();//显示创建窗口
	/*加载游戏资源创建创建窗口*/
	sf::Music bgm;
	if (!getbgm.openFromFile("get.wav")) {
		return 0;
		cout << "get.wav load flase" << endl;
	}
	if (!bgm.openFromFile("bgm.wav")) {
		return 0;
		cout << "bgm.wav load flase" << endl;
	}
	bgm.setLoop(true);//循环播放
	bgm.setVolume(50.f);
	bgm.play();

	while (window.isOpen()){//游戏主循环

		if (Sys_Count > 60000)
			Sys_Count = 0;
		Sys_Count++;
		Down_Count = Sys_Count % DOWN_SPEED;

		Get_Key();//按键输入
		Updata(&Down_Count);//数据更新
		Create_Block();//生成新方块
		Draw_Blocks();//绘出所有方块
		window.display();//刷新显示
	}
	//system("pause");
	return 0;
}


void Get_Key() {//按键输入

	Event key;//按键事件变量，每次只接收一个按键
	while (window.pollEvent(key)) {

		if (key.type == Event::KeyPressed) {
			switch (key.key.code) {
				case Keyboard::Down:
					KY = 1;
					break;
				case Keyboard::Right:
					KL = 1;
					break;
				case Keyboard::Left:
					KL = -1;
					break;
				case Keyboard::Q:	//按Q键退出
					window.close();
					break;
				case Keyboard::Space:
					space = 1;
					break;

				default:
					break;
			}
		}
	}
}
void Draw_Blocks()//绘制方块
{
	window.draw(sprite_bg);//重新绘制背景
	sprite_new_block.move(10, 10);//设置偏移量

	//静止不动的方块，已经落地的
	for (int i = 0; i < GAMESPACE_X; i++) {
		for (int j = 0; j < GAMESPACE_Y; j++) {
			if (Game_Space[i][j] != 0) {//为1输出小方块
				sprite_old_block.setTexture(texture_block[ Game_Space[i][j] ]);//生成对应方块精灵
				sprite_old_block.setPosition(i * SIZE, j * SIZE);//设置显示位置一个方格大小29
				window.draw(sprite_old_block);//绘出方块
			}
		}
	}
	//新方块，即正在下降的方块
	if (Create_Flag == 1) {
		Create_Flag = 0;
		sprite_new_block.setTexture(texture_block[New]);//生成新方块精灵
	}
	for (int k = 0; k < 4; k++) {
		sprite_new_block.setPosition(New_Block[k].x * 29, New_Block[k].y * 29);//设置显示位置
		window.draw(sprite_new_block);//绘出新方块
	}
}


//随机生成新方块位置，可非函数计算
void Create_Block()
{	
	//每个小方块(一个大方块由4个小方块组成)的坐标为对应下标的
	//x = blocks[Block_Kinds][对应下标] % 2
	//y = blocks[Block_Kinds][对应下标] / 2
	if (Create_Flag) {
		srand(time(0));//用当前时间值初始化随机种子
		Block_Kinds = rand() % 7 + 1;//获得方块随机种类
		New = rand() % 7 + 1;//方块的图标随机种类

		for (int i = 0; i < 4; i++) {
			New_Block[i].x = Blocks[Block_Kinds - 1][i] % 2;
			New_Block[i].y = Blocks[Block_Kinds - 1][i] / 2;
			if (Game_Space[New_Block[i].x][New_Block[i].y] != 0) {//game over
				while (1);
			}
		}
	}
}

//更新数据
void Updata(float* Down_Speed)
{
	/*正常下降*/
	if (*Down_Speed == 0) {
		for (int i = 0; i < 4; i++) {
			if (Game_Space[New_Block[i].x][New_Block[i].y + 1] != 0) {//Y方向下个方块时候不为空
				Create_Flag = 1;
				for (int k = 0; k < 4; k++) {
					Game_Space[New_Block[k].x][New_Block[k].y] = New;//纪录这个方块的种类，用Game_Space[][]值纪录
					//Old_Space[New_Block[k].x * New_Block[k].y] = New;
				}
				
				return;//方块中最后一个小方块的下一个位置不为空直接退出
			}
			if (New_Block[i].y >= GAMESPACE_Y - 1) {//游戏边界
				Create_Flag = 1;
				for (int k = 0; k < 4; k++) {
					Game_Space[New_Block[k].x][New_Block[k].y] = New;//纪录这个方块的种类，用Game_Space[][]值纪录
					//Old_Space[New_Block[k].x * New_Block[k].y] = New;
				}
				return;//如果大方块中最后一个小方块到达游戏边界,直接退出
			}
		}
		for (int i = 0; i < 4; i++) {//方块位置Y方向加一
			New_Block[i].y++;
		}
	}
	/*下移*/
	if (KY != 0) {
		if (New_Block[3].y >= GAMESPACE_Y - 1) {//游戏边界
			Create_Flag = 1;
			for (int k = 0; k < 4; k++) {
				Game_Space[New_Block[k].x][New_Block[k].y] = New;//纪录这个方块的种类，用Game_Space[][]值纪录
				//Old_Space[New_Block[k].x * New_Block[k].y] = New;
			}
			return;//如果大方块中最后一个小方块到达游戏边界,直接退出
		}
		for (int i = 0; i < 4; i++) {
			if (Game_Space[New_Block[i].x][New_Block[i].y + 1] != 0) {//Y方向下大方块中每个小方块下一个空不为空
				Create_Flag = 1;
				for (int k = 0; k < 4; k++) {
					Game_Space[New_Block[k].x][New_Block[k].y] = New;//纪录这个方块的种类，用Game_Space[][]值纪录
					//Old_Space[New_Block[k].x * New_Block[k].y] = New;
				}
				return;//方块中最后一个小方块的下一个位置不为空直接退出
			}
		}
		for (int i = 0; i < 4; i++) {//方块位置Y方向加一
			New_Block[i].y += KY;
		}
		KY = 0;
	}
	/*左右移*/
	if (KL != 0) {
		for (int i = 0; i < 4; i++) {
			if (New_Block[i].x <= 0 && KL == -1) {//游戏边界
				return;
			}
			if (New_Block[i].x >= GAMESPACE_X - 1 && KL == 1) {//游戏边界
				return;
			}
			if (KL == 1 && Game_Space[New_Block[i].x + 1][New_Block[i].y] != 0) {//X方向下个方块时候为空
				return;//大方块中每一个小方块的下一个位置不为空直接退出
			}
			if (KL == -1 && Game_Space[New_Block[i].x - 1][New_Block[i].y] != 0) {//X方向下个方块时候为空
				return;//大方块中每一个小方块的下一个位置不为空直接退出
			}
		}
		for (int j = 0; j < 4; j++) {
			New_Block[j].x += KL;
		}
		KL = 0;
	}
	/*旋转*/
	if (space != 0) {
		if (Block_Kinds - 1 == 6)//方块不需要旋转
			return;
		struct position Backups[4];
		for (int i = 0; i < 4; i++) {//备份，先变换，变换后检查是否超出边界或者重叠，若超出边界或者重叠则恢复备份
			Backups[i] = New_Block[i];
		}
		/*旋转计算公式
		* 以每个大方块的第二个小方块为旋转中心 = Backups[1]
		*旋转后方块X坐标 = Backups[1].x - 旋转前小方块.y + Backups[1].y
		*旋转后方块Y坐标 = 旋转前小方块.x - Backups[1].x + Backups[1].y
		*/
		for (int i = 0; i < 4; i++) {
			int temp = New_Block[i].x;
			New_Block[i].x = Backups[1].x - New_Block[i].y + Backups[1].y;
			New_Block[i].y = temp - Backups[1].x + Backups[1].y;
		}
		for (int i = 0; i < 4; i++) {
			if(New_Block[i].x > GAMESPACE_X - 1 || New_Block[i].x < 0)
				for (int i = 0; i < 4; i++) {//若超出边界则恢复备份
					New_Block[i] = Backups[i];
				}
			if (New_Block[i].y > GAMESPACE_Y - 1)
				for (int i = 0; i < 4; i++) {//若超出边界则恢复备份
					New_Block[i] = Backups[i];
				}
			if (Game_Space[New_Block[i].x][New_Block[i].y] != 0) {
				for (int i = 0; i < 4; i++) {//若重叠则恢复备份
					New_Block[i] = Backups[i];
				}
			}
		}
		space = 0;
	}
	/*清行*/
	int temper = GAMESPACE_Y - 1;
	for (int i = GAMESPACE_Y - 1; i > 0; i--) {
		int count = 0;
		for (int j = 0; j < GAMESPACE_X; j++) {
			if (Game_Space[j][i] != 0) {
				count++;//统计方块
			}
			Game_Space[j][temper] = Game_Space[j][i];//换行统计
		}
		if (count < GAMESPACE_X)
			temper--;
		else {//加分，放音乐
			getbgm.stop();
			getbgm.play();
		}
	}
}





