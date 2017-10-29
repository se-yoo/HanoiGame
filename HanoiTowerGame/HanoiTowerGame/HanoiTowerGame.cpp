#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<Windows.h>
#include<conio.h>
#include<fstream>
#include<time.h>

using namespace std;

#define Width 100 // ����ũ��
#define Height 30 // ����ũ��

struct Hanoi{
	int stick; //�� ��° ���뿡 �ִ���
	int floor; //�� ���� �ִ���
	int color;
	int up;

	Hanoi(){
		stick = 1;
		up = 0;
	}

	void drawfloor();
	void blockdown();
	void blockup();
};

struct Player{
	int rank;
	int score;
	char name[20];
};

Hanoi *hanoi=0;
Player player[5];
int floor_cnt = 0; // �ϳ��� Ÿ���� ��, ������ ����ϸ� �� ���� �ܰ踦 �غ� �� ����
int cursor; // ���� Ŀ�� ��ġ (���°)
int cnt; // �ϳ��� ����ü �迭�� ����
int move_cnt = 0; //������ �� (������ ���)
int n=-1; //���� ���õ� ���� �ε��� ( -1�� ��� ���õ� �� ����)
int coin = 0;
bool e=true; // �������� �ƴ���
int select_floor=0; //���� �÷����ϴ� ��
char block[7][15] = { 0, };
int random = 0;//���� ����
ifstream designfile;
char filename[5][3][8] = { { "3_1.txt", "3_2.txt", "3_3.txt" }, { "4_1.txt", "4_2.txt", "4_3.txt" }, { "5_1.txt", "5_2.txt", "5_3.txt" }, { "6_1.txt", "6_2.txt", "6_3.txt" }, { "7_1.txt", "7_2.txt", "7_3.txt" } };
bool hint = true;

void gotoxy(int x, int y)// Ŀ���� ��ġ �̵��ϴ� �Լ�
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
void setcolor(int color, int bgcolor)
{
	color &= 0xf;
	bgcolor &= 0xf;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (bgcolor << 4) | color);
}
void Cursor(int n)// Ŀ���� �Ⱥ��̰��ϴ� �Լ�
{
	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = n;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}
BOOL IsKeyDown(int Key)
{
	return ((GetAsyncKeyState(Key) & 0x8000) != 0);
}

void PrintScreen(); // ����ȭ��
void RankScreen(); // ��ũȭ��
void saveInfo(); //���Ͽ� ���� ����(����, ��)
void getReadyInfo(); // ���ΰ� ��
void HanoiDraw(); // �ϳ��� Ÿ���� �׸��� �Լ�
void Selectfloor(); // �� ����
void StartGame(); // �ϳ��� Ÿ�� ����
void Store(); // ����(�� ����)
void setStartHanoi(); // �Ӽ� ����
void Up(); //���� �ø��� �Լ�
void Down(); //���� ������ �Լ�
boolean end(); //���� ��������
void ExplainScreen(); // ���Ӽ��� ȭ��
void scoreCompare(); // ������
void getRank(); // ���Ͽ��� ��ũ ������ ��������
void saveRank(); //��ũ ���Ͽ� ����
void rankInput(int); //��ŷ���� ���� �Է�
void HintScreen(); //��Ʈ ȭ��

void Hanoi::drawfloor(){
	if (designfile.is_open()){
		for (int i = 0; i < floor; i++){
			designfile >> block[i];
		}
	}
	for (int i = 0; i < 15; i++){
		if (block[floor - 1][i] < 'A'){
			color = (int)(block[floor - 1][i] - '0');
		}
		else{
			color = (int)(block[floor - 1][i] - 'A') + 10;
		}
		setcolor(color, color);
		cout << "��";
	}
	designfile.close();
}

void Hanoi::blockdown(){
	up = 0;
	move_cnt++;
	stick = cursor + 1;
	n = -1;
}

void Hanoi::blockup(){
	up = 1;
}

int main(){
	while (e){
		system("cls");
		system("Color 6F");
		system("mode con:cols=100 lines=30"); // �ܼ� âũ��
		Cursor(0);
		PrintScreen();
	}

	delete[] hanoi;
	delete[] player;
	return 0;
}

