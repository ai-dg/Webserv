#include <vector>
#include <string>
#include <csignal>
#include "../headers/Server.hpp"
#include "../headers/Conf.hpp"
#include "../headers/SessionManager.hpp"
#include "../headers/Sockets.hpp"
#include "../headers/RarManager.hpp"

volatile sig_atomic_t signalReceived = 0;


void signalHandler(int signal)
{
    if (signal == SIGINT)
    {
        signalReceived = 1; 
    }
}


int main(int ac, char **av)
{

    signal(SIGPIPE, SIG_IGN);
    signal(SIGINT, signalHandler);
    try
    {

        std::string path;
        std::vector<int> fd_sockets;
        std::vector<Conf> Configs;
        
        std::vector<Server> Servers;

        /**
         * Conditions du path, si NULL, path par defaut
         */
        if (ac >= 2)
            path.assign(av[1]);
        else 
            path = "config/server.conf";

        
        try
        {        
            /**
             * Extraire les informations dans le path
             */
            get_all_server_conf(path, Configs);  
        }
        catch (std::exception& e)
        {
            std::cerr << "Error: " << e.what() << std::endl;
            return 1;
        }

        SessionManager sessionManager;

        if (start_all_servers(fd_sockets, Servers, Configs) == 1)
            return 1;
            
        while(true)
        {
            if (signalReceived)
            {
                std::cout << "Signal received, shutting down..." << std::endl;
                break;
            }
            /**
             * @brief Gestion du trafic de requetes et reponses (fd du client et du serveur)
             */
            request_and_response_fd_manager(fd_sockets, Servers, sessionManager);
            
        }
        for (size_t i = 0; i < fd_sockets.size() ; ++i)
        {
            close(fd_sockets[i]);
        }
    }
    catch (std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return (0);
}
