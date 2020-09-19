#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<Windows.h>
#include<wincon.h>
#include<conio.h>
#include<string.h>
#include<io.h>
#include<fcntl.h>
#include<sys\stat.h>
#include<ctype.h>
#include<time.h>

#ifndef TYPE_H_INCLUDED
#define TYPE_H_INCLUDED

#define SCR_ROW 25  //屏幕行数
#define SCR_COL 80  //屏幕列数

//第三重结点，支链上的租车订单信息表
typedef struct charge_node {
	char rent_id[20];   //订单编号
	char guest_id[20];   //身份证号
	char name[20];  //客人姓名
	char tel[20];   //客人电话
	int number;   //租用的车辆编号  ?为什么不是int型 char[4]
	char time_of_get_car[18];   //取车时间
	char time_of_scheduled_back[18];  //预约还车时间
	char time_of_actual_back[18];   //实际还车时间
	float deposit;    //押金为所租车辆应缴费用×5
	float should_pay;  //应缴费用
	float actual_pay;  //实缴费用
	struct charge_node* next;   //普通指针
}CHARGE_NODE;

//第二重结点，支链上的各个类型的车辆基本信息
typedef struct car_node {
	int  number;        //车辆编号，单纯的顺序1，2，3等
	char car_num[10];       //车牌号：如“鄂AW123Q”
	char car_id;        //车辆类型编码,与home_node里对应
	char car_name[20];  //车辆名称
	char car_gear[10];  //排挡方式
	float fee;     //每日租金
	char id;   //出租状态：'y' or 'n'
	struct charge_node* cnext;   //指向对应的订单
	struct car_node* next;    //指向后面的车
}CAR_NODE;

//第一重结点，主链上的车辆类型分类信息
typedef struct home_node {   
	char car_id;   //车辆类型编码
	char type[20];   //车辆类型名称
	int  num;   //库存数量
	struct car_node* snext;   //指向car_node结点，车辆基本信息
	struct home_node* next;   //指向后一个home_node结点
}HOME_NODE;   //HOME 车库

//按车辆类型出租数量统计情况表
typedef struct rent_status {
	char type[20];  //车辆类型名称
	int  total_num;  //车辆总数
	int  rent_num;   //已出租数量
	struct rent_status* next;
}RENT_STATUS;

//统计某月每种车辆类型的营业额
typedef struct car_fee {
	char type[20];  //车辆类型名称
	float money[12];   //总营业额,12个月
	struct car_fee* next;
}CAR_FEE;

//每辆车的年营业额及租用率,出租天数统计表，适用于两个统计部分
typedef struct car_rent {
	char car_num[10];   //车牌号：如“鄂AW123Q”
	char car_name[20];  //车辆名称
	float money;   //总营业额
	float rent_rate;  //租用率
	int day;  //年累计出租天数
	struct car_rent* next;
}CAR_RENT;


//年租车最多的客户
typedef struct top_rent_guest {
	char guest_name[20];   //客人身份证号
	int  day;    //租车总时间
	struct top_rent_guest* next;
}TOP_RENT_GUEST;


typedef struct date_w
{
	int year;
	int month;
	int day;
}DATE_w;


//屏幕窗口信息链结构
typedef struct layer_node {
	char LayerNo;       //弹出窗口层数
	SMALL_RECT rcArea;  //弹出窗口区域坐标
	CHAR_INFO* pContent; //弹出窗口区域字符单元原信息存储缓冲区
	char* pScrAtt;      //弹出窗口区域字符单元原属性值存储缓冲区
	struct layer_node* next;
}LAYER_NODE;

//标签束结构
typedef struct label_bundle {
	char** ppLabel;   //标签字符串数组首地址
	COORD* pLoc;     //标签定位数组首地址
	int num;         //标签个数
}LABEL_BUNDLE;

//热区结构
typedef struct hot_area {
	SMALL_RECT* pArea;   //热区定位数组首地址
	char* pSort;   //热区类别（按键，文本框，选项框）数组首地址
	char* pTag;    //热区序号数组首地址
	int num;       //热区个数
}HOT_AREA;

LAYER_NODE* gp_top_layer = NULL;  //弹出窗口信息链链头
HOME_NODE* gp_head = NULL;       //主链头指针，不存放数据
HOME_NODE** p_gp_head = &gp_head;   //二级指针，方便后面函数里的修改

