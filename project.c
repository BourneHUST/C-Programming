//project是最后的文档，car.h是头文件，另外的dorm和main是教材中的参考
#include "car.h"
unsigned long ul;     //指向变量的指针, 用来存放字符的实际数目

int main()
{
	InitInterface();  //文本菜单界面初始化
	LoadData();   //加载数据到内存
	RunSys(&gp_head);    /*系统功能模块的选择及运行*/
	CloseSys(gp_head);         /*关闭系统*/
	return 0;
}

/**
*函数名称：LoadData
*函数功能：将三类基础数据从数据文件载入到内存缓冲区和十字链表中
*输入参数：无
*输出参数：无
*返回值：BOOL,功能函数中除ExitSys的返回值可以为FALSE外，其他的必须为True
*调用说明：为了统一调用各功能函数，原型保持一致，无参数且返回BOOL，返回FALSE时程序结束运行
*/
BOOL LoadData(void)
{
	int Re = 0;
	Re = CreateList(&gp_head,FileName);
	if (Re < 28)
	{
		printf("\n数据不完整！\n");
	}
	printf("\n所有数据加载成功，按任意键继续\n");
	getch();
	ClearScreen();
	ShowMenu();
	return TRUE;
}

/**
*函数名称：CSV_read_HOME
*函数功能：读取车辆分类信息文件
*输入参数：phead主链头指针地址，用来建立主链，filename文件名
*输出参数：无
*返回值：int ,0 表示成功，-1表示失败
*调用说明：
*/
int CSV_read_HOME(HOME_NODE** phead,char* filename)  //车辆类型读取,先进先出建立链表
{
	HOME_NODE* hd = (HOME_NODE*)malloc(sizeof(HOME_NODE));   //这是个头节点
	HOME_NODE* tail = hd;
	FILE* fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("打开文件失败！\n");
		return -1;
	}
	//printf("车辆分类信息文件打开成功！\n");
	char line[1024];
	fgets(line, sizeof(line), fp);  //第一行不读
	while (fgets(line, sizeof(line), fp))
	{
		HOME_NODE* TEST = (HOME_NODE*)malloc(sizeof(HOME_NODE));
		TEST->snext = NULL;
		TEST->next = NULL;
		char* save_ptr;
		char* p = strtok_s(line, ",", &save_ptr);
		if (p == NULL) {
			return -1;
		}
		TEST->car_id = p[0];
		p = strtok_s(NULL, ",", &save_ptr);
		if (p == NULL) {
			return -1;
		}
		strcpy(TEST->type, p);
		p = strtok_s(NULL, ",", &save_ptr);
		if (p == NULL) {
			return -1;
		}
		TEST->num = atoi(p);
		tail->next = TEST;
		tail = TEST;
		//printf("%c %s %d\n", TEST->car_id, TEST->type, TEST->num);
	}
	tail->next = NULL;

	if (hd == NULL)
	{
		printf("车辆分类信息文件加载失败！\n");
		return -1;
	}
	*phead = hd;     //phead是头节点，没有数据
	fclose(fp);
	return 0;
}

/**
*函数名称：CSV_read_HOME
*函数功能：读取车辆基本信息文件
*输入参数：phead主链头指针地址，用来建立主链，filename文件名
*输出参数：无
*返回值：int ,0 表示成功，-1表示失败
*调用说明：
*/
int CSV_read_CAR(HOME_NODE** phead, char* filename)  //车辆基本数据读取,先进先出建立链表
{
	HOME_NODE* pHomeNode = (HOME_NODE*)malloc(sizeof(HOME_NODE));
	pHomeNode->next = NULL;
	pHomeNode->snext = NULL;
	FILE* fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("失败！\n");
		return -1;
	}
	char line[1024];
	fgets(line, sizeof(line), fp);  //第一行不读
	while (fgets(line, sizeof(line), fp))
	{
		CAR_NODE* pCarNode = (CAR_NODE*)malloc(sizeof(CAR_NODE));
		pCarNode->cnext = NULL;
		pCarNode->next = NULL;
		char* save_ptr;
		char* p = strtok_s(line, ",", &save_ptr);
		if (p == NULL) {
			return -1;
		}
		pCarNode->number = atoi(p);
		p = strtok_s(NULL, ",", &save_ptr);
		if (p == NULL) {
			return -1;
		}
		strcpy(pCarNode->car_num, p);
		p = strtok_s(NULL, ",", &save_ptr);
		if (p == NULL) {
			return -1;
		}
		pCarNode->car_id = p[0];
		p = strtok_s(NULL, ",", &save_ptr);
		if (p == NULL) {
			return -1;
		}
		strcpy(pCarNode->car_name, p);
		p = strtok_s(NULL, ",", &save_ptr);
		if (p == NULL) {
			return -1;
		}
		strcpy(pCarNode->car_gear, p);
		p = strtok_s(NULL, ",", &save_ptr);
		if (p == NULL) {
			return -1;
		}
		pCarNode->fee = (float)(atoi(p));

		p = strtok_s(NULL, ",", &save_ptr);
		if (p == NULL) {
			return -1;
		}
		pCarNode->id = p[0];

		pHomeNode = (*phead)->next;
		while (pHomeNode != NULL && pHomeNode->car_id != pCarNode->car_id)
		{
			pHomeNode = pHomeNode->next;
		}
		if (pHomeNode != NULL)
		{
			pCarNode->next = pHomeNode->snext;
			pHomeNode->snext = pCarNode;
		}
		else  //如果未找到，释放结点空间
		{
			free(pCarNode);
		}
	}
	fclose(fp);
	return 0;
}

/**
*函数名称：CSV_read_HOME
*函数功能：读取租车订单文件
*输入参数：phead主链头指针地址，用来建立主链，filename文件名
*输出参数：无
*返回值：int ,0 表示成功，-1表示失败
*调用说明：
*/
int CSV_read_CHARGE(HOME_NODE** phead, char* filename)  //租车订单数据读取,先进先出建立链表
{
	HOME_NODE* pHomeNode = (HOME_NODE*)malloc(sizeof(HOME_NODE));
	CAR_NODE* pCarNode = (CAR_NODE*)malloc(sizeof(CAR_NODE));
	FILE* fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("失败！\n");
		return -1;
	}
	char line[1024];
	fgets(line, sizeof(line), fp);  //第一行不读
	while (fgets(line, sizeof(line), fp))
	{
		//创建结点，存放文件中读取的租车订单信息
		CHARGE_NODE* pChargeNode = (CHARGE_NODE*)malloc(sizeof(CHARGE_NODE));
		pChargeNode->next = NULL;

		char* save_ptr;
		char* p = strtok_s(line, ",", &save_ptr);
		if (p == NULL) {
			return -1;
		}
		strcpy(pChargeNode->rent_id, p);
		p = strtok_s(NULL, ",", &save_ptr);
		if (p == NULL) {
			return -1;
		}
		strcpy(pChargeNode->guest_id, p);
		p = strtok_s(NULL, ",", &save_ptr);
		if (p == NULL) {
			return -1;
		}
		strcpy(pChargeNode->name, p);
		p = strtok_s(NULL, ",", &save_ptr);
		if (p == NULL) {
			return -1;
		}
		strcpy(pChargeNode->tel, p);
		p = strtok_s(NULL, ",", &save_ptr);
		if (p == NULL) {
			return -1;
		}
		pChargeNode->number = atoi(p);
		p = strtok_s(NULL, ",", &save_ptr);
		if (p == NULL) {
			return -1;
		}
		strcpy(pChargeNode->time_of_get_car, p);
		p = strtok_s(NULL, ",", &save_ptr);
		if (p == NULL) {
			return -1;
		}
		strcpy(pChargeNode->time_of_scheduled_back, p);


		p = strtok_s(NULL, ",", &save_ptr);
		if (p == NULL) {
			return -1;
		}
		pChargeNode->deposit = (float)atoi(p);

		p = strtok_s(NULL, ",", &save_ptr);
		if (p == NULL) {
			return -1;
		}
		strcpy(pChargeNode->time_of_actual_back, p);

		p = strtok_s(NULL, ",", &save_ptr);
		if (p == NULL) {
			return -1;
		}
		pChargeNode->should_pay = (float)atoi(p);
		p = strtok_s(NULL, ",", &save_ptr);
		if (p == NULL) {
			return -1;
		}
		pChargeNode->actual_pay = (float)atoi(p);

		//查找该车辆信息支链上对应的租车订单结点
		pHomeNode = (*phead)->next;
		int find = 0;
		while (pHomeNode != NULL && find == 0)
		{
			pCarNode = pHomeNode->snext;
			while (pCarNode != NULL && find == 0)
			{
				if (pCarNode->number == pChargeNode->number)  //通过车辆编号查找
				{
					find = 1;
					break;
				}
				pCarNode = pCarNode->next;
			}
			pHomeNode = pHomeNode->next;
		}

		//如果找到，则该节点以先进后出方式插入学生信息支链
		if (find)
		{
			pChargeNode->next = pCarNode->cnext;
			pCarNode->cnext = pChargeNode;
			//printf("已经开始读取第三支：\n%s\n", pChargeNode->rent_id);
		}
		else  //如果未找到，释放结点空间
		{
			free(pChargeNode);
		}
	}
	fclose(fp);
	//printf("租车订单打开成功！\n");
	return 0;
}

/**
*函数名称：CreateList
*函数功能：从数据文件读取基础数据，并存放到所创建的十字链表中
*输入参数：二级指针
*输出参数：phead主链头指针的地址，用来返回所创建的十字链
*返回值：int 型数值，表示链表创建的情况
		  0，空链，无数据
		  4 已加载车辆分类信息数据，无车辆基本信息和租车订单数据
		  12 已加载车辆分类信息数据和车辆基本信息，无租车订单数据
		  28 已加载三项基础数据
*调用说明：无
*/
int CreateList(HOME_NODE** phead,char* filename[3])
{
	int re = 0;  //加载结果
	if (CSV_read_HOME(phead, filename[0]) == 0)   //读取成功
	{
		re += 4;
		printf("\n车辆分类信息文件打开成功！\n");
	}
	else
		return re;

	if (CSV_read_CAR(phead, filename[1]) == 0)
	{
		printf("车辆基本信息文件打开成功！\n");
		re += 8;
	}
	else
		return re;

	if (CSV_read_CHARGE(phead, filename[2]) == 0)
	{
		printf("租车订单信息文件打开成功！\n");
		re += 16;
	}

	ChangeCarNumber(&gp_head);
	SaveSysData(gp_head,FileName);
	return re;
}

/**
*函数名称：InitInterface
*函数功能：文本界面初始化
*输入参数：无
*输出参数：无
*返回值：无
*调用说明：
*/
void InitInterface(void)
{
	COORD size = { SCR_COL,SCR_ROW };  //窗口缓冲区大小
	WORD att =  BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED; //设置前景，背景属性  
	gh_std_out = GetStdHandle(STD_OUTPUT_HANDLE);  //获取标准输出设备句柄
	gh_std_in = GetStdHandle(STD_INPUT_HANDLE);    //获取标准输入设备句柄
	SetConsoleTitle(gp_sys_name);    //设置窗口标题
	SetConsoleScreenBufferSize(gh_std_out, size);  //设置窗口缓冲区大小80*25
	SetConsoleTextAttribute(gh_std_out, att);   //设置黄色前景和蓝色背景  //可省略
	ClearScreen();   //清屏

	//创建弹出窗口信息堆栈，将初始化后的额屏幕窗口当作第一层弹出窗口
	gp_scr_att = (char*)calloc(SCR_COL * SCR_ROW, sizeof(char));//屏幕字符属性
	gp_top_layer = (LAYER_NODE*)malloc(sizeof(LAYER_NODE));//动态创建gp_top_layer结点
	gp_top_layer->LayerNo = 0;      //弹出窗口的层号为0
	gp_top_layer->rcArea.Left = 0;  //弹出窗口的区域为整个屏幕窗口
	gp_top_layer->rcArea.Top = 0;   //弹出窗口区域位置初始化
	gp_top_layer->rcArea.Right = SCR_COL - 1;
	gp_top_layer->rcArea.Bottom = SCR_ROW - 1;
	gp_top_layer->pContent = NULL;  //弹出窗口区域字符单元原信息存储缓冲区初始值为NULL
	gp_top_layer->pScrAtt = gp_scr_att; //gp_scr_att指向弹出窗口区域字符单元原属性值存储缓冲区信息
	gp_top_layer->next = NULL; //指向的下一层窗口信息为NULL
	
	ShowMenu();   //显示主菜单栏
	ShowState();   //显示状态栏
	return;
}

/**
*函数名称：ClearScreen
*函数功能：清除屏幕信息
*输入参数：无
*输出参数：无
*返回值：无
*调用说明：
*/
void ClearScreen(void)
{
	CONSOLE_SCREEN_BUFFER_INFO bInfo;
	COORD home = { 0,0 };
	unsigned long size;
	GetConsoleScreenBufferInfo(gh_std_out, &bInfo);   //取屏幕缓冲区信息
	size = bInfo.dwSize.X * bInfo.dwSize.Y;   //计算屏幕缓冲区字符单元数
	//将屏幕缓冲区所有单元的字符属性设置为当前屏幕缓冲区字符属性
	FillConsoleOutputAttribute(gh_std_out, bInfo.wAttributes, size, home, &ul);
	//将屏幕缓冲区所有单元填充为空格字符
	FillConsoleOutputCharacter(gh_std_out, ' ', size, home, &ul);
	return;
}

/**
*函数名称：ShowMenu
*函数功能：在屏幕上显示主菜单，并设置热区，在主菜单上第一项上置选中标记
*输入参数：无
*输出参数：无
*返回值：无
*调用说明：
*/
void ShowMenu()
{
	CONSOLE_SCREEN_BUFFER_INFO bInfo;
	CONSOLE_CURSOR_INFO lpCur;
	COORD size;
	COORD pos = { 0,0 };
	int i, j;
	int PosA = 2, PosB;
	char ch;
	GetConsoleScreenBufferInfo(gh_std_out, &bInfo); //获取屏幕缓冲信息，存入binfo中
	size.X = bInfo.dwSize.X;
	size.Y = 1;
	SetConsoleCursorPosition(gh_std_out, pos);  //设置光标到（0，0）
	for (i = 0; i < 5; i++)  //在窗口第一行第一列输出主菜单项
	{
		printf("%s      ", ga_main_menu[i]);
	}
	GetConsoleCursorInfo(gh_std_out, &lpCur);
	lpCur.bVisible = FALSE;       //隐藏光标
	SetConsoleCursorInfo(gh_std_out, &lpCur); 

	//申请动态存储器作为存放菜单条屏幕区字符信息的缓冲区
	gp_buff_menubar_info = (CHAR_INFO*)malloc(size.X * size.Y * sizeof(CHAR_INFO));
	SMALL_RECT rcMenu = { 0,0,size.X - 1,0 };  //设置缓冲区范围
	//将窗口第一行的内容读入到存放菜单单条屏幕区字符信息的缓冲区中
	ReadConsoleOutput(gh_std_out, gp_buff_menubar_info, size, pos, &rcMenu);
	//将这一行中英文字母设置为红色，其他字符单元设置为白底黑字
	for (i = 0; i < size.X; i++)
	{
		(gp_buff_menubar_info + i)->Attributes = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;
		ch = (char)((gp_buff_menubar_info + i)->Char.AsciiChar);
		if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
		{
			(gp_buff_menubar_info + i)->Attributes |= FOREGROUND_RED;
		}
	}
	//修改后的菜单条字符信息回写到窗口的第一行
	WriteConsoleOutput(gh_std_out, gp_buff_menubar_info, size, pos, &rcMenu);

	COORD endPos = { 0,1 };
	SetConsoleCursorPosition(gh_std_out, endPos); //将光标位置设置在第二行第一列

	//将菜单项设置为热区，热区编号为菜单项号，热区类型为0（按钮型）
	i = 0;
	do
	{
		PosB = PosA + strlen(ga_main_menu[i]);  /*定位第i+1号菜单项的起止位置*/
		for (j = PosA; j < PosB; j++)
		{
			gp_scr_att[j] |= (i + 1) << 2; /*设置菜单项所在字符单元的属性值*/
		}
		PosA = PosB + 4;
		i++;
	} while (i < 5);

	TagMainMenu(gi_sel_menu);   //在选中主菜单上做标记，gi_sel_menu初值为1
	return;
}

/**
*函数名称：PopMenu
*函数功能：弹出指定主菜单项对应的子菜单
*输入参数：num指定的主菜单号
*输出参数：无
*返回值：无
*调用说明：
*/
void PopMenu(int num)
{
	LABEL_BUNDLE labels;
	HOT_AREA areas;
	SMALL_RECT rcPop, * aArea;
	COORD pos, * aLoc;
	WORD att;
	int i, j, loc = 0;
	char* aSort, * aTag;/*aSort:热区类别 aTag:热区编号*/
	char* pch;

	if (num != gi_sel_menu)    /*如果指定主菜单不是已选中菜单*/
	{
		if (gp_top_layer->LayerNo != 0) /*如果此前已有子菜单弹出*/
		{
			PopOff();/*关闭该弹出窗口*/
			gi_sel_sub_menu = 0;/*将被选中的子菜单项号置为0*/
		}
	}
	else if (gp_top_layer->LayerNo != 0) /*若已弹出该子菜单，则返回*/
	{
		return;
	}

	gi_sel_menu = num;               /*将选中主菜单项置为指定的主菜单项*/
	TagMainMenu(gi_sel_menu);        /*在选中的主菜单项上做标记*/
	LocSubMenu(gi_sel_menu, &rcPop); /*计算弹出子菜单的区域位置, 存放在rcPop中*/
	/*计算该子菜单中的第一项在子菜单字符串数组中的位置(下标)*/
	for (i = 1; i < gi_sel_menu; i++)
	{
		loc += ga_sub_menu_count[i - 1];
	}
	/*将该组子菜单项项名存入标签束结构变量*/
	labels.ppLabel = ga_sub_menu + loc;   /*标签束第一个标签字符串的地址*/
	labels.num = ga_sub_menu_count[gi_sel_menu - 1]; /*标签束中标签字符串的个数*/
	/*定义一个坐标数组，存放每个标签字符串输出位置的坐标等价于:COORD aLoc[labels.num];*/
	aLoc = (COORD*)malloc(labels.num * sizeof(COORD));
	for (i = 0; i < labels.num; i++) /*确定标签字符串的输出位置，存放在坐标数组中*/
	{
		aLoc[i].X = rcPop.Left + 2;
		aLoc[i].Y = rcPop.Top + i + 1;
	}
	labels.pLoc = aLoc; /*使标签束结构变量labels的成员pLoc指向坐标数组的首元素*/
	/*设置子菜单热区信息*/
	areas.num = labels.num;       /*热区的个数，等于标签的个数，即子菜单的项数*/

	aArea = (SMALL_RECT*)malloc(areas.num * sizeof(SMALL_RECT));/*定义数组存放所有热区位置等价于:SMALL_RECT aArea[areas.num];*/
	aSort = (char*)malloc(areas.num * sizeof(char));    /*定义数组存放所有热区对应类别等价于:char aSort[areas.num];*/
	aTag = (char*)malloc(areas.num * sizeof(char));    /*定义数组存放每个热区的编号:char aTag[areas.num];*/
	/*热区定位*/
	for (i = 0; i < areas.num; i++)
	{/*各热区范围*/
		aArea[i].Left = rcPop.Left + 2;
		aArea[i].Top = rcPop.Top + i + 1;
		aArea[i].Right = rcPop.Right - 2;
		aArea[i].Bottom = aArea[i].Top;
		aSort[i] = 0;     /*热区类别都为0(按钮型)*/
		aTag[i] = i + 1; /*热区按顺序编号1,2,3,4...*/
	}
	areas.pArea = aArea;  /*使热区结构变量areas的成员pArea指向热区位置数组首元素*/
	areas.pSort = aSort;  /*使热区结构变量areas的成员pSort指向热区类别数组首元素*/
	areas.pTag = aTag;    /*使热区结构变量areas的成员pTag指向热区编号数组首元素*/
	att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*子菜单字符属性:白底黑字*/
	PopUp(&rcPop, att, &labels, &areas);
	DrawBox(&rcPop);       /*给弹出窗口画边框*/
	pos.X = rcPop.Left + 2;
	for (pos.Y = rcPop.Top + 1; pos.Y < rcPop.Bottom; pos.Y++)
	{ /*此循环用来在空串子菜项位置画线形成分隔，并取消此菜单项的热区属性*/
		pch = ga_sub_menu[loc + pos.Y - rcPop.Top - 1];
		if (strlen(pch) == 0) /*串长为0，表明为空串*/
		{   /*首先画横线*/
			FillConsoleOutputCharacter(gh_std_out, '-', rcPop.Right - rcPop.Left - 3, pos, &ul);
			for (j = rcPop.Left + 2; j < rcPop.Right - 1; j++)
			{   /*取消该区域字符单元的热区属性*/
				gp_scr_att[pos.Y * SCR_COL + j] &= 3; /*按位与的结果保留了低两位*/
			}
		}
	}
	/*将子菜单项的功能键设为白底红字*/
	pos.X = rcPop.Left + 3;
	att = FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;
	for (pos.Y = rcPop.Top + 1; pos.Y < rcPop.Bottom; pos.Y++)
	{
		if (strlen(ga_sub_menu[loc + pos.Y - rcPop.Top - 1]) == 0)
		{
			continue;  /*跳过空串*/
		}
		FillConsoleOutputAttribute(gh_std_out, att, 1, pos, &ul);
	}
	return;
}

/**
*函数名称：CloseSys
*函数功能：关闭系统,释放动态存储区
*输入参数：无
*输出参数：无
*返回值：无
*调用说明：
*/
BOOL CloseSys()
{
	HOME_NODE* phome1 = gp_head, *phome2;
	CAR_NODE* pcar1, *pcar2;
	CHARGE_NODE* pcharge1, * pcharge2;
	/*释放十字交叉链表的动态存储区*/
	while (phome1 != NULL)
	{
		phome2 = phome1->next;
		pcar1 = phome1->snext;
		while (pcar1 != NULL)     /*释放车辆信息支链的动态存储区*/
		{
			pcar2 = pcar1->next;
			pcharge1 = pcar1->cnext;
			while (pcharge1 != NULL) /*释放租车订单支链的动态存储区*/
			{
				pcharge2 = pcharge1->next;
				free(pcharge1);
				pcharge1= pcharge2;
			}
			free(pcar1);
			pcar1 = pcar2;
		}
		free(phome1);            /*释放航班主链结点的动态存储区*/
		phome1 = phome2;
	}

	ClearScreen();     /*清除屏幕信息*/
	/*释放存放菜单条、状态条等信息动态存储区*/
	free(gp_buff_menubar_info);
	free(gp_buff_stateBar_info);
	/*关闭标准输入和输出设备句柄*/
	CloseHandle(gh_std_out);
	CloseHandle(gh_std_in);
	//将窗口标题栏设置为结束
	SetConsoleTitle("运行结束");
	return TRUE;
}