void PrintScreen(){ //ù ���� ȭ��
	system("cls");
	gotoxy(30, 7);
	cout << "����������������������������������������";
	gotoxy(30, 8);
	cout << "��";
	gotoxy(67, 8);
	cout << "��";
	gotoxy(30, 9);
	cout << "��        REMEMBER HANOI TOWER        ��";
	gotoxy(30, 10);
	cout << "��";
	gotoxy(67, 10);
	cout << "��";
	gotoxy(30, 11);
	cout << "����������������������������������������";
	gotoxy(43, 13);
	cout << "1. �� �� �� ��";
	gotoxy(43, 15);
	cout << "2. �� �� �� ��";
	gotoxy(43, 17);
	cout << "3. �� ŷ �� ��";
	gotoxy(43, 19);
	cout << "4. �� �� �� ��";
	gotoxy(40, 22);
	cout << "[ ���ڸ� �Է��ϼ��� ]";
	gotoxy(88, 29);
	cout << "2512 ������";
	gotoxy(35, 28);

	char choose = _getch();

	switch (choose){
	case '1': Selectfloor();
		break;
	case '2': ExplainScreen();
		break;
	case '3': RankScreen();
		break;
	case '4': 
		e = false;
		return;
		break;
	default: PrintScreen();
	}
}

void RankScreen(){
	getRank();
	system("cls");
	system("Color F6");
	gotoxy(31, 7);
	cout << "��������������������������������������";
	gotoxy(31, 8);
	cout << "��               RANK               ��";
	gotoxy(31, 9);
	cout << "��������������������������������������";
	for (int i = 0; i < 5; i++){
		gotoxy(38, 11+i*2);
		printf("%4d %10s %5d\n",player[i].rank, player[i].name, player[i].score);
	}
	gotoxy(88, 29);
	cout << "2512 ������";

	char choose = _getch();
}

void saveInfo(){
	ofstream ofile;

	ofile.open("readyForGame.txt");

	ofile << floor_cnt << endl << coin;

	ofile.close();
}

void getReadyInfo(){ // ��, ����
	ifstream ifile;

	ifile.open("readyForGame.txt");

	if (ifile.is_open()){
		ifile >> floor_cnt;
		ifile >> coin;
	}

	ifile.close();
}

void getRank(){
	ifstream ifile;

	ifile.open("score.txt");

	if (ifile.is_open()){
		for (int i = 0; i < 5; i++){
			ifile >> player[i].rank;
			ifile >> player[i].name;
			ifile >> player[i].score;
		}
	}
}

void saveRank(){
	ofstream ofile("score.txt");

	for (int i = 0; i < 5; i++){
		ofile << i + 1 << "\t" << player[i].name << "\t" << player[i].score << endl;
	}

	ofile.close();
}

void Store(){
	char yn;
	char sentence[50] = { 0 };

	system("cls");
	system("Color 8F");
	gotoxy(31, 7);
	cout << "��������������������������������������";
	gotoxy(31, 8);
	cout << "��               STORE              ��";
	gotoxy(31, 9);
	cout << "��������������������������������������";
	if (floor_cnt != 7){
		gotoxy(43, 13);
		cout << "���� ���� : "<<coin;
		gotoxy(40, 15);
		setcolor(13, 8);
		cout << floor_cnt + 1 << "�� : " << (floor_cnt + 1) * 50 << " coin �ʿ�";
		setcolor(15, 8);
		gotoxy(28, 17);
		cout << "�����Ͻðڽ��ϱ�? (Y/N) / �ڷΰ���� ESC >> ";
		gotoxy(88, 29);
		cout << "2512 ������";
		yn=_getche();
		switch (yn){
		case 'Y':
		case 'y':
			if (coin >= (floor_cnt + 1) * 50){
				coin -= (floor_cnt + 1) * 50;
				floor_cnt += 1;
				saveInfo();
				strcpy(sentence, "�� �� �� ��");
			}
			else{
				strcpy(sentence, "�� �� �� ��");
			}
			system("cls");
			gotoxy(40, 14);
			cout << sentence;
			gotoxy(88, 29);
			cout << "2512 ������";
			Sleep(1000);
			return;
			break;
		case 'N':
		case 'n':
			break;
		case 27: 
			return;
			break;
		default: Store();
		}
	}
	else{
		gotoxy(40, 12);
		cout << "7������ ��� �����ؼ�";
		gotoxy(37, 13);
		cout << "�� �̻� ������ �� �����ϴ�";
		Sleep(1000);
	}
	gotoxy(35, 28);
}

