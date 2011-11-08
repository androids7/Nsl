//���ű���һ�ֹ����������ڱ����й�Դ������ĸ�����Ϣ�����ݽṹ
//���ű���أ����ǽ�Ϊÿ�����ý����Լ��ĵ����ķ��ű����ű��ÿ����Ŀ��
//������һ����ʶ����ص���Ϣ�����������ַ������������ͣ����Ĵ洢λ�ã�
//�����������Ϣ��

#include "parser.h"

//��ʼ���﷨������
void parser_init(parser_rc_t *rc, const char *file_name)
{
	rc->file_name = file_name;
	scanner_rc_t *scanner = (scanner_rc_t *)malloc(sizeof(scanner_rc_t));
	scanner_open_file(scanner,rc->file_name);
	rc->scanner_rc = scanner;
}

tree_node_t * parser_parse(parser_rc_t *rc)
{
	//
	tree_node_t * tree;
	rc->token = scanner_get_next_token(rc->scanner_rc);
	
	//tree = parser_stmt_seq(rc);
	if (rc->token.t == T_STATE)
	{
		//����state��ʼ�����
		tree = parser_state(rc);
	} else {
		//�����������
		//�����������壬if_else do_while,�����
		tree = parser_stmt_seq(rc);
	}
	if (rc->token.t != T_DONE)
	{
		parser_syntax_error(rc,"Code ends error");
	}
	return tree;
}
tree_node_t *parser_state(rc)
{
  //to create a state node.
  tree_node_t *t = new 
}
