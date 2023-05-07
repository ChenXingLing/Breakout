/*
----------------------------------------------------------------------------------
            作者：辰星凌 (Xing_Ling)                                               
            Blog 1：www.cnblog.com/Xing-Ling                                      
			Blog 2：www.luogu.cn.com/ChenXingLing/#/                              
----------------------------------------------------------------------------------
*/

#include "myprepare.h"

IMAGE bkimg_up,bkimg_down;//游戏界面大背景
IMAGE bkimg_up_st,bkimg_down_st;//开始界面大背景
IMAGE bkimg_up_option,bkimg_down_option;//选项框内大背景
IMAGE option_openedlevel_img[2];//[已开放关卡]选项
IMAGE option_closedlevel_img[2];//[未开放关卡]选项
IMAGE option_returnback_img[2];//[返回开始界面]选项
IMAGE option_gamestart_img[2];//[开始游戏]选项
IMAGE option_gameinfo_img[2];//[游戏说明]选项
IMAGE option_order_img[2];//[菜单]选项
IMAGE text_Title_img;//大标题文字
IMAGE background_HParea_img;//血量区域背景
IMAGE tempppp;

const int MAX_HP_cnt=10;//最大HP值
const int MAX_level_cnt=56;//游戏关卡数量(最多为56)
const int common_level_cnt=18;//普通关卡数量
const int playerstyle_cnt=1;//玩家样式数量
const int brickstyle_cnt=5;//砖块样式数量
const int ballstyle_cnt=3;//小球样式数量
const int MAX_brick_cnt=160;//最大砖块个数 brick_size: 25*40 MAX_brick_cnt:16*10
const int MAX_ball_cnt=5;//最大小球数量

const COLOR brick_style_color     [brickstyle_cnt+1]={0,lightgreen, lightblue, lightpurple, orange, darkpurple};//砖块填充颜色（若干style）
const COLOR brick_style_color_line[brickstyle_cnt+1]={0,brown     , brown    , brown      , brown , brown     };//砖块边框颜色（若干style）
const COLOR brick_shadow_color=darkgray;//砖块阴影颜色
const COLOR player_style_color     [playerstyle_cnt+1]={0,pink };//玩家填充颜色（若干style）
const COLOR player_style_color_line[playerstyle_cnt+1]={0,brown};//玩家边框颜色（若干style）
const COLOR player_shadow_color=darkgray;//玩家阴影颜色
const COLOR default_option_line_color[2]={darkgray,lightgray};//默认选项边框颜色（正常状态 与 鼠标指向状态）
const COLOR default_option_text_color[2]={lightcyan,white};//默认选项文字颜色（正常状态 与 鼠标指向状态）
const COLOR level_opened_option_text_color[2]={RGB(0,255,30),white};//已开放关卡选项文字颜色（正常状态 与 鼠标指向状态）
const COLOR level_closed_option_text_color[2]={lightgray,white};//未开放关卡选项文字颜色（正常状态 与 鼠标指向状态）
const COLOR default_debug_text_color=white;//默认debug区域文字颜色
//const COLOR Title_text_color=lightcyan;//大标题文字颜色
//const COLOR Title_textshadow_color=cyan;//大标题文字阴影颜色
//const COLOR Staff_text_color=cyan;