void HanoiDraw(){
	int x = 3;
	int y1 = 20, y2 = 20, y3 = 20;
	gotoxy(2,1);
	cout << move_cnt << "��";
	if (floor_cnt == 7 && select_floor==7){
		gotoxy(83, 1);
		cout << "�ְ����� : " << player[0].score;
	}
	if (hint){
		gotoxy(2, 28);
		cout << "��Ʈ(Enter)";
	}
	gotoxy(88, 29);
	cout << "2512 ������";

	switch (cursor){
	case 0:
		for (int i = 0; i < cnt; i++){
			if (hanoi[i].up == 1){
				gotoxy(3, 9);
				hanoi[i].drawfloor();
				cout << "                                                                         ";
				break;
			}
			else if (i == cnt - 1){
				gotoxy(3, 9);
				cout << "              ��                                                                             ";
			}
		}
		break;
	case 1:
		for (int i = 0; i < cnt; i++){
			if (hanoi[i].up == 1){
				gotoxy(3, 9);
				cout << "                             ";
				gotoxy(35, 9);
				hanoi[i].drawfloor();
				cout << "                             ";
				break;
			}
			else if (i == cnt - 1){
				gotoxy(3, 9);
				cout << "                                              ��                                           ";
			}
		}
		break;
	case 2:
		for (int i = 0; i < cnt; i++){
			if (hanoi[i].up == 1){
				gotoxy(3, 9);
				cout << "                                                                         ";
				gotoxy(67, 9);
				hanoi[i].drawfloor();
				break;
			}
			else if (i == cnt - 1){
				gotoxy(3, 9);
				cout << "                                                                              ��           ";
			}
		}
		break;
	}

	setcolor(15, 6);
	for (int i = 20-select_floor; i >=11; i--){
		gotoxy(3, i);
		cout << "              ||              ";
		gotoxy(35, i);
		cout << "              ||              ";
		gotoxy(67, i);
		cout << "              ||              ";
	}

	for (int i = cnt-1; i >=0; i--){
		if (hanoi[i].stick == 1&&hanoi[i].up==0){
			gotoxy(x,y1--);
			hanoi[i].drawfloor();
		}
		else if (hanoi[i].stick == 2 && hanoi[i].up == 0){
			gotoxy(x+32, y2--);
			hanoi[i].drawfloor();
		}
		else if(hanoi[i].stick == 3 && hanoi[i].up == 0){
			gotoxy(x+64, y3--);
			hanoi[i].drawfloor();
		}
	}
	setcolor(15, 6);

	if (y1 > 20 - select_floor){
		for (int i = y1; i >= 20 - select_floor; i--){
			gotoxy(3, i);
			cout << "              ||              ";
		}
	}
	if (y2 > 20 - select_floor){
		for (int i = y2; i >= 20 - select_floor; i--){
			gotoxy(35, i);
			cout << "              ||              ";
		}
	}
	if (y3 > 20 - select_floor){
		for (int i = y3; i >= 20 - select_floor; i--){
			gotoxy(67, i);
			cout << "              ||              ";
		}
	}
	gotoxy(0, 21);
	cout << "����������������������������������������������������������������������������������������������������";
}

