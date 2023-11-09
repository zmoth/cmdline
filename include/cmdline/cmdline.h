/// @file cmdline.h
/// @author Hideyuki Tanaka, moth (QianMoth@qq.com)
/// @brief 只有单个头文件的C++命令行工具
/// @details
/// This is an enhancement to [the original project](https://github.com/tanakh/cmdline)
///
/// @copyright Copyright (c) 2009, Hideyuki Tanaka
///
#pragma once

#ifdef __GNUC__
#include <cxxabi.h>
#endif

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <typeinfo>
#include <utility>
#include <vector>

namespace cmdline {

namespace detail {

template <typename Target, typename Source, bool Same>
class lexical_cast_t
{
  public:
    static Target cast(const Source &arg)
    {
        Target ret;
        std::stringstream ss;
        if (!(ss << arg && ss >> ret && ss.eof())) {
            throw std::bad_cast();
        }

        return ret;
    }
};

template <typename Target, typename Source>
class lexical_cast_t<Target, Source, true>
{
  public:
    static Target cast(const Source &arg) { return arg; }
};

template <typename Source>
class lexical_cast_t<std::string, Source, false>
{
  public:
    static std::string cast(const Source &arg)
    {
        std::ostringstream ss;
        ss << arg;
        return ss.str();
    }
};

template <typename Target>
class lexical_cast_t<Target, std::string, false>
{
  public:
    static Target cast(const std::string &arg)
    {
        Target ret;
        std::istringstream ss(arg);
        if (!(ss >> ret && ss.eof())) {
            throw std::bad_cast();
        }
        return ret;
    }
};

/// @brief 是否相等
/// @tparam T1
/// @tparam T2
template <typename T1, typename T2>
struct is_same
{
    static const bool value = false;
};

/// @brief 是否相等
/// @tparam T
template <typename T>
struct is_same<T, T>
{
    static const bool value = true;
};

template <typename Target, typename Source>
Target lexical_cast(const Source &arg)
{
    return lexical_cast_t<Target, Source, detail::is_same<Target, Source>::value>::cast(arg);
}

static inline std::string demangle(const std::string &name)
{
#ifdef _MSC_VER
    return name;  // 为MSVC编译器时直接返回name
#elif defined(__GNUC__)
    // 为gcc编译器时还调用原来的代码
    int status = 0;
    char *p = abi::__cxa_demangle(name.c_str(), nullptr, nullptr, &status);
    std::string ret(p);
    free(p);
    return ret;
#else
// 其他不支持的编译器需要自己实现这个方法
#error unexpected c complier (msc/gcc), Need to implement this method for demangle
#endif
}

template <class T>
std::string readable_typename()
{
    return demangle(typeid(T).name());
}

template <class T>
std::string default_value(T def)
{
    return detail::lexical_cast<std::string>(def);
}

template <>
inline std::string readable_typename<std::string>()
{
    return "string";
}

}  // namespace detail

// ==================================================================
// ==================================================================
// ==================================================================
// ==================================================================

/// @brief 自定义错误类型 cli错误
class cmdline_error : public std::exception
{
  public:
    explicit cmdline_error(std::string msg) : msg(std::move(msg)) {}
    ~cmdline_error() noexcept override = default;
    const char *what() const noexcept override { return msg.c_str(); }

  private:
    std::string msg;
};

template <class T>
struct default_reader
{
    T operator()(const std::string &str) { return detail::lexical_cast<T>(str); }
};

template <class T>
struct range_reader
{
    range_reader(const T &low, const T &high) : low(low), high(high) {}
    T operator()(const std::string &s) const
    {
        T ret = default_reader<T>()(s);
        if (!(ret >= low && ret <= high)) {
            throw cmdline::cmdline_error("range_error");
        }
        return ret;
    }

  private:
    T low, high;
};

template <class T>
range_reader<T> range(const T &low, const T &high)
{
    return range_reader<T>(low, high);
}

template <class T>
struct oneof_reader
{
    T operator()(const std::string &s)
    {
        T ret = default_reader<T>()(s);
        if (std::find(alt.begin(), alt.end(), ret) == alt.end()) {
            throw cmdline_error("");
        }
        return ret;
    }
    void add(const T &v) { alt.push_back(v); }

