
#include"hashtab.hpp"

void fl_log(const char* str);
int isIn(char c, const char* cs) {
	for (size_t i = 0; i < __strlen(cs); i++)
	{
		if (c == cs[i]) {
			return 1;
		}
	}
	return 0;
}

enum TokenTypes {
	TOK_NONE = 128,
	TOK_NUM,
	TOK_KWD,
	TOK_STR,

	OBJ_FUNCALL
};
const char* TokenTypeStrings[] = {
	"None",
	"Num",
	"Kwd",
	"Str",
	"funcall"
};
char singlecharbuf[] = "k";
const char* toktostr(int type) {
	if (type < 128 && type >= 0) {
		singlecharbuf[0] = '_';
		if (isprint(type)) {
			singlecharbuf[0] = type;
			return singlecharbuf;
		}
	}
	else {
		if (type >= TOK_NONE && (type - TOK_NONE) < sizeof(TokenTypeStrings) / sizeof(TokenTypeStrings[0]))
			return TokenTypeStrings[type - TOK_NONE];
	}
	return TokenTypeStrings[0];
}
typedef struct {
	enum TokenTypes type;
	char* data;
} Obj;
typedef struct {
	Obj val;
	void* next;
} Arg;
typedef struct {
	Arg arg;
	char* name;
} Function;

Obj newobjint(int f) {
	Obj newobj = { TOK_NUM,*((char**)&f) };
	return newobj;
}
Obj newobjnone(int f) {
	Obj newobj = { TOK_NONE,*((char**)&f) };
	return newobj;
}
char strings[300] = { 0 };
char* stringsptr = 0;
Obj allobjs[20];
unsigned int objindex = 0;

StaticHashMap stringsstatmap;
char* storestring(const char* str) {
	//LLNode* node = hashmap_contains(stringsmap, str);
	KeyValPair* kv = statmap_contains(&stringsstatmap, (char*)str);
	if (kv != 0) {
		return kv->key;
	}
	size_t len = __strlen(str);
	__strcpy(stringsptr, str);
	char* cstr = stringsptr;
	statmap_set(&stringsstatmap, (char*)cstr, 0);
	//hashmap_set(stringsmap, cstr, 0);
	stringsptr += len + 1;
	
	return cstr;
}
Obj newobjstr(const char* str) {
	//int len = strlen(str);
	//printf("strs %d %d %d\n", strings, len,0);
	//strcpy(stringsptr, str);
	Obj newobj = { TOK_STR, storestring(str) };

	return newobj;
}
Obj newobjfun(char* name) {
	Obj newobj = { OBJ_FUNCALL,storestring(name) };
	return newobj;
}
Obj newobjkwd(char* name) {
	Obj newobj = { TOK_KWD,storestring(name) };
	return newobj;
}
int getIntNum(Obj o) {
	if (o.type != TOK_NUM) {
		return 0.f;
	}
	return *((int*)&(o.data));
}

char* src = 0;
char txtbuf[100];
int txtbufindex = 0;
int inwhat = TOK_NONE;

