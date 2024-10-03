namespace http
{
    class TcpServer
    {
    public:
        TcpServer();
        ~TcpServer();
        void StartListen();

    private:
        int StartServer();
        void CloseServer();
        int socketReturnValue;
    };
}
