#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<Windows.h>
#include<conio.h>
#include<fstream>
#include<time.h>

using namespace std;

#define Width 100 // 가로크기
#define Height 30 // 세로크기

struct Hanoi{
	int stick; //몇 번째 막대에 있는지
	int floor; //몇 층에 있는지
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
int floor_cnt = 0; // 하노이 타워의 층, 코인을 사용하면 더 높은 단계를 해볼 수 있음
int cursor; // 현재 커서 위치 (몇번째)
int cnt; // 하노이 구조체 배열의 길이
int move_cnt = 0; //움직인 수 (점수로 사용)
int n=-1; //현재 선택된 블럭의 인덱스 ( -1일 경우 선택된 블럭 없음)
int coin = 0;
bool e=true; // 끝나는지 아닌지
int select_floor=0; //현재 플레이하는 층
char block[7][15] = { 0, };
int random = 0;//랜덤 파일
ifstream designfile;
char filename[5][3][8] = { { "3_1.txt", "3_2.txt", "3_3.txt" }, { "4_1.txt", "4_2.txt", "4_3.txt" }, { "5_1.txt", "5_2.txt", "5_3.txt" }, { "6_1.txt", "6_2.txt", "6_3.txt" }, { "7_1.txt", "7_2.txt", "7_3.txt" } };
bool hint = true;

void gotoxy(int x, int y)// 커서의 위치 이동하는 함수
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
void Cursor(int n)// 커서를 안보이게하는 함수
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

void PrintScreen(); // 시작화면
void RankScreen(); // 랭크화면
void saveInfo(); //파일에 정보 넣음(코인, 층)
void getReadyInfo(); // 코인과 층
void HanoiDraw(); // 하노이 타워를 그리는 함수
void Selectfloor(); // 층 선택
void StartGame(); // 하노이 타워 시작
void Store(); // 상점(층 구매)
void setStartHanoi(); // 속성 설정
void Up(); //블럭을 올리는 함수
void Down(); //블럭을 내리는 함수
boolean end(); //게임 성공여부
void ExplainScreen(); // 게임설명 화면
void scoreCompare(); // 점수비교
void getRank(); // 파일에서 랭크 정보들 가져오기
void saveRank(); //랭크 파일에 저장
void rankInput(int); //랭킹관련 정보 입력
void HintScreen(); //힌트 화면

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
		cout << "■";
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
		system("mode con:cols=100 lines=30"); // 콘솔 창크기
		Cursor(0);
		PrintScreen();
	}

	delete[] hanoi;
	delete[] player;
	return 0;
}

void PrintScreen(){ //첫 시작 화면
	system("cls");
	gotoxy(30, 7);
	cout << "┌──────────────────┐";
	gotoxy(30, 8);
	cout << "│";
	gotoxy(67, 8);
	cout << "│";
	gotoxy(30, 9);
	cout << "│        REMEMBER HANOI TOWER        │";
	gotoxy(30, 10);
	cout << "│";
	gotoxy(67, 10);
	cout << "│";
	gotoxy(30, 11);
	cout << "└──────────────────┘";
	gotoxy(43, 13);
	cout << "1. 게 임 시 작";
	gotoxy(43, 15);
	cout << "2. 게 임 설 명";
	gotoxy(43, 17);
	cout << "3. 랭 킹 보 기";
	gotoxy(43, 19);
	cout << "4. 게 임 종 료";
	gotoxy(40, 22);
	cout << "[ 숫자를 입력하세요 ]";
	gotoxy(88, 29);
	cout << "2512 유시은";
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
	cout << "┌─────────────────┐";
	gotoxy(31, 8);
	cout << "│               RANK               │";
	gotoxy(31, 9);
	cout << "└─────────────────┘";
	for (int i = 0; i < 5; i++){
		gotoxy(38, 11+i*2);
		printf("%4d %10s %5d\n",player[i].rank, player[i].name, player[i].score);
	}
	gotoxy(88, 29);
	cout << "2512 유시은";

	char choose = _getch();
}

void saveInfo(){
	ofstream ofile;

	ofile.open("readyForGame.txt");

	ofile << floor_cnt << endl << coin;

	ofile.close();
}

