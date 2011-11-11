/*
 * C-- ��ƣ�һ��C���Ե��Ӽ��ű���ʵ��C���ԵĻ����򵥵Ĺ���
 * created by bingxuefenggu@126.com
 */
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"
#include <ctype.h>



//���ݶ������ؼ��֣��������Ҫ���ټ�
ReservedKeyword reserved_keywords[] = 
{
	{T_IF,"if"},
	{T_ELSE,"else"},
	{T_ENDIF,"endif"},
	{T_FOR,"for"},
	{T_WHILE,"while"},
	{T_SWITCH,"switch"},
	{T_CASE,"case"},
	{T_DO,"do"},
	{T_BREAK,"break"},
	{T_CONTINUE,"continue"},
	{T_GOTO,"goto"},
	{T_UNSIGNED,"unsigned"},
	{T_ENUM,"enum"},
	{T_TYPEDEF,"typedef"},
	{T_STRUCT,"struct"},
	{T_INCLUDE,"include"},
	{T_DEFINE,"define"},
	{T_STATE,"state"},
	{T_CTS,"cts"},
	{T_STC,"stc"},
	{T_START,"start"},
	{T_END,"end"},
};


// ��ȡ�ļ�������ȡ������,������Ϣ���浽scanner_rc_t��
int scanner_open_file(scanner_rc_t *rc, const char *filename)
{
	if (rc == NULL)
	{
		return -1;
	}
	memset(rc,0,sizeof(scanner_rc_t));
	
	FILE *fp = fopen(filename,"rb");
	if (fp == NULL) {
		return -1;
	}
	fseek(fp,0L,SEEK_END);
	rc->length = ftell(fp);//�õ������ļ��ĳ���
	rewind(fp);
	rc->input = (char *)malloc(rc->length + 1);//ֱ������һ�黺����������б��档
	memset(rc->input, 0 ,rc->length +1);
	int i = 0;
	while((rc->input[i] = fgetc(fp)) != EOF)
	{
		++i;
	}
	rc->input[i] = 0;
	fclose(fp);
	return 0;
}

//ֱ�Ӵ�string�ж�ȡ���ݲ�
int scanner_set_input_from_string(scanner_rc_t *rc,const char *str,int len)
{
	if (rc == NULL || len <= 0 || str == NULL)
	{
		return -1;
	}
	rc->input = (char *)malloc(len+1);
	memset(rc->input,0,len+1);
	memcpy(rc->input,str,len);
	rc->length = len;
	return 0;
}
//���ݵ����ַ����ж��Ƿ��������

TokenType scanner_get_one_char_token(scanner_rc_t *rc,char c)
{
	//�������ж�:�ŵ�ʱ��
	switch(c) {

		case ':': return T_COLON; //
		case ';': return T_SEMI; //
		case '#': return T_POUND;//
		case '{': return T_LC;//
		case '}': return T_RC;//
		case '(': return T_LP;
		case ')': return T_RP;
		case ',': return T_COMMA;
		case '$': return T_DOLLAR;
		case '\0': return T_DONE;
		//case '*': return ; ��������ǳ˺ţ�Ҳ������ָ�롣

		default:
			return T_ERROR;
	}
}