int reset() {
	txtbuf[txtbufindex++] = 0;
	txtbufindex = 0;
	if (inwhat == TOK_NUM) {
		inwhat = TOK_NONE;
		return TOK_NUM;
	}
	if (inwhat == TOK_KWD) {
		inwhat = TOK_NONE;
		return TOK_KWD;
	}
	if (txtbufindex > 0) {

	}
	return TOK_NONE;
}
int getstr(char del) {
	src++;
	char c = *src;
	char lc = c;
	int maxloop = 100;
	while (c != 0 && c != del && lc != '\\' && maxloop > 0) {
		maxloop--;
		txtbuf[txtbufindex++] = c;
		c = *(++src);
		lc = c;
	}
	src++;
	txtbuf[txtbufindex++] = 0;
	txtbufindex = 0;
	//printf("str: '%s'", txtbuf);
	return TOK_STR;
}
int _lexer() {
	char c = *src;
	if (c == 0)return reset();
	int maxloop = 100;
	while (maxloop > 0) {
		maxloop--;
		if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
			int res = reset();
			if (res != TOK_NONE) {
				return res;
			}
			src++;
		}
		else if (isIn(c, "(),;{}")) {
			int res = reset();
			if (res != TOK_NONE) {
				return res;
			}
			src++;
			return c;
		}
		else if (c >= '0' && c <= '9') {
			if (inwhat != TOK_NUM) {
				int res = reset();
				if (res != TOK_NONE) {
					return res;
				}
			}
			inwhat = TOK_NUM;
			txtbuf[txtbufindex++] = c;
		}
		else if (c == '"' || c == '\'') {
			int res = reset();
			if (res != TOK_NONE) {
				return res;
			}
			return getstr(c);
		}
		else {
			inwhat = TOK_KWD;
			txtbuf[txtbufindex++] = c;
		}
		src++;
		c = *src;
		if (c == 0)break;
	}
	return reset();
}
int tok = 0;
int lexer() {
	tok = _lexer();
	//printf("%d '%c' %s \n", tok, isprint(tok) ? tok : '_',toktostr(tok));printf(" %s ", toktostr(tok));
	fl_log(toktostr(tok));
	return tok;
}
int expect(int exptok) {
	if (tok == exptok) {
		lexer();
		return 1;
	}
	return 0;
}
int getnum() {
	if (tok == TOK_NUM) {
		return __atoi(txtbuf);
		//return (float)strtol(txtbuf, txtbuf + txtbufindex,10);

	}
	else {
		fl_log("expected num\n");
	}
}
int ifnot(int exptok) {
	if (tok == exptok) {
		lexer();
		return 0;
	}
	fl_log("exp tok ");
	fl_log(toktostr(exptok));
	fl_log(" got ");
	fl_log(toktostr(tok));
	fl_log(" src: ");
	fl_log(src);
	fl_log("\n");
	//printf("expected tok '%s' got ", toktostr(exptok));
	//printf(" '%s' \n", toktostr(tok));
	//printf("src: %s\n", src);
	return 1;
}

