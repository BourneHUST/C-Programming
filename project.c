//project�������ĵ���car.h��ͷ�ļ��������dorm��main�ǽ̲��еĲο�
#include "car.h"
unsigned long ul;     //ָ�������ָ��, ��������ַ���ʵ����Ŀ

int main()
{
	InitInterface();  //�ı��˵������ʼ��
	LoadData();   //�������ݵ��ڴ�
	RunSys(&gp_head);    /*ϵͳ����ģ���ѡ������*/
	CloseSys(gp_head);         /*�ر�ϵͳ*/
	return 0;
}

/**
*�������ƣ�LoadData
*�������ܣ�������������ݴ������ļ����뵽�ڴ滺������ʮ��������
*�����������
*�����������
*����ֵ��BOOL,���ܺ����г�ExitSys�ķ���ֵ����ΪFALSE�⣬�����ı���ΪTrue
*����˵����Ϊ��ͳһ���ø����ܺ�����ԭ�ͱ���һ�£��޲����ҷ���BOOL������FALSEʱ�����������
*/
BOOL LoadData(void)
{
	int Re = 0;
	Re = CreateList(&gp_head,FileName);
	if (Re < 28)
	{
		printf("\n���ݲ�������\n");
	}
	printf("\n�������ݼ��سɹ��������������\n");
	getch();
	ClearScreen();
	ShowMenu();
	return TRUE;
}

/**
*�������ƣ�CSV_read_HOME
*�������ܣ���ȡ����������Ϣ�ļ�
*���������phead����ͷָ���ַ����������������filename�ļ���
*�����������
*����ֵ��int ,0 ��ʾ�ɹ���-1��ʾʧ��
*����˵����
*/
int CSV_read_HOME(HOME_NODE** phead,char* filename)  //�������Ͷ�ȡ,�Ƚ��ȳ���������
{
	HOME_NODE* hd = (HOME_NODE*)malloc(sizeof(HOME_NODE));   //���Ǹ�ͷ�ڵ�
	HOME_NODE* tail = hd;
	FILE* fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("���ļ�ʧ�ܣ�\n");
		return -1;
	}
	//printf("����������Ϣ�ļ��򿪳ɹ���\n");
	char line[1024];
	fgets(line, sizeof(line), fp);  //��һ�в���
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
		printf("����������Ϣ�ļ�����ʧ�ܣ�\n");
		return -1;
	}
	*phead = hd;     //phead��ͷ�ڵ㣬û������
	fclose(fp);
	return 0;
}

/**
*�������ƣ�CSV_read_HOME
*�������ܣ���ȡ����������Ϣ�ļ�
*���������phead����ͷָ���ַ����������������filename�ļ���
*�����������
*����ֵ��int ,0 ��ʾ�ɹ���-1��ʾʧ��
*����˵����
*/
int CSV_read_CAR(HOME_NODE** phead, char* filename)  //�����������ݶ�ȡ,�Ƚ��ȳ���������
{
	HOME_NODE* pHomeNode = (HOME_NODE*)malloc(sizeof(HOME_NODE));
	pHomeNode->next = NULL;
	pHomeNode->snext = NULL;
	FILE* fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("ʧ�ܣ�\n");
		return -1;
	}
	char line[1024];
	fgets(line, sizeof(line), fp);  //��һ�в���
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
		else  //���δ�ҵ����ͷŽ��ռ�
		{
			free(pCarNode);
		}
	}
	fclose(fp);
	return 0;
}

/**
*�������ƣ�CSV_read_HOME
*�������ܣ���ȡ�⳵�����ļ�
*���������phead����ͷָ���ַ����������������filename�ļ���
*�����������
*����ֵ��int ,0 ��ʾ�ɹ���-1��ʾʧ��
*����˵����
*/
int CSV_read_CHARGE(HOME_NODE** phead, char* filename)  //�⳵�������ݶ�ȡ,�Ƚ��ȳ���������
{
	HOME_NODE* pHomeNode = (HOME_NODE*)malloc(sizeof(HOME_NODE));
	CAR_NODE* pCarNode = (CAR_NODE*)malloc(sizeof(CAR_NODE));
	FILE* fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("ʧ�ܣ�\n");
		return -1;
	}
	char line[1024];
	fgets(line, sizeof(line), fp);  //��һ�в���
	while (fgets(line, sizeof(line), fp))
	{
		//������㣬����ļ��ж�ȡ���⳵������Ϣ
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

		//���Ҹó�����Ϣ֧���϶�Ӧ���⳵�������
		pHomeNode = (*phead)->next;
		int find = 0;
		while (pHomeNode != NULL && find == 0)
		{
			pCarNode = pHomeNode->snext;
			while (pCarNode != NULL && find == 0)
			{
				if (pCarNode->number == pChargeNode->number)  //ͨ��������Ų���
				{
					find = 1;
					break;
				}
				pCarNode = pCarNode->next;
			}
			pHomeNode = pHomeNode->next;
		}

		//����ҵ�����ýڵ����Ƚ������ʽ����ѧ����Ϣ֧��
		if (find)
		{
			pChargeNode->next = pCarNode->cnext;
			pCarNode->cnext = pChargeNode;
			//printf("�Ѿ���ʼ��ȡ����֧��\n%s\n", pChargeNode->rent_id);
		}
		else  //���δ�ҵ����ͷŽ��ռ�
		{
			free(pChargeNode);
		}
	}
	fclose(fp);
	//printf("�⳵�����򿪳ɹ���\n");
	return 0;
}

/**
*�������ƣ�CreateList
*�������ܣ��������ļ���ȡ�������ݣ�����ŵ���������ʮ��������
*�������������ָ��
*���������phead����ͷָ��ĵ�ַ������������������ʮ����
*����ֵ��int ����ֵ����ʾ�����������
		  0��������������
		  4 �Ѽ��س���������Ϣ���ݣ��޳���������Ϣ���⳵��������
		  12 �Ѽ��س���������Ϣ���ݺͳ���������Ϣ�����⳵��������
		  28 �Ѽ��������������
*����˵������
*/
int CreateList(HOME_NODE** phead,char* filename[3])
{
	int re = 0;  //���ؽ��
	if (CSV_read_HOME(phead, filename[0]) == 0)   //��ȡ�ɹ�
	{
		re += 4;
		printf("\n����������Ϣ�ļ��򿪳ɹ���\n");
	}
	else
		return re;

	if (CSV_read_CAR(phead, filename[1]) == 0)
	{
		printf("����������Ϣ�ļ��򿪳ɹ���\n");
		re += 8;
	}
	else
		return re;

	if (CSV_read_CHARGE(phead, filename[2]) == 0)
	{
		printf("�⳵������Ϣ�ļ��򿪳ɹ���\n");
		re += 16;
	}

	ChangeCarNumber(&gp_head);
	SaveSysData(gp_head,FileName);
	return re;
}

/**
*�������ƣ�InitInterface
*�������ܣ��ı������ʼ��
*�����������
*�����������
*����ֵ����
*����˵����
*/
void InitInterface(void)
{
	COORD size = { SCR_COL,SCR_ROW };  //���ڻ�������С
	WORD att =  BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED; //����ǰ������������  
	gh_std_out = GetStdHandle(STD_OUTPUT_HANDLE);  //��ȡ��׼����豸���
	gh_std_in = GetStdHandle(STD_INPUT_HANDLE);    //��ȡ��׼�����豸���
	SetConsoleTitle(gp_sys_name);    //���ô��ڱ���
	SetConsoleScreenBufferSize(gh_std_out, size);  //���ô��ڻ�������С80*25
	SetConsoleTextAttribute(gh_std_out, att);   //���û�ɫǰ������ɫ����  //��ʡ��
	ClearScreen();   //����

	//��������������Ϣ��ջ������ʼ����Ķ���Ļ���ڵ�����һ�㵯������
	gp_scr_att = (char*)calloc(SCR_COL * SCR_ROW, sizeof(char));//��Ļ�ַ�����
	gp_top_layer = (LAYER_NODE*)malloc(sizeof(LAYER_NODE));//��̬����gp_top_layer���
	gp_top_layer->LayerNo = 0;      //�������ڵĲ��Ϊ0
	gp_top_layer->rcArea.Left = 0;  //�������ڵ�����Ϊ������Ļ����
	gp_top_layer->rcArea.Top = 0;   //������������λ�ó�ʼ��
	gp_top_layer->rcArea.Right = SCR_COL - 1;
	gp_top_layer->rcArea.Bottom = SCR_ROW - 1;
	gp_top_layer->pContent = NULL;  //�������������ַ���Ԫԭ��Ϣ�洢��������ʼֵΪNULL
	gp_top_layer->pScrAtt = gp_scr_att; //gp_scr_attָ�򵯳����������ַ���Ԫԭ����ֵ�洢��������Ϣ
	gp_top_layer->next = NULL; //ָ�����һ�㴰����ϢΪNULL
	
	ShowMenu();   //��ʾ���˵���
	ShowState();   //��ʾ״̬��
	return;
}

/**
*�������ƣ�ClearScreen
*�������ܣ������Ļ��Ϣ
*�����������
*�����������
*����ֵ����
*����˵����
*/
void ClearScreen(void)
{
	CONSOLE_SCREEN_BUFFER_INFO bInfo;
	COORD home = { 0,0 };
	unsigned long size;
	GetConsoleScreenBufferInfo(gh_std_out, &bInfo);   //ȡ��Ļ��������Ϣ
	size = bInfo.dwSize.X * bInfo.dwSize.Y;   //������Ļ�������ַ���Ԫ��
	//����Ļ���������е�Ԫ���ַ���������Ϊ��ǰ��Ļ�������ַ�����
	FillConsoleOutputAttribute(gh_std_out, bInfo.wAttributes, size, home, &ul);
	//����Ļ���������е�Ԫ���Ϊ�ո��ַ�
	FillConsoleOutputCharacter(gh_std_out, ' ', size, home, &ul);
	return;
}

/**
*�������ƣ�ShowMenu
*�������ܣ�����Ļ����ʾ���˵��������������������˵��ϵ�һ������ѡ�б��
*�����������
*�����������
*����ֵ����
*����˵����
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
	GetConsoleScreenBufferInfo(gh_std_out, &bInfo); //��ȡ��Ļ������Ϣ������binfo��
	size.X = bInfo.dwSize.X;
	size.Y = 1;
	SetConsoleCursorPosition(gh_std_out, pos);  //���ù�굽��0��0��
	for (i = 0; i < 5; i++)  //�ڴ��ڵ�һ�е�һ��������˵���
	{
		printf("%s      ", ga_main_menu[i]);
	}
	GetConsoleCursorInfo(gh_std_out, &lpCur);
	lpCur.bVisible = FALSE;       //���ع��
	SetConsoleCursorInfo(gh_std_out, &lpCur); 

	//���붯̬�洢����Ϊ��Ų˵�����Ļ���ַ���Ϣ�Ļ�����
	gp_buff_menubar_info = (CHAR_INFO*)malloc(size.X * size.Y * sizeof(CHAR_INFO));
	SMALL_RECT rcMenu = { 0,0,size.X - 1,0 };  //���û�������Χ
	//�����ڵ�һ�е����ݶ��뵽��Ų˵�������Ļ���ַ���Ϣ�Ļ�������
	ReadConsoleOutput(gh_std_out, gp_buff_menubar_info, size, pos, &rcMenu);
	//����һ����Ӣ����ĸ����Ϊ��ɫ�������ַ���Ԫ����Ϊ�׵׺���
	for (i = 0; i < size.X; i++)
	{
		(gp_buff_menubar_info + i)->Attributes = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;
		ch = (char)((gp_buff_menubar_info + i)->Char.AsciiChar);
		if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
		{
			(gp_buff_menubar_info + i)->Attributes |= FOREGROUND_RED;
		}
	}
	//�޸ĺ�Ĳ˵����ַ���Ϣ��д�����ڵĵ�һ��
	WriteConsoleOutput(gh_std_out, gp_buff_menubar_info, size, pos, &rcMenu);

	COORD endPos = { 0,1 };
	SetConsoleCursorPosition(gh_std_out, endPos); //�����λ�������ڵڶ��е�һ��

	//���˵�������Ϊ�������������Ϊ�˵���ţ���������Ϊ0����ť�ͣ�
	i = 0;
	do
	{
		PosB = PosA + strlen(ga_main_menu[i]);  /*��λ��i+1�Ų˵������ֹλ��*/
		for (j = PosA; j < PosB; j++)
		{
			gp_scr_att[j] |= (i + 1) << 2; /*���ò˵��������ַ���Ԫ������ֵ*/
		}
		PosA = PosB + 4;
		i++;
	} while (i < 5);

	TagMainMenu(gi_sel_menu);   //��ѡ�����˵�������ǣ�gi_sel_menu��ֵΪ1
	return;
}

/**
*�������ƣ�PopMenu
*�������ܣ�����ָ�����˵����Ӧ���Ӳ˵�
*���������numָ�������˵���
*�����������
*����ֵ����
*����˵����
*/
void PopMenu(int num)
{
	LABEL_BUNDLE labels;
	HOT_AREA areas;
	SMALL_RECT rcPop, * aArea;
	COORD pos, * aLoc;
	WORD att;
	int i, j, loc = 0;
	char* aSort, * aTag;/*aSort:������� aTag:�������*/
	char* pch;

	if (num != gi_sel_menu)    /*���ָ�����˵�������ѡ�в˵�*/
	{
		if (gp_top_layer->LayerNo != 0) /*�����ǰ�����Ӳ˵�����*/
		{
			PopOff();/*�رոõ�������*/
			gi_sel_sub_menu = 0;/*����ѡ�е��Ӳ˵������Ϊ0*/
		}
	}
	else if (gp_top_layer->LayerNo != 0) /*���ѵ������Ӳ˵����򷵻�*/
	{
		return;
	}

	gi_sel_menu = num;               /*��ѡ�����˵�����Ϊָ�������˵���*/
	TagMainMenu(gi_sel_menu);        /*��ѡ�е����˵����������*/
	LocSubMenu(gi_sel_menu, &rcPop); /*���㵯���Ӳ˵�������λ��, �����rcPop��*/
	/*������Ӳ˵��еĵ�һ�����Ӳ˵��ַ��������е�λ��(�±�)*/
	for (i = 1; i < gi_sel_menu; i++)
	{
		loc += ga_sub_menu_count[i - 1];
	}
	/*�������Ӳ˵������������ǩ���ṹ����*/
	labels.ppLabel = ga_sub_menu + loc;   /*��ǩ����һ����ǩ�ַ����ĵ�ַ*/
	labels.num = ga_sub_menu_count[gi_sel_menu - 1]; /*��ǩ���б�ǩ�ַ����ĸ���*/
	/*����һ���������飬���ÿ����ǩ�ַ������λ�õ�����ȼ���:COORD aLoc[labels.num];*/
	aLoc = (COORD*)malloc(labels.num * sizeof(COORD));
	for (i = 0; i < labels.num; i++) /*ȷ����ǩ�ַ��������λ�ã����������������*/
	{
		aLoc[i].X = rcPop.Left + 2;
		aLoc[i].Y = rcPop.Top + i + 1;
	}
	labels.pLoc = aLoc; /*ʹ��ǩ���ṹ����labels�ĳ�ԱpLocָ�������������Ԫ��*/
	/*�����Ӳ˵�������Ϣ*/
	areas.num = labels.num;       /*�����ĸ��������ڱ�ǩ�ĸ��������Ӳ˵�������*/

	aArea = (SMALL_RECT*)malloc(areas.num * sizeof(SMALL_RECT));/*������������������λ�õȼ���:SMALL_RECT aArea[areas.num];*/
	aSort = (char*)malloc(areas.num * sizeof(char));    /*��������������������Ӧ���ȼ���:char aSort[areas.num];*/
	aTag = (char*)malloc(areas.num * sizeof(char));    /*����������ÿ�������ı��:char aTag[areas.num];*/
	/*������λ*/
	for (i = 0; i < areas.num; i++)
	{/*��������Χ*/
		aArea[i].Left = rcPop.Left + 2;
		aArea[i].Top = rcPop.Top + i + 1;
		aArea[i].Right = rcPop.Right - 2;
		aArea[i].Bottom = aArea[i].Top;
		aSort[i] = 0;     /*�������Ϊ0(��ť��)*/
		aTag[i] = i + 1; /*������˳����1,2,3,4...*/
	}
	areas.pArea = aArea;  /*ʹ�����ṹ����areas�ĳ�ԱpAreaָ������λ��������Ԫ��*/
	areas.pSort = aSort;  /*ʹ�����ṹ����areas�ĳ�ԱpSortָ���������������Ԫ��*/
	areas.pTag = aTag;    /*ʹ�����ṹ����areas�ĳ�ԱpTagָ���������������Ԫ��*/
	att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*�Ӳ˵��ַ�����:�׵׺���*/
	PopUp(&rcPop, att, &labels, &areas);
	DrawBox(&rcPop);       /*���������ڻ��߿�*/
	pos.X = rcPop.Left + 2;
	for (pos.Y = rcPop.Top + 1; pos.Y < rcPop.Bottom; pos.Y++)
	{ /*��ѭ�������ڿմ��Ӳ���λ�û����γɷָ�����ȡ���˲˵������������*/
		pch = ga_sub_menu[loc + pos.Y - rcPop.Top - 1];
		if (strlen(pch) == 0) /*����Ϊ0������Ϊ�մ�*/
		{   /*���Ȼ�����*/
			FillConsoleOutputCharacter(gh_std_out, '-', rcPop.Right - rcPop.Left - 3, pos, &ul);
			for (j = rcPop.Left + 2; j < rcPop.Right - 1; j++)
			{   /*ȡ���������ַ���Ԫ����������*/
				gp_scr_att[pos.Y * SCR_COL + j] &= 3; /*��λ��Ľ�������˵���λ*/
			}
		}
	}
	/*���Ӳ˵���Ĺ��ܼ���Ϊ�׵׺���*/
	pos.X = rcPop.Left + 3;
	att = FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;
	for (pos.Y = rcPop.Top + 1; pos.Y < rcPop.Bottom; pos.Y++)
	{
		if (strlen(ga_sub_menu[loc + pos.Y - rcPop.Top - 1]) == 0)
		{
			continue;  /*�����մ�*/
		}
		FillConsoleOutputAttribute(gh_std_out, att, 1, pos, &ul);
	}
	return;
}

