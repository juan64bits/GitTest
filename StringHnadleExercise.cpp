#include <iostream>
#include <string.h>
#include <math.h>
#include <stdio.h>
using namespace std;


int string2int(string str)
{
    int strLen = str.length();
    if(strLen==0) return -1;
    
    char * strTemp = new char[strLen+1];
	int digits=0;
	int result=0;
	
	//Parse the valid numbers on string 
	for(int i=0; i<strLen; i++)
	{
         
	    if(str.data()[i]>47 & str.data()[i]<58) //Is number? ASCII TABLE
	    {
	        strTemp[i]=str.data()[i];
	        digits++;
	    }
	    else
        {
	        strTemp[i]='0';
	        if(i)
	            break;
	        else
	            return -1;
	    }
	}
	//Calculate the integer
    int index=digits;
    
	while(digits)
	{
	    result += (strTemp[index-digits]-48)*pow(10,digits-1);
        digits --;
	}
	
    delete[] strTemp;
    return result;
}

void sortArray(string * array, int len, bool forward=true)
{
    if(len<2) return;
    //make a copy of array
    string * arrayTemp = new string[len];
    memcpy (arrayTemp, array, len * sizeof(string) );
    string tmpValue;
    int tmpPos;
    //sort array in ascend mode
    for(int i=0; i<len; i++)
    {
        
        //find maximum:
        tmpValue=arrayTemp[i];
        tmpPos=i;
        
        for(int y=0; y<len; y++)
        {
            bool cmp = forward?(arrayTemp[y]<tmpValue):(arrayTemp[y]>tmpValue);
            if(cmp)
            {
                tmpValue=arrayTemp[y];
                tmpPos=y;
            }
        }
        //Set "minimum" to position i,
        arrayTemp[tmpPos]=forward?char(255):char(0);
        array[i]=tmpValue;        
    }

    delete[] arrayTemp;
}

int splitString(string str, string * array, char a)
{
    int strLen = str.length();
    if(strLen==0) return -1;

    string strTemp = "";
	
    int words = 0;
    int nchars = 0;
	//Parse the string for spaces 
	for(int i=0; i<strLen; i++)
	{
	    if(str.data()[i]==a) //Is a? 
	    {
	        if(nchars)  // skip multiple spaces
            {
                array[words] = strTemp;
	            words++;
            }
            nchars=0;
            strTemp="";
	    }
        else
        {
            strTemp += str.data()[i];
            nchars++;
            if((i==(strLen-1)) & (nchars > 0)) //Store the last word not saved
            {
                array[words] = strTemp;
	            words++;
            }
        }        
	}
    
    
    return words;
}
    


int main() {
	string input;
	cin >> input;
	
    int N = string2int(input); 
   
    if((N<1) || (N>100)) 
    {
        return 0;
    }

    string * inputArray = new string[N]; 
    getline(cin,input); //flushing the cin stream 
    //Get array of N strings
    for(int i=0; i<N; i++)
    {
        getline(cin,inputArray[i]);
    }
    
    
    //Print array sorted
    for(int i=0; i<N; i++)
    {
        
        string * wordsArray=new string[inputArray[i].length()+1];
        int words = splitString(inputArray[i], wordsArray, ' ');
       // cout << words;
        if(words!=-1)
        {   
            if(words)
            {
                //Sort the line N 
                sortArray(wordsArray,words,false);
                for(int x=0; x<words;x++)
                {
                    cout << wordsArray[x] << ' ';
                }               
            }
            else
            {
                cout << inputArray[i];
            }
            cout << '\n';      
        }
            
        delete[] wordsArray;
    }
    
	delete[] inputArray;
	return 0;
}


