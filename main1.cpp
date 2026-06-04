
//cmake -B build ^ -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake
//    ./build/Debug/hello.exe


#include <curl/curl.h>
#include <iostream>


void set(CURL* c,std::string x){

    x = "http://192.168.4.1/" + x;

    curl_easy_setopt(c, CURLOPT_URL,x.c_str());
    curl_easy_setopt(c, CURLOPT_NOBODY,1L);
    curl_easy_setopt(c, CURLOPT_HTTPGET,1L);
    curl_easy_perform(c);
    
}

int main(){

    
    CURL* c = curl_easy_init();
    if(!c){
        printf("Failed to init Curl");
    }


    for(int i = 0; i < 100; i++){
        set(c, "on");
        Sleep(200);
        set(c, "off");
        Sleep(200);
    }
    
    
    
    curl_easy_cleanup(c);
    return 0;
}