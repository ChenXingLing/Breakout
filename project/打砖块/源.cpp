/*
----------------------------------------------------------------------------------
            ���ߣ������� (Xing_Ling)                                               
            Blog 1��www.cnblog.com/Xing-Ling                                      
			Blog 2��www.luogu.cn.com/ChenXingLing/#/                              
----------------------------------------------------------------------------------
*/

#include "myprepare.h"

IMAGE bkimg_up,bkimg_down;//��Ϸ����󱳾�
IMAGE bkimg_up_st,bkimg_down_st;//��ʼ����󱳾�
IMAGE bkimg_up_option,bkimg_down_option;//ѡ����ڴ󱳾�
IMAGE option_openedlevel_img[2];//[�ѿ��Źؿ�]ѡ��
IMAGE option_closedlevel_img[2];//[δ���Źؿ�]ѡ��
IMAGE option_returnback_img[2];//[���ؿ�ʼ����]ѡ��
IMAGE option_gamestart_img[2];//[��ʼ��Ϸ]ѡ��
IMAGE option_gameinfo_img[2];//[��Ϸ˵��]ѡ��
IMAGE option_order_img[2];//[�˵�]ѡ��
IMAGE text_Title_img;//���������
IMAGE background_HParea_img;//Ѫ�����򱳾�
IMAGE tempppp;

const int MAX_HP_cnt=10;//���HPֵ
const int MAX_level_cnt=56;//��Ϸ�ؿ�����(���Ϊ56)
const int common_level_cnt=18;//��ͨ�ؿ�����
const int playerstyle_cnt=1;//�����ʽ����
const int brickstyle_cnt=5;//ש����ʽ����
const int ballstyle_cnt=3;//С����ʽ����
const int MAX_brick_cnt=160;//���ש����� brick_size: 25*40 MAX_brick_cnt:16*10
const int MAX_ball_cnt=5;//���С������

const COLOR brick_style_color     [brickstyle_cnt+1]={0,lightgreen, lightblue, lightpurple, orange, darkpurple};//ש�������ɫ������style��
const COLOR brick_style_color_line[brickstyle_cnt+1]={0,brown     , brown    , brown      , brown , brown     };//ש��߿���ɫ������style��
const COLOR brick_shadow_color=darkgray;//ש����Ӱ��ɫ
const COLOR player_style_color     [playerstyle_cnt+1]={0,pink };//��������ɫ������style��
const COLOR player_style_color_line[playerstyle_cnt+1]={0,brown};//��ұ߿���ɫ������style��
const COLOR player_shadow_color=darkgray;//�����Ӱ��ɫ
const COLOR default_option_line_color[2]={darkgray,lightgray};//Ĭ��ѡ��߿���ɫ������״̬ �� ���ָ��״̬��
const COLOR default_option_text_color[2]={lightcyan,white};//Ĭ��ѡ��������ɫ������״̬ �� ���ָ��״̬��
const COLOR level_opened_option_text_color[2]={RGB(0,255,30),white};//�ѿ��Źؿ�ѡ��������ɫ������״̬ �� ���ָ��״̬��
const COLOR level_closed_option_text_color[2]={lightgray,white};//δ���Źؿ�ѡ��������ɫ������״̬ �� ���ָ��״̬��
const COLOR default_debug_text_color=white;//Ĭ��debug����������ɫ
//const COLOR Title_text_color=lightcyan;//�����������ɫ
//const COLOR Title_textshadow_color=cyan;//�����������Ӱ��ɫ
//const COLOR Staff_text_color=cyan;

#define level_endless MAX_level_cnt //�޾�ģʽ�ؿ�

#define UP_LINE 1
#define LEFT_LINE 2
#define RIGHT_LINE 3
#define DOWN_LINE 4
#define LEFT_UP_ARC 5
#define RIGHT_UP_ARC 6
#define LEFT_DOWN_ARC 7
#define RIGHT_DOWN_ARC 8

namespace SETBOX{
	const Range mainbox={0,0,480-1,720-1};
	const Range debugbox={0,720,480-1,820-1};
	const Range gamebox={0,24,480-1,720-1};//С���˶�����
	const Range brickbox={41,71,440,470};
	const Range playerbox={0,700-7,480-1,700+7};
	const Range gamestart_box={95,480,385,560};
	const Range gameinfo_box={95,610,385,690};
	const Range returnback_box={110,625,370,675};
	const Range HParea_box={7,1,236,23};
	const Range order_box={244,1,473,23};
	const Range gameinfo_text_box={7,7,473-1,650};
}
using namespace SETBOX;

namespace CONTROLER{
	int controlermode_make_level;
	int controlermode_level_allopen;
	inline void init(){
		controlermode_make_level=controlermode_level_allopen=0;
	}
};
using CONTROLER::controlermode_make_level;
using CONTROLER::controlermode_level_allopen;

namespace BRICKINFO{
	//brick_box={41,71,440,470}, brick_size: 25*40, MAX_brick_cnt: 16*10
	const int brick_lenth=40,brick_height=25;

	struct Brickstyle{

		int type;

		inline void getstyle(Re type_id){
			type=type_id;
			if(type==1){};//type1: 1��ש�飬һ������
		}
	}style[brickstyle_cnt+1];
	
	Range id_to_R[MAX_brick_cnt+1];
	int map_to_id[480][720];

	struct Brick{
		//styletype=0: ����
		//styletype=1: һ��ש��
		int styletype;int Rid;
		IMAGE background_img;
		inline void print(){
			if(!styletype)return;
			drawrange(id_to_R[Rid].cutmid(1,1),brick_style_color[styletype],brick_shadow_color,2);//����Ӱ
			drawrange((id_to_R[Rid].cutmid(1,1)).transfer(0,0,-1,-1),brick_style_color[styletype],brick_style_color_line[styletype],2);
		}
		inline void vanish(){
			if(styletype>0&&styletype<5){
				putimage(id_to_R[Rid].x1,id_to_R[Rid].y1,background_img);
				styletype--;
				print();
			}
		}
		inline void playgame_main_init(Re _Rid_,Re _styletype_){
			styletype=_styletype_,background_img=getimage(id_to_R[Rid=_Rid_]);
		}

		inline int controler_levelmaker_pre(){
			putimage(id_to_R[Rid].x1,id_to_R[Rid].y1,background_img);
			styletype=(styletype-1+6)%6;
			print();
			return styletype;
		}
		inline int controler_levelmaker_next(){
			putimage(id_to_R[Rid].x1,id_to_R[Rid].y1,background_img);
			styletype=(styletype+1)%6;
			print();
			return styletype;
		}
		inline void controler_levelmaker_init(Re _Rid_){
			styletype=0,background_img=getimage(id_to_R[Rid=_Rid_]);
		}
	};

	inline void init(){
//_Begin();
		for(Re i=1;i<=16;++i)
			for(Re j=1;j<=10;++j){
				Re id=(i-1)*10+j;
				Re x1=41+(j-1)*brick_lenth;
				Re y1=71+(i-1)*brick_height;
				Re x2=41+j*brick_lenth-1;
				Re y2=71+i*brick_height-1;
				id_to_R[id]=Range(x1,y1,x2,y2);
				for(Re I=x1;I<=x2;++I)
					for(Re J=y1;J<=y2;++J)
						map_to_id[I][J]=id;
			}

//_End(),_Flush();
	}
}
using BRICKINFO::Brick;
using BRICKINFO::id_to_R;
using BRICKINFO::map_to_id;

namespace LEVELINFO{//���ؿ���Ϣ��
	
	struct LEVEL{
		int type;//�ؿ����
		int HP_cnt,brick_cnt,brick_style[MAX_brick_cnt+1];
		
