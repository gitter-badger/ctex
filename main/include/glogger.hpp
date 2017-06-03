/**
 * @file glogger.hpp
 * @brief Single Header Logger
 * @copyright Shoshin Ilya (Galarius) no warranty implied; use at your own risk
 * @note requirements: -std=c++11
 *
 * Usage:
 * Add `#define __glogger_implementation__` before you include
 * this file in ONE C or C++ file to create the implementation.
 */

#ifndef __glogger_hpp__
#define __glogger_hpp__

#include <iostream>
#include <fstream>
#include <sstream>
#include <mutex>
#include <chrono>
#include <ctime>

/**
 * @class GLogger
 * @brief Singleton class for logging
 *
 * Usage example:
 * @code{.cpp}
 *     #include <...>
 *     #include <...>
 *     #include "..."
 *     #define __glogger_implementation__    // define this only once
 *     #include "glogger.hpp"
 *
 *     void main()
 *     {
 *         // GLogger setup
 *         GLogger::instance().set_output_mode(GLogger::Output::Both); // output to console and file
 *         GLogger::instance().set_min_level(GLogger::Output::Console, GLogger::Level::Info);  // min console level
 *         GLogger::instance().set_min_level(GLogger::Output::File, GLogger::Level::Trace);    // min file level
 *         GLogger::instance().set_max_log_file_size(2000000);         // max log file size in bytes, 2Mb
 *         GLogger::instance().set_log_filename("glogger.log");        // log file name
 *         //...
 *         try
 *         {
 *           //...
 *         }
 *         catch(std::runtime_error& e)
 *         {
 *           GLogger::instance().logError("operation failed. reason:", e.what());
 *         }
 *     }
 *  @endcode
 */
class GLogger
{
    GLogger();
    GLogger(const GLogger&) = delete;
    GLogger& operator=(const GLogger&) = delete;
public:
    /**
     * @brief Get single instance of GLogger
     */
    static GLogger& instance();
    ~GLogger() = default;
public:
    /**
     * @brief Output logger mode
     */
    enum Output : unsigned int
    {
        ///@{
        Off = 0,    ///< disabled
        Console,    ///< console only
        File,       ///< log file only
        Both        ///< console and log file
        ///@}
    };
    /**
     * @brief Logger level
     */
    enum Level : unsigned int
    {
        ///@{
        Trace = 0,  ///< verbose output (the lowest level)
        Debug,      ///< for debugging
        Info,       ///< for progress notifications
        Warn,       ///< warnings
        Error,      ///< errors
        None        ///< invariant to level (e.g. greeting message)
        ///@}
    };
    
