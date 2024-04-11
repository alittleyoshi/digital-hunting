#include<bits/stdc++.h>
#include<conio.h>
#include<windows.h>
#include<ctime>
#include<fstream>
#include "shlwapi.h"
using namespace std;
int level; 
int dx[4]={-1,1,0,0},dy[4]={0,0,1,-1};
const int inf=1e9+5;
int mp[200][200];
int block[200][200];
char trans[5000];
int x,y,ex,ey,px,py;
int skill1time=3;
int siz=10;
int checkvis[200][200];
int checkcnt=0;
int distnow; 
int levelnum; 
string recpath="C:\\DigitalHunting\\record.txt";
string prcpath="C:\\DigitalHunting\\process.txt";
struct node{
	int x;
	int y;
	int dis;
};
void cls()
{
	system("cls");
}
void gotoxy(int mx,int my)
{
	COORD pos={mx,my};
	HANDLE hOut=GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hOut,pos);
	return;
}
bool outof(int cx,int cy)
{
	return (cx>0&&cy>0&&cx<=siz&&cy<=siz&&block[cx][cy]==0);
}
bool outof2(int cx,int cy)
{
	return (cx>=x-1&&cy>=y-1&&cx<=x+1&&cy<=y+1);
}
void checkroad(int nx,int ny)
{
	if(checkvis[nx][ny]==1||outof(nx,ny)==0)
		return;
	if(nx==px&&ny==py){
		checkcnt++;
		return;
	}
	checkvis[nx][ny]=1;
	checkroad(nx+1,ny);
	checkroad(nx,ny+1);
	checkroad(nx-1,ny);
	checkroad(nx,ny+1);
	
 } 
int roadcheck2(int nx,int ny)
{
	queue<node> q;
	q.push((node){nx,ny,0});
	while(!q.empty())
	{
		node now=q.front();
		q.pop();
		if(now.x==x&&now.y==y)
		{
			return now.dis;
		}
		for(int i=0;i<4;i++)
		{
			int vx=now.x+dx[i],vy=now.y+dy[i];
			if(checkvis[vx][vy]!=1&&outof(vx,vy)==1){
				checkvis[vx][vy]=1;
				q.push((node){vx,vy,now.dis+1});
			}
		}
	}
	return inf;
 } 
int enemycheck(int nex,int ney)
{
	memset(checkvis,0,sizeof(checkvis));
	return roadcheck2(nex,ney);
}
char getkey()
{
	char key;
	while(1)
	{
		key=0;
		if(_kbhit()){
			key=getch();
			break;
		}
	}
	if('A'<=key&&key<='Z')
		key=key-('A'-'a');
	return key;
	
}

char settrans()
{
	trans[0]=' ';
	trans[1]='O';
	trans[2]='X';
	trans[3]='V';
	trans[4]='|';
}
void ptmap()
{
	for(int i=1;i<=siz;i++){
		for(int j=1;j<=siz;j++)
			printf("%c ",trans[mp[i][j]]);	
		printf("\n");
	}
	cout<<endl;
	cout<<"You are O, your enemy is X, you need to get V"<<endl;
	cout<<"Use WASD to move"<<endl;
	
	cout<<"Here are some skills you can use:"<<endl;
	cout<<"1.Press E: Time Freeze. ("<<skill1time<<")"<<endl;
}
void setmap()
{
	memset(mp,0,sizeof(mp));
	for(int i=1;i<=siz;i++){
		for(int j=1;j<=siz;j++)
		{
			if(block[i][j]==1){
				mp[i][j]=4;
				continue;
			}
			if(x==i&&y==j)
				mp[i][j]=1; 
			if(ex==i&&ey==j)
				mp[i][j]=2; 
			if(px==i&&py==j)
				mp[i][j]=3; 
		}
	}
}