		inline void init_endless(){//�޾�ģʽ
			
		}
		inline void init(Re type_id,char ch[50][50]){
			type=type_id;
			if(type==level_endless){ HP_cnt=10,init_endless(); return; }
			HP_cnt=3;
			for(Re i=1;i<=16;++i)for(Re j=1;j<=10;++j){
				Re id=(i-1)*10+j;
				if(ch[i][j]>='0'&&ch[i][j]<='9')
					brick_style[id]=int(ch[i][j]-'0'),brick_cnt+=(brick_style[id]>0);
			}
		}
	}Level[MAX_level_cnt+1];
	char ch[50][50];
	inline void open_file_read_levelid(int id){
		freopen("CON","r",stdin); freopen("CON","w",stdout);
		std::string _id; std::stringstream ss; ss<<id; ss>>_id;
		std::string filepath="./data/level/level_"+_id+".txt";
		freopen(filepath.c_str(),"r",stdin);
	}
	inline void close_file(){ freopen("CON","r",stdin); freopen("CON","w",stdout); }
	inline void init(){//���ؿ���Ϣ����ʼ�� ���ļ��ж�ȡ��ͼ
		for(int id=1;id<MAX_level_cnt;++id){

			open_file_read_levelid(id);
			for(Re i=1;i<=16;++i)scanf("%s",ch[i]+1);
			close_file();

			Level[id].init(id,ch);
		}
	}
}
using LEVELINFO::Level;

namespace PLAYERINFO{
	//player_box={0,700-7,480-1,700+7}

	struct PlayerStyle{

		int type;

		inline void getstyle(Re type_id){
			type=type_id;
		}
	}style[playerstyle_cnt+1];

	const double player_center_range=1.0/4.0;
	struct Player{
		IMAGE background_img;
		Range R;int styletype;
		int mid_x,lenth;
		LD center_L,center_R;
		inline void print(){
			drawrange(R.cutmid(1,1),player_style_color[styletype],player_shadow_color,2);//����Ӱ
			drawrange((R.cutmid(1,1)).transfer(0,0,-1,-1),player_style_color[styletype],player_style_color_line[styletype],2);
		}
		inline void refresh(Re x,Re len){
_Begin();
			putimage(R.x1,R.y1,background_img);

			mid_x=x,lenth=len;
			R.x1=mid_x-lenth,R.x2=mid_x+lenth;
			center_L=mid_x-lenth*player_center_range,center_R=mid_x+lenth*player_center_range;
			
			background_img=getimage(R);

			print();
_End(),_Flush();
		}
		inline void playgame_init(Re style=1){
			mid_x=240,lenth=50,styletype=style;
			R.y1=700-7,R.y2=700+7;
//R.y1=500-207,R.y2=500+207; (����ģʽ���������)
			R.x1=mid_x-lenth,R.x2=mid_x+lenth;
			center_L=mid_x-lenth*player_center_range,center_R=mid_x+lenth*player_center_range;

			background_img=getimage(R);

			print();
		}

		inline int updata(DB_Point &DB_P,Re r,Re m,Re len=0){//�������λ��,len>0:������ҳ���Ϊlen
			if(!len){
				m=max(m,0+lenth),m=min(m,480-1-lenth);
				if(m==mid_x)return 0;//δ�ƶ�

				int p=dcmp(DB_P.y+r-R.y1);
				if(p<0){ refresh(m,lenth); return 0; }//���С��û�����������

				else if(p==0){//�պõ����һ������
					DB_P.y-=eps,refresh(m,lenth);
					return (dcmp(R.x1-eps - DB_P.x)<=0&&dcmp(R.x2+eps - DB_P.x)>=0)?UP_LINE:0;//1.���߶� ��eps�����ӳ�һ�㣨������Բ���Ľ������Ϊ�ߣ�
				}

				else{//�������������
					LD Lx,Rx;//��������
					int op;//С���°벿����ȫ�������������
					
					if(dcmp(DB_P.y-R.y1)>=0)Lx=DB_P.x-r,Rx=DB_P.x+r,op=1;//С���°벿����ȫ�������������
					else{
						LD dis=sqrt(1.0*r*r-Squre(R.y1-DB_P.y));
						Lx=DB_P.x-dis,Rx=DB_P.x+dis; op=0;
					}

					if(m<mid_x&&DB_P.x<mid_x){//��������ƶ�,���������
						if(dcmp(Rx+lenth-m)>=0){//��Rx����
							m=int(Rx+lenth+eps); refresh(m,lenth);
							return op?LEFT_LINE:LEFT_UP_ARC;
						}

					}
					if(m>mid_x&&DB_P.x>mid_x){//��������ƶ�,�������ұ�
						if(dcmp(Lx-lenth-m)<=0){//��Lx����
							m=int(Lx-lenth+eps); refresh(m,lenth);
							return op?RIGHT_LINE:RIGHT_UP_ARC;
						}
					}
					refresh(m,lenth); return 0;//δײ��
				}
			}
			else{//������ҳ���(������)
				if(len==lenth)return 0;
				refresh(m,len); return 0;
			}
		}
	};

	inline void init(){
//_Begin();

//_End(),_Flush();
	}
}
using PLAYERINFO::Player;
using PLAYERINFO::player_center_range;

namespace BALLINFO{//��С����Ϣ��

	#define ball_img 1
	#define ball_black 2

	struct BallStyle{

		int type,cnt,r;Point P[277+1+16];IMAGE ballpimg[277+1+16];

		inline void getstyle(Re type_id){
			type=type_id,cnt=0;
			if(type==1){ r=9; }
			if(type==2){ r=9;return; }
			if(type==3){ r=20;return; }//���԰�
			if(type==4){ r=12;return; }//ʹ�õ��ߡ��Ŵ󡿺��2
			int pan[19][19];
			memset(pan,0,sizeof(pan));
			for(Re i=7;i<=11;++i)pan[i][0]=1;
			for(Re i=5;i<=13;++i)pan[i][1]=1;
			for(Re i=3;i<=15;++i)pan[i][2]=1;
			for(Re i=2;i<=16;++i)pan[i][3]=pan[i][4]=1;
			for(Re i=1;i<=17;++i)pan[i][5]=pan[i][6]=1;
			for(Re i=0;i<=18;++i)pan[i][7]=pan[i][8]=pan[i][9]=pan[i][10]=pan[i][11]=1;
			for(Re i=1;i<=17;++i)pan[i][12]=pan[i][13]=1;
			for(Re i=2;i<=16;++i)pan[i][14]=pan[i][15]=1;
			for(Re i=3;i<=15;++i)pan[i][16]=1;
			for(Re i=5;i<=13;++i)pan[i][17]=1;
			for(Re i=7;i<=11;++i)pan[i][18]=1;

			//pan[6][0]=pan[0][6]=pan[4][1]=pan[1][4]=1;
			//pan[18-6][0]=pan[18-0][6]=pan[18-4][1]=pan[18-1][4]=1;
			//pan[6][18-0]=pan[0][18-6]=pan[4][18-1]=pan[1][18-4]=1;
			//pan[18-6][18-0]=pan[18-0][18-6]=pan[18-4][18-1]=pan[18-1][18-4]=1;

			for(Re i=0;i<=18;++i)
				for(Re j=0;j<=18;++j)
					if(pan[i][j])
						P[++cnt]={i,j},
						ballpimg[cnt]=getimage(i,j,i,j);
		}

	}style[ballstyle_cnt+1];
	
	struct Ball{
		int r,styletype;
		Point P;//��ǰ��ͼ����
		DB_Point DB_P;//��ǰʵ������

		LD speed,steplenth;//speed:�ٶ� steplenth:ÿ���ƶ���ֱ�߳���
		DB_Point Dir_ratio,DB_Dir;//Dir_ratio��DB_Dir:�ƶ�����ֱ�Ǳ���б��֮�� DB_Dir:ʵ���ƶ�����

		Ball(Point p={100,100},Re T=2){ P=p,r=style[styletype=T].r,speed=1; }

		IMAGE background_img;
		inline void playgame_ball_getimage(){ 
			background_img=getimage(P.x-r,P.y-r,P.x+r,P.y+r);
		}
		inline void print_background(){
			putimage(P.x-r,P.y-r,background_img);
		}
		#define bs style[styletype]
		inline void print(){
			if(styletype==1){
				Re x=P.x-r,y=P.y-r;
				for(Re i=1;i<=bs.cnt;++i)
					putimage({x+bs.P[i].x,y+bs.P[i].y},bs.ballpimg[i]);
			}
			else if(styletype==2||styletype==3){
				LD st=56,ed=255,len=ed-st;Setlinestyle(1);
				for(Re i=r;i>=1;--i){
					Re tmp=int(st+len*(r-i)/r);
					drawcircle({P.x,P.y},i,RGB(tmp,tmp,tmp),1);
					drawcircle({P.x,P.y},i,RGB(tmp,tmp,tmp));
				}
			}
		}
		#undef bs
		
