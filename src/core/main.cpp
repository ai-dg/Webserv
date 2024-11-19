#include <vector>
#include <string>
#include <csignal>
#include <stdexcept>
#include "../headers/Server.hpp"
#include "../headers/signals.hpp"
#include "../headers/Conf.hpp"
#include "../headers/SessionManager.hpp"
#include "../headers/Sockets.hpp"
#include "../headers/cleanup.hpp"
#include "../headers/RarManager.hpp"
#include "../headers/SignalHandler.hpp"

int main(int ac, char **av)
{
    
    setupSignalHandler();
    try
    {
        std::string path;
        std::vector<int> fd_sockets;
        std::vector<Conf *> Configs;
        std::vector<Server *> Servers;


        /**
         * Conditions du path, si NULL, path par defaut
         */
        if (ac >= 2)
            path.assign(av[1]);
        else 
            path = "config/server.conf";



        /**
         * Extraire les informations dans le path
         */
        get_all_server_conf(path, Configs);  

          

        SessionManager sessionManager;

        if (start_all_servers(fd_sockets, Servers, Configs) == 1)
            return 1;
            
        /**
         * @brief Gestion du trafic de requetes et reponses (fd du client et du serveur)
         */
        request_and_response_fd_manager(fd_sockets, Servers, sessionManager);

        for (size_t i = 0; i < fd_sockets.size() ; ++i)
        {
            close(fd_sockets[i]);
        }
        Log::cleanup();
        clearMemory(Configs);
        clearArray(Servers);
        // clearArray(fd_sockets);
    }
    catch (SignalException const& e)
    {
        std::cerr << "SignalException caught in main: " << e.what() << std::endl;
        Log::cleanup();
        return 1;
    }
    catch (std::exception const& e)
    {
        std::cerr << "General exception caught in main: " << e.what() << std::endl;
        Log::cleanup();
        return 1;
    }
    catch (...)
    {
        std::cerr << "Unknown exception caught in main!" << std::endl;
        Log::cleanup();
        return 1;
    }
    /**
     * @brief Gestion du trafic de requetes et reponses (fd du client et du serveur)
     */
    
    // request_and_response_fd_manager(fd_sockets, Servers, sessionManager);
    // std::cerr << std::endl << "Clear memory..."<<std::endl ;
    
    // for (size_t i = 0; i < fd_sockets.size() ; ++i)
    // {
    //     close(fd_sockets[i]);
    // }
    return (0);
}
