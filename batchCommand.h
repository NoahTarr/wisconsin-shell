#ifndef __BATCHCOMMAND_H__
#define __BATCHCOMMAND_H__

#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

namespace wish 
{
    class BatchCommand
    {
        private:
            std::string EXEC_DIRECTORY;
            std::string command;
            std::vector<std::string> *args;
            bool isBuiltIn;

        public:
            BatchCommand(std::string BatchCommandLine);
            ~BatchCommand();
            bool executeBatchCommand();
            std::string getCommand();
            std::vector<std::string> getArgs();
            std::string getExecutionDirectory();
            std::string resetExecutionDirectory();
            bool executionDirectoryIsDefault();

        private:
            void parseBatchCommand(std::string batchCommand);
            std::string parseAndFix(std::string batchCommand);
            void setExecutionDirectory(std::string newExecDirectory);
            char* const* getExecArgs();
            bool CDHasArgErrors() const;
            bool exitHasArgErrors() const;
            bool executeBuiltIn();
    };
}



#endif // __BATCHCOMMAND_H__