		inline void init_refresh(){	playgame_ball_getimage(),print(); }
		inline void refresh(Point P0){
_Begin();

			print_background();
			P=P0;
			playgame_ball_getimage();
			print();

_End(),_Flush();
		}

		inline void calc_DB_Dir(LD speed0){
			speed=speed0;
			if(styletype==1)steplenth=speed*10;//����speed��������
			else steplenth=speed*5;
			
			DB_Dir=DB_Point(steplenth*Dir_ratio.x,steplenth*Dir_ratio.y);//ÿ��ˢ��С��״̬��Ҫ�ߵľ���
			if(!dcmp(DB_Dir.x))DB_Dir.x=(Rand(0,1)?1:-1)*0.01,Dir_ratio.x=DB_Dir.x/steplenth;//΢��ƫ�Ƿ�ֹ��������
			if(!dcmp(DB_Dir.y))DB_Dir.y=(Rand(0,1)?1:-1)*0.01,Dir_ratio.y=DB_Dir.y/steplenth;//΢��ƫ�Ƿ�ֹ��������
		}
		inline void start_biu(Point ms){
			Point Dir=Point(ms.x-P.x,ms.y-P.y);
			Re d=gcd(abs(Dir.x),abs(Dir.y));Dir.x/=d,Dir.y/=d;
			LD incline=sqrt(Dir.x*Dir.x+Dir.y*Dir.y);
			Dir_ratio.x=Dir.x/incline,Dir_ratio.y=Dir.y/incline;
			
			calc_DB_Dir(1.0);
			DB_P=DB_Point(1.0*P.x,1.0*P.y);
		}

		inline void reverse_dir(Re flagx,Re falgy){//��ת�˶�����
			if(flagx)Dir_ratio.x=-Dir_ratio.x,DB_Dir.x=-DB_Dir.x;
			if(falgy)Dir_ratio.y=-Dir_ratio.y,DB_Dir.y=-DB_Dir.y;
		}
		inline void reverse(int flag){//С������ײ���ı��˶��켣
			if(flag==UP_LINE)reverse_dir(0,DB_Dir.y>0);//1.���߶�
			else if(flag==LEFT_LINE)reverse_dir(DB_Dir.x>0,0);//2.���߶�
			else if(flag==RIGHT_LINE)reverse_dir(DB_Dir.x<0,0);//3.���߶�
			else if(flag==DOWN_LINE)reverse_dir(0,DB_Dir.y<0);//4.���߶�
			else if(flag==LEFT_UP_ARC)reverse_dir(DB_Dir.x>0,DB_Dir.y>0);//5.����Բ��
			else if(flag==RIGHT_UP_ARC)reverse_dir(DB_Dir.x<0,DB_Dir.y>0);//6.����Բ��
			else if(flag==LEFT_DOWN_ARC)reverse_dir(DB_Dir.x>0,DB_Dir.y<0);//7.����Բ��
			else if(flag==RIGHT_DOWN_ARC)reverse_dir(DB_Dir.x<0,DB_Dir.y<0);//8.����Բ��
		}
		
		inline void judge_wall(Player &player,Range R1,DB_Point &P,DB_Point &P_){//����ײ��⡿�ж�������ǽ����ײ���
			Re p1=dcmp(P_.x-R1.x1);//p1<=0 �����
			Re p2=dcmp(P_.y-R1.y1);//p2<=0 ���ϱ�
			Re p3=dcmp(P_.x-R1.x2);//p3>=0 ���ұ�
			if(p1<=0&&p2<=0){//���� �� ����
				if(p1==0&&p2==0)P_.x=R1.x1,P_.y=R1.y1,reverse_dir(1,1);//������ײ��ͬʱ����ں��ϱڣ�
				else{
					LD t1=(P.x-R1.x1)/DB_Dir.x,t2=(P.y-R1.y1)/DB_Dir.y;
					if(t1<t2)P_.x=R1.x1, reverse_dir(1,0);//�������
					else P_.y=R1.y1, reverse_dir(0,1);//�����ϱ�
				}
			}
			else if(p3>=0&&p2<=0){//���� �� ����
				if(p3==0&&p2==0)P_.x=R1.x2,P_.y=R1.y1,reverse_dir(1,1);//������ײ��ͬʱ���ұں��ϱڣ�
				else{
					LD t1=(R1.x2-P.x)/DB_Dir.x,t2=(P.y-R1.y1)/DB_Dir.y;
					if(t1<t2)P_.x=R1.x2, reverse_dir(1,0);//�����ұ�
					else P_.y=R1.y1, reverse_dir(0,1);//�����ϱ�
				}
			}
			else if(p1<=0)P_.x=R1.x1,reverse_dir(1,0);//����
			else if(p2<=0)P_.y=R1.y1,reverse_dir(0,1);//����
			else if(p3>=0)P_.x=R1.x2,reverse_dir(1,0);//����
			//P_.x=max(P_.x,R1.x1),P_.y=max(P_.y,R1.y1);//P.x-r>=gamebox.x1,P.y-r>=gamebox.y1
			//P_.x=min(P_.x,R1.x2),P_.y=min(P_.y,R1.y2);//P.x+r<=gamebox.x2,P.y+r<=gamebox.y2
			return;
		}
		
		inline int get_id(DB_Point P,DB_Point O){//������Բ���ĸ���λ
			if(P.x<O.x)return P.y<O.y?5:7;//����,����
			else return P.y<O.y?6:8;//����,����
		}
		inline int cross_LH(DB_Point st,DB_Point ed,DB_Point O,LD R,Re op,DB_Point &CP){//�߶���Բ���Ľ���
			DB_Point A=st,B=ed;
			Re cnt=GMT::cross_LC(A,B,O,R);
			if(!cnt)return 0;//���û��һ������
			Re idA=get_id(A,O);
			if(cnt==1){//���ֻ��һ������
				if(idA==op){ CP=A;return 1; }//�ڸ�Բ����
				return 0;
			}
			//�������������
			Re idB=get_id(B,O);
			if(idA==op&&idB==op){//����������ڸ�Բ����
				CP=GMT::Len(B-st)<GMT::Len(A-st)?B:A;//ȡ��st�����Ľ���
				return 1;
			}
			if(idA==op){ CP=A;return 1; }//A�ڸ�Բ����
			if(idB==op){ CP=B;return 1; }//B�ڸ�Բ����
			return 0;
		}
		
