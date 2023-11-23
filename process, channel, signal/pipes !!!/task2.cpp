#include <stdio.h>
#include <iostream>
#include <sys/file.h>
#include <unistd.h>
#include <vector>
#include <sstream>
#include <fstream>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>

std::vector<std::string> split(std::string s)
{
    std::vector<std::string> res;
    std::string ss = "";
    for(size_t i = 0; i < s.length(); ++i)
    {
        if(s[i] == ' ')
        {
            if(ss != "")
                res.push_back(ss);
            ss = "";
        }
        else
            ss += s[i];
    }
    if(ss != "")
        res.push_back(ss);
    return res;
}

std::vector<std::vector<std::string>> parse(std::string s)
{
    std::vector<std::vector<std::string>> res(1);
    for(auto el : split(s))
    {
        if(el == "|")
        {
            res.push_back(std::vector<std::string>());
        }
        else
        {
            res[res.size() - 1].emplace_back(std::move(el));
        }
    }
    return res;
}

std::vector<std::vector<char *>> to_c_str(std::vector<std::vector<std::string>> &&coms)
{
    std::vector<std::vector<char *>> res(coms.size());
    for(uint i = 0; i < coms.size(); ++i)
    {
        res[i] = std::vector<char *>(coms[i].size() + 1);
        for(uint j = 0; j < coms[i].size(); ++j)
        {
            res[i][j] = new char[coms[i][j].length() + 1];
            strcpy(res[i][j], coms[i][j].c_str());
        }
        res[i][coms[i].size()] = nullptr;
    }
    return res;
}

int main()
{
    std::string s;
    getline(std::cin, s);
    auto coms = to_c_str(std::move(parse(s)));
    int prevIN = 0;
    for(int i = coms.size() - 1; i >= 0; --i)
    {
        int fd[2];
        pipe(fd);
        if(fork())
        {
            close(fd[1]);
            int status;
            wait(&status);
        }
        else
        {
            close(fd[0]);
            if(i != coms.size() - 1)
            {
                close(0);
                dup2(prevIN, 0);
                close(prevIN);
            }
            if(i != 0)
            {
                close(1);
                dup2(fd[1], 1);
                close(fd[1]);
                execvp(coms[i][0], &coms[i][0]);
                close(1);
                close(0);
            }
            else
            {
                close(1);
                close(fd[1]);
                int newfd = open("result.out", O_WRONLY | O_CREAT | O_TRUNC, 0666);
                dup2(newfd, 1);
                execvp(coms[i][0], &coms[i][0]);
                close(0);
                close(newfd);
            }
            for(uint j = 0; j < coms[i].size(); ++j)
                delete[] coms[i][j];
            signal(SIGTERM, SIG_DFL);
            exit(0);
        }
        prevIN = fd[0];
    }
    return 0;
}