void ExplainScreen(){
	char b;
	system("cls");
	system("Color 0F");
	gotoxy(0, 1);
	cout << "����������������������������������������������������������������������������������������������������";

	gotoxy(31, 5);
	cout << "��������������������������������������";
	gotoxy(31, 6);
	cout << "��            �� �� �� ��           ��";
	gotoxy(31, 7);
	cout << "��������������������������������������";

	gotoxy(7, 11);
	cout << "��� �ϳ��� Ÿ���� ������ ����� �ƴ� �ٸ� ��տ� ��� ������ �ű�� �����ϴ� �����Դϴ�";
	gotoxy(3, 13);
	cout << "��, �ѹ��� �Ѱ��� ���Ǹ� �ű� �� �ְ�, �ݵ�� ��� ������ ó�� ���� ���� �� ���� �ִ� ���ǿ���";
	gotoxy(3, 15);
	cout<<"�ڽź��� �Ʒ� �ִ� ���Ǻ��� ���� ������ �մϴ�. ��, ó�� ���ǵ��� ����� �� �� ��� �״�� �ٸ� ";
	gotoxy(10, 17);
	cout << "��տ� �ű�� �˴ϴ�. ������ �ϳ��� Ÿ���� �޸� ������ �����ؾ� �ϴ� �����Դϴ�.";
	gotoxy(37, 23);
	cout << "�ƹ�Ű�� ������ ��������..";
	gotoxy(13, 26);
	gotoxy(0, 28);
	cout << "����������������������������������������������������������������������������������������������������";
	gotoxy(88, 29);
	cout << "2512 ������";
	b = _getch();
	system("cls");
	gotoxy(0, 1);
	cout << "����������������������������������������������������������������������������������������������������";
	gotoxy(1, 4);
	cout << "1. ��� ������ ó�� ���� ���� �� ���� �ִ� �ڸ����� �ڽź��� �Ʒ� �ִ� ���Ǻ��� ���� ������ ";
	gotoxy(1, 5);
	cout << "�մϴ�. �ڽź��� ���� �־��� ���� ���� �ø� �� �����ϴ�. ���� ��� �Ʒ��� ���� Ÿ���� ������ ���";
	gotoxy(15, 7);
	cout << "������";
	gotoxy(15,8);
	cout << "������";
	gotoxy(15, 9);
	cout << "������";
	gotoxy(15, 10);
	cout << "������";
	gotoxy(30, 8);
	cout << "�� ������ ���� ���� ������ ������ ";
	gotoxy(30, 9);
	cout << "�ø� �� �����ϴ�(�����ຸ�� �Ʒ� �־��� ����)";
	gotoxy(1, 12);
	cout << "2. ����Ű�� �̿��� ������ �����մϴ�.";
	gotoxy(2, 14);
	cout << "�� �� : Ŀ�� �̵� , �� : ���� �ø��� , �� : ���� ������, Enter : ��Ʈ(1��), ESC : ����(���� �ȵ�) ";
	gotoxy(1, 16);
	cout << "3. 3������ �����ϸ� ������ ��� �� ���� ���� �ϳ��� Ÿ���� �������� ������ �̿��Ͻ� �� �ֽ��ϴ�.";
	gotoxy(1, 18);
	cout << "4. 7�� �ϳ��̸� �÷����ؼ� ���� ������ ������ ��ŷ�� �̸��� �ø� �� �ֽ��ϴ�! 1� �����غ�����.";
	gotoxy(1, 19);
	cout << "7���� �����ϸ� 1���� �÷��� �� �� �ֽ��ϴ�. (�Ź� �÷��̸��� �������� �����ϼž� �մϴ�.)";
	gotoxy(1, 21);
	cout << "5. ��Ʈ�� 1��, 3�ʵ��� ���� ���� ����� �����ݴϴ� �Ʒ��� �����Դϴ�. (���� ���ڴ� ������ ��Ÿ��)";
	gotoxy(15, 23);
	cout << "1 ������";
	gotoxy(15, 24);
	cout << "2 ������";
	gotoxy(15, 25);
	cout << "3 ������";
	gotoxy(15, 26);
	cout << "4 ������";
	gotoxy(40, 25);
	cout << "�ƹ�Ű�� ������ ��������..";
	gotoxy(0, 28);
	cout << "����������������������������������������������������������������������������������������������������";
	gotoxy(88, 29);
	cout << "2512 ������";
	b = _getch();
}

void Selectfloor(){
	char choose;
	getReadyInfo();
	system("cls");

	gotoxy(0, 1);
	cout << " �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� ";

	gotoxy(30, 7);
	cout << "�÷����� �ϳ��� Ÿ���� ���� �������ּ���";
	switch (floor_cnt){
	case 7: gotoxy(44, 21);
		cout << "6. 7��";
	case 6: gotoxy(44, 19);
		cout << "5. 6��";
	case 5: gotoxy(44, 17);
		cout << "4. 5��";
	case 4: gotoxy(44, 15);
		cout << "3. 4��"; 
	case 3: gotoxy(44, 13);
		cout << "2. 3��";
		gotoxy(36, 11);
		cout << "1. ����(�� ���� �� ����)";
		break;
	}
	gotoxy(0, 28);
	cout << " �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� ";
	gotoxy(88, 29);
	cout << "2512 ������";
	choose = _getch();
	if (choose == '1'){
		Store();
	}
	else if (choose > '1' && choose < (floor_cnt+'0')){
		cnt=(int)(choose-'0')+1;
		hanoi=new Hanoi[cnt];
		select_floor = cnt;
		setStartHanoi();
		srand((unsigned int)time(NULL));
		random = rand() % 3;
		designfile.open(filename[select_floor - 3][random]);
		system("cls");
		HanoiDraw();
		StartGame();
		if (floor_cnt == 7 && select_floor==7){
			floor_cnt--;
		}
		saveInfo();
	}
	else{
		Selectfloor();
	}
}

