#include "Server.hpp"
#include "Utils.hpp"

// int sig = 0;

// void handlectrl_c(int signum)
// {
//     if (signum == SIGINT)
//     {
//         std::cout << "EXIT!\n";
//         sig = signum;
//     }
// }

int main()
{
    // signal(SIGINT, handlectrl_c);
    
    std::vector<Server> _server;
    Server test;
    // Server test1;
    if (!test.creat_socket())
        return (0);
    // if (!test1.creat_socket())
    //     return (0);
    test.configure_sockaddrin(2003);
    // test1.configure_sockaddrin(2025);
    if (!test.link_socket_adress())
        return (0);
    // if (!test1.link_socket_adress())
    //     return (0);
    if (!test.wait_connexion_from_client())
        return (0);
    // if (!test1.wait_connexion_from_client())
    //     return (0);
    while (1)
    {
        if (test.accept_client_connexion())
        {
            std::cout << "Connected!" << std::endl;
            std::string response = test.get_client_request();
            if (response.length())
            {
                int fd = test.get_fdClient();
                get_request(response, fd, "agency/");
            }
            close(test.get_fdClient());
        }
        // if (test1.accept_client_connexion())
        // {
        //     std::cout << "Connected!" << std::endl;
        //     std::string response = test1.get_client_request();
        //     if (response.length())
        //     {
        //         int fd = test.get_fdClient();
        //         get_request(response, fd, "");
        //     }
        //     close(test1.get_fdClient());
        // }
    }
    return 0;
}