    /**
     * @brief Set output logger mode
     * @param[in] mode logger mode
     * @see Output
     */
    void set_output_mode(Output mode);
    /**
     * @brief Set min logger level for specified target to filter messages
     * @param[in] target file, console or both
     * @param[in] level min level to print
     * @see Output
     * @see Level
     */
    void set_min_level(Output target, Level level);
    /**
     * @brief Set log filename to begin file logging
     * @param[in] filename log file name
     * @note If file size exceeds max_log_file_size_,
     * new log file will be created
     */
    void set_log_filename(const std::string& filename);
    /**
     * @brief Set max log file size
     * @param[in] size max log file size in bytes
     *
     * If file size exceeds max_log_file_size_,
     * new log file will be created.
     */
    void set_max_log_file_size(size_t size);
    /**
     * @brief Skip empty messages
     * param[in] flag skip or allow
     */
    void set_skip_empty_messages(bool flag);
    /**
     * @brief Trim leading and trailing spaces in messages
     * param[in] flag enable/disable
     */
    void set_trim_messages(bool flag);
    /**
     * @brief Set symbol to separate level from message, e.g. `[Level]:Message`
     * param[in] separator separation symbol
     */
    void set_separator(const std::string& separator);
    /**
     * @brief Begin recording log messages
     * @note Recording uses the same min log level as console
     * @see end_record
     */
    void start_record();
    /**
     * @brief Stop recording log messages
     * @return string with recorded log messages
     * @note must be called after start_record
     * @see start_record
     */
    std::string end_record();
public:
    /**
     * @brief Logging without level
     * @param[in] args arguments for logging
     * @see Level
     */
    template<typename ...Args>
    void log(Args&&... args)
    {
        inner_log(None, std::forward<Args>(args)...);
    }
    /**
     * @brief Logging with trace level
     * @param[in] args arguments for logging
     * @see Level
     */
    template<typename ...Args>
    void logTrace(Args&&... args)
    {
        inner_log(Trace, std::forward<Args>(args)...);
    }
    /**
     * @brief Logging with debug level
     * @param[in] args arguments for logging
     * @see Level
     */
    template<typename ...Args>
    void logDebug(Args&&... args)
    {
        inner_log(Debug, std::forward<Args>(args)...);
    }
    /**
     * @brief Logging with info level
     * @param[in] args arguments for logging
     * @see Level
     */
    template<typename ...Args>
    void logInfo(Args&&... args)
    {
        inner_log(Info, std::forward<Args>(args)...);
    }
    /**
     * @brief Logging with warn level
     * @param[in] args arguments for logging
     * @see Level
     */
    template<typename ...Args>
    void logWarn(Args&&... args)
    {
        inner_log(Warn, std::forward<Args>(args)...);
    }
    /**
     * @brief Logging with error level
     * @param[in] args arguments for logging
     * @see Level
     */
    template<typename ...Args>
    void logError(Args&&... args)
    {
        inner_log(Error, std::forward<Args>(args)...);
    }
private:
    /**
     * @brief unpacks the last argument of variadic
     * template arguments sequence
     * @tparam T argument type
     */
    template<typename T>
    void unpack(T t)
    {
        buffer_unpack_ << t;
    }
    /**
     * @brief unpacks variadic template arguments sequence
     * @tparam T first argument type
     * @tparam Args other arguments types
     */
    template<typename T, typename ...Args>
    void unpack(T t, Args&&... args)
    {
        buffer_unpack_ << t;
        unpack(std::forward<Args>(args)...);
    }
    /**
     * @brief Write constructed message to file or stdout/stderr
     * @param[in] level log level
     * @param[in] message message to write
     */
    void write_log_message(Level level, std::string& message);
    /**
     * @brief Inner logging method to perform console/file output
     * @param[in] args arguments for logging
     * @see Level
     */
    template<typename ...Args>
    void inner_log(Level level, Args&&... args)
    {
        write_lock_.lock();
        unpack(std::forward<Args>(args)...); // unpack arguments and build message
        std::string message = buffer_unpack_.str();
        message.shrink_to_fit();
        buffer_unpack_.str(std::string());	 // clear message buffer
        write_log_message(level, message);
        write_lock_.unlock();
    }
private:
    /**
     * @brief Get string name from Level
     * @param[in] lvl from Level enumeration
     * @return lvl string name
     * @see Level
     */
    static std::string level_name(Level lvl);
    /**
     * @brief Copy file from one location to another
     * @param[in] from source
     * @param[in] to destination
     */
    static void copy_file(const std::string &from, const std::string &to);
    /**
     * @brief Get file size
     * @brief filename file name
     * @return file size in bytes
     */
    static std::ifstream::pos_type file_size(const std::string& filename);
    /**
     * @brief Checks whether file exists
     * @param[in] filename file name
     */
    static bool exists(const std::string& filename);
    /**
     * @brief Current time as string
     */
    static std::string current_time();
    /**
     * @brief Remove leading and trailing spaces
     * @brief[in,out] s string to trim
     */
    static void trim(std::string& s);
    /**
     * @brief Get path basename
     * @param[in] pathname path
     */
    static std::string basename(const std::string& pathname);
private:
    // Options
    Level min_level_console_;			///< @brief console min log level
    Level min_level_file_;				///< @brief file min log level
    Output output_;						///< @brief output mode
    bool skip_empty_msgs_;              ///< @brief skip empty messages toggle
    bool trim_messages_;                ///< @brief remove whitespaces
    std::string log_file_;				///< @brief log file name
    size_t max_log_file_size_;          ///< @brief max log file size
    std::string separator_;             ///< @brief symbol to separate level from message, e.g. `:` or `>`
    // Helpers
    std::stringstream buffer_unpack_;	///< @brief temp buffer for variadic template arguments unpacking
    std::stringstream buffer_record_;	///< @brief temp buffer for recording log messages into a string
    bool record_enabled_;				///< @brief recording toggle
    std::ofstream fout_;				///< @brief log file output stream
    std::mutex write_lock_;             ///< @brief mutex for logging
};

#endif  // __glogger_hpp__

#ifdef __glogger_implementation__

#include <algorithm>	// std::find_if
#include <functional>	// std::ptr_fun<int, int>
#include <cctype>		// std::isspace

GLogger& GLogger::instance()
{
    /* In C++11 thread-safety initialization and
     destruction is enforced in the standard */
    static GLogger logger;
    return logger;
}

GLogger::GLogger() :
min_level_console_(Level::Info)
, min_level_file_(Level::Trace)
, output_(Output::Both)
, skip_empty_msgs_(false)
, trim_messages_(false)
, max_log_file_size_(2000000) // 2 Mb
, record_enabled_(false)
, separator_(": ")
{ }

void GLogger::write_log_message(Level level, std::string& message)
{
    if(trim_messages_)
        trim(message);
    
    if (output_ != Off && !(skip_empty_msgs_ && message.empty()) )
    {
        std::string snow = current_time();
        // write to log file
        if ((output_ == File || output_ == Both) && level >= min_level_file_)
        {
            if (fout_.good())
            {
                fout_ << level_name(level) << separator_ << "[" << snow << "]" << separator_ << message << std::endl;
            }
        }
        // write to console
        if ((output_ == Console || output_ == Both) && level >= min_level_console_)
        {
            if (level == Level::Error)
            {
                std::cerr << level_name(level) << separator_ << message << std::endl;
            }
            else
            {
                std::cout << level_name(level) << separator_ << message << std::endl;
            }
        }
    }
    // recording
    if (record_enabled_ && level >= min_level_console_)
        buffer_record_ << message << std::endl;
}

