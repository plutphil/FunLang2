

int __strcmp(const char* p1, const char* p2)
{
	const unsigned char* s1 = (const unsigned char*)p1;
	const unsigned char* s2 = (const unsigned char*)p2;
	unsigned char c1, c2;
	int maxwhilecount = 100;
	do
	{
		maxwhilecount--;
		c1 = (unsigned char)*s1++;
		c2 = (unsigned char)*s2++;
		if (c1 == '\0')
			return c1 - c2;
	} while (c1 == c2 && maxwhilecount>0);
	return c1 - c2;
}
size_t __strlen(const char* str)
{
	int count = 0;
	while (str[count] != 0 && count < 100) {
		count++;
	}
	return count;
}

int isprint(char c){
	if (c >= 32 && c <= 126)
		return (1);
	return (0);
}
char* __strcpy(char* strDest, const char* strSrc)
{
	int maxwhilecount = 100;
	char* temp = strDest;
	while ((*strDest++ = *strSrc++) && (maxwhilecount--)>0); // or while((*strDest++=*strSrc++) != '\0');
	return temp;
}
int __atoi(char* str)
{
	int res = 0;
	for (int i = 0; str[i] != 0 && i < 10; ++i)
		res = res * 10 + str[i] - '0';
	return res;
}
void reverse(char str[], int length)
{
	int start = 0;
	int end = length - 1;
	while (start < end) {
		char temp = str[start];
		str[start] = str[end];
		str[end] = temp;
		end--;
		start++;
	}
}
int __itoa(int i, char* _buf, int radix){
	char* buf = _buf;
	if (i == 0) {
		*buf++ = '0';
	}
	if (i < 0) {
		*buf++ = '-';
		i = -i;
	}
	int cnt = 0;
	while (i > 0) {
		*buf++ = '0' + (i % radix);
		i /= 10;
		cnt++;
	}
	reverse(buf - cnt, cnt);
	*buf++ = '\0';
	return buf - _buf;
}