  private:
    std::vector<T> alt;
};

#pragma region /* oneof_reader */

template <class T>
oneof_reader<T> oneof(T a1)
{
    oneof_reader<T> ret;
    ret.add(a1);
    return ret;
}

template <class T>
oneof_reader<T> oneof(T a1, T a2)
{
    oneof_reader<T> ret;
    ret.add(a1);
    ret.add(a2);
    return ret;
}

template <class T>
oneof_reader<T> oneof(T a1, T a2, T a3)
{
    oneof_reader<T> ret;
    ret.add(a1);
    ret.add(a2);
    ret.add(a3);
    return ret;
}

template <class T>
oneof_reader<T> oneof(T a1, T a2, T a3, T a4)
{
    oneof_reader<T> ret;
    ret.add(a1);
    ret.add(a2);
    ret.add(a3);
    ret.add(a4);
    return ret;
}

template <class T>
oneof_reader<T> oneof(T a1, T a2, T a3, T a4, T a5)
{
    oneof_reader<T> ret;
    ret.add(a1);
    ret.add(a2);
    ret.add(a3);
    ret.add(a4);
    ret.add(a5);
    return ret;
}

template <class T>
oneof_reader<T> oneof(T a1, T a2, T a3, T a4, T a5, T a6)
{
    oneof_reader<T> ret;
    ret.add(a1);
    ret.add(a2);
    ret.add(a3);
    ret.add(a4);
    ret.add(a5);
    ret.add(a6);
    return ret;
}

template <class T>
oneof_reader<T> oneof(T a1, T a2, T a3, T a4, T a5, T a6, T a7)
{
    oneof_reader<T> ret;
    ret.add(a1);
    ret.add(a2);
    ret.add(a3);
    ret.add(a4);
    ret.add(a5);
    ret.add(a6);
    ret.add(a7);
    return ret;
}

template <class T>
oneof_reader<T> oneof(T a1, T a2, T a3, T a4, T a5, T a6, T a7, T a8)
{
    oneof_reader<T> ret;
    ret.add(a1);
    ret.add(a2);
    ret.add(a3);
    ret.add(a4);
    ret.add(a5);
    ret.add(a6);
    ret.add(a7);
    ret.add(a8);
    return ret;
}

template <class T>
oneof_reader<T> oneof(T a1, T a2, T a3, T a4, T a5, T a6, T a7, T a8, T a9)
{
    oneof_reader<T> ret;
    ret.add(a1);
    ret.add(a2);
    ret.add(a3);
    ret.add(a4);
    ret.add(a5);
    ret.add(a6);
    ret.add(a7);
    ret.add(a8);
    ret.add(a9);
    return ret;
}

template <class T>
oneof_reader<T> oneof(T a1, T a2, T a3, T a4, T a5, T a6, T a7, T a8, T a9, T a10)
{
    oneof_reader<T> ret;
    ret.add(a1);
    ret.add(a2);
    ret.add(a3);
    ret.add(a4);
    ret.add(a5);
    ret.add(a6);
    ret.add(a7);
    ret.add(a8);
    ret.add(a9);
    ret.add(a10);
    return ret;
}

#pragma endregion /* oneof_reader */

// ==================================================================
// ==================================================================
// ==================================================================
// ==================================================================

/// @brief 命令行解析器
class parser
{
  public:
    parser() = default;

    ~parser()
    {
        // 析构所有选项
        for (auto &option : options) {
            delete option.second;
        }
    }

    /// @brief 新建无参选项并添加
    /// @param name 选项名
    /// @param short_name 选项名缩写
    /// @param desc 选项描述
    /// @code
    /// ```cpp
    /// parser.add("help", 'h', "print this message");
    /// ```
    /// @endcode
    void add(const std::string &name, char short_name = 0, const std::string &desc = "")
    {
        // 判断选项是否已经存在
        if (options.count(name)) {
            // 名称重复定义
            throw cmdline_error("multiple definition: " + name);
        }
        options[name] = new option_without_value(name, short_name, desc);
        ordered.push_back(options[name]);
    }

    /// @brief 添加选项
    /// @tparam T 选项参数类型
    /// @param name 选项名
    /// @param short_name 选项缩写
    /// @param desc 选项描述
    /// @param need 是否必须
    /// @param def 默认值
    template <class T>
    void add(const std::string &name, char short_name = 0, const std::string &desc = "", bool need = true,
             const T def = T())
    {
        add(name, short_name, desc, need, def, default_reader<T>());
    }

    /// @brief 新建选项并添加
    /// @tparam T 选项参数类型
    /// @tparam F
    /// @param name 选项名
    /// @param short_name 选项缩写
    /// @param desc 选项描述
    /// @param need 是否必须
    /// @param def 默认值
    /// @param reader
    template <class T, class F>
    void add(const std::string &name, char short_name = 0, const std::string &desc = "", bool need = true,
             const T def = T(), F reader = F())
    {
        // 判断选项是否已经存在
        if (options.count(name)) {
            // 名称重复定义
            throw cmdline_error("multiple definition: " + name);
        }
        // 将选项添加到map中
        options[name] = new option_with_value_with_reader<T, F>(name, short_name, need, def, desc, reader);
        ordered.push_back(options[name]);
    }