/**
*函数名称：RunSys
*函数功能：运行系统,在系统界面下运行用户所选择的功能函数
*输入参数：phead主链头指针地址
*输出参数：无
*返回值：无
*调用说明：无
*/
void RunSys(HOME_NODE** phead)
{
	INPUT_RECORD inRec;
	DWORD res;
	COORD pos = { 0, 0 };
	BOOL bRet = TRUE;
	int i, loc, num;
	int cNo, cAtt;      /*cNo:字符单元层号, cAtt:字符单元属性*/
	char vkc, asc;      /*vkc:虚拟键代码, asc:字符的ASCII码值*/

	SetMainHotArea();   /*给主菜单栏设置热区*/

	while (bRet)
	{  /*从控制台输入缓冲区中读一条记录*/
		ReadConsoleInput(gh_std_in, &inRec, 1, &res);
		if (inRec.EventType == MOUSE_EVENT) /*如果记录由鼠标事件产生*/
		{
			pos = inRec.Event.MouseEvent.dwMousePosition;    /*获取鼠标坐标位置*/
			cNo = gp_scr_att[pos.Y * SCR_COL + pos.X] & 3;   /*取该位置的层号*/
			cAtt = gp_scr_att[pos.Y * SCR_COL + pos.X] >> 2; /*取该字符单元属性*/
			if (cNo == 0)  /*层号为0,表明该位置未被弹出子菜单覆盖*/
			{
				/*cAtt>0表明该位置处于热区,cAtt!=gi_sel_menu表明该位置的主菜单项未被选中,gp_top_layer->LayerNo>0表明当前有子菜单弹出*/
				if (cAtt > 0 && cAtt != gi_sel_menu && gp_top_layer->LayerNo > 0)
				{
					PopOff();            /*关闭弹出的子菜单*/
					gi_sel_sub_menu = 0; /*将选中子菜单项的项号置为0*/
				}
				else  if (cAtt > 0)
					TagMainMenu(cAtt); /*在鼠标所在主菜单项上做选中标记*/
			}
			else if (cAtt > 0) /*鼠标所在位置为弹出子菜单的菜单项字符单元*/
				TagSubMenu(cAtt); /*在该子菜单项上做选中标记*/
			if (inRec.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) /*如果按下鼠标左键一次*/
			{
				if (cNo == 0) /*层号为0，表明该位置未被弹出子菜单覆盖*/
				{
					if (cAtt > 0) /*如果该位置处于热区*/
						PopMenu(cAtt);   /*弹出鼠标所在主菜单项对应的子菜单*/
					else if (gp_top_layer->LayerNo > 0) /*如果该位置不属于主菜单项字符单元,且有子菜单弹出*/
					{
						PopOff();             /*关闭弹出的子菜单*/
						gi_sel_sub_menu = 0;  /*将选中子菜单项的项号置为0*/
					}
				}
				else /*层号不为0,表明该位置被弹出子菜单覆盖*/
				{
					if (cAtt > 0) /*如果该位置处于热区*/
					{
						PopOff();  /*关闭弹出的子菜单*/
						gi_sel_sub_menu = 0;  /*将选中子菜单项的项号置为0*/
						bRet = ExeFunction(gi_sel_menu, cAtt); /*执行对应功能函数:gi_sel_menu主菜单项号,cAtt子菜单项号*/
					}
				}
			}
			else if (inRec.Event.MouseEvent.dwButtonState == RIGHTMOST_BUTTON_PRESSED) /*如果按下鼠标右键*/
			{
				if (cNo == 0) /*层号为0，表明该位置未被弹出子菜单覆盖*/
				{
					PopOff();            /*关闭弹出的子菜单*/
					gi_sel_sub_menu = 0; /*将选中子菜单项的项号置为0*/
				}
			}
		}
		else if (inRec.EventType == KEY_EVENT && inRec.Event.KeyEvent.bKeyDown) /*如果记录由按键产生且键被按下*/
		{
			vkc = inRec.Event.KeyEvent.wVirtualKeyCode;  /*获取按键的虚拟键码*/
			asc = inRec.Event.KeyEvent.uChar.AsciiChar;  /*获取按键的ASC码*/
			/*系统帮助快捷键F1键的处理*/
			if (vkc == 112) /*如果按下F1键*/
			{
				if (gp_top_layer->LayerNo != 0) /*如果当前有子菜单弹出*/
				{
					PopOff();                   /*关闭弹出的子菜单*/
					gi_sel_sub_menu = 0;        /*将选中子菜单项的项号置为0*/
				}
				bRet = ExeFunction(5, 1);       /*运行帮助主题功能函数*/
			}
			else if (inRec.Event.KeyEvent.dwControlKeyState & (LEFT_ALT_PRESSED | RIGHT_ALT_PRESSED))
			{ /*如果按下左或右Alt键*/
				switch (vkc)  /*判断组合键Alt+字母*/
				{
				case 88:  /*Alt+X 退出*/
					if (gp_top_layer->LayerNo != 0)
					{
						PopOff();
						gi_sel_sub_menu = 0;
					}
					bRet = ExeFunction(1, 4);
					break;
				case 70:  /*Alt+F*/
					PopMenu(1);
					break;
				case 77: /*Alt+M*/
					PopMenu(2);
					break;
				case 81: /*Alt+Q*/
					PopMenu(3);
					break;
				case 83: /*Alt+S*/
					PopMenu(4);
					break;
				case 72: /*Alt+H*/
					PopMenu(5);
					break;
				}
			}
			else if (asc == 0) /*其他控制键的处理*/
			{
				if (gp_top_layer->LayerNo == 0) /*如果未弹出子菜单*/
				{
					switch (vkc) /*处理方向键(左、右、下),不响应其他控制键*/
					{
					case 37:  /*左键*/
						gi_sel_menu--;
						if (gi_sel_menu == 0)  gi_sel_menu = 5;
						TagMainMenu(gi_sel_menu);
						break;
					case 39: /*右键*/
						gi_sel_menu++;
						if (gi_sel_menu == 6)  gi_sel_menu = 1;
						TagMainMenu(gi_sel_menu);
						break;
					case 40: /*下键*/
						PopMenu(gi_sel_menu);
						TagSubMenu(1);
						break;
					}
				}
				else  /*已弹出子菜单时*/
				{
					/*计算该子菜单中的第一项在子菜单字符串数组中的位置(下标)*/
					for (loc = 0, i = 1; i < gi_sel_menu; i++)
						loc += ga_sub_menu_count[i - 1];
					switch (vkc) /*方向键(左、右、上、下)的处理*/
					{
					case 37: /*左键*/
						gi_sel_menu--;
						if (gi_sel_menu < 1)   gi_sel_menu = 5;
						TagMainMenu(gi_sel_menu);
						PopOff();
						PopMenu(gi_sel_menu);
						TagSubMenu(1);
						break;
					case 38: /*上键*/
						num = gi_sel_sub_menu - 1;
						if (num < 1)    num = ga_sub_menu_count[gi_sel_menu - 1];
						if (strlen(ga_sub_menu[loc + num - 1]) == 0)    num--;
						TagSubMenu(num);
						break;
					case 39:  /*右键*/
						gi_sel_menu++;
						if (gi_sel_menu > 5)     gi_sel_menu = 1;
						TagMainMenu(gi_sel_menu);
						PopOff();
						PopMenu(gi_sel_menu);
						TagSubMenu(1);
						break;
					case 40: /*下键*/
						num = gi_sel_sub_menu + 1;
						if (num > ga_sub_menu_count[gi_sel_menu - 1])  num = 1;
						if (strlen(ga_sub_menu[loc + num - 1]) == 0)     num++;
						TagSubMenu(num);
						break;
					}
				}
			}
			else if ((asc - vkc == 0) || (asc - vkc == 32)) {  /*按下数字键或者字母键等普通键*/
				if (gp_top_layer->LayerNo == 0)  /*如果未弹出子菜单*/
				{
					switch (vkc)
					{
					case 70: /*f或F*/
						PopMenu(1);
						break;
					case 77: /*M或m*/
						PopMenu(2);
						break;
					case 81: /*Q或q*/
						PopMenu(3);
						break;
					case 83: /*s或S*/
						PopMenu(4);
						break;
					case 72: /*h或H*/
						PopMenu(5);
						break;
					case 13: /*回车*/
						PopMenu(gi_sel_menu);
						TagSubMenu(1);
						break;
					}
				}
				else /*已弹出子菜单时的键盘输入处理*/
				{
					if (vkc == 27) /*如果按下ESC键*/
					{
						PopOff();
						gi_sel_sub_menu = 0;
					}
					else if (vkc == 13) /*如果按下回车键*/
					{
						num = gi_sel_sub_menu;
						PopOff();
						gi_sel_sub_menu = 0;
						bRet = ExeFunction(gi_sel_menu, num);
					}
					else /*其他普通键的处理*/
					{
						/*计算该子菜单中的第一项在子菜单字符串数组中的位置(下标)*/
						for (loc = 0, i = 1; i < gi_sel_menu; i++)
							loc += ga_sub_menu_count[i - 1];
						/*依次与当前子菜单中每一项的代表字符进行比较*/
						for (i = loc; i < loc + ga_sub_menu_count[gi_sel_menu - 1]; i++)
						{
							if (strlen(ga_sub_menu[i]) > 0 && vkc == ga_sub_menu[i][1])
							{ /*如果匹配成功*/
								PopOff();
								gi_sel_sub_menu = 0;
								bRet = ExeFunction(gi_sel_menu, i - loc + 1); /*执行与字母相对应的函数*/
							}
						}
					}
				}
			}
		}
	}
}

/**
*函数名称：PopUp
*函数功能：在指定区域弹出窗口信息并设置热区，将弹出窗口位置信息入栈
*输入参数：pRc 弹出窗口位置数据存放的地址，att弹出窗口区域字符属性
*          pLable 弹出窗口中标签束信息存放的地址
*		   pHotArea 弹出窗口中热区信息存放的地址
*输出参数：无
*返回值：无
*调用说明：
*/
void PopUp(SMALL_RECT* pRc, WORD att, LABEL_BUNDLE* pLabel, HOT_AREA* pHotArea)
{
	LAYER_NODE* nextLayer;  /*弹出窗口信息结构变量指针*/
	COORD size;
	COORD pos = { 0, 0 };
	char* pCh;
	int i, j, row;
	/*弹出窗口所在位置字符单元信息入栈*/
	size.X = pRc->Right - pRc->Left + 1;    /*弹出窗口的宽度*/
	size.Y = pRc->Bottom - pRc->Top + 1;    /*弹出窗口的高度*/

	/*申请存放弹出窗口相关信息的动态存储区*/
	nextLayer = (LAYER_NODE*)malloc(sizeof(LAYER_NODE));
	nextLayer->next = gp_top_layer;
	nextLayer->LayerNo = gp_top_layer->LayerNo + 1;
	nextLayer->rcArea = *pRc;
	nextLayer->pContent = (CHAR_INFO*)malloc(size.X * size.Y * sizeof(CHAR_INFO));
	nextLayer->pScrAtt = (char*)malloc(size.X * size.Y * sizeof(char));
	pCh = nextLayer->pScrAtt;

	/*将弹出窗口覆盖区域的字符信息保存，用于在关闭弹出窗口时恢复原样*/
	ReadConsoleOutput(gh_std_out, nextLayer->pContent, size, pos, pRc);
	for (i = pRc->Top; i <= pRc->Bottom; i++)
	{   /*此二重循环将所覆盖字符单元的原先属性值存入动态存储区，便于以后恢复*/
		for (j = pRc->Left; j <= pRc->Right; j++)
		{
			*pCh = gp_scr_att[i * SCR_COL + j];  /*将pCh指向的内容放入gp_scr_att指向的字符单元属性值的缓冲区*/
			pCh++;
		}
	}
	gp_top_layer = nextLayer;  /*完成弹出窗口相关信息入栈操作,仍将gp_top_layer设置为弹出窗口信息链链头*/

	/*设置弹出窗口区域字符的新属性*/
	pos.X = pRc->Left;
	pos.Y = pRc->Top;
	for (i = pRc->Top; i <= pRc->Bottom; i++)
	{
		FillConsoleOutputAttribute(gh_std_out, att, size.X, pos, &ul);
		pos.Y++;
	}

	/*将标签束中的标签字符串在设定的位置输出*/
	for (i = 0; i < pLabel->num; i++)
	{
		pCh = pLabel->ppLabel[i];
		if (strlen(pCh) != 0)
			WriteConsoleOutputCharacter(gh_std_out, pCh, strlen(pCh), pLabel->pLoc[i], &ul);
	}

	/*设置弹出窗口区域字符单元的新属性*/
	for (i = pRc->Top; i <= pRc->Bottom; i++)
	{   /*此二重循环设置字符单元的层号*/
		for (j = pRc->Left; j <= pRc->Right; j++)
			gp_scr_att[i * SCR_COL + j] = gp_top_layer->LayerNo;
	}

	/*此二重循环设置所有热区中字符单元的热区类型和热区编号*/
	for (i = 0; i < pHotArea->num; i++)
	{
		row = pHotArea->pArea[i].Top;
		for (j = pHotArea->pArea[i].Left; j <= pHotArea->pArea[i].Right; j++)
			gp_scr_att[row * SCR_COL + j] |= (pHotArea->pSort[i] << 6) | (pHotArea->pTag[i] << 2);
	}
	return;
}

/**
*函数名称：PopOff
*函数功能：关闭弹出窗口,恢复覆盖区域原外观和字符单元原属性
*输入参数：无
*输出参数：无
*返回值：无
*调用说明：
*/
void PopOff(void)
{
	LAYER_NODE* nextLayer;
	COORD size;
	COORD pos = { 0, 0 };
	char* pCh;
	int i, j;
	/*栈底存放的主界面屏幕信息，不用关闭*/
	if ((gp_top_layer->next == NULL) || (gp_top_layer->pContent == NULL))
		return;
	nextLayer = gp_top_layer->next;

	/*恢复弹出窗口区域原外观*/
	size.X = gp_top_layer->rcArea.Right - gp_top_layer->rcArea.Left + 1;
	size.Y = gp_top_layer->rcArea.Bottom - gp_top_layer->rcArea.Top + 1;
	WriteConsoleOutput(gh_std_out, gp_top_layer->pContent, size, pos, &(gp_top_layer->rcArea));

	/*恢复字符单元原属性*/
	pCh = gp_top_layer->pScrAtt;
	for (i = gp_top_layer->rcArea.Top; i <= gp_top_layer->rcArea.Bottom; i++)
	{
		for (j = gp_top_layer->rcArea.Left; j <= gp_top_layer->rcArea.Right; j++)
		{
			gp_scr_att[i * SCR_COL + j] = *pCh;
			pCh++;
		}
	}
	/*释放动态存储区*/
	free(gp_top_layer->pContent);
	free(gp_top_layer->pScrAtt);
	free(gp_top_layer);
	gp_top_layer = nextLayer;
	gi_sel_sub_menu = 0;
	return;
}

/**
*函数名称：DrawBox
*函数功能：给弹出的子菜单窗口绘制边框.
*输入参数：无
*输出参数：无
*返回值：无
*调用说明：
*/
void DrawBox(SMALL_RECT* pRc)
{
	char chBox[] = { '#','-','|' };  /*画框用的字符*/
	COORD pos = { pRc->Left, pRc->Top };  /*定位在区域的左上角*/
	WriteConsoleOutputCharacter(gh_std_out, &chBox[0], 1, pos, &ul);  /*画边框左上角#*/
	/*此循环画上边框横线*/
	for (pos.X = pRc->Left + 1; pos.X < pRc->Right; pos.X++)
		WriteConsoleOutputCharacter(gh_std_out, &chBox[1], 1, pos, &ul);
	pos.X = pRc->Right;

	WriteConsoleOutputCharacter(gh_std_out, &chBox[0], 1, pos, &ul);   /*画边框右上角~*/
	/*此循环画边框左边线和右边线*/
	for (pos.Y = pRc->Top + 1; pos.Y < pRc->Bottom; pos.Y++)
	{
		pos.X = pRc->Left;
		WriteConsoleOutputCharacter(gh_std_out, &chBox[2], 1, pos, &ul);
		pos.X = pRc->Right;
		WriteConsoleOutputCharacter(gh_std_out, &chBox[2], 1, pos, &ul);
	}
	pos.X = pRc->Left;
	pos.Y = pRc->Bottom;

	WriteConsoleOutputCharacter(gh_std_out, &chBox[0], 1, pos, &ul);   /*画边框左下角&*/
	/*画下边框横线*/
	for (pos.X = pRc->Left + 1; pos.X < pRc->Right; pos.X++)
		WriteConsoleOutputCharacter(gh_std_out, &chBox[1], 1, pos, &ul);
	pos.X = pRc->Right;
	WriteConsoleOutputCharacter(gh_std_out, &chBox[0], 1, pos, &ul);   /*画边框右下角%*/
	return;
}

/**
*函数名称：LocSubMenu
*函数功能：计算弹出子菜单(标号为num)区域左上角和右下角的位置.信息存储在类型为SMALL_RECT结构的变量rc中
*输入参数：num为选中的子菜单项号
*输出参数：rc存放区域位置信息的地址,是个矩形结构
*返回值：无
*调用说明：
*/
void LocSubMenu(int num, SMALL_RECT* rc)
{
	int i, len, loc = 0;
	/*区域的上边定在第2行，行号为1*/
	rc->Top = 1;
	rc->Left = 1;
	/*计算区域左边界位置, 同时计算第一个子菜单项在子菜单字符串数组中的位置*/
	for (i = 1; i < num; i++)
	{
		rc->Left += strlen(ga_main_menu[i - 1]) + 4;
		loc += ga_sub_menu_count[i - 1];
	}
	rc->Right = strlen(ga_sub_menu[loc]);    /*暂时存放第一个子菜单项字符串长度*/
	/*查找最长子菜单字符串，将其长度存放在rc->Right*/
	for (i = 1; i < ga_sub_menu_count[num - 1]; i++)
	{
		len = strlen(ga_sub_menu[loc + i]);
		if (rc->Right < len)
		{
			rc->Right = len;
		}
	}
	rc->Right += rc->Left + 3;  /*计算区域的右边界*/
	rc->Bottom = rc->Top + ga_sub_menu_count[num - 1] + 1;/*计算区域下边的行号*/
	/*右边界越界的处理*/
	if (rc->Right >= SCR_COL)
	{
		len = rc->Right - SCR_COL + 1;
		rc->Left -= len;
		rc->Right = SCR_COL - 1;
	}
	return;
}

/**
*函数名称：ShowState
*函数功能：显示状态条
*输入参数：无
*输出参数：无
*返回值：无
*调用说明：状态条字符属性为白底黑字，初始状态无状态信息
*/
void ShowState()
{
	CONSOLE_SCREEN_BUFFER_INFO bInfo;
	COORD size;
	COORD pos = { 0, 0 };
	int i;

	GetConsoleScreenBufferInfo(gh_std_out, &bInfo);
	size.X = bInfo.dwSize.X;
	size.Y = 1;
	SMALL_RECT rcMenu = { 0, bInfo.dwSize.Y - 1, size.X - 1, bInfo.dwSize.Y - 1 };

	if (gp_buff_stateBar_info == NULL)
	{
		gp_buff_stateBar_info = (CHAR_INFO*)malloc(size.X * size.Y * sizeof(CHAR_INFO));
		ReadConsoleOutput(gh_std_out, gp_buff_stateBar_info, size, pos, &rcMenu);
	}

	for (i = 0; i < size.X; i++)
	{
		(gp_buff_stateBar_info + i)->Attributes = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;
	}

	WriteConsoleOutput(gh_std_out, gp_buff_stateBar_info, size, pos, &rcMenu);

	return;
}

/**
*函数名称：TagMainMenu
*函数功能：在指定主菜单项上选中标记
*输入参数：num选中的主菜单编号
*输出参数：无
*返回值：无
*调用说明：
*/
void TagMainMenu(int num)
{
	CONSOLE_SCREEN_BUFFER_INFO bInfo;
	COORD size;
	COORD pos = { 0,0 };
	int PosA = 2, PosB;
	char ch;
	int i;
	if (num == 0)   //num为0 时，将会去除主菜单选项中标记
	{
		PosA = 0;
		PosB = 0;
	}
	else  //否则，定位选中主菜单项的起止位置，PosA是起始位置，PosB是截止位置
	{
		for (i = 1; i < num; i++)
		{
			PosA += strlen(ga_main_menu[i - 1]) + 4;   //这里的4是什么意思？
		}
		PosB = PosA += strlen(ga_main_menu[num - 1]);
	}
	GetConsoleScreenBufferInfo(gh_std_out, &bInfo);  //获取屏幕缓冲区信息，存入binfo中
	size.X = bInfo.dwSize.X;
	size.Y = 1;

	//去除选中的菜单项前面的菜单项选中标记
	for (i = 0; i < PosA; i++)
	{
		(gp_buff_menubar_info + i)->Attributes = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;
		ch = (char)((gp_buff_menubar_info + i)->Char.AsciiChar);
		if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
		{
			(gp_buff_menubar_info + i)->Attributes |= FOREGROUND_RED;
		}
	}

	//在选中的菜单项上做标记，白底黑字
	for (i = PosA; i < PosB; i++)
	{
		(gp_buff_menubar_info + i)->Attributes = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
	}

	//去除选中菜单项后面的菜单项选中标记
	for (i = PosB; i < bInfo.dwSize.X; i++)
	{
		(gp_buff_menubar_info + i)->Attributes = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;
		ch = (char)((gp_buff_menubar_info + i)->Char.AsciiChar);
		if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
		{
			(gp_buff_menubar_info + i)->Attributes |= FOREGROUND_RED;
		}
	}
	//将做好标记的菜单条信息写的窗口第一行
	SMALL_RECT rcMenu = { 0,0,size.X - 1,0 };
	WriteConsoleOutput(gh_std_out, gp_buff_menubar_info, size, pos, &rcMenu);
	return;
}

