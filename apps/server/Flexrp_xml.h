#ifndef FLEXRP_XML_H
#define FLEXRP_XML_H

#include <boost/asio.hpp>
#include <boost/filesystem.hpp>
#include <string>
#include <vector>
#include <array>
#include <fstream>
#include <memory>


namespace  FLEXRP
{
    struct Readers{
        std::string name;
    };

    struct Writers{
        std::string name;
    };

    struct Recon_modules_Properties{
        std::string name;
        std::string value;
    };

    struct Recon_modules{
        std::string name;
        std::vector<Recon_modules_Properties> properties;
    };


    struct Flexrp_configuration{
        std::vector<Readers> readers;
        std::vector<Writers> writers;
        std::vector<Recon_modules> recon_modules;
    };


    class Session
        : public std::enable_shared_from_this<Session>
    {
    public:
        using TcpSocket = boost::asio::ip::tcp::socket;

        explicit Session(TcpSocket t_socket);

        void start()
        {
            doRead();
        }

    private:
        void doRead();
        void processRead(size_t t_bytesTransferred);
        void createFile();
        void readData(std::istream &stream);
        void doReadFileContent(size_t t_bytesTransferred);
        void handleError(std::string const& t_functionName, boost::system::error_code const& t_ec);


        TcpSocket m_socket;
        enum { MaxLength = 40960 };
        std::array<char, MaxLength> m_buf;
        boost::asio::streambuf m_requestBuf_;
        std::ofstream m_outputFile;
        size_t m_fileSize;
        std::string m_fileName;
    };


    class Server
    {
    public:
        using TcpSocket = boost::asio::ip::tcp::socket;
        using TcpAcceptor = boost::asio::ip::tcp::acceptor;
        using IoService = boost::asio::io_service;

        Server(IoService& t_ioService, short t_port, std::string const& t_workDirectory);

    private:
        void doAccept();
        void createWorkDirectory();

        TcpSocket m_socket;
        TcpAcceptor m_acceptor;

        std::string m_workDirectory;
    };

    static std::string config_filename{};
    void deserialize(Flexrp_configuration &fcg);
    void run_processes(const Flexrp_configuration &fcg);
}

#endif // FLEXRP_XML_H
