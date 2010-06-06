// Jass2 parser for bison/yacc
// by Rudi Cilibrasi
// Sun Jun  8 00:51:53 CEST 2003
// thanks to Jeff Pang for the handy documentation that this was based
// on at http://jass.sourceforge.net
// Released under the BSD license
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <malloc.h>
#include "grammar.tab.h"
#include "misc.h"

#define VERSIONSTR "0.93"
#define ERRORLEVELNUM 4

/*
preperation for gettext
Added by Tamino Dauth.
*/
#include <libintl.h>
#include <locale.h>
#define _(string) gettext(string)

int lineno;
int haderrors;
int ignorederrors;
int totlines;
int didparse;
int inloop;
int afterendglobals;
int *showerrorlevel;

int hashfunc(const char *name);
struct hashtable functions, globals, locals, params, types;
struct hashtable *curtab;
struct typenode *retval, *retcheck;
char *curfile;
struct typenode *gInteger, *gReal, *gBoolean, *gString, *gCode, *gHandle, *gNothing, *gNull, *gAny, *gNone;

void addPrimitiveType(const char *name, struct typenode **toSave)
{
  put(&types, name, *toSave = newtypenode(name, NULL));
}

void init(int argc, char **argv)
{
  int i;
  addPrimitiveType("handle", &gHandle);
  addPrimitiveType("integer", &gInteger);
  addPrimitiveType("real", &gReal);
  addPrimitiveType("boolean", &gBoolean);
  addPrimitiveType("string", &gString);
  addPrimitiveType("code", &gCode);
  gNothing = newtypenode("nothing", NULL);
  gNull = newtypenode("null", NULL);
  gAny = newtypenode("any", NULL);
  gNone = newtypenode("none", NULL);
  curtab = &globals;
  haderrors = 0;
  ignorederrors = 0;
  showerrorlevel = malloc(ERRORLEVELNUM*sizeof(int));
  for(i=0;i<ERRORLEVELNUM;i++)
    showerrorlevel[i] = 1;
  if (lookup(&functions, "ConvertRace") != NULL) {
     printf(_("Major error!\n"));
     exit(0);
  }
}

struct typenode *getFunctionType(const char *funcname)
{
  char ebuf[1024];
  struct funcdecl *result;
  result = lookup(&functions, funcname);
  if (result) return result->ret;
  sprintf(ebuf, _("Undeclared function: %s\n"), funcname);
  yyerrorex(3, ebuf);
}

const struct typeandname *getVariable(const char *varname)
{
  char ebuf[1024];
  struct typeandname *result;
  result = lookup(&locals, varname);
  if (result) return result;
  result = lookup(&params, varname);
  if (result) return result;
  result = lookup(&globals, varname);
  if (result) return result;
  sprintf(ebuf, _("Undeclared variable: %s\n"), varname);
  yyerrorex(2, ebuf);
  // Store it as unidentified variable
  put(curtab, varname, newtypeandname(gAny, varname));
  return getVariable(varname);
}

struct typeandname *newtypeandname(const struct typenode *ty, const char *name)
{
  struct typeandname *tan = calloc(sizeof(struct typeandname), 1);
  tan->ty = ty;
  tan->name = strdup(name);
  tan->next = NULL;
  return tan;
}

struct typenode *newtypenode(const char *typename, const struct typenode *superclass)
{
  struct typenode *result;
  result = calloc(sizeof(struct typenode), 1);
  result->typename = strdup(typename);
  result->superclass = superclass;
  return result;
}

struct paramlist *newparamlist()
{
  struct paramlist *tl = calloc(sizeof(struct paramlist), 1);
  tl->head = NULL;
  tl->tail = &tl->head;
  return tl;
}

void addParam(struct paramlist *pl, struct typeandname *tan)
{
  tan->next = *(pl->tail);
  *(pl->tail) = tan;
}

struct funcdecl *newfuncdecl()
{
  struct funcdecl *fd = calloc(sizeof(struct funcdecl), 1);
  fd->name = NULL;
  fd->p = NULL;
  fd->ret = NULL;
  return fd;
}

const struct typenode *getPrimitiveAncestor(const struct typenode *cur)
{
  while (cur->superclass)
    cur = cur->superclass;
  return cur;
}

int isDerivedFrom(const struct typenode *cur, const struct typenode *base)
{
  do {
    if (cur == base) return 1;
    cur = cur->superclass;
  } while (cur);
  return 0;
}

void showtypenode(const struct typenode *td)
{
  const char *tp = NULL;
  const char *tn = "";
  char *extends = "";
  char ebuf[1024];
  assert(td);
  assert(td->typename);
  /*
  if (td->superclass) {
    sprintf(ebuf, " extends %s", td->superclass->typename);
    extends = ebuf;
  }
  */
  printf("%s%s", td->typename, extends);
}

void showfuncdecl(struct funcdecl *fd)
{
  struct typeandname *tan;
  printf("%s takes ", fd->name);
  if (fd->p->head == NULL)
    printf("nothing ");
  for (tan = fd->p->head; tan; tan = tan->next) {
    showtypenode(tan->ty);
    printf(" %s", tan->name);
    if (tan->next)
      printf(",");
    printf(" ");
  }
  printf("returns ");
  showtypenode(fd->ret);
  printf("\n");
}