		inline int judge_player(Player &py,DB_Point &st,DB_Point &ed){//����ײ��⡿�ж�����ҵ���ײ���
			if(dcmp(ed.y+r-py.R.y1)<0)return 0;//С��֦������˶��켣���յ㻹û���������
			DB_Point CP;Re flag=0;LD dis_CPP=2e9;

			DB_Point A=st,B=ed,A_,B_;
			A_=DB_Point(py.R.x1,py.R.y1-r),B_=DB_Point(py.R.x2,py.R.y1-r);
			if(GMT::pan_cross_LL(A,B,A_,B_)){//1.���߶�
				CP=GMT::cross_LL(A,B,A_,B_); flag=UP_LINE; dis_CPP=GMT::Len(CP-st);
			}
			A_=DB_Point(py.R.x1-r,py.R.y1),B_=DB_Point(py.R.x1-r,py.R.y2);
			if(GMT::pan_cross_LL(A,B,A_,B_)){//2.���߶�
				DB_Point tmp_CP=GMT::cross_LL(A,B,A_,B_); LD tmp_dis=GMT::Len(tmp_CP-st);
				if(tmp_dis<dis_CPP)CP=tmp_CP,flag=LEFT_LINE,dis_CPP=tmp_dis;
			}
			A_=DB_Point(py.R.x2+r,py.R.y1),B_=DB_Point(py.R.x2+r,py.R.y2);
			if(GMT::pan_cross_LL(A,B,A_,B_)){//3.���߶�
				DB_Point tmp_CP=GMT::cross_LL(A,B,A_,B_); LD tmp_dis=GMT::Len(tmp_CP-st);
				if(tmp_dis<dis_CPP)CP=tmp_CP,flag=RIGHT_LINE,dis_CPP=tmp_dis;
			}

			DB_Point tmp_CP;Re pan;
			pan=cross_LH(A,B,DB_Point(py.R.x1,py.R.y1),r,LEFT_UP_ARC,tmp_CP);//5.����Բ��
			if(pan){
				LD tmp_dis=GMT::Len(tmp_CP-st);
				if(tmp_dis<dis_CPP)CP=tmp_CP,flag=LEFT_UP_ARC,dis_CPP=tmp_dis;
			}
			pan=cross_LH(A,B,DB_Point(py.R.x2,py.R.y1),r,RIGHT_UP_ARC,tmp_CP);//6.����Բ��
			if(pan){
				LD tmp_dis=GMT::Len(tmp_CP-st);
				if(tmp_dis<dis_CPP)CP=tmp_CP,flag=RIGHT_UP_ARC,dis_CPP=tmp_dis;
			}

			if(flag){//������ײ���ı��˶��켣
//ed=CP,reverse(flag); return flag;
				if(flag==2||flag==3){ ed=CP,reverse(flag); return flag; }//(���淴��) 2.���߶� 3.���߶�
				if(flag==1){//1.���߶�
					if(dcmp(CP.x-py.center_L)>=0&&dcmp(CP.x-py.center_R)<=0){ ed=CP,reverse(flag); return flag; }//(���淴��) ��������
					LD rad;
					if(CP.x<py.center_L)rad=toPi(75.0-60.0*(py.center_L-CP.x)/(py.lenth*(1-player_center_range))),Dir_ratio.x=-cos(rad);//(���ⷴ��) ������
					if(CP.x>py.center_R)rad=toPi(75.0-60.0*(CP.x-py.center_R)/(py.lenth*(1-player_center_range))),Dir_ratio.x= cos(rad);//(���ⷴ��) ������
					Dir_ratio.y=-sin(rad);
					calc_DB_Dir(speed);
					ed=CP; return flag;
				}
				if(flag==5||flag==6){//5.����Բ�� 6.����Բ��
					LD rad=toPi(15.0);//��Բ��ֱ��15����Сƫת��
					if(flag==5)Dir_ratio.x=-cos(rad);//(���ⷴ��) 5.����Բ��
					if(flag==6)Dir_ratio.x= cos(rad);//(���ⷴ��) 6.����Բ��
					Dir_ratio.y=-sin(rad);
					calc_DB_Dir(speed);
					ed=CP; return flag;
				}
			}
			else return 0;//δ������ײ
		}
		
		int Rid_tail,Rid_stack[MAX_brick_cnt+1],Rid_stack_pan[MAX_brick_cnt+1];
		inline void find_Rid_area(DB_Point &st,DB_Point &ed,Brick *brick){//�ҵ�С�����������ש��
			Re x1=max(min(int(st.x-r)-1,int(ed.x-r)-1),0),     y1=max(min(int(st.y-r)-1,int(ed.y-r)-1),0);
			Re x2=min(max(int(st.x+r)+1,int(ed.x+r)+1),480-1), y2=min(max(int(st.y+r)+1,int(ed.y+r)+1),720-1);
			for(Re i=x1;i<=x2;++i) for(Re j=y1;j<=y2;++j)
				if(map_to_id[i][j]&&!Rid_stack_pan[map_to_id[i][j]]&&brick[map_to_id[i][j]].styletype)
					Rid_stack_pan[Rid_stack[++Rid_tail]=map_to_id[i][j]]=1;
		}
		inline void judge_brick_line(Re Rid,Re now_flag,DB_Point &A,DB_Point &B,DB_Point &A_,DB_Point &B_,DB_Point &CP,Re &flag,Re &bk_Rid,LD &dis_CPP){
			if(GMT::pan_cross_LL(A,B,A_,B_)){
				DB_Point tmp_CP=GMT::cross_LL(A,B,A_,B_); LD tmp_dis=GMT::Len(tmp_CP-A);
				if(tmp_dis<dis_CPP)CP=tmp_CP,flag=now_flag,bk_Rid=Rid,dis_CPP=tmp_dis;
			}
		}
		inline void judge_brick_arc(Re Rid,Re now_flag,DB_Point A,DB_Point B,DB_Point O,LD r,DB_Point &CP,Re &flag,Re &bk_Rid,LD &dis_CPP){
			DB_Point tmp_CP;Re pan=cross_LH(A,B,O,r,now_flag,tmp_CP);
			if(pan){
				LD tmp_dis=GMT::Len(tmp_CP-A);
				if(tmp_dis<dis_CPP)CP=tmp_CP,flag=now_flag,bk_Rid=Rid,dis_CPP=tmp_dis;
			}
		}
		inline void judge_brick_(Re Rid,DB_Point &A,DB_Point &B,DB_Point &CP,Re &flag,Re &bk_Rid,LD &dis_CPP){
			Range R=id_to_R[Rid];

			judge_brick_line(Rid,UP_LINE   ,A,B,DB_Point(R.x1,R.y1-r),DB_Point(R.x2,R.y1-r),CP,flag,bk_Rid,dis_CPP);//1.���߶�
			judge_brick_line(Rid,LEFT_LINE ,A,B,DB_Point(R.x1-r,R.y1),DB_Point(R.x1-r,R.y2),CP,flag,bk_Rid,dis_CPP);//2.���߶�
			judge_brick_line(Rid,RIGHT_LINE,A,B,DB_Point(R.x2+r,R.y1),DB_Point(R.x2+r,R.y2),CP,flag,bk_Rid,dis_CPP);//3.���߶�
			judge_brick_line(Rid,DOWN_LINE ,A,B,DB_Point(R.x1,R.y2+r),DB_Point(R.x2,R.y2+r),CP,flag,bk_Rid,dis_CPP);//4.���߶�

			judge_brick_arc(Rid,LEFT_UP_ARC    ,A,B,DB_Point(R.x1,R.y1),r,CP,flag,bk_Rid,dis_CPP);//5.����Բ��
			judge_brick_arc(Rid,RIGHT_UP_ARC   ,A,B,DB_Point(R.x2,R.y1),r,CP,flag,bk_Rid,dis_CPP);//6.����Բ��
			judge_brick_arc(Rid,LEFT_DOWN_ARC  ,A,B,DB_Point(R.x1,R.y2),r,CP,flag,bk_Rid,dis_CPP);//7.����Բ��
			judge_brick_arc(Rid,RIGHT_DOWN_ARC ,A,B,DB_Point(R.x2,R.y2),r,CP,flag,bk_Rid,dis_CPP);//8.����Բ��
		}
		inline int judge_brick(DB_Point &st,DB_Point &ed,Brick *brick,Re &bk_cnt){//����ײ��⡿�ж���ש�����ײ���
			if(st.y-r>brickbox.y2&&ed.y-r>brickbox.y2)return 0;//С��֦�������û���������
			
			//for(Re i=1;i<=Rid_tail;++i)Rid_stack_pan[Rid_stack[i]]=0;
			memset(Rid_stack_pan,0,sizeof(Rid_stack_pan));
			Rid_tail=0,find_Rid_area(st,ed,brick);

			Re minRid=1000000000,maxRid=-1000000000; DB_Point CP;Re flag=0,bk_Rid=0;LD dis_CPP=2e9;
			for(Re i=1;i<=Rid_tail;++i)
				judge_brick_(brick[Rid_stack[i]].Rid,st,ed,CP,flag,bk_Rid,dis_CPP);
			if(flag){//������ײ
//if(brick[1].styletype>1)
	//printdebug(_T("FuckYOU!!!!!!!!!!!!!!")),system("pause"); 
//CH say[100];_stprintf(say,_T("��ײ��%d"),bk_Rid),printdebug(say); system("pause");
				ed=CP,reverse(flag);//�ı��˶��켣
				
				//ש���ܵ�����
				print_background();//ש��仯ǰ�ȿٵ�С��
				brick[bk_Rid].vanish();//ש��仯
				playgame_ball_getimage();//���»�ȡС�򱳾�ͼ��
				print();//�������С��

				bk_cnt-=(brick[bk_Rid].styletype==0);//ש��ȫ������ͨ����
				return !bk_cnt?-100:flag;
			}
			return 0;//δ������ײ
		}

		inline char judge_collide(Player &player,Range R1,DB_Point &st,DB_Point &ed,Brick *brick,Re &bk_cnt){
			if(dcmp(ed.y-R1.y2)>=0){ ed.y=R1.y2;return 'D'; }//���¡�������
			int cmd_player=judge_player(player,st,ed);//�ж�����ҵ���ײ���
			//if(!cmd_player){//�ж���ש�����ײ���
				Re cmd_brick=judge_brick(st,ed,brick,bk_cnt);
				if(cmd_brick==-100)return 'P';//ש��ȫ������ͨ�ء�
				if(cmd_brick!=0)return 'B';//ײש��
			//}
			judge_wall(player,R1,st,ed);//�ж�������ǽ����ײ���
			return 0;
		}