void getReadyInfo(){ // 층, 코인
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
	cout << "┌─────────────────┐";
	gotoxy(31, 8);
	cout << "│               STORE              │";
	gotoxy(31, 9);
	cout << "└─────────────────┘";
	if (floor_cnt != 7){
		gotoxy(43, 13);
		cout << "보유 코인 : "<<coin;
		gotoxy(40, 15);
		setcolor(13, 8);
		cout << floor_cnt + 1 << "층 : " << (floor_cnt + 1) * 50 << " coin 필요";
		setcolor(15, 8);
		gotoxy(28, 17);
		cout << "구매하시겠습니까? (Y/N) / 뒤로가기는 ESC >> ";
		gotoxy(88, 29);
		cout << "2512 유시은";
		yn=_getche();
		switch (yn){
		case 'Y':
		case 'y':
			if (coin >= (floor_cnt + 1) * 50){
				coin -= (floor_cnt + 1) * 50;
				floor_cnt += 1;
				saveInfo();
				strcpy(sentence, "구 매 완 료");
			}
			else{
				strcpy(sentence, "코 인 부 족");
			}
			system("cls");
			gotoxy(40, 14);
			cout << sentence;
			gotoxy(88, 29);
			cout << "2512 유시은";
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
		cout << "7층까지 모두 구매해서";
		gotoxy(37, 13);
		cout << "더 이상 구매할 수 없습니다";
		Sleep(1000);
	}
	gotoxy(35, 28);
}

void HanoiDraw(){
	int x = 3;
	int y1 = 20, y2 = 20, y3 = 20;
	gotoxy(2,1);
	cout << move_cnt << "번";
	if (floor_cnt == 7 && select_floor==7){
		gotoxy(83, 1);
		cout << "최고점수 : " << player[0].score;
	}
	if (hint){
		gotoxy(2, 28);
		cout << "힌트(Enter)";
	}
	gotoxy(88, 29);
	cout << "2512 유시은";

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
				cout << "              ▽                                                                             ";
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
				cout << "                                              ▽                                           ";
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
				cout << "                                                                              ▽           ";
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
	cout << "──────────────────────────────────────────────────";
}

void ExplainScreen(){
	char b;
	system("cls");
	system("Color 0F");
	gotoxy(0, 1);
	cout << "──────────────────────────────────────────────────";

	gotoxy(31, 5);
	cout << "┌─────────────────┐";
	gotoxy(31, 6);
	cout << "│            게 임 설 명           │";
	gotoxy(31, 7);
	cout << "└─────────────────┘";

	gotoxy(7, 11);
	cout << "기억 하노이 타워는 시작한 기둥이 아닌 다른 기둥에 모든 원판을 옮기면 성공하는 게임입니다";
	gotoxy(3, 13);
	cout << "단, 한번에 한개의 원판만 옮길 수 있고, 반드시 모든 원판은 처음 시작 했을 때 놓여 있던 원판에서";
	gotoxy(3, 15);
	cout<<"자신보다 아래 있던 원판보다 위에 놓여야 합니다. 즉, 처음 원판들을 기억한 후 그 모습 그대로 다른 ";
	gotoxy(10, 17);
	cout << "기둥에 옮기면 됩니다. 원래의 하노이 타워와 달리 기억력을 발휘해야 하는 게임입니다.";
	gotoxy(37, 23);
	cout << "아무키나 누르면 다음으로..";
	gotoxy(13, 26);
	gotoxy(0, 28);
	cout << "──────────────────────────────────────────────────";
	gotoxy(88, 29);
	cout << "2512 유시은";
	b = _getch();
	system("cls");
	gotoxy(0, 1);
	cout << "──────────────────────────────────────────────────";
	gotoxy(1, 4);
	cout << "1. 모든 원판은 처음 시작 했을 때 놓여 있던 자리에서 자신보다 아래 있던 원판보다 위에 놓여야 ";
	gotoxy(1, 5);
	cout << "합니다. 자신보다 위에 있었던 원판 위에 올릴 수 없습니다. 예를 들어 아래와 같은 타워가 나왔을 경우";
	gotoxy(15, 7);
	cout << "□□□□□";
	gotoxy(15,8);
	cout << "■□■□■";
	gotoxy(15, 9);
	cout << "■■□■■";
	gotoxy(15, 10);
	cout << "■□□□■";
	gotoxy(30, 8);
	cout << "※ □□□□□ 원판 위에 ■□■□■ 원판을 ";
	gotoxy(30, 9);
	cout << "올릴 수 없습니다(□□□□□보다 아래 있었기 때문)";
	gotoxy(1, 12);
	cout << "2. 방향키를 이용해 게임을 진행합니다.";
	gotoxy(2, 14);
	cout << "← → : 커서 이동 , ↑ : 원판 올리기 , ↓ : 원판 내리기, Enter : 힌트(1번), ESC : 종료(저장 안됨) ";
	gotoxy(1, 16);
	cout << "3. 3층부터 시작하며 코인을 모아 더 높은 층의 하노이 타워를 상점에서 구매해 이용하실 수 있습니다.";
	gotoxy(1, 18);
	cout << "4. 7층 하노이를 플레이해서 높은 점수를 얻으면 랭킹에 이름을 올릴 수 있습니다! 1등에 도전해보세요.";
	gotoxy(1, 19);
	cout << "7층은 구매하면 1번만 플레이 할 수 있습니다. (매번 플레이마다 상점에서 구매하셔야 합니다.)";
	gotoxy(1, 21);
	cout << "5. 힌트는 1번, 3초동안 원래 블럭의 모습을 보여줍니다 아래는 예시입니다. (왼쪽 숫자는 순서를 나타냄)";
	gotoxy(15, 23);
	cout << "1 □□□□□";
	gotoxy(15, 24);
	cout << "2 ■□■□■";
	gotoxy(15, 25);
	cout << "3 ■■□■■";
	gotoxy(15, 26);
	cout << "4 ■□□□■";
	gotoxy(40, 25);
	cout << "아무키나 누르면 메인으로..";
	gotoxy(0, 28);
	cout << "──────────────────────────────────────────────────";
	gotoxy(88, 29);
	cout << "2512 유시은";
	b = _getch();
}

void Selectfloor(){
	char choose;
	getReadyInfo();
	system("cls");

	gotoxy(0, 1);
	cout << " ◇ ◆ ◇ ◆ ◇ ◆ ◇ ◆ ◇ ◆ ◇ ◆ ◇ ◆ ◇ ◆ ◇ ◆ ◇ ◆ ◇ ◆ ◇ ◆ ◇ ◆ ◇ ◆ ◇ ◆ ◇ ◆ ◇ ";

	gotoxy(30, 7);
	cout << "플레이할 하노이 타워의 층을 선택해주세요";
	switch (floor_cnt){
	case 7: gotoxy(44, 21);
		cout << "6. 7층";
	case 6: gotoxy(44, 19);
		cout << "5. 6층";
	case 5: gotoxy(44, 17);
		cout << "4. 5층";
	case 4: gotoxy(44, 15);
		cout << "3. 4층"; 
	case 3: gotoxy(44, 13);
		cout << "2. 3층";
		gotoxy(36, 11);
		cout << "1. 상점(더 높은 층 구매)";
		break;
	}
	gotoxy(0, 28);
	cout << " ◇ ◆ ◇ ◆ ◇ ◆ ◇ ◆ ◇ ◆ ◇ ◆ ◇ ◆ ◇ ◆ ◇ ◆ ◇ ◆ ◇ ◆ ◇ ◆ ◇ ◆ ◇ ◆ ◇ ◆ ◇ ◆ ◇ ";
	gotoxy(88, 29);
	cout << "2512 유시은";
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
			cout << "실패했습니다 저장되지 않고 메인으로 돌아갑니다";
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
	if (move_cnt <= pow(2,select_floor)*5/4){ //이동수가 최소 이동수의 5/4 이하가 되면 추가 코인
		coin2 += (int)((pow(2, select_floor)*5/4 - move_cnt) * 10);
	}
	coin += coin2;
	Sleep(500);
	system("cls");
	gotoxy(47,12);
	cout << move_cnt << "번 이동";
	gotoxy(38,14);
	cout <<"획득코인 : "<<coin2<<" 성공했습니다";
	Sleep(1000);
	if(select_floor==7)
		scoreCompare();

	setStartHanoi(); // 게임이 끝나면 다시 초기화
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

	if (j == 0){ // 그 장대에 아무 블럭이 없을때
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
			cout<<"랭 킹 등 록 완 료";
			Sleep(1000);
			break;
		}
	}
}

void rankInput(int a){
	char n[20];

	system("cls");
	gotoxy(20, 14);
	cout << move_cnt<<"개로 "<< a+1 <<"등입니다 이름을 입력해주세요 >> ";
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
	cout << "힌 트";
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
	cout << "2512 유시은";
	Sleep(3000);

	hint = false;

	system("cls");
}