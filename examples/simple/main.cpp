/*
Copyright (c) 2009, Hideyuki Tanaka
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the <organization> nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY <copyright holder> ''AS IS'' AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL <copyright holder> BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <cmdline/cmdline.h>

#include <iostream>

int main(int argc, char *argv[])
{
    // 创建一个解析器
    cmdline::parser a;

    // 添加指定类型的变量。
    // 第1个参数是全称
    // 第2个参数是缩写，只有一个字符. '\0'表示不设置
    // 第3个参数是描述
    // 第4个参数是强制标识，如果是true表示这个参数必须被填入，默认是false
    // 第6个参数是默认值
    a.add<std::string>("host", '\0', "host name", true, "");

    // 第6个参数是额外的约束
    // 这里表示，port 参数内容需要被限制在1～65535之间
    a.add<int>("port", 'p', "port number", false, 80, cmdline::range(1, 65535));

    // cmdline::oneof() 函数也可以约束，使用户只能选择则其一
    a.add<std::string>("type", 't', "protocol type", false, "http",
                       cmdline::oneof<std::string>("http", "https", "ssh", "ftp"));

    // 也可以定义布尔参数，只要参数里包含就可以
    // 调用不带类型参数的add方法
    a.add("gzip", '\0', "gzip when transfer");

    a.set_program_name("simple");

    // 运行解析器
    // 它仅在命令行参数有效时返回。
    // 如果参数无效，解析器将输出错误消息，然后退出程序。
    // 如果指定了帮助标志('--help'或'-h')，解析器将输出使用信息，然后退出程序。
    a.parse_check(argc, argv);

    // 获取参数
    std::cout << a.get<std::string>("type") << "://" << a.get<std::string>("host") << ":" << a.get<int>("port")
              << std::endl;

    // 布尔参数可以用 exist() 获取
    if (a.exist("gzip")) {
        std::cout << "gzip" << std::endl;
    }

    return 0;
}