    /// @brief 在使用提示后面追加
    /// @param[in] f
    void footer(const std::string &f) { ftr = f; }

    /// @brief 设置展示出来的可执行程序的名称
    /// @details 如果不设置则展示完整的程序路径
    /// @param[in] name
    void set_program_name(const std::string &name) { prog_name = name; }

    /// @brief 判断是否存在某个选项
    /// @param[in] name 选项名称
    /// @return true 存在
    /// @return false 不存在
    bool exist(const std::string &name) const
    {
        if (!options.count(name)) {
            throw cmdline_error("there is no flag: --" + name);
        }
        return options.find(name)->second->has_set();
    }

    /// @brief 根据选项名称获取参数
    /// @tparam T
    /// @param[in] name 选项名称
    /// @return const T&
    template <class T>
    const T &get(const std::string &name) const
    {
        if (!options.count(name)) {  // 选项不存在
            throw cmdline_error("there is no flag: --" + name);
        }
        const option_with_value<T> *p = dynamic_cast<const option_with_value<T> *>(options.find(name)->second);
        if (p == NULL) {
            throw cmdline_error("type mismatch flag '" + name + "'");
        }
        return p->get();
    }

    /// @brief
    /// @return const std::vector<std::string>&
    const std::vector<std::string> &rest() const { return others; }

    /// @brief 解析字符串
    /// @param[in] arg
    /// @return true
    /// @return false
    bool parse(const std::string &arg)
    {
        std::vector<std::string> args;

        std::string buf;
        bool in_quote = false;
        for (std::string::size_type i = 0; i < arg.length(); i++) {
            if (arg[i] == '\"') {
                in_quote = !in_quote;
                continue;
            }

            if (arg[i] == ' ' && !in_quote) {
                args.push_back(buf);
                buf = "";
                continue;
            }

            if (arg[i] == '\\') {
                i++;
                if (i >= arg.length()) {
                    errors.emplace_back("unexpected occurrence of '\\' at end of string");
                    return false;
                }
            }

            buf += arg[i];
        }

        if (in_quote) {
            errors.emplace_back("quote is not closed");
            return false;
        }

        if (buf.length() > 0) {
            args.push_back(buf);
        }

        // for (auto &a : args) {
        //     std::cout << "\"" << a << "\"" << std::endl;
        // }

        return parse(args);
    }

    /// @brief 根据参数列表进行解析
    /// @param args 参数列表
    /// @return true 解析正常
    /// @return false 解析失败
    bool parse(const std::vector<std::string> &args)
    {
        int const argc = static_cast<int>(args.size());
        std::vector<const char *> argv(argc);

        for (int i = 0; i < argc; i++) {
            argv[i] = args[i].c_str();
        }

        return parse(argc, argv.data());
    }

    /// @brief 根据命令行输入的内容进行解析
    /// @param argc 参数个数
    /// @param argv 参数内容
    /// @return true 解析正常
    /// @return false 解析失败
    bool parse(int argc, const char *const argv[])
    {
        // 清除错误
        errors.clear();
        others.clear();

        if (argc < 1) {
            errors.emplace_back("argument number must be longer than 0");
            return false;
        }
        if (prog_name.empty()) {
            prog_name = argv[0];
        }

        std::map<char, std::string> lookup;
        for (auto &option : options) {
            // 初始化
            option.second->set(false);

            if (option.first.length() == 0) {
                continue;
            }

            char const initial = option.second->short_name();
            if (initial) {
                if (lookup.count(initial)) {
                    lookup[initial] = "";
                    errors.push_back(std::string("short option '") + initial + "' is ambiguous");
                    return false;
                }
                lookup[initial] = option.first;
            }
        }

        for (int i = 1; i < argc; i++) {
            if (strncmp(argv[i], "--", 2) == 0) {
                const char *p = strchr(argv[i] + 2, '=');
                if (p) {
                    std::string const name(argv[i] + 2, p);
                    std::string const val(p + 1);
                    set_option(name, val);
                } else {
                    std::string const name(argv[i] + 2);
                    if (!options.count(name)) {
                        errors.push_back("undefined option: --" + name);
                        continue;
                    }
                    if (options[name]->has_value()) {
                        if (i + 1 >= argc) {
                            errors.push_back("option needs value: --" + name);
                            continue;
                        }
                        i++;
                        set_option(name, argv[i]);

                    } else {
                        set_option(name);
                    }
                }
            } else if (strncmp(argv[i], "-", 1) == 0) {
                if (!argv[i][1]) {
                    continue;
                }
                char last = argv[i][1];
                for (int j = 2; argv[i][j]; j++) {
                    last = argv[i][j];
                    if (!lookup.count(argv[i][j - 1])) {
                        errors.push_back(std::string("undefined short option: -") + argv[i][j - 1]);
                        continue;
                    }
                    if (lookup[argv[i][j - 1]].empty()) {
                        errors.push_back(std::string("ambiguous short option: -") + argv[i][j - 1]);
                        continue;
                    }
                    set_option(lookup[argv[i][j - 1]]);
                }

                if (!lookup.count(last)) {
                    errors.push_back(std::string("undefined short option: -") + last);
                    continue;
                }
                if (lookup[last].empty()) {
                    errors.push_back(std::string("ambiguous short option: -") + last);
                    continue;
                }

                if (i + 1 < argc && options[lookup[last]]->has_value()) {
                    set_option(lookup[last], argv[i + 1]);
                    i++;
                } else {
                    set_option(lookup[last]);
                }
            } else {
                others.emplace_back(argv[i]);
            }
        }

        for (auto &option : options) {
            if (!option.second->valid()) {
                errors.push_back("need option: --" + std::string(option.first));
            }
        }

        return errors.empty();
    }