/**
*�������ƣ�CloseSys
*�������ܣ��ر�ϵͳ,�ͷŶ�̬�洢��
*�����������
*�����������
*����ֵ����
*����˵����
*/
BOOL CloseSys()
{
	HOME_NODE* phome1 = gp_head, *phome2;
	CAR_NODE* pcar1, *pcar2;
	CHARGE_NODE* pcharge1, * pcharge2;
	/*�ͷ�ʮ�ֽ�������Ķ�̬�洢��*/
	while (phome1 != NULL)
	{
		phome2 = phome1->next;
		pcar1 = phome1->snext;
		while (pcar1 != NULL)     /*�ͷų�����Ϣ֧���Ķ�̬�洢��*/
		{
			pcar2 = pcar1->next;
			pcharge1 = pcar1->cnext;
			while (pcharge1 != NULL) /*�ͷ��⳵����֧���Ķ�̬�洢��*/
			{
				pcharge2 = pcharge1->next;
				free(pcharge1);
				pcharge1= pcharge2;
			}
			free(pcar1);
			pcar1 = pcar2;
		}
		free(phome1);            /*�ͷź����������Ķ�̬�洢��*/
		phome1 = phome2;
	}

	ClearScreen();     /*�����Ļ��Ϣ*/
	/*�ͷŴ�Ų˵�����״̬������Ϣ��̬�洢��*/
	free(gp_buff_menubar_info);
	free(gp_buff_stateBar_info);
	/*�رձ�׼���������豸���*/
	CloseHandle(gh_std_out);
	CloseHandle(gh_std_in);
	//�����ڱ���������Ϊ����
	SetConsoleTitle("���н���");
	return TRUE;
}

