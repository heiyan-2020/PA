#include <isa.h>
#include <string.h>
#include <memory/paddr.h>
/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>
int precedence[32] = {0, 4, 7, 4, 3, 3, 1, 1, 0, 0, 0, 7, 11, 2};

enum {
  TK_NOTYPE, TK_PLUS, TK_EQ, TK_SUB, TK_DIV, TK_MUL, TK_LEFT, TK_RIGHT, TK_NUM, TK_HEXNUM, TK_REG, TK_NEQ, TK_AND, TK_DEREF
};
int find_op(int begin, int end);
bool check_parentheses(int begin, int end);
uint32_t eval(int begin, int end, bool* success3) ;
static struct rule {
  const char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {" +", TK_NOTYPE},    // spaces
	{"^0x[0-9A-Fa-f]+", TK_HEXNUM},
  {"[0-9]+", TK_NUM},
  {"==", TK_EQ},        // equal
  {"\\+", TK_PLUS},         // plus
	{"-", TK_SUB},
	{"\\*", TK_MUL},
	{"/", TK_DIV},
	{"\\(", TK_LEFT},
	{"\\)", TK_RIGHT},
	{"^\\$[a-z0-9_]+", TK_REG},
	{"!=", TK_NEQ},
	{"&&", TK_AND},
	{"^\\*", TK_DEREF}
};

#define NR_REGEX ARRLEN(rules)


static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

static Token tokens[3200] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

//static void print_test(int begin,int end) {
//	for(int i = begin; i <= end; i++) {
//		switch (tokens[i].type) {
//						case TK_PLUS: printf("+");break;
//						case TK_SUB: printf("-");break;
//						case TK_MUL: printf("*");break;
//						case TK_DIV: printf("/");break;
//						case TK_NUM: printf("%s", tokens[i].str);break;
//						case TK_LEFT: printf("(");break;
//						case TK_RIGHT: printf(")");break;
//						default: break;
//		}
//	}
//	printf("\n");
//}


static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;
        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);

        position += substr_len;
				
        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */

        switch (rules[i].token_type) {
								case TK_NOTYPE: break;
								case TK_MUL: 
								case TK_DIV:
								case TK_PLUS:
								case TK_SUB:
								case TK_LEFT:
								case TK_EQ:
								case TK_NEQ:
								case TK_AND:
								case TK_RIGHT: {
									struct token tmp_token;
									tmp_token.type = rules[i].token_type;
									tokens[nr_token++] = tmp_token;
									break;
							  }
								case TK_REG:
								case TK_NUM: {
									assert(substr_len < 32);
									if (substr_len < 32) {
										struct token single_token;	
										single_token.type = TK_NUM;
										strncpy(single_token.str, substr_start, substr_len);
										single_token.str[substr_len] = '\0';
										tokens[nr_token++] = single_token;
									}
									break;
								//	else {
								//					assert(0);
								//		int offset = 0;
								//		while (substr_len - offset >= 32) {
								//			struct token single_token;
								//			single_token.type = TK_NUM;
								//			strncpy(single_token.str, substr_start + offset, 31);
								//			single_token.str[31] = '\0';
								//			offset += 31;
								//			tokens[nr_token++] = single_token;
								//		}
								//		struct token single_token;
								//		single_token.type = TK_NUM;
								//		strncpy(single_token.str, substr_start + offset, substr_len - offset);
								//		single_token.str[substr_len - offset] = '\0';
								//		tokens[nr_token++] = single_token;
								//	}					
								}
								case TK_HEXNUM: {
									Token single_token;
									single_token.type = TK_HEXNUM;
									strncpy(single_token.str, substr_start + 2, substr_len);
									single_token.str[substr_len] = '\0';
									tokens[nr_token++] = single_token;
									break;									
								}
         				default: break;
        }

        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}