//�õ����Token�Ľ��
Token scanner_get_next_token(scanner_rc_t *rc)
{
	//
	Token token = {T_ERROR,NULL};
	if (rc == NULL) return token;
	ScannerState state = ST_START;
	char c = rc->input[rc->index];
	char buf[TOKEN_SIZE_MAX] = {0};
	int bufindex = 0;

	//delete the space 
	while(c == ' ' || c == '\t' || c == '\r' || c == '\n')
	{
		if (c == '\n') ++rc->linecount;
		++rc->index;
		c = rc->input[rc->index];
	}

	//�ж��Ƿ�Ϊ��ĸ�������ֻ��Ǳ������
	if (isalpha(c)){
		state = ST_ID;
		buf[bufindex++] = c;
		token.t = T_ID;
	} else if (isdigit(c))
	{
		state = ST_NUMBER;
		buf[bufindex++] = c;
		token.t = T_NUMBER;
	} else if (c == '\'' || c == '"')
	{
		state = ST_STRING;
		buf[bufindex++] = c;
		token.t = T_LITERAL;
	} else if (c == '/'){
		state = ST_COMMENT; //������Ϊ�����۵Ŀ�ʼ
		buf[bufindex++] = c;	
	} else if (c == '=')
	{
		state = ST_EQ; //Ҳ�п����Ǹ��ơ�
	} else if (c == '<')
	{
		state = ST_LE; //������<,Ҳ�п�����<=
	} else if (c == '>')
	{
		state = ST_GE; //�����Ǵ��ڣ�Ҳ�п�����>=
	} else if (c == '&')
	{
		state = ST_AND; //������&,Ҳ������ &&
	} else if (c == '|')
	{
		state = ST_OR;//������|,Ҳ������||
	} else {
		state = ST_DONE;
		token.t = scanner_get_one_char_token(rc,c);
	}
	++rc->index;
	while (state != ST_DONE)
	{
		c = rc->input[rc->index];
		switch(state) {
			case ST_ID:
				if (isalpha(c) || c == '_')
				{
					buf[bufindex++] = c;
				} 
				else {
					state = ST_DONE;
					-- rc->index;
				}
				break;
			case ST_NUMBER:
				if (isdigit(c) || c == '.' || (c == 'x' || c == 'X') &&(buf[bufindex-1] == '0'))
				{
					buf[bufindex++] = c;
				} else {
					state = ST_DONE;
					-- rc->index;
				}
				break;
			case ST_EQ:
				if (c == '=')
				{
					state = ST_DONE;
					token.t = T_EQ; //���ڲ���
				} else {
					state = ST_DONE;
					token.t = T_ASSIGN; //���Ʋ���
				}
				break;
			case ST_LE:
				if (c == '=')
				{
					//>=
				}
			case ST_COMMENT:
				if (c == '/')
				{
					// //����ע����ʽ
					bufindex = 0;
					for (int i = rc->index; i < rc->length; ++i)
					{
						if (rc->input[i] == '\n' || rc->input[i] == 0)
						{
							--rc->index; //���˵������˳�ѭ��
							break;
						} else {
							++rc->index; //�����������һ��
						}
					}
				} else if (c == '*')
				{
					// /* */��������comment
					while(rc->index < rc->length)
					{ 
						if (rc->input[rc->index] == '/' && rc->input[rc->index -1] == '*')
						{
							break;
						}
						if (rc->input[rc->index] == '\n')
						{
							rc->linecount ++;
						}
						++rc->index;
					}

				} else {
					// �п�����div��������
					token.t = T_DIV;
					--rc->index;
				}
				state = ST_DONE;
				break;
			default:
				state = ST_DONE;
				token.t= T_ERROR;
				break;

		}
		++rc->index;
	}
	//

	//������������ַ���Ϊ�գ���
	if (bufindex != 0)
	{
		int copy = 1;
		if (token.t == T_ID)
		{
			token.t = lookup_reserved_keyword(buf);
			if (token.t != T_ID)
			{
				copy = 1;
			}
		}
		if (copy == 1 && (token.t == T_ID || token.t == T_UNSIGNED || token.t == T_NUMBER))
		{
			token.c = (char *)malloc(bufindex+1);
			memset(token.c,0,bufindex +1);
			memcpy(token.c,buf,bufindex);
		}
	}
	return token;
}
void scanner_get_error(scanner_rc_t *rc)
{
	if (rc == NULL) return;
	fprintf(stderr, "find error on line: %d\n",rc->linecount +1);
}
int scanner_get_curr_line(scanner_rc_t *rc)
{
	return rc->linecount+1;
}

void scanner_debug(scanner_rc_t *rc)
{
	fprintf(stdout, "Code Source:\n%s\n",rc->input);
	fprintf(stdout, "File Length:%d\n",rc->length);
	fprintf(stdout, "File Index: %d\n",rc->index);

}

TokenType lookup_reserved_keyword(const char *str)
{
	for (int i = 0; i <sizeof(reserved_keywords)/sizeof(reserved_keywords[0]); ++i)
	{
		if (strlen(reserved_keywords[i].reserved_name) == strlen(str) 
			&& strcmp(reserved_keywords[i].reserved_name,str) == 0 )
		{
			return reserved_keywords[i].t;
		}
	}
	return T_ID;
}