    /// @brief 检查解析器设置是否正确
    /// @param arg
    void parse_check(const std::string &arg)
    {
        if (!options.count("help")) {
            add("help", 'h', "print this message");
        }
        check(0, parse(arg));
    }

    /// @brief 检查解析器设置是否正确
    /// @param args
    void parse_check(const std::vector<std::string> &args)
    {
        if (!options.count("help")) {
            add("help", 'h', "print this message");
        }
        check((int)args.size(), parse(args));
    }

    /// @brief 检查解析器设置是否正确
    /// @param argc 参数个数
    /// @param argv
    void parse_check(int argc, char *argv[])
    {
        if (!options.count("help")) {  // 如果不存在help选项自己创建一个
            add("help", 'h', "print this message");
        }
        check(argc, parse(argc, argv));
    }

    /// @brief 错误信息
    /// @return std::string
    std::string error() const { return !errors.empty() ? errors[0] : ""; }

    /// @brief
    /// @return std::string
    std::string error_full() const
    {
        std::ostringstream oss;
        for (const auto &error : errors) {
            oss << error << std::endl;
        }
        return oss.str();
    }

    /// @brief 使用帮助
    /// @return std::string
    std::string usage() const
    {
        std::ostringstream oss;
        oss << "usage: " << prog_name << " ";
        for (auto *i : ordered) {
            if (i->must()) {
                oss << i->short_description() << " ";
            }
        }

        oss << "[options] ... " << ftr << std::endl;
        oss << "options:" << std::endl;

        size_t max_width = 0;
        for (auto *i : ordered) {
            max_width = std::max(max_width, i->name().length());
        }
        for (auto *i : ordered) {
            if (i->short_name()) {
                oss << "  -" << i->short_name() << ", ";
            } else {
                oss << "      ";
            }

            oss << "--" << i->name();
            for (size_t j = i->name().length(); j < max_width + 4; j++) {
                oss << ' ';
            }
            oss << i->description() << std::endl;
        }
        return oss.str();
    }

  private:
    /// @brief 检查
    /// @param argc
    /// @param ok
    void check(int argc, bool ok) const
    {
        if ((argc == 1 && !ok) || exist("help")) {
            std::cout << usage();
            exit(0);
        }

        if (!ok) {
            std::cerr << error() << std::endl;
            std::cout << usage();
            exit(1);
        }
    }

    /// @brief 设置选项标记
    /// @param name
    void set_option(const std::string &name)
    {
        if (!options.count(name)) {
            errors.push_back("undefined option: --" + name);
            return;
        }
        if (!options[name]->set(true)) {
            errors.push_back("option needs value: --" + name);
            return;
        }
    }

    /// @brief 设置选项内容
    /// @param name
    /// @param value
    void set_option(const std::string &name, const std::string &value)
    {
        if (!options.count(name)) {
            errors.push_back("undefined option: --" + name);
            return;
        }
        if (!options[name]->set(value)) {
            errors.push_back("option value is invalid: --" + name + "=" + value);
            return;
        }
    }

    /// @brief 选项基类
    class option_base
    {
      public:
        virtual ~option_base() = default;

