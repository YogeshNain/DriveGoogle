#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <dirent.h>

#ifdef _WIN32
#include <windows.h>
#include <Lmcons.h>
#include <io.h>
#include <time.h>
#endif
#ifdef _linux
#include <linux.h>
#endif
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
//https://accounts.google.com/o/oauth2/auth?scope=https%3A%2F%2Fwww.googleapis.com%2Fauth%2Fdrive&redirect_uri=urn:ietf:wg:oauth:2.0:oob&response_type=code&client_id=873048095801-ttmv2nrpud8trq24e4tdbsir5stf0bej.apps.googleusercontent.com
//
//
//
//
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

size_t handleResponse(char *ptr, size_t size, size_t nmemb, void *stream)
{
    int i;
    size_t len = size * nmemb;

#ifdef debug
    printf("\nResponse size %u\n", len); 
#endif


    for(i = 0; i < len; i++){
        fprintf((FILE *) stream,"%c", ptr[i]);

#ifdef debug
        printf("\nResponse %c\n", ptr[i]); 
#endif

    }

    return len;
}

void auth_tok()
{
char url[] = "https://www.googleapis.com/oauth2/v3/token";
char scope[100] = "scope=https%3A%2F%2Fwww.googleapis.com%2Fauth%2Fdrive";
char redirect_uri[]= "&redirect_uri=urn:ietf:wg:oauth:2.0:oob";
char response_type[]="&response_type=code";
//char client_id[1024]="&client_id=873048095801-ttmv2nrpud8trq24e4tdbsir5stf0bej.apps.googleusercontent.com";
char client_id[1024] = "&client_id=852034184117-j123aml7k7ntcj7s0o4sh1n11qakirbl.apps.googleusercontent.com";
char grant_type[1024]="&grant_type=authorization_code";
char code[1024] = "&code=4/MxQXNkYVGmt0I3Xv3PASA16bX4ioAcn7IbSwsdiO-ao";
//char client_secret[1024] = "&client_secret=kGeuPRejqpy0X2vw7tz4EiBS";
char client_secret[1024] = "&client_secret=wkCz8GiZNly7-WMlWfbi7Mah";

CURL *curl;
CURLcode res;
char POST_DATA[2048] ={0};
FILE *auth = fopen("auth_tok.txt","w");
curl = curl_easy_init();
  if(curl) {
	 
	strcpy(POST_DATA,scope);
	strcpy(POST_DATA,grant_type);
	strcat(POST_DATA,redirect_uri);
	strcat(POST_DATA,response_type);
	strcat(POST_DATA,client_id);
	strcat(POST_DATA,code);
	strcat(POST_DATA,client_secret);
	printf("POST_DATA is : %s\n",POST_DATA);
    curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, POST_DATA);
	curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/44.0.2403.125 Safari/537.36");
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, handleResponse);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, auth);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    res = curl_easy_perform(curl);
	 if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
      curl_easy_strerror(res));
	  curl_easy_cleanup(curl);
	   }
  //return 0;
}

void refresh_tok(){
char url[] = "https://www.googleapis.com/oauth2/v3/token";
//char client_id[1024]="client_id=873048095801-ttmv2nrpud8trq24e4tdbsir5stf0bej.apps.googleusercontent.com";
char client_id[1024] = "&client_id=852034184117-j123aml7k7ntcj7s0o4sh1n11qakirbl.apps.googleusercontent.com";
char client_secret[1024] = "&client_secret=wkCz8GiZNly7-WMlWfbi7Mah";
char refresh_token[1024] = "&refresh_token=1/mFpUDKKel-iIo60lqJsA0BiOBE3mX-T-6oF2-f4yCE8";
char grant_type [1024] = "&grant_type=refresh_token";


CURL *curl;
CURLcode res;
char POST_DATA[2048] ={0};
FILE *refresh = fopen("refresh_tok.txt","w");
curl = curl_easy_init();
  if(curl) {
	 
	strcpy(POST_DATA,client_id);
	strcat(POST_DATA,client_secret);
	strcat(POST_DATA,refresh_token);
	strcat(POST_DATA,grant_type);
    curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, POST_DATA);
	curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/44.0.2403.125 Safari/537.36");
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, handleResponse);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, refresh);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    res = curl_easy_perform(curl);
	 if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
      curl_easy_strerror(res));
	  curl_easy_cleanup(curl);
	   }
//  return 0;

}


int main()

{

//	auth_tok();
    refresh_tok();

	return 0;
}