//-----------------------------------------------------------------------------------------
// mutators
//-----------------------------------------------------------------------------------------

void GLogger::set_output_mode(Output mode)
{
    output_ = mode;
}

void GLogger::set_min_level(Output target, Level level)
{
    switch (target)
    {
        case Console:
            min_level_console_ = level;
            break;
        case File:
            min_level_file_ = level;
            break;
        case Both:
            min_level_console_ = level;
            min_level_file_ = level;
            break;
        default:
            std::cerr << "[GLogger Error]: invalid target" << std::endl;
            break;
    }
}

void GLogger::set_log_filename(const std::string& filename)
{
    static std::locale sysLoc("");	// get system locale
    log_file_ = filename;
    
    if (!log_file_.empty())
    {
        std::string snow = current_time();
        if (output_ == File || output_ == Both)
        {
            if (exists(log_file_) && file_size(log_file_) > max_log_file_size_)
            {
                std::string bname = basename(log_file_);
                copy_file(log_file_, snow + "_" + bname);
                fout_.open(log_file_, std::ios::out);
            }
            else
            {
                fout_.open(log_file_, std::ios::out | std::ios::app);
            }
            if (fout_.good())
            {
                fout_.imbue(sysLoc);
                fout_ << std::endl
                << "----------------------------------------------------------------" << std::endl
                << "--------------------" << snow << "--------------------" << std::endl
                << "----------------------------------------------------------------"
                << std::endl << std::endl;
            }
            else
            {
                GLogger::instance().logError("failed to open log file");
            }
        }
    }
}

void GLogger::set_max_log_file_size(size_t size)
{
    max_log_file_size_ = size;
}

void GLogger::set_skip_empty_messages(bool flag)
{
    skip_empty_msgs_ = flag;
}

void GLogger::set_trim_messages(bool flag)
{
    trim_messages_ = flag;
}

void GLogger::set_separator(const std::string& separator)
{
    separator_ = separator;
}

//-----------------------------------------------------------------------------------------
// recording
//-----------------------------------------------------------------------------------------

void GLogger::start_record()
{
    if (!record_enabled_)
    {
        record_enabled_ = true;
    }
    else
    {
        GLogger::instance().logError("already recording!");
    }
}

std::string GLogger::end_record()
{
    std::string text;
    if (record_enabled_)
    {
        record_enabled_ = false;
        text = buffer_record_.str();
        buffer_record_.str(std::string());
    }
    else
    {
        GLogger::instance().logError("recording is not started!");
    }
    return text;
}

//-----------------------------------------------------------------------------------------
// static helpers
//-----------------------------------------------------------------------------------------

std::string GLogger::level_name(Level lvl)
{
    static std::string levels[] {"Trace", "Debug", "Info", "Warn", "Error", ""};
    return levels[lvl];
}

void GLogger::copy_file(const std::string &from, const std::string &to)
{
    std::ifstream is(from, std::ios::in | std::ios::binary);
    std::ofstream os(to, std::ios::out | std::ios::binary);
    os << is.rdbuf();
}

std::ifstream::pos_type GLogger::file_size(const std::string& filename)
{
    std::ifstream in(filename, std::ifstream::binary | std::ios::ate);
    return in.tellg();
}

bool GLogger::exists(const std::string& name)
{
    std::ifstream f(name);
    return f.good();
}

std::string GLogger::current_time()
{
    auto now = std::chrono::system_clock::now();
    auto ttnow = std::chrono::system_clock::to_time_t(now);
#ifdef _WIN32
    char cnow[50];
    ctime_s(cnow, 50, &ttnow);
    std::string snow(cnow);
#else
    std::string snow(ctime(&ttnow));
#endif
    snow.shrink_to_fit();
    snow.pop_back();
    return snow;
}

void GLogger::trim(std::string& s) {
#ifdef _WIN32
    bool if_ascii = std::all_of(s.begin(), s.end(),
                                [](char c) { return c >= -1 && c < 255; });
#else
    bool if_ascii = true;
#endif
    if(if_ascii)
    {
        // ltrim
        s.erase(s.begin(), find_if(s.begin(), s.end(),
                                   std::not1(std::ptr_fun<int, int>(std::isspace))));
        // rtrim
        s.erase(find_if(s.rbegin(), s.rend(),
                        std::not1(std::ptr_fun<int, int>(std::isspace))).base(),
                s.end());
    }
}

std::string GLogger::basename(const std::string& pathname) {
    return std::string(std::find_if(pathname.rbegin(), pathname.rend(),
                                    [](char ch) {
#ifdef _WIN32
                                        return ch == '\\';
#else
                                        return ch == '/';
#endif
                                    }).base(),
                       pathname.end());
}
#endif  // __glogger_implementation__