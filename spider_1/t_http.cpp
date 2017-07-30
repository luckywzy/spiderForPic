/*************************************************************************
	> File Name: t_http.cpp
	> Author: 
	> Mail: 
	> Created Time: 2017年07月26日 星期三 23时12分18秒
 ************************************************************************/
#include"parsepage.h"
#include"httpdownload.h"
#include"downloadImage.h"
#include<iostream>
#include<set>
#include<queue>
#include<algorithm>
#include<cstdio>
using namespace std;

string getImagename(const string& url)
{
    string res(url.end()-20, url.end());
    for(int i=0; i<res.length(); i++)
    {
        if(res[i] == '/')
            res[i] = '_';
    }
    return res;
}

int main()
{
    //下载页面
    string url = "www.meizitu.com";
    httpdownload h(url);
    if(!h.run())
    {
        cout<<"parse http failed\n";
    }
    //cout<<h.getcontent()<<"\n";
    //保存到文件
   /* FILE* fp = open("页面.txt","wb");
    string& pstr = h.getcontent();
    fwrite(pstr.c_str(), pstr.length(), 1, fp);
    */
    cout<<"length:\t"<<h.size()<<"\n";
    //解析page
    string pa = "http(s)?://([\\w-]+\\.)+[\\w-]+(/[\\w-./?%&=]*)?";
    pa = "[[:alpha:]]*" + pa + "[[:alpha:]]*";
    regex reg(pa);
    parsepage re(reg, h.getcontent());
    re.match();
    set<string>& purls = re.getJpgUrl();
    set<string>& hurls = re.getHtmlUrl();
    
    cout<<"log: pic size: "<<purls.size()<<"\n";
    cout<<"log: html size: "<<hurls.size()<<"\n";

    //试下载图片
    int cnt=0;
    for(auto& it: purls)
    {
        string name = getImagename(it);
        downloadImage ima(it, name);
        if(! ima.run())
        {
            cout<<"log: download: "<<it<<" failed\n";
            continue;
        }
        cout<<"log: download: "<<++cnt<<"\t pics\n";
    }
    cout<<"log: total is : "<<cnt <<" pictures\n";
    queue<string> que;
    for(auto& it: hurls)
    {
        que.push(it);
    }

   //将html的url 做成一个队列，然后下载
    while(! que.empty())
    {
        //下载页面
        httpdownload hd(que.front());
        if(hd.run())
        {
            cout<<"log: download html failed\n";
        }
        //解析页面
        parsepage psp(reg, hd.getcontent());
        psp.match();
        set<string>& jpgUrl = psp.getJpgUrl();
        set<string>& htmlUrl = psp.getHtmlUrl();
        
        cout<<"log: jpg count: "<<jpgUrl.size()<<"\n";
        //下载图片
        for_each(jpgUrl.begin(), 
                 jpgUrl.end(), 
                 [](string url){
                    string name = getImagename(url);
                    downloadImage image(url, name);
                    if(! image.run()){
                        cout<<"log: download failed : "<<url<<"\n";
                    }
                    else{
                        cout<<"log: download succeed\n";
                    }
        });

        //加入队列    
        cout<<"log: new url count: "<<htmlUrl.size()<<"\n";
        for(auto& hit: htmlUrl)
        {
            que.push(hit);
        }
        //删除已经下载的url
        que.pop();
    }

    return 0;
}

