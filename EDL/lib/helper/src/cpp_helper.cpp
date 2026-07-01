#include"cpp_helper.hpp"

bool TypeConverter::checkTypeToInt(const std::string strData){
    if(strData.length() == 0){
        for(int i = 0;i < strData.length();i++){
            if(!isdigit(strData[i])){
                return false;
            }
        }
    }
    return true;
}