void enemymove(){
	int dist1=enemycheck(ex,ey);
	int dist2=inf;
	node choicemove=(node){ex,ey};
	
	for(int i=0;i<4;i++)
	{
		int vx=ex+dx[i],vy=ey+dy[i];
		if(outof(vx,vy)==0)
			continue;
		if(choicemove.x==ex&&choicemove.y==ey)
			choicemove=(node){vx,vy,0};
		dist2=enemycheck(vx,vy);
		if(dist2<dist1)
			choicemove=(node){vx,vy,0};
	}
	
	ex=choicemove.x;
	ey=choicemove.y;
	if(x==ex&&y==ey)
	{
		return;
	}
	
}
void puttres()
{
	int anum=0;
	while(1){
		srand(time(NULL));
		ex=(abs(rand()+anum)%siz)+1;
		srand(time(NULL));
		ey=(abs(rand()+anum)%siz)+1;
		if(enemycheck(ex,ey)>=siz)
			return;
		anum++;
	}
}

void putblock()
{
	int ranum=3;
	while(1){
		memset(checkvis,0,sizeof(checkvis));
		checkcnt=0;
		ranum++;
		for(int i=1;i<=siz;i++)
		{
			for(int j=1;j<=siz;j++)
			{
				srand(time(NULL));
				if(((i+j)/i+j/ranum+i*j+rand())%ranum==0)
					block[i][j]=1;
				else
					block[i][j]=0; 
			}
		}
		checkroad(1,1);
		if(checkcnt>=3)
			return;
	}
	block[px][py]=0;
	block[1][1]=0;
	block[ex][ey]=0;
}
void skill1()
{
	cls();
	int vx=x,vy=y;
	while(1){
		gotoxy(0,0);
		for(int i=1;i<=siz;i++){
			for(int j=1;j<=siz;j++)
			{
				if(vx==i&&vy==j)
					printf("O ");
				else if(i<=x+1&&j<=y+1&&i>=x-1&&j>=y-1&&!(i==ex&&j==ey)&&!(i==px&&j==py)&&block[i][j]==0)
					printf("  ");
				if((vx==i&&vy==j)||(i<=x+1&&j<=y+1&&i>=x-1&&j>=y-1&&!(i==ex&&j==ey)&&!(i==px&&j==py)&&block[i][j]==0))
					continue;
				if(mp[i][j]==0)
				{
					printf(". ");
					continue;
				}
				printf("%c ",trans[mp[i][j]]);	
			}
			printf("\n");
		}
		cout<<endl;
		cout<<"Time Freeze! You can move freely within a range of 3 * 3. "<<endl;
		cout<<"You won't fail during this period of time."<<endl;
		cout<<"Press E to end this state."<<endl;
		int fl=1;
		while(fl){//�ƶ���� 
			char nk=getkey();
			if(nk=='w'&&(outof(x-1,vy))&&(outof2(vx-1,vy))){
				fl=0;vx--;
			}
			if(nk=='s'&&(outof(vx+1,vy))&&(outof2(vx+1,vy))){
				fl=0;vx++;
			}
			if(nk=='a'&&(outof(vx,vy-1))&&(outof2(vx,vy-1))){
				fl=0;vy--;
			}
			if(nk=='d'&&(outof(vx,vy+1))&&(outof2(vx,vy+1))){
				fl=0;vy++;
			}
			if(nk=='e'){
				x=vx;y=vy;
				cls();
				return;
			}
		}
	}
}

void choosesiz()
{
	int nowchoice=1;
	while(1)
	{
		gotoxy(0,0);
		cout<<"Please choose the size of the game map."<<endl;
		for(int i=1;i<=4;i++){
			if(i==nowchoice)
				cout<<"-> ";
			else
				cout<<"   ";
			cout<<i*10<<" * "<<i*10<<endl;
		}
		if(5==nowchoice)
			cout<<"-> ";
		else
			cout<<"   ";
		cout<<"Enter the size yourself."<<endl;
		cout<<"(Press S,W to choose, press E to confirm)"<<endl;
		char nchr=getkey();
		if(nchr=='s'&&nowchoice<5)
			nowchoice++;
		if(nchr=='w'&&nowchoice>1)
			nowchoice--;
		if(nchr=='e')
		{
			if(nowchoice==5)
			{
				cls();
				cout<<"Plese input the size:";
				cin>>siz;
				cls();
				return;
			}
			else
			{
				siz=nowchoice*10;
				return;
			}
		}
		
	}
 } 
