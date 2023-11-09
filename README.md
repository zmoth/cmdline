# cmdline: 一个 C++ 写的简单的命令行解析器

## 关于

一个 C++ 写的简单的命令行解析器。

- 简单易用
- 只有一个头文件
- 自动类型检查

## 案例

下面是简单的使用案例

### 基本使用

这是一个简单的使用案例

```cpp
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
```

一些运行结果:

```bash
$ ./simple
usage: simple --host=string [options] ...
options:
      --host    host name (string)
  -p, --port    port number (int [=80])
  -t, --type    protocol type (string [=http])
      --gzip    gzip when transfer
  -q, --quit    quit
      --exit    quit
  -h, --help    print this message
$
```

```bash
$ ./simple -h
usage: simple --host=string [options] ...
options:
      --host    host name (string)
  -p, --port    port number (int [=80])
  -t, --type    protocol type (string [=http])
      --gzip    gzip when transfer
  -q, --quit    quit
      --exit    quit
  -h, --help    print this message
$
```

```bash
$ ./simple --host=github.com
http://github.com:80
$
```

```bash
$ ./simple --host=github.com -t ftp
ftp://github.com:80
$
```

```bash
$ ./simple --host=github.com -t ttp
option value is invalid: --type=ttp
usage: simple --host=string [options] ...
options:
      --host    host name (string)
  -p, --port    port number (int [=80])
  -t, --type    protocol type (string [=http])
      --gzip    gzip when transfer
  -q, --quit    quit
      --exit    quit
  -h, --help    print this message
$
```

```bash
$ ./simple --host=github.com -p 4545
http://github.com:4545
$
```

```bash
$ ./simple --host=github.com -p 100000
option value is invalid: --port=100000
usage: simple --host=string [options] ...
options:
      --host    host name (string)
  -p, --port    port number (int [=80])
  -t, --type    protocol type (string [=http])
      --gzip    gzip when transfer
  -q, --quit    quit
      --exit    quit
  -h, --help    print this message
$
```

```bash
$ ./simple --host=github.com --gzip
http://github.com:80
gzip
$
```

```bash
$ ./simple -q
$
```

### 其他用法

- 其余参数

其余参数由 `rest()` 方法引用。
它返回 string 类型的 vector。
通常，它们用于指定文件名等。

```cpp
for (int i = 0; i < a.rest().size(); i++) {
    std::cout << a.rest()[i] << std::endl;
}
```

- 脚注

```cpp
footer() // 该函数可以添加脚注
```

```cpp
...
a.footer("filename ...");
...
```

结果:

```bash
$ ./simple
usage: simple --host=string [options] ... filename ...
options:
      --host    host name (string)
  -p, --port    port number (int [=80])
  -t, --type    protocol type (string [=http])
      --gzip    gzip when transfer
  -q, --quit    quit
      --exit    quit
  -h, --help    print this message
$
```

- 程序名称

解析器在打印使用方法时会打印程序名称。默认的程序名称是 argv[0]。`set_program_name()`函数可以重新设置程序名称。

## 手动处理

`parse_check()` 方法能够解析命令行参数、检查错误、打印帮助信息。

您可以手动执行此过程。
`bool parse()` 方法能够解析命令行参数，如果无效则返回 false。
之后你应该检查一下结果，然后自己做你想做的。
