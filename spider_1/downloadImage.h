/*************************************************************************
	> File Name: downloadImage.h
	> Author: 
	> Mail: 
	> Created Time: 2017年07月28日 星期五 21时36分23秒
 ************************************************************************/

#ifndef _DOWNLOADIMAGE_H
#define _DOWNLOADIMAGE_H
#include<curl/curl.h>
#include<cstdio>
#include<string>
#include<iostream>
using namespace std;
class downloadImage
{
public:
    downloadImage(const string url, string name);
    void setImageName(string name);
    bool run();
    ~downloadImage(){};
private:
    static size_t write_image(void* ptr, size_t size, size_t nmemb, FILE* stream);
private:
    string ImaUrl;
    string ImaName;    
};

#endif