word_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }

	for(int i = 0; i < nr_token; i ++) {
		if (tokens[i].type == TK_MUL && (i == 0 || tokens[i - 1].type  == TK_AND || tokens[i - 1].type  == TK_NEQ ||tokens[i - 1].type  == TK_EQ ||tokens[i - 1].type == TK_DIV ||tokens[i - 1].type  == TK_MUL || tokens[i - 1].type  ==  TK_SUB ||tokens[i - 1].type  == TK_PLUS ||tokens[i - 1].type  == TK_LEFT)) {
		tokens[i].type = TK_DEREF;
		}
	}
	
	int result = eval(0, nr_token - 1, success);
	return result;
}
  /* TODO: Insert codes to evaluate the expression. */
uint32_t eval(int begin, int end, bool* success) {
	if (begin > end) {
		printf("DEBUG INFO\nbegin = %d, end = %d\n",begin, end);
		printf("A syntax error in expression!\n");
		assert(0);	
	} else if(begin == end) {
			uint32_t result = 0;
			switch (tokens[begin].type){
							case TK_NUM: {
														sscanf(tokens[begin].str, "%u", &result);
														break;				
													 }
							case TK_HEXNUM: {
															sscanf(tokens[begin].str, "%x", &result);
															break;
															}
							case TK_REG: {
													 result = isa_reg_str2val(tokens[begin].str, success);
													 break;
													 }
							default :break;
		}	
			return result;
} else if(check_parentheses(begin, end) == true){
		if (begin + 1 > end - 1) {
		}
	
		return eval(begin + 1, end - 1, success);
	} else {
		int op = find_op(begin, end);
		if (tokens[op].type != TK_DEREF) {
			int lhs = eval(begin, op - 1, success);
			int rhs = eval(op + 1, end, success);
			switch (tokens[op].type) {
				case TK_PLUS : return lhs + rhs;
				case TK_SUB : return lhs - rhs;
				case TK_MUL : return lhs * rhs;
				case TK_DIV : return lhs / rhs;
				case TK_EQ : return lhs == rhs;
				case TK_NEQ: return lhs != rhs;
				case TK_AND: return lhs && rhs;
				default : assert(0);
			}		
		} else {
			assert(tokens[op].type == TK_DEREF);
			uint32_t result = 0;
			paddr_t address;
			sscanf(tokens[op].str, "%x", &address);
			for (int i = 3; i >= 0; i --) {
				uint8_t bits = *(guest_to_host(address + (3 - i)));
				uint32_t tmp = (uint32_t) bits;
				result |= (tmp << (8 * i));
			}
			return result;
		}
	}
	return 0;
}

bool check_parentheses(int begin, int end) {
	if (tokens[begin].type != TK_LEFT || tokens[end].type != TK_RIGHT) {
		return false;
	}
	int stack[400];
	int ptr = 0;
	for (int i = begin; i <= end; i ++) {
		if (tokens[i].type == TK_LEFT) {
			stack[ptr++] = i;
		} else if (tokens[i].type == TK_RIGHT) {
			ptr--;
		}
		if (ptr < 0) {
			assert(0);
			return false;
		}
	}
	if (ptr != 0) {
		assert(0);
	}
	return stack[0] == begin;
}

int find_op(int begin, int end) {
	int stack[400];
	int ptr = 0;
	for (int i = begin; i <= end; i ++) {
		if (tokens[i].type == TK_RIGHT){
			while (tokens[stack[ptr - 1]].type != TK_LEFT) {
				ptr--;
				if (ptr < 0) {
					assert(0);
				}
			}
			ptr--;
			assert(ptr >= 0);
		} else {
			stack[ptr++] = i;
		}
	}
	int op_pos = stack[ptr - 1];
	for (int i = ptr - 1; i >= 0; i --) {
		if (precedence[tokens[stack[i]].type] > precedence[tokens[op_pos].type]) {
			op_pos = stack[i];
		}
	}
	if (precedence[tokens[op_pos].type] >= 2) {
		return op_pos;
	}
	return -1;
}