        /// @brief 是否存在参数
        /// @return bool true-存在参数; false-不存在参数
        virtual bool has_value() const = 0;
        /// @brief 设置选项的标记
        /// @param[in] value
        /// @return bool 始终为true
        virtual bool set(bool value)
        {
            _has = value;
            return true;
        }
        /// @brief 设置选项的内容
        /// @param[in] value
        /// @return bool true-参数合法
        virtual bool set(const std::string & /*value*/) { return false; }
        /// @brief 输入参数中是否存在本选项
        /// @return bool
        virtual bool has_set() const { return _has; }
        virtual bool valid() const = 0;
        virtual bool must() const = 0;

        virtual const std::string &name() const = 0;
        virtual char short_name() const = 0;
        virtual const std::string &description() const = 0;
        virtual std::string short_description() const = 0;

      protected:
        bool _has{false};
    };

    /// @brief 无参数选项
    class option_without_value : public option_base
    {
      public:
        /// @brief 无参数选项
        /// @param name 选项名
        /// @param short_name 选项名缩写
        /// @param desc 描述
        option_without_value(std::string name, char short_name, std::string desc)
            : _name(std::move(name)), _short_name(short_name), _desc(std::move(desc))
        {
        }
        ~option_without_value() override = default;

        bool has_value() const override { return false; }

        bool valid() const override { return true; }

        bool must() const override { return false; }

        const std::string &name() const override { return _name; }

        char short_name() const override { return _short_name; }

        const std::string &description() const override { return _desc; }

        std::string short_description() const override { return "--" + _name; }

      private:
        std::string _name{};
        char _short_name{'\0'};
        std::string _desc{};
    };

    /// @brief 有参数选项
    /// @tparam T 参数类型
    template <class T>
    class option_with_value : public option_base
    {
      public:
        /// @brief 有参数选项
        /// @param name 选项名
        /// @param short_name 选项名缩写
        /// @param need 必填项？
        /// @param def 默认值
        /// @param desc 描述
        option_with_value(std::string name, char short_name, bool need, const T &def, const std::string &desc)
            : _name(std::move(name)), _short_name(short_name), _need(need), _def(def), _actual(def)
        {
            this->_desc = full_description(desc);
        }
        ~option_with_value() override = default;

        const T &get() const { return _actual; }

        bool has_value() const override { return true; }

        /// @brief 设置选项的内容
        /// @param value 选项参数内容
        /// @return bool true-参数合法
        bool set(const std::string &value) override
        {
            try {
                _actual = read(value);
                _has = true;
            } catch (const std::exception & /*e*/) {
                return false;
            }
            return true;
        }

        bool valid() const override { return !static_cast<bool>(_need && !_has); }

        bool must() const override { return _need; }

        const std::string &name() const override { return _name; }

        char short_name() const override { return _short_name; }

        const std::string &description() const override { return _desc; }

        std::string short_description() const override { return "--" + _name + "=" + detail::readable_typename<T>(); }

      protected:
        std::string full_description(const std::string &description)
        {
            return description + " (" + detail::readable_typename<T>() +
                   (_need ? "" : " [=" + detail::default_value<T>(_def) + "]") + ")";
        }

        virtual T read(const std::string &s) = 0;

        std::string _name{};
        char _short_name{'\0'};
        bool _need{true};
        std::string _desc{};

        T _def;
        T _actual;
    };

    /// @brief 有参数并且限制范围的选项
    /// @tparam T 参数类型
    /// @tparam F oneof
    template <class T, class F>
    class option_with_value_with_reader : public option_with_value<T>
    {
      public:
        /// @brief 有参数并且限制范围的选项
        /// @param name 选项名
        /// @param short_name 选项名缩写
        /// @param need 必填项？
        /// @param def 默认值
        /// @param desc 描述
        /// @param reader 范围限制
        option_with_value_with_reader(const std::string &name, char short_name, bool need, const T def,
                                      const std::string &desc, F reader)
            : option_with_value<T>(name, short_name, need, def, desc), reader(reader)
        {
        }

      private:
        T read(const std::string &s) { return reader(s); }

        F reader;
    };

    /// @brief 存储所有的选项
    std::map<std::string, option_base *> options{};
    /// @brief 将options中的option_base *排序存储
    std::vector<option_base *> ordered{};
    /// @brief 脚注
    std::string ftr{};

    /// @brief 用于展示的可执行文件名
    std::string prog_name{};
    std::vector<std::string> others{};

    /// @brief 错误信息
    std::vector<std::string> errors{};
};

}  // namespace cmdline
