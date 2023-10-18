#include <cmdline/cmdline.h>

#include <iostream>

int main()
{
    cmdline::parser parser;
    parser.add<std::string>("host", 0, "host name", true, "");
    parser.add<int>("port", 'p', "port number", false, 80, cmdline::range(1, 65535));
    parser.add<std::string>("type", 't', "protocol type", false, "http",
                            cmdline::oneof<std::string>("http", "https", "ssh", "ftp"));

    // 退出
    parser.add("quit", 'q', "quit");
    parser.add("exit", 0, "quit");
    // 帮助
    parser.add("help", 'h', "print this message");

    parser.footer("filename ...");
    parser.set_program_name("sh");

    while (true) {
        std::cout << "> ";

        std::string input{};
        std::getline(std::cin, input);
        bool const ok = parser.parse(std::string("> ") + input);

        if (parser.exist("quit") || parser.exist("exit")) {
            break;
        }

        if (parser.exist("help")) {
            std::cout << parser.usage();
            continue;
        }

        if (!ok) {
            std::cerr << parser.error() << std::endl;
            std::cout << parser.usage();
            continue;
        }

        std::cout << parser.get<std::string>("host") << ":" << parser.get<int>("port") << std::endl;

        for (const auto &i : parser.rest()) {
            std::cout << "- " << i << std::endl;
        }
    }

    return 0;
}
