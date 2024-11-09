#include <vector>
#include <string>
#include <csignal>
#include "../headers/Server.hpp"
#include "../headers/signals.hpp"
#include "../headers/Conf.hpp"
#include "../headers/SessionManager.hpp"
#include "../headers/Sockets.hpp"
#include "../headers/RarManager.hpp"


int main(int ac, char **av)
{
    std::string path;
    std::vector<int> fd_sockets;
    SessionManager sessionManager;
    std::vector<Conf *> Configs;
    std::vector<Server> Servers;

    signal(SIGPIPE, SIG_IGN);
    signal(SIGINT, handle_sig);

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
   
    if (start_all_servers(fd_sockets, Servers, Configs) == 1)
        return 1;
    /**
     * @brief Gestion du trafic de requetes et reponses (fd du client et du serveur)
     */
    
    request_and_response_fd_manager(fd_sockets, Servers, sessionManager);
    std::cerr << "Clean memory..."<<std::endl ;
    for (size_t i = 0; i < fd_sockets.size() ; ++i)
    {
        close(fd_sockets[i]);
    }
    return (0);
}