int hashfunc(const char *name)
{
  int h = 0;
  const unsigned char *s;
  for (s = name; *s; ++s)
    h = ((811 * h + (*s)) % 19205861);
  return ((h % BUCKETS) + BUCKETS) % BUCKETS;
}

void *lookup(struct hashtable *h, const char *name)
{
  struct hashnode *hn;
  int hf = hashfunc(name);
  hn = h->h[hf];
  while (hn) {
    if (strcmp(hn->name, name) == 0)
      return hn->val;
    hn = hn->next;
  }
  return NULL;
}

void put(struct hashtable *h, const char *name, void *val)
{
  struct hashnode *hn;
  int hf;
  
  if (lookup(h, name) != NULL) {
    char ebuf[1024];
    sprintf(ebuf, _("Symbol %s multiply defined."), name);
    yyerrorex(3, ebuf);
    return;
  }
  hf = hashfunc(name);
  hn = calloc(sizeof(struct hashnode), 1);
  hn->name = strdup(name);
  hn->val = val;
  hn->next = h->h[hf];
  h->h[hf] = hn;
}

void clear(struct hashtable *h)
{
  int i;
  struct hashnode *hn;
  for (i = 0; i < BUCKETS; ++i) {
    hn = h->h[i];
    while (hn) {
      struct hashnode *tofree = hn;
      hn = hn->next;
      free(tofree->name);
      free(tofree);
    }
    h->h[i] = NULL;
  }
}

struct typenode *binop(const struct typenode *a, const struct typenode *b)
{
  a = getPrimitiveAncestor(a);
  b = getPrimitiveAncestor(b);
  if (a == gInteger && b == gInteger)
    return gInteger;
  if (a == gAny)
    return b;
  if (b == gAny)
    return a;
  if ((a != gInteger && a != gReal) || (b != gInteger && b != gReal)) {
    yyerrorex(3, _("Bad types for binary operator."));
  }
  return gReal;
}

int canconvert(const struct typenode *ufrom, const struct typenode *uto)
{
  const struct typenode *from = ufrom, *to = uto;
  char ebuf[1024];
#if 0
  if (lineno > 2400 && lineno < 2500) {
    yydebug = 1;
    fprintf(stderr, "LINE: %d\n", lineno);
  }
  else
    yydebug = 0;
#endif
  if (from == NULL || to == NULL) return 0;
  if (from == gAny || to == gAny) return 1;
  if (isDerivedFrom(from, to))
    return 1;
  /* Blizzard bug: allows downcasting erroneously */
  /* TODO: get Blizzard to fix this in Blizzard.j and the language */
//  if (isDerivedFrom(to, from))
//    return 1;
  if (from->typename == NULL || to->typename == NULL) return 0;
  if (from == gNone || to == gNone) return 0;
  from = getPrimitiveAncestor(from);
  to = getPrimitiveAncestor(to);
  if ((from == gNull) && (to != gInteger) && (to != gReal))
    return 1;
  if ((from == gInteger) &&
      (to == gInteger || to == gReal))
    return 1;
  sprintf(ebuf, _("Cannot convert %s to %s."), ufrom->typename, uto->typename);
  yyerrorex(3, ebuf);
}

int canconvertreturn(const struct typenode *ufrom, const struct typenode *uto)
{
  const struct typenode *from = ufrom, *to = uto;
  char ebuf[1024];
  if (from == NULL || to == NULL) return 0;
  if (from == gAny || to == gAny) return 1;
  if (isDerivedFrom(from, to))
    return 1;
  /* Blizzard bug: allows downcasting erroneously */
  /* TODO: get Blizzard to fix this in Blizzard.j and the language */
  //  if (isDerivedFrom(to, from))
  //    return 1;
  if (from->typename == NULL || to->typename == NULL) return 0;
  if (from == gNone || to == gNone) return 0;
  from = getPrimitiveAncestor(from);
  to = getPrimitiveAncestor(to);
  
  // can't return integer when it expects a real (added 9.5.2005)
  if ((to == gReal) && (from == gInteger))
    return 0;
    
  // can't return null when it expects integer, real or boolean (added 9.5.2005)
  if ((from == gNull) && (to != gInteger) && (to != gReal) && (to != gBoolean))
    return 1;
    
  if (from == to)
    return 1;
  sprintf(ebuf, _("Cannot convert %s to %s"), ufrom->typename, uto->typename);
  yyerrorex(1, ebuf);
}

struct typenode *combinetype(struct typenode *n1, struct typenode *n2) {
  if ((n1 == gNone) || (n2 == gNone)) return gNone;
  n1 = getPrimitiveAncestor(n1);
  n2 = getPrimitiveAncestor(n2);
  if (n1 == n2) return n1;
  if (n1 == gNull)
    return n2;
  if (n2 == gNull)
    return n1;
  if ((n1 == gInteger) && (n2 == gReal))
    return gReal;
  if ((n1 == gReal) && (n2 == gInteger))
    return gInteger;
//  printf("Cannot convert %s to %s", n1->typename, n2->typename);
  return gNone;
}