Obj statement(int execute);
Obj expression(int execute);
int getargs(Obj* args, int execute) {
	int argnum = 0;
	if (ifnot('(')) {
		return -3;
	}
	if (tok != ')') {
		while (argnum < 10) {
			args[argnum++] = expression(execute);
			if (tok == ',') {
				lexer();
				continue;
			}
			else if (tok == ')') {
				break;
			}
			else if (tok == TOK_NONE) {
				return -4;
			}
			else {
				fl_log("unexpected");
				fl_log(toktostr(tok));
				fl_log("\n");
				break;
			}
		}
	}
	if (ifnot(')')) {
		return -2;
	}
	return argnum;
}
Obj setvar(char* key, Obj val) {
	//LLNode* node = hashmap_contains(strhmap, key);
	/*char* node;
	if (node != 0) {
		allobjs[(size_t)node] = val;
	}
	else {
		allobjs[objindex] = val;
		//hashmap_set(strhmap, key, (void*)objindex);
		objindex++;
	}*/

	KeyValPair* kv = statmap_contains(&stringsstatmap, key);
	if (kv == 0) {
		allobjs[objindex] = val;
		statmap_set(&stringsstatmap, key, (char*)objindex);
		objindex++;
	}
	else {
		allobjs[(unsigned int)kv->val] = val;
	}
	return val;
}
Obj getvar(char* key) {
	//TODO
	//LLNode* node = hashmap_contains(strhmap, key);
	/*char* = 0;
	if (node != 0) {
		return allobjs[(size_t)node];
	}
	else {
		return newobjnone(-13);
	}*/
	KeyValPair* kv = statmap_contains(&stringsstatmap, key);
	if (kv != 0) {
		return allobjs[(unsigned int)kv->val];
	}
	return newobjnone(-13);
}
int setargs(Obj* args) {
	int argnum = 0;
	if (ifnot('(')) {
		return -3;
	}
	int cnt = 0;
	int maxloop = 100;
	while (maxloop > 0) {
		maxloop--;
		if (tok == TOK_KWD) {
			//setvar(args[0].data, args[1]);
			setvar(storestring(txtbuf), args[cnt++]);
			lexer();
		}
		else if (tok == ')') {
			break;
		}
		else if (tok == ',') {
			lexer();
			continue;
		}
		else {
			break;
		}
	}
	if (ifnot(')')) {
		return -2;
	}
	return cnt;
}
Obj expression(int execute) {
	char* start = src;
	if (tok == TOK_KWD) {
		char fname[100];
		__strcpy(fname, txtbuf);
		char* loopstart = src;
		int sum = 0;
		lexer();
		if (tok != '(') {
			if (execute) {
				//LLNode* node = hashmap_contains(strhmap, fname);
				//TODO
				/*char* node;
				if (node != 0) {
					return allobjs[(size_t)node];
				}
				else {
					//printf("keyword %s not found!", fname);
				}*/
			}
			return newobjkwd(fname);
		}
		Obj args[2];
		int argnum = getargs(args, execute);
		if (argnum < 0) {
			return newobjnone(argnum);
		}
		if (__strcmp(fname, "add") == 0) {
			int sum = 0;
			for (size_t i = 0; i < argnum; i++)
			{
				sum += getIntNum(args[i]);
			}
			return newobjint(sum);
		}
		else if (__strcmp(fname, "sub") == 0) {
			int sum = getIntNum(args[0]);
			for (size_t i = 1; i < argnum; i++)
			{
				sum -= getIntNum(args[i]);
			}
			return newobjint(sum);
		}
		else if (__strcmp(fname, "set") == 0) {
			if (execute) {
				if (argnum >= 2) {
					if (args[0].type == TOK_STR) {
						return setvar(args[0].data, args[1]);
					}
				}
				else {
					fl_log("expected more arguments!\n");
				}
			}
			return args[1];
		}
		else if (__strcmp(fname, "get") == 0) {
			if (argnum >= 1) {
				if (args[0].type == TOK_STR) {
					return getvar(args[0].data);
				}
				else {
					return newobjnone(-23);
				}
			}
			else {
				fl_log("expected more arguments!\n");
			}
		}
		else if (__strcmp(fname, "print") == 0) {
			int ret = 0;
			if (execute) {
				for (size_t i = 0; i < argnum; i++)
				{
					if (args[i].type == TOK_NUM) {
						//ret = printf("%d ", (int)getfloat(args[i]));
						char buf[30];
						__itoa((int)getIntNum(args[i]), buf, 10);
						fl_log(buf);

					}
					else if (args[i].type == TOK_STR) {
						//ret = printf("%s ", args[i].data);
						fl_log(args[i].data);
					}
					else {
						//ret = printf("%s ", toktostr(args[i].type));
						fl_log(toktostr(args[i].type));
					}
				}
				fl_log("\n");
			}
			return newobjint(ret);
		}
		else if (__strcmp(fname, "if") == 0) {
			if (execute) {
				for (size_t i = 0; i < argnum; i++)
				{
					sum += getIntNum(args[i]);
				}
			}
			if (sum == 0) {
				statement(0);
			}
			else {
				statement(execute);
			}
			return newobjstr("if");
		}
		else if (__strcmp(fname, "while") == 0) {
			Obj out = newobjnone(-1);
			char* aftergetargsptr = 0;
			if (execute) {
				while (1) {
					if (argnum < 0) {
						return newobjnone(argnum);
					}
					sum = 0.f;
					for (size_t i = 0; i < argnum; i++)
					{
						sum += getIntNum(args[i]);
					}
					if (sum == 0) {
						statement(0);
						break;
					}
					else {
						out = statement(execute);
					}
					reset();
					src = loopstart;
					lexer();
					argnum = getargs(args, execute);

					aftergetargsptr = src;
				}
			}
			else {
				statement(0);
			}
			return newobjstr("while");
		}
		else {
			if (tok == '{') {
				//printf("new function %s .\n", fname);
				//hashmap_set(stringsmap, storestring(fname), loopstart);
				lexer();
				statement(0);
				if (ifnot('}')) {
					return newobjnone(-2);
				}
				return newobjkwd(fname);
			}
			else {
				char* node=0;
				if (node != 0) {
					char* returnptr = src;
					//printf("found function %s %p \n", fname, node->val);
					src = node;
					lexer();
					//argnum = getargs(args, execute);
					int newargnum = setargs(args);
					//printf("argnums: %d %d\n", argnum, newargnum);
					Obj o = statement(execute);
					
					src = returnptr;
					lexer();
					return o;
				}
				else
				{
					//printf("unknown keyword '%s' %s!\n", fname, start);
					fl_log("unknown keyword ");
					fl_log(fname);
					fl_log(start);
					fl_log("\n");
				}
			}
		}

		return newobjkwd(fname);
	}
	else if (tok == TOK_NUM) {
		int num = getnum();
		lexer();
		return newobjint(num);
	}
	else if (tok == TOK_STR) {
		Obj o = newobjstr(txtbuf);
		lexer();
		return o;
	}
	else if (tok == '{') {
		return statement(execute);
	}
	else if (tok == TOK_NONE) {
		return newobjnone(-20);
	}
	else {
		fl_log("don't know what to do with token");
		fl_log(toktostr(tok));
		return newobjnone(-30);
	}
	return newobjnone(-10);
}
Obj statements(int execute) {
	Obj st = statement(execute);
	Obj last = st;
	int maxloop = 100;
	while (maxloop) {
		maxloop--;
		if (st.type == TOK_NONE)return last;
		if (tok == '}')return st;
		last = st;
		st = statement(execute);
	}
	return last;
}
Obj statement(int execute) {
	if (tok == '{') {
		lexer();
		Obj ret = statements(execute);
		//printf("ret:%f ", getfloat(ret));
		if (ifnot('}')) {
			return newobjnone(-2);
		}
		return ret;
	}
	else {
		Obj ex = expression(execute);
		if (tok == ';') {
			lexer();
		}
		return ex;
		//printf("don't know what to do with token %d !\n", tok);
	}
	return newobjnone(0);
}
Obj parser(int execute) {
	return statements(execute);
}
void test2(const char* textcode, int res) {
	//printf("\"%s\"\n", textcode);
	fl_log("'");
	fl_log(textcode);
	fl_log("'\n");
	src = (char*)textcode;
	tok = 0;
	stringsptr = strings;
	txtbufindex = 0;
	inwhat = TOK_NONE;
	lexer();
	//parser(0);
	Obj ret = parser(1);
	//printf("RETURN: %f", getfloat(ret));
	fl_log(toktostr(ret.type));
	fl_log("\n");
}
void test(const char* textcode) {
	test2(textcode, 0.);
}