		inline void change_speed(char cmd){//�ı�С���ٶ�
			if(cmd=='F'&&dcmp(speed-3.0)<0)calc_DB_Dir(speed+0.5);
			if(cmd=='S'&&dcmp(speed-0.5)>0)calc_DB_Dir(speed-0.5);
		}

		inline char updata(Player &player,Range R0,Brick *brick,Re &bk_cnt){//����С��λ��
			DB_Point st=DB_P;DB_P+=DB_Dir;

			char cmd=judge_collide(player,R0.cutmid(r,r),st,DB_P,brick,bk_cnt);//����ײ��⡿
			Point P_=Point(int(DB_P.x+eps),int(DB_P.y+eps));
			if(P_!=P)refresh(P_);//����ͼ��

			if(cmd=='D')return 'D';//��������
			if(cmd=='P')return 'P';//��ͨ�ء�

			return 0;
		}
	};

	inline void getball_1(){
		IMAGE img;
		loadimage(&img,_T("./resource/image/ball/ball.png"));
		putimage(0,0,img);
		style[1].getstyle(1);
	}

	inline void init(){
_Begin();
		getball_1();
		style[2].getstyle(2);
		style[3].getstyle(3);
_End(),_Flush();
	}
}
using BALLINFO::Ball;

namespace OPTION{
	struct Option{//ѡ��
		bool msin;//ѡ��״̬(����Ƿ�������ײ������)
		Range box;IMAGE bkimg;//��ײ�䡢ѡ���
		int height,weight;STR text;//����
		int line_weight;//�߿���
		COLOR line_color[2],text_color[2];

		Option(Range R={0,0,0,0},Re H=0,Re W=0,STR T=_T(""),
				COLOR line_color_0=default_option_line_color[0],COLOR line_color_1=default_option_line_color[1],
				COLOR text_color_0=default_option_text_color[0],COLOR text_color_1=default_option_text_color[1],
				Re line_W=3){//���캯��
			line_color[0]=line_color_0,line_color[1]=line_color_1;
			text_color[0]=text_color_0,text_color[1]=text_color_1;
			box=R,height=H,weight=W,text=T,line_weight=line_W,msin=0;
			bkimg=H?getimage(box.boost(1,1)):IMAGE(0,0);
		}

		inline void refresh(){//ˢ��ѡ��
			Setbkmode(bkmode2),ClearRange(box.boost(1,1),bkimg);//���û�ͼ͸������ѡ�������ÿ�
			drawrange(box,line_color[msin],line_weight);//���߿�
			Settextstyle(height,weight,text_color[msin],0,msin),print(box,text);//������
		}

		inline bool updata_action(MOUSE m){//���ѡ��״̬
			Point ms=Point(m.x,m.y);
			if(PIR(ms,box)^msin)msin^=1,refresh();//��ѡ��洢��״̬�뵱ǰ״̬��ͬ����ˢ��״̬
			return (msin&&m.ms_info==ms_lefthit);//�������ѡ��
		}
	};
	struct Option_img{//ѡ��(��ͼ��)
		bool msin;//ѡ��״̬(����Ƿ�������ײ������)
		Range box;IMAGE img0,img1;//��ײ�䡢��ͼ��ѡ���

		Option_img(Range R={0,0,0,0},IMAGE img0_=IMAGE(0,0),IMAGE img1_=IMAGE(0,0)){//���캯��
			img0=img0_,img1=img1_,box=R,msin=0;
		}
		inline void refresh(){ putimage_transparancy(box.x1,box.y1,msin?&img1:&img0); }//ˢ��ѡ��
		inline bool updata_action(MOUSE m){//���ѡ��״̬
			Point ms=Point(m.x,m.y);
			if(PIR(ms,box)^msin)msin^=1,refresh();//��ѡ��洢��״̬�뵱ǰ״̬��ͬ����ˢ��״̬
			return (msin&&m.ms_info==ms_lefthit);//�������ѡ��
		}
	};
	struct Option_bkimg_text{//ѡ��
		bool msin;//ѡ��״̬(����Ƿ�������ײ������)
		Range box;IMAGE bkimg;//��ײ�䡢ѡ���
		int height,weight;STR text;//����
		IMAGE bkimg0,bkimg1;//������ͼ
		COLOR text_color[2];

		Option_bkimg_text(Range R={0,0,0,0},Re H=0,Re W=0,STR T=_T(""),
				IMAGE img0_=IMAGE(0,0),IMAGE img1_=IMAGE(0,0),
				COLOR text_color_0=default_option_text_color[0],COLOR text_color_1=default_option_text_color[1]
				){//���캯��
			bkimg0=img0_,bkimg1=img1_;
			text_color[0]=text_color_0,text_color[1]=text_color_1;
			box=R,height=H,weight=W,text=T,msin=0;
			bkimg=H?getimage(box):IMAGE(0,0);
		}

		inline void refresh(){//ˢ��ѡ��
			Setbkmode(bkmode2),ClearRange(box,bkimg);//���û�ͼ͸������ѡ�������ÿ�
			putimage_transparancy(box.x1,box.y1,msin?&bkimg1:&bkimg0);//������
			Settextstyle(height,weight,text_color[msin],0,msin),print(box,text);//������
		}

		inline bool updata_action(MOUSE m){//���ѡ��״̬
			Point ms=Point(m.x,m.y);
			if(PIR(ms,box)^msin)msin^=1,refresh();//��ѡ��洢��״̬�뵱ǰ״̬��ͬ����ˢ��״̬
			return (msin&&m.ms_info==ms_lefthit);//�������ѡ��
		}
	};
	struct Level_option{//��ѡ��ؿ���ѡ��
		Option_bkimg_text option;CH text[100];
		int levelid,levelopen;//�ùؿ��Ƿ��Ѿ�ͨ��(level�޶�)
		
		inline void refresh(){ option.refresh(); }//ˢ��ѡ��
		inline bool updata_action(MOUSE m){ return option.updata_action(m); }//���ѡ��״̬
		inline void get_option_img_and_color(IMAGE img[],COLOR text_color[]){
			if(levelopen)
				img[0]=option_openedlevel_img[0],img[1]=option_openedlevel_img[1],
				text_color[0]=level_opened_option_text_color[0],text_color[1]=level_opened_option_text_color[1];
			else
				img[0]=option_closedlevel_img[0],img[1]=option_closedlevel_img[1],
				text_color[0]=level_closed_option_text_color[0],text_color[1]=level_closed_option_text_color[1];
		}
		inline void open(){
			levelopen=1;
			IMAGE bkimg[2];COLOR text_color[2]; get_option_img_and_color(bkimg,text_color);
			option.bkimg0=bkimg[0],option.bkimg1=bkimg[1];
			option.text_color[0]=text_color[0],option.text_color[1]=text_color[1];
		}
		inline void init(Range R,Re id){
			levelid=id;
			if(id<=common_level_cnt)_stprintf(text,_T("Level %d"),id);
			else if(id==level_endless)_stprintf(text,_T("�޾�ģʽ"));
			else _stprintf(text,_T("��������"));

			levelopen=(id==1);//�ؿ�ȫ��ģʽ
			if(levelid>common_level_cnt)levelopen=0;//�ؿ���δ����

			IMAGE bkimg[2];COLOR text_color[2]; get_option_img_and_color(bkimg,text_color);
			option=Option_bkimg_text(R,20,200,text,bkimg[0],bkimg[1],text_color[0],text_color[1]);
		}
	};
}
using OPTION::Option;
using OPTION::Option_img;
using OPTION::Level_option;

namespace GAMERUNNING{//����Ϸ�����С�

	int ballstyle,leveltype;
	int HP;BALLINFO::Ball HPB[MAX_HP_cnt+1];
	Option_img HParea,order;//Ѫ���򡢲˵�

	Player game_player;
	Ball game_ball;
	
	int game_brick_cnt;
	Brick game_brick[MAX_brick_cnt+1];

