#include<string.h>
void mytrimF(char *s)
{

   char *t;
	t=s;
	while(*t==' '||*t=='\t')
		t++;
	while(*s++=*t++);
}
void  mytrimE(char *s)
{
   int  i=strlen(s)-1;
   for(;i>=0;i--)
          if(s[i]!='\t'&&s[i]!=' '&&s[i]!='\n')
                 break;
   s[i+1]='\0';
}