/**
*函数名称：TagSubMenu
*函数功能：在指定子菜单项上做选中标记
*输入参数：num为选中的子菜单项号
*输出参数：无
*返回值：无
*调用说明：
*/
void TagSubMenu(int num)
{
	SMALL_RECT rcPop;
	COORD pos;
	WORD  att;
	int   width;
	LocSubMenu(gi_sel_menu, &rcPop);/*计算弹出子菜单的区域位置, 存放在rcPop中*/
	/*如果子菜单项号越界，或该项子菜单已被选中，则返回*/
	if ((num < 1) || (num == gi_sel_sub_menu) || (num > rcPop.Bottom - rcPop.Top - 1))
		return;
	pos.X = rcPop.Left + 2;
	width = rcPop.Right - rcPop.Left - 3;
	/*首先取消原选中子菜单项上的标记*/
	if (gi_sel_sub_menu != 0)
	{
		pos.Y = rcPop.Top + gi_sel_sub_menu;
		att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
		FillConsoleOutputAttribute(gh_std_out, att, width, pos, &ul);
		pos.X += 1;
		att |= FOREGROUND_RED | FOREGROUND_INTENSITY;    /*白底亮红字*/
		FillConsoleOutputAttribute(gh_std_out, att, 1, pos, &ul);
	}

	/*在子菜单项上做选中标记(亮红底亮白字)*/
	pos.X = rcPop.Left + 2;
	pos.Y = rcPop.Top + num;
	att = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_INTENSITY;/*亮红底亮白字*/
	FillConsoleOutputAttribute(gh_std_out, att, width, pos, &ul);
	gi_sel_sub_menu = num;  /*修改选中子菜单项号*/
	return;
}

/**
 * 函数名称: DealInput
 * 函数功能: 在弹出窗口区域设置热区, 等待并相应用户输入.
 * 输入参数: pHotArea
 *           piHot 焦点热区编号的存放地址, 即指向焦点热区编号的指针
 * 输出参数: piHot 用鼠标单击、按回车或空格时返回当前热区编号
 * 返 回 值:
 *
 * 调用说明:
 */
int DealInput(HOT_AREA* pHotArea, int* piHot)
{
	INPUT_RECORD inRec;
	CONSOLE_CURSOR_INFO lpCur;
	DWORD res;
	COORD pos = { 0, 0 };
	int num, arrow, iRet = 0;
	int cNo, cTag, cSort;/*cNo:层号, cTag:热区编号, cSort: 热区类型*/
	char vkc, asc;       /*vkc:虚拟键代码, asc:字符的ASCII码值*/

	SetHotPoint(pHotArea, *piHot);
	while (TRUE)
	{    /*循环*/
		ReadConsoleInput(gh_std_in, &inRec, 1, &res);
		if ((inRec.EventType == MOUSE_EVENT) &&
			(inRec.Event.MouseEvent.dwButtonState
				== FROM_LEFT_1ST_BUTTON_PRESSED))
		{
			pos = inRec.Event.MouseEvent.dwMousePosition;   //获取光标位置
			cNo = gp_scr_att[pos.Y * SCR_COL + pos.X] & 3;
			cTag = (gp_scr_att[pos.Y * SCR_COL + pos.X] >> 2) & 15;
			cSort = (gp_scr_att[pos.Y * SCR_COL + pos.X] >> 6) & 3;    //获取该处页面属性

			if ((cNo == gp_top_layer->LayerNo) && cTag > 0)   //如果该点在热区上
			{
				*piHot = cTag;
				SetHotPoint(pHotArea, *piHot);
				if (cSort == 0)
				{
					iRet = 13;
					break;
				}
			}
			else if (cTag == 0)  //不在热区上
			{
				GetConsoleCursorInfo(gh_std_out, &lpCur);
				lpCur.bVisible = FALSE;
				SetConsoleCursorInfo(gh_std_out, &lpCur);   //隐藏光标
			}
		}

		//如果是键盘输入
		else if (inRec.EventType == KEY_EVENT && inRec.Event.KeyEvent.bKeyDown)   
		{
			vkc = inRec.Event.KeyEvent.wVirtualKeyCode;
			asc = inRec.Event.KeyEvent.uChar.AsciiChar;;
			if (asc == 0)
			{
				arrow = 0;
				switch (vkc)
				{  /*方向键(左、上、右、下)的处理*/
				case 37: arrow = 1; break;
				case 38: arrow = 2; break;
				case 39: arrow = 3; break;
				case 40: arrow = 4; break;
				}
				if (arrow > 0)
				{
					num = *piHot;
					while (TRUE)
					{
						if (arrow < 3)
						{
							num--;
						}
						else
						{
							num++;
						}
						if ((num < 1) || (num > pHotArea->num) ||
							((arrow % 2) && (pHotArea->pArea[num - 1].Top
								== pHotArea->pArea[*piHot - 1].Top)) || ((!(arrow % 2))
									&& (pHotArea->pArea[num - 1].Top
										!= pHotArea->pArea[*piHot - 1].Top)))
						{
							break;
						}
					}
					if (num > 0 && num <= pHotArea->num)
					{
						*piHot = num;
						SetHotPoint(pHotArea, *piHot);
					}
				}
			}
			else if (vkc == 27)
			{  /*ESC键*/
				iRet = 27;
				break;
			}
			else if (vkc == 13 || vkc == 32)
			{  /*回车键或空格表示按下当前按钮*/
				iRet = 13;
				break;
			}
		}
	}
	return iRet;
}
void SetHotPoint(HOT_AREA* pHotArea, int iHot)
{
	CONSOLE_CURSOR_INFO lpCur;
	COORD pos = { 0, 0 };
	WORD att1, att2;
	int i, width;

	att1 = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;  /*黑底白字*/
	att2 = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
	for (i = 0; i < pHotArea->num; i++)
	{  /*将按钮类热区置为白底黑字*/
		pos.X = pHotArea->pArea[i].Left;
		pos.Y = pHotArea->pArea[i].Top;
		width = pHotArea->pArea[i].Right - pHotArea->pArea[i].Left + 1;
		if (pHotArea->pSort[i] == 0)
		{  /*热区是按钮类*/
			FillConsoleOutputAttribute(gh_std_out, att2, width, pos, &ul);
		}
	}

	pos.X = pHotArea->pArea[iHot - 1].Left;
	pos.Y = pHotArea->pArea[iHot - 1].Top;
	width = pHotArea->pArea[iHot - 1].Right - pHotArea->pArea[iHot - 1].Left + 1;
	if (pHotArea->pSort[iHot - 1] == 0)
	{  /*被激活热区是按钮类*/
		FillConsoleOutputAttribute(gh_std_out, att1, width, pos, &ul);
	}
	else if (pHotArea->pSort[iHot - 1] == 1)
	{  /*被激活热区是文本框类*/
		SetConsoleCursorPosition(gh_std_out, pos);
		GetConsoleCursorInfo(gh_std_out, &lpCur);
		lpCur.bVisible = TRUE;
		SetConsoleCursorInfo(gh_std_out, &lpCur);
	}
}

/**
*函数名称：ExeFunction
*函数功能：执行由主菜单号为m和子菜单号为n所确定的功能函数
*输入参数：m主菜单号，s子菜单号
*输出参数：无
*返回值：BOOL类型，TRUE OR FALSE
*调用说明：仅在执行函数ExitSys时，才可能返回FALSE
*/
BOOL ExeFunction(int m, int n)
{
	BOOL bRet = TRUE;
	/*函数指针数组，用来存放所有功能函数的入口地址*/
	//BOOL(*pFunction[ga_sub_menu_count[0]+ga_sub_menu_count[1]+ ga_sub_menu_count[2]+ ga_sub_menu_count[3]+ ga_sub_menu_count[4]])(void);
	BOOL(*pFunction[26])(void);  //这里还是自己设计好有几个函数
	int i, loc;
	/*将功能函数入口地址存入与功能函数所在主菜单号和子菜单号对应下标的数组元素*/

	pFunction[0] = SaveData;  //文件部分，4个
	pFunction[1] = BackupData;
	pFunction[2] = RestoreData;
	pFunction[3] = CloseSys;
	                      
	pFunction[4] = MaintainHomeInfo;   //数据维护部分
	pFunction[5] = MaintainCarInfo;
	pFunction[6] = MaintainChargeInfo;

	pFunction[7] = QueryHomeInfo;    //数据查询部分，3个
	pFunction[8] = QueryCarInfo;
	pFunction[9] = QueryChargeInfo;  

	pFunction[10] = Stat_Car_Type_Rent_Num;        //统计部分，五个函数
	pFunction[11] = Stat_Car_Type_Month_Fee;
	pFunction[12] = Stat_Car_Fee_and_rent_rate;
	pFunction[13] = Stat_Car_Most_Rent;
	pFunction[14] = Stat_Guest_Most_Rent;

	pFunction[15] = HelpTopic;   //帮助部分，2个
	pFunction[16] = NULL;
	pFunction[17] = AboutSys;

	/*根据主菜单号和子菜单号计算对应下标*/
	for (i = 1, loc = 0; i < m; i++)
		loc += ga_sub_menu_count[i - 1];    //{5，11，4，5，3}
	loc += n - 1;
	/*用函数指针调用所指向的功能函数*/
	if (pFunction[loc] != NULL)
		bRet = (*pFunction[loc])();
	return bRet;
}

/**
 * 函数名称: ChangeCarNumber
 * 函数功能: 自动统计当前车辆数目，根据车辆基本信息统计，然后更新
 * 输入参数: 无
 *      
 * 输出参数: phaed,链表头指针地址
 * 返 回 值: BOOL类型，恒为TRUE
 *
 * 调用说明:
 */
BOOL ChangeCarNumber(HOME_NODE** phead)   //自动统计当前车辆数目，根据车辆基本信息统计，然后更新
{
	HOME_NODE* p1 = (HOME_NODE*)malloc(sizeof(HOME_NODE));
	CAR_NODE* p2 = (CAR_NODE*)malloc(sizeof(CAR_NODE));
	p1 = (*phead)->next;
	while (p1 != NULL)
	{
		int number = 0;
		p2 = p1->snext;
		while (p2 != NULL)
		{
			number++;
			p2 = p2->next;
		}
		p1->num = number;
		p1 = p1->next;
	}
	return TRUE;
}

/*
   函数名称：SaveSysData
   函数功能：将当前内存中数据存入外存文件
   输入参数：hd主链头指针和存入的文件名
   输出参数:
   返回值：BOOL，总为TRUE
   调用说明：
*/
BOOL SaveSysData(HOME_NODE* phead, char* filename[3])
{
	FILE* fout1, * fout2, * fout3;
	HOME_NODE* p1 = (HOME_NODE*)malloc(sizeof(HOME_NODE));
	CAR_NODE* p2=(CAR_NODE*)malloc(sizeof(CAR_NODE));
	CHARGE_NODE* p3 = (CHARGE_NODE*)malloc(sizeof(CHARGE_NODE));
	p1 = phead->next;

	if ((fout1 = fopen(filename[0], "wb")) == NULL)
		return FALSE;
	if ((fout2 = fopen(filename[1], "wb")) == NULL)
		return FALSE;
	if ((fout3 = fopen(filename[2], "wb")) == NULL)
		return FALSE;

	ChangeCarNumber(&phead);
	ChangeID(&phead);    //根据订单信息改变出租状态
	
	fprintf(fout1, "%s,%s,%s,\n", "车辆类型编码", "车辆类型名称", "库存数量");
	fprintf(fout2, "%s,%s,%s,%s,%s,%s,%s,\n", "车辆编号", "车牌号", "车辆类型编码", "车辆名称", "排挡方式", "每日租金（元）", "出租状态");
	fprintf(fout3, "%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,\n", "订单编号", "身份证号", "客人姓名", "手机号码", "租用车辆编号", "取车时间", "预约还车时间", "押金（元）", "实际还车时间", "应缴费用（元）", "实缴费用（元）");

	while (p1 != NULL)
	{
		fprintf(fout1, "%c,%s,%d,\n", p1->car_id, p1->type, p1->num);
		p2 = p1->snext;
		while (p2 != NULL)
		{
			fprintf(fout2, "%d,%s,%c,%s,%s,%f,%c,\n", p2->number, p2->car_num, p2->car_id, p2->car_name, p2->car_gear, p2->fee, p2->id);
			p3 = p2->cnext;
			while (p3 != NULL)
			{
				fprintf(fout3, "%s,%s,%s,%s,%d,%s,%s,%f,%s,%f,%f,\n", p3->rent_id, p3->guest_id, p3->name, p3->tel, p3->number, p3->time_of_get_car, p3->time_of_scheduled_back, p3->deposit, p3->time_of_actual_back, p3->should_pay, p3->actual_pay);
				p3 = p3->next;
			}
			p2 = p2->next;
		}
		p1 = p1->next;
	}
	fclose(fout1);
	fclose(fout2);
	fclose(fout3);
	printf("相关数据已保存!\n");
	return TRUE;
}

/*
   函数名称：SaveData
   函数功能：将当前内存中数据存入外存文件
   输入参数：无
   输出参数: 无
   返回值：BOOL，总为TRUE
   调用说明：本函数是为了方便调用
*/
BOOL SaveData(void)
{
	if(SaveSysData(gp_head, FileName)==FALSE)
		printf("出错！\n");
	rewind(stdin);
	getchar();
	ClearScreen();
	ShowMenu();
	return TRUE;
}

/*
   函数名称：BackupData
   函数功能：备份数据
   输入参数：无
   输出参数: 无
   返回值：BOOL，总为TRUE
   调用说明：
*/
BOOL BackupData(void)
{
	ScreenFresh();
	BOOL bRet = TRUE;
	if(SaveSysData(gp_head, BeiFen_FileName))
		printf("数据备份成功！  按任意键继续\n");
	else
		printf("数据备份失败！  按任意键继续\n");
	rewind(stdin);
	getchar();
	ClearScreen();
	ShowMenu();
	return bRet;
}

/*
   函数名称：RestoreData
   函数功能：恢复备份数据
   输入参数：无
   输出参数: 无
   返回值：BOOL，总为TRUE
   调用说明：
*/
BOOL RestoreData(void)
{
	BOOL bRet = TRUE;
	char ch;
	ScreenFresh();
	rewind(stdin);
	printf("注意:如果恢复备份数据当前的数据将被覆盖,你确定继续恢复吗?确定(Y) 取消(N):\n");
	ch = getchar(); 
	getchar();
	
	if (ch == 'N' || ch == 'n')
		puts("取消成功！\n");
	else if (ch == 'Y' || ch == 'y')
	{
		CreateList(&gp_head, BeiFen_FileName);
		if (SaveSysData(gp_head, FileName))
			printf("备份数据恢复成功!\n");
		else
			printf("备份数据恢复失败!\n");
	}
	else
		printf("输入错误!本次操作自动退出\n");

	rewind(stdin);
	printf("\n按任意键继续\n");
	getchar();
	ClearScreen();
	ShowMenu();
	return bRet;
}

/* 退出系统
BOOL ExitSys(void)
{
	LABEL_BUNDLE labels;
	HOT_AREA areas;
	BOOL bRet = TRUE;
	SMALL_RECT rcPop, aArea[2];
	COORD pos, aLoc[2];
	WORD att;

	char* pCh[] = { " 确认退出系统？", "确定(Y)  取消(N)" };
	int iHot = 1;
	char  aSort[] = { 0, 0 }, aTag[] = { 1, 2 };

	ScreenFresh();//先清除屏幕
	pos.X = strlen(pCh[0]) + 7;
	pos.Y = 8;
	rcPop.Left = (SCR_COL - pos.X) / 2;
	rcPop.Right = rcPop.Left + pos.X - 1;
	rcPop.Top = (SCR_ROW - pos.Y) / 2;
	rcPop.Bottom = rcPop.Top + pos.Y - 1;
	att = FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;白底亮红字
	labels.num = 2;
	labels.ppLabel = pCh;
	设置按钮位置坐标
	aLoc[0].X = rcPop.Left + 3;
	aLoc[0].Y = rcPop.Top + 2;
	aLoc[1].X = rcPop.Left + 5;
	aLoc[1].Y = rcPop.Top + 5;
	labels.pLoc = aLoc;
	areas.num = 2;
	设置热区范围
	aArea[0].Left = rcPop.Left + 5;
	aArea[0].Top = rcPop.Top + 5;
	aArea[0].Right = rcPop.Left + 8;
	aArea[0].Bottom = rcPop.Top + 5;
	aArea[1].Left = rcPop.Left + 14;
	aArea[1].Top = rcPop.Top + 5;
	aArea[1].Right = rcPop.Left + 17;
	aArea[1].Bottom = rcPop.Top + 5;
	areas.pArea = aArea;
	areas.pSort = aSort;
	areas.pTag = aTag;
	PopUp(&rcPop, att, &labels, &areas);在指定区域输出弹出窗口信息并将信息入栈,设置热键
	/*画一条分隔线
	pos.X = rcPop.Left + 1;
	pos.Y = rcPop.Top + 4;
	FillConsoleOutputCharacter(gh_std_out, '*', rcPop.Right - rcPop.Left - 1, pos, &ul);
	if (DealQuitInput(&areas, &iHot) == 1 && iHot == 1)
		bRet = FALSE;
	else  bRet = TRUE;
	PopOff();关闭弹出窗口
	return bRet;
}
*/

/*
   函数名称：ShowModule
   函数功能：将标签束设置为热区
   输入参数：pString存放标签束,以及标签个数n
   输出参数: 无
   返回值：鼠标点击，回车或者空格返回当前热区编号
   调用说明：
*/
BOOL ShowModule(char** pString, int n)
{
	LABEL_BUNDLE labels;
	HOT_AREA areas;
	BOOL bRet = TRUE;
	SMALL_RECT rcPop;
	COORD pos;
	WORD att;
	int iHot = 1;
	int i, maxlen, str_len;

	for (i = 0, maxlen = 0; i < n; i++) {
		str_len = strlen(pString[i]);
		if (maxlen < str_len) {
			maxlen = str_len;
		}
	}

	pos.X = maxlen + 6;
	pos.Y = n + 5;
	rcPop.Left = (SCR_COL - pos.X) / 2;
	rcPop.Right = rcPop.Left + pos.X - 1;
	rcPop.Top = (SCR_ROW - pos.Y) / 2;
	rcPop.Bottom = rcPop.Top + pos.Y - 1;

	att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
	labels.num = n;
	labels.ppLabel = pString;
	COORD* aLoc = (COORD*)malloc(sizeof(COORD) * n);

	for (i = 0; i < n; i++) {
		aLoc[i].X = rcPop.Left + 3;
		aLoc[i].Y = rcPop.Top + 2 + i;

	}
	str_len = strlen(pString[n - 1]);
	aLoc[n - 1].X = rcPop.Left + 3 + (maxlen - str_len) / 2;
	aLoc[n - 1].Y = aLoc[n - 1].Y + 2;

	labels.pLoc = aLoc;

	areas.num = 1;
	SMALL_RECT aArea[] = { {aLoc[n - 1].X, aLoc[n - 1].Y,
						   aLoc[n - 1].X + 3, aLoc[n - 1].Y} };

	char aSort[] = { 0 };
	char aTag[] = { 1 };

	areas.pArea = aArea;
	areas.pSort = aSort;
	areas.pTag = aTag;
	PopUp(&rcPop, att, &labels, &areas);

	pos.X = rcPop.Left + 1;
	pos.Y = rcPop.Top + 2 + n;
	FillConsoleOutputCharacter(gh_std_out, '-', rcPop.Right - rcPop.Left - 1, pos, &ul);

	DealInput(&areas, &iHot);
	PopOff();

	return bRet;

}


//--------------------------------------------------------------
//数据维护板块

//车辆分类信息维护
/*
   函数名称：MaintainHomeInfo
   函数功能：数据维护，车辆分类信息
   输入参数：无
   输出参数:
   返回值：BOOL，总为TRUE
   调用说明：
*/
BOOL MaintainHomeInfo(void)
{
	char x;
	int choice;
	while (TRUE)
	{
		printf("现在开始维护车辆分类信息：\n请选择：\n");
		printf("1.插入    2.修改     3.删除\n");
		scanf("%d", &choice);
		getchar();
		if (choice < 1 || choice>3)
		{
			char x;
			printf("输入错误，是否重新输入？y-是 n-否\n");
			scanf("%c", &x);
			getchar();
			if (x == 'y' || x == 'Y')
			{
				ClearScreen();
				ShowMenu();
				continue;
			}
		}
		else
		{
			switch (choice)
			{
			case 1: InsertHome(); break;
			case 2: ModifyHome(); break;
			case 3: DelHome(); break;
			}
		}

		rewind(stdin);
		printf("\n请问是否继续维护？\n");
		scanf("%c", &x);
		getchar();
		if (x == 'y' || x == 'Y')
		{
			ClearScreen();
			ShowMenu();
			continue;
		}
		else
		{
			printf("\n本次维护结束---------按任意键退出\n");
			break;
		}
	}

	rewind(stdin);
	getchar();
	ClearScreen();
	ShowMenu();
	return TRUE;
}

/*
   函数名称：InsertHome
   函数功能：插入车辆分类信息
   输入参数：无
   输出参数:
   返回值：BOOL，总为TRUE
   调用说明：
*/
BOOL InsertHome(void)
{
	ScreenFresh();
	HOME_NODE* ptemp= (HOME_NODE*)malloc(sizeof(HOME_NODE));
	ptemp->snext = NULL;
	ptemp->next = NULL;
	printf("下面输入增加的车辆分类基本信息：\n\n");
	printf("请输入车辆类型编码:"); 
	if (!scanf_s("%c", &ptemp->car_id, sizeof(char)))
	{
		printf("输入错误，自动退出！\n");
		free(ptemp);
		return TRUE;
	}
	getchar(); 
	printf("\n");
	printf("请输入车辆类型名称:");
	if (!scanf_s("%s", ptemp->type, sizeof(char)*20))
	{
		printf("输入错误，自动退出！\n");
		free(ptemp);
		return TRUE;
	}
	getchar(); 
	printf("\n");
	
	printf("车辆数量已自动设置为0！如需添加，请在车辆基本信息中添加！\n\n");
	
	printf("按任意键继续\n");
	getchar();
	rewind(stdin);  //清空输入缓冲区
	ScreenFresh();
	if (InsertHomeNode(&gp_head, ptemp) == TRUE)   //输入安全
	{
		printf("添加车辆分类信息成功！\n");
		//SortHomeNode(&gp_head);
		ChangeCarNumber(&gp_head);
		if (SaveSysData(gp_head, FileName))
			printf("数据自动保存！\n");
		else
			printf("保存失败！\n");
	}
	else
		printf("此车辆信息已存在，插入失败！\n");
	return TRUE;
}

