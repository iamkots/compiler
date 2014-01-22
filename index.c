#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libfdr/jval.h"
#include "libfdr/jrb.h"

void stopword(char*, JRB);
int is_stopword(char*, JRB);
char* wordlist(char *dong, char *word, int *flag);

int main()
{
	JRB b = make_jrb();
	JRB tree = make_jrb();
	JRB bn;

	FILE *fin = fopen("vanban.txt", "r");
	char dong[1000] = {};
	char *s = NULL;
	int flag = 0;
	int nline = 1;

	stopword("stopword.txt", b);

	while(fgets(dong, sizeof(dong), fin) != NULL)
	{
		dong[strlen(dong) - 2] = 'q';
		dong[strlen(dong) - 1] = '\0';
		s = dong;
		while(s != NULL)
		{
			char word[100] = {};
			char info[100] = {};
			s = wordlist(s, word, &flag);

			if(flag == 0) {
				if((word[0] <= 'Z' && word[0] >= 'A') || word[0] == '\0')
					continue;
			}
			else {
				if(word[0] == '\0')
					continue;
			}

			if(!is_stopword(word, b) || (flag = 0)) {
				if(jrb_find_str(tree, word) == NULL) {
					info[0] = 1;
					info[1] = nline;
					jrb_insert_str(tree, strdup(word), new_jval_s(strdup(info)));
				}
				else {
					JRB tmp = jrb_find_str(tree, word);
					char *s = jval_s(jrb_val(tmp));
					s[0]++;
					s[s[0]] = nline;
				}
			}
		}
		nline++;
	}

	jrb_traverse(bn, tree)
	{
		printf("%-20s %d ", jval_s(bn->key), jval_s(jrb_val(bn))[0]);
		char *s = jval_s(jrb_val(bn));
		for(++s; *s; ++s)
			printf("%d ", *s);
		printf("\n");
	}


	return 0;
}

int is_stopword(char *word, JRB b)
{
	char *s = word;
	for ( ; *s; ++s) *s = tolower(*s);
	if(jrb_find_str(b, word) != NULL){
		return 1;
	}
	else
		return 0;
}

void stopword(char *filename, JRB b)
{
	FILE *fin;
	fin = fopen(filename, "r");
	char stopword[100] = {};
	while(fscanf(fin, "%s", stopword) > 0)
	{
		(void) jrb_insert_str(b, strdup(stopword), new_jval_v(NULL));
	}
	fclose(fin);
}

char* wordlist(char *dong, char *word, int *flag)
{
	int i = 0;
	char c ;
	while( (c = *dong) && ((c > 'z' || c < 'a') && (c > 'Z' || c < 'A')) )
	{
		if(c == '.') {
			*flag = 1;
		}
		++dong;
	}

	if(c == '\0') {
		return NULL;
	}

	do{
		word[i++] = c;
		++dong;
	}while((c = *dong) && ((c <='z' && c >= 'a') || (c <= 'Z' && c >= 'A')));
	return dong;
}
