/*
�����ˣ��
����ʱ�䣺2020/12/26
��Ŀ���ƣ�����˹����
ͼ�ο⣺SFMLͼ�ο�
�޸���־��



��Ŀ���ȣ�
--->>һ�����
--->>С���⣬����

*/
#include <SFML/Graphics.hpp>	//SFMLͼ���
#include <SFML/Audio.hpp>		//����ͼ�ο�
#include <time.h>
#include <iostream>
#include <Windows.h>
using namespace std;
using namespace sf;
//#pragma comment(lib,"sfml-audio-d.lib")
//#pragma comment(lib,"sfml-graphics-d.lib")
//#pragma comment(lib,"sfml-window-d.lib")
//#pragma comment(lib,"sfml-system-d.lib")

#define SIZE		29		//�����زĴ�С29*29
#define GAMESPACE_X 11		//��Ϸ�ռ��С11*17
#define GAMESPACE_Y 17
#define DOWN_SPEED	1200		//�����½��ٶ�

int Game_Space[GAMESPACE_X][GAMESPACE_Y] = { 0 };//��Ϸ�ռ��С����30��10��0�հף���0�з���
												//ֵΪn����ʾ�ǵ�n�з��飬��Ϊ��n����ɫ
//int Old_Space[GAMESPACE_X * GAMESPACE_Y] = { 0 };//��¼���·����ͼ��� 1-7

int KY = 0;//�ƶ��ٶ�
int KL = 0;
int space = 0;

int Block_Kinds = 0;//Ϊ���ظ�����ʾ��ǰ��������
int New = 0;//�·���ʹ�õ�����

const int Blocks[7][4] = {//7�ַ���
	{1,3,5,7},//I����
	{2,4,5,7},//��Z����
	{3,4,5,6},//��Z����
	{3,4,5,7},//T����
	{2,3,5,7},//��L��
	{3,5,6,7},//��L��
	{2,3,4,5}//����
};
struct position {//ÿ�����鶼��4��С������ɣ�Pos_Block[4]��ʾ4��С��������λ�ã����ڱ任λ�ü�����
	int x, y;
};
struct position New_Block[4];//�����ɷ����ÿ��С����λ��
int Create_Flag = 1;//�����·����־��ͬһʱ��ֻ��һ�������½��ķ���
sf::Texture texture_block[8];//���ط�������
sf::Sprite sprite_bg;//������������

sf::RenderWindow window;//����������

void Updata(float *Down_Speed);
void Get_Key();
void Create_Block();//�����·���
void Draw_Blocks();//���Ʒ���

sf::Sprite sprite_old_block;//�����ɷ��龫�飬��ֹ��ص�
sf::Sprite sprite_new_block;//�����·��龫�飬�����½���
sf::Music getbgm;


