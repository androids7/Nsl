
/*
�������޹��ķ�

���ս���ţ����ս���ţ�һ����ʼ���ţ�һ�����ʽ���




BNF�ķ�


�����ķ����﷨�����������Ͽ��Է�Ϊ���֣�ͨ�õģ�
�Զ�����
�Ӷ�����ʼ�����﷨��

�Ե�����
��Ҷ�ӽ�㿪ʼ�����﷨��

���ǰ��մ����ҵķ�ʽ��ɨ�裬ÿ��ɨ��һ������

LL�ķ�

LR�ķ�

E��>E + T | T
T -> T * F | F
F -> (E) | id
�����ķ�����LR�ķ����������Ե����ϵ��﷨����������

E->TE1;
E1-> + TE1|e;
T->FT1;
T1->* FT1 | e
F->(E)|id  ����ݹ�汾���������Զ����µ��﷨������



*/
#include "scanner.h"
#include "node.h"



typedef struct 
{
	const char *file_name;
	Token token;
	scanner_rc_t *scanner_rc;
}parser_rc_t;

void parser_init(parser_rc_t *rc, const char *file_name);

tree_node_t *parser_parse(parser_rc_t *rc);

//����state�����
tree_node_t *parser_state(parser_rc_t *rc);

void parser_debug(parser_rc_t *rc,tree_node_t * tree);

void parser_match(parser_rc_t *rc,TokenType expected);
tree_node_t *parser_stmt(parser_rc_t *rc);
tree_node_t *parser_if_stmt(parser_rc_t *rc);
tree_node_t *parser_while_stmt(parser_rc_t *rc);
tree_node_t *parser_do_stmt(parser_rc_t *rc);
tree_node_t *parser_assign_stmt(parser_rc_t *rc);
tree_node_t *parser_unsigned_stmt(parser_rc_t *rc);
tree_node_t *parser_include(parser_rc_t *rc);
tree_node_t *parser_define(parser_rc_t *rc);
tree_node_t *parser_exp(parser_rc_t *rc);
tree_node_t *parser_simple_exp(parser_rc_t *rc);
tree_node_t *parser_term(parser_rc_t *rc);
tree_node_t *parser_factor(parser_rc_t *rc);
tree_node_t *parser_stmt_seq(parser_rc_t *rc);
tree_node_t *parser_substate(parser_rc_t *rc);
tree_node_t *parser_state(parser_rc_t *rc);