/*int main()
{
	strhmap = hashmap_create_string(20);
	stringsmap = hashmap_create_string(40);
	test("add(  1,1); ");
	test("add(1,1)");
	test("add(  1,1)");
	test("add(  1,1);     ");
	test("add(  1,1);");
	test("sub(23,3)");
	test("set('asdf',23);get('asdf');");
	//test("get('asdf');");
	test("print('jo',1);");
	test("print('jo',1);");
	test("set('asdf',23);print('test:',get('asdf')); ");
	test("set('v',231);if(0)set('v',3);get('v')");
	test("if(0){print('scope');}if(0)print('test2');if(1)print('4321');");
	test("set('i',12);while(get('i')){print('i:',get('i'));set('i',sub(get('i'),1));}");
	test("\
set('i',0);\
while(sub(get('i'),10)){\
print('i:',get('i'));\
set('i',add(get('i'),1))\
}");

	test("\
set('i',2);\
set('n',9);\
set('a',0);\
set('b',1);\
while(sub(get('i'),sub(get('n'),1))){\
set('c',add(get('a'),get('b')));\
set('a',get('b'));\
set('b',get('c'));\
set('i',add(get('i'),1));\
}\
print('fibonacci number:',get('b'));");
	test("testfun(){print('asdftest')};print(testfun())");
	test("testfun(a){print('asdftest',a)};print(testfun(1))");
	test("x ");
	test("x");
	test("set('x',31)");
	test("x");
	test("\
funwithparamters(a,b){print(add(a,b))};\
funwithparamters(2,3);\
");
	return 0;
}*/