int main(void)
{
	ShowWindow(GetForegroundWindow(), 0);//���غڴ���
	int Display_Count = 0;//��ʾ������
	float Down_Count = 1;//�½�������
	int Sys_Count = 0;//ϵͳ������
	/*������Ϸ��Դ������������*/
	window.create(sf::VideoMode(320, 520), "MiaoA");//��ʼ�������ڣ���Ƶģʽ����С320*520������MiaoA
	sf::Texture texture_bg;//���ر�������
	if (!texture_bg.loadFromFile("bg.png")) {//���ر���ͼƬ����
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

	sprite_bg.setTexture(texture_bg);//���ɱ�������
	window.draw(sprite_bg);//��ʾ���鱳��ͼƬ
	window.display();//��ʾ��������
	/*������Ϸ��Դ������������*/
	sf::Music bgm;
	if (!getbgm.openFromFile("get.wav")) {
		return 0;
		cout << "get.wav load flase" << endl;
	}
	if (!bgm.openFromFile("bgm.wav")) {
		return 0;
		cout << "bgm.wav load flase" << endl;
	}
	bgm.setLoop(true);//ѭ������
	bgm.setVolume(50.f);
	bgm.play();

	while (window.isOpen()){//��Ϸ��ѭ��

		if (Sys_Count > 60000)
			Sys_Count = 0;
		Sys_Count++;
		Down_Count = Sys_Count % DOWN_SPEED;

		Get_Key();//��������
		Updata(&Down_Count);//���ݸ���
		Create_Block();//�����·���
		Draw_Blocks();//������з���
		window.display();//ˢ����ʾ
	}
	//system("pause");
	return 0;
}


void Get_Key() {//��������

	Event key;//�����¼�������ÿ��ֻ����һ������
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
				case Keyboard::Q:	//��Q���˳�
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
void Draw_Blocks()//���Ʒ���
{
	window.draw(sprite_bg);//���»��Ʊ���
	sprite_new_block.move(10, 10);//����ƫ����

	//��ֹ�����ķ��飬�Ѿ���ص�
	for (int i = 0; i < GAMESPACE_X; i++) {
		for (int j = 0; j < GAMESPACE_Y; j++) {
			if (Game_Space[i][j] != 0) {//Ϊ1���С����
				sprite_old_block.setTexture(texture_block[ Game_Space[i][j] ]);//���ɶ�Ӧ���龫��
				sprite_old_block.setPosition(i * SIZE, j * SIZE);//������ʾλ��һ�������С29
				window.draw(sprite_old_block);//�������
			}
		}
	}
	//�·��飬�������½��ķ���
	if (Create_Flag == 1) {
		Create_Flag = 0;
		sprite_new_block.setTexture(texture_block[New]);//�����·��龫��
	}
	for (int k = 0; k < 4; k++) {
		sprite_new_block.setPosition(New_Block[k].x * 29, New_Block[k].y * 29);//������ʾλ��
		window.draw(sprite_new_block);//����·���
	}
}


//��������·���λ�ã��ɷǺ�������
void Create_Block()
{	
	//ÿ��С����(һ���󷽿���4��С�������)������Ϊ��Ӧ�±��
	//x = blocks[Block_Kinds][��Ӧ�±�] % 2
	//y = blocks[Block_Kinds][��Ӧ�±�] / 2
	if (Create_Flag) {
		srand(time(0));//�õ�ǰʱ��ֵ��ʼ���������
		Block_Kinds = rand() % 7 + 1;//��÷����������
		New = rand() % 7 + 1;//�����ͼ���������

		for (int i = 0; i < 4; i++) {
			New_Block[i].x = Blocks[Block_Kinds - 1][i] % 2;
			New_Block[i].y = Blocks[Block_Kinds - 1][i] / 2;
			if (Game_Space[New_Block[i].x][New_Block[i].y] != 0) {//game over
				while (1);
			}
		}
	}
}

//��������
void Updata(float* Down_Speed)
{
	/*�����½�*/
	if (*Down_Speed == 0) {
		for (int i = 0; i < 4; i++) {
			if (Game_Space[New_Block[i].x][New_Block[i].y + 1] != 0) {//Y�����¸�����ʱ��Ϊ��
				Create_Flag = 1;
				for (int k = 0; k < 4; k++) {
					Game_Space[New_Block[k].x][New_Block[k].y] = New;//��¼�����������࣬��Game_Space[][]ֵ��¼
					//Old_Space[New_Block[k].x * New_Block[k].y] = New;
				}
				
				return;//���������һ��С�������һ��λ�ò�Ϊ��ֱ���˳�
			}
			if (New_Block[i].y >= GAMESPACE_Y - 1) {//��Ϸ�߽�
				Create_Flag = 1;
				for (int k = 0; k < 4; k++) {
					Game_Space[New_Block[k].x][New_Block[k].y] = New;//��¼�����������࣬��Game_Space[][]ֵ��¼
					//Old_Space[New_Block[k].x * New_Block[k].y] = New;
				}
				return;//����󷽿������һ��С���鵽����Ϸ�߽�,ֱ���˳�
			}
		}
		for (int i = 0; i < 4; i++) {//����λ��Y�����һ
			New_Block[i].y++;
		}
	}
	/*����*/
	if (KY != 0) {
		if (New_Block[3].y >= GAMESPACE_Y - 1) {//��Ϸ�߽�
			Create_Flag = 1;
			for (int k = 0; k < 4; k++) {
				Game_Space[New_Block[k].x][New_Block[k].y] = New;//��¼�����������࣬��Game_Space[][]ֵ��¼
				//Old_Space[New_Block[k].x * New_Block[k].y] = New;
			}
			return;//����󷽿������һ��С���鵽����Ϸ�߽�,ֱ���˳�
		}
		for (int i = 0; i < 4; i++) {
			if (Game_Space[New_Block[i].x][New_Block[i].y + 1] != 0) {//Y�����´󷽿���ÿ��С������һ���ղ�Ϊ��
				Create_Flag = 1;
				for (int k = 0; k < 4; k++) {
					Game_Space[New_Block[k].x][New_Block[k].y] = New;//��¼�����������࣬��Game_Space[][]ֵ��¼
					//Old_Space[New_Block[k].x * New_Block[k].y] = New;
				}
				return;//���������һ��С�������һ��λ�ò�Ϊ��ֱ���˳�
			}
		}
		for (int i = 0; i < 4; i++) {//����λ��Y�����һ
			New_Block[i].y += KY;
		}
		KY = 0;
	}
	/*������*/
	if (KL != 0) {
		for (int i = 0; i < 4; i++) {
			if (New_Block[i].x <= 0 && KL == -1) {//��Ϸ�߽�
				return;
			}
			if (New_Block[i].x >= GAMESPACE_X - 1 && KL == 1) {//��Ϸ�߽�
				return;
			}
			if (KL == 1 && Game_Space[New_Block[i].x + 1][New_Block[i].y] != 0) {//X�����¸�����ʱ��Ϊ��
				return;//�󷽿���ÿһ��С�������һ��λ�ò�Ϊ��ֱ���˳�
			}
			if (KL == -1 && Game_Space[New_Block[i].x - 1][New_Block[i].y] != 0) {//X�����¸�����ʱ��Ϊ��
				return;//�󷽿���ÿһ��С�������һ��λ�ò�Ϊ��ֱ���˳�
			}
		}
		for (int j = 0; j < 4; j++) {
			New_Block[j].x += KL;
		}
		KL = 0;
	}
	/*��ת*/
	if (space != 0) {
		if (Block_Kinds - 1 == 6)//���鲻��Ҫ��ת
			return;
		struct position Backups[4];
		for (int i = 0; i < 4; i++) {//���ݣ��ȱ任���任�����Ƿ񳬳��߽�����ص����������߽�����ص���ָ�����
			Backups[i] = New_Block[i];
		}
		/*��ת���㹫ʽ
		* ��ÿ���󷽿�ĵڶ���С����Ϊ��ת���� = Backups[1]
		*��ת�󷽿�X���� = Backups[1].x - ��תǰС����.y + Backups[1].y
		*��ת�󷽿�Y���� = ��תǰС����.x - Backups[1].x + Backups[1].y
		*/
		for (int i = 0; i < 4; i++) {
			int temp = New_Block[i].x;
			New_Block[i].x = Backups[1].x - New_Block[i].y + Backups[1].y;
			New_Block[i].y = temp - Backups[1].x + Backups[1].y;
		}
		for (int i = 0; i < 4; i++) {
			if(New_Block[i].x > GAMESPACE_X - 1 || New_Block[i].x < 0)
				for (int i = 0; i < 4; i++) {//�������߽���ָ�����
					New_Block[i] = Backups[i];
				}
			if (New_Block[i].y > GAMESPACE_Y - 1)
				for (int i = 0; i < 4; i++) {//�������߽���ָ�����
					New_Block[i] = Backups[i];
				}
			if (Game_Space[New_Block[i].x][New_Block[i].y] != 0) {
				for (int i = 0; i < 4; i++) {//���ص���ָ�����
					New_Block[i] = Backups[i];
				}
			}
		}
		space = 0;
	}
	/*����*/
	int temper = GAMESPACE_Y - 1;
	for (int i = GAMESPACE_Y - 1; i > 0; i--) {
		int count = 0;
		for (int j = 0; j < GAMESPACE_X; j++) {
			if (Game_Space[j][i] != 0) {
				count++;//ͳ�Ʒ���
			}
			Game_Space[j][temper] = Game_Space[j][i];//����ͳ��
		}
		if (count < GAMESPACE_X)
			temper--;
		else {//�ӷ֣�������
			getbgm.stop();
			getbgm.play();
		}
	}
}