	inline void refresh_debug_level_info(){//ˢ��debug����ؿ���Ϣ
		if(leveltype==level_endless)printdebug(_T("���޾�ģʽ��"));
		else{ CH say[100];_stprintf(say,_T("��Level %d��"),leveltype),printdebug(say); }
	}
	inline void refresh(){//����Ϸ�����С�ˢ�½���

		Setmain(mainbox,bkimg_up);//��Ϸ����������
		Setdebug(debugbox,bkimg_down,30,100,default_debug_text_color);//��Ϸ�����������

		refresh_debug_level_info();

		order.msin=0,order.refresh();
		//putimage(7,1,option_order_img[0]);
		putimage_transparancy(HParea_box.x1,HParea_box.y1,&background_HParea_img);
		//HParea.msin=0,HParea.refresh();
		//drawrange(HPbox,default_option_line_color[0],2);
		for(Re i=1,j=0;i<=HP;++i,++j)HPB[i].init_refresh();
	}

	struct Gamerunning_Debuginfo{//����Ϸ�����С�������Ϣ

		LD Max_delaytime,Min_delaytime;

		inline void init(){
			Max_delaytime=-1,Min_delaytime=1e8;
		}
		inline void coordinate(){//��Debug��С�����꣬�������
			CH say[100];_stprintf(say,_T("С��:(%.2lf,%.2lf) ���:(%d)"),game_ball.DB_P.x,game_ball.DB_P.y,game_player.mid_x);
			printdebug(say,__debug_color,25,700);
		}
		inline void delaytime(LD t){
			Max_delaytime=max(Max_delaytime,t),Min_delaytime=min(Min_delaytime,t);
			//if(t*1000>35)system("pause");
			CH say[100];_stprintf(say,_T("��ǰ:%.1lfms, ��С:%.1lfms, ���:%.1lfms"),t*1000,Min_delaytime*1000,Max_delaytime*1000);
			printdebug(say,__debug_color,21,700);
		}
		inline void brick_styletype(){
			CH say[1000];memset(say,0,sizeof(say));
			for(Re i=1;i<=10;++i){
				_stprintf(say+lstrlen(say),_T("%d,"),game_brick[i].styletype);
				//if(game_brick[i].styletype>1)system("pause");
			}
			printdebug(say,__debug_color,11,700);
		}
	}debug;

	inline void dead_anime(Re HP){//������������
		for(Re O=1;O<=5;++O){//��˸����
			HPB[HP].print_background(),game_ball.print_background();
			print({0,224,480-1,424-1},_T("��Oh yeah~ You dead!!!��"),red,40,200);
			Sleep(200);
			
			HPB[HP].print(),game_ball.print();
			print({0,224,480-1,424-1},_T("��Oh yeah~ You dead!!!��"),darkgray,40,200);
			Sleep(200);
		}
	}
	inline void pass_anime(){//��ͨ�ض�����
		for(Re O=1;O<=5;++O){//��˸����
			print({0,224,480-1,324-1},_T("��Oh Noooo! You have passed! ��"),red,30,200);
			print({0,324,480-1,424-1},_T("��Why?! I can't agree that!!!��"),red,30,200);
			Sleep(200);

			print({0,224,480-1,324-1},_T("��Oh Noooo! You have passed! ��"),darkgray,30,200);
			print({0,324,480-1,424-1},_T("��Why?! I can't agree that!!!��"),darkgray,30,200);
			Sleep(200);
		}
	}

	inline void game_pause(){//����Ϸ��ͣ��
		printdebug(_T("����Ϸ��ͣ������������Լ���..."),white,28,100);
		system("pause"),refresh_debug_level_info();
	}
	inline int G_controller(){//�����Ҳ������桿
		while(1){
			if(_kbhit()){
				char c=_getch();
				if(c==kb_esc||c==kb_G)return 0;//����[ESC]��[G]���޲���
				else if(c==kb_P)return 'P';//��[P]���ң�ֱ�ӡ����ء�
				else if(c==kb_D)return 'D';//��[D]���ң�ֱ�ӡ�������
				else if(c==kb_L){ game_player.updata(game_ball.DB_P,game_ball.r,game_player.mid_x,game_player.lenth*2); return 0; }//��[L]���ң��õ����ߡ��쳤��
				else if(c==kb_F){ game_ball.change_speed('F'); return 0; }//��[F]���ң��õ�buff fast�����١�
				else if(c==kb_S){ game_ball.change_speed('S'); return 0; }//��[S]���ң��õ�buff slow�����١�
			}
			Sleep(20);
		}
	}

	inline char updata(Re flag){//����Ϸ�����С������Ϊ
		if(_kbhit()){
			char c=_getch();
			if(c==kb_esc)return 'E';//����[ESC]
			else if(c==kb_space)return 'S';//��space����ͣ��
			else if(c==kb_P)return 'P';//��[P]���ң�ֱ�ӡ����ء�
			else if(c==kb_D)return 'D';//��[D]���ң�ֱ�ӡ�������
			else if(c==kb_G)return 'G';//��[G]���ң����롾���Ҳ������桿
		}
		if(flag){//�ڡ�׼�����䡿�׶�
			if(_mshit()){
				MOUSE m=_getms();
				if(order.updata_action(m))return 'E';//���¡��˵���
				//if(m.ms_info==ms_righthit)return '';
				if(PIR({m.x,m.y},Range(gamebox.x1+1,gamebox.y1+1,gamebox.x2-1,game_ball.P.y-1))&&m.ms_info==ms_lefthit){
					game_ball.start_biu({m.x,m.y});return 'B';//�����䡿
				}
			}
			return 0;
		}
		MOUSE m;Re Cnt=0;
		while(_mshit()){
			m=_getms(),++Cnt;
			if(order.updata_action(m))return 'E';
		}
		if(Cnt){
			int player_cmd=game_player.updata(game_ball.DB_P,game_ball.r,m.x);//�������λ��
			if(player_cmd)game_ball.reverse(player_cmd);//�����������ײ���ı��˶��켣
		}
		char cmd=game_ball.updata(game_player,gamebox.cutmid(1,1),game_brick,game_brick_cnt);//����С��λ��
		if(cmd=='D')return 'D';//��������
		if(cmd=='P')return 'P';//��ͨ�ء�
		_Flushms();
		return 0;
	}
	inline char playgame_(Re levelid){//����Ϸ�����С�

		refresh();
		
		for(int i=1;i<=MAX_brick_cnt;++i)game_brick[i].print();

		game_player.playgame_init();
		
		game_ball=Ball({game_player.mid_x,game_player.R.y1-BALLINFO::style[2].r-1},2);
//game_ball=Ball({game_player.mid_x,game_player.R.y1-BALLINFO::style[3].r-1},3); (����ģʽ��������)
		game_ball.init_refresh();
		
		debug.init();

		while(1){//׼������
			char cmd=updata(1);
			if(cmd=='E')return 'E';//����[ESC]����[�˵�]
			else if(cmd=='B')break;//�����䡿
			else if(cmd=='P')return 'P';//��[P]���ң�ֱ�ӡ����ء�
			else if(cmd=='D')return 'D';//��[D]���ң�ֱ�ӡ�������
			Sleep(1);
		}
		LD temp=gettime(),tp=gettime();
		while(1){
			LD nowt=gettime();
			if((nowt-tp)*1000<15)continue;
			else tp=nowt;
			char cmd=updata(0);
			if(cmd=='E')return 'E';//refresh();//����[ESC]����[�˵�]
			else if(cmd=='S')game_pause();//��space����ͣ��
			else if(cmd=='D')return 'D';//��Ȼ���� �� ��[D]���ҡ�������;
			else if(cmd=='P')return 'P';//��[P]���ң�ֱ�ӡ����ء�
			else if(cmd=='G'){//��G���뿪�Ҳ�������
				int G_cmd=G_controller();
				if(G_cmd=='P')return 'P';//��[P]���ң�ֱ�ӡ����ء�
				else if(G_cmd=='D')return 'D';//��[D]���ң�ֱ�ӡ�������
			}
debug.delaytime(gettime()-temp);temp=gettime();
//debug.coordinate();
//debug.brick_styletype();
		}
	}
	inline int playgame_main(Re levelid){//����Ϸ�����С�������
		refresh();

		HP=Level[leveltype=levelid].HP_cnt;

		game_brick_cnt=Level[levelid].brick_cnt;
		for(int i=1;i<=MAX_brick_cnt;++i)
			game_brick[i].playgame_main_init(i,Level[levelid].brick_style[i]);

		while(HP){
			Re cmd=playgame_(levelid);
			if(cmd=='E')return 0;//����[ESC]����[�˵�] ��δͨ�ء�
			else if(cmd=='P'){ pass_anime(); return 1; }//��ͨ�ء�
			else if(cmd=='D')dead_anime(HP),--HP;//���š�����������
		}
		return 0;//��δͨ�ء�
	}