int menu()
{
	string str[3]={"Game start","record","quit"};
	int nowchoice=0;
	while(1)
	{
		gotoxy(0,0);
		cout<<"Digital Hunting"<<endl;
		for(int i=0;i<3;i++){
			if(i==nowchoice)
				cout<<"-> ";
			else
				cout<<"   ";
			cout<<str[i]<<endl;
		}
		cout<<"(Press S,W to choose, press E to confirm)"<<endl;
		char nchr=getkey();
		if(nchr=='s'&&nowchoice<2)
			nowchoice++;
		if(nchr=='w'&&nowchoice>0)
			nowchoice--;
		if(nchr=='e')
		{
			if(nowchoice==0)
			{
				cls();
				return 0;
			}
			else if(nowchoice==1)
			{
				cls();
				return 1;
			}
			else
			{
				cls();
				return 2;
			}
		}
		
	}
}  
int gamestart()
{
	cls(); 
	cout<<"LEVEL "<<levelnum<<endl;
	Sleep(1000);
	skill1time=siz/4+1;
	cls();
	cout<<"Loading map......"<<endl;
	py=siz,px=siz;
	x=1,y=1;
	puttres();
	putblock();
	int step=0;
	cls();
	while(1)
	{
		gotoxy(0,0);
		setmap();
		ptmap();
		 
		bool fl=1;
		while(fl){//�ƶ���� 
			char nk=getkey();
			if(nk=='w'&&(outof(x-1,y))){
				fl=0;x--;
			}
			if(nk=='s'&&(outof(x+1,y))){
				fl=0;x++;
			}
			if(nk=='a'&&(outof(x,y-1))){
				fl=0;y--;
			}
			if(nk=='d'&&(outof(x,y+1))){
				fl=0;y++;
			}
			if(nk=='e'&&skill1time>0){
				skill1time--;
				skill1();
				goto loop;
			}
		}
		step++;
		srand(time(NULL));
		if(step%(rand()%2+1)==0)
		{
			srand(time(NULL));
			for(int i=1;i<=(step+rand())%3+1;i++)
				enemymove();
		}
		loop:
		if(x==px&&y==py)
		{
			char keyn=' ';
			while(keyn!='q'&&keyn!='s'){
				cls();
				cout<<"YOU WIN!"<<endl;
				cout<<"press \"q\"to go to the next level."<<endl;
				cout<<"press \"s\"to save your process."<<endl;
				keyn=getkey();
			}
			cls();
			if(keyn=='s')
			{
				char clist[50]; 
				sprintf(clist,"%d %d\n",levelnum+1,siz);
				const char* cpath=prcpath.c_str();
				FILE* fp=fopen(cpath,"w");
				fprintf(fp,clist);
				fclose(fp); 
				cls();
				return 2;
			}
			return 1;
		}
		if(x==ex&&y==ey)
		{
			char keyn=' ';
			while(keyn!='q'){
				cls();
				cout<<"YOU FAIL!"<<endl;
				cout<<"Your adventure ends up in LEVEL "<<levelnum<<endl; 
				cout<<"press \"q\"to quit."<<endl;
				keyn=getkey();
			}
			cls();
			return 0;
		}
	}
}
void seerecord(){
		const char* cpath=recpath.c_str();
		FILE *fp = fopen(cpath,"r");
		if(fp==NULL)
		{
			ofstream file(cpath);
			file.close();
			cout<<"No record."<<endl;
			cout<<"(Press E to quit)"<<endl;
			while(1){
				char keyn=getkey();
				if(keyn=='e')
				{
					cls();
					return;
				}
			}
		}
		char recs[1024];
		stack<int> numsta;
		stack<int> sizsta;
		stack<string> timsta;
		while(!feof(fp))
		{
			fscanf(fp,"%s",recs);
			int recnum=0,recflg=0,recsiz=0;
			string rectim="aaaaaaaaaa";
			for(int i=0;i<=9;i++)
				rectim[i]=recs[i];
			for(int i=10;recs[i]!=')';i++)
			{
				if(recs[i]=='('){
					recflg=1;
					continue;
				}
				if(recs[i]==')'){
					recflg=0;
					continue;
				}
					
				if(recflg==1){
					recsiz=recsiz*10+(recs[i]-'0');
				}
				else{
					recnum=recnum*10+(recs[i]-'0');
				}
				
			}
			numsta.push(recnum);
			timsta.push(rectim);
			sizsta.push(recsiz);
		}
		sizsta.pop();numsta.pop();timsta.pop();
		if(timsta.empty())
			cout<<"No record."<<endl;
		while(!timsta.empty())
		{
			int nowsiz=sizsta.top(),nownum=numsta.top();
			string nowtim=timsta.top();
			sizsta.pop();numsta.pop();timsta.pop();
			cout<<nowtim<<": LEVEL"<<nownum<<" (map size: "<<nowsiz<<" * "<<nowsiz<<" ) "<<endl; 
		}
		fclose(fp);
		cout<<"(Press E to quit)"<<endl;
		while(1){
			char keyn=getkey();
			if(keyn=='e')
			{
				cls();
				return;
			}
		}
}
int main()
{
	
    CreateDirectory("C:\\DigitalHunting", NULL);
    const char* cpath=prcpath.c_str();
	ifstream fp(cpath);
	if(fp){
		if(fp.peek() == std::ifstream::traits_type::eof()){
			fp.close();
			FILE* fp=fopen(cpath,"w");
			fprintf(fp,"NULL");
			fclose(fp);
		}
	}
	else
	{
		fp.close();
		std::ofstream file(cpath);
		file.close();
		FILE* fp=fopen(cpath,"w");
		fprintf(fp,"NULL");
		fclose(fp);
	}
	
	
	
	settrans();
	while(1){
		cls();
		int menuch=menu();
		cls();
		if(menuch==0){
			const char* cpath=prcpath.c_str();
			FILE *fp = fopen(cpath,"r");
			char recs[1024];
			fscanf(fp,"%s",recs);
			string instr0(recs);
			if(instr0!="NULL"){
				cout<<"There's already a game process.";
				Sleep(500);
				cls();
				
				int i=0,rnum=0;
				string instr(recs);
				for(int i=0;i<instr.length();i++)
					rnum=rnum*10+(instr[i]-'0');
				levelnum=rnum;
				fscanf(fp,"%s",recs);
				i=0,rnum=0;
				string instr2(recs);
				for(int i=0;i<instr2.length();i++)
					rnum=rnum*10+(instr2[i]-'0');
				siz=rnum;
			}
			else{
				choosesiz();
				levelnum=1;	
			}
			fclose(fp);
			int result;
			while(1){
				result=gamestart();
				if(result==0||result==1)
				{
					cpath=prcpath.c_str();
					FILE* fp=fopen(cpath,"w");
					fprintf(fp,"NULL");
					fclose(fp); 
				}
				if(result==0||result==2)
					break;
				
				levelnum++;
			}
			if(result==0||result==1)
			{
				cout<<"Saving record......"<<endl;
				cpath=recpath.c_str();
				fp=fopen(cpath,"a");
				SYSTEMTIME time;
				GetLocalTime(&time);
				char clist[50]; 
				sprintf(clist,"%04d/%02d/%02d%d(%d)\n" ,time.wYear, time.wMonth, time.wDay, levelnum,siz);
				fprintf(fp,clist);
				fclose(fp); 	
			}
			cls();
		}
		if(menuch==1){
			seerecord();
		}
		if(menuch==2){
			return 0;
		}
	}
	
	
	
}
