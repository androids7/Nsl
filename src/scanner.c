/*
 * C-- 设计，一个C语言的子集脚本，实现C语言的基本简单的功能
 * created by bingxuefenggu@126.com
 */
#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"
#include <ctype.h>

const char *token_strings[] = { 
    "T_GLOBAL",
    "T_ID", //表示字母开始的数字或字符串，可用来表示函数名，常量等                                                                           
    "T_NUMBER", //表示任何数字常量                                                                                                           
    "T_IF","T_ELSE","T_ENDIF",//表示if else,endif                                                                                                
    "T_FOR","T_WHILE","T_SWITCH","T_CASE","T_BREAK","T_CONTINUE","T_GOTO","T_DO",//表示switch                                                              
    "T_UNSIGNED",     //表示字符类型相关 unsigned 可以定义无符号数。                                                                         
    "T_ENUM","T_TYPEDEF","T_STRUCT",//定义结构相关类型                                                                                           
    "T_BIT_AND","T_BIT_OR","T_BIT_XOR","T_LT","T_LE","T_GT","T_GE","T_EQ","T_NEQ","T_AND","T_OR","T_NOT",//表示运算符                                              
    "T_COLON","T_SEMI","T_POUND","T_LC","T_RC","T_LP","T_RP","T_COMMA",    //表示各种分隔符#{}();:<>\\''"%&                                                
    "T_ASSIGN","T_PLUS","T_MINUS","T_MUL","T_DIV","T_MOD",//如果需要支持，可以再往里加                                                                 
    "T_INCLUDE","T_DEFINE",//表示一些头文件和一些固定值的定义,include,define                                                                   
    "T_LITERAL",//表示两个双引号中间的任意内容                                                                                               
    "T_ERROR",//表示出错时的类型                                                                                                             
    //下面是C--扩展中固有的一些关键字                                                                                                      
    "T_STATE",//表示状态                                                                                                                     
   	"T_SUB",
	"T_CTS", //表示从客户端到服务器端                                                                                                        
    "T_STC", //表示从服务器端到客户端                                                                                                        
    "T_START","T_END",//表示状态开始，状态结束                                                                                                 
    "T_REGEX",//正则串                                                                                                                       
    "T_DONE",
    "T_DOLLAR"//还有$号未定义,璧如$proto这该如何表示 
};


//先暂定下述关键字，如果有需要，再加
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


// 读取文件，从中取得内容,将其信息保存到scanner_rc_t中
int scanner_open_file(scanner_rc_t *rc, const char *filename)
{
	if (rc == NULL)
	{
		return -1;
	}
	memset(rc,0,sizeof(scanner_rc_t));
	
	FILE *fp = fopen(filename,"rb");
	if (fp == NULL) {
		printf("file open error");
		return -1;
	}
	fseek(fp,0L,SEEK_END);
	rc->length = ftell(fp);//得到整个文件的长度
	rewind(fp);
	rc->input = (char *)malloc(rc->length + 1);//直接申请一块缓冲区对其进行保存。
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

//直接从string中读取数据并
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
//根据单个字符来判断是否符合条件

TokenType scanner_get_one_char_token(scanner_rc_t *rc,char c)
{
	//现在是判断:号的时候
	switch(c) {
		case ':': 
			return T_COLON; //
		case ';': 
			return T_SEMI; //
		case '#': 
			return T_POUND;//
		case '{': 
			return T_LC;//
		case '}': 
			return T_RC;//
		case '(': 
			return T_LP;
		case ')': 
			return T_RP;
		case ',': 
			return T_COMMA;
		case '$': 
			return T_DOLLAR;
	    case '\0': 
			return T_DONE;
 	    case '/': 
			return T_DIV;
		//case '*': return ; 这个可能是乘号，也可能是指针。

		default:
			return T_ERROR;
	}
}

//得到这个Token的结点
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
	//jmp the comment
	if (c == '/'){
		++ rc->index;
		c = rc->input[rc->index];
		if (c == '/'){
			for (int i = rc->index; i < rc->length; ++i)
			{
				if (rc->input[i] == '\n' || rc->input[i] == 0)
				{
					++ rc->index;
					++ rc->linecount;
					break;
				} else {
					++rc->index;
				}
			}
		} else if (c == '*') {
			while(rc->index < rc->length)
			{
				if (rc->input[rc->index] == '/' && rc->input[rc->index-1] == '*') {
					++rc->index;
					break;
				}
				if (rc->input[rc->index] == '\n'){
					++ rc->linecount;
				}
				++rc->index;
			}
		} else {
			-- rc->index; //the div flag
		}
		c = rc->input[rc->index];
	}
	
	//delete the space again 
	while(c == ' ' || c == '\t' || c == '\r' || c == '\n')
	{
		if (c == '\n') ++rc->linecount;
		++rc->index;
		c = rc->input[rc->index];
	}
	//判断是否为字母还是数字还是别的内容
	if (isalpha(c)){
		state = ST_ID;
		buf[bufindex++] = c;
		token.t = T_ID;
	} else if (isdigit(c))
	{
		state = ST_NUMBER;
		buf[bufindex++] = c;
		token.t = T_NUMBER;
	} else if (c == '"')
	{
		state = ST_STRING;
		//buf[bufindex++] = c;
		token.t = T_LITERAL;	
	} else if (c == '=')
	{
		state = ST_EQ; //也有可能是复制。
	} else if (c == '<')
	{
		state = ST_LE; //可能是<,也有可能是<=
	} else if (c == '>')
	{
		state = ST_GE; //可能是大于，也有可能是>=
	} else if (c == '&')
	{
		state = ST_AND; //可能是&,也可能是 &&
	} else if (c == '|')
	{
		state = ST_OR;//可能是|,也可能是||
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
				if (isalpha(c) || c == '_' || isdigit(c))
				{
					buf[bufindex++] = c;
				} 
				else {
					state = ST_DONE;
					-- rc->index;
				}
				break;
			case ST_STRING:
				//the things between ' ""
				if (c == '\"')
				{
					state = ST_DONE;
					++ rc->index;
				} else {
					buf[bufindex++] = c;
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
					token.t = T_EQ; //等于操作
				} else {
					state = ST_DONE;
					token.t = T_ASSIGN; //复制操作
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
					// //这种注释形式
					bufindex = 0;
					for (int i = rc->index; i < rc->length; ++i)
					{
						if (rc->input[i] == '\n' || rc->input[i] == 0)
						{
							--rc->index; //回退到重新退出循环
							break;
						} else {
							++rc->index; //否则继续找下一个
						}
					}
					token.t = T_COMMENT;//注释
				} else if (c == '*')
				{
					// /* */类似这种comment
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
				    token.t = T_COMMENT; //

				} else {
					// 有可能是div符号这里
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

	//如果缓冲区内字符不为空，则
	if (bufindex != 0)
	{
		int copy = 1;
		if (token.t == T_ID)
		{
			token.t = lookup_reserved_keyword(buf);
			if (token.t != T_ID)
			{
				copy = 0;
			}
		}
		if (copy == 1 && (token.t == T_ID || token.t == T_UNSIGNED || token.t == T_NUMBER ||token.t == T_LITERAL))
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