/*
   函数名称：InsertHomeNode
   函数功能：插入车辆分类信息结点
   输入参数：主链表头指针地址，以及存放修改信息的节点ptemp
   输出参数: 
   返回值：BOOL,TRUE说明插入成功，FALSE说明插入失败
   调用说明：
*/
BOOL InsertHomeNode(HOME_NODE** phead, HOME_NODE* ptemp)
{
	HOME_NODE* p = (*phead)->next;
	while (p != NULL && p->car_id != ptemp->car_id)
	{
		p = p->next;
	}
	if (p == NULL)    //头插法，后续写一个排序代码按编号顺序排
	{
		ptemp->next = (*phead)->next;
		(*phead)->next = ptemp;
		return TRUE;
	}
	else   //车辆分类已经被使用
		return FALSE;
}

/*
   函数名称：ModifyHome
   函数功能：修改车辆分类信息
   输入参数：无
   输出参数:
   返回值：BOOL,TRUE说明修改成功，FALSE说明修改失败
   调用说明：
*/
BOOL ModifyHome(void)
{
	ScreenFresh();
	HOME_NODE* ptemp = (HOME_NODE*)malloc(sizeof(HOME_NODE));
	ptemp->snext = NULL;
	ptemp->next = NULL;
	char type[20];
	printf("请输入所要修改的车辆类型名称(如经济型）:");
	if (!scanf_s("%s", type, sizeof(char) * 20))
	{
		printf("输入错误，自动退出！\n");
		return TRUE;
	} 
	getchar();

	if (!SeekHomeNodeByCarName(gp_head, type))   //发现了一样的类型，不符
	{
		printf("无此类型，自动退出！\n");
		free(ptemp);
		return TRUE;
	}

	printf("\n下面输入修改的车辆分类基本信息：\n\n");
	printf("请输入车辆类型编码:");
	if (!scanf_s("%c", &ptemp->car_id, sizeof(char)))
	{
		printf("输入错误，自动退出！\n");
		free(ptemp);
		return TRUE;
	}
	getchar();
	if (SeekHomeNodeByCarID(gp_head, ptemp->car_id))   //发现了一样的编码，不符
	{
		printf("编码重复，自动退出！\n");
		free(ptemp);
		return TRUE;
	}
	printf("\n");
	printf("请输入车辆类型名称:");
	if (!scanf_s("%s", &ptemp->type, sizeof(char) * 20))
	{
		printf("输入错误，自动退出！\n");
		free(ptemp);
		return TRUE;
	}
	getchar();
	if (SeekHomeNodeByCarName(gp_head, ptemp->type))   //发现了一样的类型，不符
	{
		printf("类型重复，自动退出！\n");
		free(ptemp);
		return TRUE;
	}
	printf("\n");

	printf("按任意键继续\n");
	getchar();

	rewind(stdin);  //清空输入缓冲区
	ScreenFresh();
	if (ModifyHomeNode(&gp_head, type, ptemp) == TRUE)   //输入安全
	{
		printf("修改成功！\n");
		//SortHomeNode(&gp_head);
		if (SaveSysData(gp_head, FileName))
			printf("数据自动保存！\n");
		else
			printf("保存失败！\n");
	}
	else
		printf("该类型不存在，修改失败!\n");
	return TRUE;
}

/*
   函数名称：ModifyHomeNode
   函数功能：修改车辆分类节点
   输入参数：主链头指针地址phead，车辆类型名称type，存放修改信息的节点ptemp
   输出参数:
   返回值：BOOL,TRUE说明修改成功，FALSE说明修改失败
   调用说明：
*/
BOOL  ModifyHomeNode(HOME_NODE** phead, char type[20], HOME_NODE* phome_node)
{
	HOME_NODE* phome_temp;
	//HOME_NODE* phome_next;
	BOOL  bRet = FALSE;
	phome_temp = SeekHomeNodeByCarName(*phead, type);
	if (phome_temp != NULL)   //找到了
	{
		phome_temp->car_id = phome_node->car_id;
		strcpy(phome_temp->type, phome_node->type);
		bRet = TRUE;
	}
	return bRet;
}

/*
   函数名称：DelHome
   函数功能：删除车辆分类信息
   输入参数：无
   输出参数:
   返回值：BOOL,TRUE说明删除成功，FALSE说明删除失败
   调用说明：
*/
BOOL DelHome(void)
{
	char type[20];
	printf("请输入所要删除的车辆类型名称(如经济型）:\n");
	if (!scanf_s("%s", type, sizeof(char) * 20))
	{
		printf("输入错误，自动退出！\n");
		return TRUE;
	}
	getchar();
	printf("\n");
	if (!SeekHomeNodeByCarName(gp_head, type))   
	{
		printf("无此车辆类型，自动退出！\n");
		return TRUE;
	}
	rewind(stdin);  //清空输入缓冲区

	if (DelHomeNode(&gp_head, type) == TRUE)
		printf("删除成功!\n");
	else
		printf("删除失败！\n");
	SaveSysData(gp_head, FileName);/*保存信息*/
	return TRUE;
}

/*
   函数名称：DelHomeNode
   函数功能：删除车辆分类信息结点
   输入参数：主链头指针地址phead，车辆类型名称type
   输出参数:
   返回值：BOOL,TRUE说明删除成功，FALSE说明删除失败
   调用说明：
*/
BOOL  DelHomeNode(HOME_NODE** phead, char type[20])
{
	HOME_NODE* phome_prior= NULL;
	HOME_NODE* phome_current= (*phead)->next;
	CAR_NODE* pcar_prior=(CAR_NODE*)malloc(sizeof(CAR_NODE));
	CAR_NODE* pcar_current= (CAR_NODE*)malloc(sizeof(CAR_NODE));
	CHARGE_NODE* pcharge_prior= (CHARGE_NODE*)malloc(sizeof(CHARGE_NODE));
	CHARGE_NODE* pcharge_current= (CHARGE_NODE*)malloc(sizeof(CHARGE_NODE));
	int bRet = FALSE;
	for (; phome_current != NULL; phome_prior = phome_current, phome_current = phome_current->next)
	{
		if (strcmp(phome_current->type,type)==0)
		{
			bRet = TRUE;
			break;    //找到了
		}
	}
	
	if (phome_current != NULL)        //找到了对应节点
	{
		if (phome_prior == NULL)            //如果释放的是车辆分类信息链的链头结点
		{
			(*phead) = phome_current->next;
		}
		else
		{
			phome_prior->next = phome_current->next;
		}

		pcar_current = phome_current->snext;
		free(phome_current);         //释放要删除的车辆分类信息结点

		while (pcar_current != NULL)    //释放对应的车辆分类和订单信息
		{
			pcharge_current = pcar_current->cnext;
			pcar_prior = pcar_current;
			while (pcharge_current != NULL)
			{
				pcharge_prior = pcharge_current;
				pcharge_current = pcharge_current->next;
				free(pcharge_prior);     //删除汽车租赁订单结点
			}
			pcar_current = pcar_current->next;
			free(pcar_prior);     //删除该车型对应的车辆基本信息结点
		}
	}
	return bRet;
}

//----------------------------------------------------------------------------------------
//车辆基本信息维护
/*
   函数名称：MaintainCarInfo
   函数功能：数据维护，车辆基本信息
   输入参数：无
   输出参数:
   返回值：BOOL，总为TRUE
   调用说明：
*/
BOOL MaintainCarInfo(void)
{
	char x;
	int choice;
	while (TRUE)
	{
		printf("现在开始维护车辆基本信息：\n请选择：\n");
		printf("1.插入    2.修改     3.删除\n");
		scanf("%d", &choice);
		getchar();

		if (choice < 1 || choice>3)
		{
			char x;
			printf("输入错误，是否重新输入？y-是 n-否\n");
			scanf("%c", &x);
			getchar();
			if (x == 'y' || x == 'Y')
			{
				ClearScreen();
				ShowMenu();
				continue;
			}
		}

		else
		{
			switch (choice)
			{
			case 1: InsertCar(); break;
			case 2: ModifyCar(); break;
			case 3: DelCar(); break;
			}
		}

		rewind(stdin);
		printf("\n请问是否继续维护？\n");
		scanf("%c", &x);
		getchar();
		if (x == 'y' || x == 'Y')
		{
			ClearScreen();
			ShowMenu();
			continue;
		}
		else
		{
			printf("\n本次维护结束---------按任意键退出\n");
			break;
		}
	}

	rewind(stdin);
	getchar();
	ClearScreen();
	ShowMenu();
	return TRUE;
}

/*
   函数名称：InsertCar
   函数功能：插入车辆基本信息
   输入参数：无
   输出参数:
   返回值：BOOL,TRUE说明添加成功，FALSE说明添加失败
   调用说明：
*/
BOOL InsertCar(void)
{
	ScreenFresh();
	rewind(stdin);
	BOOL bRet = TRUE;
	int iRet;
	CAR_NODE* ptemp = (CAR_NODE*)malloc(sizeof(CAR_NODE));
	ptemp->cnext = NULL;
	ptemp->next = NULL;
	printf("下面输入增加的车辆基本信息\n");
	printf("--------------------------\n");
	printf("请输入车辆编号:"); 
	if (!scanf_s("%d", &ptemp->number))
	{
		printf("输入错误，自动退出！\n");
		free(ptemp);
		return TRUE;
	}
	getchar(); printf("\n");

	if (SeekCarNodeByNumber(gp_head, ptemp->number))
	{
		printf("编号重复，自动退出！\n");
		free(ptemp);
		return TRUE;
	}

	printf("请输入车牌号:"); 
	if (!scanf_s("%s", ptemp->car_num, sizeof(char) * 10))
	{
		printf("输入错误，自动退出！\n");
		free(ptemp);
		return TRUE;
	}
	getchar(); printf("\n");

	printf("请输入车辆类型编码:"); 
	if (!scanf_s("%c", &ptemp->car_id, sizeof(char)))
	{
		printf("输入错误，自动退出！\n");
		free(ptemp);
		return TRUE;
	}
	getchar(); printf("\n");

	printf("请输入车辆名称:"); 
	if (!scanf_s("%s", ptemp->car_name, sizeof(char) * 20))
	{
		printf("输入错误，自动退出！\n");
		free(ptemp);
		return TRUE;
	}
	getchar(); printf("\n");

	printf("请输入车辆排挡方式:"); 
	if (!scanf_s("%s", ptemp->car_gear, sizeof(char) * 10))
	{
		printf("输入错误，自动退出！\n");
		free(ptemp);
		return TRUE;
	}
    getchar(); printf("\n");

	printf("请输入每日租金:"); 
	if (!scanf_s("%f", &ptemp->fee))
	{
		printf("输入错误，自动退出！\n");
		free(ptemp);
		return TRUE;
	}
	getchar(); printf("\n");

	ptemp->id = 'n';     //自动默认设置状态为未出租
	rewind(stdin);

	ScreenFresh();
	iRet = InsertCarNode(&gp_head,ptemp);
	if (iRet == 1)       
		printf("车辆基本信息插入成功!\n");
	else if (iRet == -1)  
		printf("车辆已存在，插入失败！\n");
	else  
		printf("不存在该类车辆，插入失败！\n");
	SaveSysData(gp_head, FileName);
	return bRet;
}

/*
   函数名称：InsertCarNode
   函数功能：插入车辆基本信息节点
   输入参数：主链头指针地址phead,要插入的信息节点p
   输出参数:
   返回值：BOOL,TRUE说明添加成功，FALSE说明添加失败
   调用说明：
*/
BOOL InsertCarNode(HOME_NODE** phead,CAR_NODE* p)   //由于有多种输出，用Int
{
	HOME_NODE* phome_node=(HOME_NODE*)malloc(sizeof(HOME_NODE));
	CAR_NODE* pcar_temp;    //查找时用的
	int find = 0;

	pcar_temp = SeekCarNodeByCarNum(*phead, p->car_num);   //用车牌号类型查找

	if (pcar_temp != NULL)
	{
		find = -1;  /*该车辆已存在*/
	}

	else   /*所要插入的车辆不存在,将其插入到对应的车辆类型链下*/
	{
		for (phome_node = (*phead)->next; phome_node != NULL; phome_node = phome_node->next)
		{
			if (phome_node->car_id == p->car_id)
			{
				find = 1;  /*找到该车辆对应的类别*/
				break;
			}
		}
	}

	if (find)   //将该车辆插入对应的车辆分类链表,头插法
	{
		p->next = phome_node->snext;
		phome_node->snext = p;
	}
	else    //没找到对应的车辆类型
		free(p);
	return find;
}

/*
   函数名称：ModifyCar
   函数功能：修改车辆基本信息
   输入参数：无
   输出参数:
   返回值：BOOL,TRUE说明修改成功，FALSE说明修改失败
   调用说明：
*/
BOOL ModifyCar(void)
{
	rewind(stdin);
	BOOL bRet = TRUE;
	char car_num[10];
	ScreenFresh();
	printf("请输入要修改的车牌号：");
	if (!scanf_s("%s", car_num, sizeof(char)*10))
	{
		printf("输入错误，自动退出！\n");
		return TRUE;
	}
	getchar();

	if (!SeekCarNodeByCarNum(gp_head, car_num))
	{
		printf("未找到对应车辆，自动退出！\n");
		return TRUE;
	}

	CAR_NODE* ptemp = (CAR_NODE*)malloc(sizeof(CAR_NODE));
	ptemp->cnext = NULL;
	ptemp->next = NULL;
	printf("下面输入修改的车辆基本信息\n");
	printf("--------------------------\n");

	printf("请输入车牌号:");
	if (!scanf_s("%s", ptemp->car_num, sizeof(char) * 10))
	{
		printf("输入错误，自动退出！\n");
		free(ptemp);
		return TRUE;
	}
	getchar(); printf("\n");

	if (!SeekCarNodeByCarNum(gp_head, car_num))
	{
		printf("未找到对应车辆，自动退出！\n");
		return TRUE;
	}

	printf("请输入车辆类型编码:");
	if (!scanf_s("%c", &ptemp->car_id, sizeof(char)))
	{
		printf("输入错误，自动退出！\n");
		free(ptemp);
		return TRUE;
	}
	getchar(); printf("\n");

	printf("请输入车辆名称:");
	if (!scanf_s("%s", ptemp->car_name, sizeof(char) * 20))
	{
		printf("输入错误，自动退出！\n");
		free(ptemp);
		return TRUE;
	}
	getchar(); printf("\n");

	printf("请输入车辆排挡方式:");
	if (!scanf_s("%s", ptemp->car_gear, sizeof(char) * 10))
	{
		printf("输入错误，自动退出！\n");
		free(ptemp);
		return TRUE;
	}
	getchar(); printf("\n");

	printf("请输入每日租金:");
	if (!scanf_s("%f", &ptemp->fee))
	{
		printf("输入错误，自动退出！\n");
		free(ptemp);
		return TRUE;
	}
	getchar(); printf("\n");

	rewind(stdin);
	ScreenFresh();
	if (ModifyCarNode(&gp_head, car_num, ptemp) == TRUE)
	{
		printf("修改成功！\n");
		SaveSysData(gp_head, FileName);
	}
    else  
	{ 
		printf("修改失败！\n"); 
		free(ptemp); 
	}
    return bRet;
}

/*
   函数名称：ModifyHome
   函数功能：修改车辆基本信息结点
   输入参数：主链表头节点指针地址phead,车牌号car_num，存放修改信息的节点指针pcar_node
   输出参数:
   返回值：BOOL,TRUE说明修改成功，FALSE说明修改失败
   调用说明：
*/
BOOL ModifyCarNode(HOME_NODE **phead,char *car_num, CAR_NODE *pcar_node)
{
	BOOL bRet = FALSE;
	HOME_NODE* phome_node = (*phead)->next;
	HOME_NODE* pHomeNode;
	CAR_NODE* pcar_cur, * pcar_prior;
	int find = 0;
	while (phome_node != NULL)
	{
		pcar_prior = NULL;
		pcar_cur = phome_node->snext;
		while (pcar_cur != NULL)
		{
			if (strcmp(pcar_cur->car_num, car_num) == 0)
			{
				find = 1;
				break;    /*退出第一层循环*/
			}
			pcar_prior = pcar_cur;
			pcar_cur = pcar_cur->next;
		}
		if (find)
			break;/*退出第二层循环*/
		phome_node = phome_node->next;
	}

    if(pcar_cur != NULL)   //查找成功
    {
		if (pcar_cur->car_id != pcar_node->car_id)  //如果修改了类型
		{
			pHomeNode = (*phead)->next;
			while (pHomeNode != NULL && pHomeNode->car_id != pcar_node->car_id)
			{
				pHomeNode = pHomeNode->next;
			}
			if (pHomeNode != NULL)    
			{
				if (pcar_prior == NULL)
					phome_node->snext = pcar_cur->next;
				else
                    pcar_prior->next = pcar_cur->next;
				
				pcar_cur->next = pHomeNode->snext;  //插入
				pHomeNode->snext = pcar_cur;
			}
			else  //如果未找到，释放结点空间
			{
				printf("修改的类型不存在！\n");
				return FALSE;
			}
		}

		strcpy(pcar_cur->car_num, pcar_node->car_num);
		strcpy(pcar_cur->car_gear, pcar_node->car_gear);
		strcpy(pcar_cur->car_name, pcar_node->car_name);
		pcar_cur->car_id=pcar_node->car_id;
		pcar_cur->fee = pcar_node->fee;
        bRet = TRUE;
    }
    return bRet;
}

/*
   函数名称：DelCar
   函数功能：删除车辆基本信息
   输入参数：无
   输出参数:
   返回值：BOOL,TRUE说明删除成功，FALSE说明删除失败
   调用说明：
*/
BOOL DelCar(void)
{
    BOOL bRet=TRUE;
    char car_num[10];
    ScreenFresh();

	printf("请输入车牌号:");
	if (!scanf_s("%s", car_num, sizeof(char) * 10))
	{
		printf("输入错误，自动退出！\n");
		return TRUE;
	}

    getchar();
	if (DelCarNode(&gp_head, car_num) == TRUE)
	{
		printf("删除成功!\n");
		SaveSysData(gp_head, FileName);
	}
    else 
		printf("未找到该车辆，删除失败！\n");
    return bRet;
}

/*
   函数名称：DelCarNode
   函数功能：删除车辆基本信息结点
   输入参数：主链表头指针地址phead,车牌号car_num
   输出参数:
   返回值：BOOL,TRUE说明删除成功，FALSE说明删除失败
   调用说明：
*/
BOOL DelCarNode(HOME_NODE **phead,char *car_num)
{
    HOME_NODE* phome_node = (*phead)->next;
	CAR_NODE* pcar_prior;
	CAR_NODE* pcar_current;
	CHARGE_NODE* pcharge_prior;
	CHARGE_NODE* pcharge_current;
	BOOL bRet = FALSE;
    
    while(phome_node != NULL)
    {
        pcar_current = phome_node ->snext;
        pcar_prior = NULL;
        while(pcar_current != NULL && strcmp(pcar_current->car_num, car_num))
        {
            pcar_prior = pcar_current; 
            pcar_current = pcar_current ->next;
        }
        if(pcar_current != NULL)   //找到了车辆结点
        {
            bRet = TRUE;
            break;
        }
        phome_node = phome_node ->next;
    }

    if(phome_node != NULL)
    {
        if(pcar_prior == NULL)   /*删除的是车辆基本信息链的链头*/
			phome_node ->snext = pcar_current ->next;
        else
            pcar_prior ->next = pcar_current ->next;
        pcharge_current = pcar_current ->cnext;
        free(pcar_current);
        while(pcharge_current != NULL)
        {
			pcharge_prior = pcharge_current;
			pcharge_current = pcharge_current -> next;
            free(pcharge_prior);
        }
    }
    return TRUE;

}

//-------------------------------------------------------------------------------------------
//租车订单信息维护函数
/*
   函数名称：MaintainChargeInfo
   函数功能：数据维护，车辆租借基本信息
   输入参数：无
   输出参数:
   返回值：BOOL，总为TRUE
   调用说明：
*/
BOOL MaintainChargeInfo(void)
{
	char x;
	int choice;
	ChangeID(&gp_head);
	while (TRUE)
	{
		printf("现在开始维护租车信息：\n请选择：\n");
		printf("1.插入    2.修改     3.删除\n");
		scanf("%d", &choice);
		getchar();
		if (choice < 1 || choice>3)
		{
			char x;
			printf("输入错误，是否重新输入？y-是 n-否\n");
			scanf("%c", &x);
			getchar();
			if (x == 'y' || x == 'Y')
			{
				ClearScreen();
				ShowMenu();
				continue;
			}
		}
		else
		{
			switch (choice)
			{
			case 1: InsertCharge(); break;
			case 2: ModifyCharge(); break;
			case 3: DelCharge(); break;
			}
		}

		rewind(stdin);
		//ChangeID(&gp_head);
		printf("\n请问是否继续维护？\n");
		scanf("%c", &x);
		getchar();
		if (x == 'y' || x == 'Y')
		{
			ClearScreen();
			ShowMenu();
			continue;
		}
		else
		{
			printf("\n本次维护结束---------按任意键退出\n");
			break;
		}
	}

	rewind(stdin);
	getchar();
	ClearScreen();
	ShowMenu();
	return TRUE;
}