//!!!注意，在VS2019中依次点击项目->属性->C/C+±>语言->符合模式，将原来的“是”改为“否”即可
//这里我修改了，不然const char* 类型变量无法赋值给char* ,要么就定义为const char*型，但是可能后面还要修改
//3.1日 我又考虑先不动，如果后面需要修改再按上面的
const char* gp_sys_name = "[车辆租赁系统] Copyright 2020-03 by Bourne and all rights reserved."; //系统名称
char* FileName[3] = { "车辆分类信息表.csv","车辆基本信息表.csv","租车订单信息表.csv" };      /*保存文件*/
char* BeiFen_FileName[3] = { "车辆分类信息表备份.csv","车辆基本信息表备份.csv","租车订单信息表备份.csv" };/*备份文件*/

char* StatFileName[5] = { "Stat1.csv","Stat2.csv","Stat3.csv","Stat4.csv","Stat5.csv" };

const char* gp_car_info_filename = "车辆基本信息表.csv";        //车辆基本信息数据文件
const char* gp_charge_info_filename = "租车订单信息表.csv";     //租车订单信息数据文件
char* gp_home_info_filename = "车辆分类信息表.csv";             //车辆分类信息数据文件

char* ga_main_menu[] = {             //系统主菜单名
	                         "文件(F)",
							 "数据维护(M)",
							 "数据查询(Q)",
							 "数据统计(S)",
							 "帮助(H)" 
                             };

char* ga_sub_menu[] = { "[S]数据保存",   //系统子菜单名
						 "[B]数据备份",
						 "[R]数据恢复",
						 "[X]退出 Alt+X",      //文件模块
	                    
						 "[H]车辆分类信息", 
						 "[C]车辆基本信息",
						 "[R]租车订单信息",     //数据维护模块
	                   
						 "[H]车辆分类信息",     //数据查询模块
						 "[C]车辆基本信息",
						 "[R]租车订单信息",
	           

						 "[A]车型出租数量",          //统计模块
						 "[B]车型月营业额",
						 "[C]年营业额及租用率",
	                     "[D]最多出租车辆",
						 "[E]最常租车客户",
						 "[T]帮助主题F1",     //3个 帮助
	                     "",
						 "[A]关于...", 
                         };


int ga_sub_menu_count[5] = { 4, 3, 3, 5, 3 }; //各主菜单项下子菜单的个数
int gi_sel_menu = 1;     //被选中的主菜单项号,初始为1
int gi_sel_sub_menu = 0; //被选中的子菜单项号,初始为0,表示未选中

unsigned long gul_home_code_len = 0;   //车辆分类信息代码表长度
unsigned long gul_car_code_len = 0;  //车辆基本信息代码表长度
unsigned long gul_charge_code_len = 0;  //租车订单基本信息代码表长度

CHAR_INFO* gp_buff_menubar_info = NULL;  //存放菜单条屏幕区字符信息的缓冲区
CHAR_INFO* gp_buff_stateBar_info = NULL; //存放状态条屏幕区字符信息的缓冲区
HANDLE gh_std_out;   //标准输出设备句柄
HANDLE gh_std_in;    //标准输入设备句柄

char* gp_scr_att = NULL;   //存放屏幕上字符单元属性值的缓冲区
char* gp_home_code = NULL;  //存放车辆分类的数据缓冲区
char* gp_car_code = NULL; //存放车辆基本信息的数据缓冲区
char* gp_charge_code = NULL; //存放租车订单的数据缓冲区
char gc_sys_state = '\0';  //用来保存系统状态的字符


/**<框架函数的声明*/
int CSV_read_HOME(HOME_NODE** phead, char* filename);  //车辆类型读取,先进先出建立链表
int CSV_read_CAR(HOME_NODE** phead, char* filename);  //车辆基本数据读取,先进先出建立链表
int CSV_read_CHARGE(HOME_NODE** phead, char* filename);  //租车订单数据读取,先进先出建立链表

