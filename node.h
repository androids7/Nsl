//���������������
typedef enum{
	NodeType_State = 1,
	NodeType_Block,
	NodeType_Stmt
}node_type_t;

typedef enum{

};

//���һ��������ӽ��
/*
s -> state id { block }
block-> S  x : { stmt }
s->stc|cts
stmt -> stmt; //�������Ƴ����

*/
#define MAXCHILDREN 6

typedef struct TreeNode
{
	struct TreeNode *childs[MAXCHILDREN]; //�����ӽ��ĸ���
	struct TreeNode *sibling; //����Ҷ��
	node_type_t node_type_t; //����������

}tree_node_t;

