#pragma once

#include "globals.h"
#include <json/json.h>

namespace json
{

    inline static Json::Value parse(const std::string &mystr)
    {
        Json::CharReaderBuilder ReaderBuilder;
        ReaderBuilder["emitUTF8"] = true; // utf8支持,不加这句,utf8的中文字符会编程\uxxx
        // 2.通过工厂对象创建 json阅读器对象
        unique<Json::CharReader> charread(ReaderBuilder.newCharReader());
        // 3.创建json对象
        Json::Value root;
        // 4.把字符串转变为json对象,数据写入root
        std::string strerr;
        bool isok = charread->parse(mystr.c_str(), mystr.c_str() + mystr.size(), &root, &strerr);
        if (!isok || strerr.size() != 0)
        {
            printf("json error");
        }

        return root;
    }

}