int CreateList(HOME_NODE** phead, char* filename[3]);   //数据链表初始化
void InitInterface(void);   //系统界面初始化
void ClearScreen(void);     //清屏
void ShowMenu(void);     //显示菜单栏
void PopMenu(int num);   //显示下拉菜单
void PopUp(SMALL_RECT*, WORD, LABEL_BUNDLE*, HOT_AREA*);    //弹出窗口屏幕信息维护
void PopOff(void);     //关闭顶层弹出窗口
void DrawBox(SMALL_RECT* parea);   //绘制子菜单边框
void LocSubMenu(int num, SMALL_RECT* peara);   //主菜单下拉菜单定位
void ShowState(void);    //显示状态栏
void TagMainMenu(int num);   //标记被选中的主菜单项
void TagSubMenu(int num);   //标记被选中的子菜单项

int DealInput(HOT_AREA* phot_area, int* pihot);  //控制台输入处理
void SetHotPoint(HOT_AREA* phot_aera, int hot_num); //设置热区
void RunSys(HOME_NODE** pphd);  
BOOL ExeFunction(int main_menu_num, int sub_menu_num);
BOOL CloseSys(); 	//关闭系统并退出
BOOL ShowModule(char** pString, int n);
void  ShowTime(void); 	//动态显示系统时间
inline void Gotoxy(COORD pos);		//将光标置于坐标pos处
inline void Delay(void);		//时间暂延0.05s
inline void  ScreenFresh(void);		//刷屏函数
BOOL LoadData(void);    //数据加载
BOOL SaveData(void);
BOOL SaveSysData(HOME_NODE* phead, char* filename[3]);  //保存系统数据
BOOL BackupData(void);
BOOL RestoreData(void);
void  SetMainHotArea(void);  /**< 给主菜单栏设置热区*/
//BOOL ExitSys(void);    //退出系统




//----------------------------------------------------------------------------------------
//数据维护
BOOL MaintainHomeInfo(void);  //维护车库信息
BOOL MaintainCarInfo(void);  //维护汽车基本信息
BOOL MaintainChargeInfo(void);  //维护租车缴费信息

//车辆分类信息维护
BOOL InsertHome(void);  /*插入分类信息*/
BOOL InsertHomeNode(HOME_NODE** phead, HOME_NODE* ptemp);/*插入分类结点*/
BOOL  DelHome(void);   //删除分类信息
BOOL  DelHomeNode(HOME_NODE** phead, char type[20]);	/*删除分类结点*/
BOOL ModifyHome(void);	 /*修改分类信息*/
BOOL  ModifyHomeNode(HOME_NODE** phead, char type[20], HOME_NODE* phome_node);	/*修改分类结点*/

//车辆基本信息维护
BOOL InsertCar(void);	/*插入车辆基本信息*/
BOOL InsertCarNode(HOME_NODE** phead, CAR_NODE* p);  /*插入车辆基本信息结点*/
BOOL ModifyCar(void);		/*修改车辆基本信息*/
BOOL ModifyCarNode(HOME_NODE** phead, char* car_num, CAR_NODE* pcar_node);	/*修改车辆基本信息结点*/
BOOL DelCar(void);	/*删除车辆基本信息*/
BOOL DelCarNode(HOME_NODE** phead, char* car_num);	/*删除车辆基本信息结点*/

/**<车辆租借订单信息维护*/
BOOL InsertCharge(void);		/*插入车辆租借订单信息*/
BOOL InsertChargeNode(HOME_NODE** phead, CHARGE_NODE* pcharge_node);/*插入车辆租借订单结点*/
BOOL DelCharge(void);	           /*删除车辆租借订单信息*/
BOOL DelChargeNode(HOME_NODE** hd, char* rent_id); /*删除车辆租借订单结点*/
BOOL ModifyCharge(void);	          /*修改车辆租借订单信息*/
BOOL ModifyChargeNode(HOME_NODE** phd, char* rent_id, CHARGE_NODE* pcharge_node);/*修改车辆租借订单结点*/


