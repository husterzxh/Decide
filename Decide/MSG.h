#pragma once
const int MSG_LOGIN = 0;	//����Ⱥ��
const int MSG_VERSION = 1;	//�汾��Ϣ
const int MSG_REQUEST = 2;		//����IP�Լ������˿���Ϣ
const int MSG_LIST = 3;		//�û��б�
const int MSG_VOTE = 4;		//ͶƱ��Ϣ

// IP�������˿���Ϣ
typedef struct ipinfo {
	char ip[20];
	int port;
}IPINFO;

//��Ϣͷ
typedef struct msg {
	int type;
	int length;
	IPINFO ipinfo;
}MSGHEAD;



//����ͶƱ���ɼ�Ⱥ�е�ĳһ���ڵ㷢��ͶƱ����������ʹ𰸣����͸���Ⱥ�е����нڵ㣨��������ͶƱ�Ľڵ㣩
typedef struct lchvote
{
	char question[100];  //��ҪͶƱ����ѡ�������
	char answer1[50];  //ѡ��1
	char answer2[50];  //ѡ��2
}LCHVOTE;

//����ѡ��ÿ���ڵ����ѡ��һ���ڵ㣨�ڵ㲻���ظ�)�����Լ���ѡ��
typedef struct choice
{
	int answer;
	int flag;  //��ʶ����һ����ͨ��ѡ��������ѡ�����flagΪ1��˵���ýڵ��Ѿ��յ���2f+1����ͬ��ѡ��,���������ս��
	int count;  //���ս�������Ľڵ���������������ս������ʼ��Ϊ0�������ս�������Ľڵ����ﵽ��ֵʱ��ֹͣ����
}CHOICE;

//ÿ���ڵ�ά��һ���ṹ����¼�����յ��������ڵ��ѡ�񣬵��յ�ѡ��Ϊ2f+1��ʱ�����ݶ����ڵ��ѡ��ı��Լ���ѡ��
typedef struct record
{
	int count1;  //�ýڵ��յ������ڵ��ѡ���У�ѡ��ѡ��1�Ľڵ����
	int count2;  //�ýڵ��յ������ڵ��ѡ���У�ѡ��ѡ��2�Ľڵ����
}RECORD;