void setStartHanoi(){
	getRank();
	for (int i = 0; i < cnt; i++){
		hanoi[i].floor = i+1;
	}
	cursor = 0;
	move_cnt = 0;
	n = -1;
	hint = true;
}

void StartGame(){
	char key;
	while (end()){
		key = _getch();
		switch (key) {
		case 75:
			if (cursor > 0) cursor -= 1;
			break;
		case 77:
			if (cursor < 2) cursor += 1;
			break;
		case 72:
			Up();
			break;
		case 80:
			Down();
			break;
		case 27:
			system("cls");
			gotoxy(30, 14);
			cout << "�����߽��ϴ� ������� �ʰ� �������� ���ư��ϴ�";
			Sleep(1500);
			setStartHanoi();
			if (floor_cnt == 7 && select_floor == 7){
				floor_cnt--;
			}
			return;
			break;
		case 13:
			if (hint) HintScreen();
			break;
		}
		HanoiDraw();
	}
	int coin2 = select_floor*30;
	if (move_cnt <= pow(2,select_floor)*5/4){ //�̵����� �ּ� �̵����� 5/4 ���ϰ� �Ǹ� �߰� ����
		coin2 += (int)((pow(2, select_floor)*5/4 - move_cnt) * 10);
	}
	coin += coin2;
	Sleep(500);
	system("cls");
	gotoxy(47,12);
	cout << move_cnt << "�� �̵�";
	gotoxy(38,14);
	cout <<"ȹ������ : "<<coin2<<" �����߽��ϴ�";
	Sleep(1000);
	if(select_floor==7)
		scoreCompare();

	setStartHanoi(); // ������ ������ �ٽ� �ʱ�ȭ
	return;
}

void Up(){
	for (int i = 0; i < cnt; i++){
		if (hanoi[i].stick == cursor + 1 && n == -1){
			hanoi[i].blockup();
			n = i;
			return;
		}
	}
}

void Down(){
	int j=0;
	for (int i = 0; i < cnt; i++){
		if ((hanoi[i].stick == cursor + 1) && (hanoi[n].up == 1)){
			if (hanoi[n].floor <= hanoi[i].floor){
				hanoi[n].blockdown();
				return;
			}
			else return;
		}
		if (hanoi[i].stick == cursor + 1 && hanoi[i].up == 0) j++;
	}

	if (j == 0){ // �� ��뿡 �ƹ� ���� ������
		hanoi[n].blockdown();
		return;
	}
}

boolean end(){
	int s;
	if (hanoi[0].stick != 1) s = hanoi[0].stick;
	else return true;

	for (int i = 1; i < cnt; i++){
		if (hanoi[i].stick != s) return true;
	}

	return false;
}

void scoreCompare(){
	getRank();
	for (int i = 0; i < 5; i++){
		if (player[i].score>move_cnt){
			rankInput(i);
			saveRank();

			system("cls");
			gotoxy(40, 14);
			cout<<"�� ŷ �� �� �� ��";
			Sleep(1000);
			break;
		}
	}
}

void rankInput(int a){
	char n[20];

	system("cls");
	gotoxy(20, 14);
	cout << move_cnt<<"���� "<< a+1 <<"���Դϴ� �̸��� �Է����ּ��� >> ";
	gets_s(n);

	for (int i = 4; i >= a; i--){
		strcpy(player[i].name , player[i - 1].name);
		player[i].score = player[i - 1].score;
	}

	strcpy(player[a].name, n);
	player[a].score = move_cnt;
}

void HintScreen(){
	int a= 11;

	system("cls");
	gotoxy(47, 7);
	cout << "�� Ʈ";
	for (int i = 0; i < select_floor; i++){
		gotoxy(15, a + i);
		hanoi[i].drawfloor(); 
		gotoxy(45, a + (i*2));
		setcolor(15, 6);
		cout << i + 1 << endl;
		gotoxy(47, a + (i*2));
		hanoi[i].drawfloor();
	}
	gotoxy(88, 29);
	setcolor(15, 6);
	cout << "2512 ������";
	Sleep(3000);

	hint = false;

	system("cls");
}