//查找等其他辅助函数
int leapyear(int year);   //闰年
int compare(DATE_w* d1, DATE_w* d2);		//如果第一个日期比第二个日期大，交换日期
int diff(DATE_w* date1, DATE_w* date2);
int diff_day(char time1[], char time2[]);   //计算日期相差的时间，必须输入12位标准时间字符串
char* GetTime(void);    //获取时间
int time_test(char time[]);   //输入时间测试
void SortHomeNode(HOME_NODE** phead);  //为了csv里数据是顺序排列写的一个排序函数，存入文件前先排序
HOME_NODE* SeekHomeNodeByCarID(HOME_NODE* phead, char car_id);
HOME_NODE* SeekHomeNodeByCarName(HOME_NODE* phead, char type[20]);
CAR_NODE* SeekCarNodeByCarNum(HOME_NODE* phead, char* car_num);   //用车牌号查找 //查找车辆基本信息结点
CAR_NODE* SeekCarNodeByNumber(HOME_NODE* phead, int number);   //用车辆编号查找
CHARGE_NODE* SeekChargeNodeByRentId(HOME_NODE* phead, char* rent_id);  //根据订单号查找订单节点
CHARGE_NODE* SeekChargeNode(HOME_NODE* phead, char* rent_id, char* car_num);/*查找车辆租借订单结点*/
//void ChangeNum(HOME_NODE** phead);   //在车辆基本信息维护后，重新对车辆编号
void ChangeID(HOME_NODE** phead);
BOOL ChangeCarNumber(HOME_NODE** phead);

//----------------------------------------------------------------------------------------
//数据查询
BOOL QueryHomeInfo(void);  //查询车库信息
void QueryHomeInfoNode1(HOME_NODE* head, char type[20]);
void QueryHomeInfoNode2(HOME_NODE* head);
void QueryHomeInfoNode3(HOME_NODE* head);

BOOL QueryCarInfo(void);  //查询汽车基本信息
void QueryCarInfoNode1(HOME_NODE* head);
void QueryCarInfoNode2(HOME_NODE* head);
void QueryCarInfoNode3(HOME_NODE* head);
void QueryCarInfoNode4(HOME_NODE* head);
char* ChangeCarInfoToStr(CAR_NODE* pcarnode);  //将汽车基本信息转化为字符串，用逗号分隔
BOOL satisfy(CAR_NODE* pcarnode, char mixsearch[]);

BOOL QueryChargeInfo(void);  //查询租车缴费信息
void QueryChargeInfoNode1(HOME_NODE* head);
void QueryChargeInfoNode2(HOME_NODE* head);
char* del_the_blank(char* str);  //处理租车订单里的时间，把/和空格,-，：删除
char* pad(char* str);   //填充为标准时间;
char* func(char* str);   //直接读入一个随意有分隔符的字符串，添加为标准12位时间;
void QueryChargeInfoNode3(HOME_NODE* head);


//Part4 统计        
BOOL Stat_Car_Type_Rent_Num(void);    //车型出租数量统计
void Stat_Car_Type_Rent_Num_Info(HOME_NODE* phead, RENT_STATUS** phead1);
BOOL SaveStat1Data(RENT_STATUS* phead);
void FreeSpace1(RENT_STATUS* phead);

BOOL Stat_Car_Type_Month_Fee(void);    //车型月营业额统计
void Stat_Car_Type_Month_Fee_Info(HOME_NODE* phead, CAR_FEE** phead1, char year[5]);
BOOL SaveStat2Data(CAR_FEE* phead, char year[5]);
void FreeSpace2(CAR_FEE* phead);

BOOL Stat_Car_Fee_and_rent_rate(void);    //年营业额及租用率统计
BOOL SaveStat3Data(CAR_RENT* phead, char year[5]);
void FreeSpace3(CAR_RENT* phead);
void Stat_Car_Fee_and_rent_rate_Info(HOME_NODE* phead, CAR_RENT** phead1, char year[5]);

BOOL Stat_Car_Most_Rent(void);  //最多出租车辆统计
BOOL SaveStat4Data(CAR_RENT* phead, char year[5]);
void Stat_Car_Most_Rent_Info(HOME_NODE* phead, CAR_RENT** phead1, char year[5]);
void SortCarMostRentNode(CAR_RENT** phead);

BOOL Stat_Guest_Most_Rent(void);  //最常租车客户统计
BOOL SaveStat5Data(TOP_RENT_GUEST* phead, char year[5]);
void FreeSpace5(TOP_RENT_GUEST* phead);
void Stat_Guest_Most_Rent_Info(HOME_NODE* phead, TOP_RENT_GUEST** phead1, char year[5]);
TOP_RENT_GUEST* SeekNode(TOP_RENT_GUEST* phead, char name[20]);
int countday(char time1[], char time2[], char year[5]);

//Part5 帮助
BOOL HelpTopic(void);  //帮助主题
BOOL AboutSys(void);  //关于系统

#endif // !TYPE_H_INCLUDED

