#include <stdio.h>
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>

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
void uploadfile(char *fileName){

  CURL *curl;
  CURLcode res;
  char data[] = "This is a test data for file name myfile.txt";
  char url[] = "https://www.googleapis.com/upload/drive/v2/files?uploadType=multipart";  
  char tok[] = "ya29.ygGUnruZf_PLYfyUjf6VMTfaKOyw25vqysE2Uap03ZiXmLFDOvqo4_YIEukkfosKHYj0mA";
  char auth_ber[] = "Authorization: Bearer ";
  char cont_type[] = "Content-Type: multipart/related; boundary=\"my_data\"";
  printf("\ncontent type is : %s\n",cont_type);
  char cont_len[] = "Content-Length: 44";
  char json_cont[] = "Content-Type: application/json; charset=UTF-8";
  char file_content_type[] = "Content-Type: plain/text";
  char head_bound[5000] = "--my_data\n";
  char end_bound[] = "--my_data--";
  char json_obj[] = "{ \"title\": \"My File.txt\"}";
  curl_global_init(CURL_GLOBAL_ALL);
  curl = curl_easy_init();
  if(curl){
        struct curl_slist *chunk = NULL;
        printf("\nURL is :%s\n",url);
        curl_easy_setopt(curl, CURLOPT_URL, url);
        strcat(auth_ber,tok);
        chunk = curl_slist_append(chunk, cont_type);
        chunk = curl_slist_append(chunk, auth_ber); 
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS,head_bound );
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, handleResponse);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, head_bound);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_cont);  
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_obj);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, head_bound);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, file_content_type);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, end_bound);
     /*   strcat(head_bound,json_cont);
         printf("1\n");
        strcat(head_bound,json_obj);
          printf("2\n");
        strcat(head_bound,"--my_data");
          printf("3\n");
        strcat(head_bound,file_content_type);
           printf("4\n");
        strcat(head_bound,data);
            printf("5\n");
        strcat(head_bound,end_bound);
        printf("\n\n%s\n\n",head_bound);*/
        res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
        res = curl_easy_perform(curl);
        if(res != CURLE_OK)
        fprintf(stderr, "curl_easy_perform() failed: %s\n",
        curl_easy_strerror(res));
        curl_easy_cleanup(curl);
  

}
}

int main(){

uploadfile("myfile.txt");

}