/**
*�������ƣ�RunSys
*�������ܣ�����ϵͳ,��ϵͳ�����������û���ѡ��Ĺ��ܺ���
*���������phead����ͷָ���ַ
*�����������
*����ֵ����
*����˵������
*/
void RunSys(HOME_NODE** phead)
{
	INPUT_RECORD inRec;
	DWORD res;
	COORD pos = { 0, 0 };
	BOOL bRet = TRUE;
	int i, loc, num;
	int cNo, cAtt;      /*cNo:�ַ���Ԫ���, cAtt:�ַ���Ԫ����*/
	char vkc, asc;      /*vkc:���������, asc:�ַ���ASCII��ֵ*/

	SetMainHotArea();   /*�����˵�����������*/

	while (bRet)
	{  /*�ӿ���̨���뻺�����ж�һ����¼*/
		ReadConsoleInput(gh_std_in, &inRec, 1, &res);
		if (inRec.EventType == MOUSE_EVENT) /*�����¼������¼�����*/
		{
			pos = inRec.Event.MouseEvent.dwMousePosition;    /*��ȡ�������λ��*/
			cNo = gp_scr_att[pos.Y * SCR_COL + pos.X] & 3;   /*ȡ��λ�õĲ��*/
			cAtt = gp_scr_att[pos.Y * SCR_COL + pos.X] >> 2; /*ȡ���ַ���Ԫ����*/
			if (cNo == 0)  /*���Ϊ0,������λ��δ�������Ӳ˵�����*/
			{
				/*cAtt>0������λ�ô�������,cAtt!=gi_sel_menu������λ�õ����˵���δ��ѡ��,gp_top_layer->LayerNo>0������ǰ���Ӳ˵�����*/
				if (cAtt > 0 && cAtt != gi_sel_menu && gp_top_layer->LayerNo > 0)
				{
					PopOff();            /*�رյ������Ӳ˵�*/
					gi_sel_sub_menu = 0; /*��ѡ���Ӳ˵���������Ϊ0*/
				}
				else  if (cAtt > 0)
					TagMainMenu(cAtt); /*������������˵�������ѡ�б��*/
			}
			else if (cAtt > 0) /*�������λ��Ϊ�����Ӳ˵��Ĳ˵����ַ���Ԫ*/
				TagSubMenu(cAtt); /*�ڸ��Ӳ˵�������ѡ�б��*/
			if (inRec.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) /*�������������һ��*/
			{
				if (cNo == 0) /*���Ϊ0��������λ��δ�������Ӳ˵�����*/
				{
					if (cAtt > 0) /*�����λ�ô�������*/
						PopMenu(cAtt);   /*��������������˵����Ӧ���Ӳ˵�*/
					else if (gp_top_layer->LayerNo > 0) /*�����λ�ò��������˵����ַ���Ԫ,�����Ӳ˵�����*/
					{
						PopOff();             /*�رյ������Ӳ˵�*/
						gi_sel_sub_menu = 0;  /*��ѡ���Ӳ˵���������Ϊ0*/
					}
				}
				else /*��Ų�Ϊ0,������λ�ñ������Ӳ˵�����*/
				{
					if (cAtt > 0) /*�����λ�ô�������*/
					{
						PopOff();  /*�رյ������Ӳ˵�*/
						gi_sel_sub_menu = 0;  /*��ѡ���Ӳ˵���������Ϊ0*/
						bRet = ExeFunction(gi_sel_menu, cAtt); /*ִ�ж�Ӧ���ܺ���:gi_sel_menu���˵����,cAtt�Ӳ˵����*/
					}
				}
			}
			else if (inRec.Event.MouseEvent.dwButtonState == RIGHTMOST_BUTTON_PRESSED) /*�����������Ҽ�*/
			{
				if (cNo == 0) /*���Ϊ0��������λ��δ�������Ӳ˵�����*/
				{
					PopOff();            /*�رյ������Ӳ˵�*/
					gi_sel_sub_menu = 0; /*��ѡ���Ӳ˵���������Ϊ0*/
				}
			}
		}
		else if (inRec.EventType == KEY_EVENT && inRec.Event.KeyEvent.bKeyDown) /*�����¼�ɰ��������Ҽ�������*/
		{
			vkc = inRec.Event.KeyEvent.wVirtualKeyCode;  /*��ȡ�������������*/
			asc = inRec.Event.KeyEvent.uChar.AsciiChar;  /*��ȡ������ASC��*/
			/*ϵͳ������ݼ�F1���Ĵ���*/
			if (vkc == 112) /*�������F1��*/
			{
				if (gp_top_layer->LayerNo != 0) /*�����ǰ���Ӳ˵�����*/
				{
					PopOff();                   /*�رյ������Ӳ˵�*/
					gi_sel_sub_menu = 0;        /*��ѡ���Ӳ˵���������Ϊ0*/
				}
				bRet = ExeFunction(5, 1);       /*���а������⹦�ܺ���*/
			}
			else if (inRec.Event.KeyEvent.dwControlKeyState & (LEFT_ALT_PRESSED | RIGHT_ALT_PRESSED))
			{ /*������������Alt��*/
				switch (vkc)  /*�ж���ϼ�Alt+��ĸ*/
				{
				case 88:  /*Alt+X �˳�*/
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
			else if (asc == 0) /*�������Ƽ��Ĵ���*/
			{
				if (gp_top_layer->LayerNo == 0) /*���δ�����Ӳ˵�*/
				{
					switch (vkc) /*�������(���ҡ���),����Ӧ�������Ƽ�*/
					{
					case 37:  /*���*/
						gi_sel_menu--;
						if (gi_sel_menu == 0)  gi_sel_menu = 5;
						TagMainMenu(gi_sel_menu);
						break;
					case 39: /*�Ҽ�*/
						gi_sel_menu++;
						if (gi_sel_menu == 6)  gi_sel_menu = 1;
						TagMainMenu(gi_sel_menu);
						break;
					case 40: /*�¼�*/
						PopMenu(gi_sel_menu);
						TagSubMenu(1);
						break;
					}
				}
				else  /*�ѵ����Ӳ˵�ʱ*/
				{
					/*������Ӳ˵��еĵ�һ�����Ӳ˵��ַ��������е�λ��(�±�)*/
					for (loc = 0, i = 1; i < gi_sel_menu; i++)
						loc += ga_sub_menu_count[i - 1];
					switch (vkc) /*�����(���ҡ��ϡ���)�Ĵ���*/
					{
					case 37: /*���*/
						gi_sel_menu--;
						if (gi_sel_menu < 1)   gi_sel_menu = 5;
						TagMainMenu(gi_sel_menu);
						PopOff();
						PopMenu(gi_sel_menu);
						TagSubMenu(1);
						break;
					case 38: /*�ϼ�*/
						num = gi_sel_sub_menu - 1;
						if (num < 1)    num = ga_sub_menu_count[gi_sel_menu - 1];
						if (strlen(ga_sub_menu[loc + num - 1]) == 0)    num--;
						TagSubMenu(num);
						break;
					case 39:  /*�Ҽ�*/
						gi_sel_menu++;
						if (gi_sel_menu > 5)     gi_sel_menu = 1;
						TagMainMenu(gi_sel_menu);
						PopOff();
						PopMenu(gi_sel_menu);
						TagSubMenu(1);
						break;
					case 40: /*�¼�*/
						num = gi_sel_sub_menu + 1;
						if (num > ga_sub_menu_count[gi_sel_menu - 1])  num = 1;
						if (strlen(ga_sub_menu[loc + num - 1]) == 0)     num++;
						TagSubMenu(num);
						break;
					}
				}
			}
			else if ((asc - vkc == 0) || (asc - vkc == 32)) {  /*�������ּ�������ĸ������ͨ��*/
				if (gp_top_layer->LayerNo == 0)  /*���δ�����Ӳ˵�*/
				{
					switch (vkc)
					{
					case 70: /*f��F*/
						PopMenu(1);
						break;
					case 77: /*M��m*/
						PopMenu(2);
						break;
					case 81: /*Q��q*/
						PopMenu(3);
						break;
					case 83: /*s��S*/
						PopMenu(4);
						break;
					case 72: /*h��H*/
						PopMenu(5);
						break;
					case 13: /*�س�*/
						PopMenu(gi_sel_menu);
						TagSubMenu(1);
						break;
					}
				}
				else /*�ѵ����Ӳ˵�ʱ�ļ������봦��*/
				{
					if (vkc == 27) /*�������ESC��*/
					{
						PopOff();
						gi_sel_sub_menu = 0;
					}
					else if (vkc == 13) /*������»س���*/
					{
						num = gi_sel_sub_menu;
						PopOff();
						gi_sel_sub_menu = 0;
						bRet = ExeFunction(gi_sel_menu, num);
					}
					else /*������ͨ���Ĵ���*/
					{
						/*������Ӳ˵��еĵ�һ�����Ӳ˵��ַ��������е�λ��(�±�)*/
						for (loc = 0, i = 1; i < gi_sel_menu; i++)
							loc += ga_sub_menu_count[i - 1];
						/*�����뵱ǰ�Ӳ˵���ÿһ��Ĵ����ַ����бȽ�*/
						for (i = loc; i < loc + ga_sub_menu_count[gi_sel_menu - 1]; i++)
						{
							if (strlen(ga_sub_menu[i]) > 0 && vkc == ga_sub_menu[i][1])
							{ /*���ƥ��ɹ�*/
								PopOff();
								gi_sel_sub_menu = 0;
								bRet = ExeFunction(gi_sel_menu, i - loc + 1); /*ִ������ĸ���Ӧ�ĺ���*/
							}
						}
					}
				}
			}
		}
	}
}

/**
*�������ƣ�PopUp
*�������ܣ���ָ�����򵯳�������Ϣ����������������������λ����Ϣ��ջ
*���������pRc ��������λ�����ݴ�ŵĵ�ַ��att�������������ַ�����
*          pLable ���������б�ǩ����Ϣ��ŵĵ�ַ
*		   pHotArea ����������������Ϣ��ŵĵ�ַ
*�����������
*����ֵ����
*����˵����
*/
void PopUp(SMALL_RECT* pRc, WORD att, LABEL_BUNDLE* pLabel, HOT_AREA* pHotArea)
{
	LAYER_NODE* nextLayer;  /*����������Ϣ�ṹ����ָ��*/
	COORD size;
	COORD pos = { 0, 0 };
	char* pCh;
	int i, j, row;
	/*������������λ���ַ���Ԫ��Ϣ��ջ*/
	size.X = pRc->Right - pRc->Left + 1;    /*�������ڵĿ��*/
	size.Y = pRc->Bottom - pRc->Top + 1;    /*�������ڵĸ߶�*/

	/*�����ŵ������������Ϣ�Ķ�̬�洢��*/
	nextLayer = (LAYER_NODE*)malloc(sizeof(LAYER_NODE));
	nextLayer->next = gp_top_layer;
	nextLayer->LayerNo = gp_top_layer->LayerNo + 1;
	nextLayer->rcArea = *pRc;
	nextLayer->pContent = (CHAR_INFO*)malloc(size.X * size.Y * sizeof(CHAR_INFO));
	nextLayer->pScrAtt = (char*)malloc(size.X * size.Y * sizeof(char));
	pCh = nextLayer->pScrAtt;

	/*���������ڸ���������ַ���Ϣ���棬�����ڹرյ�������ʱ�ָ�ԭ��*/
	ReadConsoleOutput(gh_std_out, nextLayer->pContent, size, pos, pRc);
	for (i = pRc->Top; i <= pRc->Bottom; i++)
	{   /*�˶���ѭ�����������ַ���Ԫ��ԭ������ֵ���붯̬�洢���������Ժ�ָ�*/
		for (j = pRc->Left; j <= pRc->Right; j++)
		{
			*pCh = gp_scr_att[i * SCR_COL + j];  /*��pChָ������ݷ���gp_scr_attָ����ַ���Ԫ����ֵ�Ļ�����*/
			pCh++;
		}
	}
	gp_top_layer = nextLayer;  /*��ɵ������������Ϣ��ջ����,�Խ�gp_top_layer����Ϊ����������Ϣ����ͷ*/

	/*���õ������������ַ���������*/
	pos.X = pRc->Left;
	pos.Y = pRc->Top;
	for (i = pRc->Top; i <= pRc->Bottom; i++)
	{
		FillConsoleOutputAttribute(gh_std_out, att, size.X, pos, &ul);
		pos.Y++;
	}

	/*����ǩ���еı�ǩ�ַ������趨��λ�����*/
	for (i = 0; i < pLabel->num; i++)
	{
		pCh = pLabel->ppLabel[i];
		if (strlen(pCh) != 0)
			WriteConsoleOutputCharacter(gh_std_out, pCh, strlen(pCh), pLabel->pLoc[i], &ul);
	}

	/*���õ������������ַ���Ԫ��������*/
	for (i = pRc->Top; i <= pRc->Bottom; i++)
	{   /*�˶���ѭ�������ַ���Ԫ�Ĳ��*/
		for (j = pRc->Left; j <= pRc->Right; j++)
			gp_scr_att[i * SCR_COL + j] = gp_top_layer->LayerNo;
	}

	/*�˶���ѭ�����������������ַ���Ԫ���������ͺ��������*/
	for (i = 0; i < pHotArea->num; i++)
	{
		row = pHotArea->pArea[i].Top;
		for (j = pHotArea->pArea[i].Left; j <= pHotArea->pArea[i].Right; j++)
			gp_scr_att[row * SCR_COL + j] |= (pHotArea->pSort[i] << 6) | (pHotArea->pTag[i] << 2);
	}
	return;
}

/**
*�������ƣ�PopOff
*�������ܣ��رյ�������,�ָ���������ԭ��ۺ��ַ���Ԫԭ����
*�����������
*�����������
*����ֵ����
*����˵����
*/
void PopOff(void)
{
	LAYER_NODE* nextLayer;
	COORD size;
	COORD pos = { 0, 0 };
	char* pCh;
	int i, j;
	/*ջ�״�ŵ���������Ļ��Ϣ�����ùر�*/
	if ((gp_top_layer->next == NULL) || (gp_top_layer->pContent == NULL))
		return;
	nextLayer = gp_top_layer->next;

	/*�ָ�������������ԭ���*/
	size.X = gp_top_layer->rcArea.Right - gp_top_layer->rcArea.Left + 1;
	size.Y = gp_top_layer->rcArea.Bottom - gp_top_layer->rcArea.Top + 1;
	WriteConsoleOutput(gh_std_out, gp_top_layer->pContent, size, pos, &(gp_top_layer->rcArea));

	/*�ָ��ַ���Ԫԭ����*/
	pCh = gp_top_layer->pScrAtt;
	for (i = gp_top_layer->rcArea.Top; i <= gp_top_layer->rcArea.Bottom; i++)
	{
		for (j = gp_top_layer->rcArea.Left; j <= gp_top_layer->rcArea.Right; j++)
		{
			gp_scr_att[i * SCR_COL + j] = *pCh;
			pCh++;
		}
	}
	/*�ͷŶ�̬�洢��*/
	free(gp_top_layer->pContent);
	free(gp_top_layer->pScrAtt);
	free(gp_top_layer);
	gp_top_layer = nextLayer;
	gi_sel_sub_menu = 0;
	return;
}

/**
*�������ƣ�DrawBox
*�������ܣ����������Ӳ˵����ڻ��Ʊ߿�.
*�����������
*�����������
*����ֵ����
*����˵����
*/
void DrawBox(SMALL_RECT* pRc)
{
	char chBox[] = { '#','-','|' };  /*�����õ��ַ�*/
	COORD pos = { pRc->Left, pRc->Top };  /*��λ����������Ͻ�*/
	WriteConsoleOutputCharacter(gh_std_out, &chBox[0], 1, pos, &ul);  /*���߿����Ͻ�#*/
	/*��ѭ�����ϱ߿����*/
	for (pos.X = pRc->Left + 1; pos.X < pRc->Right; pos.X++)
		WriteConsoleOutputCharacter(gh_std_out, &chBox[1], 1, pos, &ul);
	pos.X = pRc->Right;

	WriteConsoleOutputCharacter(gh_std_out, &chBox[0], 1, pos, &ul);   /*���߿����Ͻ�~*/
	/*��ѭ�����߿�����ߺ��ұ���*/
	for (pos.Y = pRc->Top + 1; pos.Y < pRc->Bottom; pos.Y++)
	{
		pos.X = pRc->Left;
		WriteConsoleOutputCharacter(gh_std_out, &chBox[2], 1, pos, &ul);
		pos.X = pRc->Right;
		WriteConsoleOutputCharacter(gh_std_out, &chBox[2], 1, pos, &ul);
	}
	pos.X = pRc->Left;
	pos.Y = pRc->Bottom;

	WriteConsoleOutputCharacter(gh_std_out, &chBox[0], 1, pos, &ul);   /*���߿����½�&*/
	/*���±߿����*/
	for (pos.X = pRc->Left + 1; pos.X < pRc->Right; pos.X++)
		WriteConsoleOutputCharacter(gh_std_out, &chBox[1], 1, pos, &ul);
	pos.X = pRc->Right;
	WriteConsoleOutputCharacter(gh_std_out, &chBox[0], 1, pos, &ul);   /*���߿����½�%*/
	return;
}

/**
*�������ƣ�LocSubMenu
*�������ܣ����㵯���Ӳ˵�(���Ϊnum)�������ϽǺ����½ǵ�λ��.��Ϣ�洢������ΪSMALL_RECT�ṹ�ı���rc��
*���������numΪѡ�е��Ӳ˵����
*���������rc�������λ����Ϣ�ĵ�ַ,�Ǹ����νṹ
*����ֵ����
*����˵����
*/
void LocSubMenu(int num, SMALL_RECT* rc)
{
	int i, len, loc = 0;
	/*������ϱ߶��ڵ�2�У��к�Ϊ1*/
	rc->Top = 1;
	rc->Left = 1;
	/*����������߽�λ��, ͬʱ�����һ���Ӳ˵������Ӳ˵��ַ��������е�λ��*/
	for (i = 1; i < num; i++)
	{
		rc->Left += strlen(ga_main_menu[i - 1]) + 4;
		loc += ga_sub_menu_count[i - 1];
	}
	rc->Right = strlen(ga_sub_menu[loc]);    /*��ʱ��ŵ�һ���Ӳ˵����ַ�������*/
	/*������Ӳ˵��ַ��������䳤�ȴ����rc->Right*/
	for (i = 1; i < ga_sub_menu_count[num - 1]; i++)
	{
		len = strlen(ga_sub_menu[loc + i]);
		if (rc->Right < len)
		{
			rc->Right = len;
		}
	}
	rc->Right += rc->Left + 3;  /*����������ұ߽�*/
	rc->Bottom = rc->Top + ga_sub_menu_count[num - 1] + 1;/*���������±ߵ��к�*/
	/*�ұ߽�Խ��Ĵ���*/
	if (rc->Right >= SCR_COL)
	{
		len = rc->Right - SCR_COL + 1;
		rc->Left -= len;
		rc->Right = SCR_COL - 1;
	}
	return;
}

/**
*�������ƣ�ShowState
*�������ܣ���ʾ״̬��
*�����������
*�����������
*����ֵ����
*����˵����״̬���ַ�����Ϊ�׵׺��֣���ʼ״̬��״̬��Ϣ
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
*�������ƣ�TagMainMenu
*�������ܣ���ָ�����˵�����ѡ�б��
*���������numѡ�е����˵����
*�����������
*����ֵ����
*����˵����
*/
void TagMainMenu(int num)
{
	CONSOLE_SCREEN_BUFFER_INFO bInfo;
	COORD size;
	COORD pos = { 0,0 };
	int PosA = 2, PosB;
	char ch;
	int i;
	if (num == 0)   //numΪ0 ʱ������ȥ�����˵�ѡ���б��
	{
		PosA = 0;
		PosB = 0;
	}
	else  //���򣬶�λѡ�����˵������ֹλ�ã�PosA����ʼλ�ã�PosB�ǽ�ֹλ��
	{
		for (i = 1; i < num; i++)
		{
			PosA += strlen(ga_main_menu[i - 1]) + 4;   //�����4��ʲô��˼��
		}
		PosB = PosA += strlen(ga_main_menu[num - 1]);
	}
	GetConsoleScreenBufferInfo(gh_std_out, &bInfo);  //��ȡ��Ļ��������Ϣ������binfo��
	size.X = bInfo.dwSize.X;
	size.Y = 1;

	//ȥ��ѡ�еĲ˵���ǰ��Ĳ˵���ѡ�б��
	for (i = 0; i < PosA; i++)
	{
		(gp_buff_menubar_info + i)->Attributes = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;
		ch = (char)((gp_buff_menubar_info + i)->Char.AsciiChar);
		if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
		{
			(gp_buff_menubar_info + i)->Attributes |= FOREGROUND_RED;
		}
	}

	//��ѡ�еĲ˵���������ǣ��׵׺���
	for (i = PosA; i < PosB; i++)
	{
		(gp_buff_menubar_info + i)->Attributes = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
	}

	//ȥ��ѡ�в˵������Ĳ˵���ѡ�б��
	for (i = PosB; i < bInfo.dwSize.X; i++)
	{
		(gp_buff_menubar_info + i)->Attributes = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;
		ch = (char)((gp_buff_menubar_info + i)->Char.AsciiChar);
		if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
		{
			(gp_buff_menubar_info + i)->Attributes |= FOREGROUND_RED;
		}
	}
	//�����ñ�ǵĲ˵�����Ϣд�Ĵ��ڵ�һ��
	SMALL_RECT rcMenu = { 0,0,size.X - 1,0 };
	WriteConsoleOutput(gh_std_out, gp_buff_menubar_info, size, pos, &rcMenu);
	return;
}

/**
*�������ƣ�TagSubMenu
*�������ܣ���ָ���Ӳ˵�������ѡ�б��
*���������numΪѡ�е��Ӳ˵����
*�����������
*����ֵ����
*����˵����
*/
void TagSubMenu(int num)
{
	SMALL_RECT rcPop;
	COORD pos;
	WORD  att;
	int   width;
	LocSubMenu(gi_sel_menu, &rcPop);/*���㵯���Ӳ˵�������λ��, �����rcPop��*/
	/*����Ӳ˵����Խ�磬������Ӳ˵��ѱ�ѡ�У��򷵻�*/
	if ((num < 1) || (num == gi_sel_sub_menu) || (num > rcPop.Bottom - rcPop.Top - 1))
		return;
	pos.X = rcPop.Left + 2;
	width = rcPop.Right - rcPop.Left - 3;
	/*����ȡ��ԭѡ���Ӳ˵����ϵı��*/
	if (gi_sel_sub_menu != 0)
	{
		pos.Y = rcPop.Top + gi_sel_sub_menu;
		att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*�׵׺���*/
		FillConsoleOutputAttribute(gh_std_out, att, width, pos, &ul);
		pos.X += 1;
		att |= FOREGROUND_RED | FOREGROUND_INTENSITY;    /*�׵�������*/
		FillConsoleOutputAttribute(gh_std_out, att, 1, pos, &ul);
	}

	/*���Ӳ˵�������ѡ�б��(�����������)*/
	pos.X = rcPop.Left + 2;
	pos.Y = rcPop.Top + num;
	att = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_INTENSITY;/*�����������*/
	FillConsoleOutputAttribute(gh_std_out, att, width, pos, &ul);
	gi_sel_sub_menu = num;  /*�޸�ѡ���Ӳ˵����*/
	return;
}

/**
 * ��������: DealInput
 * ��������: �ڵ�������������������, �ȴ�����Ӧ�û�����.
 * �������: pHotArea
 *           piHot ����������ŵĴ�ŵ�ַ, ��ָ�򽹵�������ŵ�ָ��
 * �������: piHot ����굥�������س���ո�ʱ���ص�ǰ�������
 * �� �� ֵ:
 *
 * ����˵��:
 */
int DealInput(HOT_AREA* pHotArea, int* piHot)
{
	INPUT_RECORD inRec;
	CONSOLE_CURSOR_INFO lpCur;
	DWORD res;
	COORD pos = { 0, 0 };
	int num, arrow, iRet = 0;
	int cNo, cTag, cSort;/*cNo:���, cTag:�������, cSort: ��������*/
	char vkc, asc;       /*vkc:���������, asc:�ַ���ASCII��ֵ*/

	SetHotPoint(pHotArea, *piHot);
	while (TRUE)
	{    /*ѭ��*/
		ReadConsoleInput(gh_std_in, &inRec, 1, &res);
		if ((inRec.EventType == MOUSE_EVENT) &&
			(inRec.Event.MouseEvent.dwButtonState
				== FROM_LEFT_1ST_BUTTON_PRESSED))
		{
			pos = inRec.Event.MouseEvent.dwMousePosition;   //��ȡ���λ��
			cNo = gp_scr_att[pos.Y * SCR_COL + pos.X] & 3;
			cTag = (gp_scr_att[pos.Y * SCR_COL + pos.X] >> 2) & 15;
			cSort = (gp_scr_att[pos.Y * SCR_COL + pos.X] >> 6) & 3;    //��ȡ�ô�ҳ������

			if ((cNo == gp_top_layer->LayerNo) && cTag > 0)   //����õ���������
			{
				*piHot = cTag;
				SetHotPoint(pHotArea, *piHot);
				if (cSort == 0)
				{
					iRet = 13;
					break;
				}
			}
			else if (cTag == 0)  //����������
			{
				GetConsoleCursorInfo(gh_std_out, &lpCur);
				lpCur.bVisible = FALSE;
				SetConsoleCursorInfo(gh_std_out, &lpCur);   //���ع��
			}
		}

		//����Ǽ�������
		else if (inRec.EventType == KEY_EVENT && inRec.Event.KeyEvent.bKeyDown)   
		{
			vkc = inRec.Event.KeyEvent.wVirtualKeyCode;
			asc = inRec.Event.KeyEvent.uChar.AsciiChar;;
			if (asc == 0)
			{
				arrow = 0;
				switch (vkc)
				{  /*�����(���ϡ��ҡ���)�Ĵ���*/
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
			{  /*ESC��*/
				iRet = 27;
				break;
			}
			else if (vkc == 13 || vkc == 32)
			{  /*�س�����ո��ʾ���µ�ǰ��ť*/
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

	att1 = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;  /*�ڵװ���*/
	att2 = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*�׵׺���*/
	for (i = 0; i < pHotArea->num; i++)
	{  /*����ť��������Ϊ�׵׺���*/
		pos.X = pHotArea->pArea[i].Left;
		pos.Y = pHotArea->pArea[i].Top;
		width = pHotArea->pArea[i].Right - pHotArea->pArea[i].Left + 1;
		if (pHotArea->pSort[i] == 0)
		{  /*�����ǰ�ť��*/
			FillConsoleOutputAttribute(gh_std_out, att2, width, pos, &ul);
		}
	}

	pos.X = pHotArea->pArea[iHot - 1].Left;
	pos.Y = pHotArea->pArea[iHot - 1].Top;
	width = pHotArea->pArea[iHot - 1].Right - pHotArea->pArea[iHot - 1].Left + 1;
	if (pHotArea->pSort[iHot - 1] == 0)
	{  /*�����������ǰ�ť��*/
		FillConsoleOutputAttribute(gh_std_out, att1, width, pos, &ul);
	}
	else if (pHotArea->pSort[iHot - 1] == 1)
	{  /*�������������ı�����*/
		SetConsoleCursorPosition(gh_std_out, pos);
		GetConsoleCursorInfo(gh_std_out, &lpCur);
		lpCur.bVisible = TRUE;
		SetConsoleCursorInfo(gh_std_out, &lpCur);
	}
}

/**
*�������ƣ�ExeFunction
*�������ܣ�ִ�������˵���Ϊm���Ӳ˵���Ϊn��ȷ���Ĺ��ܺ���
*���������m���˵��ţ�s�Ӳ˵���
*�����������
*����ֵ��BOOL���ͣ�TRUE OR FALSE
*����˵��������ִ�к���ExitSysʱ���ſ��ܷ���FALSE
*/
BOOL ExeFunction(int m, int n)
{
	BOOL bRet = TRUE;
	/*����ָ�����飬����������й��ܺ�������ڵ�ַ*/
	//BOOL(*pFunction[ga_sub_menu_count[0]+ga_sub_menu_count[1]+ ga_sub_menu_count[2]+ ga_sub_menu_count[3]+ ga_sub_menu_count[4]])(void);
	BOOL(*pFunction[26])(void);  //���ﻹ���Լ���ƺ��м�������
	int i, loc;
	/*�����ܺ�����ڵ�ַ�����빦�ܺ����������˵��ź��Ӳ˵��Ŷ�Ӧ�±������Ԫ��*/

	pFunction[0] = SaveData;  //�ļ����֣�4��
	pFunction[1] = BackupData;
	pFunction[2] = RestoreData;
	pFunction[3] = CloseSys;
	                      
	pFunction[4] = MaintainHomeInfo;   //����ά������
	pFunction[5] = MaintainCarInfo;
	pFunction[6] = MaintainChargeInfo;

	pFunction[7] = QueryHomeInfo;    //���ݲ�ѯ���֣�3��
	pFunction[8] = QueryCarInfo;
	pFunction[9] = QueryChargeInfo;  

	pFunction[10] = Stat_Car_Type_Rent_Num;        //ͳ�Ʋ��֣��������
	pFunction[11] = Stat_Car_Type_Month_Fee;
	pFunction[12] = Stat_Car_Fee_and_rent_rate;
	pFunction[13] = Stat_Car_Most_Rent;
	pFunction[14] = Stat_Guest_Most_Rent;

	pFunction[15] = HelpTopic;   //�������֣�2��
	pFunction[16] = NULL;
	pFunction[17] = AboutSys;

	/*�������˵��ź��Ӳ˵��ż����Ӧ�±�*/
	for (i = 1, loc = 0; i < m; i++)
		loc += ga_sub_menu_count[i - 1];    //{5��11��4��5��3}
	loc += n - 1;
	/*�ú���ָ�������ָ��Ĺ��ܺ���*/
	if (pFunction[loc] != NULL)
		bRet = (*pFunction[loc])();
	return bRet;
}

/**
 * ��������: ChangeCarNumber
 * ��������: �Զ�ͳ�Ƶ�ǰ������Ŀ�����ݳ���������Ϣͳ�ƣ�Ȼ�����
 * �������: ��
 *      
 * �������: phaed,����ͷָ���ַ
 * �� �� ֵ: BOOL���ͣ���ΪTRUE
 *
 * ����˵��:
 */
BOOL ChangeCarNumber(HOME_NODE** phead)   //�Զ�ͳ�Ƶ�ǰ������Ŀ�����ݳ���������Ϣͳ�ƣ�Ȼ�����
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
   �������ƣ�SaveSysData
   �������ܣ�����ǰ�ڴ������ݴ�������ļ�
   ���������hd����ͷָ��ʹ�����ļ���
   �������:
   ����ֵ��BOOL����ΪTRUE
   ����˵����
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
	ChangeID(&phead);    //���ݶ�����Ϣ�ı����״̬
	
	fprintf(fout1, "%s,%s,%s,\n", "�������ͱ���", "������������", "�������");
	fprintf(fout2, "%s,%s,%s,%s,%s,%s,%s,\n", "�������", "���ƺ�", "�������ͱ���", "��������", "�ŵ���ʽ", "ÿ�����Ԫ��", "����״̬");
	fprintf(fout3, "%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,\n", "�������", "���֤��", "��������", "�ֻ�����", "���ó������", "ȡ��ʱ��", "ԤԼ����ʱ��", "Ѻ��Ԫ��", "ʵ�ʻ���ʱ��", "Ӧ�ɷ��ã�Ԫ��", "ʵ�ɷ��ã�Ԫ��");

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
	printf("��������ѱ���!\n");
	return TRUE;
}

/*
   �������ƣ�SaveData
   �������ܣ�����ǰ�ڴ������ݴ�������ļ�
   �����������
   �������: ��
   ����ֵ��BOOL����ΪTRUE
   ����˵������������Ϊ�˷������
*/
BOOL SaveData(void)
{
	if(SaveSysData(gp_head, FileName)==FALSE)
		printf("����\n");
	rewind(stdin);
	getchar();
	ClearScreen();
	ShowMenu();
	return TRUE;
}

/*
   �������ƣ�BackupData
   �������ܣ���������
   �����������
   �������: ��
   ����ֵ��BOOL����ΪTRUE
   ����˵����
*/
BOOL BackupData(void)
{
	ScreenFresh();
	BOOL bRet = TRUE;
	if(SaveSysData(gp_head, BeiFen_FileName))
		printf("���ݱ��ݳɹ���  �����������\n");
	else
		printf("���ݱ���ʧ�ܣ�  �����������\n");
	rewind(stdin);
	getchar();
	ClearScreen();
	ShowMenu();
	return bRet;
}

/*
   �������ƣ�RestoreData
   �������ܣ��ָ���������
   �����������
   �������: ��
   ����ֵ��BOOL����ΪTRUE
   ����˵����
*/
BOOL RestoreData(void)
{
	BOOL bRet = TRUE;
	char ch;
	ScreenFresh();
	rewind(stdin);
	printf("ע��:����ָ��������ݵ�ǰ�����ݽ�������,��ȷ�������ָ���?ȷ��(Y) ȡ��(N):\n");
	ch = getchar(); 
	getchar();
	
	if (ch == 'N' || ch == 'n')
		puts("ȡ���ɹ���\n");
	else if (ch == 'Y' || ch == 'y')
	{
		CreateList(&gp_head, BeiFen_FileName);
		if (SaveSysData(gp_head, FileName))
			printf("�������ݻָ��ɹ�!\n");
		else
			printf("�������ݻָ�ʧ��!\n");
	}
	else
		printf("�������!���β����Զ��˳�\n");

	rewind(stdin);
	printf("\n�����������\n");
	getchar();
	ClearScreen();
	ShowMenu();
	return bRet;
}

/* �˳�ϵͳ
BOOL ExitSys(void)
{
	LABEL_BUNDLE labels;
	HOT_AREA areas;
	BOOL bRet = TRUE;
	SMALL_RECT rcPop, aArea[2];
	COORD pos, aLoc[2];
	WORD att;

	char* pCh[] = { " ȷ���˳�ϵͳ��", "ȷ��(Y)  ȡ��(N)" };
	int iHot = 1;
	char  aSort[] = { 0, 0 }, aTag[] = { 1, 2 };

	ScreenFresh();//�������Ļ
	pos.X = strlen(pCh[0]) + 7;
	pos.Y = 8;
	rcPop.Left = (SCR_COL - pos.X) / 2;
	rcPop.Right = rcPop.Left + pos.X - 1;
	rcPop.Top = (SCR_ROW - pos.Y) / 2;
	rcPop.Bottom = rcPop.Top + pos.Y - 1;
	att = FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;�׵�������
	labels.num = 2;
	labels.ppLabel = pCh;
	���ð�ťλ������
	aLoc[0].X = rcPop.Left + 3;
	aLoc[0].Y = rcPop.Top + 2;
	aLoc[1].X = rcPop.Left + 5;
	aLoc[1].Y = rcPop.Top + 5;
	labels.pLoc = aLoc;
	areas.num = 2;
	����������Χ
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
	PopUp(&rcPop, att, &labels, &areas);��ָ�������������������Ϣ������Ϣ��ջ,�����ȼ�
	/*��һ���ָ���
	pos.X = rcPop.Left + 1;
	pos.Y = rcPop.Top + 4;
	FillConsoleOutputCharacter(gh_std_out, '*', rcPop.Right - rcPop.Left - 1, pos, &ul);
	if (DealQuitInput(&areas, &iHot) == 1 && iHot == 1)
		bRet = FALSE;
	else  bRet = TRUE;
	PopOff();�رյ�������
	return bRet;
}
*/

/*
   �������ƣ�ShowModule
   �������ܣ�����ǩ������Ϊ����
   ���������pString��ű�ǩ��,�Լ���ǩ����n
   �������: ��
   ����ֵ����������س����߿ո񷵻ص�ǰ�������
   ����˵����
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

	att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*�׵׺���*/
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
//����ά�����

//����������Ϣά��
/*
   �������ƣ�MaintainHomeInfo
   �������ܣ�����ά��������������Ϣ
   �����������
   �������:
   ����ֵ��BOOL����ΪTRUE
   ����˵����
*/
BOOL MaintainHomeInfo(void)
{
	char x;
	int choice;
	while (TRUE)
	{
		printf("���ڿ�ʼά������������Ϣ��\n��ѡ��\n");
		printf("1.����    2.�޸�     3.ɾ��\n");
		scanf("%d", &choice);
		getchar();
		if (choice < 1 || choice>3)
		{
			char x;
			printf("��������Ƿ��������룿y-�� n-��\n");
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
		printf("\n�����Ƿ����ά����\n");
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
			printf("\n����ά������---------��������˳�\n");
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
   �������ƣ�InsertHome
   �������ܣ����복��������Ϣ
   �����������
   �������:
   ����ֵ��BOOL����ΪTRUE
   ����˵����
*/
BOOL InsertHome(void)
{
	ScreenFresh();
	HOME_NODE* ptemp= (HOME_NODE*)malloc(sizeof(HOME_NODE));
	ptemp->snext = NULL;
	ptemp->next = NULL;
	printf("�����������ӵĳ������������Ϣ��\n\n");
	printf("�����복�����ͱ���:"); 
	if (!scanf_s("%c", &ptemp->car_id, sizeof(char)))
	{
		printf("��������Զ��˳���\n");
		free(ptemp);
		return TRUE;
	}
	getchar(); 
	printf("\n");
	printf("�����복����������:");
	if (!scanf_s("%s", ptemp->type, sizeof(char)*20))
	{
		printf("��������Զ��˳���\n");
		free(ptemp);
		return TRUE;
	}
	getchar(); 
	printf("\n");
	
	printf("�����������Զ�����Ϊ0��������ӣ����ڳ���������Ϣ����ӣ�\n\n");
	
	printf("�����������\n");
	getchar();
	rewind(stdin);  //������뻺����
	ScreenFresh();
	if (InsertHomeNode(&gp_head, ptemp) == TRUE)   //���밲ȫ
	{
		printf("��ӳ���������Ϣ�ɹ���\n");
		//SortHomeNode(&gp_head);
		ChangeCarNumber(&gp_head);
		if (SaveSysData(gp_head, FileName))
			printf("�����Զ����棡\n");
		else
			printf("����ʧ�ܣ�\n");
	}
	else
		printf("�˳�����Ϣ�Ѵ��ڣ�����ʧ�ܣ�\n");
	return TRUE;
}

/*
   �������ƣ�InsertHomeNode
   �������ܣ����복��������Ϣ���
   ���������������ͷָ���ַ���Լ�����޸���Ϣ�Ľڵ�ptemp
   �������: 
   ����ֵ��BOOL,TRUE˵������ɹ���FALSE˵������ʧ��
   ����˵����
*/
BOOL InsertHomeNode(HOME_NODE** phead, HOME_NODE* ptemp)
{
	HOME_NODE* p = (*phead)->next;
	while (p != NULL && p->car_id != ptemp->car_id)
	{
		p = p->next;
	}
	if (p == NULL)    //ͷ�巨������дһ��������밴���˳����
	{
		ptemp->next = (*phead)->next;
		(*phead)->next = ptemp;
		return TRUE;
	}
	else   //���������Ѿ���ʹ��
		return FALSE;
}

/*
   �������ƣ�ModifyHome
   �������ܣ��޸ĳ���������Ϣ
   �����������
   �������:
   ����ֵ��BOOL,TRUE˵���޸ĳɹ���FALSE˵���޸�ʧ��
   ����˵����
*/
BOOL ModifyHome(void)
{
	ScreenFresh();
	HOME_NODE* ptemp = (HOME_NODE*)malloc(sizeof(HOME_NODE));
	ptemp->snext = NULL;
	ptemp->next = NULL;
	char type[20];
	printf("��������Ҫ�޸ĵĳ�����������(�羭���ͣ�:");
	if (!scanf_s("%s", type, sizeof(char) * 20))
	{
		printf("��������Զ��˳���\n");
		return TRUE;
	} 
	getchar();

	if (!SeekHomeNodeByCarName(gp_head, type))   //������һ�������ͣ�����
	{
		printf("�޴����ͣ��Զ��˳���\n");
		free(ptemp);
		return TRUE;
	}

	printf("\n���������޸ĵĳ������������Ϣ��\n\n");
	printf("�����복�����ͱ���:");
	if (!scanf_s("%c", &ptemp->car_id, sizeof(char)))
	{
		printf("��������Զ��˳���\n");
		free(ptemp);
		return TRUE;
	}
	getchar();
	if (SeekHomeNodeByCarID(gp_head, ptemp->car_id))   //������һ���ı��룬����
	{
		printf("�����ظ����Զ��˳���\n");
		free(ptemp);
		return TRUE;
	}
	printf("\n");
	printf("�����복����������:");
	if (!scanf_s("%s", &ptemp->type, sizeof(char) * 20))
	{
		printf("��������Զ��˳���\n");
		free(ptemp);
		return TRUE;
	}
	getchar();
	if (SeekHomeNodeByCarName(gp_head, ptemp->type))   //������һ�������ͣ�����
	{
		printf("�����ظ����Զ��˳���\n");
		free(ptemp);
		return TRUE;
	}
	printf("\n");

	printf("�����������\n");
	getchar();

	rewind(stdin);  //������뻺����
	ScreenFresh();
	if (ModifyHomeNode(&gp_head, type, ptemp) == TRUE)   //���밲ȫ
	{
		printf("�޸ĳɹ���\n");
		//SortHomeNode(&gp_head);
		if (SaveSysData(gp_head, FileName))
			printf("�����Զ����棡\n");
		else
			printf("����ʧ�ܣ�\n");
	}
	else
		printf("�����Ͳ����ڣ��޸�ʧ��!\n");
	return TRUE;
}

/*
   �������ƣ�ModifyHomeNode
   �������ܣ��޸ĳ�������ڵ�
   �������������ͷָ���ַphead��������������type������޸���Ϣ�Ľڵ�ptemp
   �������:
   ����ֵ��BOOL,TRUE˵���޸ĳɹ���FALSE˵���޸�ʧ��
   ����˵����
*/
BOOL  ModifyHomeNode(HOME_NODE** phead, char type[20], HOME_NODE* phome_node)
{
	HOME_NODE* phome_temp;
	//HOME_NODE* phome_next;
	BOOL  bRet = FALSE;
	phome_temp = SeekHomeNodeByCarName(*phead, type);
	if (phome_temp != NULL)   //�ҵ���
	{
		phome_temp->car_id = phome_node->car_id;
		strcpy(phome_temp->type, phome_node->type);
		bRet = TRUE;
	}
	return bRet;
}

/*
   �������ƣ�DelHome
   �������ܣ�ɾ������������Ϣ
   �����������
   �������:
   ����ֵ��BOOL,TRUE˵��ɾ���ɹ���FALSE˵��ɾ��ʧ��
   ����˵����
*/
BOOL DelHome(void)
{
	char type[20];
	printf("��������Ҫɾ���ĳ�����������(�羭���ͣ�:\n");
	if (!scanf_s("%s", type, sizeof(char) * 20))
	{
		printf("��������Զ��˳���\n");
		return TRUE;
	}
	getchar();
	printf("\n");
	if (!SeekHomeNodeByCarName(gp_head, type))   
	{
		printf("�޴˳������ͣ��Զ��˳���\n");
		return TRUE;
	}
	rewind(stdin);  //������뻺����

	if (DelHomeNode(&gp_head, type) == TRUE)
		printf("ɾ���ɹ�!\n");
	else
		printf("ɾ��ʧ�ܣ�\n");
	SaveSysData(gp_head, FileName);/*������Ϣ*/
	return TRUE;
}

/*
   �������ƣ�DelHomeNode
   �������ܣ�ɾ������������Ϣ���
   �������������ͷָ���ַphead��������������type
   �������:
   ����ֵ��BOOL,TRUE˵��ɾ���ɹ���FALSE˵��ɾ��ʧ��
   ����˵����
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
			break;    //�ҵ���
		}
	}
	
	if (phome_current != NULL)        //�ҵ��˶�Ӧ�ڵ�
	{
		if (phome_prior == NULL)            //����ͷŵ��ǳ���������Ϣ������ͷ���
		{
			(*phead) = phome_current->next;
		}
		else
		{
			phome_prior->next = phome_current->next;
		}

		pcar_current = phome_current->snext;
		free(phome_current);         //�ͷ�Ҫɾ���ĳ���������Ϣ���

		while (pcar_current != NULL)    //�ͷŶ�Ӧ�ĳ�������Ͷ�����Ϣ
		{
			pcharge_current = pcar_current->cnext;
			pcar_prior = pcar_current;
			while (pcharge_current != NULL)
			{
				pcharge_prior = pcharge_current;
				pcharge_current = pcharge_current->next;
				free(pcharge_prior);     //ɾ���������޶������
			}
			pcar_current = pcar_current->next;
			free(pcar_prior);     //ɾ���ó��Ͷ�Ӧ�ĳ���������Ϣ���
		}
	}
	return bRet;
}

//----------------------------------------------------------------------------------------
//����������Ϣά��
/*
   �������ƣ�MaintainCarInfo
   �������ܣ�����ά��������������Ϣ
   �����������
   �������:
   ����ֵ��BOOL����ΪTRUE
   ����˵����
*/
BOOL MaintainCarInfo(void)
{
	char x;
	int choice;
	while (TRUE)
	{
		printf("���ڿ�ʼά������������Ϣ��\n��ѡ��\n");
		printf("1.����    2.�޸�     3.ɾ��\n");
		scanf("%d", &choice);
		getchar();

		if (choice < 1 || choice>3)
		{
			char x;
			printf("��������Ƿ��������룿y-�� n-��\n");
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
		printf("\n�����Ƿ����ά����\n");
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
			printf("\n����ά������---------��������˳�\n");
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
   �������ƣ�InsertCar
   �������ܣ����복��������Ϣ
   �����������
   �������:
   ����ֵ��BOOL,TRUE˵����ӳɹ���FALSE˵�����ʧ��
   ����˵����
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
	printf("�����������ӵĳ���������Ϣ\n");
	printf("--------------------------\n");
	printf("�����복�����:"); 
	if (!scanf_s("%d", &ptemp->number))
	{
		printf("��������Զ��˳���\n");
		free(ptemp);
		return TRUE;
	}
	getchar(); printf("\n");

	if (SeekCarNodeByNumber(gp_head, ptemp->number))
	{
		printf("����ظ����Զ��˳���\n");
		free(ptemp);
		return TRUE;
	}

	printf("�����복�ƺ�:"); 
	if (!scanf_s("%s", ptemp->car_num, sizeof(char) * 10))
	{
		printf("��������Զ��˳���\n");
		free(ptemp);
		return TRUE;
	}
	getchar(); printf("\n");

	printf("�����복�����ͱ���:"); 
	if (!scanf_s("%c", &ptemp->car_id, sizeof(char)))
	{
		printf("��������Զ��˳���\n");
		free(ptemp);
		return TRUE;
	}
	getchar(); printf("\n");

	printf("�����복������:"); 
	if (!scanf_s("%s", ptemp->car_name, sizeof(char) * 20))
	{
		printf("��������Զ��˳���\n");
		free(ptemp);
		return TRUE;
	}
	getchar(); printf("\n");

	printf("�����복���ŵ���ʽ:"); 
	if (!scanf_s("%s", ptemp->car_gear, sizeof(char) * 10))
	{
		printf("��������Զ��˳���\n");
		free(ptemp);
		return TRUE;
	}
    getchar(); printf("\n");

	printf("������ÿ�����:"); 
	if (!scanf_s("%f", &ptemp->fee))
	{
		printf("��������Զ��˳���\n");
		free(ptemp);
		return TRUE;
	}
	getchar(); printf("\n");

	ptemp->id = 'n';     //�Զ�Ĭ������״̬Ϊδ����
	rewind(stdin);

	ScreenFresh();
	iRet = InsertCarNode(&gp_head,ptemp);
	if (iRet == 1)       
		printf("����������Ϣ����ɹ�!\n");
	else if (iRet == -1)  
		printf("�����Ѵ��ڣ�����ʧ�ܣ�\n");
	else  
		printf("�����ڸ��೵��������ʧ�ܣ�\n");
	SaveSysData(gp_head, FileName);
	return bRet;
}

/*
   �������ƣ�InsertCarNode
   �������ܣ����복��������Ϣ�ڵ�
   �������������ͷָ���ַphead,Ҫ�������Ϣ�ڵ�p
   �������:
   ����ֵ��BOOL,TRUE˵����ӳɹ���FALSE˵�����ʧ��
   ����˵����
*/
BOOL InsertCarNode(HOME_NODE** phead,CAR_NODE* p)   //�����ж����������Int
{
	HOME_NODE* phome_node=(HOME_NODE*)malloc(sizeof(HOME_NODE));
	CAR_NODE* pcar_temp;    //����ʱ�õ�
	int find = 0;

	pcar_temp = SeekCarNodeByCarNum(*phead, p->car_num);   //�ó��ƺ����Ͳ���

	if (pcar_temp != NULL)
	{
		find = -1;  /*�ó����Ѵ���*/
	}

	else   /*��Ҫ����ĳ���������,������뵽��Ӧ�ĳ�����������*/
	{
		for (phome_node = (*phead)->next; phome_node != NULL; phome_node = phome_node->next)
		{
			if (phome_node->car_id == p->car_id)
			{
				find = 1;  /*�ҵ��ó�����Ӧ�����*/
				break;
			}
		}
	}

	if (find)   //���ó��������Ӧ�ĳ�����������,ͷ�巨
	{
		p->next = phome_node->snext;
		phome_node->snext = p;
	}
	else    //û�ҵ���Ӧ�ĳ�������
		free(p);
	return find;
}

/*
   �������ƣ�ModifyCar
   �������ܣ��޸ĳ���������Ϣ
   �����������
   �������:
   ����ֵ��BOOL,TRUE˵���޸ĳɹ���FALSE˵���޸�ʧ��
   ����˵����
*/
BOOL ModifyCar(void)
{
	rewind(stdin);
	BOOL bRet = TRUE;
	char car_num[10];
	ScreenFresh();
	printf("������Ҫ�޸ĵĳ��ƺţ�");
	if (!scanf_s("%s", car_num, sizeof(char)*10))
	{
		printf("��������Զ��˳���\n");
		return TRUE;
	}
	getchar();

	if (!SeekCarNodeByCarNum(gp_head, car_num))
	{
		printf("δ�ҵ���Ӧ�������Զ��˳���\n");
		return TRUE;
	}

	CAR_NODE* ptemp = (CAR_NODE*)malloc(sizeof(CAR_NODE));
	ptemp->cnext = NULL;
	ptemp->next = NULL;
	printf("���������޸ĵĳ���������Ϣ\n");
	printf("--------------------------\n");

	printf("�����복�ƺ�:");
	if (!scanf_s("%s", ptemp->car_num, sizeof(char) * 10))
	{
		printf("��������Զ��˳���\n");
		free(ptemp);
		return TRUE;
	}
	getchar(); printf("\n");

	if (!SeekCarNodeByCarNum(gp_head, car_num))
	{
		printf("δ�ҵ���Ӧ�������Զ��˳���\n");
		return TRUE;
	}

	printf("�����복�����ͱ���:");
	if (!scanf_s("%c", &ptemp->car_id, sizeof(char)))
	{
		printf("��������Զ��˳���\n");
		free(ptemp);
		return TRUE;
	}
	getchar(); printf("\n");

	printf("�����복������:");
	if (!scanf_s("%s", ptemp->car_name, sizeof(char) * 20))
	{
		printf("��������Զ��˳���\n");
		free(ptemp);
		return TRUE;
	}
	getchar(); printf("\n");

	printf("�����복���ŵ���ʽ:");
	if (!scanf_s("%s", ptemp->car_gear, sizeof(char) * 10))
	{
		printf("��������Զ��˳���\n");
		free(ptemp);
		return TRUE;
	}
	getchar(); printf("\n");

	printf("������ÿ�����:");
	if (!scanf_s("%f", &ptemp->fee))
	{
		printf("��������Զ��˳���\n");
		free(ptemp);
		return TRUE;
	}
	getchar(); printf("\n");

	rewind(stdin);
	ScreenFresh();
	if (ModifyCarNode(&gp_head, car_num, ptemp) == TRUE)
	{
		printf("�޸ĳɹ���\n");
		SaveSysData(gp_head, FileName);
	}
    else  
	{ 
		printf("�޸�ʧ�ܣ�\n"); 
		free(ptemp); 
	}
    return bRet;
}

/*
   �������ƣ�ModifyHome
   �������ܣ��޸ĳ���������Ϣ���
   ���������������ͷ�ڵ�ָ���ַphead,���ƺ�car_num������޸���Ϣ�Ľڵ�ָ��pcar_node
   �������:
   ����ֵ��BOOL,TRUE˵���޸ĳɹ���FALSE˵���޸�ʧ��
   ����˵����
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
				break;    /*�˳���һ��ѭ��*/
			}
			pcar_prior = pcar_cur;
			pcar_cur = pcar_cur->next;
		}
		if (find)
			break;/*�˳��ڶ���ѭ��*/
		phome_node = phome_node->next;
	}

    if(pcar_cur != NULL)   //���ҳɹ�
    {
		if (pcar_cur->car_id != pcar_node->car_id)  //����޸�������
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
				
				pcar_cur->next = pHomeNode->snext;  //����
				pHomeNode->snext = pcar_cur;
			}
			else  //���δ�ҵ����ͷŽ��ռ�
			{
				printf("�޸ĵ����Ͳ����ڣ�\n");
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
   �������ƣ�DelCar
   �������ܣ�ɾ������������Ϣ
   �����������
   �������:
   ����ֵ��BOOL,TRUE˵��ɾ���ɹ���FALSE˵��ɾ��ʧ��
   ����˵����
*/
BOOL DelCar(void)
{
    BOOL bRet=TRUE;
    char car_num[10];
    ScreenFresh();

	printf("�����복�ƺ�:");
	if (!scanf_s("%s", car_num, sizeof(char) * 10))
	{
		printf("��������Զ��˳���\n");
		return TRUE;
	}

    getchar();
	if (DelCarNode(&gp_head, car_num) == TRUE)
	{
		printf("ɾ���ɹ�!\n");
		SaveSysData(gp_head, FileName);
	}
    else 
		printf("δ�ҵ��ó�����ɾ��ʧ�ܣ�\n");
    return bRet;
}

/*
   �������ƣ�DelCarNode
   �������ܣ�ɾ������������Ϣ���
   ���������������ͷָ���ַphead,���ƺ�car_num
   �������:
   ����ֵ��BOOL,TRUE˵��ɾ���ɹ���FALSE˵��ɾ��ʧ��
   ����˵����
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
        if(pcar_current != NULL)   //�ҵ��˳������
        {
            bRet = TRUE;
            break;
        }
        phome_node = phome_node ->next;
    }

    if(phome_node != NULL)
    {
        if(pcar_prior == NULL)   /*ɾ�����ǳ���������Ϣ������ͷ*/
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
//�⳵������Ϣά������
/*
   �������ƣ�MaintainChargeInfo
   �������ܣ�����ά����������������Ϣ
   �����������
   �������:
   ����ֵ��BOOL����ΪTRUE
   ����˵����
*/
BOOL MaintainChargeInfo(void)
{
	char x;
	int choice;
	ChangeID(&gp_head);
	while (TRUE)
	{
		printf("���ڿ�ʼά���⳵��Ϣ��\n��ѡ��\n");
		printf("1.����    2.�޸�     3.ɾ��\n");
		scanf("%d", &choice);
		getchar();
		if (choice < 1 || choice>3)
		{
			char x;
			printf("��������Ƿ��������룿y-�� n-��\n");
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
		printf("\n�����Ƿ����ά����\n");
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
			printf("\n����ά������---------��������˳�\n");
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
   �������ƣ�InsertCharge
   �������ܣ���Ӷ�����Ϣ
   �����������
   �������:
   ����ֵ��BOOL,TRUE˵����ӳɹ���FALSE˵�����ʧ��
   ����˵����
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
	printf("����������ӵĶ�����Ϣ��\n\n");
	ShowTime();
	//���������ȡ�������Զ�����
	printf("\n������������֤��:");
	if (!scanf_s("%s", ptemp->guest_id, sizeof(char) * 20))
	{
		printf("��������Զ��˳���\n");
		free(ptemp);
		return TRUE;
	}
	getchar(); printf("\n");

	printf("�������������:");
	if (!scanf_s("%s", ptemp->name, sizeof(char) * 20))
	{
		printf("��������Զ��˳���\n");
		free(ptemp);
		return TRUE;
	}
	getchar(); printf("\n");

	printf("��������˵绰����:");
	if (!scanf_s("%s", ptemp->tel, sizeof(char) * 20))
	{
		printf("��������Զ��˳���\n");
		free(ptemp);
		return TRUE;
	}
	getchar(); printf("\n");

	printf("���������õĳ������:");
	if (!scanf_s("%d", &ptemp->number))
	{
		printf("��������Զ��˳���\n");
		free(ptemp);
		return TRUE;
	}
	getchar(); printf("\n");

	//��ѯ���õĳ�������Ƿ����
	pcar = SeekCarNodeByNumber(gp_head, ptemp->number);

	if (pcar == NULL)
	{
		printf("��Ǹ�����⳵�����ڿ��У����Զ��˳�\n");
		free(ptemp);
		return TRUE;
	}

	if (pcar->id == 'y')
	{
		printf("%s %s %d %c\n", pcar->car_num, pcar->car_name, pcar->number, pcar->id);
		printf("��Ǹ�����⳵���ѳ��⣡���Զ��˳�\n");
		free(ptemp);
		return TRUE;
	}

	printf("������ȡ��ʱ��   ����ʾ����2019/02/15-13:00\n\n");
	if (!scanf_s("%s", ptemp->time_of_get_car, sizeof(char) * 20))
	{
		printf("��������Զ��˳���\n");
		free(ptemp);
		return TRUE;
	}
	getchar(); printf("\n");

	strcpy(s1, ptemp->time_of_get_car);

	if (!time_test(ptemp->time_of_get_car))
	{
		printf("��������Զ��˳���\n");
		free(ptemp);
		return TRUE;
	}
	printf("������ԤԼ����ʱ��   ����ʾ����2019/02/15-13:00\n\n");
	if (!scanf_s("%s", ptemp->time_of_scheduled_back, sizeof(char) * 20))
	{
		printf("��������Զ��˳���\n");
		free(ptemp);
		return TRUE;
	}
	getchar(); printf("\n");

	strcpy(s2, ptemp->time_of_scheduled_back);

	if (!time_test(ptemp->time_of_scheduled_back) || strcmp(ptemp->time_of_get_car, ptemp->time_of_scheduled_back) > 0)
	{
		printf("��������Զ��˳���\n");
		free(ptemp);
		return TRUE;
	}

	rewind(stdin);
	printf("�����Ƿ��Ѿ��黹   �ǣ�y)  ��n)\n");
	char c;
	scanf("%c", &c);
	getchar();
	if (c == 'y' || c == 'Y')
	{
		printf("\n������ʵ�ʻ���ʱ��     ����ʾ����2019/02/15-13:00\n\n");
		if (!scanf_s("%s", ptemp->time_of_actual_back, sizeof(char) * 18))
		{
			printf("��������Զ��˳���\n");
			free(ptemp);
			return TRUE;
		}
		getchar();
		printf("\n");
		strcpy(s3, ptemp->time_of_actual_back);

		if (!time_test(ptemp->time_of_actual_back))
		{
			printf("��������Զ��˳���\n");
			free(ptemp);
			return TRUE;
		}

		char time2[20];
		strcpy(time2, GetTime());   //��ǰʱ��

		if (strcmp(func(ptemp->time_of_actual_back), time2) > 0)  //�ȵ�ǰʱ����
		{
			printf("��������Զ��˳���\n");
			free(ptemp);
			return TRUE;
		}
	}

	else if (c == 'n' || c == 'N')
	{
		strcpy(ptemp->time_of_actual_back, "0");   //δ�黹
		strcpy(s3, ptemp->time_of_actual_back);
		ptemp->actual_pay = 0.0;
	}

	else
	{
		printf("��������Զ��˳�\n");
		return TRUE;
	}

	char time0[9] = { "" };  //��������Ϣ
	strncpy(time0, ptemp->time_of_get_car, 8);
	time0[8] = '\0';

	//puts(time0);

	int count = 1;  //���㵱ǰ�����м�������
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
				if (strstr(p3->time_of_get_car, time0))   //��ǰ�������ж���
					count++;
				p3 = p3->next;
			}
			p2 = p2->next;
		}
		p1 = p1->next;
	}

	//������ȫ��ȷ������£�������ã��������
	if (count >= 100)
	{
		printf("��ȫ��Ҫ���Զ�ֹͣ\n");
		return TRUE;
	}
	char t[3] = { "" };
	itoa(count, t, 10);
	strcpy(ptemp->rent_id, ptemp->time_of_get_car);
	strncpy(ptemp->rent_id + 8, t, 3);
	ptemp->rent_id[10] = '\0';
	//��funcǰһ��Ҫ����ԭ�ַ���

	ptemp->should_pay = pcar->fee * diff_day(func(ptemp->time_of_get_car), func(ptemp->time_of_scheduled_back));   //Ӧ�ɷ���
	ptemp->deposit = ptemp->should_pay * 5;

	if (strcmp(ptemp->time_of_actual_back, "0") > 0)
		ptemp->actual_pay = pcar->fee * diff_day(func(ptemp->time_of_get_car), func(ptemp->time_of_actual_back));   //ʵ�ɷ���
	else
		ptemp->actual_pay = 0.0;

	strcpy(ptemp->time_of_get_car, s1);
	strcpy(ptemp->time_of_scheduled_back, s2);
	strcpy(ptemp->time_of_actual_back, s3);

	printf("\n�����㣬����������£�\n");
	printf("Ѻ��        Ӧ�ɷ���      ʵ�ɷ���\n");
	printf("%-12.2f%-14.2f%-12.2f", ptemp->deposit, ptemp->should_pay, ptemp->actual_pay);

	rewind(stdin);
	printf("\n�����������\n");
	getchar();
	ClearScreen();
	ShowMenu();
	ScreenFresh();

	int re = InsertChargeNode(&gp_head, ptemp);
	if (re == 1)
		printf("��ӳɹ���\n");
	else if (re == -1)
		printf("�ö����Ѿ����ڣ����ʧ�ܣ�\n");
	else
		printf("�ö�����Ӧ�ĳ��������ڣ����߸ö��������ڣ�����ʧ�ܣ�\n");

	SaveSysData(gp_head, FileName);
	return bRet;
}

/**
*�������ƣ�InsertChargeNode
*�������ܣ���ʮ�������в���һ���ɷ���Ϣ���
*���������hd����ͷָ�룬pcharge_node��ָ����Ҫ�������ָ��
*�����������
*����ֵ��int���ͣ�1 OR -1 or else
*����˵����
*/
BOOL InsertChargeNode(HOME_NODE** phead, CHARGE_NODE* pcharge_node)
{
	CAR_NODE* pcar_node = (CAR_NODE*)malloc(sizeof(CAR_NODE));
	CHARGE_NODE* ptemp_node = (CHARGE_NODE*)malloc(sizeof(CHARGE_NODE));
	BOOL bret = FALSE;
	//������ʮ���������ҵ���Ӧ�ĳ���������Ϣ���
	pcar_node = SeekCarNodeByNumber(*phead, pcharge_node->number);   //������ݳ�����Ų���
	if (pcar_node != NULL)
		ptemp_node = SeekChargeNode(*phead, pcharge_node->rent_id, pcar_node->car_num);
	if (ptemp_node != NULL)
	{
		bret = -1;    //�ö����Ѿ�����
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
   �������ƣ�DelCharge
   �������ܣ�ɾ��������趩����Ϣ
   �����������
   �������:
   ����ֵ��BOOL,TRUE˵��ɾ���ɹ���FALSE˵��ɾ��ʧ��
   ����˵����
*/
BOOL DelCharge(void)
{
	char rent_id[20];
	ScreenFresh();
	rewind(stdin);
	printf("����������Ҫɾ���Ķ������\n\n");
	if (!scanf_s("%s", rent_id, sizeof(char) * 20))
	{
		printf("��������Զ��˳���\n");
		return TRUE;
	}
	getchar(); printf("\n");

	if (DelChargeNode(&gp_head, rent_id) == TRUE)
	{
		printf("ɾ���ɹ�!\n");
		SaveSysData(gp_head, FileName);
	}
	else
		printf("�ö��������ڣ�ɾ��ʧ�ܣ�\n");
	return TRUE;
}

/**
*�������ƣ�DelChargeNode
*�������ܣ���ʮ��������ɾ��ָ�����⳵������Ϣ���
*���������hd����ͷָ�룬rent_id�����ţ�car_num���ƺ�
*�����������
*����ֵ��BOOL���ͣ�TRUE OR FALSE
*����˵�������ݶ����ſ���ȷ��Ψһ�Ľɷ���Ϣ
*/
BOOL DelChargeNode(HOME_NODE** phead, char* rent_id)    //ɾ���⳵������Ϣ���
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
			//�ڶ�����Ϣ֧���ϲ��Ҹ��������ŵĶ�����Ϣ���
			pcharge_node_current = pcar_node->cnext;
			pcharge_node_prior = NULL;
			while (pcharge_node_current != NULL && strcmp(pcharge_node_current->rent_id, rent_id) != 0)
			{
				pcharge_node_prior = pcharge_node_current;           //��ǰ����
				pcharge_node_current = pcharge_node_current->next;
			}

			if (pcharge_node_current != NULL)  //����ҵ�
			{
				bRet = TRUE;
				if (pcharge_node_prior == NULL)  //���λ�ڶ���֧���ϵĵ�һ��λ��
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

			pcar_node = pcar_node->next;   //û�ҵ���������
		}
		p = p->next;
	}
	return bRet;
}

/*
   �������ƣ�ModifyCharge
   �������ܣ��޸��⳵������Ϣ
   �����������
   �������:
   ����ֵ��BOOL,TRUE˵���޸ĳɹ���FALSE˵���޸�ʧ��
   ����˵����
*/
BOOL ModifyCharge(void)
{
	char* time = GetTime();   //��ǰʱ��
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

	printf("��������Ҫ�޸ĵĶ������\n");
	if (!scanf_s("%s", rent_id, sizeof(char) * 20))
	{
		printf("��������Զ��˳���\n");
		free(ptemp);
		return TRUE;
	}
	getchar(); printf("\n");

	CHARGE_NODE* p = SeekChargeNodeByRentId(gp_head, rent_id);

	if (!p)
	{
		printf("δ�ҵ��ö����� �Զ��˳���\n");
		return TRUE;
	}

	pcar = SeekCarNodeByNumber(gp_head, p->number);
	ptemp->number = p->number;

	ScreenFresh();
	printf("���������޸ĵĶ�����Ϣ��\n\n");
	ShowTime();

	printf("\n������������֤��:");
	if (!scanf_s("%s", ptemp->guest_id, sizeof(char) * 20))
	{
		printf("��������Զ��˳���\n");
		free(ptemp);
		return TRUE;
	}
	getchar(); printf("\n");

	printf("�������������:");
	if (!scanf_s("%s", ptemp->name, sizeof(char) * 20))
	{
		printf("��������Զ��˳���\n");
		free(ptemp);
		return TRUE;
	}
	getchar(); printf("\n");

	printf("��������˵绰����:");
	if (!scanf_s("%s", ptemp->tel, sizeof(char) * 20))
	{
		printf("��������Զ��˳���\n");
		free(ptemp);
		return TRUE;
	}
	getchar(); printf("\n");

	printf("������ȡ��ʱ��   ����ʾ����2019/02/15-13:00\n\n");
	if (!scanf_s("%s", ptemp->time_of_get_car, sizeof(char) * 20))
	{
		printf("��������Զ��˳���\n");
		free(ptemp);
		return TRUE;
	}
	getchar(); printf("\n");

	strcpy(s1, ptemp->time_of_get_car);

	if (!time_test(ptemp->time_of_get_car))
	{
		printf("��������Զ��˳���\n");
		free(ptemp);
		return TRUE;
	}

	printf("������ԤԼ����ʱ��   ����ʾ����2019/02/15-13:00\n\n");
	if (!scanf_s("%s", ptemp->time_of_scheduled_back, sizeof(char) * 20))
	{
		printf("��������Զ��˳���\n");
		free(ptemp);
		return TRUE;
	}
	getchar(); printf("\n");

	strcpy(s2, ptemp->time_of_scheduled_back);

	if (!time_test(ptemp->time_of_scheduled_back) || strcmp(ptemp->time_of_get_car, ptemp->time_of_scheduled_back) > 0)
	{
		printf("��������Զ��˳���\n");
		free(ptemp);
		return TRUE;
	}

	rewind(stdin);
	printf("�����Ƿ��Ѿ��黹   �ǣ�y)  ��n)\n");
	char c;
	scanf("%c", &c);
	getchar();
	if (c == 'y' || c == 'Y')
	{
		printf("\n������ʵ�ʻ���ʱ��     ����ʾ����2019/02/15-13:00\n\n");
		if (!scanf_s("%s", ptemp->time_of_actual_back, sizeof(char) * 18))
		{
			printf("��������Զ��˳���\n");
			free(ptemp);
			return TRUE;
		}
		getchar(); printf("\n");
		strcpy(s3, ptemp->time_of_actual_back);
		if (!time_test(ptemp->time_of_actual_back))
		{
			printf("��������Զ��˳���\n");
			free(ptemp);
			return TRUE;
		}
	}

	else if (c == 'n' || c == 'N')
	{
		strcpy(ptemp->time_of_actual_back, "0");   //δ�黹
		strcpy(s3, ptemp->time_of_actual_back);
		ptemp->actual_pay = 0.0;
	}

	else
	{
		printf("��������Զ��˳�\n");
		return TRUE;
	}

	char time0[9] = { "" };  //��������Ϣ
	strncpy(time0, ptemp->time_of_get_car, 8);
	time0[8] = '\0';

	int count = 1;  //���㵱ǰ�����м�������
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
				if (strstr(p3->time_of_get_car, time0))   //��ǰ�������ж���
					count++;
				p3 = p3->next;
			}
			p2 = p2->next;
		}
		p1 = p1->next;
	}

	//������ȫ��ȷ������£�������ã��������
	if (count >= 100)
	{
		printf("��ȫ��Ҫ���Զ�ֹͣ\n");
		return TRUE;
	}
	char t[3] = { "" };
	itoa(count, t, 10);
	strcpy(ptemp->rent_id, ptemp->time_of_get_car);
	strncpy(ptemp->rent_id + 8, t, 3);
	ptemp->rent_id[10] = '\0';
	//��funcǰһ��Ҫ����ԭ�ַ���
	ptemp->should_pay = pcar->fee * diff_day(func(ptemp->time_of_get_car), func(ptemp->time_of_scheduled_back));   //Ӧ�ɷ���
	ptemp->deposit = ptemp->should_pay * 5;

	if (strcmp(ptemp->time_of_actual_back, "0") > 0)
		ptemp->actual_pay = pcar->fee * diff_day(func(ptemp->time_of_get_car), func(ptemp->time_of_actual_back));   //ʵ�ɷ���
	else
		ptemp->actual_pay = 0.0;

	strcpy(ptemp->time_of_get_car, s1);
	strcpy(ptemp->time_of_scheduled_back, s2);
	strcpy(ptemp->time_of_actual_back, s3);
	printf("\n�����㣬����������£�\n");
	printf("Ѻ��        Ӧ�ɷ���      ʵ�ɷ���\n");
	printf("%-12.2f%-14.2f%-12.2f", ptemp->deposit, ptemp->should_pay, ptemp->actual_pay);

	rewind(stdin);
	printf("\n�����������\n");
	getchar();
	ClearScreen();
	ShowMenu();
	ScreenFresh();

	if (ModifyChargeNode(&gp_head, rent_id, ptemp) == TRUE)
		printf("�޸ĳɹ���\n");
	else
		printf("�޸�ʧ�ܣ�\n");

	SaveSysData(gp_head, FileName);

	return bRet;
}

/**
*�������ƣ�ModifyChargeNode
*�������ܣ���ָ���⳵��Ϣ�������޸�
*���������hd����ͷָ�룬pcharge_nodeָ�����޸����ݵĽ��ָ�룬rent_id�����ţ�car_num���ƺ�
*�����������
*����ֵ��BOOL���ͣ�TRUE OR FALSE
*����˵����
*/
BOOL ModifyChargeNode(HOME_NODE** phead, char* rent_id, CHARGE_NODE* pcharge_node)
{
	CHARGE_NODE* pcharge_node_temp;
	CHARGE_NODE* pcharge_node_next;
	pcharge_node_temp = SeekChargeNodeByRentId(*phead, rent_id);   //����ָ���ɷ���Ϣ���

	if (pcharge_node_temp != NULL)//���ҳɹ���������޸�
	{
		pcharge_node_next = pcharge_node_temp->next;  //��ֵǰ����ָ����
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
//������һЩ���������������ڹ���ʵ��

/*
   �������ƣ�ChangeID
   �������ܣ��޸ĳ���������Ϣ
   �������������ͷָ���ַphead
   �������:
   ����ֵ��
   ����˵���������������Ҫ�����ݶ�����Ϣ�޸ĳ����ĳ���״̬
*/
void ChangeID(HOME_NODE** phead)       //�����ĳ���״̬���ݶ����͵�ǰʱ�����
{
	char time[20] = "\0";
	strcpy(time, GetTime());   //��ȡ��ǰʱ�䣬��׼12λ�ַ���

	HOME_NODE* p = (HOME_NODE*)malloc(sizeof(HOME_NODE));
	CAR_NODE* pcar_temp = (CAR_NODE*)malloc(sizeof(CAR_NODE));
	CHARGE_NODE* p3 = (CHARGE_NODE*)malloc(sizeof(CHARGE_NODE));
	p = (*phead)->next;

	char status = 'n'; //Ĭ��״̬��δ����
	char save[18];

	while (p != NULL)
	{
		pcar_temp = p->snext;

		while (pcar_temp != NULL)
		{
			status = 'n'; //Ĭ��״̬��δ����
			p3 = pcar_temp->cnext;
			while (p3 != NULL)
			{
				strcpy(save, p3->time_of_actual_back);
				//�ж������������״̬Ϊy
				status = 'y';

				//����Ѿ��黹���ҹ黹ʱ�䲻������ǰ
				if (strcmp(p3->time_of_actual_back, "0") != 0 && strcmp(func(p3->time_of_actual_back), time) < 0)  //�Ѿ��黹
				{
					//printf("�ѹ黹!\n");
					status = 'n';   //�Ѿ��黹��
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
   �������ƣ�SortCarNode
   �������ܣ�����������Ϣ���򣬰����
   �������������ͷָ���ַphead
   �������:
   ����ֵ��
   ����˵��������ʹ��
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
						//����������
						*pre = *q;
						*q = *p;
						*p = *pre;
						//����nextָ����
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
   �������ƣ�SortHomeNode
   �������ܣ�Ϊ��csv�ļ���������˳������д��һ���������������ļ�ǰ������
   ���������������ͷָ���ַ
   �������:
   ����ֵ����
   ����˵����
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
   �������ƣ�SeekHomeNodeByCarID
   �������ܣ���������������ң����ճ������car_id���Ҷ�Ӧ�ĳ���������Ϣ�ڵ�
   ���������������ͷָ�룬�������id
   �������:
   ����ֵ��HOME_NODE*�������ڵ�ָ��
   ����˵����
*/
HOME_NODE* SeekHomeNodeByCarID(HOME_NODE* phead, char car_id)
{
	HOME_NODE* p = phead->next;
	while (p != NULL)
	{
		if (p->car_id == car_id)   //�ҵ���
		{
			return p;
			break;
		}
		p = p->next;
	}
	return NULL;
}

/*
   �������ƣ�SeekHomeNodeByCarName
   �������ܣ���������������ң����ճ�����������type���Ҷ�Ӧ�ĳ���������Ϣ�ڵ�
   ���������������ͷָ�룬
   �������:
   ����ֵ��BOOL,TRUE˵������ɹ���FALSE˵������ʧ��
   ����˵����
*/
HOME_NODE* SeekHomeNodeByCarName(HOME_NODE* phead, char type[20])
{
	HOME_NODE* p = phead->next;
	while (p != NULL)
	{
		if (!strcmp(p->type, type))   //�ҵ���
		{
			return p;
			break;
		}
		p = p->next;
	}
	return NULL;
}

/*
   �������ƣ�SeekCarNodeByCarNum
   �������ܣ����ҳ���������Ϣ��� ʹ�ó��ƺ�
   �������������ͷָ��phead,���ƺ�car_num
   �������:
   ����ֵ��CAR_NODE* �����ڵ�ָ��
   ����˵����
*/
CAR_NODE* SeekCarNodeByCarNum(HOME_NODE* phead, char* car_num)   //�ó��ƺŲ���
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
				break;    /*�˳���һ��ѭ��*/
			}
			pcar_node = pcar_node->next;
		}
		if (find)   break;/*�˳��ڶ���ѭ��*/
		phome_node = phome_node->next;
	}
	if (find)
		return pcar_node;
	else
		return NULL;
}

/*
   �������ƣ�SeekCarNodeByNumber
   �������ܣ����ҳ���������Ϣ��� ʹ�ó������
   �������������ͷָ��phead,�������number
   �������:
   ����ֵ��CAR_NODE* �����ڵ�ָ��
   ����˵����
*/
CAR_NODE* SeekCarNodeByNumber(HOME_NODE* phead, int number)   //�ó�����Ų���
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
				break;    /*�˳���һ��ѭ��*/
			}
			pcar_node = pcar_node->next;
		}
		if (find)
			break;/*�˳��ڶ���ѭ��*/
		phome_node = phome_node->next;
	}
	if (find)
		return pcar_node;
	else
		return NULL;
}

/*
   �������ƣ�SeekChargeNode
   �������ܣ������⳵�������
   �������������ͷָ�룬������ţ����ƺ�
   �������:
   ����ֵ��CHARGE_NODE* �����ڵ�ָ��
   ����˵�������������Ҫ��Ϊ�˲����⳵��Ϣ�ڵ�ʱ�����ж���û������������ظ�������ʵ�ʲ��Ҷ����ڵ�ֻ�ж�����
*/
CHARGE_NODE* SeekChargeNode(HOME_NODE* phead, char* rent_id, char* car_num)  //rent_id�����ţ�car_num�ǳ��ƺţ�����Ҫ�����ƺ��ҵ���Ӧ�ĳ���
{
	CAR_NODE* pcar_node;
	CHARGE_NODE* pcharge_node;
	int find = 0;
	pcar_node = SeekCarNodeByCarNum(phead, car_num);      /*���Ȳ��Ҷ�Ӧ�ĳ�����Ϣ���*/
	/*����ҵ������ڳ�����Ϣ֧���ϼ�����ָ���Ķ�����Ϣ���*/
	if (pcar_node != NULL)
	{
		pcharge_node = pcar_node->cnext;
		while (pcharge_node != NULL)
		{
			if (strcmp(pcharge_node->rent_id, rent_id) == 0)  //�ȶԶ�����
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
   �������ƣ�SeekChargeNodeByRentId
   �������ܣ������⳵�������
   �������������ͷָ�룬�������
   �������:
   ����ֵ��CHARGE_NODE* �����ڵ�ָ��
   ����˵����
*/
CHARGE_NODE* SeekChargeNodeByRentId(HOME_NODE* phead, char* rent_id)  //rent_id������
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
			pcar_node = pcar_node->next;   //û�ҵ���������
		}
		p = p->next;
	}
	return NULL;
}

/*
   �������ƣ�ShowTime
   �������ܣ���ʾʱ��
   �����������
   �������: ��
   ����ֵ����
   ����˵�����û�ά��������Ϣ����ʱ��ʱ����ʾ��ǰʱ��
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
	strftime(str, 100, "%Y��%m��%d�� %H:%M", lt);
	if (x)
	{
		printf("��ǰʱ�䣺%s\n", str);
	}
}

/*
   �������ƣ�time_test
   �������ܣ����ʱ���Ƿ�ϸ�
   ���������ʱ��
   �������:
   ����ֵ��int,1˵�������ʱ��ϸ�0��ʾ�����ʱ�䲻�ϸ�
   ����˵���������������Ҫ�����ں����޸Ķ�����Ϣʱ����Ҫ����ʱ����Ϣ������Ҫ�����ܶž�����ģ������߼���ʱ�����룬
            ����д��������������������жϳ����Ƿ���ʣ��پ��Ƿ����������ʱ��ȣ��ж����Ƿ�����߼�
*/
int time_test(char time[])  //������붩����ʱ���Ƿ���Ϲ淶
{
	if (strlen(time) != 16 && strlen(time) != 17 && strlen(time) != 10)
	{
		puts("δ���淶����");
		return 0;   //δ���淶����
	}
	char* str = func(time);
	if (strlen(time) != 12)
	{
		puts("δ��ȷִ��func");
		return 0;   //δ���淶����
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

		if (strcmp(day, "01") < 0 || strcmp(day, "31") > 0)   //���ﻹ���Ͻ�Ҫ���������31��30�ţ�ֻ�Ǹ���Ź���
			return 0;
		j = 0;

		while (i < 10)
		{
			hour[j] = str[i];
			j++;
			i++;
		}
		hour[j] = '\0';

		if (strcmp(hour, "00") < 0 || strcmp(hour, "23") > 0)   //���ﻹ���Ͻ�Ҫ���������31��30�ţ�ֻ�Ǹ���Ź���
			return 0;
		j = 0;

		while (i < 12)
		{
			minute[j] = str[i];
			j++;
			i++;
		}
		minute[j] = '\0';
		if (strcmp(minute, "00") < 0 || strcmp(minute, "59") > 0)   //���ﻹ���Ͻ�Ҫ���������31��30�ţ�ֻ�Ǹ���Ź���
			return 0;
		j = 0;

		break;
	}
	return 1;
}

/*
   �������ƣ�GetTime
   �������ܣ���õ�ǰʱ��
   �����������
   �������:
   ����ֵ���ַ���
   ����˵�������������Ϊ�˼������ʱ��ʱ�ж�ʱ�õģ��ܶ�ʱ����Ҫ��ȡ��ǰ��ʱ����бȽϣ�����ʵ�ʻ���ʱ�䲻���ܱȵ�ǰʱ����
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
   �������ƣ�leapyear
   �������ܣ������ж�
   �����������
   �������:
   ����ֵ��int,1��ʾΪ���꣬0��ʾ������
   ����˵��������ʱ���ʱ��
*/
int leapyear(int year)
{
	if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
		return 1;
	else
		return 0;
}

/*
   �������ƣ�compare
   �������ܣ������һ�����ڱȵڶ������ڴ󣬽�������
   �����������
   �������:
   ����ֵ��ûʲô��������
   ����˵����
*/
int compare(DATE_w* d1, DATE_w* d2)				//�����һ�����ڱȵڶ������ڴ󣬽�������
{
	DATE_w* tmp;
	if (d1->year == d2->year)				//�������
	{
		if (d1->month > d2->month)			//�������
		{
			tmp = d1;
			d1 = d2;
			d2 = d1;
		}
		else if (d1->month == d2->month)		//�������
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
   �������ƣ�diff
   �������ܣ���������ʱ��ṹ���������
   �������������ʱ��ṹ��ָ��
   �������:
   ����ֵ���������
   ����˵����
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
		for (i = date1->month + 1; i <= 12; i++)				//date1������ĩ������
		{
			diff += month[i];
		}
		diff += month[date1->month] - date1->day;
		if (date1->month <= 2)
			if (leapyear(date1->year))
				diff++;
		for (i = 1; i < date2->month; i++)					//date2�������������
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
   �������ƣ�diff_day
   �������ܣ�����������
   ���������������׼12λ�������ַ���ʱ��
   �������:
   ����ֵ��ʱ���
   ����˵���������ַ��������Ǳ�׼12λ��Ҳ���Ǿ���func�����������
*/
int diff_day(char time1[], char time2[])
{
	int days = 0;
	DATE_w day1, day2;
	DATE_w* date1, * date2;
	date1 = &day1;
	date2 = &day2;
	//��Ҫ��ʱ��ת���ṹ����
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
   �������ƣ�ChangeCarInfoToStr
   �������ܣ��������ڵ���Ϣת��Ϊһ���ַ���
   �������������������Ϣ�ڵ�ָ��
   �������: 
   ����ֵ����Ϣ�ַ���
   ����˵�����˺�����Ϊ�˱����ۺϲ���д��
*/
char* ChangeCarInfoToStr(CAR_NODE* pcarnode)   //������������Ϣת��Ϊ�ַ������ö��ŷָ�
{
	char* x = "��";
	char* str = (char*)malloc(sizeof(char) * 100);
	//char str[100] = "\0";
	str[0] = '\0';
	char num[5];
	char id[2] = "\0";
	char str1[10] = "�ѳ���";
	char str2[10] = "δ����";
	id[0] = pcarnode->car_id;
	itoa(pcarnode->number, num, 10);   //�ַ���ת��Ϊ����
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
   �������ƣ�satisfy
   �������ܣ��жϵ�ǰ���������ڵ��Ƿ������������
   �������������������Ϣ�ڵ�ָ�룬�����������mixsearch
   �������:
   ����ֵ��BOOL��true��ʾ���ϣ�false��ʾ������
   ����˵����mixsearch��ı䣬�ô˺���ǰҪ����mixsearch
*/
BOOL satisfy(CAR_NODE* pcarnode, char mixsearch[])   //mixsearch��ʾ�û�����Ĳ�ѯ����
{
	char str[100];
	strcpy(str, ChangeCarInfoToStr(pcarnode));   //�Ȱѵ�ǰ�ڵ����Ϣת��Ϊ�ַ���
	const char* delim = ", ��/";   //����������м��ǿո���Ӣ�ĵĶ��Ŷ�Ҫ�ֿ�
	char* token;
	token = strtok(mixsearch, delim);  //��ȡ��һ����Ϣ
	if (token == NULL)   //û����ȷ��������
		return FALSE;
	if (!strstr(str, token))  //��������������
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
		return TRUE;  //����
	}
}

/*
   �������ƣ�del_the_blank
   �������ܣ������⳵�������ʱ�䣬��/�Ϳո�,-����ɾ��
   ���������ʱ���ַ���
   �������: ��
   ����ֵ���������ַ���
   ����˵����
*/
char* del_the_blank(char* str)   //�����⳵�������ʱ�䣬��/�Ϳո�,-����ɾ��
{
	int i, j;
	for (i = j = 0; str[i] != '\0'; i++)
	{
		if (str[i] != ' ' && str[i] != '/' && str[i] != '-' && str[i] != ':' && str[i] != '��' && str[i] != ',' && str[i] != '��')
		{
			str[j++] = str[i];
		}
	}
	str[j] = '\0';
	return str;
}

/*
   �������ƣ�pad
   �������ܣ����Ϊ��׼ʱ��
   ���������ʱ���ַ���
   �������:
   ����ֵ�������ʱ���ַ���
   ����˵����
*/
char* pad(char* str)   //���Ϊ��׼ʱ��
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
   �������ƣ�func
   �������ܣ�ֱ�Ӷ���һ�������зָ������ַ��������Ϊ��׼12λʱ��  
   ���������ʱ���ַ���
   �������:
   ����ֵ����׼12λ������ʱ��
   ����˵����ǧ��ע�⣬�˺������ԭ�ַ��������Ե���ǰҪ�ȱ���
*/
char* func(char* str)   //ֱ�Ӷ���һ�������зָ������ַ��������Ϊ��׼12λʱ��  
{
	strcpy(str, del_the_blank(str));
	strcpy(str, pad(str));
	return str;
}

/*
   �������ƣ�SeekNode
   �������ܣ����ݿͻ���������
   ����������ͻ�ͳ����ͷָ���Լ��ͻ�����
   �������:
   ����ֵ��TOP_RENT_GUEST*
   ����˵����
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
   �������ƣ�countday
   �������ܣ����ݶ������⳵ʱ���ʵ�ʹ黹ʱ�䣬ָ����ݼ���������
   ����������������⳵ʱ���ʵ�ʹ黹ʱ�䣬ָ�����
   �������:
   ����ֵ���������
   ����˵�����˺�����ͳ��3��4��5�����úܴ�
*/
int countday(char time1[], char time2[], char year[5])  //year�ǵ�ǰ�������
{
	int res = 0;
	int d;
	char year1[5];

	if (strstr(time1, year))     //ƥ�����,����ǵ���Ķ���
	{
		if (strcmp(time2, "000000000000") != 0)   //�����Ѿ��黹
		{
			strncpy(year1, time2, 4);
			year1[4] = '\0';
			if (strcmp(year1, year) > 0)  //���������ˣ�����ڵ��꣬�黹����ָ�����
			{
				char m[9] = "\0";
				strcpy(m, year);
				strcat(m, "1231");
				m[8] = '\0';
				d = diff_day(time1, m);
				res += d;
			}
			else  //����������黹���ھ���ָ�������
			{
				d = diff_day(time1, time2);
				res += d;
			}
		}
	}

	//����ָ����������,��������ָ����ݻ���
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

//< ʱ������0.05s
inline void Delay(void)
{
	int goal;
	goal = 50 + clock();
	while (goal > clock());
}

/**< ���������posλ�ô�*/
inline void Gotoxy(COORD pos)
{
	gh_std_out = GetStdHandle(STD_OUTPUT_HANDLE);  /*��ȡ��׼����豸���*/
	SetConsoleCursorPosition(gh_std_out, pos);      /*���ù��λ����pos��*/
}

/**< ˢ������.*/
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

/**< �����˵�����������.*/
void SetMainHotArea(void)
{
	int i, j;
	char aSort[5] = { 0 };
	char aTag[5] = { 1,2,3,4,5 };
	HOT_AREA areas;
	SMALL_RECT aArea[5] = { {2,0,8,0},{12,0,20,0},{24,0,30,0},{34,0,40,0},{44,0,54,0} };/*�������˵���������Χ*/
	areas.pArea = aArea;   /*ʹ�����ṹ����areas�ĳ�ԱpAreaָ������λ��������Ԫ��*/
	areas.pSort = aSort;   /*ʹ�����ṹ����areas�ĳ�ԱpSortָ���������������Ԫ��*/
	areas.pTag = aTag;    /*ʹ�����ṹ����areas�ĳ�ԱpTag ָ���������������Ԫ��*/
	/*�������˵����������ַ���Ԫ���������ͺ��������*/
	for (i = 0; i < 5; i++)
	{
		for (j = areas.pArea[i].Left; j <= areas.pArea[i].Right; j++)
			gp_scr_att[j] |= (areas.pSort[i] << 6) | (areas.pTag[i] << 2);
	}
}

/*
   �������ƣ�SortRentMostGuestNode
   �������ܣ��ڿͻ�ͳ���жԳ���������������������
   ����������ͻ�ͳ����ͷָ���ַ
   �������:
   ����ֵ����
   ����˵����
*/
void SortRentMostGuestNode(TOP_RENT_GUEST** phead)    //�Գ���������������������
{
	TOP_RENT_GUEST* p, * q;
	TOP_RENT_GUEST pre;
	for (p = (*phead)->next; p != NULL; p = p->next)
		for (q = p->next; q != NULL; q = q->next)
			if (p->day < q->day)
			{
				//����������
				pre = *q;
				*q = *p;
				*p = pre;
				//����nextָ����
				pre.next = q->next;
				q->next = p->next;
				p->next = pre.next;
			}
	return;
}

/*
   �������ƣ�SortCarMostRentNode
   �������ܣ�����ݳ���������೵��ͳ���жԳ���������������������
   �������������ͳ����ͷָ���ַ
   �������:
   ����ֵ����
   ����˵����
*/
void SortCarMostRentNode(CAR_RENT** phead)    //�Գ���������������������
{
	CAR_RENT* p, * q;
	CAR_RENT pre;
	for (p = (*phead)->next; p != NULL; p = p->next)
		for (q = p->next; q != NULL; q = q->next)
			if (p->day < q->day)
			{
				//����������
				pre = *q;
				*q = *p;
				*p = pre;
				//����nextָ����
				pre.next = q->next;
				q->next = p->next;
				p->next = pre.next;
			}
	return;
}


//-----------------------------------------------------------------------------------------------------
//��ѯģ��
/*
   �������ƣ�QueryHomeInfo
   �������ܣ�����������Ϣ��ѯ
   �����������
   �������: ��
   ����ֵ��BOOL,��ΪTRUE
   ����˵����
*/
BOOL QueryHomeInfo(void)  //��ѯ������Ϣ
{
	char x;
	int choice;
	while (TRUE)
	{
		printf("������Ҫ���ֲ�ѯ��\n");
		printf("1.��׼��ѯ    2.ģ����ѯ    3.�ۺϲ�ѯ   ��������\n");
		scanf("%d", &choice);
		//getchar();

		if (choice < 1 || choice>3)
		{
			char x;
			printf("��������Ƿ��������룿y-�� n-��\n");
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
			case 1: printf("������Ҫ��ѯ�ĳ������\n");
				    char type[20];
					if (!scanf_s("%s", type, sizeof(char) * 20))
					{
						printf("������󣡼�����볤�ȣ��˴β�ѯʧ�ܣ�\n");
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
		printf("\n�����Ƿ������ѯ��\n");
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
			printf("\n���β�ѯ����---------��������˳�\n");
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
   �������ƣ�QueryHomeInfoNode1
   �������ܣ���׼��ѯ���Գ������Ϊ���������Ҳ���ʾ���������ĳ���������Ϣ��
   �������������ͷָ�룬�����������
   �������: ��
   ����ֵ����
   ����˵����
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
			printf("�ҵ�������͵Ŀ����޳�������Ϣ����:\n"); Delay();
			p2 = p1->snext;
			while (p2 != NULL)
			{
				if (p2->id == 'n')
				{
					count++;
					printf("\n");
					printf("���ƺ�      ��������      ��������        �ŵ���ʽ       ÿ�����\t\n"); Delay();
					printf("%s      %c           %-14s  %8s       %.2f\t\n", p2->car_num, p2->car_id, p2->car_name, p2->car_gear, p2->fee); Delay();
					printf("-------------------------------------------------------------------------------\n");
				}
				p2 = p2->next;
			}
		}
		p1 = p1->next;
	}
	if (count == 0)
		printf("û�ҵ������� ���� �����������޿��⳵��!\n");
}

/*
   �������ƣ�QueryHomeInfoNode2
   �������ܣ�ģ����ѯ����δָ�������������ƽ��в�ѯʱ��Ӧ��ʾ���е�ǰ�����޳�����Ϣ
   �������������ͷָ��
   �������: ��
   ����ֵ����
   ����˵����
*/
void QueryHomeInfoNode2(HOME_NODE* head)
{
	rewind(stdin);
	HOME_NODE* p1 = head->next;
	CAR_NODE* p2;
	char type[20];
	int count = 0;
	int flag = 0;  //�Ƿ��ҵ�������
	printf("�����복�����ؼ���\n");
	if (!scanf_s("%s", type, sizeof(char) * 20))
	{
		printf("������󣡼�����볤�ȣ��˴β�ѯʧ�ܣ�\n");
		return;
	}
	getchar();
	ScreenFresh();
	
	while (p1 != NULL)
	{
		if (strstr(p1->type, type))   //ģ������
		{
			flag = 1;
			printf("�ҵ������޳�������Ϣ����:\n"); Delay();
			p2 = p1->snext;
			while (p2 != NULL)
			{
				if (p2->id == 'n')
				{
					count++;
					printf("\n");
					printf("���ƺ�      ��������      ��������        �ŵ���ʽ       ÿ�����\t\n"); Delay();
					printf("%s      %c           %-14s  %8s       %.2f\t\n", p2->car_num, p2->car_id, p2->car_name, p2->car_gear, p2->fee); Delay();
					printf("-------------------------------------------------------------------------------\n");
				}
				p2 = p2->next;
			}
		}
		p1 = p1->next;
	}

	p1 = head->next;
	if (flag == 0)   //û����������
	{
		printf("\n������δ������Ч���ͣ��ҵ����п����޳�������Ϣ����:\n\n");
		while (p1 != NULL)
		{
			p2 = p1->snext;
			while (p2 != NULL)
			{
				if (p2->id == 'n')
				{
					count++;
					printf("\n");
					printf("���ƺ�      ��������      ��������        �ŵ���ʽ       ÿ�����\t\n"); Delay();
					printf("%s      %c           %-14s  %8s       %.2f\t\n", p2->car_num, p2->car_id, p2->car_name, p2->car_gear, p2->fee); Delay();
					printf("-------------------------------------------------------------------------------\n");
				}
				p2 = p2->next;
			}
			p1 = p1->next;
		}
		printf("������������Ϣģ����������!\n");
		return;
	}

	if (count == 0)
		printf("�Բ���û���ҵ���������������\n");
	printf("������������Ϣģ����������!\n");
	return;
}

/*
   �������ƣ�QueryHomeInfoNode3
   �������ܣ��ۺϲ�ѯ����ͬʱѡ�������ͺ;����ͽ��в�ѯʱ��Ӧ��ʾ�����೵�͵ĵ�ǰ�����޳�����Ϣ
   �������������ͷָ��
   �������: ��
   ����ֵ����
   ����˵����
*/
void QueryHomeInfoNode3(HOME_NODE* head)
{
	rewind(stdin);
	char n[50];
	printf("������Ҫ������Щ���͵ĳ���\n");
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
			     printf("�ҵ�������͵Ŀ����޳�������Ϣ����:\n"); Delay();
			p2 = p1->snext;
			while (p2 != NULL)
			{
				if (p2->id == 'n')
				{
					count++;
					printf("\n");
					printf("���ƺ�      ��������      ��������        �ŵ���ʽ       ÿ�����\t\n"); Delay();
					printf("%s      %c           %-14s  %8s       %.2f\t\n", p2->car_num, p2->car_id, p2->car_name, p2->car_gear, p2->fee); Delay();
					printf("-------------------------------------------------------------------------------\n");
				}
				p2 = p2->next;
			}
		}
		p1 = p1->next;
	}
	if (count == 0)
		printf("�Բ���û���ҵ���������������\n");
}

/*
   �������ƣ�QueryHomeInfo
   �������ܣ�����������Ϣ��ѯ
   �����������
   �������: ��
   ����ֵ��BOOL,��ΪTRUE
   ����˵����
*/
BOOL QueryCarInfo(void) //��ѯ����������Ϣ
{
	int choice;
	char x;
	while (TRUE)
	{
		printf("������Ҫ���ֲ�ѯ��\n");
		printf("1.���ƺŲ�ѯ    2.�������Ʋ�ѯ    3.����״̬��ѯ    4.���������    ��������\n");
		scanf("%d", &choice);
		getchar();
		
		if (choice < 1 || choice>4)
		{
			printf("��������Ƿ��������룿y-�� n-��\n");
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
		printf("\n�����Ƿ������ѯ��\n");
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
			printf("\n���β�ѯ����---------��������˳�\n");
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
   �������ƣ�QueryCarInfoNode1
   �������ܣ��Գ��ƺ���Ϊ���������Ҳ���ʾ���������ĳ���������Ϣ��֧��ģ����ѯ
   �������������ͷָ��
   �������: ��
   ����ֵ����
   ����˵����
*/
void QueryCarInfoNode1(HOME_NODE * head)
{
	rewind(stdin);

	printf("������Ҫ��ѯ�ĳ��ƺŻ��ƺŹؼ���\n");
	char car_num[10];
	if (!scanf_s("%s", car_num, sizeof(char) * 10))
	{
		printf("������󣡼�����볤�ȣ��˴β�ѯʧ�ܣ�\n");
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
					printf("�ҵ�������Ϣ����:\n"); Delay();
				printf("\n");
				printf("���ƺ�      ��������      ��������        �ŵ���ʽ       ÿ�����     �Ƿ��ѳ���\t\n"); Delay();
				printf("%s      %c           %-14s  %8s       %.2f        %c\t\n", p2->car_num,p2->car_id, p2->car_name, p2->car_gear, p2->fee,p2->id); Delay();
				printf("-------------------------------------------------------------------------------\n");
			}
			p2 = p2->next;
		}
		p1 = p1->next;
	}
	if (count == 0)
		printf("�Բ���û���ҵ���������������\n");
}

/*
   �������ƣ�QueryCarInfoNode2
   �������ܣ��Գ�������Ϊ���������Ҳ���ʾ���������ĳ���������Ϣ��֧��ģ����ѯ
   �������������ͷָ��
   �������: ��
   ����ֵ����
   ����˵����
*/
void QueryCarInfoNode2(HOME_NODE* head)
{
	printf("������Ҫ��ѯ�ĳ��������ؼ���\n");
	char car_name[20];
	getchar();
	if (!scanf_s("%s", car_name, sizeof(char) * 20))
	{
		printf("������󣡼�����볤�ȣ��˴β�ѯʧ�ܣ�\n");
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
					printf("�ҵ�������Ϣ����:\n"); Delay();
				printf("\n");
				printf("���ƺ�      ��������      ��������        �ŵ���ʽ       ÿ�����     �Ƿ��ѳ���\t\n"); Delay();
				printf("%s      %c           %-14s  %8s       %.2f        %c\t\n", p2->car_num, p2->car_id, p2->car_name, p2->car_gear, p2->fee, p2->id); Delay();
				printf("-------------------------------------------------------------------------------\n");
			}
			p2 = p2->next;
		}
		p1 = p1->next;
	}
	if (count == 0)
		printf("�Բ���û���ҵ���������������\n");
}

/*
   �������ƣ�QueryCarInfoNode3
   �������ܣ��Գ���״̬Ϊ���������Ҳ���ʾ���������ĳ���������Ϣ.���磬���Ҳ���ʾ��ǰδ���⳵��������Ϣ
   �������������ͷָ��
   �������: ��
   ����ֵ����
   ����˵����
*/
void QueryCarInfoNode3(HOME_NODE* head)
{
	HOME_NODE* p1 = head->next;
	CAR_NODE* p2;
	int count = 0;
	char id;
	printf("��������������� �ѳ���-y δ����-n\n");
	if (!scanf_s("%c", &id, sizeof(char)))
	{
		printf("��������Զ��˳���\n");
		return;
	}
	getchar();
	if(id!='y' && id!='n')
	{
		printf("��������Զ��˳���\n");
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
					printf("\n�ҵ�������Ϣ���£�\n"); Delay();
				printf("\n");
				printf("���ƺ�      ��������      ��������        �ŵ���ʽ       ÿ�����     �Ƿ��ѳ���\t\n"); Delay();
				printf("%s      %c           %-14s  %8s       %.2f        %c\t\n", p2->car_num, p2->car_id, p2->car_name, p2->car_gear, p2->fee, p2->id); Delay();
				printf("-------------------------------------------------------------------------------\n");
			}
			p2 = p2->next;
		}
		p1 = p1->next;
	}
	if (count == 0)
		printf("�Բ���û���ҵ���������������\n");
}

/*
   �������ƣ�QueryCarInfoNode4
   �������ܣ��Գ��ƺ��롢�������ơ�����״̬��������ϣ����Ҳ���ʾ���������ĳ���������Ϣ��
   �������������ͷָ��
   �������: ��
   ����ֵ����
   ����˵�����˲�ѯ����Ҫ������ʵ�ֹ��̼���Ʋ���
*/
void QueryCarInfoNode4(HOME_NODE* head)
{
	rewind(stdin);
	printf("����������������в��ң�������Ӣ�ʣ�δ���⣬����Ϊ��AW223Q�ȵ�\n");
	char mixsearch[100];
	fgets(mixsearch, 100, stdin);
	int i = 0;                          //����fgets���Ļس���
	while (mixsearch[i] != '\n')
		i++;
	mixsearch[i] = '\0';


	char save[100];   //����char* s��char s[100]���������ʱ��һ�֣�s=mixsearch����ô�����߾���ȫ�����
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
					printf("\n������������������Ϣ���£�\n");
				printf("\n");
				printf("���ƺ�      ��������      ��������        �ŵ���ʽ       ÿ�����     �Ƿ��ѳ���\t\n"); Delay();
				printf("%s      %c           %-14s  %8s       %.2f        %c\t\n", p2->car_num, p2->car_id, p2->car_name, p2->car_gear, p2->fee, p2->id); Delay();
				printf("-------------------------------------------------------------------------------\n");
			}

			strcpy(mixsearch, save);
			p2 = p2->next;
		}
		p1 = p1->next;
	}
	if (count == 0)
		printf("�Բ���û���ҵ���������������\n");
}

/*
   �������ƣ�QueryChargeInfo
   �������ܣ��⳵������ѯ
   �����������
   �������: ��
   ����ֵ��BOOL,��ΪTRUE
   ����˵����
*/
BOOL QueryChargeInfo(void)
{
	rewind(stdin);
	char x;
	int choice;
	while (TRUE)
	{
		printf("������Ҫ���ֲ�ѯ��\n");
		printf("1.������Ϣ��ѯ    2.������Ϣ��ѯ    3.ʱ�䷶Χ��ѯ        ��������\n");
		scanf("%d", &choice);
		getchar();
		if (choice < 1 || choice>4)
		{
			printf("��������Ƿ��������룿y-�� n-��\n");
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
		printf("\n�����Ƿ������ѯ��\n");
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
			printf("\n���β�ѯ����---------��������˳�\n");
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
   �������ƣ�QueryChargeInfoNode1
   �������ܣ����������֤��Ϊ���������Ҳ���ʾ���������Ŀ����⳵������Ϣ��Ϣ��
   �������������ͷָ��
   �������: ��
   ����ֵ����
   ����˵����
*/
void QueryChargeInfoNode1(HOME_NODE* head)   //���������֤�����ѯ
{
	rewind(stdin);
	HOME_NODE* p1 = head->next;
	CAR_NODE* p2;
	CHARGE_NODE* p3;
	int find = 0;
	char id[20];
	printf("���������֤��\n");
	if (!scanf_s("%s", id, sizeof(char) * 20))
	{
		printf("������󣡼�����볤�ȣ��˴β�ѯʧ�ܣ�\n");
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
				if (!strcmp(p3->guest_id, id))   //�Ҷ�Ӧ�Ķ���
				{
					find++;
					printf("�ҵ�����û���������Ϣ����:\n\n"); Delay();
				
					printf("�������:%s\n", p3->rent_id); Delay();
					printf("�������֤��:%s\n", p3->guest_id); Delay();
					printf("��������:%s\n", p3->name); Delay();
					printf("���ó������:%d\n", p3->number); Delay();
					printf("���˵绰:%s\n", p3->tel); Delay();
					printf("ȡ��ʱ��:%s\n", p3->time_of_get_car); Delay();
					printf("ԤԼ����ʱ��:%s\n", p3->time_of_scheduled_back); Delay();
					printf("ʵ�ʻ���ʱ��:%s\n", p3->time_of_actual_back); Delay();
					printf("Ѻ��:%.2f\n", p3->should_pay*5.0); Delay();
					printf("Ӧ�ɷ���:%.2f\n", p3->should_pay); Delay();
					printf("ʵ�ɷ���:%.2f\n", p3->actual_pay); Delay();
					printf("-------------------------------------------\n");
				}
				p3 = p3->next;
			}
			p2 = p2->next;
		}
		p1 = p1->next;
	}

	if (find == 0)
		printf("δ�ҵ����û�\n");
}

/*
   �������ƣ�QueryChargeInfoNode2
   �������ܣ����������ƺ���Ϊ���������Ҳ���ʾ�����������⳵������Ϣ��
   �������������ͷָ��
   �������: ��
   ����ֵ����
   ����˵����
*/
void QueryChargeInfoNode2(HOME_NODE* head)    //���������ƺ����ѯ
{
	rewind(stdin);
	
	char num[20];  //���ƺ�
	printf("��������ҵĳ��ƺ�\n\n");
	if (!scanf_s("%s", num, sizeof(char) * 20))
	{
		printf("������󣡼�����볤�ȣ��˴β�ѯʧ�ܣ�\n");
		return;
	}
	getchar();

	CAR_NODE* pcarnode = SeekCarNodeByCarNum(head, num);
	if (pcarnode == NULL) //���ҽڵ�ʧ��
	{
		printf("δ�ҵ��ó�����\n");
		return;
	}

	CHARGE_NODE* p = pcarnode->cnext;
	if(p!=NULL)
		printf("�ҵ��ó������ж������£�\n\n");

	while (p != NULL)
	{
		printf("�������:%s\n", p->rent_id); Delay();
		printf("�������֤��:%s\n", p->guest_id); Delay();
		printf("��������:%s\n", p->name); Delay();
		printf("���ó������:%d\n", p->number); Delay();
		printf("���˵绰:%s\n", p->tel); Delay();
		printf("ȡ��ʱ��:%s\n", p->time_of_get_car); Delay();
		printf("ԤԼ����ʱ��:%s\n", p->time_of_scheduled_back); Delay();
		printf("ʵ�ʻ���ʱ��:%s\n", p->time_of_actual_back); Delay();
		printf("Ѻ��:%.2f\n", p->should_pay * 5.0); Delay();
		printf("Ӧ�ɷ���:%.2f\n", p->should_pay); Delay();
		printf("ʵ�ɷ���:%.2f\n", p->actual_pay); Delay();
		printf("---------------------------------------\n");
		p = p->next;
	}
	printf("�������\n");
}

/*
   �������ƣ�QueryChargeInfoNode3
   �������ܣ����⳵ʱ�䷶ΧΪ���������Ҳ���ʾ���������������⳵������Ϣ
   �������������ͷָ��
   �������: ��
   ����ֵ����
   ����˵�����˲�������func�����Լ��ܶศ������
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
	char save_ptr[20];  //Ϊ�˷�ֹfunc���˽ṹ���������ȱ���

	printf("\n���ڿ�ʼ��ʱ�䷶Χ����\n");
	printf("-----------------------\n\n");
	printf("��������ʼʱ��:  ��2019/02/13\n");
	if (!scanf_s("%s", time_start, sizeof(char) * 20))
	{
		printf("������󣡼�����볤�ȣ��˴β�ѯʧ�ܣ�\n");
		return;
	}
	getchar();

	if (!time_test(time_start))
	{
		printf("�����ʽ�����Զ��˳���\n");
		return ;
	}

	printf("\n���������ʱ��:  ��2019/02/18\n");
	if (!scanf_s("%s", time_end, sizeof(char) * 20))
	{
		printf("������󣡼�����볤�ȣ��˴β�ѯʧ�ܣ�\n");
	}
	getchar();

	if (!time_test(time_end))
	{
		printf("�����ʽ�����Զ��˳���\n");
		return ;
	}

	if (strcmp(func(time_end), func(time_start)) < 0)  
	{
		printf("����ʱ�䲻�����߼���\n");
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
				strcpy(s , func(p3->time_of_get_car));   //ת��Ϊ��׼ʱ���ַ���
				if (strcmp(func(time_start), s) <= 0 && strcmp(func(time_end), s) >= 0)
				{
					find++;
					if(find==1)
					    printf("�ҵ���ض�������Ϣ����:\n"); Delay();
					printf("�������:%s\n", p3->rent_id); Delay();
					printf("�������֤��:%s\n", p3->guest_id); Delay();
					printf("��������:%s\n", p3->name); Delay();
					printf("���ó������:%d\n", p3->number); Delay();
					printf("���˵绰:%s\n", p3->tel); Delay();
					printf("ȡ��ʱ��:%s\n", p3->time_of_get_car); Delay();
					printf("ԤԼ����ʱ��:%s\n", p3->time_of_scheduled_back); Delay();
					printf("ʵ�ʻ���ʱ��:%s\n", p3->time_of_actual_back); Delay();
					printf("Ѻ��:%.2f\n", p3->should_pay * 5.0); Delay();
					printf("Ӧ�ɷ���:%.2f\n", p3->should_pay); Delay();
					printf("ʵ�ɷ���:%.2f\n", p3->actual_pay); Delay();
					printf("--------------------------------\n\n");
				}
				strcpy(p3->time_of_get_car, save_ptr); //��ԭ
				p3 = p3->next;
			}
			p2 = p2->next;
		}
		p1 = p1->next;
	}
	if (find == 0)
		printf("δ�ҵ���ض���\n");
}


//-----------------------------------------------------------------------------------------
//ͳ��       
//ͳ��1
/*
   �������ƣ�Stat_Car_Type_Rent_Num
   �������ܣ�ͳ�Ƹ������͵ĳ���������δ��������
   �����������
   �������: ��
   ����ֵ����
   ����˵������ֻ�Ǹ���������Ҫ�������������Ӻ���ʵ��
*/
BOOL Stat_Car_Type_Rent_Num(void)    //���ͳ�������ͳ�� ֱ���г����е�ǰ���͵�
{
	RENT_STATUS* pheadstat1 = (RENT_STATUS*)malloc(sizeof(RENT_STATUS));   //ͷ�ڵ�
	pheadstat1->next = NULL;
	ChangeCarNumber(&gp_head);
	Stat_Car_Type_Rent_Num_Info(gp_head,&pheadstat1);
	SaveStat1Data(pheadstat1);
	rewind(stdin);
	printf("�����������\n");
	getchar();
	ClearScreen();
	ShowMenu();
	FreeSpace1(pheadstat1);
	return TRUE;
}

/*
   �������ƣ�Stat_Car_Type_Rent_Num_Info
   �������ܣ�ͳ�Ƹ������͵ĳ���������δ�������������нڵ����
   �������������ͷָ�룬ͳ��1����ͷָ���ַ
   �������: ��
   ����ֵ����
   ����˵�����˺���ʵ��ͳ��1����Ľ������ڵ�Ĳ����
*/
void Stat_Car_Type_Rent_Num_Info(HOME_NODE* phead,RENT_STATUS** phead1)
{

	HOME_NODE* p1 = phead->next;
	CAR_NODE* p2;
	RENT_STATUS* ptail = *phead1;   //�Ƚ��ȳ�
	ptail->next = NULL;
	while (p1 != NULL)
	{
		RENT_STATUS* ptemp = (RENT_STATUS*)malloc(sizeof(RENT_STATUS));   //�ȳ�ʼ��
		strcpy(ptemp->type, p1->type);
		ptemp->total_num = p1->num;   //��������
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
   �������ƣ�FreeSpace1
   �������ܣ��ͷ�ͳ��1����ռ�
   ���������ͳ��1����ͷָ��
   �������: ��
   ����ֵ����
   ����˵����
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
   �������ƣ�SaveStat1Data
   �������ܣ���ͳ��1����д���ļ�
   ���������ͳ��1����ͷָ��
   �������: ��
   ����ֵ����
   ����˵����
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
	strftime(str, 100, "%Y��%m��%d��", lt);

	FILE* fout1;
	RENT_STATUS* p1 = (RENT_STATUS*)malloc(sizeof(RENT_STATUS));
	p1 = phead->next;
	if ((fout1 = fopen(StatFileName[0], "wb")) == NULL)
		return FALSE;
	fprintf(fout1, "%s %s,\n",str,"����������Ϣͳ��");
	fprintf(fout1, "%s,%s,%s,%s,\n","��������","��������", "�ѳ�����", "δ������");
	while (p1 != NULL)
	{
		fprintf(fout1, "%s,%d,%d,%d,\n", p1->type,p1->total_num, p1->rent_num, p1->total_num-p1->rent_num);
		p1 = p1->next;
	}
	fclose(fout1);
	printf("��������ѱ��浽�ļ�!\n");
	return TRUE;
}

//ͳ��2
/*
   �������ƣ�Stat_Car_Type_Month_Fee
   �������ܣ�ͳ�Ƹ������͵���Ӫҵ��
   �����������
   �������: ��
   ����ֵ����
   ����˵������ֻ�Ǹ���������Ҫ�������������Ӻ���ʵ��
*/
BOOL Stat_Car_Type_Month_Fee(void)   //������Ӫҵ��ͳ��
{
	CAR_FEE* pheadstat = (CAR_FEE*)malloc(sizeof(CAR_FEE));   //ͷ�ڵ�
	pheadstat->next = NULL;
	char year[5];
	rewind(stdin);
	printf("��������Ҫ���ҵ����\n");
	if (!scanf_s("%s", year, sizeof(char) * 5))
	{
		printf("��������Զ��˳���\n");
		rewind(stdin);
		printf("�����������\n");
		getchar();
		ClearScreen();
		ShowMenu();
		return TRUE;
	}

	Stat_Car_Type_Month_Fee_Info(gp_head, &pheadstat, year);
	SaveStat2Data(pheadstat, year);
	rewind(stdin);
	printf("�����������\n");
	getchar();
	ClearScreen();
	ShowMenu();
	FreeSpace2(pheadstat);
	return TRUE;
}

/*
   �������ƣ�Stat_Car_Type_Month_Fee_Info
   �������ܣ�ͳ�Ƹ������͵���Ӫҵ��
   �������������ͷָ�룬ͳ��2����ͷָ���ַ����Ҫͳ�Ƶ����
   �������: ��
   ����ֵ����
   ����˵�����˺���ʵ��ͳ��2����Ľ������ڵ�Ĳ����
*/
void Stat_Car_Type_Month_Fee_Info(HOME_NODE* phead, CAR_FEE** phead1, char year[5])
{
	//char s1[18];
	//char s2[18];
	int d;
	HOME_NODE* p1 = phead->next;
	CAR_NODE* p2;
	CHARGE_NODE* p3;
	CAR_FEE* ptail = *phead1;   //�Ƚ��ȳ�
	ptail->next = NULL;

	while (p1 != NULL)   //�Գ�������ͳ�ƣ�ÿ��Ҫ����12���µ�Ӫҵ�ÿ�����Ͷ�Ӧһ��CAR_FEE�ڵ�
	{
		CAR_FEE* ptemp = (CAR_FEE*)malloc(sizeof(CAR_FEE));   //�ȳ�ʼ��
		strcpy(ptemp->type, p1->type);
		int i = 0;
		while (i < 12)
			 ptemp->money[i++] = 0.0;
		ptemp->next = NULL;

		p2 = p1->snext;
		while (p2 != NULL)
		{
			p3 = p2->cnext;
			while (p3 != NULL)    //Ѱ�ҵ�ǰ���������������������Ķ���
			{
				//strcpy(s1, p3->time_of_get_car);
				//strcpy(s2, p3->time_of_actual_back);
				if (strstr(p3->time_of_get_car, year))  //ƥ�����,����ǵ���Ķ���
				{
					int month1, month2;
					char month1_str[3];  //�Ӷ�����Ϣ��ȡ�·�
					char month2_str[3];

					strncpy(month1_str, p3->time_of_get_car+5, 2);
					month1_str[2] = '\0';
					month1 = atoi(month1_str);
					
					if (p3->time_of_actual_back != "0")  //�Ѿ��黹
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
					//û�黹����
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
   �������ƣ�SaveStat2Data
   �������ܣ���ͳ��2����д���ļ�
   ���������ͳ��2����ͷָ��
   �������: ��
   ����ֵ����
   ����˵����
*/
BOOL SaveStat2Data(CAR_FEE* phead,char year[5])
{
	FILE* fout1;
	CAR_FEE* p1 = (CAR_FEE*)malloc(sizeof(CAR_FEE));
	p1 = phead->next;
	if ((fout1 = fopen(StatFileName[1], "wb")) == NULL)
		return FALSE;

	fprintf(fout1, "                                                                                               %s%s,\n", year, "����Ӫҵ��ͳ��");

	fprintf(fout1, "%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,\n", "", "1��", "2��", "3��", "4��", "5��", "6��", "7��", "8��", "9��", "10��", "11��", "12��");
	while (p1 != NULL)
	{
		fprintf(fout1, "%s,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,\n", p1->type, 
			p1->money[0], p1->money[1], p1->money[2], p1->money[3], p1->money[4], p1->money[5], 
			p1->money[6], p1->money[7], p1->money[8], p1->money[9],p1->money[10], p1->money[11]);
		p1 = p1->next;
	}
	fclose(fout1);
	printf("��������ѱ��浽�ļ�!\n");
	return TRUE;
}

/*
   �������ƣ�FreeSpace2
   �������ܣ��ͷ�ͳ��2����ռ�
   ���������ͳ��2����ͷָ��
   �������: ��
   ����ֵ����
   ����˵����
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

//ͳ��3
/*
   �������ƣ�Stat_Car_Fee_and_rent_rate
   �������ܣ�ͳ�Ƹ�����������Ӫҵ��ͳ�����
   �����������
   �������: ��
   ����ֵ����
   ����˵������ֻ�Ǹ���������Ҫ�������������Ӻ���ʵ��
*/
BOOL Stat_Car_Fee_and_rent_rate(void)    //��Ӫҵ�������ͳ��
{
	CAR_RENT* pheadstat = (CAR_RENT*)malloc(sizeof(CAR_RENT));   //ͷ�ڵ�
	pheadstat->next = NULL;
	char year[5];
	rewind(stdin);
	printf("��������Ҫ���ҵ����\n");
	if (!scanf_s("%s", year, sizeof(char) * 5))
	{
		printf("��������Զ��˳���\n");
		rewind(stdin);
		printf("�����������\n");
		getchar();
		ClearScreen();
		ShowMenu();
		return TRUE;
	}
	Stat_Car_Fee_and_rent_rate_Info(gp_head, &pheadstat, year);
	SaveStat3Data(pheadstat,year);

	rewind(stdin);
	printf("�����������\n");
	getchar();
	ClearScreen();
	ShowMenu();
	FreeSpace3(pheadstat);
	return TRUE;
}

/*
   �������ƣ�Stat_Car_Fee_and_rent_rate_Info
   �������ܣ�ͳ�Ƹ�����������Ӫҵ��ͳ�����
   �������������ͷָ�룬ͳ��3����ͷָ���ַ����Ҫͳ�Ƶ����
   �������: ��
   ����ֵ����
   ����˵�����˺���ʵ��ͳ��3����Ľ������ڵ�Ĳ����
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
		printf("����������\n");
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
	CAR_RENT* ptail = *phead1;   //�Ƚ��ȳ�
	ptail->next = NULL;

	while (p1 != NULL)
	{
		p2 = p1->snext;
		while (p2 != NULL)
		{
			CAR_RENT* ptemp = (CAR_RENT*)malloc(sizeof(CAR_RENT));   //�ȳ�ʼ��
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
   �������ƣ�SaveStat3Data
   �������ܣ���ͳ��3����д���ļ�
   ���������ͳ��3����ͷָ��
   �������: ��
   ����ֵ����
   ����˵����
*/
BOOL SaveStat3Data(CAR_RENT* phead, char year[5])
{
	FILE* fout1;
	CAR_RENT* p1 = (CAR_RENT*)malloc(sizeof(CAR_RENT));
	p1 = phead->next;
	if ((fout1 = fopen(StatFileName[2], "wb")) == NULL)
		return FALSE;

	fprintf(fout1, "            %s%s,\n", year,"�공������ͳ��");
	fprintf(fout1, "%s,%s,%s,%s,%s,\n", "���ƺ�", "��������", "��Ӫҵ��", "������", "��������");
	while (p1 != NULL)
	{
		fprintf(fout1, "%s,%s,%.2f,%.2f,%d,\n", p1->car_num, p1->car_name, p1->money, p1->rent_rate,p1->day);
		p1 = p1->next;
	}
	fclose(fout1);
	printf("��������ѱ��浽�ļ�!\n");
	return TRUE;
}

/*
   �������ƣ�FreeSpace3
   �������ܣ��ͷ�ͳ��3����ռ�
   ���������ͳ��3����ͷָ��
   �������: ��
   ����ֵ����
   ����˵����
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

//ͳ��4
/*
   �������ƣ�Stat_Car_Most_Rent
   �������ܣ�ͳ������⳵�����top10
   �����������
   �������: ��
   ����ֵ����
   ����˵������ֻ�Ǹ���������Ҫ�������������Ӻ���ʵ��
*/
BOOL Stat_Car_Most_Rent(void)  //�����⳵��ͳ��
{
	CAR_RENT* pheadstat = (CAR_RENT*)malloc(sizeof(CAR_RENT));   //ͷ�ڵ�
	//CAR_RENT* p = pheadstat;
	pheadstat->next = NULL;
	char year[5];
	rewind(stdin);
	printf("��������Ҫ���ҵ����\n");
	if (!scanf_s("%s", year, sizeof(char) * 5))
	{
		printf("��������Զ��˳���\n");
		rewind(stdin);
		printf("�����������\n");
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
	printf("�����������\n");
	getchar();
	ClearScreen();
	ShowMenu();
	FreeSpace3(pheadstat);
	return TRUE;
}

/*
   �������ƣ�SaveStat4Data
   �������ܣ���ͳ��4����д���ļ�
   ���������ͳ��4����ͷָ��
   �������: ��
   ����ֵ����
   ����˵����
*/
BOOL SaveStat4Data(CAR_RENT* phead, char year[5])
{
	
	int i = 1;
	FILE* fout1;
	CAR_RENT* p1 = (CAR_RENT*)malloc(sizeof(CAR_RENT));
	p1 = phead->next;
	if ((fout1 = fopen(StatFileName[3], "wb")) == NULL)
		return FALSE;

	fprintf(fout1, "                %s%s,\n", year, "�������⳵��Top10");
	fprintf(fout1, "%s,%s,%s,%s,\n", "���ƺ�", "��������", "������", "��������");
	while (p1 != NULL)
	{
		i++;
		if (i > 11)  //ֻͳ��top10
			break;
		fprintf(fout1, "%s,%s,%.2f,%d,\n", p1->car_num, p1->car_name, p1->rent_rate, p1->day);
		p1 = p1->next;
	}
	fclose(fout1);
	printf("��������ѱ��浽�ļ�!\n");
	return TRUE;
}

/*
   �������ƣ�Stat_Car_Most_Rent_Info
   �������ܣ�ͳ������⳵�����top10
   �������������ͷָ�룬ͳ��4����ͷָ���ַ����Ҫͳ�Ƶ����
   �������: ��
   ����ֵ����
   ����˵�����˺���ʵ��ͳ��4����Ľ������ڵ�Ĳ����
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
	CAR_RENT* ptail = *phead1;   //�Ƚ��ȳ�
	ptail->next = NULL;

	while (p1 != NULL)
	{
		p2 = p1->snext;
		while (p2 != NULL)
		{
			CAR_RENT* ptemp = (CAR_RENT*)malloc(sizeof(CAR_RENT));   //�ȳ�ʼ��
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

//ͳ��5
/*
   �������ƣ�Stat_Guest_Most_Rent
   �������ܣ�ͳ�����⳵����û�top5
   �����������
   �������: ��
   ����ֵ����
   ����˵������ֻ�Ǹ���������Ҫ�������������Ӻ���ʵ��
*/
BOOL Stat_Guest_Most_Rent(void)  //��⳵�ͻ�ͳ��
{
	TOP_RENT_GUEST* pheadstat = (TOP_RENT_GUEST*)malloc(sizeof(TOP_RENT_GUEST));   //ͷ�ڵ�
	pheadstat->next = NULL;
	char year[5];
	rewind(stdin);
	printf("��������Ҫ���ҵ����\n");
	if (!scanf_s("%s", year, sizeof(char) * 5))
	{
		printf("��������Զ��˳���\n");
		rewind(stdin);
		printf("�����������\n");
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
	printf("�����������\n");
	getchar();
	ClearScreen();
	ShowMenu();
	return TRUE;
}

/*
   �������ƣ�SaveStat5Data
   �������ܣ���ͳ��5����д���ļ�
   ���������ͳ��5����ͷָ��
   �������: ��
   ����ֵ����
   ����˵����
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

	fprintf(fout1, "%s%s,\n", year, "���⳵����û�");
	fprintf(fout1, "%s,%s,\n", "�ͻ�", "�⳵����");

	while (p1 != NULL)
	{
		i++;
		if (i > 6)  //ֻͳ��top5
			break;
		fprintf(fout1, "%s,%d,\n", p1->guest_name, p1->day);
		p1 = p1->next;
	}

	fclose(fout1);
	printf("��������ѱ��浽�ļ�!\n");
	return TRUE;
}

/*
   �������ƣ�FreeSpace5
   �������ܣ��ͷ�ͳ��5����ռ�
   ���������ͳ��5����ͷָ��
   �������: ��
   ����ֵ����
   ����˵����
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
   �������ƣ�Stat_Guest_Most_Rent_Info
   �������ܣ�ͳ�����⳵����û�top5
   �������������ͷָ�룬ͳ��5����ͷָ���ַ����Ҫͳ�Ƶ����
   �������: ��
   ����ֵ����
   ����˵�����˺���ʵ��ͳ��5����Ľ������ڵ�Ĳ����
*/
void Stat_Guest_Most_Rent_Info(HOME_NODE* phead, TOP_RENT_GUEST** phead1, char year[5])
{
	int d;
	char s1[18];
	char s2[18];
	HOME_NODE* p1 = phead->next;
	CAR_NODE* p2;
	CHARGE_NODE* p3;
	TOP_RENT_GUEST* ptail = *phead1;   //�Ƚ��ȳ�
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
				if (strstr(p3->time_of_get_car, year))  //ƥ�����,����ǵ���Ķ���
				{

					TOP_RENT_GUEST* p = SeekNode(*phead1, p3->name);
					if (p == NULL)   //�ҵ�һ�����û�
					{
						TOP_RENT_GUEST* ptemp = (TOP_RENT_GUEST*)malloc(sizeof(TOP_RENT_GUEST));   //�ȳ�ʼ��
						strcpy(ptemp->guest_name, p3->name);
						ptemp->day = 0;
						ptemp->next = NULL;
		
						d = countday(func(p3->time_of_get_car), func(p3->time_of_actual_back),year);
						ptemp->day += d;
						
						ptail->next = ptemp;
						ptail = ptemp;
					}

					else  //���û��Ѵ���
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
//����
/*
   �������ƣ�HelpTopic
   �������ܣ�ϵͳ����
   �����������
   �������: ��
   ����ֵ����
   ����˵����
*/
BOOL HelpTopic(void) //��������
{
	printf("��ϵͳ����ʹ�� ALT+F/E/Q/S, F1��������ȿ���\n");
	rewind(stdin);
	printf("\n�����������\n");
	getchar();
	ClearScreen();
	ShowMenu();
	return TRUE;
}

/*
   �������ƣ�AboutSys
   �������ܣ�����ϵͳ
   �����������
   �������: ��
   ����ֵ����
   ����˵����
*/
BOOL AboutSys(void)  //����ϵͳ
{
	printf("\n\n2020-06-08 ���пƼ���ѧ ����ռ䰲ȫѧԺ 2018�� C���Կγ����\n\n");
	printf("�Ű�1804 ��ƽ�� U201811237\n\n");
	rewind(stdin);
	printf("\n�����������\n");
	getchar();
	ClearScreen();
	ShowMenu();
	return TRUE;
}