	inline void init(){

		Setmain(mainbox,bkimg_up);//��Ϸ����������
		Setdebug(debugbox,bkimg_down,30,100,default_debug_text_color);//��Ϸ�����������

		BALLINFO::init();

		//HPbox=HParea_box;//{0 +7,0 +1,23*MAX_HP_cnt-1 +7,22 +1}
		for(Re i=1,j=0;i<=MAX_HP_cnt;++i,++j)HPB[i]=BALLINFO::Ball({23*j+12-1 +7,12-1 +1},2);

		Setmain(mainbox,bkimg_up_option);//ѡ�����������
		Setdebug(debugbox,bkimg_down_option,30,100,default_debug_text_color);//ѡ������������

		//HParea=Option_img(HParea_box,background_HParea_img,background_HParea_img);
		//order=Option({244,1,473,23},16,500,_T("���˵���"));//{480-230+1 -7,0 +1,480 -7,23-1 +1}
		order=Option_img(order_box,option_order_img[0],option_order_img[1]);
		//{0,0,229,22}
	}
}

namespace LEVELMAKER{
	int leveltype;
	int HP;Range HPbox;BALLINFO::Ball HPB[MAX_HP_cnt+1];
	Option order;//�˵�
	inline void open_file_write(int id){
		freopen("CON","r",stdin); freopen("CON","w",stdout);
		std::string _id; std::stringstream ss; ss<<id; ss>>_id;
		std::string filepath="./data/level/level_"+_id+".txt";
		freopen(filepath.c_str(),"w",stdout);
	}
	inline void close_file(){ freopen("CON","r",stdin); freopen("CON","w",stdout); }
	inline void controler_savefile(Brick brick[]){//��������ģʽ�������ļ�
		open_file_write(leveltype);
		for(Re i=1;i<=16;++i,puts(""))
			for(Re j=1;j<=10;++j)
				putchar(brick[(i-1)*10+j].styletype+'0');
		close_file();
	}

	inline void refresh_debug_level_info(){//ˢ��debug����ؿ���Ϣ
		if(leveltype==level_endless)printdebug(_T("���޾�ģʽ��������ģʽ"));
		else{ CH say[100];_stprintf(say,_T("��Level %d��������ģʽ"),leveltype),printdebug(say); }
	}
	inline void refresh(){//����Ϸ�����С�ˢ�½���

		Setmain(mainbox,bkimg_up);//��Ϸ����������
		Setdebug(debugbox,bkimg_down,30,100,default_debug_text_color);//��Ϸ�����������

		refresh_debug_level_info();

		order.msin=0,order.refresh();

		drawrange(HPbox,default_option_line_color[0],2);
		for(Re i=1,j=0;i<=HP;++i,++j)HPB[i].init_refresh();
	}

	Brick brick[MAX_brick_cnt+1];
	inline char updata(){//����Ϸ�����С������Ϊ
		if(_kbhit()){
			char c=_getch();
			if(c==kb_esc)return 'E';//����[ESC]
			else if(c==kb_S)return 'S';//����[S]�����ļ�
		}
		MOUSE m;Re Cnt=0;
		while(_mshit()){
			m=_getms(),++Cnt;
			if(order.updata_action(m))return 'E';
			if(m.ms_info==ms_lefthit){
				Re id=map_to_id[m.x][m.y];
				if(id)brick[id].controler_levelmaker_next();
			}
			if(m.ms_info==ms_righthit){
				Re id=map_to_id[m.x][m.y];
				if(id)brick[id].controler_levelmaker_pre();
			}
		}

		//_Flushms();
		return 0;
	}
	inline void controler_levelmaker(Re levelid){//��������ģʽ���ؿ�����
		leveltype=levelid;
		refresh();

		HP=3;

		for(Re i=1;i<=MAX_brick_cnt;++i)brick[i].controler_levelmaker_init(i);

		while(1){
			Re cmd=updata();
			if(cmd=='E')return;//����[ESC]����[�˵�]
			else if(cmd=='S'){ controler_savefile(brick);return; }//����[S]�����ļ�
		}
		return;
	}
	inline void init(){

		Setmain(mainbox,bkimg_up);//��Ϸ����������
		Setdebug(debugbox,bkimg_down,30,100,default_debug_text_color);//��Ϸ�����������
		
		HPbox={0 +7,0 +1,23*MAX_HP_cnt-1 +7,22 +1};
		for(Re i=1,j=0;i<=MAX_HP_cnt;++i,++j)HPB[i]=BALLINFO::Ball({23*j+12-1 +7,12-1 +1},2);

		Setmain(mainbox,bkimg_up_option);//ѡ�����������
		Setdebug(debugbox,bkimg_down_option,30,100,default_debug_text_color);//ѡ������������

		order=Option({480-230+1 -7,0 +1,480 -7,23-1 +1},16,500,_T("���˵���"));
	}
}

namespace CHOOSELEVEL{//��ѡ��ؿ���

	Option_img returnback;
	Level_option level_option[MAX_level_cnt+1];

	inline void refresh(){//��ѡ��ؿ���ˢ�½���

		Setmain(mainbox,bkimg_up);//��Ϸ����������
		Setdebug(debugbox,bkimg_down,30,100,default_debug_text_color);//��Ϸ�����������

		printdebug(_T("��ѡ��ؿ���"));

		returnback.msin=0,returnback.refresh();
_Begin();
		for(Re i=1;i<=MAX_level_cnt;++i)level_option[i].option.msin=0,level_option[i].refresh();
_End(),_Flush();
	}

	inline int updata(){//��ѡ��ؿ��������Ϊ
		if(_kbhit()){
			char c=_getch();
			if(c==kb_esc)return -1;
			if(c=='O')return -1000+'O';//��[O]���ҡ��ؿ�ȫ����
		}
		if(_mshit()){
			MOUSE m=_getms();
			if(returnback.updata_action(m))return -1;
			for(Re i=1;i<=MAX_level_cnt;++i)if(level_option[i].updata_action(m))return i;
		}
		return 0;
	}

	inline void level_allopen(){
		for(Re i=1;i<=common_level_cnt;++i)level_option[i].open();
	}

	inline void ChooseLevel(){//��ѡ��ؿ���������
		Re warning_showtime=0,warning_type=-1;refresh();
		while(1){
			Re cmd=updata();
			if(cmd==-1)return;//����[ESC]����[���ؿ�ʼ����]
			if(cmd==-1000+'O')controlermode_level_allopen=1,CHOOSELEVEL::level_allopen(),refresh();//��[O]���ҡ��ؿ�ȫ����
			if(cmd>=1){
				if(controlermode_make_level)LEVELMAKER::controler_levelmaker(cmd),refresh();//��������ģʽ���ؿ�����
				else if(level_option[cmd].levelopen){//����[level *]
					Re cmd_pass=GAMERUNNING::playgame_main(cmd);
					if(cmd_pass)level_option[min(cmd+1,common_level_cnt)].open();
					warning_showtime=0,refresh();
				}
				else warning_showtime=(2000*10),warning_type=-1;//��㵽�˲��õ�Ķ���QAQ
			}
			if(warning_showtime){
				Re type=warning_showtime/(250*10)&1;//Լÿ0.25s����һ����ɫ
				if(warning_type!=type)printdebug(_T("��FBI Warning���������ǰһ�ء�"),(warning_type=type)?lightred:lightgray,25,200);
				if(!--warning_showtime)printdebug(_T("��ѡ��ؿ���"));//Լ2s���ٷ���Warning
			}
		}
	}

	inline void init(){//��ѡ��ؿ�����ʼ��
		Setmain(mainbox,bkimg_up_option);//ѡ�����������
		Setdebug(debugbox,bkimg_down_option,30,100,default_debug_text_color);//ѡ������������

		returnback=Option_img(returnback_box,option_returnback_img[0],option_returnback_img[1]);

		Re stx=35+10,sty=35+10;
		for(Re i=1;i<=MAX_level_cnt;++i){
			level_option[i].init({stx,sty,stx+90,sty+30},i);//{0,0,90,30}
			stx+=100;if(stx==445)stx=45,sty+=40;
		}
	}
}
using CHOOSELEVEL::ChooseLevel;