/*
   函数名称：InsertCharge
   函数功能：添加订单信息
   输入参数：无
   输出参数:
   返回值：BOOL,TRUE说明添加成功，FALSE说明添加失败
   调用说明：
*/
BOOL InsertCharge(void)
{
	BOOL bRet = TRUE;
	CAR_NODE* pcar = (CAR_NODE*)malloc(sizeof(CAR_NODE));
	CHARGE_NODE* ptemp = (CHARGE_NODE*)malloc(sizeof(CHARGE_NODE));
	ptemp->next = NULL;
	char s1[18];
	char s2[18];
	char s3[18];
	ScreenFresh();
	printf("下面输入添加的订单信息：\n\n");
	ShowTime();
	//订单编号由取车日期自动生成
	printf("\n请输入客人身份证号:");
	if (!scanf_s("%s", ptemp->guest_id, sizeof(char) * 20))
	{
		printf("输入错误，自动退出！\n");
		free(ptemp);
		return TRUE;
	}
	getchar(); printf("\n");

	printf("请输入客人姓名:");
	if (!scanf_s("%s", ptemp->name, sizeof(char) * 20))
	{
		printf("输入错误，自动退出！\n");
		free(ptemp);
		return TRUE;
	}
	getchar(); printf("\n");

	printf("请输入客人电话号码:");
	if (!scanf_s("%s", ptemp->tel, sizeof(char) * 20))
	{
		printf("输入错误，自动退出！\n");
		free(ptemp);
		return TRUE;
	}
	getchar(); printf("\n");

	printf("请输入租用的车辆编号:");
	if (!scanf_s("%d", &ptemp->number))
	{
		printf("输入错误，自动退出！\n");
		free(ptemp);
		return TRUE;
	}
	getchar(); printf("\n");

	//查询租用的车辆编号是否合适
	pcar = SeekCarNodeByNumber(gp_head, ptemp->number);

	if (pcar == NULL)
	{
		printf("抱歉，所租车辆不在库中！将自动退出\n");
		free(ptemp);
		return TRUE;
	}

	if (pcar->id == 'y')
	{
		printf("%s %s %d %c\n", pcar->car_num, pcar->car_name, pcar->number, pcar->id);
		printf("抱歉，所租车辆已出租！将自动退出\n");
		free(ptemp);
		return TRUE;
	}

	printf("请输入取车时间   输入示例：2019/02/15-13:00\n\n");
	if (!scanf_s("%s", ptemp->time_of_get_car, sizeof(char) * 20))
	{
		printf("输入错误，自动退出！\n");
		free(ptemp);
		return TRUE;
	}
	getchar(); printf("\n");

	strcpy(s1, ptemp->time_of_get_car);

	if (!time_test(ptemp->time_of_get_car))
	{
		printf("输入错误，自动退出！\n");
		free(ptemp);
		return TRUE;
	}
	printf("请输入预约还车时间   输入示例：2019/02/15-13:00\n\n");
	if (!scanf_s("%s", ptemp->time_of_scheduled_back, sizeof(char) * 20))
	{
		printf("输入错误，自动退出！\n");
		free(ptemp);
		return TRUE;
	}
	getchar(); printf("\n");

	strcpy(s2, ptemp->time_of_scheduled_back);

	if (!time_test(ptemp->time_of_scheduled_back) || strcmp(ptemp->time_of_get_car, ptemp->time_of_scheduled_back) > 0)
	{
		printf("输入错误，自动退出！\n");
		free(ptemp);
		return TRUE;
	}

	rewind(stdin);
	printf("车辆是否已经归还   是（y)  否（n)\n");
	char c;
	scanf("%c", &c);
	getchar();
	if (c == 'y' || c == 'Y')
	{
		printf("\n请输入实际还车时间     输入示例：2019/02/15-13:00\n\n");
		if (!scanf_s("%s", ptemp->time_of_actual_back, sizeof(char) * 18))
		{
			printf("输入错误，自动退出！\n");
			free(ptemp);
			return TRUE;
		}
		getchar();
		printf("\n");
		strcpy(s3, ptemp->time_of_actual_back);

		if (!time_test(ptemp->time_of_actual_back))
		{
			printf("输入错误，自动退出！\n");
			free(ptemp);
			return TRUE;
		}

		char time2[20];
		strcpy(time2, GetTime());   //当前时间

		if (strcmp(func(ptemp->time_of_actual_back), time2) > 0)  //比当前时间晚
		{
			printf("输入错误，自动退出！\n");
			free(ptemp);
			return TRUE;
		}
	}

	else if (c == 'n' || c == 'N')
	{
		strcpy(ptemp->time_of_actual_back, "0");   //未归还
		strcpy(s3, ptemp->time_of_actual_back);
		ptemp->actual_pay = 0.0;
	}

	else
	{
		printf("输入错误，自动退出\n");
		return TRUE;
	}

	char time0[9] = { "" };  //年月日信息
	strncpy(time0, ptemp->time_of_get_car, 8);
	time0[8] = '\0';

	//puts(time0);

	int count = 1;  //计算当前日期有几个订单
	HOME_NODE* p1 = gp_head->next;
	CAR_NODE* p2 = (CAR_NODE*)malloc(sizeof(CAR_NODE));
	CHARGE_NODE* p3 = (CHARGE_NODE*)malloc(sizeof(CHARGE_NODE));

	while (p1 != NULL)
	{
		p2 = p1->snext;
		while (p2 != NULL)
		{
			p3 = p2->cnext;
			while (p3 != NULL)
			{
				if (strstr(p3->time_of_get_car, time0))   //当前日期已有订单
					count++;
				p3 = p3->next;
			}
			p2 = p2->next;
		}
		p1 = p1->next;
	}

	//输入完全正确的情况下，计算费用，订单编号
	if (count >= 100)
	{
		printf("安全需要，自动停止\n");
		return TRUE;
	}
	char t[3] = { "" };
	itoa(count, t, 10);
	strcpy(ptemp->rent_id, ptemp->time_of_get_car);
	strncpy(ptemp->rent_id + 8, t, 3);
	ptemp->rent_id[10] = '\0';
	//在func前一定要保存原字符串

	ptemp->should_pay = pcar->fee * diff_day(func(ptemp->time_of_get_car), func(ptemp->time_of_scheduled_back));   //应缴费用
	ptemp->deposit = ptemp->should_pay * 5;

	if (strcmp(ptemp->time_of_actual_back, "0") > 0)
		ptemp->actual_pay = pcar->fee * diff_day(func(ptemp->time_of_get_car), func(ptemp->time_of_actual_back));   //实缴费用
	else
		ptemp->actual_pay = 0.0;

	strcpy(ptemp->time_of_get_car, s1);
	strcpy(ptemp->time_of_scheduled_back, s2);
	strcpy(ptemp->time_of_actual_back, s3);

	printf("\n经计算，费用情况如下：\n");
	printf("押金        应缴费用      实缴费用\n");
	printf("%-12.2f%-14.2f%-12.2f", ptemp->deposit, ptemp->should_pay, ptemp->actual_pay);

	rewind(stdin);
	printf("\n按任意键继续\n");
	getchar();
	ClearScreen();
	ShowMenu();
	ScreenFresh();

	int re = InsertChargeNode(&gp_head, ptemp);
	if (re == 1)
		printf("添加成功！\n");
	else if (re == -1)
		printf("该订单已经存在，添加失败！\n");
	else
		printf("该订单对应的车辆不存在，或者该订单不存在，插入失败！\n");

	SaveSysData(gp_head, FileName);
	return bRet;
}

/**
*函数名称：InsertChargeNode
*函数功能：在十字链表中插入一个缴费信息结点
*输入参数：hd主链头指针，pcharge_node是指向所要插入结点的指针
*输出参数：无
*返回值：int类型，1 OR -1 or else
*调用说明：
*/
BOOL InsertChargeNode(HOME_NODE** phead, CHARGE_NODE* pcharge_node)
{
	CAR_NODE* pcar_node = (CAR_NODE*)malloc(sizeof(CAR_NODE));
	CHARGE_NODE* ptemp_node = (CHARGE_NODE*)malloc(sizeof(CHARGE_NODE));
	BOOL bret = FALSE;
	//首先在十字链表中找到对应的车辆基本信息结点
	pcar_node = SeekCarNodeByNumber(*phead, pcharge_node->number);   //这里根据车辆编号查找
	if (pcar_node != NULL)
		ptemp_node = SeekChargeNode(*phead, pcharge_node->rent_id, pcar_node->car_num);
	if (ptemp_node != NULL)
	{
		bret = -1;    //该订单已经存在
	}
	else
	{
		if (pcar_node != NULL)
		{
			pcharge_node->next = pcar_node->cnext;
			pcar_node->cnext = pcharge_node;
			bret = TRUE;
		}
	}
	return bret;
}

/*
   函数名称：DelCharge
   函数功能：删除车辆租借订单信息
   输入参数：无
   输出参数:
   返回值：BOOL,TRUE说明删除成功，FALSE说明删除失败
   调用说明：
*/
BOOL DelCharge(void)
{
	char rent_id[20];
	ScreenFresh();
	rewind(stdin);
	printf("请输入你所要删除的订单编号\n\n");
	if (!scanf_s("%s", rent_id, sizeof(char) * 20))
	{
		printf("输入错误，自动退出！\n");
		return TRUE;
	}
	getchar(); printf("\n");

	if (DelChargeNode(&gp_head, rent_id) == TRUE)
	{
		printf("删除成功!\n");
		SaveSysData(gp_head, FileName);
	}
	else
		printf("该订单不存在，删除失败！\n");
	return TRUE;
}

/**
*函数名称：DelChargeNode
*函数功能：在十字链表中删除指定的租车订单信息结点
*输入参数：hd主链头指针，rent_id订单号，car_num车牌号
*输出参数：无
*返回值：BOOL类型，TRUE OR FALSE
*调用说明：根据订单号可以确定唯一的缴费信息
*/
BOOL DelChargeNode(HOME_NODE** phead, char* rent_id)    //删除租车订单信息结点
{
	HOME_NODE* p = (*phead)->next;
	CAR_NODE* pcar_node;
	CHARGE_NODE* pcharge_node_prior;
	CHARGE_NODE* pcharge_node_current;
	BOOL bRet = FALSE;

	while (p != NULL)
	{
		pcar_node = p->snext;
		while (pcar_node != NULL)
		{
			//在订单信息支链上查找给定订单号的订单信息结点
			pcharge_node_current = pcar_node->cnext;
			pcharge_node_prior = NULL;
			while (pcharge_node_current != NULL && strcmp(pcharge_node_current->rent_id, rent_id) != 0)
			{
				pcharge_node_prior = pcharge_node_current;           //向前查找
				pcharge_node_current = pcharge_node_current->next;
			}

			if (pcharge_node_current != NULL)  //如果找到
			{
				bRet = TRUE;
				if (pcharge_node_prior == NULL)  //如果位于订单支链上的第一个位置
				{
					pcar_node->cnext = pcharge_node_current->next;
				}
				else
				{
					pcharge_node_prior->next = pcharge_node_current->next;
				}
				free(pcharge_node_current);
				return TRUE;
			}

			pcar_node = pcar_node->next;   //没找到，继续找
		}
		p = p->next;
	}
	return bRet;
}

/*
   函数名称：ModifyCharge
   函数功能：修改租车订单信息
   输入参数：无
   输出参数:
   返回值：BOOL,TRUE说明修改成功，FALSE说明修改失败
   调用说明：
*/
BOOL ModifyCharge(void)
{
	char* time = GetTime();   //当前时间
	CAR_NODE* pcar = (CAR_NODE*)malloc(sizeof(CAR_NODE));
	rewind(stdin);
	BOOL  bRet = TRUE;
	char rent_id[20];
	CHARGE_NODE* ptemp = (CHARGE_NODE*)malloc(sizeof(CHARGE_NODE));
	ptemp->next = NULL;
	char s1[18];
	char s2[18];
	char s3[18];
	rewind(stdin);

	printf("请输入需要修改的订单编号\n");
	if (!scanf_s("%s", rent_id, sizeof(char) * 20))
	{
		printf("输入错误，自动退出！\n");
		free(ptemp);
		return TRUE;
	}
	getchar(); printf("\n");

	CHARGE_NODE* p = SeekChargeNodeByRentId(gp_head, rent_id);

	if (!p)
	{
		printf("未找到该订单！ 自动退出！\n");
		return TRUE;
	}

	pcar = SeekCarNodeByNumber(gp_head, p->number);
	ptemp->number = p->number;

	ScreenFresh();
	printf("下面输入修改的订单信息：\n\n");
	ShowTime();

	printf("\n请输入客人身份证号:");
	if (!scanf_s("%s", ptemp->guest_id, sizeof(char) * 20))
	{
		printf("输入错误，自动退出！\n");
		free(ptemp);
		return TRUE;
	}
	getchar(); printf("\n");

	printf("请输入客人姓名:");
	if (!scanf_s("%s", ptemp->name, sizeof(char) * 20))
	{
		printf("输入错误，自动退出！\n");
		free(ptemp);
		return TRUE;
	}
	getchar(); printf("\n");

	printf("请输入客人电话号码:");
	if (!scanf_s("%s", ptemp->tel, sizeof(char) * 20))
	{
		printf("输入错误，自动退出！\n");
		free(ptemp);
		return TRUE;
	}
	getchar(); printf("\n");

	printf("请输入取车时间   输入示例：2019/02/15-13:00\n\n");
	if (!scanf_s("%s", ptemp->time_of_get_car, sizeof(char) * 20))
	{
		printf("输入错误，自动退出！\n");
		free(ptemp);
		return TRUE;
	}
	getchar(); printf("\n");

	strcpy(s1, ptemp->time_of_get_car);

	if (!time_test(ptemp->time_of_get_car))
	{
		printf("输入错误，自动退出！\n");
		free(ptemp);
		return TRUE;
	}

	printf("请输入预约还车时间   输入示例：2019/02/15-13:00\n\n");
	if (!scanf_s("%s", ptemp->time_of_scheduled_back, sizeof(char) * 20))
	{
		printf("输入错误，自动退出！\n");
		free(ptemp);
		return TRUE;
	}
	getchar(); printf("\n");

	strcpy(s2, ptemp->time_of_scheduled_back);

	if (!time_test(ptemp->time_of_scheduled_back) || strcmp(ptemp->time_of_get_car, ptemp->time_of_scheduled_back) > 0)
	{
		printf("输入错误，自动退出！\n");
		free(ptemp);
		return TRUE;
	}

	rewind(stdin);
	printf("车辆是否已经归还   是（y)  否（n)\n");
	char c;
	scanf("%c", &c);
	getchar();
	if (c == 'y' || c == 'Y')
	{
		printf("\n请输入实际还车时间     输入示例：2019/02/15-13:00\n\n");
		if (!scanf_s("%s", ptemp->time_of_actual_back, sizeof(char) * 18))
		{
			printf("输入错误，自动退出！\n");
			free(ptemp);
			return TRUE;
		}
		getchar(); printf("\n");
		strcpy(s3, ptemp->time_of_actual_back);
		if (!time_test(ptemp->time_of_actual_back))
		{
			printf("输入错误，自动退出！\n");
			free(ptemp);
			return TRUE;
		}
	}

	else if (c == 'n' || c == 'N')
	{
		strcpy(ptemp->time_of_actual_back, "0");   //未归还
		strcpy(s3, ptemp->time_of_actual_back);
		ptemp->actual_pay = 0.0;
	}

	else
	{
		printf("输入错误，自动退出\n");
		return TRUE;
	}

	char time0[9] = { "" };  //年月日信息
	strncpy(time0, ptemp->time_of_get_car, 8);
	time0[8] = '\0';

	int count = 1;  //计算当前日期有几个订单
	HOME_NODE* p1 = gp_head->next;
	CAR_NODE* p2 = (CAR_NODE*)malloc(sizeof(CAR_NODE));
	CHARGE_NODE* p3 = (CHARGE_NODE*)malloc(sizeof(CHARGE_NODE));

	while (p1 != NULL)
	{
		p2 = p1->snext;
		while (p2 != NULL)
		{
			p3 = p2->cnext;
			while (p3 != NULL)
			{
				if (strstr(p3->time_of_get_car, time0))   //当前日期已有订单
					count++;
				p3 = p3->next;
			}
			p2 = p2->next;
		}
		p1 = p1->next;
	}

	//输入完全正确的情况下，计算费用，订单编号
	if (count >= 100)
	{
		printf("安全需要，自动停止\n");
		return TRUE;
	}
	char t[3] = { "" };
	itoa(count, t, 10);
	strcpy(ptemp->rent_id, ptemp->time_of_get_car);
	strncpy(ptemp->rent_id + 8, t, 3);
	ptemp->rent_id[10] = '\0';
	//在func前一定要保存原字符串
	ptemp->should_pay = pcar->fee * diff_day(func(ptemp->time_of_get_car), func(ptemp->time_of_scheduled_back));   //应缴费用
	ptemp->deposit = ptemp->should_pay * 5;

	if (strcmp(ptemp->time_of_actual_back, "0") > 0)
		ptemp->actual_pay = pcar->fee * diff_day(func(ptemp->time_of_get_car), func(ptemp->time_of_actual_back));   //实缴费用
	else
		ptemp->actual_pay = 0.0;

	strcpy(ptemp->time_of_get_car, s1);
	strcpy(ptemp->time_of_scheduled_back, s2);
	strcpy(ptemp->time_of_actual_back, s3);
	printf("\n经计算，费用情况如下：\n");
	printf("押金        应缴费用      实缴费用\n");
	printf("%-12.2f%-14.2f%-12.2f", ptemp->deposit, ptemp->should_pay, ptemp->actual_pay);

	rewind(stdin);
	printf("\n按任意键继续\n");
	getchar();
	ClearScreen();
	ShowMenu();
	ScreenFresh();

	if (ModifyChargeNode(&gp_head, rent_id, ptemp) == TRUE)
		printf("修改成功！\n");
	else
		printf("修改失败！\n");

	SaveSysData(gp_head, FileName);

	return bRet;
}

