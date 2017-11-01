#ifndef STRING_H
#define STRING_H
int indexOfChar(char *string,char toTest);
int lastIndexOfChar(char *string,char toTest);
int getStringSize(char *string);
int isWhiteSpace(char c);
int isEqual(char *string1,char *string2);
char *substringWithEndIndex(char *string,int endIndex);
int indexOfString(char *string,char *searchString);
char *replaceString(char *string,char *searchPattern,char *replacement);
char *substring(char *string,int beginIndex);
char *reduceWhiteSpace(char *string);
char  *concatenate(char *string1,char *string2);
char *trim(char *string);
int startsWith(char *testString,char *prefix);

int indexOfChar(char *string,char toTest){
	int index=0;
	for(;string[index];index++){
		if(string[index]==toTest)
		  return index;
	}
	return -1;
}

int lastIndexOfChar(char *string,char toTest){
	int index=-1;
	for(int i=0;string[i];i++){
		if(string[i]==toTest){
		  index=i;
		}
	}
	return index;
}

void copyString(char *input, char *output){
	while((*(output++)=*(input++))!='\0');
}
int getStringSize(char *string){
	int finalSize=0;
        while(*(string+finalSize))
        {
                finalSize++;
        }
	return finalSize;
}

int isWhiteSpace(char c){
        if(c==' '||c=='\t'||c=='\n'|| c=='\r'||c=='\v'||c=='\f'){
        return 1;
        }
        return 0;
}

int isEqual(char *string1,char *string2){
	int i=0;
	for(;*(string1+i)!='\0'&&*(string2+i)!='\0';i++)
	{
		if(*(string1+i)!=*(string2+i)){
			return 0;
		}
	}
	if((*(string1+i)==*(string2+i))&& *(string1+i)=='\0'){
		return 1;
	}
	return 0;
}

char *substringWithEndIndex(char *string,int endIndex){
        char *finalString=(char*)_malloc((endIndex+1)*sizeof(char));
        for(int i=0;i<endIndex && string[i]!='\0';i++)
        {
                finalString[i]=string[i];
        }
        finalString[endIndex]='\0';
        return finalString;
}

int indexOfString(char *string,char *searchString){
	int searchStringSize=getStringSize(searchString);
	for(int i=0;*(string+i);i++)
	{
		if(*(string+i)==searchString[0] &&isEqual(substringWithEndIndex((string+i),searchStringSize),searchString) ){
			return i;
		}
	}
	return -1;
}
char *replaceString(char *string,char *searchPattern,char *replacement){
	int indexOfSearchPattern=indexOfString(string,searchPattern);
	if(indexOfSearchPattern==-1){
		return string;
	}
	int finalSize=getStringSize(string)+getStringSize(replacement)-getStringSize(searchPattern);
	char *toReturn=(char*)_malloc((finalSize+1)*sizeof(char));
	int i=0;
	for(;i<indexOfSearchPattern;i++)
	{
		toReturn[i]=string[i];
	}
	for(int j=0;*(replacement+j)!='\0';j++,i++){
		toReturn[i]=replacement[j];
	}
	for(int j=indexOfSearchPattern+getStringSize(searchPattern);string[j]!='\0';j++,i++){
		toReturn[i]=string[j];
	}
	toReturn[i]='\0';
	return toReturn;
}

char *substring(char *string,int beginIndex){
         int finalSize=0;
        while(*(string+finalSize))
        {
                finalSize++;
        }
        finalSize-=beginIndex;
        char *finalString=(char*)_malloc((finalSize+1)*sizeof(char));
        for(int i=0;i<finalSize;i++)
        {
                *(finalString+i)=*(string+beginIndex+i);
        }
        *(finalString+finalSize)='\0';
        return finalString;
}

char ** split(char *string,char* pattern,int *splittedCount){
        char **toReturn=(char**)_malloc(20*sizeof(char*));
	int counter=0;
	char *inputString=string;
	while(1)
	{
	 	int patternIndex=indexOfString(inputString,pattern);
		if(patternIndex==-1)
		{
			break;
		}
		toReturn[counter]=substringWithEndIndex(inputString,patternIndex);
		inputString=substring(inputString,patternIndex+getStringSize(pattern));
		counter++;
	}
	toReturn[counter]=inputString;
	*splittedCount=counter+1;
	return toReturn;
}
char *reduceWhiteSpace(char *string)
{
	int size=getStringSize(string);
	char *finalString=(char*)_malloc((size+1)*sizeof(char));
	int j=0;
	for(int i=0;i<size;i++,j++)
	{
		finalString[j]=string[i];
		if(isWhiteSpace(string[i])){
			finalString[j]=' ';
			while(isWhiteSpace(string[i])){
				i++;
			}
			i--;
		}

	}
	return finalString;
}

char  *concatenate(char *string1,char *string2){
        int finalSize=0;
	while(*(string1+finalSize))
	{
		finalSize++;
	}
	for(int j=0;*(string2+j);j++)
	{
		finalSize++;
	}
        char *finalString=(char*)_malloc((finalSize+1)*sizeof(char));
        int i=0;
        for(;(finalString[i]=string1[i])!='\0';i++);
        for(int j=0;(finalString[i]=string2[j])!='\0';j++,i++);
	return finalString;
}
char *trim(char *string){
        int ofset=0;
        for(;(isWhiteSpace(*(string+ofset)))&& (*(string+ofset)!='\0');ofset++);

	char *finalString=(char*)_malloc(getStringSize(string)+1-ofset);
        int j=0;
        for(;*(string+j+ofset)!='\0';j++)
        {
                *(finalString+j)=*(string+j+ofset);

        }
        j--;
        for(;isWhiteSpace(*(finalString+j));j--);
        *(finalString+j+1)='\0';

	return finalString;
}

int startsWith(char *testString,char *prefix)
{
	for(int i=0;prefix[i]!='\0';i++){
		if(testString[i]=='\0'){
			return 0;
		}
		if(testString[i]!=prefix[i]){
			return 0;
		}
	}
	return 1;
}
#endif 

