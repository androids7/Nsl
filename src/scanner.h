#ifndef __NSL_SCANNER_H__
#define __NSL_SCANNER_H__

#include <string.h>

#define token_get_name(token) (token_strings[token-T_GLOBAL])

#define BUFFER_SIZE 4096
#define TOKEN_SIZE_MAX 100

//���ﶨ��һЩ���ͣ���Ҫ�ǹؼ��ֺ�һЩ��ص����ͣ������������
typedef enum  _TokenType {
	T_GLOBAL = 257,
	T_ID, //��ʾ��ĸ��ʼ�����ֻ��ַ�������������ʾ��������������
	T_NUMBER, //��ʾ�κ����ֳ���
	T_IF,T_ELSE,T_ENDIF,//��ʾif else,endif
	T_FOR,T_WHILE,T_SWITCH,T_CASE,T_BREAK,T_CONTINUE,T_GOTO,T_DO,//��ʾswitch
	T_UNSIGNED,		//��ʾ�ַ�������� unsigned ���Զ����޷�������
	T_ENUM,T_TYPEDEF,T_STRUCT,//����ṹ�������
    T_BIT_AND,T_BIT_OR,T_BIT_XOR,T_LT,T_LE,T_GT,T_GE,T_EQ,T_NEQ,T_AND,T_OR,T_NOT,//��ʾ�����
	T_COLON,T_SEMI,T_POUND,T_LC,T_RC,T_LP,T_RP,T_COMMA,	   //��ʾ���ַָ���#{}();:<>\\''"%&
	T_ASSIGN,T_PLUS,T_MINUS,T_MUL,T_DIV,T_MOD,//�����Ҫ֧�֣������������
	T_INCLUDE,T_DEFINE,//��ʾһЩͷ�ļ���һЩ�̶�ֵ�Ķ���,include,define
	T_LITERAL,//��ʾ����˫�����м����������
	T_ERROR,//��ʾ����ʱ������
	//������C--��չ�й��е�һЩ�ؼ���
	T_STATE,//��ʾ״̬
	T_SUB,
	T_CTS, //��ʾ�ӿͻ��˵���������
	T_STC, //��ʾ�ӷ������˵��ͻ���
	T_START,T_END,//��ʾ״̬��ʼ��״̬����
	T_REGEX,//����
	T_DONE,
    T_DOLLAR,//����$��δ����,���$proto�����α�ʾ
	T_COMMENT//ȡ��ע��
}TokenType;

extern const char *token_strings[];

typedef struct _Token
{
	TokenType t; //���token������
	char * c;//ָ���λ�ã���������֣���ָ����ַ���Ϊ�������ͣ������Ҫ����ת��Ϊ��Ӧ������

}Token;

//ScannerState;��¼��ǰscanner��״̬,���ȱ�������״̬�������Ҫ��Ȼ���ټ�
typedef enum {
ST_START,
ST_ID, //����������ʾ����������
ST_ERROR,
ST_DONE,
ST_COMMENT,
ST_NUMBER,
ST_STRING,
ST_EQ,
ST_LE,
ST_GE,
ST_AND,
ST_OR,
}ScannerState;

//һ����¼�����Ϣ�Ľṹ
typedef struct {
	char *input; //ָ�򻺳���
	int linecount;//��¼�ڼ���
	int length;//��¼����,����ļ����ܳ���
	int index;//��¼�ڼ���������
}scanner_rc_t;

typedef struct  
{
	TokenType t;
	const char *reserved_name;
}ReservedKeyword;


TokenType lookup_reserved_keyword(const char *str);
//some function we need to do
//���ļ�����ɨ��,��ȡ�ļ���Ϣ
int scanner_open_file(scanner_rc_t *rc, const char *filename);

//������ػ�����
int scanner_set_input();
//ͨ�������ַ�����������ػ�����
int scanner_set_input_from_string();

//����һ�������ַ����ж�
TokenType scanner_get_one_char_token(scanner_rc_t *rc,char c);


//�õ�һ���ַ��Ĳ���
Token scanner_get_next_token(scanner_rc_t *);

void scanner_get_error(scanner_rc_t *);

int scanner_get_curr_line(scanner_rc_t *rc);

void scanner_debug(scanner_rc_t *rc);



#endif
