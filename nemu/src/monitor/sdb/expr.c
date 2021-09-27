#include <isa.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>

enum {
  TK_NOTYPE = 256, TK_PLUS, TK_EQ, TK_SUB, TK_MUL, TK_DIV, TK_LEFT, TK_RIGHT,
	TK_NUM

};
int find_op(int begin, int end);
bool check_parentheses(int begin, int end);
uint32_t eval(int begin, int end) ;
static struct rule {
  const char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {" +", TK_NOTYPE},    // spaces
  {"[0-9]+", TK_NUM},
  {"==", TK_EQ},        // equal
  {"\\+", TK_PLUS},         // plus
	{"-", TK_SUB},
	{"\\*", TK_MUL},
	{"/", TK_DIV},
	{"\\(", TK_LEFT},
	{"\\)", TK_RIGHT}
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

static Token tokens[32] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

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
								case TK_RIGHT: {
									struct token tmp_token;
									tmp_token.type = rules[i].token_type;
									tokens[nr_token++] = tmp_token;
									break;
							  }
								case TK_NUM: {
									if (substr_len < 32) {
										struct token single_token;	
										single_token.type = TK_NUM;
										strcpy(single_token.str, substr_start);
										tokens[nr_token++] = single_token;
									}	else {
										int offset = 0;
										while (substr_len - offset >= 32) {
											struct token single_token;
											single_token.type = TK_NUM;
											strncpy(single_token.str, substr_start + offset, 31);
											single_token.str[31] = '\0';
											offset += 31;
											tokens[nr_token++] = single_token;
										}
										struct token single_token;
										single_token.type = TK_NUM;
										strcpy(single_token.str, substr_start + offset);
										tokens[nr_token++] = single_token;
									}					
									break;
								}
          default: TODO();
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
	printf("make %d tokens\n", nr_token);
	return (word_t) eval(0, nr_token- 1);
}
  /* TODO: Insert codes to evaluate the expression. */
uint32_t eval(int begin, int end) {
	if (begin > end) {
		assert(0);
	} else if(begin == end) {
		uint32_t rtnValue;
		assert(tokens[begin].type == TK_NUM);
		sscanf(tokens[begin].str, "%u", &rtnValue);
	} else if(check_parentheses(begin, end) == true){
		return eval(begin + 1, end - 1);
	} else {
		int op = find_op(begin, end);
		uint32_t lhs = eval(begin, op - 1);
		uint32_t rhs = eval(op + 1, end);
		switch (tokens[op].type) {
			case TK_PLUS : return lhs + rhs;
			case TK_SUB : return lhs - rhs;
			case TK_MUL : return lhs * rhs;
			case TK_DIV : return lhs / rhs;
			default : assert(0);
		}		
	}
	return 0;
}

bool check_parentheses(int begin, int end) {
	if (tokens[begin].type != TK_LEFT || tokens[end].type != TK_RIGHT) {
		return false;
	}
	int stack[40];
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
	int stack[40];
	int ptr = 0;
	for (int i = begin; i <= end; i ++) {
		if (tokens[i].type == TK_RIGHT){
			while (tokens[stack[ptr - 1]].type != TK_LEFT) {
				ptr--;
				if (ptr < 0) {
					assert(0);
				}
			}
		} else {
			stack[ptr++] = i;
		}
	}
	int op_pos = stack[ptr - 1];
	for (int i = ptr - 1; i >= 0; i --) {
		if (tokens[stack[i]].type > tokens[op_pos].type) {
			op_pos = stack[i];
		}
	}
	if (tokens[op_pos].type >= TK_PLUS && tokens[op_pos].type <= TK_DIV) {
		return op_pos;
	}
	return -1;
}