void checkParameters(const struct paramlist *func, const struct paramlist *inp)
{
  const struct typeandname *fi = func->head;
  const struct typeandname *pi = inp->head;
  int pnum = 1;
  for (;;) {
    if (fi == NULL && pi == NULL)
      return;
    if (fi == NULL && pi != NULL) {
      yyerrorex(3, _("Too many arguments passed to function."));
      return;
    }
    if (fi != NULL && pi == NULL) {
      yyerrorex(3, _("Not enough arguments passed to function."));
      return;
    }
    canconvert(pi->ty, fi->ty);
    pi = pi->next;
    fi = fi->next;
  }
}

void isnumeric(const struct typenode *ty)
{
  ty = getPrimitiveAncestor(ty);
  if (!(ty == gInteger || ty == gReal || ty == gAny))
    yyerrorex(3, _("Cannot be converted to numeric type."));
}

void checkcomparison(const struct typenode *a, const struct typenode *b)
{
  const struct typenode *pa, *pb;
  pa = getPrimitiveAncestor(a);
  pb = getPrimitiveAncestor(b);
  if (pa == gString || pa == gHandle || pa == gCode || pa == gBoolean || pb == gString || pb == gCode || pb == gHandle || pb == gBoolean) {
    yyerrorex(3, _("Comparing the order/size of 2 variables only works on Reals and Integers."));
    return;
  }
  if (pa == gNull && pb == gNull)
    yyerrorex(3, _("null > null and similar is not allowed."));
}

void checkeqtest(const struct typenode *a, const struct typenode *b)
{
  const struct typenode *pa, *pb;
  pa = getPrimitiveAncestor(a);
  pb = getPrimitiveAncestor(b);
  if ((pa == gInteger || pa == gReal) && (pb == gInteger || pb == gReal))
    return;
  if (pa == gNull || pb == gNull)
    return;
  if (pa != pb) {
    yyerrorex(3, _("Comparing two variables of different primitive types (except real and integer) is not allowed."));
    return;
  }
}

void dofile(FILE *fp, const char *name)
{
	void *cur;
  lineno = 1;
  afterendglobals = 0;
  int olderrs = haderrors;
	cur = yy_create_buffer(fp, BUFSIZE);
  yy_switch_to_buffer(cur);
  curfile = name;
	while (yyparse())
    ;
  if (olderrs == haderrors)
		printf(_("Parse successful: %8d lines: %s.\n"), lineno, curfile);
  else
		printf(_("%s failed with %d error%s.\n"), curfile, haderrors - olderrs,(haderrors == olderrs + 1) ? "" : "s");
	totlines += lineno;
}

void printversion()
{
	printf(_("pjass version %s by Rudi Cilibrasi, modified by AIAndy and Baradé.\n"), VERSIONSTR);
}

void doparse(int argc, char **argv)
{
	int i = 1;
	
	for (; i < argc; ++i) {
		if (argv[i][0] == '-' && argv[i][1] == 0) {
			dofile(stdin, "<stdin>");
			didparse = 1;
			continue;
		}
		if (strcmp(argv[i], "--help") == 0) {
			printversion();
			printf(_(
			"To use this program, list the files you would like to parse in order.\n"
			"If you would like to parse from standard input (the keyboard), then\n"
			"use - as an argument.  If you supply no arguments to pjass, it will\n"
			"parse the console standard input by default.\n"
			"To test this program, go into your Scripts directory, and type:\n"
			"pjass common.j common.ai Blizzard.j\n"
			"pjass accepts some options:\n"
			"pjass --help       Display this help\n"
			"pjass --version    Display version information and exit\n"
			"pjass -e1          Ignores error level 1"
			"pjass +e2          Undo Ignore of error level 2"
			"pjass -            Read from standard input (may appear in a list)\n"
			));
			exit(0);
			continue;
		}
		if (strcmp(argv[i], "--version") == 0) {
			printf("pjass %s.\n", VERSIONSTR);
			exit(0);
			continue;
		}
		if (argv[i][0] == '-' && argv[i][1] == 'e' && argv[i][2] >= '0' && argv[i][2] < ('0' + ERRORLEVELNUM)) {
			showerrorlevel[argv[i][2]-'0'] = 0;
			continue;
		}
		if (argv[i][0] == '+' && argv[i][1] == 'e' && argv[i][2] >= '0' && argv[i][2] < ('0' + ERRORLEVELNUM)) {
			showerrorlevel[argv[i][2]-'0'] = 1;
			continue;
		}
		FILE *fp;
		fp = fopen(argv[i], "rb");
		if (fp == NULL) {
			printf(_("Error: Cannot open %s.\n"), argv[i]);
			continue;
		}
		dofile(fp, argv[i]);
		didparse = 1;
		fclose(fp);
	}
  	if (argc == 1) {
		didparse = 1;
		dofile(stdin, "<stdin>");
  	}
}
