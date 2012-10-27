#include "comandexecuter.h"
#include <stdio.h>
#include <stdlib.h>

void ComandExecuter::PrintVec(std::vector<std::string> vec)
{
    for (int i = 0; i < vec.size(); i++)
    {
        printf("%d:%s", i, vec[i].c_str());
    }
}

std::vector<std::string> ComandExecuter::Execute(std::string cmd)
{
    std::vector<std::string> res;
    FILE* ptr = popen(cmd.c_str(), "r");

    if (ptr != NULL)
    {
        char buf[BUFSIZ];
        while (fgets(buf, BUFSIZ, ptr) != NULL)
            res.push_back(buf);
            //(void) printf("~>%s", buf);
        (void) pclose(ptr);
    }
    return res;
}

ComandExecuter::ComandExecuter()
{

}
