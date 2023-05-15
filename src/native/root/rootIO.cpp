#include "native/root/rootIO.h"
#include "value/dataType.h"
#include "semantics/astRunner.h"
#include "value/import.h"
#include <fstream>
#include <windows.h>

namespace Snapp::Native {

    FunctionValue createPrintFunction() {
        FunctionValue printFunction;
        printFunction.addOverload({
            DataType::Void,
            {DataType::Str},
            [](const std::vector<DataValue>& args, const Scope* scope) {
                std::cout << *coerceStr(args.at(0)) << std::endl;
                return std::nullopt;
            }
        });
        printFunction.addOverload({
            DataType::Void,
            {DataType::Int},
            [](const std::vector<DataValue>& args, const Scope* scope) {
                std::cout << *coerceInt(args.at(0)) << std::endl;
                return std::nullopt;
            }
        });
        printFunction.addOverload({
            DataType::Void,
            {DataType::Float},
            [](const std::vector<DataValue>& args, const Scope* scope) {
                std::cout << *coerceFloat(args.at(0)) << std::endl;
                return std::nullopt;
            }
        });
        printFunction.addOverload({
            DataType::Void,
            {DataType::Bool},
            [](const std::vector<DataValue>& args, const Scope* scope) {
                std::cout << (*coerceBool(args.at(0)) ? "true" : "false") << std::endl;
                return std::nullopt;
            }
        });
        return printFunction;
    }

    FunctionValue createInputFunction() {
        FunctionValue inputFunction;
        inputFunction.addOverload({
            DataType::Str,
            {DataType::Str},
            [](const std::vector<DataValue>& args, const Scope* scope) {
                std::cout << *coerceStr(args.at(0));
                std::string input;
                std::cin.ignore(); // unclear exactly what this fixes, but getline() doesn't work otherwise
                std::getline(std::cin, input);
                return input;
            }
        });
        return inputFunction;
    }

    FunctionValue instantiatePrintIRL(){
        FunctionValue instantiatePrintIRL;
        instantiatePrintIRL.addOverload({
            DataType::Void,
            {},
            [](const std::vector<DataValue>& args, const Scope* scope) {
                std::ofstream tempfile("temp.txt"); 
                tempfile.close();
                return std::nullopt;
            }
        });
        return instantiatePrintIRL;
    }

    FunctionValue printIRL(){
        FunctionValue printIRL;
        printIRL.addOverload({
            DataType::Void,
            {DataType::Void},
            [](const std::vector<DataValue>& args, const Scope* scope){
                std::ofstream tempFile("tempPrint.txt", std::ios::app);
                tempFile << *convertStr(args.at(0)) + '\n'; 
                tempFile.close();
                return std::nullopt;
            }
        });
        return printIRL;
    }

    FunctionValue lastLine(){
        FunctionValue lastLine;
        lastLine.addOverload({
            DataType::Void,
            {},
            [](const std::vector<DataValue>& args, const Scope* scope){
                // print temp file to default printer
                const char* printername = NULL;
                STARTUPINFO startup_info = {0};
                startup_info.cb = sizeof(STARTUPINFO);
                PROCESS_INFORMATION process_info;
                DWORD exit_code;
    
                // use ShellExecute to print file
                ShellExecute(NULL, "print", "tempPrint.txt", NULL, NULL, SW_HIDE);
                return std::nullopt;
            }
        });
        return lastLine;
    }




    ImportValue* createRootIO() {
        auto scope = new Scope(nullptr, true);
        auto rootIO = new ImportValue(scope);

        scope->add("print", createPrintFunction());
        scope->add("input", createInputFunction());
        scope->add("instantiatePrintIRL", instantiatePrintIRL());
        scope->add("printIRL", printIRL());
        scope->add("lastLine", lastLine());
        return rootIO;
    }

}