namespace GAMEINFO{//����Ϸ˵����

	Range textbox;//����
	Option_img returnback;//���ؿ�ʼ����
	STR gameinfo_text;

	inline void refresh(){//����Ϸ˵����ˢ�½���
		Setmain(mainbox,bkimg_up);//��Ϸ����������
		Setdebug(debugbox,bkimg_down,30,100,default_debug_text_color);//��Ϸ�����������
		
		printdebug(_T("����Ϸ˵����ʩ���У���"));

		returnback.msin=0,returnback.refresh();

		print(textbox,gameinfo_text,lightred,20,100,2);
	}

	inline int updata(){//����Ϸ˵���������Ϊ
		if(_kbhit()){
			char c=_getch();
			if(c==kb_esc)return -1;
		}
		if(_mshit()){
			MOUSE m=_getms();
			if(returnback.updata_action(m))return -1;
		}
		return 0;
	}

	inline void GameInfo(){//����Ϸ˵����������
		refresh();
		cmd_openfile("\\��Ϸ˵��.pdf");
		while(1){
			Re cmd=updata();
			if(cmd==-1)return;//����[ESC]����[���ؿ�ʼ����]
		}
	}

	inline void init(){//����Ϸ˵������ʼ��
		Setmain(mainbox,bkimg_up);//��Ϸ����������
		Setdebug(debugbox,bkimg_down,30,100,default_debug_text_color);//��Ϸ�����������
		
		textbox=gameinfo_text_box;
		gameinfo_text=_T("");

		Setmain(mainbox,bkimg_up_option);//ѡ�����������
		Setdebug(debugbox,bkimg_down_option,30,100,default_debug_text_color);//ѡ������������

		returnback=Option_img(returnback_box,option_returnback_img[0],option_returnback_img[1]);
		//returnback=Option({110,625,370,675},30,80,_T("���ؿ�ʼ����"));
		//{0,0,260,50}
	}
}
using GAMEINFO::GameInfo;

namespace STARTGRAPH{//����ʼ���桿

	Range Titlebox;//�����
	const int Title_shadow=5;
	//Range Staffbox;IMAGE Staff_bkimg;//����
	
	Option_img gamestart,gameinfo;

	inline void refresh(){//����ʼ���桿ˢ�½���

		Setmain(mainbox,bkimg_up_st);//��ʼ����������
		Setdebug(debugbox,bkimg_down_st,30,100,default_debug_text_color);//��ʼ�����������

		printdebug(_T("����ʼ���桿"));

		putimage_transparancy(Titlebox.x1,Titlebox.y1,&text_Title_img);
		//Setbkmode(bkmode2);//͸������
		//Settextstyle(140,1000,Title_textshadow_color);
		//print(Titlebox.transfer(+Title_shadow),_T("��ש��"));
		//Settextstyle(140,1000,Title_text_color);
		//print(Titlebox,_T("��ש��"));

		//putimage(Staffbox.x1,Staffbox.y1,Staff_bkimg);
		//Setbkmode(bkmode2);//͸������
		//Settextstyle(20,100,default_Staff_text_color,0,1);
		//print(Staffbox,_T("���ߣ������� (Xing_Ling)"));

		gamestart.msin=0,gamestart.refresh();
		gameinfo.msin=0,gameinfo.refresh();
	}

	inline int updata(){//����ʼ���桿�����Ϊ
		if(_kbhit()){
			char c=_getch();
			if(c=='M')return 'M';//��������ģʽ���ؿ�����
			if(c=='O')return 'O';//��[O]���ҡ��ؿ�ȫ����
			if(c==kb_esc)return -1;
		}
		if(_mshit()){
			MOUSE m=_getms();
			if(gamestart.updata_action(m))return 1;
			if(gameinfo.updata_action(m))return 2;
		}
		return 0;
	}

	inline void StartGraph(){//����ʼ���桿������
		refresh();

		while(1){
			Re cmd=updata();
			if(cmd==-1)return;//����[ESC]
			if(cmd==1)ChooseLevel(),refresh();//����[��ʼ��Ϸ]
			if(cmd==2)GameInfo(),refresh();//����[��Ϸ˵��]
			if(cmd=='M')controlermode_make_level=1;//��������ģʽ���ؿ�����
			if(cmd=='O')controlermode_level_allopen=1,CHOOSELEVEL::level_allopen(),refresh();//��[O]���ҡ��ؿ�ȫ����
			Sleep(1);
		}
	}

	inline void init(){//����ʼ���桿��ʼ��

		//Setmain(mainbox,bkimg_up);//��Ϸ����������
		//Setdebug(debugbox,bkimg_down,30,100,default_debug_text_color);//��Ϸ�����������

		Titlebox={30,100,450,280}; //Title_bkimg=getimage(Titlebox);
		//{0,0,420,180}
		//Staffbox={30,340,450,370}; //Staff_bkimg=getimage(Staffbox);

		//Setmain(mainbox,bkimg_up_st);//��ʼ����������
		//Setdebug(debugbox,bkimg_down_st,30,100,default_debug_text_color);//��ʼ�����������

		Setmain(mainbox,bkimg_up_option);//ѡ�����������
		Setdebug(debugbox,bkimg_down_option,30,100,default_debug_text_color);//ѡ������������

		gamestart=Option_img(gamestart_box,option_gamestart_img[0],option_gamestart_img[1]);//{0,0,290,80}
		gameinfo=Option_img(gameinfo_box,option_gameinfo_img[0],option_gameinfo_img[1]);//{0,0,290,80}
		//gamestart=Option({95,480,385,560},50,100,_T("��ʼ��Ϸ"));
		//gameinfo=Option({95,610,385,690},50,100,_T("��Ϸ˵��"));
	}
}
using STARTGRAPH::StartGraph;

inline void init(){

	Setgraphsize(480,820);
	SetWindowstext(_T("�����ߣ�������/Xing_Ling����ש�� "));

	loadimage(&bkimg_up,_T("./resource/image/background/background_up.jpg"));
	loadimage(&bkimg_down,_T("./resource/image/background/background_down.jpg"));
	loadimage(&bkimg_up_st,_T("./resource/image/background/background_up_st.jpg"));
	loadimage(&bkimg_down_st,_T("./resource/image/background/background_down_st.jpg"));
	loadimage(&bkimg_up_option,_T("./resource/image/background/background_up_option.jpg"));
	loadimage(&bkimg_down_option,_T("./resource/image/background/background_down_option.jpg"));
	loadimage(&option_openedlevel_img[0],_T("./resource/image/option/openedlevel.png"));
	loadimage(&option_openedlevel_img[1],_T("./resource/image/option/openedlevel_.png"));
	loadimage(&option_closedlevel_img[0],_T("./resource/image/option/closedlevel.png"));
	loadimage(&option_closedlevel_img[1],_T("./resource/image/option/closedlevel_.png"));
	loadimage(&option_returnback_img[0],_T("./resource/image/option/returnback.png"));
	loadimage(&option_returnback_img[1],_T("./resource/image/option/returnback_.png"));
	loadimage(&option_gamestart_img[0],_T("./resource/image/option/gamestart.png"));
	loadimage(&option_gamestart_img[1],_T("./resource/image/option/gamestart_.png"));
	loadimage(&option_gameinfo_img[0],_T("./resource/image/option/gameinfo.png"));
	loadimage(&option_gameinfo_img[1],_T("./resource/image/option/gameinfo_.png"));
	loadimage(&background_HParea_img,_T("./resource/image/background/HParea.png"));
	loadimage(&option_order_img[0],_T("./resource/image/option/order.png"));
	loadimage(&option_order_img[1],_T("./resource/image/option/order_.png"));
	loadimage(&text_Title_img,_T("./resource/image/text/title.png"));

	//press_kb_down(kb_caps_lock);

	CONTROLER::init();
	STARTGRAPH::init();
	CHOOSELEVEL::init();
	GAMEINFO::init();
	GAMERUNNING::init();
	LEVELMAKER::init();
	LEVELINFO::init();
	//BALLINFO::init();//��GAMERUNNING::init();���Ѿ�����
	BRICKINFO::init();
	PLAYERINFO::init();
	
}
int main(){ init(),StartGraph(),CloseProgram(); }