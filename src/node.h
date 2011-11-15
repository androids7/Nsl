#ifndef __NSL_NODE_H__
#define __NSL_NODE_H__

#include "scanner.h"

//���������������,һ��״̬���ͣ�һ�����ͣ�һ��ͷ�����ͣ�һ�ִ�������
typedef enum{
	NodeType_State = 1,
	NodeType_Sub, //
	NodeType_Header, //
	NodeType_Stmt, //
	NodeType_Exp //���ʽ����
}node_type_t;

//����ͷ���ļ�������
typedef enum{
	HeaderType_Include = 1,
	HeaderType_Define,
	HeaderType_Enum,
	HeaderType_Struct
}header_type_t;

//�����������
typedef enum{
	StmtType_If = 1,
	StmtType_While,
	StmtType_For,
	StmtType_Assign,
	StmtType_Decl,
	StmtType_Do,
}stmt_type_t;


//������ʽ����
typedef enum{
	ExpType_Op
}exp_type_t;

#define MAXCHILDREN 6

typedef struct TreeNode
{
	struct TreeNode *childs[MAXCHILDREN]; //�����ӽ��ĸ���
	struct TreeNode *sibling; //����Ҷ��
	node_type_t node_type; //����������
	union{
		stmt_type_t stmt;
		header_type_t header;
		exp_type_t exp;
	}type;
	int attr;
	exp_type_t exp_type;
}tree_node_t;
tree_node_t *new_exp_node(exp_type_t type);
tree_node_t *new_stmt_node(stmt_type_t type);
tree_node_t *new_state_node();
tree_node_t *new_include_node();
tree_node_t *new_sub_node();
tree_node_t *new_header_node();
#endif
