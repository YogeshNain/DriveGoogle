#include <stdio.h>
#include <curl/curl.h>
#include <linux.h>
#include <sys/stat.h>
#include <dirent.h>

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


static size_t read_callback(void *ptr, size_t size, size_t nmemb, void *stream)
{
  curl_off_t nread;
  /* in real-world cases, this would probably get this data differently
     as this fread() stuff is exactly what the library already would do
     by default internally */ 
  size_t retcode = fread(ptr, size, nmemb, stream);
 
  nread = (curl_off_t)retcode;
 
#ifdef debug
  fprintf(stderr, "*** We read %" CURL_FORMAT_CURL_OFF_T
          " bytes from file\n", nread);
#endif
  return retcode;
}

void refresh_tok(){

char url[] = "https://www.googleapis.com/oauth2/v3/token";

char client_id[1024]="client_id=873048095801-ttmv2nrpud8trq24e4tdbsir5stf0bej.apps.googleusercontent.com";

char client_secret[1024] = "&client_secret=kGeuPRejqpy0X2vw7tz4EiBS";

char refresh_token[1024] = "&refresh_token=1/QQJ6IXYKw6r-k-bVeK14bqBDOteMzF-3rKK48EreXRtIgOrJDtdun6zK6XiATCKT";

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


void auth_tok()

{

char url[] = "https://www.googleapis.com/oauth2/v3/token";

char scope[100] = "scope=https%3A%2F%2Fwww.googleapis.com%2Fauth%2Fdrive";

char redirect_uri[]= "&redirect_uri=urn:ietf:wg:oauth:2.0:oob";

char response_type[]="&response_type=code";

char client_id[1024]="&client_id=873048095801-ttmv2nrpud8trq24e4tdbsir5stf0bej.apps.googleusercontent.com";

char grant_type[1024]="&grant_type=authorization_code";

char code[1024] = "&code=4/7OCySu-pVlE4oAMzzTVn7JAsldhU62H69CKcem6XGCA";

char client_secret[1024] = "&client_secret=kGeuPRejqpy0X2vw7tz4EiBS";

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


void uploadFile(char *fileName)
    {
    
    
  printf("\nUploading file ... %s\n", fileName);
  CURL *curl;
  CURLcode res;
  FILE *hd_src;
  struct stat file_info;
  curl_off_t fsize;
 
  /* get the file size of the local file */ 
  if(stat(fileName, &file_info)) {
    printf("Couldnt open '%s': %s\n", fileName, strerror(errno));
    return ;
  }
  fsize = (curl_off_t)file_info.st_size;
 
  printf("Local file size: %" CURL_FORMAT_CURL_OFF_T " bytes.\n", fsize);
 
  /* get a FILE * of the same file */ 
  hd_src = fopen(fileName, "rb");
 
  /* In windows, this will init the winsock stuff */ 
  curl_global_init(CURL_GLOBAL_ALL);
 
  /* get a curl handle */ 
  curl = curl_easy_init();
  if(curl) {
 
    /* we want to use our own read function */ 
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
 
    /* enable uploading */ 
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
 
    /* specify target */ 
    curl_easy_setopt(curl,CURLOPT_URL, POST_ADDR);
    curl_easy_setopt(curl, CURLOPT_PORT, 2901);
 
    /* now specify which file to upload */ 
    curl_easy_setopt(curl, CURLOPT_READDATA, hd_src);
    
    curl_easy_setopt(curl, CURLOPT_USE_SSL, 0L);

    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
     
    /* Set the size of the file to upload (optional).  If you give a *_LARGE
       option you MUST make sure that the type of the passed-in argument is a
       curl_off_t. If you use CURLOPT_INFILESIZE (without _LARGE) you must
       make sure that to pass in a type 'long' argument. */ 
    curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE,
                     (curl_off_t)fsize);
 
    /* Now run off and do what you've been told! */ 
    res = curl_easy_perform(curl);
    /* Check for errors */ 
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
  
    /* always cleanup */ 
    curl_easy_cleanup(curl);
  }
  fclose(hd_src); /* close the local file */ 
 
  curl_global_cleanup();

}
        
        
void uploadFilePath(char *filePath, char *fileName)
    {
    
    
    
    printf("\nUploading file ... %s\n", file);

  CURL *curl;
  CURLcode res;
  FILE *hd_src;
  struct stat file_info;
  curl_off_t fsize;
 
  /* get the file size of the local file */ 
  if(stat(fileName, &file_info)) {
    printf("Couldnt open '%s': %s\n", file, strerror(errno));
    return ;
  }
  fsize = (curl_off_t)file_info.st_size;
 
  printf("Local file size: %" CURL_FORMAT_CURL_OFF_T " bytes.\n", fsize);
 
  /* get a FILE * of the same file */ 
  hd_src = fopen(filePath, "rb");
 
  /* In windows, this will init the winsock stuff */ 
  curl_global_init(CURL_GLOBAL_ALL);
 
  /* get a curl handle */ 
  curl = curl_easy_init();
  if(curl) {
 
    /* we want to use our own read function */ 
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
 
    /* enable uploading */ 
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
 
    /* specify target */ 
    curl_easy_setopt(curl,CURLOPT_URL, POST_ADDR);
    curl_easy_setopt(curl, CURLOPT_PORT, 2901);
 
    /* now specify which file to upload */ 
    curl_easy_setopt(curl, CURLOPT_READDATA, hd_src);
    
    curl_easy_setopt(curl, CURLOPT_USE_SSL, 0L);

    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
     
    /* Set the size of the file to upload (optional).  If you give a *_LARGE
       option you MUST make sure that the type of the passed-in argument is a
       curl_off_t. If you use CURLOPT_INFILESIZE (without _LARGE) you must
       make sure that to pass in a type 'long' argument. */ 
    curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE,
                     (curl_off_t)fsize);
 
    /* Now run off and do what you've been told! */ 
    res = curl_easy_perform(curl);
    /* Check for errors */ 
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
  
    /* always cleanup */ 
    curl_easy_cleanup(curl);
  }
  fclose(hd_src); /* close the local file */ 
  curl_global_cleanup();
}


int main(){


 refresh_tok();

}
