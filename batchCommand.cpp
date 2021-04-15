#include "batchCommand.h"

namespace wish
{
    BatchCommand::BatchCommand(std::string BatchCommandLine) 
    {
        this->EXEC_DIRECTORY = "/bin";
        this->args = NULL;
        parseBatchCommand(fixRepeatingWhiteSpace(BatchCommandLine));
    }
    
    BatchCommand::~BatchCommand() 
    {
        //TODO: desctructor
        delete (this->args);
    }
    
    bool BatchCommand::executeBatchCommand() 
    {
        if (!this->isBuiltIn) 
        {
            // std::string execPath = this->EXEC_DIRECTORY + "/" + this->command;
            char* const* execArgs = getExecArgs();
            execv(execArgs[0], execArgs);
        }
        else 
            executeBuiltIn();

        return true;
    }
    
    std::string BatchCommand::getCommand() 
    {
        return this->command;
    }
    
    std::vector<std::string> BatchCommand::getArgs() 
    {
        return *this->args;
    }
    
    std::string BatchCommand::getExecutionDirectory() 
    {
        return this->EXEC_DIRECTORY;
    }
    
    std::string BatchCommand::resetExecutionDirectory() 
    {
        this->EXEC_DIRECTORY = "/bin";
        return getExecutionDirectory();
    }
    
    bool BatchCommand::executionDirectoryIsDefault() 
    {
        return this->EXEC_DIRECTORY == "/bin";
    }
    
    void BatchCommand::parseBatchCommand(std::string batchCommand) 
    {
        //TODO: Improve this by using a start and end pntr instead string.erase()
        std::string deliminator = " ";
        size_t deliminatorIndex = batchCommand.find(deliminator);
        this->args = new std::vector<std::string>;
        this->command = batchCommand.substr(0, deliminatorIndex);
        batchCommand.erase(0, deliminatorIndex + deliminator.length());
        std::string arg;
        while ((deliminatorIndex = batchCommand.find(deliminator)) != std::string::npos)
        {
            arg = batchCommand.substr(0, deliminatorIndex);
            batchCommand.erase(0, deliminatorIndex + deliminator.length());
            if (arg != deliminator) 
                this->args->push_back(arg);
        }
        
    }
    
    std::string BatchCommand::fixRepeatingWhiteSpace(std::string batchCommand) 
    {
        std::string fixedBatchCommand = "";
        char crntChar;
        char lastAdded;
        size_t i = 0;
        for (std::string::const_iterator it = batchCommand.begin(); it != batchCommand.end(); ++it)
        {
            crntChar = *it.base();
            if (std::isblank(crntChar) && lastAdded != ' ') 
            {
                fixedBatchCommand += ' ';
                lastAdded = ' ';
            }
            else if (!std::isblank(crntChar))
            {
                fixedBatchCommand += crntChar;
                lastAdded = crntChar;
            }
        }
        //End with an extra white space to simplify parse code
        fixedBatchCommand += ' ';
        return fixedBatchCommand;
    }
    
    void BatchCommand::setExecutionDirectory(std::string newExecDirectory) 
    {
        this->EXEC_DIRECTORY = newExecDirectory;
    }
    
    char* const* BatchCommand::getExecArgs() 
    {
        //FIXME: This may need fixing
        char ** argsAsCStrings = new char*[this->args->size() + 2];
        std::string execPath = this->EXEC_DIRECTORY + "/" + this->command;
        argsAsCStrings[0] = strdup(execPath.c_str());
        size_t i = 1;
        for (std::vector<std::string>::iterator it = this->args->begin(); it != this->args->end(); ++it) 
        {
            argsAsCStrings[i++] = strdup(it.base()->c_str());
        }
        //Must be NULL terminated
        argsAsCStrings[this->args->size() + 1] = NULL;
        return argsAsCStrings;
    }
    
    void BatchCommand::executeBuiltIn() 
    {
        const char* builtInCommand = this->command.c_str();
        if (builtInCommand == "cd") 
        {
            //TODO: built in cd function
        }
        else if (builtInCommand == "path")
        {
            //TODO: built in path function
        }
        else
            exit(0);
    }
}