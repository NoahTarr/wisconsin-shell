#include "batchCommand.h"

namespace wish
{
    BatchCommand::BatchCommand(std::string BatchCommandLine) 
    {
        this->EXEC_DIRECTORY = "/bin";
        this->args = NULL;
        this->isBuiltIn = false;
        parseBatchCommand(parseAndFix(BatchCommandLine));
        if (!(this->command.compare("exit") && this->command.compare("cd") && this->command.compare("path")))
            this->isBuiltIn = true;
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
            pid_t pid = fork();
            if (pid < 0) //fork failed
                return false;
            else if (pid == 0)
            {
                std::string execPath = this->EXEC_DIRECTORY + "/" + this->command;
                char* const* execArgs = getExecArgs();
                execv(execPath.c_str(), execArgs);
                char error_message[30] = "An error has occurred\n";
                write(STDERR_FILENO, error_message, strlen(error_message));
                exit(1);
            }
            else
            {
                if (wait(NULL) == -1)
                    return false;
            }
            
        }
        else 
        {
            //TODO: Check Builtins for argument errors
            return executeBuiltIn();
        }

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
    
    std::string BatchCommand::parseAndFix(std::string batchCommand) 
    {
        std::string fixedBatchCommand = "";
        char crntChar;
        char lastAdded;
        size_t i = 0;
        for (std::string::const_iterator it = batchCommand.begin(); it != batchCommand.end(); ++it)
        {
            crntChar = *it.base();
            if (crntChar == ' ' ||  crntChar == '\t' || crntChar == '\r' || crntChar == '\0') 
            {
                if (lastAdded != ' ')
                {
                    fixedBatchCommand += ' ';
                    lastAdded = ' ';
                }
            }
            else
            {
                fixedBatchCommand += crntChar;
                lastAdded = crntChar;
            }
        }
        //End with an extra white space to simplify parse code
        if (lastAdded != ' ')
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
        argsAsCStrings[0] = strdup(this->command.c_str());
        size_t i = 1;
        for (std::vector<std::string>::iterator it = this->args->begin(); it != this->args->end(); ++it) 
        {
            argsAsCStrings[i++] = strdup(it.base()->c_str());
        }
        //Must be NULL terminated
        argsAsCStrings[this->args->size() + 1] = NULL;
        return argsAsCStrings;
    }
    
    bool BatchCommand::CDHasArgErrors() const
    {
        return (this->args->size() == 0 || this->args->size() > 1);
    }
    
    bool BatchCommand::exitHasArgErrors() const
    {
        return this->args->size() > 0;
    }
    
    bool BatchCommand::executeBuiltIn() 
    {
        if (!this->command.compare("cd")) 
        {
            //TODO: built in cd function
            if (CDHasArgErrors())
                return false;
            const char* newDir = this->args->at(0).c_str();
            if (chdir(newDir))
                return false;
        }
        else if (!this->command.compare("path")) 
        {
            //TODO: built in path function
        }
        else if (!this->command.compare("exit")) 
        {
            if (exitHasArgErrors())
                return false;
            exit(0);
        }
        else
            return false;
        return true;
    }
}