/**
*函数名称：ModifyChargeNode
*函数功能：对指定租车信息结点进行修改
*输入参数：hd主链头指针，pcharge_node指向存放修改内容的结点指针，rent_id订单号，car_num车牌号
*输出参数：无
*返回值：BOOL类型，TRUE OR FALSE
*调用说明：
*/
BOOL ModifyChargeNode(HOME_NODE** phead, char* rent_id, CHARGE_NODE* pcharge_node)
{
	CHARGE_NODE* pcharge_node_temp;
	CHARGE_NODE* pcharge_node_next;
	pcharge_node_temp = SeekChargeNodeByRentId(*phead, rent_id);   //查找指定缴费信息结点

	if (pcharge_node_temp != NULL)//查找成功，则进行修改
	{
		pcharge_node_next = pcharge_node_temp->next;  //赋值前保存指针域
		*pcharge_node_temp = *pcharge_node;
		pcharge_node_temp->next = pcharge_node_next;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}



//----------------------------------------------------------------------------------------------
//下面是一些辅助函数，有助于功能实现

/*
   函数名称：ChangeID
   函数功能：修改车辆分类信息
   输入参数：主链头指针地址phead
   输出参数:
   返回值：
   调用说明：这个函数很重要，根据订单信息修改车辆的出租状态
*/
void ChangeID(HOME_NODE** phead)       //车辆的出租状态根据订单和当前时间决定
{
	char time[20] = "\0";
	strcpy(time, GetTime());   //获取当前时间，标准12位字符串

	HOME_NODE* p = (HOME_NODE*)malloc(sizeof(HOME_NODE));
	CAR_NODE* pcar_temp = (CAR_NODE*)malloc(sizeof(CAR_NODE));
	CHARGE_NODE* p3 = (CHARGE_NODE*)malloc(sizeof(CHARGE_NODE));
	p = (*phead)->next;

	char status = 'n'; //默认状态是未出租
	char save[18];

	while (p != NULL)
	{
		pcar_temp = p->snext;

		while (pcar_temp != NULL)
		{
			status = 'n'; //默认状态是未出租
			p3 = pcar_temp->cnext;
			while (p3 != NULL)
			{
				strcpy(save, p3->time_of_actual_back);
				//有订单，先设出租状态为y
				status = 'y';

				//如果已经归还而且归还时间不超过当前
				if (strcmp(p3->time_of_actual_back, "0") != 0 && strcmp(func(p3->time_of_actual_back), time) < 0)  //已经归还
				{
					//printf("已归还!\n");
					status = 'n';   //已经归还了
				}

				strcpy(p3->time_of_actual_back, save);

				p3 = p3->next;
			}

			pcar_temp->id = status;
			pcar_temp = pcar_temp->next;
		}
		p = p->next;
	}
}

/*
   函数名称：SortCarNode
   函数功能：车辆基本信息排序，按编号
   输入参数：主链头指针地址phead
   输出参数:
   返回值：
   调用说明：谨慎使用
*/
void SortCarNode(HOME_NODE** phead)
{
	HOME_NODE* p1 = (*phead)->next;
	CAR_NODE* p2;
	while (p1 != NULL)
	{
		p2 = p1->snext;
		if (p2 != NULL)
		{
			CAR_NODE* p, * q;
			CAR_NODE* pre = p2;

			for (p = p2; p != NULL; p = p->next)
				for (q = p->next; q != NULL; q = q->next)
					if (p->number > q->number)
					{
						//交换数据域
						*pre = *q;
						*q = *p;
						*p = *pre;
						//交换next指针域
						(*pre).next = q->next;
						q->next = p->next;
						p->next = (*pre).next;
					}
		}
		p1 = p1->next;
	}
	return;
}

/*
   函数名称：SortHomeNode
   函数功能：为了csv文件里数据是顺序排列写的一个排序函数，存入文件前先排序
   输入参数：主链表头指针地址
   输出参数:
   返回值：无
   调用说明：
*/
void SortHomeNode(HOME_NODE** phead)
{
	HOME_NODE* p, * q;
	HOME_NODE* pre = (*phead);
	for (p = (*phead)->next; p != NULL; p = p->next)
		for (q = p->next; q != NULL; q = q->next)
			if ((p->car_id) > (q->car_id))
			{
				while (pre->next != p)
					pre = pre->next;
				p->next = q->next;
				pre->next = q;
				q->next = p;
			}
	return;
}

/*
   函数名称：SeekHomeNodeByCarID
   函数功能：方便后续函数查找，按照车辆编号car_id查找对应的车辆分类信息节点
   输入参数：主链表头指针，车辆编号id
   输出参数:
   返回值：HOME_NODE*，主链节点指针
   调用说明：
*/
HOME_NODE* SeekHomeNodeByCarID(HOME_NODE* phead, char car_id)
{
	HOME_NODE* p = phead->next;
	while (p != NULL)
	{
		if (p->car_id == car_id)   //找到了
		{
			return p;
			break;
		}
		p = p->next;
	}
	return NULL;
}

/*
   函数名称：SeekHomeNodeByCarName
   函数功能：方便后续函数查找，按照车辆类型名称type查找对应的车辆分类信息节点
   输入参数：主链表头指针，
   输出参数:
   返回值：BOOL,TRUE说明插入成功，FALSE说明插入失败
   调用说明：
*/
HOME_NODE* SeekHomeNodeByCarName(HOME_NODE* phead, char type[20])
{
	HOME_NODE* p = phead->next;
	while (p != NULL)
	{
		if (!strcmp(p->type, type))   //找到了
		{
			return p;
			break;
		}
		p = p->next;
	}
	return NULL;
}

/*
   函数名称：SeekCarNodeByCarNum
   函数功能：查找车辆基本信息结点 使用车牌号
   输入参数：主链头指针phead,车牌号car_num
   输出参数:
   返回值：CAR_NODE* 车辆节点指针
   调用说明：
*/
CAR_NODE* SeekCarNodeByCarNum(HOME_NODE* phead, char* car_num)   //用车牌号查找
{
	HOME_NODE* phome_node = phead->next;
	CAR_NODE* pcar_node;
	int find = 0;

	while (phome_node != NULL)
	{
		pcar_node = phome_node->snext;
		while (pcar_node != NULL)
		{
			if (strcmp(pcar_node->car_num, car_num) == 0)
			{
				find = 1;
				break;    /*退出第一层循环*/
			}
			pcar_node = pcar_node->next;
		}
		if (find)   break;/*退出第二层循环*/
		phome_node = phome_node->next;
	}
	if (find)
		return pcar_node;
	else
		return NULL;
}

/*
   函数名称：SeekCarNodeByNumber
   函数功能：查找车辆基本信息结点 使用车辆编号
   输入参数：主链头指针phead,车辆编号number
   输出参数:
   返回值：CAR_NODE* 车辆节点指针
   调用说明：
*/
CAR_NODE* SeekCarNodeByNumber(HOME_NODE* phead, int number)   //用车辆编号查找
{
	HOME_NODE* phome_node = phead->next;
	CAR_NODE* pcar_node;
	int find = 0;
	while (phome_node != NULL)
	{
		pcar_node = phome_node->snext;
		while (pcar_node != NULL)
		{
			if (pcar_node->number == number)
			{
				find = 1;
				break;    /*退出第一层循环*/
			}
			pcar_node = pcar_node->next;
		}
		if (find)
			break;/*退出第二层循环*/
		phome_node = phome_node->next;
	}
	if (find)
		return pcar_node;
	else
		return NULL;
}

/*
   函数名称：SeekChargeNode
   函数功能：查找租车订单结点
   输入参数：主链头指针，订单编号，车牌号
   输出参数:
   返回值：CHARGE_NODE* 订单节点指针
   调用说明：这个函数主要是为了插入租车信息节点时，先判断有没有这个车辆的重复订单，实际查找订单节点只有订单号
*/
CHARGE_NODE* SeekChargeNode(HOME_NODE* phead, char* rent_id, char* car_num)  //rent_id订单号，car_num是车牌号，首先要靠车牌号找到对应的车辆
{
	CAR_NODE* pcar_node;
	CHARGE_NODE* pcharge_node;
	int find = 0;
	pcar_node = SeekCarNodeByCarNum(phead, car_num);      /*首先查找对应的车辆信息结点*/
	/*如果找到，则在车辆信息支链上继续找指定的订单信息结点*/
	if (pcar_node != NULL)
	{
		pcharge_node = pcar_node->cnext;
		while (pcharge_node != NULL)
		{
			if (strcmp(pcharge_node->rent_id, rent_id) == 0)  //比对订单号
			{
				find = 1;
				break;
			}
			pcharge_node = pcharge_node->next;
		}
	}
	if (find)
		return pcharge_node;
	else
		return NULL;
}

/*
   函数名称：SeekChargeNodeByRentId
   函数功能：查找租车订单结点
   输入参数：主链头指针，订单编号
   输出参数:
   返回值：CHARGE_NODE* 订单节点指针
   调用说明：
*/
CHARGE_NODE* SeekChargeNodeByRentId(HOME_NODE* phead, char* rent_id)  //rent_id订单号
{
	HOME_NODE* p = phead->next;
	CAR_NODE* pcar_node;
	CHARGE_NODE* pcharge_node;
	while (p != NULL)
	{
		pcar_node = p->snext;
		while (pcar_node != NULL)
		{
			pcharge_node = pcar_node->cnext;
			while (pcharge_node != NULL)
			{
				if (strcmp(pcharge_node->rent_id, rent_id) == 0)
					return pcharge_node;
				pcharge_node = pcharge_node->next;
			}
			pcar_node = pcar_node->next;   //没找到，继续找
		}
		p = p->next;
	}
	return NULL;
}

/*
   函数名称：ShowTime
   函数功能：显示时间
   输入参数：无
   输出参数: 无
   返回值：无
   调用说明：用户维护订单信息输入时间时，提示当前时间
*/
void ShowTime(void)
{
	int  x = 0;
	char str[100];
	static time_t tm;
	struct tm* lt;
	if (time(NULL) != tm)  x = 1;
	tm = time(NULL);
	lt = localtime(&tm);
	strftime(str, 100, "%Y年%m月%d日 %H:%M", lt);
	if (x)
	{
		printf("当前时间：%s\n", str);
	}
}

/*
   函数名称：time_test
   函数功能：检查时间是否合格
   输入参数：时间
   输出参数:
   返回值：int,1说明输入的时间合格，0表示输入的时间不合格
   调用说明：这个函数很重要，是在后面修改订单信息时，需要输入时间信息，我们要尽可能杜绝错误的，不合逻辑的时间输入，
            所以写了这个函数，我们首先判断长度是否合适，再就是分离出年月日时间等，判断其是否符合逻辑
*/
int time_test(char time[])  //检查输入订单的时间是否符合规范
{
	if (strlen(time) != 16 && strlen(time) != 17 && strlen(time) != 10)
	{
		puts("未按规范输入");
		return 0;   //未按规范输入
	}
	char* str = func(time);
	if (strlen(time) != 12)
	{
		puts("未正确执行func");
		return 0;   //未按规范输入
	}

	char year[5];
	char month[3];
	char day[3];
	char hour[3];
	char minute[3];

	int i = 0, j = 0;
	while (str[i] != '\0')
	{
		while (i < 4)
		{
			year[j] = str[i];
			j++;
			i++;
		}
		year[j] = '\0';

		if (strcmp(year, "201001010000") < 0 || strcmp(year, "203001010000") > 0)
			return 0;
		j = 0;

		while (i < 6)
		{
			month[j] = str[i];
			j++;
			i++;
		}
		month[j] = '\0';

		if (strcmp(month, "01") < 0 || strcmp(month, "12") > 0)
			return 0;
		j = 0;

		while (i < 8)
		{
			day[j] = str[i];
			j++;
			i++;
		}
		day[j] = '\0';

		if (strcmp(day, "01") < 0 || strcmp(day, "31") > 0)   //这里还不严谨要考虑闰年和31，30号，只是个大概估计
			return 0;
		j = 0;

		while (i < 10)
		{
			hour[j] = str[i];
			j++;
			i++;
		}
		hour[j] = '\0';

		if (strcmp(hour, "00") < 0 || strcmp(hour, "23") > 0)   //这里还不严谨要考虑闰年和31，30号，只是个大概估计
			return 0;
		j = 0;

		while (i < 12)
		{
			minute[j] = str[i];
			j++;
			i++;
		}
		minute[j] = '\0';
		if (strcmp(minute, "00") < 0 || strcmp(minute, "59") > 0)   //这里还不严谨要考虑闰年和31，30号，只是个大概估计
			return 0;
		j = 0;

		break;
	}
	return 1;
}

/*
   函数名称：GetTime
   函数功能：获得当前时间
   输入参数：无
   输出参数:
   返回值：字符串
   调用说明：这个函数是为了计算租界时间时判断时用的，很多时候需要获取当前的时间进行比较，比如实际还车时间不可能比当前时间晚
*/
char* GetTime(void)
{
	int  x = 0;
	char* str=(char*)malloc(sizeof(char)*100);
	static time_t tm;
	struct tm* lt;

	if (time(NULL) != tm)
		x = 1;
	tm = time(NULL);
	lt = localtime(&tm);

	strftime(str, 100, "%Y%m%d%H%M", lt);

	if (x)
	{
		return str;
	}

	return NULL;
}

/*
   函数名称：leapyear
   函数功能：闰年判断
   输入参数：无
   输出参数:
   返回值：int,1表示为闰年，0表示非闰年
   调用说明：计算时间差时用
*/
int leapyear(int year)
{
	if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
		return 1;
	else
		return 0;
}

/*
   函数名称：compare
   函数功能：如果第一个日期比第二个日期大，交换日期
   输入参数：无
   输出参数:
   返回值：没什么特殊作用
   调用说明：
*/
int compare(DATE_w* d1, DATE_w* d2)				//如果第一个日期比第二个日期大，交换日期
{
	DATE_w* tmp;
	if (d1->year == d2->year)				//年数相等
	{
		if (d1->month > d2->month)			//月数相等
		{
			tmp = d1;
			d1 = d2;
			d2 = d1;
		}
		else if (d1->month == d2->month)		//日期相等
		{
			if (d1->day > d2->day)
			{
				tmp = d1;
				d1 = d2;
				d2 = d1;
			}
		}
	}
	else if (d1->year > d2->year)
	{
		tmp = d1;
		d1 = d2;
		d2 = tmp;
	}

	return 0;
}

/*
   函数名称：diff
   函数功能：计算两个时间结构体相差天数
   输入参数：两个时间结构体指针
   输出参数:
   返回值：相差天数
   调用说明：
*/
int diff(DATE_w* date1, DATE_w* date2)
{
	int i;
	int diff = 0;
	const int month[13] = { 0,31,28,31,30,31,30,31,31,30,31,30,31 };
	if (date1->year == date2->year)
	{
		if (date1->month == date2->month)
		{
			diff = date2->day - date1->day;
		}
		else
		{
			for (i = date1->month + 1; i < date2->month; i++)
			{
				diff += month[i];
			}
			diff += month[date1->month] - date1->day + date2->day;
			if (leapyear(date1->year))
				if (date1->month <= 2 && date2->month > 2)
					diff++;
		}
	}
	else
	{
		for (i = date1->year + 1; i < date2->year; i++)
		{
			if (leapyear(i))
				diff += 366;
			else
				diff += 365;
		}
		for (i = date1->month + 1; i <= 12; i++)				//date1距离年末多少天
		{
			diff += month[i];
		}
		diff += month[date1->month] - date1->day;
		if (date1->month <= 2)
			if (leapyear(date1->year))
				diff++;
		for (i = 1; i < date2->month; i++)					//date2距离年初多少天
		{
			diff += month[i];
		}
		diff += date2->day;
		if (date1->month > 2)
			if (leapyear(date2->year))
				diff++;
	}

	return diff;
}

/*
   函数名称：diff_day
   函数功能：计算天数差
   输入参数：两个标准12位纯数字字符串时间
   输出参数:
   返回值：时间差
   调用说明：输入字符串必须是标准12位，也就是经过func函数处理过的
*/
int diff_day(char time1[], char time2[])
{
	int days = 0;
	DATE_w day1, day2;
	DATE_w* date1, * date2;
	date1 = &day1;
	date2 = &day2;
	//先要把时间转到结构体中
	char year[5] = { "" };
	char month[3] = { "" };
	char day[3] = { "" };
	strncpy(year, time1, 4); year[4] = '\0';
	strncpy(month, time1 + 4, 2); month[2] = '\0';
	strncpy(day, time1 + 6, 2); day[2] = '\0';

	date1->year = atoi(year);
	date1->month = atoi(month);
	date1->day = atoi(day);

	strncpy(year, time2, 4); year[4] = '\0';
	strncpy(month, time2 + 4, 2); month[2] = '\0';
	strncpy(day, time2 + 6, 2); day[2] = '\0';

	date2->year = atoi(year);
	date2->month = atoi(month);
	date2->day = atoi(day);

	compare(date1, date2);
	days = diff(date1, date2);

	return days;
}

/*
   函数名称：ChangeCarInfoToStr
   函数功能：将车辆节点信息转化为一个字符串
   输入参数：车辆基本信息节点指针
   输出参数: 
   返回值：信息字符串
   调用说明：此函数是为了便于综合查找写的
*/
char* ChangeCarInfoToStr(CAR_NODE* pcarnode)   //将汽车基本信息转化为字符串，用逗号分隔
{
	char* x = "，";
	char* str = (char*)malloc(sizeof(char) * 100);
	//char str[100] = "\0";
	str[0] = '\0';
	char num[5];
	char id[2] = "\0";
	char str1[10] = "已出租";
	char str2[10] = "未出租";
	id[0] = pcarnode->car_id;
	itoa(pcarnode->number, num, 10);   //字符串转化为整数
	strcat(str, num);
	strcat(str, x);
	strcat(str, pcarnode->car_num);
	strcat(str, x);
	strcat(str, id);
	strcat(str, x);
	strcat(str, pcarnode->car_name);
	strcat(str, x);
	strcat(str, pcarnode->car_gear);
	strcat(str, x);
	if (pcarnode->id == 'y')
		strcat(str, str1);
	else
		strcat(str, str2);
	return str;
}

/*
   函数名称：satisfy
   函数功能：判断当前车辆基本节点是否满足组合条件
   输入参数：车辆基本信息节点指针，组合搜索条件mixsearch
   输出参数:
   返回值：BOOL，true表示符合，false表示不符合
   调用说明：mixsearch会改变，用此函数前要保存mixsearch
*/
BOOL satisfy(CAR_NODE* pcarnode, char mixsearch[])   //mixsearch表示用户输入的查询条件
{
	char str[100];
	strcpy(str, ChangeCarInfoToStr(pcarnode));   //先把当前节点的信息转化为字符串
	const char* delim = ", ，/";   //输入的内容中间是空格，中英文的逗号都要分开
	char* token;
	token = strtok(mixsearch, delim);  //先取第一个信息
	if (token == NULL)   //没有正确输入条件
		return FALSE;
	if (!strstr(str, token))  //车辆不满足条件
		return FALSE;
	else
	{
		while (token != NULL)
		{
			token = strtok(NULL, delim);
			if (token != NULL)
			{
				if (!strstr(str, token))
					return FALSE;
			}
		}
		return TRUE;  //符合
	}
}

/*
   函数名称：del_the_blank
   函数功能：处理租车订单里的时间，把/和空格,-，：删除
   输入参数：时间字符串
   输出参数: 无
   返回值：处理后的字符串
   调用说明：
*/
char* del_the_blank(char* str)   //处理租车订单里的时间，把/和空格,-，：删除
{
	int i, j;
	for (i = j = 0; str[i] != '\0'; i++)
	{
		if (str[i] != ' ' && str[i] != '/' && str[i] != '-' && str[i] != ':' && str[i] != '：' && str[i] != ',' && str[i] != '，')
		{
			str[j++] = str[i];
		}
	}
	str[j] = '\0';
	return str;
}

/*
   函数名称：pad
   函数功能：填充为标准时间
   输入参数：时间字符串
   输出参数:
   返回值：填充后的时间字符串
   调用说明：
*/
char* pad(char* str)   //填充为标准时间
{
	if (strlen(str) < 12)
	{
		int i = 0;
		while (str[i] != '\0')
			i++;
		int l = strlen(str);
		while (l < 12)
		{
			str[i++] = '0';
			l++;
		}
		str[l] = '\0';
	}
	return str;
}

/*
   函数名称：func
   函数功能：直接读入一个随意有分隔符的字符串，添加为标准12位时间  
   输入参数：时间字符串
   输出参数:
   返回值：标准12位纯数字时间
   调用说明：千万注意，此函数会改原字符串，所以调用前要先保存
*/
char* func(char* str)   //直接读入一个随意有分隔符的字符串，添加为标准12位时间  
{
	strcpy(str, del_the_blank(str));
	strcpy(str, pad(str));
	return str;
}

/*
   函数名称：SeekNode
   函数功能：根据客户名字搜索
   输入参数：客户统计链头指针以及客户名字
   输出参数:
   返回值：TOP_RENT_GUEST*
   调用说明：
*/
TOP_RENT_GUEST* SeekNode(TOP_RENT_GUEST* phead, char name[20])
{
	TOP_RENT_GUEST* p = phead->next;
	while (p != NULL)
	{
		if (!strcmp(p->guest_name, name))
		{
			return p;
			break;
		}
		p = p->next;
	}
	return NULL;
}

/*
   函数名称：countday
   函数功能：根据订单的租车时间和实际归还时间，指定年份计算天数差
   输入参数：订单的租车时间和实际归还时间，指定年份
   输出参数:
   返回值：相差天数
   调用说明：此函数在统计3，4，5中作用很大
*/
int countday(char time1[], char time2[], char year[5])  //year是当前查找年份
{
	int res = 0;
	int d;
	char year1[5];

	if (strstr(time1, year))     //匹配年份,如果是当年的订单
	{
		if (strcmp(time2, "000000000000") != 0)   //车辆已经归还
		{
			strncpy(year1, time2, 4);
			year1[4] = '\0';
			if (strcmp(year1, year) > 0)  //订单跨年了，租出在当年，归还不在指定年份
			{
				char m[9] = "\0";
				strcpy(m, year);
				strcat(m, "1231");
				m[8] = '\0';
				d = diff_day(time1, m);
				res += d;
			}
			else  //车辆租出，归还日期均在指定年份中
			{
				d = diff_day(time1, time2);
				res += d;
			}
		}
	}

	//不是指定年份租出的,但是是在指定年份还的
	else
	{
		if (strstr(time2, year))
		{
			char m[9] = "\0";
			strcpy(m, year);
			strcat(m, "0101");
			m[8] = '\0';
			d = diff_day(m, time2);
			res += d;
		}
	}

	return res;
}

//< 时间暂延0.05s
inline void Delay(void)
{
	int goal;
	goal = 50 + clock();
	while (goal > clock());
}

/**< 将光标置于pos位置处*/
inline void Gotoxy(COORD pos)
{
	gh_std_out = GetStdHandle(STD_OUTPUT_HANDLE);  /*获取标准输出设备句柄*/
	SetConsoleCursorPosition(gh_std_out, pos);      /*设置光标位置在pos处*/
}

/**< 刷屏函数.*/
inline void ScreenFresh(void)
{
	COORD home = { 0, 1 };
	//unsigned long size;
	int i;
	Gotoxy(home);
	for (i = 0; i < SCR_ROW; i++)
	{
		printf("%80s", " ");
		Sleep(35);
	}
	Gotoxy(home);
}

/**< 给主菜单栏设置热区.*/
void SetMainHotArea(void)
{
	int i, j;
	char aSort[5] = { 0 };
	char aTag[5] = { 1,2,3,4,5 };
	HOT_AREA areas;
	SMALL_RECT aArea[5] = { {2,0,8,0},{12,0,20,0},{24,0,30,0},{34,0,40,0},{44,0,54,0} };/*设置主菜单的热区范围*/
	areas.pArea = aArea;   /*使热区结构变量areas的成员pArea指向热区位置数组首元素*/
	areas.pSort = aSort;   /*使热区结构变量areas的成员pSort指向热区类别数组首元素*/
	areas.pTag = aTag;    /*使热区结构变量areas的成员pTag 指向热区编号数组首元素*/
	/*设置主菜单的热区中字符单元的热区类型和热区编号*/
	for (i = 0; i < 5; i++)
	{
		for (j = areas.pArea[i].Left; j <= areas.pArea[i].Right; j++)
			gp_scr_att[j] |= (areas.pSort[i] << 6) | (areas.pTag[i] << 2);
	}
}

/*
   函数名称：SortRentMostGuestNode
   函数功能：在客户统计中对车辆出租天数按降序排列
   输入参数：客户统计链头指针地址
   输出参数:
   返回值：无
   调用说明：
*/
void SortRentMostGuestNode(TOP_RENT_GUEST** phead)    //对车辆出租天数按降序排列
{
	TOP_RENT_GUEST* p, * q;
	TOP_RENT_GUEST pre;
	for (p = (*phead)->next; p != NULL; p = p->next)
		for (q = p->next; q != NULL; q = q->next)
			if (p->day < q->day)
			{
				//交换数据域
				pre = *q;
				*q = *p;
				*p = pre;
				//交换next指针域
				pre.next = q->next;
				q->next = p->next;
				p->next = pre.next;
			}
	return;
}

/*
   函数名称：SortCarMostRentNode
   函数功能：在年份出租天数最多车辆统计中对车辆出租天数按降序排列
   输入参数：车辆统计链头指针地址
   输出参数:
   返回值：无
   调用说明：
*/
void SortCarMostRentNode(CAR_RENT** phead)    //对车辆出租天数按降序排列
{
	CAR_RENT* p, * q;
	CAR_RENT pre;
	for (p = (*phead)->next; p != NULL; p = p->next)
		for (q = p->next; q != NULL; q = q->next)
			if (p->day < q->day)
			{
				//交换数据域
				pre = *q;
				*q = *p;
				*p = pre;
				//交换next指针域
				pre.next = q->next;
				q->next = p->next;
				p->next = pre.next;
			}
	return;
}


//-----------------------------------------------------------------------------------------------------
//查询模块
/*
   函数名称：QueryHomeInfo
   函数功能：车辆分类信息查询
   输入参数：无
   输出参数: 无
   返回值：BOOL,恒为TRUE
   调用说明：
*/
BOOL QueryHomeInfo(void)  //查询车库信息
{
	char x;
	int choice;
	while (TRUE)
	{
		printf("请问需要哪种查询？\n");
		printf("1.精准查询    2.模糊查询    3.综合查询   请输入编号\n");
		scanf("%d", &choice);
		//getchar();

		if (choice < 1 || choice>3)
		{
			char x;
			printf("输入错误，是否重新输入？y-是 n-否\n");
			scanf("%c", &x);
			getchar();
			if (x == 'y' || x == 'Y')
			{
				ClearScreen();
				ShowMenu();
				continue;
			}
		}
		else
		{
			switch (choice)
			{
			case 1: printf("请输入要查询的车辆类别\n");
				    char type[20];
					if (!scanf_s("%s", type, sizeof(char) * 20))
					{
						printf("输入错误！检查输入长度！此次查询失败！\n");
						return TRUE;
					}
					getchar();
					QueryHomeInfoNode1(gp_head, type);
				    break;
			case 2: QueryHomeInfoNode2(gp_head); break;
			case 3: QueryHomeInfoNode3(gp_head); break;
			}
		}

		rewind(stdin);
		printf("\n请问是否继续查询？\n");
		scanf("%c", &x);
		getchar();
		if (x == 'y' || x == 'Y')
		{
			ClearScreen();
			ShowMenu();
			continue;
		}
		else
		{
			printf("\n本次查询结束---------按任意键退出\n");
			break;
		}
	}

	rewind(stdin);
	getchar();
	ClearScreen();
	ShowMenu();
	return TRUE;
}

/*
   函数名称：QueryHomeInfoNode1
   函数功能：精准查询，以车辆类别为条件来查找并显示满足条件的车辆分类信息。
   输入参数：主链头指针，车辆类别名称
   输出参数: 无
   返回值：无
   调用说明：
*/
void QueryHomeInfoNode1(HOME_NODE* head,char type[20])
{
	ScreenFresh();
	HOME_NODE* p1 = head->next;
	CAR_NODE* p2;
	int count = 0;
	while (p1 != NULL)
	{
		if (strcmp(p1->type, type) == 0)
		{
			printf("找到相关类型的可租赁车辆的信息如下:\n"); Delay();
			p2 = p1->snext;
			while (p2 != NULL)
			{
				if (p2->id == 'n')
				{
					count++;
					printf("\n");
					printf("车牌号      车辆类型      车辆名称        排挡方式       每日租金\t\n"); Delay();
					printf("%s      %c           %-14s  %8s       %.2f\t\n", p2->car_num, p2->car_id, p2->car_name, p2->car_gear, p2->fee); Delay();
					printf("-------------------------------------------------------------------------------\n");
				}
				p2 = p2->next;
			}
		}
		p1 = p1->next;
	}
	if (count == 0)
		printf("没找到该类型 或者 该类型现在无可租车辆!\n");
}

/*
   函数名称：QueryHomeInfoNode2
   函数功能：模糊查询，如未指定车辆类型名称进行查询时，应显示所有当前可租赁车辆信息
   输入参数：主链头指针
   输出参数: 无
   返回值：无
   调用说明：
*/
void QueryHomeInfoNode2(HOME_NODE* head)
{
	rewind(stdin);
	HOME_NODE* p1 = head->next;
	CAR_NODE* p2;
	char type[20];
	int count = 0;
	int flag = 0;  //是否找到了类型
	printf("请输入车辆类别关键词\n");
	if (!scanf_s("%s", type, sizeof(char) * 20))
	{
		printf("输入错误！检查输入长度！此次查询失败！\n");
		return;
	}
	getchar();
	ScreenFresh();
	
	while (p1 != NULL)
	{
		if (strstr(p1->type, type))   //模糊查找
		{
			flag = 1;
			printf("找到可租赁车辆的信息如下:\n"); Delay();
			p2 = p1->snext;
			while (p2 != NULL)
			{
				if (p2->id == 'n')
				{
					count++;
					printf("\n");
					printf("车牌号      车辆类型      车辆名称        排挡方式       每日租金\t\n"); Delay();
					printf("%s      %c           %-14s  %8s       %.2f\t\n", p2->car_num, p2->car_id, p2->car_name, p2->car_gear, p2->fee); Delay();
					printf("-------------------------------------------------------------------------------\n");
				}
				p2 = p2->next;
			}
		}
		p1 = p1->next;
	}

	p1 = head->next;
	if (flag == 0)   //没有输入类型
	{
		printf("\n由于您未输入有效类型，找到所有可租赁车辆的信息如下:\n\n");
		while (p1 != NULL)
		{
			p2 = p1->snext;
			while (p2 != NULL)
			{
				if (p2->id == 'n')
				{
					count++;
					printf("\n");
					printf("车牌号      车辆类型      车辆名称        排挡方式       每日租金\t\n"); Delay();
					printf("%s      %c           %-14s  %8s       %.2f\t\n", p2->car_num, p2->car_id, p2->car_name, p2->car_gear, p2->fee); Delay();
					printf("-------------------------------------------------------------------------------\n");
				}
				p2 = p2->next;
			}
			p1 = p1->next;
		}
		printf("按车辆类型信息模糊检索结束!\n");
		return;
	}

	if (count == 0)
		printf("对不起，没有找到满足条件车辆！\n");
	printf("按车辆类型信息模糊检索结束!\n");
	return;
}

/*
   函数名称：QueryHomeInfoNode3
   函数功能：综合查询，如同时选定商务型和经济型进行查询时，应显示这两类车型的当前可租赁车辆信息
   输入参数：主链头指针
   输出参数: 无
   返回值：无
   调用说明：
*/
void QueryHomeInfoNode3(HOME_NODE* head)
{
	rewind(stdin);
	char n[50];
	printf("请问需要查找哪些类型的车辆\n");
	fgets(n, 50, stdin);
	int count = 0;
	
	HOME_NODE* p1 = head->next;
	CAR_NODE* p2;
	while (p1 != NULL)
	{
		if (strstr(n, p1->type))
		{
			count++;
			if(count==1)
			     printf("找到相关类型的可租赁车辆的信息如下:\n"); Delay();
			p2 = p1->snext;
			while (p2 != NULL)
			{
				if (p2->id == 'n')
				{
					count++;
					printf("\n");
					printf("车牌号      车辆类型      车辆名称        排挡方式       每日租金\t\n"); Delay();
					printf("%s      %c           %-14s  %8s       %.2f\t\n", p2->car_num, p2->car_id, p2->car_name, p2->car_gear, p2->fee); Delay();
					printf("-------------------------------------------------------------------------------\n");
				}
				p2 = p2->next;
			}
		}
		p1 = p1->next;
	}
	if (count == 0)
		printf("对不起，没有找到满足条件车辆！\n");
}

/*
   函数名称：QueryHomeInfo
   函数功能：车辆基本信息查询
   输入参数：无
   输出参数: 无
   返回值：BOOL,恒为TRUE
   调用说明：
*/
BOOL QueryCarInfo(void) //查询汽车基本信息
{
	int choice;
	char x;
	while (TRUE)
	{
		printf("请问需要哪种查询？\n");
		printf("1.车牌号查询    2.车辆名称查询    3.出租状态查询    4.多条件组合    请输入编号\n");
		scanf("%d", &choice);
		getchar();
		
		if (choice < 1 || choice>4)
		{
			printf("输入错误，是否重新输入？y-是 n-否\n");
			scanf("%c", &x);
			getchar();
			if (x == 'n' || x == 'N')
				break;
			else
			{
				//getch();
				ClearScreen();
				ShowMenu();
				continue;
			}
		}
		else
		{
			switch (choice)
			{
			case 1: QueryCarInfoNode1(gp_head); break;
			case 2: QueryCarInfoNode2(gp_head); break;
			case 3: QueryCarInfoNode3(gp_head); break;
		    case 4: QueryCarInfoNode4(gp_head); break;
			}
		}

		rewind(stdin);
		printf("\n请问是否继续查询？\n");
		scanf("%c", &x);
		getchar();
		if (x == 'y' || x == 'Y')
		{
			ClearScreen();
			ShowMenu();
			continue;
		}
		else
		{
			printf("\n本次查询结束---------按任意键退出\n");
			break;
		}
	}
	rewind(stdin);
	getchar();
	ClearScreen();
	ShowMenu();
	return TRUE;
}

/*
   函数名称：QueryCarInfoNode1
   函数功能：以车牌号码为条件，查找并显示满足条件的车辆基本信息，支持模糊查询
   输入参数：主链头指针
   输出参数: 无
   返回值：无
   调用说明：
*/
void QueryCarInfoNode1(HOME_NODE * head)
{
	rewind(stdin);

	printf("请输入要查询的车牌号或车牌号关键词\n");
	char car_num[10];
	if (!scanf_s("%s", car_num, sizeof(char) * 10))
	{
		printf("输入错误！检查输入长度！此次查询失败！\n");
		return;
	}
	getchar();
	ScreenFresh();
	HOME_NODE* p1 = head->next;
	CAR_NODE* p2;
	int count=0;
	while (p1 != NULL)
	{
		p2 = p1->snext;
		while (p2 != NULL)
		{
			if (strstr(p2->car_num, car_num))
			{
				count++;
				if (count == 1)
					printf("找到车辆信息如下:\n"); Delay();
				printf("\n");
				printf("车牌号      车辆类型      车辆名称        排挡方式       每日租金     是否已出租\t\n"); Delay();
				printf("%s      %c           %-14s  %8s       %.2f        %c\t\n", p2->car_num,p2->car_id, p2->car_name, p2->car_gear, p2->fee,p2->id); Delay();
				printf("-------------------------------------------------------------------------------\n");
			}
			p2 = p2->next;
		}
		p1 = p1->next;
	}
	if (count == 0)
		printf("对不起，没有找到满足条件车辆！\n");
}

/*
   函数名称：QueryCarInfoNode2
   函数功能：以车辆名称为条件，查找并显示满足条件的车辆基本信息，支持模糊查询
   输入参数：主链头指针
   输出参数: 无
   返回值：无
   调用说明：
*/
void QueryCarInfoNode2(HOME_NODE* head)
{
	printf("请输入要查询的车名或车名关键词\n");
	char car_name[20];
	getchar();
	if (!scanf_s("%s", car_name, sizeof(char) * 20))
	{
		printf("输入错误！检查输入长度！此次查询失败！\n");
		return;
	}
	HOME_NODE* p1 = head->next;
	CAR_NODE* p2;
	int count=0;
	while (p1 != NULL)
	{
		p2 = p1->snext;
		while (p2 != NULL)
		{
			if (strstr(p2->car_name, car_name))
			{
				count++;
				if (count == 1)
					printf("找到车辆信息如下:\n"); Delay();
				printf("\n");
				printf("车牌号      车辆类型      车辆名称        排挡方式       每日租金     是否已出租\t\n"); Delay();
				printf("%s      %c           %-14s  %8s       %.2f        %c\t\n", p2->car_num, p2->car_id, p2->car_name, p2->car_gear, p2->fee, p2->id); Delay();
				printf("-------------------------------------------------------------------------------\n");
			}
			p2 = p2->next;
		}
		p1 = p1->next;
	}
	if (count == 0)
		printf("对不起，没有找到满足条件车辆！\n");
}

/*
   函数名称：QueryCarInfoNode3
   函数功能：以出租状态为条件，查找并显示满足条件的车辆基本信息.例如，查找并显示当前未出租车辆基本信息
   输入参数：主链头指针
   输出参数: 无
   返回值：无
   调用说明：
*/
void QueryCarInfoNode3(HOME_NODE* head)
{
	HOME_NODE* p1 = head->next;
	CAR_NODE* p2;
	int count = 0;
	char id;
	printf("请输入查找条件： 已出租-y 未出租-n\n");
	if (!scanf_s("%c", &id, sizeof(char)))
	{
		printf("输入错误，自动退出！\n");
		return;
	}
	getchar();
	if(id!='y' && id!='n')
	{
		printf("输入错误，自动退出！\n");
		return;
	}
	while (p1 != NULL)
	{
		p2 = p1->snext;
		while (p2 != NULL)
		{
			if (p2->id == id )
			{
				count++;
				if (count == 1)
					printf("\n找到车辆信息如下：\n"); Delay();
				printf("\n");
				printf("车牌号      车辆类型      车辆名称        排挡方式       每日租金     是否已出租\t\n"); Delay();
				printf("%s      %c           %-14s  %8s       %.2f        %c\t\n", p2->car_num, p2->car_id, p2->car_name, p2->car_gear, p2->fee, p2->id); Delay();
				printf("-------------------------------------------------------------------------------\n");
			}
			p2 = p2->next;
		}
		p1 = p1->next;
	}
	if (count == 0)
		printf("对不起，没有找到满足条件车辆！\n");
}

/*
   函数名称：QueryCarInfoNode4
   函数功能：以车牌号码、车辆名称、出租状态多条件组合，查找并显示满足条件的车辆基本信息。
   输入参数：主链头指针
   输出参数: 无
   返回值：无
   调用说明：此查询最重要，具体实现过程见设计部分
*/
void QueryCarInfoNode4(HOME_NODE* head)
{
	rewind(stdin);
	printf("请输入多种条件进行查找，例如别克英朗，未出租，车牌为鄂AW223Q等等\n");
	char mixsearch[100];
	fgets(mixsearch, 100, stdin);
	int i = 0;                          //处理fgets最后的回车符
	while (mixsearch[i] != '\n')
		i++;
	mixsearch[i] = '\0';


	char save[100];   //认清char* s和char s[100]的区别，如果时第一种，s=mixsearch，那么这两者就完全相等了
	strcpy(save,mixsearch);

	HOME_NODE* p1 = head->next;
	CAR_NODE* p2;
	int count = 0;  
	while (p1 != NULL)
	{
		p2 = p1->snext;
		while (p2 != NULL)
		{
			if ( satisfy(p2, mixsearch) )
			{
				count++;
				if(count==1)
					printf("\n所有满足条件车辆信息如下：\n");
				printf("\n");
				printf("车牌号      车辆类型      车辆名称        排挡方式       每日租金     是否已出租\t\n"); Delay();
				printf("%s      %c           %-14s  %8s       %.2f        %c\t\n", p2->car_num, p2->car_id, p2->car_name, p2->car_gear, p2->fee, p2->id); Delay();
				printf("-------------------------------------------------------------------------------\n");
			}

			strcpy(mixsearch, save);
			p2 = p2->next;
		}
		p1 = p1->next;
	}
	if (count == 0)
		printf("对不起，没有找到满足条件车辆！\n");
}

/*
   函数名称：QueryChargeInfo
   函数功能：租车订单查询
   输入参数：无
   输出参数: 无
   返回值：BOOL,恒为TRUE
   调用说明：
*/
BOOL QueryChargeInfo(void)
{
	rewind(stdin);
	char x;
	int choice;
	while (TRUE)
	{
		printf("请问需要哪种查询？\n");
		printf("1.客人信息查询    2.车辆信息查询    3.时间范围查询        请输入编号\n");
		scanf("%d", &choice);
		getchar();
		if (choice < 1 || choice>4)
		{
			printf("输入错误，是否重新输入？y-是 n-否\n");
			scanf("%c", &x);
			if (x == 'n' || x == 'N')
				break;
			else
				continue;
		}
		else
		{
			switch (choice)
			{
			case 1: QueryChargeInfoNode1(gp_head); break;
			case 2: QueryChargeInfoNode2(gp_head); break;
			case 3: QueryChargeInfoNode3(gp_head); break;
			}
		}

		rewind(stdin);
		printf("\n请问是否继续查询？\n");
		scanf("%c", &x);
		getchar();
		if (x == 'y' || x == 'Y')
		{
			ClearScreen();
			ShowMenu();
			continue;
		}
		else
		{
			printf("\n本次查询结束---------按任意键退出\n");
			break;
		}
	}

	rewind(stdin);
	getchar();
	ClearScreen();
	ShowMenu();
	return TRUE;
}

/*
   函数名称：QueryChargeInfoNode1
   函数功能：按客人身份证号为条件，查找并显示满足条件的客人租车订单信息信息。
   输入参数：主链头指针
   输出参数: 无
   返回值：无
   调用说明：
*/
void QueryChargeInfoNode1(HOME_NODE* head)   //本例按身份证号码查询
{
	rewind(stdin);
	HOME_NODE* p1 = head->next;
	CAR_NODE* p2;
	CHARGE_NODE* p3;
	int find = 0;
	char id[20];
	printf("请输入身份证号\n");
	if (!scanf_s("%s", id, sizeof(char) * 20))
	{
		printf("输入错误！检查输入长度！此次查询失败！\n");
		return;
	}
	getchar();
	while (p1 != NULL)
	{
		p2 = p1->snext;
		while (p2 != NULL)
		{
			p3 = p2->cnext;
			while (p3 != NULL)
			{
				if (!strcmp(p3->guest_id, id))   //找对应的订单
				{
					find++;
					printf("找到相关用户订单的信息如下:\n\n"); Delay();
				
					printf("订单编号:%s\n", p3->rent_id); Delay();
					printf("客人身份证号:%s\n", p3->guest_id); Delay();
					printf("客人姓名:%s\n", p3->name); Delay();
					printf("租用车辆编号:%d\n", p3->number); Delay();
					printf("客人电话:%s\n", p3->tel); Delay();
					printf("取车时间:%s\n", p3->time_of_get_car); Delay();
					printf("预约还车时间:%s\n", p3->time_of_scheduled_back); Delay();
					printf("实际还车时间:%s\n", p3->time_of_actual_back); Delay();
					printf("押金:%.2f\n", p3->should_pay*5.0); Delay();
					printf("应缴费用:%.2f\n", p3->should_pay); Delay();
					printf("实缴费用:%.2f\n", p3->actual_pay); Delay();
					printf("-------------------------------------------\n");
				}
				p3 = p3->next;
			}
			p2 = p2->next;
		}
		p1 = p1->next;
	}

	if (find == 0)
		printf("未找到该用户\n");
}

/*
   函数名称：QueryChargeInfoNode2
   函数功能：按车辆车牌号码为条件，查找并显示满足条件的租车订单信息。
   输入参数：主链头指针
   输出参数: 无
   返回值：无
   调用说明：
*/
void QueryChargeInfoNode2(HOME_NODE* head)    //本例按车牌号码查询
{
	rewind(stdin);
	
	char num[20];  //车牌号
	printf("请输入查找的车牌号\n\n");
	if (!scanf_s("%s", num, sizeof(char) * 20))
	{
		printf("输入错误！检查输入长度！此次查询失败！\n");
		return;
	}
	getchar();

	CAR_NODE* pcarnode = SeekCarNodeByCarNum(head, num);
	if (pcarnode == NULL) //查找节点失败
	{
		printf("未找到该车辆！\n");
		return;
	}

	CHARGE_NODE* p = pcarnode->cnext;
	if(p!=NULL)
		printf("找到该车辆所有订单如下：\n\n");

	while (p != NULL)
	{
		printf("订单编号:%s\n", p->rent_id); Delay();
		printf("客人身份证号:%s\n", p->guest_id); Delay();
		printf("客人姓名:%s\n", p->name); Delay();
		printf("租用车辆编号:%d\n", p->number); Delay();
		printf("客人电话:%s\n", p->tel); Delay();
		printf("取车时间:%s\n", p->time_of_get_car); Delay();
		printf("预约还车时间:%s\n", p->time_of_scheduled_back); Delay();
		printf("实际还车时间:%s\n", p->time_of_actual_back); Delay();
		printf("押金:%.2f\n", p->should_pay * 5.0); Delay();
		printf("应缴费用:%.2f\n", p->should_pay); Delay();
		printf("实缴费用:%.2f\n", p->actual_pay); Delay();
		printf("---------------------------------------\n");
		p = p->next;
	}
	printf("查找完毕\n");
}

/*
   函数名称：QueryChargeInfoNode3
   函数功能：以租车时间范围为条件，查找并显示满足条件的所有租车订单信息
   输入参数：主链头指针
   输出参数: 无
   返回值：无
   调用说明：此查找利用func函数以及很多辅助函数
*/
void QueryChargeInfoNode3(HOME_NODE* head)
{
	rewind(stdin);
	HOME_NODE* p1 = head->next;
	CAR_NODE* p2 = NULL;
	CHARGE_NODE* p3 = NULL;
	int find = 0;
	char time_start[20];
	char time_end[20];
	char s[20];
	char save_ptr[20];  //为了防止func改了结构体数据域，先保存

	printf("\n现在开始按时间范围查找\n");
	printf("-----------------------\n\n");
	printf("请输入起始时间:  如2019/02/13\n");
	if (!scanf_s("%s", time_start, sizeof(char) * 20))
	{
		printf("输入错误！检查输入长度！此次查询失败！\n");
		return;
	}
	getchar();

	if (!time_test(time_start))
	{
		printf("输入格式错误，自动退出！\n");
		return ;
	}

	printf("\n请输入结束时间:  如2019/02/18\n");
	if (!scanf_s("%s", time_end, sizeof(char) * 20))
	{
		printf("输入错误！检查输入长度！此次查询失败！\n");
	}
	getchar();

	if (!time_test(time_end))
	{
		printf("输入格式错误，自动退出！\n");
		return ;
	}

	if (strcmp(func(time_end), func(time_start)) < 0)  
	{
		printf("输入时间不符合逻辑！\n");
		return;
	}

	ClearScreen();
	ShowMenu();

	while (p1 != NULL)
	{
		p2 = p1->snext;
		while (p2 != NULL)
		{
			p3 = p2->cnext;
			while (p3 != NULL)
			{
				strcpy(save_ptr, p3->time_of_get_car);
				strcpy(s , func(p3->time_of_get_car));   //转化为标准时间字符串
				if (strcmp(func(time_start), s) <= 0 && strcmp(func(time_end), s) >= 0)
				{
					find++;
					if(find==1)
					    printf("找到相关订单的信息如下:\n"); Delay();
					printf("订单编号:%s\n", p3->rent_id); Delay();
					printf("客人身份证号:%s\n", p3->guest_id); Delay();
					printf("客人姓名:%s\n", p3->name); Delay();
					printf("租用车辆编号:%d\n", p3->number); Delay();
					printf("客人电话:%s\n", p3->tel); Delay();
					printf("取车时间:%s\n", p3->time_of_get_car); Delay();
					printf("预约还车时间:%s\n", p3->time_of_scheduled_back); Delay();
					printf("实际还车时间:%s\n", p3->time_of_actual_back); Delay();
					printf("押金:%.2f\n", p3->should_pay * 5.0); Delay();
					printf("应缴费用:%.2f\n", p3->should_pay); Delay();
					printf("实缴费用:%.2f\n", p3->actual_pay); Delay();
					printf("--------------------------------\n\n");
				}
				strcpy(p3->time_of_get_car, save_ptr); //还原
				p3 = p3->next;
			}
			p2 = p2->next;
		}
		p1 = p1->next;
	}
	if (find == 0)
		printf("未找到相关订单\n");
}


//-----------------------------------------------------------------------------------------
//统计       
//统计1
/*
   函数名称：Stat_Car_Type_Rent_Num
   函数功能：统计各个类型的出租数量和未出租数量
   输入参数：无
   输出参数: 无
   返回值：无
   调用说明：这只是个主函数，要调用下面三个子函数实现
*/
BOOL Stat_Car_Type_Rent_Num(void)    //车型出租数量统计 直接列出所有当前车型的
{
	RENT_STATUS* pheadstat1 = (RENT_STATUS*)malloc(sizeof(RENT_STATUS));   //头节点
	pheadstat1->next = NULL;
	ChangeCarNumber(&gp_head);
	Stat_Car_Type_Rent_Num_Info(gp_head,&pheadstat1);
	SaveStat1Data(pheadstat1);
	rewind(stdin);
	printf("按任意键继续\n");
	getchar();
	ClearScreen();
	ShowMenu();
	FreeSpace1(pheadstat1);
	return TRUE;
}

/*
   函数名称：Stat_Car_Type_Rent_Num_Info
   函数功能：统计各个类型的出租数量和未出租数量，进行节点添加
   输入参数：主链头指针，统计1链表头指针地址
   输出参数: 无
   返回值：无
   调用说明：此函数实现统计1链表的建立，节点的插入等
*/
void Stat_Car_Type_Rent_Num_Info(HOME_NODE* phead,RENT_STATUS** phead1)
{

	HOME_NODE* p1 = phead->next;
	CAR_NODE* p2;
	RENT_STATUS* ptail = *phead1;   //先进先出
	ptail->next = NULL;
	while (p1 != NULL)
	{
		RENT_STATUS* ptemp = (RENT_STATUS*)malloc(sizeof(RENT_STATUS));   //先初始化
		strcpy(ptemp->type, p1->type);
		ptemp->total_num = p1->num;   //车辆总数
		ptemp->rent_num = 0;
		ptemp->next = NULL;

		p2 = p1->snext;
		while (p2!=NULL)
		{
			if (p2->id == 'y')
				ptemp->rent_num++;
			p2 = p2->next;
		}
		ptail->next = ptemp;
		ptail = ptemp;
		p1 = p1->next;
	}
	ptail->next = NULL;
}

/*
   函数名称：FreeSpace1
   函数功能：释放统计1链表空间
   输入参数：统计1链表头指针
   输出参数: 无
   返回值：无
   调用说明：
*/
void FreeSpace1(RENT_STATUS* phead)
{
	RENT_STATUS* p_current = phead;
	RENT_STATUS* p_prior = NULL;
	while (p_current != NULL)
	{
		p_prior = p_current->next;
		free(p_current);
		p_current = p_prior;
	}
}

/*
   函数名称：SaveStat1Data
   函数功能：将统计1内容写入文件
   输入参数：统计1链表头指针
   输出参数: 无
   返回值：无
   调用说明：
*/
BOOL SaveStat1Data(RENT_STATUS* phead)
{
	int  x = 0;
	char str[100];
	static time_t tm;
	struct tm* lt;
	if (time(NULL) != tm)  x = 1;
	tm = time(NULL);
	lt = localtime(&tm);
	strftime(str, 100, "%Y年%m月%d日", lt);

	FILE* fout1;
	RENT_STATUS* p1 = (RENT_STATUS*)malloc(sizeof(RENT_STATUS));
	p1 = phead->next;
	if ((fout1 = fopen(StatFileName[0], "wb")) == NULL)
		return FALSE;
	fprintf(fout1, "%s %s,\n",str,"车辆出租信息统计");
	fprintf(fout1, "%s,%s,%s,%s,\n","车辆类型","车辆总数", "已出租数", "未出租数");
	while (p1 != NULL)
	{
		fprintf(fout1, "%s,%d,%d,%d,\n", p1->type,p1->total_num, p1->rent_num, p1->total_num-p1->rent_num);
		p1 = p1->next;
	}
	fclose(fout1);
	printf("相关数据已保存到文件!\n");
	return TRUE;
}

//统计2
/*
   函数名称：Stat_Car_Type_Month_Fee
   函数功能：统计各个类型的月营业额
   输入参数：无
   输出参数: 无
   返回值：无
   调用说明：这只是个主函数，要调用下面三个子函数实现
*/
BOOL Stat_Car_Type_Month_Fee(void)   //车型月营业额统计
{
	CAR_FEE* pheadstat = (CAR_FEE*)malloc(sizeof(CAR_FEE));   //头节点
	pheadstat->next = NULL;
	char year[5];
	rewind(stdin);
	printf("请输入需要查找的年份\n");
	if (!scanf_s("%s", year, sizeof(char) * 5))
	{
		printf("输入错误，自动退出！\n");
		rewind(stdin);
		printf("按任意键继续\n");
		getchar();
		ClearScreen();
		ShowMenu();
		return TRUE;
	}

	Stat_Car_Type_Month_Fee_Info(gp_head, &pheadstat, year);
	SaveStat2Data(pheadstat, year);
	rewind(stdin);
	printf("按任意键继续\n");
	getchar();
	ClearScreen();
	ShowMenu();
	FreeSpace2(pheadstat);
	return TRUE;
}

/*
   函数名称：Stat_Car_Type_Month_Fee_Info
   函数功能：统计各个类型的月营业额
   输入参数：主链头指针，统计2链表头指针地址，需要统计的年份
   输出参数: 无
   返回值：无
   调用说明：此函数实现统计2链表的建立，节点的插入等
*/
void Stat_Car_Type_Month_Fee_Info(HOME_NODE* phead, CAR_FEE** phead1, char year[5])
{
	//char s1[18];
	//char s2[18];
	int d;
	HOME_NODE* p1 = phead->next;
	CAR_NODE* p2;
	CHARGE_NODE* p3;
	CAR_FEE* ptail = *phead1;   //先进先出
	ptail->next = NULL;

	while (p1 != NULL)   //对车辆类型统计，每次要计算12个月的营业额，每个类型对应一个CAR_FEE节点
	{
		CAR_FEE* ptemp = (CAR_FEE*)malloc(sizeof(CAR_FEE));   //先初始化
		strcpy(ptemp->type, p1->type);
		int i = 0;
		while (i < 12)
			 ptemp->money[i++] = 0.0;
		ptemp->next = NULL;

		p2 = p1->snext;
		while (p2 != NULL)
		{
			p3 = p2->cnext;
			while (p3 != NULL)    //寻找当前车辆类型所有满足条件的订单
			{
				//strcpy(s1, p3->time_of_get_car);
				//strcpy(s2, p3->time_of_actual_back);
				if (strstr(p3->time_of_get_car, year))  //匹配年份,如果是当年的订单
				{
					int month1, month2;
					char month1_str[3];  //从订单信息提取月份
					char month2_str[3];

					strncpy(month1_str, p3->time_of_get_car+5, 2);
					month1_str[2] = '\0';
					month1 = atoi(month1_str);
					
					if (p3->time_of_actual_back != "0")  //已经归还
					{
						strncpy(month2_str, p3->time_of_actual_back + 5, 2);
						month2_str[2] = '\0';
						month2 = atoi(month2_str);

						int i = month1;
						for (i = month1; i < month2; i++)
						{
							if (i == 1 || i == 3 || i == 5 || i == 7 || i == 8 || i == 10 || i == 12)
								d = 31;
							else
								d = 30;
							ptemp->money[i - 1] += p2->fee * d;
						}
					}
					//没归还不算
				}
				//strcpy(p3->time_of_get_car, s1);
				//strcpy(p3->time_of_actual_back, s2);
				p3 = p3->next;
			}
			p2 = p2->next;
		}

		ptail->next = ptemp;
		ptail = ptemp;
		p1 = p1->next;
	}

	ptail->next = NULL;
}

/*
   函数名称：SaveStat2Data
   函数功能：将统计2内容写入文件
   输入参数：统计2链表头指针
   输出参数: 无
   返回值：无
   调用说明：
*/
BOOL SaveStat2Data(CAR_FEE* phead,char year[5])
{
	FILE* fout1;
	CAR_FEE* p1 = (CAR_FEE*)malloc(sizeof(CAR_FEE));
	p1 = phead->next;
	if ((fout1 = fopen(StatFileName[1], "wb")) == NULL)
		return FALSE;

	fprintf(fout1, "                                                                                               %s%s,\n", year, "年月营业额统计");

	fprintf(fout1, "%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,\n", "", "1月", "2月", "3月", "4月", "5月", "6月", "7月", "8月", "9月", "10月", "11月", "12月");
	while (p1 != NULL)
	{
		fprintf(fout1, "%s,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,\n", p1->type, 
			p1->money[0], p1->money[1], p1->money[2], p1->money[3], p1->money[4], p1->money[5], 
			p1->money[6], p1->money[7], p1->money[8], p1->money[9],p1->money[10], p1->money[11]);
		p1 = p1->next;
	}
	fclose(fout1);
	printf("相关数据已保存到文件!\n");
	return TRUE;
}

/*
   函数名称：FreeSpace2
   函数功能：释放统计2链表空间
   输入参数：统计2链表头指针
   输出参数: 无
   返回值：无
   调用说明：
*/
void FreeSpace2(CAR_FEE* phead)
{
	CAR_FEE* p_current = phead;
	CAR_FEE* p_prior = NULL;
	while (p_current != NULL)
	{
		p_prior = p_current->next;
		free(p_current);
		p_current = p_prior;
	}
}

//统计3
/*
   函数名称：Stat_Car_Fee_and_rent_rate
   函数功能：统计各个车辆的年营业额和出租率
   输入参数：无
   输出参数: 无
   返回值：无
   调用说明：这只是个主函数，要调用下面三个子函数实现
*/
BOOL Stat_Car_Fee_and_rent_rate(void)    //年营业额及租用率统计
{
	CAR_RENT* pheadstat = (CAR_RENT*)malloc(sizeof(CAR_RENT));   //头节点
	pheadstat->next = NULL;
	char year[5];
	rewind(stdin);
	printf("请输入需要查找的年份\n");
	if (!scanf_s("%s", year, sizeof(char) * 5))
	{
		printf("输入错误，自动退出！\n");
		rewind(stdin);
		printf("按任意键继续\n");
		getchar();
		ClearScreen();
		ShowMenu();
		return TRUE;
	}
	Stat_Car_Fee_and_rent_rate_Info(gp_head, &pheadstat, year);
	SaveStat3Data(pheadstat,year);

	rewind(stdin);
	printf("按任意键继续\n");
	getchar();
	ClearScreen();
	ShowMenu();
	FreeSpace3(pheadstat);
	return TRUE;
}

/*
   函数名称：Stat_Car_Fee_and_rent_rate_Info
   函数功能：统计各个车辆的年营业额和出租率
   输入参数：主链头指针，统计3链表头指针地址，需要统计的年份
   输出参数: 无
   返回值：无
   调用说明：此函数实现统计3链表的建立，节点的插入等
*/
void Stat_Car_Fee_and_rent_rate_Info(HOME_NODE* phead, CAR_RENT** phead1, char year[5])
{
	int count;  

	char time[20] = "\0";
	strcpy(time, GetTime());
	char year1[5];
	strncpy(year1, time, 4);
	year1[4] = '\0';
	
	if (strcmp(year, year1) < 0)
		count = 365;
	else if(strcmp(year, year1) > 0)
	{
		printf("年份输入错误！\n");
		return;
	}
	else
		count = diff_day("20200101", time);

	char s1[18];
	char s2[18];
	int d=0;
	HOME_NODE* p1 = phead->next;
	CAR_NODE* p2;
	CHARGE_NODE* p3;
	CAR_RENT* ptail = *phead1;   //先进先出
	ptail->next = NULL;

	while (p1 != NULL)
	{
		p2 = p1->snext;
		while (p2 != NULL)
		{
			CAR_RENT* ptemp = (CAR_RENT*)malloc(sizeof(CAR_RENT));   //先初始化
			strcpy(ptemp->car_name, p2->car_name);
			strcpy(ptemp->car_num, p2->car_num);
			ptemp->day = 0;
			ptemp->money = 0.0;
			ptemp->rent_rate = 0.0;
			ptemp->next = NULL;

			p3 = p2->cnext;
			while (p3 != NULL)
			{
				strcpy(s1, p3->time_of_get_car);
				strcpy(s2, p3->time_of_actual_back);
				
				d = countday(func(p3->time_of_get_car), func(p3->time_of_actual_back),year);
				ptemp->day += d;
				ptemp->money += p2->fee * d;
			
				strcpy(p3->time_of_get_car,s1);
				strcpy(p3->time_of_actual_back,s2);
				p3 = p3->next;
			}

			ptail->next = ptemp;
			ptail = ptemp;
			p2 = p2->next;
		}
		p1 = p1->next;
	}

	CAR_RENT* p_current = (*phead1)->next;
	while (p_current != NULL)
	{
		p_current->rent_rate = (float)p_current->day / count;
		p_current = p_current->next;
	}

	ptail->next = NULL;
}

/*
   函数名称：SaveStat3Data
   函数功能：将统计3内容写入文件
   输入参数：统计3链表头指针
   输出参数: 无
   返回值：无
   调用说明：
*/
BOOL SaveStat3Data(CAR_RENT* phead, char year[5])
{
	FILE* fout1;
	CAR_RENT* p1 = (CAR_RENT*)malloc(sizeof(CAR_RENT));
	p1 = phead->next;
	if ((fout1 = fopen(StatFileName[2], "wb")) == NULL)
		return FALSE;

	fprintf(fout1, "            %s%s,\n", year,"年车辆出租统计");
	fprintf(fout1, "%s,%s,%s,%s,%s,\n", "车牌号", "车辆名称", "总营业额", "租用率", "出租天数");
	while (p1 != NULL)
	{
		fprintf(fout1, "%s,%s,%.2f,%.2f,%d,\n", p1->car_num, p1->car_name, p1->money, p1->rent_rate,p1->day);
		p1 = p1->next;
	}
	fclose(fout1);
	printf("相关数据已保存到文件!\n");
	return TRUE;
}

/*
   函数名称：FreeSpace3
   函数功能：释放统计3链表空间
   输入参数：统计3链表头指针
   输出参数: 无
   返回值：无
   调用说明：
*/
void FreeSpace3(CAR_RENT* phead)
{
	CAR_RENT* p_current = phead;
	CAR_RENT* p_prior = NULL;
	while (p_current != NULL)
	{
		p_prior = p_current->next;
		free(p_current);
		p_current = p_prior;
	}
}

//统计4
/*
   函数名称：Stat_Car_Most_Rent
   函数功能：统计年出租车辆最多top10
   输入参数：无
   输出参数: 无
   返回值：无
   调用说明：这只是个主函数，要调用下面三个子函数实现
*/
BOOL Stat_Car_Most_Rent(void)  //最多出租车辆统计
{
	CAR_RENT* pheadstat = (CAR_RENT*)malloc(sizeof(CAR_RENT));   //头节点
	//CAR_RENT* p = pheadstat;
	pheadstat->next = NULL;
	char year[5];
	rewind(stdin);
	printf("请输入需要查找的年份\n");
	if (!scanf_s("%s", year, sizeof(char) * 5))
	{
		printf("输入错误，自动退出！\n");
		rewind(stdin);
		printf("按任意键继续\n");
		getchar();
		free(pheadstat);
		ClearScreen();
		ShowMenu();
		return TRUE;
	}
	Stat_Car_Most_Rent_Info(gp_head, &pheadstat, year);
	SortCarMostRentNode(&pheadstat);

	SaveStat4Data(pheadstat, year);
	
	rewind(stdin);
	printf("按任意键继续\n");
	getchar();
	ClearScreen();
	ShowMenu();
	FreeSpace3(pheadstat);
	return TRUE;
}

/*
   函数名称：SaveStat4Data
   函数功能：将统计4内容写入文件
   输入参数：统计4链表头指针
   输出参数: 无
   返回值：无
   调用说明：
*/
BOOL SaveStat4Data(CAR_RENT* phead, char year[5])
{
	
	int i = 1;
	FILE* fout1;
	CAR_RENT* p1 = (CAR_RENT*)malloc(sizeof(CAR_RENT));
	p1 = phead->next;
	if ((fout1 = fopen(StatFileName[3], "wb")) == NULL)
		return FALSE;

	fprintf(fout1, "                %s%s,\n", year, "年最多出租车辆Top10");
	fprintf(fout1, "%s,%s,%s,%s,\n", "车牌号", "车辆名称", "租用率", "出租天数");
	while (p1 != NULL)
	{
		i++;
		if (i > 11)  //只统计top10
			break;
		fprintf(fout1, "%s,%s,%.2f,%d,\n", p1->car_num, p1->car_name, p1->rent_rate, p1->day);
		p1 = p1->next;
	}
	fclose(fout1);
	printf("相关数据已保存到文件!\n");
	return TRUE;
}

/*
   函数名称：Stat_Car_Most_Rent_Info
   函数功能：统计年出租车辆最多top10
   输入参数：主链头指针，统计4链表头指针地址，需要统计的年份
   输出参数: 无
   返回值：无
   调用说明：此函数实现统计4链表的建立，节点的插入等
*/
void Stat_Car_Most_Rent_Info(HOME_NODE* phead, CAR_RENT** phead1, char year[5])
{
	int count;

	char time[20] = "\0";
	strcpy(time, GetTime());
	char year1[5];
	strncpy(year1, time, 4);
	year1[4] = '\0';

	if (strcmp(year, year1) < 0)
		count = 365;
	else
		count = diff_day("20200101", time);

	char s1[18];
	char s2[18];
	int d = 0;
	HOME_NODE* p1 = phead->next;
	CAR_NODE* p2;
	CHARGE_NODE* p3;
	CAR_RENT* ptail = *phead1;   //先进先出
	ptail->next = NULL;

	while (p1 != NULL)
	{
		p2 = p1->snext;
		while (p2 != NULL)
		{
			CAR_RENT* ptemp = (CAR_RENT*)malloc(sizeof(CAR_RENT));   //先初始化
			strcpy(ptemp->car_name, p2->car_name);
			strcpy(ptemp->car_num, p2->car_num);
			ptemp->day = 0;
			ptemp->money = 0.0;
			ptemp->rent_rate = 0.0;
			ptemp->next = NULL;

			p3 = p2->cnext;
			while (p3 != NULL)
			{
				strcpy(s1, p3->time_of_get_car);
				strcpy(s2, p3->time_of_actual_back);
				
				d = countday(func(p3->time_of_get_car), func(p3->time_of_actual_back),year);
				ptemp->day += d;
				ptemp->money += p2->fee * d;
					
				strcpy(p3->time_of_get_car, s1);
				strcpy(p3->time_of_actual_back, s2);
				p3 = p3->next;
			}

			ptail->next = ptemp;
			ptail = ptemp;
			p2 = p2->next;
		}
		p1 = p1->next;
	}
	ptail->next = NULL;

	CAR_RENT* p_current = (*phead1)->next;
	while (p_current != NULL)
	{
		p_current->rent_rate = (float)p_current->day / count;
		p_current = p_current->next;
	}
}

//统计5
/*
   函数名称：Stat_Guest_Most_Rent
   函数功能：统计年租车最多用户top5
   输入参数：无
   输出参数: 无
   返回值：无
   调用说明：这只是个主函数，要调用下面三个子函数实现
*/
BOOL Stat_Guest_Most_Rent(void)  //最常租车客户统计
{
	TOP_RENT_GUEST* pheadstat = (TOP_RENT_GUEST*)malloc(sizeof(TOP_RENT_GUEST));   //头节点
	pheadstat->next = NULL;
	char year[5];
	rewind(stdin);
	printf("请输入需要查找的年份\n");
	if (!scanf_s("%s", year, sizeof(char) * 5))
	{
		printf("输入错误，自动退出！\n");
		rewind(stdin);
		printf("按任意键继续\n");
		getchar();
		free(pheadstat);
		ClearScreen();
		ShowMenu();
		return TRUE;
	}

	Stat_Guest_Most_Rent_Info(gp_head, &pheadstat, year);
	SaveStat5Data(pheadstat, year);
	FreeSpace5(pheadstat);
	rewind(stdin);
	printf("按任意键继续\n");
	getchar();
	ClearScreen();
	ShowMenu();
	return TRUE;
}

/*
   函数名称：SaveStat5Data
   函数功能：将统计5内容写入文件
   输入参数：统计5链表头指针
   输出参数: 无
   返回值：无
   调用说明：
*/
BOOL SaveStat5Data(TOP_RENT_GUEST* phead, char year[5])
{
	SortRentMostGuestNode(&phead);
	int i = 1;
	FILE* fout1;
	TOP_RENT_GUEST* p1 = (TOP_RENT_GUEST*)malloc(sizeof(TOP_RENT_GUEST));
	p1 = phead->next;
	if ((fout1 = fopen(StatFileName[4], "wb")) == NULL)
		return FALSE;

	fprintf(fout1, "%s%s,\n", year, "年租车最多用户");
	fprintf(fout1, "%s,%s,\n", "客户", "租车天数");

	while (p1 != NULL)
	{
		i++;
		if (i > 6)  //只统计top5
			break;
		fprintf(fout1, "%s,%d,\n", p1->guest_name, p1->day);
		p1 = p1->next;
	}

	fclose(fout1);
	printf("相关数据已保存到文件!\n");
	return TRUE;
}

/*
   函数名称：FreeSpace5
   函数功能：释放统计5链表空间
   输入参数：统计5链表头指针
   输出参数: 无
   返回值：无
   调用说明：
*/
void FreeSpace5(TOP_RENT_GUEST* phead)
{
	TOP_RENT_GUEST* p_current = phead;
	TOP_RENT_GUEST* p_prior = NULL;
	while (p_current != NULL)
	{
		p_prior = p_current->next;
		free(p_current);
		p_current = p_prior;
	}
}

/*
   函数名称：Stat_Guest_Most_Rent_Info
   函数功能：统计年租车最多用户top5
   输入参数：主链头指针，统计5链表头指针地址，需要统计的年份
   输出参数: 无
   返回值：无
   调用说明：此函数实现统计5链表的建立，节点的插入等
*/
void Stat_Guest_Most_Rent_Info(HOME_NODE* phead, TOP_RENT_GUEST** phead1, char year[5])
{
	int d;
	char s1[18];
	char s2[18];
	HOME_NODE* p1 = phead->next;
	CAR_NODE* p2;
	CHARGE_NODE* p3;
	TOP_RENT_GUEST* ptail = *phead1;   //先进先出
	ptail->next = NULL;

	while (p1 != NULL)
	{
		p2 = p1->snext;
		while (p2 != NULL)
		{
			p3 = p2->cnext;
			while (p3 != NULL)
			{
				strcpy(s1, p3->time_of_get_car);
				strcpy(s2, p3->time_of_actual_back);
				if (strstr(p3->time_of_get_car, year))  //匹配年份,如果是当年的订单
				{

					TOP_RENT_GUEST* p = SeekNode(*phead1, p3->name);
					if (p == NULL)   //找到一个新用户
					{
						TOP_RENT_GUEST* ptemp = (TOP_RENT_GUEST*)malloc(sizeof(TOP_RENT_GUEST));   //先初始化
						strcpy(ptemp->guest_name, p3->name);
						ptemp->day = 0;
						ptemp->next = NULL;
		
						d = countday(func(p3->time_of_get_car), func(p3->time_of_actual_back),year);
						ptemp->day += d;
						
						ptail->next = ptemp;
						ptail = ptemp;
					}

					else  //该用户已存在
					{
						d = countday(func(p3->time_of_get_car), func(p3->time_of_actual_back), year);
						p->day += d;
					}
				}
				strcpy(p3->time_of_get_car, s1);
				strcpy(p3->time_of_actual_back, s2);
				p3 = p3->next;
		    }
			p2 = p2->next;
		}
		p1 = p1->next;
	}
	ptail->next = NULL;
}

//---------------------------------------------------------------------------
//关于
/*
   函数名称：HelpTopic
   函数功能：系统帮助
   输入参数：无
   输出参数: 无
   返回值：无
   调用说明：
*/
BOOL HelpTopic(void) //帮助主题
{
	printf("本系统可以使用 ALT+F/E/Q/S, F1，方向键等控制\n");
	rewind(stdin);
	printf("\n按任意键继续\n");
	getchar();
	ClearScreen();
	ShowMenu();
	return TRUE;
}

/*
   函数名称：AboutSys
   函数功能：关于系统
   输入参数：无
   输出参数: 无
   返回值：无
   调用说明：
*/
BOOL AboutSys(void)  //关于系统
{
	printf("\n\n2020-06-08 华中科技大学 网络空间安全学院 2018级 C语言课程设计\n\n");
	printf("信安1804 黄平奇 U201811237\n\n");
	rewind(stdin);
	printf("\n按任意键继续\n");
	getchar();
	ClearScreen();
	ShowMenu();
	return TRUE;
}