#define level_endless MAX_level_cnt //无尽模式关卡

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
	const Range gamebox={0,24,480-1,720-1};//小球运动区域
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
			if(type==1){};//type1: 1级砖块，一次碰坏
		}
	}style[brickstyle_cnt+1];
	
	Range id_to_R[MAX_brick_cnt+1];
	int map_to_id[480][720];

	struct Brick{
		//styletype=0: 空气
		//styletype=1: 一级砖块
		int styletype;int Rid;
		IMAGE background_img;
		inline void print(){
			if(!styletype)return;
			drawrange(id_to_R[Rid].cutmid(1,1),brick_style_color[styletype],brick_shadow_color,2);//画阴影
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

namespace LEVELINFO{//【关卡信息】
	
	struct LEVEL{
		int type;//关卡编号
		int HP_cnt,brick_cnt,brick_style[MAX_brick_cnt+1];
		
		inline void init_endless(){//无尽模式
			
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
	inline void init(){//【关卡信息】初始化 从文件中读取地图
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
			drawrange(R.cutmid(1,1),player_style_color[styletype],player_shadow_color,2);//画阴影
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
//R.y1=500-207,R.y2=500+207; (调试模式：超高玩家)
			R.x1=mid_x-lenth,R.x2=mid_x+lenth;
			center_L=mid_x-lenth*player_center_range,center_R=mid_x+lenth*player_center_range;

			background_img=getimage(R);

			print();
		}

		inline int updata(DB_Point &DB_P,Re r,Re m,Re len=0){//更新玩家位置,len>0:更改玩家长度为len
			if(!len){
				m=max(m,0+lenth),m=min(m,480-1-lenth);
				if(m==mid_x)return 0;//未移动

				int p=dcmp(DB_P.y+r-R.y1);
				if(p<0){ refresh(m,lenth); return 0; }//如果小球还没到达玩家区域

				else if(p==0){//刚好到达，有一个交点
					DB_P.y-=eps,refresh(m,lenth);
					return (dcmp(R.x1-eps - DB_P.x)<=0&&dcmp(R.x2+eps - DB_P.x)>=0)?UP_LINE:0;//1.上线段 ±eps，稍延长一点（把线与圆弧的交点归属为线）
				}

				else{//进入了玩家区域
					LD Lx,Rx;//拦截区域
					int op;//小球下半部分完全进入了玩家区域
					
					if(dcmp(DB_P.y-R.y1)>=0)Lx=DB_P.x-r,Rx=DB_P.x+r,op=1;//小球下半部分完全进入了玩家区域
					else{
						LD dis=sqrt(1.0*r*r-Squre(R.y1-DB_P.y));
						Lx=DB_P.x-dis,Rx=DB_P.x+dis; op=0;
					}

					if(m<mid_x&&DB_P.x<mid_x){//玩家向左移动,且球在左边
						if(dcmp(Rx+lenth-m)>=0){//被Rx拦截
							m=int(Rx+lenth+eps); refresh(m,lenth);
							return op?LEFT_LINE:LEFT_UP_ARC;
						}

					}
					if(m>mid_x&&DB_P.x>mid_x){//玩家向右移动,且球在右边
						if(dcmp(Lx-lenth-m)<=0){//被Lx拦截
							m=int(Lx-lenth+eps); refresh(m,lenth);
							return op?RIGHT_LINE:RIGHT_UP_ARC;
						}
					}
					refresh(m,lenth); return 0;//未撞球
				}
			}
			else{//更改玩家长度(咕咕咕)
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

namespace BALLINFO{//【小球信息】

	#define ball_img 1
	#define ball_black 2

	struct BallStyle{

		int type,cnt,r;Point P[277+1+16];IMAGE ballpimg[277+1+16];

		inline void getstyle(Re type_id){
			type=type_id,cnt=0;
			if(type==1){ r=9; }
			if(type==2){ r=9;return; }
			if(type==3){ r=20;return; }//调试版
			if(type==4){ r=12;return; }//使用道具【放大】后的2
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
		Point P;//当前绘图中心
		DB_Point DB_P;//当前实数坐标

		LD speed,steplenth;//speed:速度 steplenth:每次移动的直线长度
		DB_Point Dir_ratio,DB_Dir;//Dir_ratio，DB_Dir:移动方向直角边与斜边之比 DB_Dir:实际移动长度

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
			if(styletype==1)steplenth=speed*10;//根据speed调整步幅
			else steplenth=speed*5;
			
			DB_Dir=DB_Point(steplenth*Dir_ratio.x,steplenth*Dir_ratio.y);//每次刷新小球状态所要走的距离
			if(!dcmp(DB_Dir.x))DB_Dir.x=(Rand(0,1)?1:-1)*0.01,Dir_ratio.x=DB_Dir.x/steplenth;//微调偏角防止反复横跳
			if(!dcmp(DB_Dir.y))DB_Dir.y=(Rand(0,1)?1:-1)*0.01,Dir_ratio.y=DB_Dir.y/steplenth;//微调偏角防止反复横跳
		}
		inline void start_biu(Point ms){
			Point Dir=Point(ms.x-P.x,ms.y-P.y);
			Re d=gcd(abs(Dir.x),abs(Dir.y));Dir.x/=d,Dir.y/=d;
			LD incline=sqrt(Dir.x*Dir.x+Dir.y*Dir.y);
			Dir_ratio.x=Dir.x/incline,Dir_ratio.y=Dir.y/incline;
			
			calc_DB_Dir(1.0);
			DB_P=DB_Point(1.0*P.x,1.0*P.y);
		}

		inline void reverse_dir(Re flagx,Re falgy){//翻转运动方向
			if(flagx)Dir_ratio.x=-Dir_ratio.x,DB_Dir.x=-DB_Dir.x;
			if(falgy)Dir_ratio.y=-Dir_ratio.y,DB_Dir.y=-DB_Dir.y;
		}
		inline void reverse(int flag){//小球发生碰撞，改变运动轨迹
			if(flag==UP_LINE)reverse_dir(0,DB_Dir.y>0);//1.上线段
			else if(flag==LEFT_LINE)reverse_dir(DB_Dir.x>0,0);//2.左线段
			else if(flag==RIGHT_LINE)reverse_dir(DB_Dir.x<0,0);//3.右线段
			else if(flag==DOWN_LINE)reverse_dir(0,DB_Dir.y<0);//4.下线段
			else if(flag==LEFT_UP_ARC)reverse_dir(DB_Dir.x>0,DB_Dir.y>0);//5.左上圆弧
			else if(flag==RIGHT_UP_ARC)reverse_dir(DB_Dir.x<0,DB_Dir.y>0);//6.右上圆弧
			else if(flag==LEFT_DOWN_ARC)reverse_dir(DB_Dir.x>0,DB_Dir.y<0);//7.左下圆弧
			else if(flag==RIGHT_DOWN_ARC)reverse_dir(DB_Dir.x<0,DB_Dir.y<0);//8.右下圆弧
		}
		
		inline void judge_wall(Player &player,Range R1,DB_Point &P,DB_Point &P_){//【碰撞检测】判断与四面墙的碰撞情况
			Re p1=dcmp(P_.x-R1.x1);//p1<=0 碰左壁
			Re p2=dcmp(P_.y-R1.y1);//p2<=0 碰上壁
			Re p3=dcmp(P_.x-R1.x2);//p3>=0 碰右壁
			if(p1<=0&&p2<=0){//碰左 且 碰上
				if(p1==0&&p2==0)P_.x=R1.x1,P_.y=R1.y1,reverse_dir(1,1);//角落碰撞（同时碰左壁和上壁）
				else{
					LD t1=(P.x-R1.x1)/DB_Dir.x,t2=(P.y-R1.y1)/DB_Dir.y;
					if(t1<t2)P_.x=R1.x1, reverse_dir(1,0);//先碰左壁
					else P_.y=R1.y1, reverse_dir(0,1);//先碰上壁
				}
			}
			else if(p3>=0&&p2<=0){//碰右 且 碰上
				if(p3==0&&p2==0)P_.x=R1.x2,P_.y=R1.y1,reverse_dir(1,1);//角落碰撞（同时碰右壁和上壁）
				else{
					LD t1=(R1.x2-P.x)/DB_Dir.x,t2=(P.y-R1.y1)/DB_Dir.y;
					if(t1<t2)P_.x=R1.x2, reverse_dir(1,0);//先碰右壁
					else P_.y=R1.y1, reverse_dir(0,1);//先碰上壁
				}
			}
			else if(p1<=0)P_.x=R1.x1,reverse_dir(1,0);//碰左
			else if(p2<=0)P_.y=R1.y1,reverse_dir(0,1);//碰上
			else if(p3>=0)P_.x=R1.x2,reverse_dir(1,0);//碰右
			//P_.x=max(P_.x,R1.x1),P_.y=max(P_.y,R1.y1);//P.x-r>=gamebox.x1,P.y-r>=gamebox.y1
			//P_.x=min(P_.x,R1.x2),P_.y=min(P_.y,R1.y2);//P.x+r<=gamebox.x2,P.y+r<=gamebox.y2
			return;
		}
		
		inline int get_id(DB_Point P,DB_Point O){//检查点在圆的哪个方位
			if(P.x<O.x)return P.y<O.y?5:7;//左上,左下
			else return P.y<O.y?6:8;//右上,右下
		}
		inline int cross_LH(DB_Point st,DB_Point ed,DB_Point O,LD R,Re op,DB_Point &CP){//线段与圆弧的交点
			DB_Point A=st,B=ed;
			Re cnt=GMT::cross_LC(A,B,O,R);
			if(!cnt)return 0;//如果没有一个交点
			Re idA=get_id(A,O);
			if(cnt==1){//如果只有一个交点
				if(idA==op){ CP=A;return 1; }//在该圆弧内
				return 0;
			}
			//如果有两个交点
			Re idB=get_id(B,O);
			if(idA==op&&idB==op){//如果两个都在该圆弧内
				CP=GMT::Len(B-st)<GMT::Len(A-st)?B:A;//取离st更近的交点
				return 1;
			}
			if(idA==op){ CP=A;return 1; }//A在该圆弧内
			if(idB==op){ CP=B;return 1; }//B在该圆弧内
			return 0;
		}
		
		inline int judge_player(Player &py,DB_Point &st,DB_Point &ed){//【碰撞检测】判断与玩家的碰撞情况
			if(dcmp(ed.y+r-py.R.y1)<0)return 0;//小剪枝：如果运动轨迹的终点还没到玩家区域
			DB_Point CP;Re flag=0;LD dis_CPP=2e9;

			DB_Point A=st,B=ed,A_,B_;
			A_=DB_Point(py.R.x1,py.R.y1-r),B_=DB_Point(py.R.x2,py.R.y1-r);
			if(GMT::pan_cross_LL(A,B,A_,B_)){//1.上线段
				CP=GMT::cross_LL(A,B,A_,B_); flag=UP_LINE; dis_CPP=GMT::Len(CP-st);
			}
			A_=DB_Point(py.R.x1-r,py.R.y1),B_=DB_Point(py.R.x1-r,py.R.y2);
			if(GMT::pan_cross_LL(A,B,A_,B_)){//2.左线段
				DB_Point tmp_CP=GMT::cross_LL(A,B,A_,B_); LD tmp_dis=GMT::Len(tmp_CP-st);
				if(tmp_dis<dis_CPP)CP=tmp_CP,flag=LEFT_LINE,dis_CPP=tmp_dis;
			}
			A_=DB_Point(py.R.x2+r,py.R.y1),B_=DB_Point(py.R.x2+r,py.R.y2);
			if(GMT::pan_cross_LL(A,B,A_,B_)){//3.右线段
				DB_Point tmp_CP=GMT::cross_LL(A,B,A_,B_); LD tmp_dis=GMT::Len(tmp_CP-st);
				if(tmp_dis<dis_CPP)CP=tmp_CP,flag=RIGHT_LINE,dis_CPP=tmp_dis;
			}

			DB_Point tmp_CP;Re pan;
			pan=cross_LH(A,B,DB_Point(py.R.x1,py.R.y1),r,LEFT_UP_ARC,tmp_CP);//5.左上圆弧
			if(pan){
				LD tmp_dis=GMT::Len(tmp_CP-st);
				if(tmp_dis<dis_CPP)CP=tmp_CP,flag=LEFT_UP_ARC,dis_CPP=tmp_dis;
			}
			pan=cross_LH(A,B,DB_Point(py.R.x2,py.R.y1),r,RIGHT_UP_ARC,tmp_CP);//6.右上圆弧
			if(pan){
				LD tmp_dis=GMT::Len(tmp_CP-st);
				if(tmp_dis<dis_CPP)CP=tmp_CP,flag=RIGHT_UP_ARC,dis_CPP=tmp_dis;
			}

			if(flag){//发生碰撞，改变运动轨迹
//ed=CP,reverse(flag); return flag;
				if(flag==2||flag==3){ ed=CP,reverse(flag); return flag; }//(镜面反弹) 2.左线段 3.右线段
				if(flag==1){//1.上线段
					if(dcmp(CP.x-py.center_L)>=0&&dcmp(CP.x-py.center_R)<=0){ ed=CP,reverse(flag); return flag; }//(镜面反弹) 中心区域
					LD rad;
					if(CP.x<py.center_L)rad=toPi(75.0-60.0*(py.center_L-CP.x)/(py.lenth*(1-player_center_range))),Dir_ratio.x=-cos(rad);//(特殊反弹) 左特区
					if(CP.x>py.center_R)rad=toPi(75.0-60.0*(CP.x-py.center_R)/(py.lenth*(1-player_center_range))),Dir_ratio.x= cos(rad);//(特殊反弹) 右特区
					Dir_ratio.y=-sin(rad);
					calc_DB_Dir(speed);
					ed=CP; return flag;
				}
				if(flag==5||flag==6){//5.左上圆弧 6.右上圆弧
					LD rad=toPi(15.0);//碰圆弧直接15°最小偏转角
					if(flag==5)Dir_ratio.x=-cos(rad);//(特殊反弹) 5.左上圆弧
					if(flag==6)Dir_ratio.x= cos(rad);//(特殊反弹) 6.右上圆弧
					Dir_ratio.y=-sin(rad);
					calc_DB_Dir(speed);
					ed=CP; return flag;
				}
			}
			else return 0;//未发生碰撞
		}
		
		int Rid_tail,Rid_stack[MAX_brick_cnt+1],Rid_stack_pan[MAX_brick_cnt+1];
		inline void find_Rid_area(DB_Point &st,DB_Point &ed,Brick *brick){//找到小球可能碰到的砖块
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

			judge_brick_line(Rid,UP_LINE   ,A,B,DB_Point(R.x1,R.y1-r),DB_Point(R.x2,R.y1-r),CP,flag,bk_Rid,dis_CPP);//1.上线段
			judge_brick_line(Rid,LEFT_LINE ,A,B,DB_Point(R.x1-r,R.y1),DB_Point(R.x1-r,R.y2),CP,flag,bk_Rid,dis_CPP);//2.左线段
			judge_brick_line(Rid,RIGHT_LINE,A,B,DB_Point(R.x2+r,R.y1),DB_Point(R.x2+r,R.y2),CP,flag,bk_Rid,dis_CPP);//3.右线段
			judge_brick_line(Rid,DOWN_LINE ,A,B,DB_Point(R.x1,R.y2+r),DB_Point(R.x2,R.y2+r),CP,flag,bk_Rid,dis_CPP);//4.下线段

			judge_brick_arc(Rid,LEFT_UP_ARC    ,A,B,DB_Point(R.x1,R.y1),r,CP,flag,bk_Rid,dis_CPP);//5.左上圆弧
			judge_brick_arc(Rid,RIGHT_UP_ARC   ,A,B,DB_Point(R.x2,R.y1),r,CP,flag,bk_Rid,dis_CPP);//6.右上圆弧
			judge_brick_arc(Rid,LEFT_DOWN_ARC  ,A,B,DB_Point(R.x1,R.y2),r,CP,flag,bk_Rid,dis_CPP);//7.左下圆弧
			judge_brick_arc(Rid,RIGHT_DOWN_ARC ,A,B,DB_Point(R.x2,R.y2),r,CP,flag,bk_Rid,dis_CPP);//8.右下圆弧
		}
		inline int judge_brick(DB_Point &st,DB_Point &ed,Brick *brick,Re &bk_cnt){//【碰撞检测】判断与砖块的碰撞情况
			if(st.y-r>brickbox.y2&&ed.y-r>brickbox.y2)return 0;//小剪枝：如果还没到玩家区域
			
			//for(Re i=1;i<=Rid_tail;++i)Rid_stack_pan[Rid_stack[i]]=0;
			memset(Rid_stack_pan,0,sizeof(Rid_stack_pan));
			Rid_tail=0,find_Rid_area(st,ed,brick);

			Re minRid=1000000000,maxRid=-1000000000; DB_Point CP;Re flag=0,bk_Rid=0;LD dis_CPP=2e9;
			for(Re i=1;i<=Rid_tail;++i)
				judge_brick_(brick[Rid_stack[i]].Rid,st,ed,CP,flag,bk_Rid,dis_CPP);
			if(flag){//发生碰撞
//if(brick[1].styletype>1)
	//printdebug(_T("FuckYOU!!!!!!!!!!!!!!")),system("pause"); 
//CH say[100];_stprintf(say,_T("碰撞：%d"),bk_Rid),printdebug(say); system("pause");
				ed=CP,reverse(flag);//改变运动轨迹
				
				//砖块受到攻击
				print_background();//砖块变化前先抠掉小球
				brick[bk_Rid].vanish();//砖块变化
				playgame_ball_getimage();//重新获取小球背景图像
				print();//重新输出小球

				bk_cnt-=(brick[bk_Rid].styletype==0);//砖块全部消灭【通过】
				return !bk_cnt?-100:flag;
			}
			return 0;//未发生碰撞
		}

		inline char judge_collide(Player &player,Range R1,DB_Point &st,DB_Point &ed,Brick *brick,Re &bk_cnt){
			if(dcmp(ed.y-R1.y2)>=0){ ed.y=R1.y2;return 'D'; }//碰下【死亡】
			int cmd_player=judge_player(player,st,ed);//判断与玩家的碰撞情况
			//if(!cmd_player){//判断与砖块的碰撞情况
				Re cmd_brick=judge_brick(st,ed,brick,bk_cnt);
				if(cmd_brick==-100)return 'P';//砖块全部消灭【通关】
				if(cmd_brick!=0)return 'B';//撞砖块
			//}
			judge_wall(player,R1,st,ed);//判断与四面墙的碰撞情况
			return 0;
		}

		inline void change_speed(char cmd){//改变小球速度
			if(cmd=='F'&&dcmp(speed-3.0)<0)calc_DB_Dir(speed+0.5);
			if(cmd=='S'&&dcmp(speed-0.5)>0)calc_DB_Dir(speed-0.5);
		}

		inline char updata(Player &player,Range R0,Brick *brick,Re &bk_cnt){//更新小球位置
			DB_Point st=DB_P;DB_P+=DB_Dir;

			char cmd=judge_collide(player,R0.cutmid(r,r),st,DB_P,brick,bk_cnt);//【碰撞检测】
			Point P_=Point(int(DB_P.x+eps),int(DB_P.y+eps));
			if(P_!=P)refresh(P_);//更新图像

			if(cmd=='D')return 'D';//【死亡】
			if(cmd=='P')return 'P';//【通关】

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
	struct Option{//选项
		bool msin;//选项状态(鼠标是否在其碰撞箱以内)
		Range box;IMAGE bkimg;//碰撞箱、选项背景
		int height,weight;STR text;//文字
		int line_weight;//边框厚度
		COLOR line_color[2],text_color[2];

		Option(Range R={0,0,0,0},Re H=0,Re W=0,STR T=_T(""),
				COLOR line_color_0=default_option_line_color[0],COLOR line_color_1=default_option_line_color[1],
				COLOR text_color_0=default_option_text_color[0],COLOR text_color_1=default_option_text_color[1],
				Re line_W=3){//构造函数
			line_color[0]=line_color_0,line_color[1]=line_color_1;
			text_color[0]=text_color_0,text_color[1]=text_color_1;
			box=R,height=H,weight=W,text=T,line_weight=line_W,msin=0;
			bkimg=H?getimage(box.boost(1,1)):IMAGE(0,0);
		}

		inline void refresh(){//刷新选项
			Setbkmode(bkmode2),ClearRange(box.boost(1,1),bkimg);//设置绘图透明、用选项背景清除该块
			drawrange(box,line_color[msin],line_weight);//画边框
			Settextstyle(height,weight,text_color[msin],0,msin),print(box,text);//画文字
		}

		inline bool updata_action(MOUSE m){//检查选项状态
			Point ms=Point(m.x,m.y);
			if(PIR(ms,box)^msin)msin^=1,refresh();//若选项存储的状态与当前状态不同，则刷新状态
			return (msin&&m.ms_info==ms_lefthit);//鼠标点击了选项
		}
	};
	struct Option_img{//选项(贴图版)
		bool msin;//选项状态(鼠标是否在其碰撞箱以内)
		Range box;IMAGE img0,img1;//碰撞箱、贴图、选项背景

		Option_img(Range R={0,0,0,0},IMAGE img0_=IMAGE(0,0),IMAGE img1_=IMAGE(0,0)){//构造函数
			img0=img0_,img1=img1_,box=R,msin=0;
		}
		inline void refresh(){ putimage_transparancy(box.x1,box.y1,msin?&img1:&img0); }//刷新选项
		inline bool updata_action(MOUSE m){//检查选项状态
			Point ms=Point(m.x,m.y);
			if(PIR(ms,box)^msin)msin^=1,refresh();//若选项存储的状态与当前状态不同，则刷新状态
			return (msin&&m.ms_info==ms_lefthit);//鼠标点击了选项
		}
	};
	struct Option_bkimg_text{//选项
		bool msin;//选项状态(鼠标是否在其碰撞箱以内)
		Range box;IMAGE bkimg;//碰撞箱、选项背景
		int height,weight;STR text;//文字
		IMAGE bkimg0,bkimg1;//背景贴图
		COLOR text_color[2];

		Option_bkimg_text(Range R={0,0,0,0},Re H=0,Re W=0,STR T=_T(""),
				IMAGE img0_=IMAGE(0,0),IMAGE img1_=IMAGE(0,0),
				COLOR text_color_0=default_option_text_color[0],COLOR text_color_1=default_option_text_color[1]
				){//构造函数
			bkimg0=img0_,bkimg1=img1_;
			text_color[0]=text_color_0,text_color[1]=text_color_1;
			box=R,height=H,weight=W,text=T,msin=0;
			bkimg=H?getimage(box):IMAGE(0,0);
		}

		inline void refresh(){//刷新选项
			Setbkmode(bkmode2),ClearRange(box,bkimg);//设置绘图透明、用选项背景清除该块
			putimage_transparancy(box.x1,box.y1,msin?&bkimg1:&bkimg0);//画背景
			Settextstyle(height,weight,text_color[msin],0,msin),print(box,text);//画文字
		}

		inline bool updata_action(MOUSE m){//检查选项状态
			Point ms=Point(m.x,m.y);
			if(PIR(ms,box)^msin)msin^=1,refresh();//若选项存储的状态与当前状态不同，则刷新状态
			return (msin&&m.ms_info==ms_lefthit);//鼠标点击了选项
		}
	};
	struct Level_option{//【选择关卡】选项
		Option_bkimg_text option;CH text[100];
		int levelid,levelopen;//该关卡是否已经通过(level限定)
		
		inline void refresh(){ option.refresh(); }//刷新选项
		inline bool updata_action(MOUSE m){ return option.updata_action(m); }//检查选项状态
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
			else if(id==level_endless)_stprintf(text,_T("无尽模式"));
			else _stprintf(text,_T("待开发中"));

			levelopen=(id==1);//关卡全开模式
			if(levelid>common_level_cnt)levelopen=0;//关卡还未开发

			IMAGE bkimg[2];COLOR text_color[2]; get_option_img_and_color(bkimg,text_color);
			option=Option_bkimg_text(R,20,200,text,bkimg[0],bkimg[1],text_color[0],text_color[1]);
		}
	};
}
using OPTION::Option;
using OPTION::Option_img;
using OPTION::Level_option;

namespace GAMERUNNING{//【游戏进行中】

	int ballstyle,leveltype;
	int HP;BALLINFO::Ball HPB[MAX_HP_cnt+1];
	Option_img HParea,order;//血量框、菜单

	Player game_player;
	Ball game_ball;
	
	int game_brick_cnt;
	Brick game_brick[MAX_brick_cnt+1];

	inline void refresh_debug_level_info(){//刷新debug区域关卡信息
		if(leveltype==level_endless)printdebug(_T("【无尽模式】"));
		else{ CH say[100];_stprintf(say,_T("【Level %d】"),leveltype),printdebug(say); }
	}
	inline void refresh(){//【游戏进行中】刷新界面

		Setmain(mainbox,bkimg_up);//游戏界面主区域
		Setdebug(debugbox,bkimg_down,30,100,default_debug_text_color);//游戏界面调试区域

		refresh_debug_level_info();

		order.msin=0,order.refresh();
		//putimage(7,1,option_order_img[0]);
		putimage_transparancy(HParea_box.x1,HParea_box.y1,&background_HParea_img);
		//HParea.msin=0,HParea.refresh();
		//drawrange(HPbox,default_option_line_color[0],2);
		for(Re i=1,j=0;i<=HP;++i,++j)HPB[i].init_refresh();
	}

	struct Gamerunning_Debuginfo{//【游戏进行中】调试信息

		LD Max_delaytime,Min_delaytime;

		inline void init(){
			Max_delaytime=-1,Min_delaytime=1e8;
		}
		inline void coordinate(){//【Debug】小球坐标，玩家坐标
			CH say[100];_stprintf(say,_T("小球:(%.2lf,%.2lf) 玩家:(%d)"),game_ball.DB_P.x,game_ball.DB_P.y,game_player.mid_x);
			printdebug(say,__debug_color,25,700);
		}
		inline void delaytime(LD t){
			Max_delaytime=max(Max_delaytime,t),Min_delaytime=min(Min_delaytime,t);
			//if(t*1000>35)system("pause");
			CH say[100];_stprintf(say,_T("当前:%.1lfms, 最小:%.1lfms, 最大:%.1lfms"),t*1000,Min_delaytime*1000,Max_delaytime*1000);
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

	inline void dead_anime(Re HP){//【死亡动画】
		for(Re O=1;O<=5;++O){//闪烁次数
			HPB[HP].print_background(),game_ball.print_background();
			print({0,224,480-1,424-1},_T("【Oh yeah~ You dead!!!】"),red,40,200);
			Sleep(200);
			
			HPB[HP].print(),game_ball.print();
			print({0,224,480-1,424-1},_T("【Oh yeah~ You dead!!!】"),darkgray,40,200);
			Sleep(200);
		}
	}
	inline void pass_anime(){//【通关动画】
		for(Re O=1;O<=5;++O){//闪烁次数
			print({0,224,480-1,324-1},_T("【Oh Noooo! You have passed! 】"),red,30,200);
			print({0,324,480-1,424-1},_T("【Why?! I can't agree that!!!】"),red,30,200);
			Sleep(200);

			print({0,224,480-1,324-1},_T("【Oh Noooo! You have passed! 】"),darkgray,30,200);
			print({0,324,480-1,424-1},_T("【Why?! I can't agree that!!!】"),darkgray,30,200);
			Sleep(200);
		}
	}

	inline void game_pause(){//【游戏暂停】
		printdebug(_T("【游戏暂停】输入任意键以继续..."),white,28,100);
		system("pause"),refresh_debug_level_info();
	}
	inline int G_controller(){//【开挂操作界面】
		while(1){
			if(_kbhit()){
				char c=_getch();
				if(c==kb_esc||c==kb_G)return 0;//按下[ESC]或[G]，无操作
				else if(c==kb_P)return 'P';//按[P]开挂，直接【过关】
				else if(c==kb_D)return 'D';//按[D]开挂，直接【死亡】
				else if(c==kb_L){ game_player.updata(game_ball.DB_P,game_ball.r,game_player.mid_x,game_player.lenth*2); return 0; }//按[L]开挂，得到道具【伸长】
				else if(c==kb_F){ game_ball.change_speed('F'); return 0; }//按[F]开挂，得到buff fast【加速】
				else if(c==kb_S){ game_ball.change_speed('S'); return 0; }//按[S]开挂，得到buff slow【减速】
			}
			Sleep(20);
		}
	}

	inline char updata(Re flag){//【游戏进行中】检测行为
		if(_kbhit()){
			char c=_getch();
			if(c==kb_esc)return 'E';//按下[ESC]
			else if(c==kb_space)return 'S';//按space【暂停】
			else if(c==kb_P)return 'P';//按[P]开挂，直接【过关】
			else if(c==kb_D)return 'D';//按[D]开挂，直接【死亡】
			else if(c==kb_G)return 'G';//按[G]开挂，进入【开挂操作界面】
		}
		if(flag){//在【准备发射】阶段
			if(_mshit()){
				MOUSE m=_getms();
				if(order.updata_action(m))return 'E';//按下【菜单】
				//if(m.ms_info==ms_righthit)return '';
				if(PIR({m.x,m.y},Range(gamebox.x1+1,gamebox.y1+1,gamebox.x2-1,game_ball.P.y-1))&&m.ms_info==ms_lefthit){
					game_ball.start_biu({m.x,m.y});return 'B';//【发射】
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
			int player_cmd=game_player.updata(game_ball.DB_P,game_ball.r,m.x);//更新玩家位置
			if(player_cmd)game_ball.reverse(player_cmd);//如果发生了碰撞，改变运动轨迹
		}
		char cmd=game_ball.updata(game_player,gamebox.cutmid(1,1),game_brick,game_brick_cnt);//更新小球位置
		if(cmd=='D')return 'D';//【死亡】
		if(cmd=='P')return 'P';//【通关】
		_Flushms();
		return 0;
	}
	inline char playgame_(Re levelid){//【游戏进行中】

		refresh();
		
		for(int i=1;i<=MAX_brick_cnt;++i)game_brick[i].print();

		game_player.playgame_init();
		
		game_ball=Ball({game_player.mid_x,game_player.R.y1-BALLINFO::style[2].r-1},2);
//game_ball=Ball({game_player.mid_x,game_player.R.y1-BALLINFO::style[3].r-1},3); (调试模式：超大球)
		game_ball.init_refresh();
		
		debug.init();

		while(1){//准备发射
			char cmd=updata(1);
			if(cmd=='E')return 'E';//按下[ESC]或者[菜单]
			else if(cmd=='B')break;//【发射】
			else if(cmd=='P')return 'P';//按[P]开挂，直接【过关】
			else if(cmd=='D')return 'D';//按[D]开挂，直接【死亡】
			Sleep(1);
		}
		LD temp=gettime(),tp=gettime();
		while(1){
			LD nowt=gettime();
			if((nowt-tp)*1000<15)continue;
			else tp=nowt;
			char cmd=updata(0);
			if(cmd=='E')return 'E';//refresh();//按下[ESC]或者[菜单]
			else if(cmd=='S')game_pause();//按space【暂停】
			else if(cmd=='D')return 'D';//自然死亡 或 按[D]开挂【死亡】;
			else if(cmd=='P')return 'P';//按[P]开挂，直接【过关】
			else if(cmd=='G'){//按G进入开挂操作界面
				int G_cmd=G_controller();
				if(G_cmd=='P')return 'P';//按[P]开挂，直接【过关】
				else if(G_cmd=='D')return 'D';//按[D]开挂，直接【死亡】
			}
debug.delaytime(gettime()-temp);temp=gettime();
//debug.coordinate();
//debug.brick_styletype();
		}
	}
	inline int playgame_main(Re levelid){//【游戏进行中】主程序
		refresh();

		HP=Level[leveltype=levelid].HP_cnt;

		game_brick_cnt=Level[levelid].brick_cnt;
		for(int i=1;i<=MAX_brick_cnt;++i)
			game_brick[i].playgame_main_init(i,Level[levelid].brick_style[i]);

		while(HP){
			Re cmd=playgame_(levelid);
			if(cmd=='E')return 0;//按下[ESC]或者[菜单] 【未通关】
			else if(cmd=='P'){ pass_anime(); return 1; }//【通关】
			else if(cmd=='D')dead_anime(HP),--HP;//播放【死亡动画】
		}
		return 0;//【未通关】
	}

	inline void init(){

		Setmain(mainbox,bkimg_up);//游戏界面主区域
		Setdebug(debugbox,bkimg_down,30,100,default_debug_text_color);//游戏界面调试区域

		BALLINFO::init();

		//HPbox=HParea_box;//{0 +7,0 +1,23*MAX_HP_cnt-1 +7,22 +1}
		for(Re i=1,j=0;i<=MAX_HP_cnt;++i,++j)HPB[i]=BALLINFO::Ball({23*j+12-1 +7,12-1 +1},2);

		Setmain(mainbox,bkimg_up_option);//选项界面主区域
		Setdebug(debugbox,bkimg_down_option,30,100,default_debug_text_color);//选项界面调试区域

		//HParea=Option_img(HParea_box,background_HParea_img,background_HParea_img);
		//order=Option({244,1,473,23},16,500,_T("【菜单】"));//{480-230+1 -7,0 +1,480 -7,23-1 +1}
		order=Option_img(order_box,option_order_img[0],option_order_img[1]);
		//{0,0,229,22}
	}
}

namespace LEVELMAKER{
	int leveltype;
	int HP;Range HPbox;BALLINFO::Ball HPB[MAX_HP_cnt+1];
	Option order;//菜单
	inline void open_file_write(int id){
		freopen("CON","r",stdin); freopen("CON","w",stdout);
		std::string _id; std::stringstream ss; ss<<id; ss>>_id;
		std::string filepath="./data/level/level_"+_id+".txt";
		freopen(filepath.c_str(),"w",stdout);
	}
	inline void close_file(){ freopen("CON","r",stdin); freopen("CON","w",stdout); }
	inline void controler_savefile(Brick brick[]){//【开发者模式】保存文件
		open_file_write(leveltype);
		for(Re i=1;i<=16;++i,puts(""))
			for(Re j=1;j<=10;++j)
				putchar(brick[(i-1)*10+j].styletype+'0');
		close_file();
	}

	inline void refresh_debug_level_info(){//刷新debug区域关卡信息
		if(leveltype==level_endless)printdebug(_T("【无尽模式】开发者模式"));
		else{ CH say[100];_stprintf(say,_T("【Level %d】开发者模式"),leveltype),printdebug(say); }
	}
	inline void refresh(){//【游戏进行中】刷新界面

		Setmain(mainbox,bkimg_up);//游戏界面主区域
		Setdebug(debugbox,bkimg_down,30,100,default_debug_text_color);//游戏界面调试区域

		refresh_debug_level_info();

		order.msin=0,order.refresh();

		drawrange(HPbox,default_option_line_color[0],2);
		for(Re i=1,j=0;i<=HP;++i,++j)HPB[i].init_refresh();
	}

	Brick brick[MAX_brick_cnt+1];
	inline char updata(){//【游戏进行中】检测行为
		if(_kbhit()){
			char c=_getch();
			if(c==kb_esc)return 'E';//按下[ESC]
			else if(c==kb_S)return 'S';//按下[S]保存文件
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
	inline void controler_levelmaker(Re levelid){//【开发者模式】关卡制造
		leveltype=levelid;
		refresh();

		HP=3;

		for(Re i=1;i<=MAX_brick_cnt;++i)brick[i].controler_levelmaker_init(i);

		while(1){
			Re cmd=updata();
			if(cmd=='E')return;//按下[ESC]或者[菜单]
			else if(cmd=='S'){ controler_savefile(brick);return; }//按下[S]保存文件
		}
		return;
	}
	inline void init(){

		Setmain(mainbox,bkimg_up);//游戏界面主区域
		Setdebug(debugbox,bkimg_down,30,100,default_debug_text_color);//游戏界面调试区域
		
		HPbox={0 +7,0 +1,23*MAX_HP_cnt-1 +7,22 +1};
		for(Re i=1,j=0;i<=MAX_HP_cnt;++i,++j)HPB[i]=BALLINFO::Ball({23*j+12-1 +7,12-1 +1},2);

		Setmain(mainbox,bkimg_up_option);//选项界面主区域
		Setdebug(debugbox,bkimg_down_option,30,100,default_debug_text_color);//选项界面调试区域

		order=Option({480-230+1 -7,0 +1,480 -7,23-1 +1},16,500,_T("【菜单】"));
	}
}

namespace CHOOSELEVEL{//【选择关卡】

	Option_img returnback;
	Level_option level_option[MAX_level_cnt+1];

	inline void refresh(){//【选择关卡】刷新界面

		Setmain(mainbox,bkimg_up);//游戏界面主区域
		Setdebug(debugbox,bkimg_down,30,100,default_debug_text_color);//游戏界面调试区域

		printdebug(_T("【选择关卡】"));

		returnback.msin=0,returnback.refresh();
_Begin();
		for(Re i=1;i<=MAX_level_cnt;++i)level_option[i].option.msin=0,level_option[i].refresh();
_End(),_Flush();
	}

	inline int updata(){//【选择关卡】检查行为
		if(_kbhit()){
			char c=_getch();
			if(c==kb_esc)return -1;
			if(c=='O')return -1000+'O';//按[O]开挂【关卡全开】
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

	inline void ChooseLevel(){//【选择关卡】主程序
		Re warning_showtime=0,warning_type=-1;refresh();
		while(1){
			Re cmd=updata();
			if(cmd==-1)return;//按下[ESC]或者[返回开始界面]
			if(cmd==-1000+'O')controlermode_level_allopen=1,CHOOSELEVEL::level_allopen(),refresh();//按[O]开挂【关卡全开】
			if(cmd>=1){
				if(controlermode_make_level)LEVELMAKER::controler_levelmaker(cmd),refresh();//【开发者模式】关卡制作
				else if(level_option[cmd].levelopen){//按下[level *]
					Re cmd_pass=GAMERUNNING::playgame_main(cmd);
					if(cmd_pass)level_option[min(cmd+1,common_level_cnt)].open();
					warning_showtime=0,refresh();
				}
				else warning_showtime=(2000*10),warning_type=-1;//你点到了不该点的东西QAQ
			}
			if(warning_showtime){
				Re type=warning_showtime/(250*10)&1;//约每0.25s更新一次颜色
				if(warning_type!=type)printdebug(_T("【FBI Warning】请先完成前一关。"),(warning_type=type)?lightred:lightgray,25,200);
				if(!--warning_showtime)printdebug(_T("【选择关卡】"));//约2s后不再发出Warning
			}
		}
	}

	inline void init(){//【选择关卡】初始化
		Setmain(mainbox,bkimg_up_option);//选项界面主区域
		Setdebug(debugbox,bkimg_down_option,30,100,default_debug_text_color);//选项界面调试区域

		returnback=Option_img(returnback_box,option_returnback_img[0],option_returnback_img[1]);

		Re stx=35+10,sty=35+10;
		for(Re i=1;i<=MAX_level_cnt;++i){
			level_option[i].init({stx,sty,stx+90,sty+30},i);//{0,0,90,30}
			stx+=100;if(stx==445)stx=45,sty+=40;
		}
	}
}
using CHOOSELEVEL::ChooseLevel;

namespace GAMEINFO{//【游戏说明】

	Range textbox;//文字
	Option_img returnback;//返回开始界面
	STR gameinfo_text;

	inline void refresh(){//【游戏说明】刷新界面
		Setmain(mainbox,bkimg_up);//游戏界面主区域
		Setdebug(debugbox,bkimg_down,30,100,default_debug_text_color);//游戏界面调试区域
		
		printdebug(_T("【游戏说明（施工中）】"));

		returnback.msin=0,returnback.refresh();

		print(textbox,gameinfo_text,lightred,20,100,2);
	}

	inline int updata(){//【游戏说明】检查行为
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

	inline void GameInfo(){//【游戏说明】主程序
		refresh();
		cmd_openfile("\\游戏说明.pdf");
		while(1){
			Re cmd=updata();
			if(cmd==-1)return;//按下[ESC]或者[返回开始界面]
		}
	}

	inline void init(){//【游戏说明】初始化
		Setmain(mainbox,bkimg_up);//游戏界面主区域
		Setdebug(debugbox,bkimg_down,30,100,default_debug_text_color);//游戏界面调试区域
		
		textbox=gameinfo_text_box;
		gameinfo_text=_T("");

		Setmain(mainbox,bkimg_up_option);//选项界面主区域
		Setdebug(debugbox,bkimg_down_option,30,100,default_debug_text_color);//选项界面调试区域

		returnback=Option_img(returnback_box,option_returnback_img[0],option_returnback_img[1]);
		//returnback=Option({110,625,370,675},30,80,_T("返回开始界面"));
		//{0,0,260,50}
	}
}
using GAMEINFO::GameInfo;

namespace STARTGRAPH{//【开始界面】

	Range Titlebox;//大标题
	const int Title_shadow=5;
	//Range Staffbox;IMAGE Staff_bkimg;//作者
	
	Option_img gamestart,gameinfo;

	inline void refresh(){//【开始界面】刷新界面

		Setmain(mainbox,bkimg_up_st);//开始界面主区域
		Setdebug(debugbox,bkimg_down_st,30,100,default_debug_text_color);//开始界面调试区域

		printdebug(_T("【开始界面】"));

		putimage_transparancy(Titlebox.x1,Titlebox.y1,&text_Title_img);
		//Setbkmode(bkmode2);//透明字体
		//Settextstyle(140,1000,Title_textshadow_color);
		//print(Titlebox.transfer(+Title_shadow),_T("打砖块"));
		//Settextstyle(140,1000,Title_text_color);
		//print(Titlebox,_T("打砖块"));

		//putimage(Staffbox.x1,Staffbox.y1,Staff_bkimg);
		//Setbkmode(bkmode2);//透明字体
		//Settextstyle(20,100,default_Staff_text_color,0,1);
		//print(Staffbox,_T("作者：辰星凌 (Xing_Ling)"));

		gamestart.msin=0,gamestart.refresh();
		gameinfo.msin=0,gameinfo.refresh();
	}

	inline int updata(){//【开始界面】检查行为
		if(_kbhit()){
			char c=_getch();
			if(c=='M')return 'M';//【开发者模式】关卡制作
			if(c=='O')return 'O';//按[O]开挂【关卡全开】
			if(c==kb_esc)return -1;
		}
		if(_mshit()){
			MOUSE m=_getms();
			if(gamestart.updata_action(m))return 1;
			if(gameinfo.updata_action(m))return 2;
		}
		return 0;
	}

	inline void StartGraph(){//【开始界面】主程序
		refresh();

		while(1){
			Re cmd=updata();
			if(cmd==-1)return;//按下[ESC]
			if(cmd==1)ChooseLevel(),refresh();//按下[开始游戏]
			if(cmd==2)GameInfo(),refresh();//按下[游戏说明]
			if(cmd=='M')controlermode_make_level=1;//【开发者模式】关卡制作
			if(cmd=='O')controlermode_level_allopen=1,CHOOSELEVEL::level_allopen(),refresh();//按[O]开挂【关卡全开】
			Sleep(1);
		}
	}

	inline void init(){//【开始界面】初始化

		//Setmain(mainbox,bkimg_up);//游戏界面主区域
		//Setdebug(debugbox,bkimg_down,30,100,default_debug_text_color);//游戏界面调试区域

		Titlebox={30,100,450,280}; //Title_bkimg=getimage(Titlebox);
		//{0,0,420,180}
		//Staffbox={30,340,450,370}; //Staff_bkimg=getimage(Staffbox);

		//Setmain(mainbox,bkimg_up_st);//开始界面主区域
		//Setdebug(debugbox,bkimg_down_st,30,100,default_debug_text_color);//开始界面调试区域

		Setmain(mainbox,bkimg_up_option);//选项界面主区域
		Setdebug(debugbox,bkimg_down_option,30,100,default_debug_text_color);//选项界面调试区域

		gamestart=Option_img(gamestart_box,option_gamestart_img[0],option_gamestart_img[1]);//{0,0,290,80}
		gameinfo=Option_img(gameinfo_box,option_gameinfo_img[0],option_gameinfo_img[1]);//{0,0,290,80}
		//gamestart=Option({95,480,385,560},50,100,_T("开始游戏"));
		//gameinfo=Option({95,610,385,690},50,100,_T("游戏说明"));
	}
}
using STARTGRAPH::StartGraph;

inline void init(){

	Setgraphsize(480,820);
	SetWindowstext(_T("【作者：辰星凌/Xing_Ling】打砖块 "));

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
	//BALLINFO::init();//在GAMERUNNING::init();中已经调用
	BRICKINFO::init();
	PLAYERINFO::init();
	
}
int main(){ init(),StartGraph(),CloseProgram(); }