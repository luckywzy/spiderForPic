/*************************************************************************
	> File Name: downloadImage.cpp
	> Author: 
	> Mail: 
	> Created Time: 2017年07月28日 星期五 21时42分09秒
 ************************************************************************/
#include"downloadImage.h"
#include<stdio.h>
#include<curl/curl.h>
#include<string>
#include<iostream>
using namespace std;

downloadImage::downloadImage(string url, string name):
    ImaUrl(url),
    ImaName(name)
    {

    }

void downloadImage::setImageName(string name)
{
    ImaName = name;
}
size_t downloadImage::write_image(void* ptr, size_t size, size_t nmemb, FILE* stream)
{
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

bool downloadImage::run()
{
    CURL* curl = curl_easy_init();
    FILE* fd = fopen(ImaName.c_str(), "wb");
    CURLcode res;
    if(fd == NULL)
    {
        cout<<"log: file open failed\n";
        return false;
    }

    curl_easy_setopt(curl, CURLOPT_URL, ImaUrl.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_image);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fd);
    res = curl_easy_perform(curl);
    if(res != 0)
    {
        curl_easy_cleanup(curl);
        cout<<"log: download file faile\n";
        return false;
    }
    
    curl_easy_cleanup(curl);
    fclose(fd);
    
    cout<<"log: download succeed\t"<<ImaUrl<<"\n";
    return true;
}


